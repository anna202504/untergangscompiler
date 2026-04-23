%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "klammer.h"
#include "symbol_table.h"
#include "tree.h"
#include "optimierung/optimierung.h"

struct tableEntry *symbolTable = NULL;
struct treeNode *ast = NULL;

int yylex(void);
void yyerror(const char *s);
extern FILE *yyin;
%}

%union {
    char* str;
    int val;
    struct treeNode *p;
    char id[101];
}

%token DECLARE PREDICATE FUNCTION VARIABLE
%token EXIST ALL
%token TRUE FALSE
%token AND OR NOT IMPLIES EQUIV
%token BRACKET_OPEN BRACKET_CLOSE
%token SQUARE_BRACKET_OPEN SQUARE_BRACKET_CLOSE
%token COMMA COLON SEMICOLON

%token <val> INT 
%token <str> STRING

%left OR
%left AND
%left IMPLIES
%left EQUIV
%right NOT

%start input

%type <p> formula
%type <p> equiv_formula implies_formula or_formula and_formula not_formula quant_or_atom atom
%type <p> term_list_opt term_list term
%%

input:
     /* leer */
    | input block
    ;

block:
    declarations formula SEMICOLON { 
        fprintf(stderr, "PAR: Formula completed with Semicolon.\n");
        
        fprintf(stderr, "\n----- New Block Parsed -----\n"); 

        fprintf(stderr, "\n----- Start Syntax Tree Printout. -----\n");
        printTree($2, 0);
        fprintf(stderr, "----- End of Syntax Tree Printout. -----\n");

        struct treeNode *opt = eliminate_double_negation($2);
        opt = eliminate_implications_and_equivalences(opt);
        opt = push_negations_to_predicates(opt);

        fprintf(stderr, "\n----- Optimized Syntax Tree -----\n");
        printTree(opt, 0);
        fprintf(stderr, "----- End of Optimized Syntax Tree -----\n");

        printSymbolTable(symbolTable);
        printDeclarationsFromSymbolTable(symbolTable);
        fprintf(stdout, "\n");
        printFormulaFromSyntaxTree(opt);
        fprintf(stdout, ";\n");

        deleteTree(opt);
        }
    ;

declarations:
    declarations declaration 
    | declaration
    ;

declaration:
    DECLARE PREDICATE STRING COLON INT {
        fprintf(stderr, "PAR: Declaration: Predicate -%s- Arity: %d\n", $3, $5);

        struct tableEntry *entry = getSymbolEntry(symbolTable, $3);
        if (entry != NULL) {
            if (strcmp(entry->type, "predicate") != 0 || entry->arity != $5) {
                fprintf(stderr, "ERROR: Identifier %s already declared with different type\n", $3);
            exit(1);
            }
        } else {
            addSymbolEntry(&symbolTable, $3, "predicate", $5);  
        }
    }
    | DECLARE FUNCTION STRING COLON INT {
        fprintf(stderr, "PAR: Declaration: Function -%s- Arity: %d\n", $3, $5);

        struct tableEntry *entry = getSymbolEntry(symbolTable, $3);
        if (entry != NULL) {
            if (strcmp(entry->type, "function") != 0 || entry->arity != $5) {
                fprintf(stderr, "ERROR: Identifier %s already declared with different type\n", $3);
            exit(1);
            }
        } else {
            addSymbolEntry(&symbolTable, $3, "function", $5);
        }
    }
    | DECLARE VARIABLE STRING COLON STRING {
        fprintf(stderr, "PAR: Declaration: Variable -%s- Type: %s\n", $3, $5);

        if(strcmp($5, "int") != 0) {
            fprintf(stderr, "ERROR: Variable %s must have type int\n", $3);
            exit(1);
        }
        struct tableEntry *entry = getSymbolEntry(symbolTable, $3);
        if (entry != NULL) {
            if (strcmp(entry->type, "variable") != 0) {
                fprintf(stderr, "ERROR: Identifier %s already declared with different type\n", $3);
            exit(1);
            }
        } else {
            addSymbolEntry(&symbolTable, $3, "variable", 0);
        }
    }
;

formula:
    equiv_formula
;

equiv_formula:
      implies_formula { $$ = $1; }
    | equiv_formula EQUIV implies_formula { 
        fprintf(stderr,"PAR: Formula reduced - JUNCTOR: EQUIVALENT\n"); 
        $$ = makeNode(NODE_BINARY_OPERATOR);
        $$->treeTypes.binaryType.left = $1;
        $$->treeTypes.binaryType.right = $3;
        $$->treeTypes.binaryType.operatorType = BINOP_IFF;
        fprintf(stderr, "SYT: Binary Node created - Type <->\n");
    }
;

implies_formula:
      or_formula { $$ = $1; }
    | implies_formula IMPLIES or_formula { 
        fprintf(stderr,"PAR: Formula reduced - JUNCTOR: IMPLICATION\n"); 
        $$ = makeNode(NODE_BINARY_OPERATOR);
        $$->treeTypes.binaryType.left = $1;
        $$->treeTypes.binaryType.right = $3;
        $$->treeTypes.binaryType.operatorType = BINOP_IMPLIES;
        fprintf(stderr, "SYT: Binary Node created - Type ->\n");
    }
;

or_formula:
      and_formula { $$ = $1; }
    | or_formula OR and_formula { 
        fprintf(stderr,"PAR: Formula reduced - JUNCTOR: OR\n"); 
        $$ = makeNode(NODE_BINARY_OPERATOR);
        $$->treeTypes.binaryType.left = $1;
        $$->treeTypes.binaryType.right = $3;
        $$->treeTypes.binaryType.operatorType = BINOP_OR;
        fprintf(stderr, "SYT: Binary Node created - Type |\n");
    }
;

and_formula:
      not_formula { $$ = $1; }
    | and_formula AND not_formula { 
        fprintf(stderr,"PAR: Formula reduced - JUNCTOR: AND\n"); 
        $$ = makeNode(NODE_BINARY_OPERATOR);
        $$->treeTypes.binaryType.left = $1;
        $$->treeTypes.binaryType.right = $3;
        $$->treeTypes.binaryType.operatorType = BINOP_AND;
        fprintf(stderr, "SYT: Binary Node created - Type &\n");
    }
;

not_formula:
      quant_or_atom { $$ = $1; }
    | NOT not_formula { 
        fprintf(stderr,"PAR: Formula reduced - JUNCTOR: NEGATION\n"); 
        $$ = makeNode(NODE_UNARY_OPERATOR);
        $$->treeTypes.unaryType.child = $2;
        $$->treeTypes.unaryType.operatorType = UOP_NOT;
        fprintf(stderr, "SYT: Unary Node created - Type ~\n");
    }
;

quant_or_atom:
      TRUE { 
        $$ = makeNode(NODE_BOOL);
        $$->treeTypes.boolType.value = 1;
        fprintf(stderr, "PAR: Formula reduced - ATOM: TRUE\n");
        fprintf(stderr,"SYT: TRUE Node created\n"); 
      }
    | FALSE { 
        $$ = makeNode(NODE_BOOL);
        $$->treeTypes.boolType.value = 0;
        fprintf(stderr, "PAR: Formula reduced - ATOM: FALSE\n");
        fprintf(stderr,"SYT: FALSE Node created\n"); 
      }
    | atom { $$ = $1; }
    | ALL SQUARE_BRACKET_OPEN STRING SQUARE_BRACKET_CLOSE not_formula {
            struct tableEntry *entry = getSymbolEntry(symbolTable, $3);
            if(entry == NULL) {
                fprintf(stderr, "ERROR: Variable %s not declared\n", $3);
                exit(1);
            }
            if(strcmp(entry->type, "variable") != 0) {
                fprintf(stderr, "ERROR: %s is not a variable\n", $3);
                exit(1);
            }

            $$ = makeNode(NODE_QUANTOR);
            $$->treeTypes.quantorType.quantorType = FORALL;
            $$->treeTypes.quantorType.var = entry;
            $$->treeTypes.quantorType.formula = $5;

            fprintf(stderr, "PAR: Formula reduced - QUANTOR: ALL %s\n", $3);
            fprintf(stderr, "SYT: Variable Node created\n");
            fprintf(stderr, "SYT: Quantor Node created - Type ALL\n");
        }
    | EXIST SQUARE_BRACKET_OPEN STRING SQUARE_BRACKET_CLOSE not_formula {
        struct tableEntry *entry = getSymbolEntry(symbolTable, $3);
        if(entry == NULL) {
            fprintf(stderr, "ERROR: Variable %s not declared\n", $3);
            exit(1);
        }
        
        if(strcmp(entry->type, "variable") != 0) {
            fprintf(stderr, "ERROR: %s is not a variable\n", $3);
            exit(1);
        }
        $$ = makeNode(NODE_QUANTOR);
        $$->treeTypes.quantorType.quantorType = EXISTS;
        $$->treeTypes.quantorType.var = entry;
        $$->treeTypes.quantorType.formula = $5;

        fprintf(stderr,"PAR: Formula reduced - QUANTOR: EXIST %s\n",$3); 
        fprintf(stderr, "SYT: Variable Node created\n");
        fprintf(stderr,"SYT: Quantor Node created - Type EXIST\n"); 
    }
    | BRACKET_OPEN formula BRACKET_CLOSE { $$ = $2; }
;

atom:
    STRING BRACKET_OPEN term_list_opt BRACKET_CLOSE {
        struct tableEntry *entry = getSymbolEntry(symbolTable, $1);
        if(entry == NULL) { 
            fprintf(stderr, "ERROR: %s not declared\n", $1); 
            exit(1); 
            }
        if(strcmp(entry->type, "predicate") != 0) {
            fprintf(stderr, "ERROR: %s is not declared as predicate\n", $1);
            exit(1);
        }

        int argCount = countArguments($3);
        if(argCount != entry->arity) {
            fprintf(stderr, "ERROR: Predicate %s expects %d argument(s), but got %d\n", $1, entry->arity, argCount);
            exit(1);
        }

        $$ = makeNode(NODE_PREDICATE);
        $$->treeTypes.predicatType.entry = entry;
        $$->treeTypes.predicatType.arguments = $3;
        fprintf(stderr, "PAR: Formula reduced - ATOM: %s() - %d arguments\n", $1, argCount);
        fprintf(stderr, "SYT: Predicate Node created - %s\n", $1);
        
    }
;

term_list_opt:
    term_list { $$ = $1; }
    | { $$ = NULL; }
;

term_list:
    term { $$ = $1; 
    fprintf(stderr, "SYT: Argument Node created\n"); 
    }
    | term_list COMMA term {
        struct treeNode *last = $1;
        while(last->next) last = last->next;
        last->next = $3;
        $$ = $1; 
        fprintf(stderr, "SYT: Argument Node added to list\n");
    }
;

term:
    STRING {
        struct tableEntry *entry = getSymbolEntry(symbolTable, $1);
        if(entry == NULL) {
            fprintf(stderr, "ERROR: %s not declared\n", $1);
            exit(1);
        }

        if(strcmp(entry->type, "variable") == 0) {
            fprintf(stderr, "PAR: TERM: Variable/Constant %s\n", $1);
            $$ = makeNode(NODE_VARIABLE);
            $$->treeTypes.variableType.entry = entry;
            fprintf(stderr, "SYT: Variable Node created - %s\n", $1);
        } else if(strcmp(entry->type, "function") == 0) {
            if(entry->arity != 0) {
                fprintf(stderr, "ERROR: Function %s expects %d argument(s), but got 0\n", $1, entry->arity);
                exit(1);
            }

            fprintf(stderr, "PAR: TERM: Variable/Constant %s\n", $1);
            $$ = makeNode(NODE_FUNCTION);
            $$->treeTypes.functionType.entry = entry;
            $$->treeTypes.functionType.arguments = NULL;
            fprintf(stderr, "SYT: Function Node created - %s\n", $1);
        } else {
            fprintf(stderr, "ERROR: %s cannot be used as term\n", $1);
            exit(1);
        }
    }
  | INT {
        fprintf(stderr, "PAR: TERM: Number: %d\n", $1);
        $$ = makeNode(NODE_NUMBER);
        $$->treeTypes.numberType.value = $1;
        fprintf(stderr, "SYT: Number Node created - %d\n", $1);
    }
  | STRING BRACKET_OPEN term_list_opt BRACKET_CLOSE {
        struct tableEntry *entry = getSymbolEntry(symbolTable, $1);
        if(entry == NULL) { 
            fprintf(stderr, "ERROR: %s not declared\n", $1); 
            exit(1); 
            }
        if(strcmp(entry->type, "function") !=0) {
            fprintf(stderr, "ERROR: %s is not a function\n", $1);
            exit(1);
        }

        int argCount = countArguments($3);
        if(argCount != entry->arity) {
            fprintf(stderr, "ERROR: Function %s expects %d argument(s), but got %d\n", $1, entry->arity, argCount);
            exit(1);
        }

        fprintf(stderr, "PAR: TERM: Function %s() - %d arguments\n", $1, argCount);
        $$ = makeNode(NODE_FUNCTION);
        $$->treeTypes.functionType.entry = entry;
        $$->treeTypes.functionType.arguments = $3;
        fprintf(stderr, "SYT: Function Node created - %s(...)\n", $1);
    }
;

%%

void yyerror(const char *s) {
    fprintf(stderr,"Parser error: %s\n", s);
}

int main(int argc, char *argv[]){
    if(argc < 2) {
        fprintf(stderr,"Usage: %s <inputfile>\n", argv[0]);
        return 1;
    }
    FILE *fp = fopen(argv[1], "r");
    if(!fp) {
        perror("fopen");
        return 1;
    }
    yyin = fp;

    int result = yyparse();
    fclose(fp);

    clearSymbolTable(&symbolTable);

    return result;
}