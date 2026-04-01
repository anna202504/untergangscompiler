/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DECLARE = 258,
     PREDICATE = 259,
     FUNCTION = 260,
     VARIABLE = 261,
     ID = 262,
     EXIST = 263,
     ALL = 264,
     TRUE = 265,
     FALSE = 266,
     AND = 267,
     OR = 268,
     NOT = 269,
     IMPLIES = 270,
     EQUIV = 271,
     BRACKET_OPEN = 272,
     BRACKET_CLOSE = 273,
     SQUARE_BRACKET_OPEN = 274,
     SQUARE_BRACKET_CLOSE = 275,
     COMMA = 276,
     COLON = 277,
     SEMICOLON = 278,
     DIGIT = 279,
     INT = 280,
     STRING = 281
   };
#endif
/* Tokens.  */
#define DECLARE 258
#define PREDICATE 259
#define FUNCTION 260
#define VARIABLE 261
#define ID 262
#define EXIST 263
#define ALL 264
#define TRUE 265
#define FALSE 266
#define AND 267
#define OR 268
#define NOT 269
#define IMPLIES 270
#define EQUIV 271
#define BRACKET_OPEN 272
#define BRACKET_CLOSE 273
#define SQUARE_BRACKET_OPEN 274
#define SQUARE_BRACKET_CLOSE 275
#define COMMA 276
#define COLON 277
#define SEMICOLON 278
#define DIGIT 279
#define INT 280
#define STRING 281




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 13 "parser.y"
{
    char* str;
    int val;
}
/* Line 1529 of yacc.c.  */
#line 106 "parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

