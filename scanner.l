%{
    #include <stdio.h>
%}

DIGIT [0-9]
ID [a-zA-Z][a-zA-Z0-9]*

%%

"/*"([^*]|\*+[^*/])*\*+"/";

"DECLARE" {fprintf(stderr, "LEX: KEY: DECLARE\n");}
"PREDICATE" {fprintf(stderr, "LEX: KEY: PREDICATE\n");}
"FUNCTION" {fprintf(stderr, "LEX: KEY: FUNCTION\n");}
"VARIABLE" {fprintf(stderr, "LEX: KEY: VARIABLE\n");}

"EXIST" {fprintf(stderr, "LEX: KEY: EXIST\n");}
"ALL" {fprintf(stderr, "LEX: KEY: ALL\n");}

"TRUE" {fprintf(stderr, "LEX: CONST: TRUE\n");}
"FALSE" {fprintf(stderr, "LEX: CONST: FALSE\n");}

"->" { fprintf(stderr, "LEX: OPERATOR: IMPLIES\n"); }
"<->" { fprintf(stderr, "LEX: OPERATOR: EQUIV\n"); }
"&" { fprintf(stderr, "LEX: OPERATOR: AND\n"); }
"|" { fprintf(stderr, "LEX: OPERATOR: OR\n"); }
"~" { fprintf(stderr, "LEX: OPERATOR: NOT\n"); }

"(" {fprintf(stderr, "LEX: KEY: Bracket Open\n");}
")" {fprintf(stderr, "LEX: KEY: Bracket Close\n");}
"[" {fprintf(stderr, "LEX: KEY: Square bracket Open\n");}
"]" {fprintf(stderr, "LEX: KEY: Square bracket Close\n");}

"," {fprintf(stderr, "LEX: KEY: Comma\n");}
":" {fprintf(stderr, "LEX: KEY: Colon\n");}
";" {fprintf(stderr, "LEX: KEY: Semicolon\n");}

{DIGIT}+ { fprintf(stderr, "LEX: INT: %s\n", yytext); }
{ID} {fprintf(stderr, "LEX: STRING: %s\n", yytext);}

[ \t\r\n]+ ; 

. {fprintf(stderr, "LEX ERROR: %s\n",yytext);}

%%
 
int yywrap(){
    return 1;
}

int main(int argc, char *argv[]){

FILE *fp;
fp = fopen(argv[1], "r");
yyin = fp;

yylex();
return 0;
}