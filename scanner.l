%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include "parser.h"
%}

DIGIT [0-9]
ID [a-zA-Z][a-zA-Z0-9]*

%%

"/*"(.|\n)*?"*/" ;  
[ \t\r\n]+ ; 

"DECLARE" {fprintf(stderr, "LEX: KEY: DECLARE\n"); return DECLARE;}
"PREDICATE" {fprintf(stderr, "LEX: KEY: PREDICATE\n"); return PREDICATE;}
"FUNCTION" {fprintf(stderr, "LEX: KEY: FUNCTION\n"); return FUNCTION;}
"VARIABLE" {fprintf(stderr, "LEX: KEY: VARIABLE\n"); return VARIABLE;}

"EXIST" {fprintf(stderr, "LEX: KEY: EXIST\n"); return EXIST;}
"ALL" {fprintf(stderr, "LEX: KEY: ALL\n"); return ALL;}

"TRUE" {fprintf(stderr, "LEX: CONST: TRUE\n"); return TRUE;}
"FALSE" {fprintf(stderr, "LEX: CONST: FALSE\n"); return FALSE;}

"->" { fprintf(stderr, "LEX: OPERATOR: IMPLIES\n"); return IMPLIES;}
"<->" { fprintf(stderr, "LEX: OPERATOR: EQUIV\n"); return EQUIV;}
"&" { fprintf(stderr, "LEX: OPERATOR: AND\n"); return AND;}
"|" { fprintf(stderr, "LEX: OPERATOR: OR\n"); return OR;}
"~" { fprintf(stderr, "LEX: OPERATOR: NOT\n"); return NOT;}

"(" {fprintf(stderr, "LEX: KEY: Bracket Open\n"); return BRACKET_OPEN; }
")" {fprintf(stderr, "LEX: KEY: Bracket Close\n"); return BRACKET_CLOSE;}
"[" {fprintf(stderr, "LEX: KEY: Square bracket Open\n"); return SQUARE_BRACKET_OPEN;}
"]" {fprintf(stderr, "LEX: KEY: Square bracket Close\n"); return SQUARE_BRACKET_CLOSE;}

"," {fprintf(stderr, "LEX: KEY: Comma\n"); return COMMA;}
":" {fprintf(stderr, "LEX: KEY: Colon\n"); return COLON;}
";" {fprintf(stderr, "LEX: KEY: Semicolon\n"); return SEMICOLON;}

{DIGIT}+ { yylval.val = atoi(yytext); fprintf(stderr, "LEX: INT: %d\n", yylval.val); return INT;}
{ID} {fprintf(stderr, "LEX: STRING: %s\n", yytext); yylval.str = strdup(yytext); return STRING;}


. {fprintf(stderr, "LEX ERROR: %s\n",yytext);}

%%
 
int yywrap(){
    return 1;
}

