%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "tree.h"

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
%token BIN_AND BIN_OR UNARY_NOT BIN_IMPLIES BIN_EQUIV
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
        fprintf(stderr, "\n----- New Block Parsed -----\n"); 

        fprintf(stderr, "\n----- Start Syntax Tree Printout. -----\n");
        printTree($2, 0);
        fprintf(stderr, "----- End of Syntax Tree Printout. -----\n");

        printSymbolTable(symbolTable);

        deleteTree($2);

        clearSymbolTable(&symbolTable);
        }
    ;

declarations:
    declarations declaration 
    | declaration
    ;

declaration:
    DECLARE PREDICATE STRING COLON INT {
        fprintf(stderr, "PAR: Declaration: Predicate -%s- Arity: %d\n", $3, $5);
        if (getSymbolEntry(symbolTable, $3) != NULL) {
            fprintf(stderr, "ERROR: Predicate %s already declared\n", $3);
            exit(1);
        } else {
            addSymbolEntry(&symbolTable, $3, "predicate", $5);  
        }
    }
    | DECLARE FUNCTION STRING COLON INT {
        fprintf(stderr, "PAR: Declaration: Function -%s- Arity: %d\n", $3, $5);
        if (getSymbolEntry(symbolTable, $3) != NULL) {
            fprintf(stderr, "ERROR: Function already declared\n");
            exit(1);
        } else {
            addSymbolEntry(&symbolTable, $3, "function", $5);
        }
    }
    | DECLARE VARIABLE STRING COLON STRING {
        fprintf(stderr, "PAR: Declaration: Variable -%s- Type: %s\n", $3, $5);
        if (getSymbolEntry(symbolTable, $3) != NULL) {
            fprintf(stderr, "ERROR: Variable already declared\n");
            exit(1);
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
        fprintf(stderr,"PAR: JUNCTOR: EQUIV\n"); 
        $$ = makeNode(NODE_BINARY_OPERATOR);
        $$->treeTypes.binaryType.left = $1;
        $$->treeTypes.binaryType.right = $3;
        $$->treeTypes.binaryType.operatorType = BINOP_IFF;
        fprintf(stderr, "SYT: Binary Node created - Type IFF\n");
    }
;

implies_formula:
      or_formula { $$ = $1; }
    | implies_formula IMPLIES or_formula { 
        fprintf(stderr,"PAR: JUNCTOR: IMPLICATION\n"); 
        $$ = makeNode(NODE_BINARY_OPERATOR);
        $$->treeTypes.binaryType.left = $1;
        $$->treeTypes.binaryType.right = $3;
        $$->treeTypes.binaryType.operatorType = BINOP_IMPLIES;
        fprintf(stderr, "SYT: Binary Node created - Type IMPLIES\n");
    }
;

or_formula:
      and_formula { $$ = $1; }
    | or_formula OR and_formula { 
        fprintf(stderr,"PAR: JUNCTOR: OR\n"); 
        $$ = makeNode(NODE_BINARY_OPERATOR);
        $$->treeTypes.binaryType.left = $1;
        $$->treeTypes.binaryType.right = $3;
        $$->treeTypes.binaryType.operatorType = BINOP_OR;
        fprintf(stderr, "SYT: Binary Node created - Type OR\n");
    }
;

and_formula:
      not_formula { $$ = $1; }
    | and_formula AND not_formula { 
        fprintf(stderr,"PAR: JUNCTOR: AND\n"); 
        $$ = makeNode(NODE_BINARY_OPERATOR);
        $$->treeTypes.binaryType.left = $1;
        $$->treeTypes.binaryType.right = $3;
        $$->treeTypes.binaryType.operatorType = BINOP_AND;
        fprintf(stderr, "SYT: Binary Node created - Type AND\n");
    }
;

not_formula:
      quant_or_atom { $$ = $1; }
    | NOT not_formula { 
        fprintf(stderr,"PAR: JUNCTOR: NOT\n"); 
        $$ = makeNode(NODE_UNARY_OPERATOR);
        $$->treeTypes.unaryType.child = $2;
        $$->treeTypes.unaryType.operatorType = UOP_NOT;
        fprintf(stderr, "SYT: Unary Node created - Type NOT\n");
    }
;

quant_or_atom:
      TRUE { 
        $$ = makeNode(NODE_BOOL);
        $$->treeTypes.boolType.value = 1;
        fprintf(stderr,"SYT: Bool Node created (TRUE)\nPAR: CONST: TRUE\n"); 
      }
    | FALSE { 
        $$ = makeNode(NODE_BOOL);
        $$->treeTypes.boolType.value = 0;
        fprintf(stderr,"SYT: Bool Node created (FALSE)\nPAR: CONST: FALSE\n"); 
      }
    | atom { $$ = $1; }
    | ALL SQUARE_BRACKET_OPEN STRING SQUARE_BRACKET_CLOSE not_formula {
            struct tableEntry *entry = malloc(sizeof(struct tableEntry));
            entry->identifier = strdup($3);
            entry->type = strdup("variable");
            entry->arity = 0;
            entry->next = NULL;

            if(getSymbolEntry(symbolTable, $3) == NULL)
                addSymbolEntry(&symbolTable, $3, "variable", 0);
            $$ = makeNode(NODE_QUANTOR);
            $$->treeTypes.quantorType.quantorType = FORALL;
            $$->treeTypes.quantorType.var = entry;
            $$->treeTypes.quantorType.formula = $5;

            fprintf(stderr, "SYT: Quantor Node created - FORALL %s\n", $3);
            fprintf(stderr, "PAR: QUANTOR: ALL %s\n", $3);
    }
    | EXIST SQUARE_BRACKET_OPEN STRING SQUARE_BRACKET_CLOSE not_formula {
        struct tableEntry *entry = malloc(sizeof(struct tableEntry));
        entry->identifier = strdup($3);
        entry->type = strdup("variable");
        entry->arity = 0;
        entry->next = NULL;

        if(getSymbolEntry(symbolTable, $3) == NULL)
            addSymbolEntry(&symbolTable, $3, "variable", 0);
        $$ = makeNode(NODE_QUANTOR);
        $$->treeTypes.quantorType.quantorType = EXISTS;
        $$->treeTypes.quantorType.var = entry;
        $$->treeTypes.quantorType.formula = $5;

        fprintf(stderr, "SYT: Quantor Node created - EXISTS %s\n", $3);
        fprintf(stderr,"PAR: QUANTOR: EXIST %s\n",$3); 
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
        $$ = makeNode(NODE_PREDICATE);
        $$->treeTypes.predicatType.entry = entry;
        $$->treeTypes.predicatType.arguments = $3;
        fprintf(stderr, "SYT: Predicate Node created - %s\nPAR: ATOM: %s()\n", $1, $1);
    }
;

term_list_opt:
    term_list { $$ = $1; }
    | { $$ = NULL; }
;

term_list:
    term { $$ = $1; }
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
            entry = malloc(sizeof(struct tableEntry));
            entry->identifier = strdup($1);
            entry->type = strdup("variable");
            entry->arity = 0;
            entry->next = NULL;

            addSymbolEntry(&symbolTable, $1, "variable", 0);
            $$ = makeNode(NODE_VARIABLE);
            $$->treeTypes.variableType.entry = entry;
            fprintf(stderr, "SYT: Term Node created - %s (implicit variable)\n", $1);
        } else {
            $$ = makeNode(strcmp(entry->type, "variable") ==0 ? NODE_VARIABLE : NODE_FUNCTION);

            if(strcmp(entry->type,"variable")==0)
                $$->treeTypes.variableType.entry = entry;
            else
                $$->treeTypes.functionType.entry = entry;

            fprintf(stderr, "SYT: Variable Node created - %s\n", $1);
        }
    }
  | INT {
        $$ = makeNode(NODE_NUMBER);
        $$->treeTypes.numberType.value = $1;
        fprintf(stderr, "SYT: Number Node created - %d\nPAR: TERM: Constant %d\n", $1, $1);
    }
  | STRING BRACKET_OPEN term_list_opt BRACKET_CLOSE {
        struct tableEntry *entry = getSymbolEntry(symbolTable, $1);
        if(entry == NULL) { 
            fprintf(stderr, "ERROR: %s not declared\n", $1); 
            exit(1); 
            }
        if(!entry || strcmp(entry->type, "function") !=0) {
            fprintf(stderr, "ERROR: %s is not a function\n", $1);
            exit(1);
        }
        $$ = makeNode(NODE_FUNCTION);
        $$->treeTypes.functionType.entry = entry;
        $$->treeTypes.functionType.arguments = $3;
        fprintf(stderr, "SYT: Function Node created - %s(...)\nPAR: TERM: Function %s(...)\n", $1, $1);
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

    return result;
}