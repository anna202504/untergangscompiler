%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboltable.h"
#include "tree.h"

struct tableEntry *symbolTable = NULL; 
struct node *astRoot = NULL;

static struct tableEntry *findSymbolEntrySilent(struct tableEntry *head, const char *identifier)
{
    struct tableEntry *current = head;
    while (current != NULL)
    {
        if (strcmp(current->identifier, identifier) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

static int countArgumentNodes(struct node *head)
{
    int count = 0;
    struct node *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}

int yylex(void);
void yyerror (const char *s);
extern FILE *yyin; 
%}

%union {
    char* str;
    int val;
    struct node *p; 
    char id[101];
}


/* Token-Definition */
%token DECLARE PREDICATE FUNCTION VARIABLE ID
%token EXIST ALL
%token TRUE FALSE
%token AND OR NOT IMPLIES EQUIV
%token BRACKET_OPEN BRACKET_CLOSE
%token SQUARE_BRACKET_OPEN SQUARE_BRACKET_CLOSE
%token COMMA COLON SEMICOLON

%token DIGIT

%token <val> INT 
%token <str> STRING

%type <p> block formula equiv_formula implies_formula or_formula and_formula not_formula quant_or_atom atom term
%type <p> term_list term_list_opt

%left OR
%left AND
%left IMPLIES
%left EQUIV
%right NOT

%start input


%%

/* Grammatik-Regeln*/

input: 
    input block | block
    ; 


block:
    declarations formula SEMICOLON {
        if (astRoot != NULL)
            deleteNode(astRoot);
        astRoot = $2;
        fprintf(stderr, "PAR: SEMICOLON\n");
        printNode(astRoot, 0);
        $$ = $2;
    }
    ;

declarations:
    declarations declaration | declaration
    ;


declaration: 

        DECLARE PREDICATE STRING COLON INT {
            fprintf(stderr, "PAR: Declaration: Predicate -%s- Arity: %d\n", $3, $5);
            if(findSymbolEntrySilent(symbolTable, $3)){
                yyerror("Predicate already declared"); 
            }
            else {
                addSymbolEntry(&symbolTable, $3, "predicate", $5);
            }
            free($3);
        }
        | DECLARE FUNCTION STRING COLON INT {
            fprintf(stderr, "PAR: Declaration: Function -%s- Arity: %d\n", $3, $5);            
            if(findSymbolEntrySilent(symbolTable, $3)){
                yyerror("Function already declared"); 
            }
            else {
                addSymbolEntry(&symbolTable, $3, "function", $5);
            }
            free($3);
        }
        | DECLARE VARIABLE STRING COLON STRING {
            fprintf(stderr, "PAR: Declaration: Variable -%s- Type: %s\n", $3, $5);
            if(findSymbolEntrySilent(symbolTable, $3)){
                yyerror("Variable already declared"); 
            }
            else {
                addSymbolEntry(&symbolTable, $3, $5, 0);
            }

            free($3);
            free($5);
        }
;

/* Formeln nach Vorrangreihenfolge der PL*/
formula:
    equiv_formula { $$ = $1; }
;

equiv_formula:
      implies_formula { $$ = $1; }
    | equiv_formula EQUIV implies_formula {
        fprintf(stderr,"PAR: JUNCTOR: EQUIV\n");
        $$ = makeNode(BType);
        if (!$$) YYABORT;
        $$->bType.binaryOperator = B_EQUIV;
        $$->bType.leftnode = $1;
        $$->bType.rightnode = $3;
                fprintf(stderr, "SYT: Binary Node created - Type EQUIV\n");
      }
;

implies_formula:
      or_formula { $$ = $1; }
    | implies_formula IMPLIES or_formula {
        fprintf(stderr,"PAR: JUNCTOR: IMPLICATION\n");
        $$ = makeNode(BType);
        if (!$$) YYABORT;
        $$->bType.binaryOperator = B_IMPLIES;
        $$->bType.leftnode = $1;
        $$->bType.rightnode = $3;
                fprintf(stderr, "SYT: Binary Node created - Type IMPLIES\n");
      }
;

or_formula:
      and_formula { $$ = $1; }
    | or_formula OR and_formula {
        fprintf(stderr,"PAR: JUNCTOR: OR\n");
        $$ = makeNode(BType);
        if (!$$) YYABORT;
        $$->bType.binaryOperator = B_OR;
        $$->bType.leftnode = $1;
        $$->bType.rightnode = $3;
                fprintf(stderr, "SYT: Binary Node created - Type OR\n");
      }
;

and_formula:
      not_formula { $$ = $1; }
    | and_formula AND not_formula {
        fprintf(stderr,"PAR: JUNCTOR: AND\n");
        $$ = makeNode(BType);
        if (!$$) YYABORT;
        $$->bType.binaryOperator = B_AND;
        $$->bType.leftnode = $1;
        $$->bType.rightnode = $3;
                fprintf(stderr, "SYT: Binary Node created - Type AND\n");
      }
;

not_formula:
      quant_or_atom { $$ = $1; }
    | NOT not_formula {
        fprintf(stderr,"PAR: JUNCTOR: NOT\n");
        $$ = makeNode(UType);
        if (!$$) YYABORT;
                $$->uType.unaryOperator = U_NOT;
        $$->uType.operand = $2;
        fprintf(stderr, "SYT: Unary Node created - Type NOT\n");
      }
;

quant_or_atom:
      TRUE {
        fprintf(stderr,"PAR: CONST: TRUE\n");
        $$ = makeNode(BooType);
        if (!$$) YYABORT;
        $$->booType.value = true;
                fprintf(stderr, "SYT: Bool Node created (TRUE)\n");
      }
    | FALSE {
        fprintf(stderr,"PAR: CONST: FALSE\n");
        $$ = makeNode(BooType);
        if (!$$) YYABORT;
        $$->booType.value = false;
                fprintf(stderr, "SYT: Bool Node created (FALSE)\n");
      }
    | atom { $$ = $1; }
    | ALL SQUARE_BRACKET_OPEN STRING SQUARE_BRACKET_CLOSE not_formula {
        fprintf(stderr,"PAR: QUANTOR: ALL %s\n",$3); 
        struct tableEntry *entry = findSymbolEntrySilent(symbolTable, $3);
        if (!entry)
            yyerror("Quantified variable not declared");

        struct node *varNode = makeNode(VType);
        struct node *qNode = makeNode(QType);
        if (!varNode || !qNode)
        {
            deleteNode(varNode);
            deleteNode(qNode);
            free($3);
            YYABORT;
        }

        varNode->vType.symbolTableEntry = entry;
        fprintf(stderr, "SYT: Variable Node created\n");
        qNode->qType.quantorOperator = FORALL;
        qNode->qType.quantorVariable = varNode;
        qNode->qType.quantorBody = $5;
        $$ = qNode;
        fprintf(stderr, "SYT: Quantor Node created - Type ALL\n");
        free($3); 
        }
    | EXIST SQUARE_BRACKET_OPEN STRING SQUARE_BRACKET_CLOSE not_formula {
        fprintf(stderr,"PAR: QUANTOR: EXIST %s\n",$3); 
        struct tableEntry *entry = findSymbolEntrySilent(symbolTable, $3);
        if (!entry)
            yyerror("Quantified variable not declared");

        struct node *varNode = makeNode(VType);
        struct node *qNode = makeNode(QType);
        if (!varNode || !qNode)
        {
            deleteNode(varNode);
            deleteNode(qNode);
            free($3);
            YYABORT;
        }

        varNode->vType.symbolTableEntry = entry;
        fprintf(stderr, "SYT: Variable Node created\n");
        qNode->qType.quantorOperator = EXISTS;
        qNode->qType.quantorVariable = varNode;
        qNode->qType.quantorBody = $5;
        $$ = qNode;
        fprintf(stderr, "SYT: Quantor Node created - Type EXIST\n");
        free($3); 
        }
    | BRACKET_OPEN formula BRACKET_CLOSE { $$ = $2; }
;

atom:
    STRING BRACKET_OPEN term_list_opt BRACKET_CLOSE {
        fprintf(stderr, "PAR: ATOM: %s()\n", $1);
        struct tableEntry *entry = findSymbolEntrySilent(symbolTable, $1);
        int argCount = countArgumentNodes($3);
        if (!entry)
            yyerror("Predicate not declared");
        else if (entry->arity != argCount)
            yyerror("Predicate arity mismatch");

        $$ = makeNode(PType);
        if (!$$)
        {
            deleteNode($3);
            free($1);
            YYABORT;
        }

        $$->pType.symbolTableEntry = entry;
        $$->pType.arguments = $3;
        fprintf(stderr, "SYT: Predicate Node created\n");
        free($1);
    }
;

term_list_opt:
    term_list { $$ = $1; }
    | { $$ = NULL; }
;

term_list:
    term { $$ = $1; }
    | term_list COMMA term {
        struct node *last = $1;
        while (last->next != NULL)
            last = last->next;
        last->next = $3;
        $$ = $1;
        fprintf(stderr, "SYT: Argument Node added to list\n");
    }
;

term:
    STRING {
        fprintf(stderr, "PAR: TERM: Variable/Constant %s\n", $1);
        struct tableEntry *entry = findSymbolEntrySilent(symbolTable, $1);
        if (!entry)
            yyerror("Variable/constant not declared");

        $$ = makeNode(VType);
        if (!$$)
        {
            free($1);
            YYABORT;
        }
        $$->vType.symbolTableEntry = entry;
        fprintf(stderr, "SYT: Variable Node created\n");
        fprintf(stderr, "SYT: Argument Node created\n");
        free($1);
    }
  | INT {
        fprintf(stderr, "PAR: TERM: Constant %d\n", $1);
        $$ = makeNode(NType);
        if (!$$) YYABORT;
        $$->nType.value = $1;
        fprintf(stderr, "SYT: Number Node created\n");
        fprintf(stderr, "SYT: Argument Node created\n");
    }
  | STRING BRACKET_OPEN term_list_opt BRACKET_CLOSE {
        fprintf(stderr, "PAR: TERM: Function %s(...)\n", $1);
        struct tableEntry *entry = findSymbolEntrySilent(symbolTable, $1);
        int argCount = countArgumentNodes($3);
        if (!entry)
            yyerror("Function not declared");
        else if (entry->arity != argCount)
            yyerror("Function arity mismatch");

        $$ = makeNode(FType);
        if (!$$)
        {
            deleteNode($3);
            free($1);
            YYABORT;
        }

        $$->fType.symbolTableEntry = entry;
        $$->fType.arguments = $3;
                fprintf(stderr, "SYT: Function Node created\n");
                fprintf(stderr, "SYT: Argument Node created\n");
        free($1);
      }
;




%%

void yyerror(const char *s){
    fprintf(stderr, "Error: %s\n", s);

}





int main(int argc, char *argv[]){
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("fopen");
        return 1;
    }

    yyin = fp;

    yyparse();

    fclose(fp);
    deleteNode(astRoot);

    return 0;
}