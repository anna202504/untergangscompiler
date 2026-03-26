%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
extern int yyparse();
%}

DIGIT [0-9]
ID [a-zA-Z][a-zA-Z0-9]*

%%
"/*"([^*]|\*+[^*/])*\*+"/" { fprintf(stderr, "LEX: COMMENT: %s\n", yytext); }

"DECLARE" { fprintf(stderr, "LEX: KEY: DECLARE\n"); return DECLARE; }
"PREDICATE" { fprintf(stderr, "LEX: KEY: PREDICATE\n"); return PREDICATE; }
"FUNCTION" { fprintf(stderr, "LEX: KEY: FUNCTION\n"); return FUNCTION; }
"VARIABLE" { fprintf(stderr, "LEX: KEY: Variable\n"); return VARIABLE; }

"EXIST" { fprintf(stderr, "LEX: KEY: EXIST\n"); return EXIST;}
"ALL" { fprintf(stderr, "LEX: KEY: ALL\n"); return ALL;}

"TRUE" { fprintf(stderr, "LEX: KEY: TRUE\n"); return TRUE;}
"FALSE" { fprintf(stderr, "LEX: KEY: FALSE\n"); return FALSE;}

"->" { fprintf(stderr, "LEX: KEY: IMPLICATE\n"); return IMPLIES;}
"<->" { fprintf(stderr, "LEX: KEY: EQUIVALENT\n"); return EQUIV;}
"&" { fprintf(stderr, "LEX: KEY: AND\n"); return AND;}
"|" { fprintf(stderr, "LEX: KEY: OR\n"); return OR;}
"~" { fprintf(stderr, "LEX: KEY: NOT\n"); return NOT;}

"(" { fprintf(stderr, "LEX: KEY: Bracket Open\n"); return BRACKET_OPEN;}
")" { fprintf(stderr, "LEX: KEY: Bracket Close\n"); return BRACKET_CLOSE;}
"[" { fprintf(stderr, "LEX: KEY: Square Open\n"); return SQUARE_BRACKET_OPEN;}
"]" { fprintf(stderr, "LEX: KEY: Square Close\n"); return SQUARE_BRACKET_CLOSE;}

"," { fprintf(stderr, "LEX: KEY: Comma\n"); return COMMA;}
":" { fprintf(stderr, "LEX: KEY: Colon\n"); return COLON;}
";" { fprintf(stderr, "LEX: KEY: Semicolon\n"); return SEMICOLON;}

"int" { fprintf(stderr, "LEX: KEY: INT\n"); yylval.str = strdup(yytext); return STRING; }

{DIGIT}+ { fprintf(stderr, "LEX: NUMBER: %d\n", atoi(yytext)); yylval.val = atoi(yytext); return INT;}
{ID} { fprintf(stderr, "LEX: STRING: %s\n", yytext); yylval.str = strdup(yytext); return STRING;}

[ \t\r\n]+ ;

. { fprintf(stderr, "LEX ERROR: %s\n", yytext);}

%%

int yywrap() { return 1; }