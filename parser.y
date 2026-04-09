%{
#include <stdio.h>
#include <stdlib.h>
#include "symboltable.h"
#include "tree.h"
#include <string.h>
#include <stdarg.h>

struct tableEntry *symbolTable = NULL;
FILE *stp_output = NULL;
struct node *root_tree = NULL;

// Struktur für Formeln-Liste
struct formula_list {
    struct node *tree;
    struct formula_list *next;
};

struct formula_list *formulas = NULL;

int yylex(void);
void yyerror (const char *s);
extern FILE *yyin; 
%}

%union {
    char* str;
    int val;
    struct node *node;
    struct tableEntry *entry;
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

%type <node> formula equiv_formula implies_formula or_formula and_formula not_formula quant_or_atom atom
%type <node> term term_list term_list_opt
%type <node> input block
%type <entry> variable_ref

%left OR
%left AND
%left IMPLIES
%left EQUIV
%right NOT

%start input


%%

/* Grammatik-Regeln*/

input:
    input block {
        // Mehrere unabhängige Blöcke verarbeiten
        root_tree = $2;
    }
    | block {
        root_tree = $1;
    }
    ;

block:
    declarations formula SEMICOLON {
        fprintf(stderr, "PAR: SEMICOLON\n");
        // Formula zur Liste hinzufügen (behalte die globale Symbolentabelle für alle Blöcke)
        struct formula_list *new_entry = (struct formula_list *)malloc(sizeof(struct formula_list));
        new_entry->tree = $2;
        new_entry->next = formulas;
        formulas = new_entry;
        $$ = $2;
    }
    ;

declarations:
    declarations declaration | declaration
    ;


declaration: 

        DECLARE PREDICATE STRING COLON INT {
            fprintf(stderr, "PAR: Declaration: Predicate -%s- Arity: %d\n", $3, $5);
            addSymbolEntry(&symbolTable, $3, "predicate", $5);
            free($3);

        }
        | DECLARE FUNCTION STRING COLON INT {
            fprintf(stderr, "PAR: Declaration: Function -%s- Arity: %d\n", $3, $5);
            addSymbolEntry(&symbolTable, $3, "function", $5);
            free($3);
        }
        | DECLARE VARIABLE STRING COLON STRING {
            fprintf(stderr, "PAR: Declaration: Variable -%s- Type: %s\n", $3, $5);
            if (getSymbolEntry(symbolTable, $3)) {
                yyerror("Variable already declared");
            }
            else {
                addSymbolEntry(&symbolTable, $3, $5, 0);
            }
            free($3); free($5);
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
        $$ = create_binary_node(OP_EQUIV, $1, $3);
    }
;

implies_formula:
      or_formula { $$ = $1; }
    | implies_formula IMPLIES or_formula { 
        fprintf(stderr,"PAR: JUNCTOR: IMPLICATION\n"); 
        $$ = create_binary_node(OP_IMPLIES, $1, $3);
    }
;

or_formula:
      and_formula { $$ = $1; }
    | or_formula OR and_formula { 
        fprintf(stderr,"PAR: JUNCTOR: OR\n"); 
        $$ = create_binary_node(OP_OR, $1, $3);
    }
;

and_formula:
      not_formula { $$ = $1; }
    | and_formula AND not_formula { 
        fprintf(stderr,"PAR: JUNCTOR: AND\n"); 
        $$ = create_binary_node(OP_AND, $1, $3);
    }
;

not_formula:
      quant_or_atom { $$ = $1; }
    | NOT not_formula { 
        fprintf(stderr,"PAR: JUNCTOR: NOT\n"); 
        $$ = create_unary_node(OP_NOT, $2);
    }
;

quant_or_atom:
      TRUE { 
        fprintf(stderr,"PAR: CONST: TRUE\n"); 
        $$ = create_bool_node(true);
    }
    | FALSE { 
        fprintf(stderr,"PAR: CONST: FALSE\n"); 
        $$ = create_bool_node(false);
    }
    | atom { $$ = $1; }
    | ALL SQUARE_BRACKET_OPEN variable_ref SQUARE_BRACKET_CLOSE not_formula {
        fprintf(stderr,"PAR: QUANTOR: ALL\n");
        $$ = create_quantor_node(OP_FORALL, $3, $5);
    }
    | EXIST SQUARE_BRACKET_OPEN variable_ref SQUARE_BRACKET_CLOSE not_formula {
        fprintf(stderr,"PAR: QUANTOR: EXIST\n");
        $$ = create_quantor_node(OP_EXISTS, $3, $5);
    }
    | BRACKET_OPEN formula BRACKET_CLOSE { $$ = $2; }
;

variable_ref:
    STRING {
        $$ = getSymbolEntry(symbolTable, $1);
        if (!$$) {
            fprintf(stderr, "Error: Variable %s not declared\n", $1);
        }
        free($1);
    }
;

atom:
    STRING BRACKET_OPEN term_list_opt BRACKET_CLOSE {
        fprintf(stderr, "PAR: ATOM: %s()\n", $1);
        struct tableEntry *entry = getSymbolEntry(symbolTable, $1);
        if (!entry) {
            fprintf(stderr, "Error: Predicate %s not declared\n", $1);
            entry = symbolTable;
        }
        $$ = create_predicate_node(entry, $3);
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
        // Link terms together in argument list using ARGLIST binary nodes
        $$ = create_binary_node(OP_ARGLIST, $1, $3);
    }
;

term:
    STRING {
        fprintf(stderr, "PAR: TERM: Variable/Constant %s\n", $1);
        struct tableEntry *entry = getSymbolEntry(symbolTable, $1);
        if (entry && strcmp(entry->type, "function") == 0) {
            // It's a function (even if arity is 0)
            $$ = create_function_node(entry, NULL);
        } else if (entry) {
            $$ = create_variable_node(entry);
        } else {
            // Try to create a variable node anyway
            addSymbolEntry(&symbolTable, $1, "unknown", 0);
            entry = getSymbolEntry(symbolTable, $1);
            $$ = create_variable_node(entry);
        }
        free($1);
    }
  | INT {
        fprintf(stderr, "PAR: TERM: Constant %d\n", $1);
        $$ = create_number_node($1);
    }
  | STRING BRACKET_OPEN term_list_opt BRACKET_CLOSE {
        fprintf(stderr, "PAR: TERM: Function %s(...)\n", $1);
        struct tableEntry *entry = getSymbolEntry(symbolTable, $1);
        if (!entry) {
            addSymbolEntry(&symbolTable, $1, "unknown", 0);
            entry = getSymbolEntry(symbolTable, $1);
        }
        $$ = create_function_node(entry, $3);
        free($1);
    }
;

%%

void yyerror(const char *s){
    printf("Error: %s\n", s);
}

int main(int argc, char *argv[]){

    FILE *fp;
    fp = fopen(argv[1], "r");
    yyin = fp;
    
    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "%s.tree.txt", argv[1]);
    stp_output = fopen(output_filename, "w");
    if (!stp_output) {
        fprintf(stderr, "Error: Could not open output file %s\n", output_filename);
        return 1;
    }

    yyparse();
    
    // Liste umkehren, damit Formeln in korrekter Reihenfolge ausgegeben werden
    struct formula_list *reversed = NULL;
    struct formula_list *current = formulas;
    while (current != NULL) {
        struct formula_list *next = current->next;
        current->next = reversed;
        reversed = current;
        current = next;
    }
    
    // Alle Formeln ausgeben (jetzt in korrekter Reihenfolge)
    current = reversed;
    while (current != NULL) {
        if (current->tree) {
            fprintf(stp_output, "STP: ----- Start Syntax Tree Printout. -----\n");
            print_tree(stp_output, current->tree, 0);
            fprintf(stp_output, "STP: ----- End of Syntax Tree Printout. -----\n");
            free_tree(current->tree);
        }
        struct formula_list *next = current->next;
        free(current);
        current = next;
    }
    
    // Gebe die Symbolentabelle am Ende frei
    clearSymbolTable(&symbolTable);
    
    if (stp_output) fclose(stp_output);
    if (fp) fclose(fp);

    return 0;
}
