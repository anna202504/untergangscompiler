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
     EXIST = 262,
     ALL = 263,
     TRUE = 264,
     FALSE = 265,
     BIN_AND = 266,
     BIN_OR = 267,
     UNARY_NOT = 268,
     BIN_IMPLIES = 269,
     BIN_EQUIV = 270,
     BRACKET_OPEN = 271,
     BRACKET_CLOSE = 272,
     SQUARE_BRACKET_OPEN = 273,
     SQUARE_BRACKET_CLOSE = 274,
     COMMA = 275,
     COLON = 276,
     SEMICOLON = 277,
     INT = 278,
     STRING = 279,
     OR = 280,
     AND = 281,
     IMPLIES = 282,
     EQUIV = 283,
     NOT = 284
   };
#endif
/* Tokens.  */
#define DECLARE 258
#define PREDICATE 259
#define FUNCTION 260
#define VARIABLE 261
#define EXIST 262
#define ALL 263
#define TRUE 264
#define FALSE 265
#define BIN_AND 266
#define BIN_OR 267
#define UNARY_NOT 268
#define BIN_IMPLIES 269
#define BIN_EQUIV 270
#define BRACKET_OPEN 271
#define BRACKET_CLOSE 272
#define SQUARE_BRACKET_OPEN 273
#define SQUARE_BRACKET_CLOSE 274
#define COMMA 275
#define COLON 276
#define SEMICOLON 277
#define INT 278
#define STRING 279
#define OR 280
#define AND 281
#define IMPLIES 282
#define EQUIV 283
#define NOT 284




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 15 "parser.y"
{
    char* str;
    int val;
    struct treeNode *p;
    char id[101];
}
/* Line 1529 of yacc.c.  */
#line 114 "parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

