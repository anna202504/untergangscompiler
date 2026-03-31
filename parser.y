%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

struct tableEntry *symbolTable = NULL;

int yylex(void);
void yyerror(const char *s);
extern FILE *yyin;
%}

%union {
    char* str;
    int val;
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

%%

input:
    input block | block
    ;

block:
    declarations formula SEMICOLON {fprintf(stderr, "PAR: SEMICOLON\n");}
    ;
    
declarations:
    declarations declaration | declaration
    ;

declaration:
    DECLARE PREDICATE STRING COLON INT {
        fprintf(stderr, "PAR: Declaration: Predicate -%s- Arity: %d\n", $3, $5);
        if (getSymbolEntry(symbolTable, $3) != NULL) {
            fprintf(stderr, "ERROR: Predicate %s already declared\n", $3);
            exit(1);
        }
        else {
            addSymbolEntry(&symbolTable, $3, "predicate", $5);
        }
        free($3);
    }
    | DECLARE FUNCTION STRING COLON INT {
        fprintf(stderr, "PAR: Declaration: Function -%s- Arity: %d\n", $3, $5);
        if (getSymbolEntry(symbolTable, $3) != NULL) {
            fprintf(stderr, "ERROR: FUnction already declared\n");
            exit(1);
        }
        else {
            addSymbolEntry(&symbolTable, $3, "function", $5);
        }
        free($3);
    }
    | DECLARE VARIABLE STRING COLON STRING {
        fprintf(stderr, "PAR: Declaration: Variable -%s- Type: %s\n", $3, $5);
        if (getSymbolEntry(symbolTable, $3) != NULL) {
            fprintf(stderr, "ERROR: Variable already declared\n");
            exit(1);
        }
        else {
            addSymbolEntry(&symbolTable, $3, "variable", 0);
        }

        free($3);
        free($5);
    }
;

formula:
    equiv_formula
;

equiv_formula:
      implies_formula
    | equiv_formula EQUIV implies_formula { fprintf(stderr,"PAR: JUNCTOR: EQUIV\n"); }
;

implies_formula:
      or_formula
    | implies_formula IMPLIES or_formula { fprintf(stderr,"PAR: JUNCTOR: IMPLICATION\n"); }
;

or_formula:
      and_formula
    | or_formula OR and_formula { fprintf(stderr,"PAR: JUNCTOR: OR\n"); }
;

and_formula:
      not_formula
    | and_formula AND not_formula { fprintf(stderr,"PAR: JUNCTOR: AND\n"); }
;

not_formula:
      quant_or_atom
    | NOT not_formula { fprintf(stderr,"PAR: JUNCTOR: NOT\n"); }
;

quant_or_atom:
      TRUE { fprintf(stderr,"PAR: CONST: TRUE\n"); }
    | FALSE { fprintf(stderr,"PAR: CONST: FALSE\n"); }
    | atom
    | ALL SQUARE_BRACKET_OPEN STRING SQUARE_BRACKET_CLOSE not_formula 
{
    struct tableEntry *entry = getSymbolEntry(symbolTable, $3);

    if (entry == NULL) {
        fprintf(stderr, "ERROR: Variable %s not declared\n", $3);
        exit(1);
    }

    if (strcmp(entry->type, "variable") != 0) {
        fprintf(stderr, "ERROR: %s is not a variable\n", $3);
        exit(1);
    }

    fprintf(stderr, "PAR: QUANTOR: ALL %s\n", $3);
    free($3);
}

    | EXIST SQUARE_BRACKET_OPEN STRING SQUARE_BRACKET_CLOSE not_formula {
        fprintf(stderr,"PAR: QUANTOR: EXIST %s\n",$3); free($3); }
    | BRACKET_OPEN formula BRACKET_CLOSE
;

atom:
    STRING BRACKET_OPEN term_list_opt BRACKET_CLOSE {
        struct tableEntry *entry = getSymbolEntry(symbolTable, $1);

        if(entry == NULL) {
            fprintf(stderr, "ERROR: %s not declared\n", $1);
            exit(1);
        }
        if(strcmp(entry->type, "predicate") != 0) {
            fprintf(stderr, "ERROR: %s is not a predicate\n", $1);
            exit(1);
        }
        fprintf(stderr, "PAR: ATOM: %s()\n", $1);
        free($1);
    }
;

term_list_opt:
    term_list 
    |
;

term_list:
    term | term_list COMMA term
;

term:
    STRING {
        struct tableEntry *entry = getSymbolEntry(symbolTable, $1);
        if(entry == NULL) {
            fprintf(stderr, "ERROR: %s not declared\n", $1);
            exit(1);
        }
        if(strcmp(entry->type, "variable") != 0 &&
        strcmp(entry->type, "function") != 0) {
            fprintf(stderr, "ERROR: %s is not a valid term\n", $1);
            exit(1);
        }
        fprintf(stderr, "PAR: TERM: Variable/Constant %s\n", $1);
        free($1);
    }
  | INT {
        fprintf(stderr, "PAR: TERM: Constant %d\n", $1);
    }
  | STRING BRACKET_OPEN term_list_opt BRACKET_CLOSE {
    struct tableEntry *entry = getSymbolEntry(symbolTable, $1);

    if(entry == NULL) {
        fprintf(stderr, "ERROR: %s not declared\n", $1);
        exit(1);
    }
    if(strcmp(entry->type, "function") != 0) {
        fprintf(stderr, "ERROR: %s is not a function\n", $1);
        exit(1);
    }
        fprintf(stderr, "PAR: TERM: Function %s(...)\n", $1);
        free($1);
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

    printSymbolTable(symbolTable);

    return result;
} 