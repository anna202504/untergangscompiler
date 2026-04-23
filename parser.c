/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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

#line 88 "parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DECLARE = 3,                    /* DECLARE  */
  YYSYMBOL_PREDICATE = 4,                  /* PREDICATE  */
  YYSYMBOL_FUNCTION = 5,                   /* FUNCTION  */
  YYSYMBOL_VARIABLE = 6,                   /* VARIABLE  */
  YYSYMBOL_EXIST = 7,                      /* EXIST  */
  YYSYMBOL_ALL = 8,                        /* ALL  */
  YYSYMBOL_TRUE = 9,                       /* TRUE  */
  YYSYMBOL_FALSE = 10,                     /* FALSE  */
  YYSYMBOL_AND = 11,                       /* AND  */
  YYSYMBOL_OR = 12,                        /* OR  */
  YYSYMBOL_NOT = 13,                       /* NOT  */
  YYSYMBOL_IMPLIES = 14,                   /* IMPLIES  */
  YYSYMBOL_EQUIV = 15,                     /* EQUIV  */
  YYSYMBOL_BRACKET_OPEN = 16,              /* BRACKET_OPEN  */
  YYSYMBOL_BRACKET_CLOSE = 17,             /* BRACKET_CLOSE  */
  YYSYMBOL_SQUARE_BRACKET_OPEN = 18,       /* SQUARE_BRACKET_OPEN  */
  YYSYMBOL_SQUARE_BRACKET_CLOSE = 19,      /* SQUARE_BRACKET_CLOSE  */
  YYSYMBOL_COMMA = 20,                     /* COMMA  */
  YYSYMBOL_COLON = 21,                     /* COLON  */
  YYSYMBOL_SEMICOLON = 22,                 /* SEMICOLON  */
  YYSYMBOL_INT = 23,                       /* INT  */
  YYSYMBOL_STRING = 24,                    /* STRING  */
  YYSYMBOL_YYACCEPT = 25,                  /* $accept  */
  YYSYMBOL_input = 26,                     /* input  */
  YYSYMBOL_block = 27,                     /* block  */
  YYSYMBOL_declarations = 28,              /* declarations  */
  YYSYMBOL_declaration = 29,               /* declaration  */
  YYSYMBOL_formula = 30,                   /* formula  */
  YYSYMBOL_equiv_formula = 31,             /* equiv_formula  */
  YYSYMBOL_implies_formula = 32,           /* implies_formula  */
  YYSYMBOL_or_formula = 33,                /* or_formula  */
  YYSYMBOL_and_formula = 34,               /* and_formula  */
  YYSYMBOL_not_formula = 35,               /* not_formula  */
  YYSYMBOL_quant_or_atom = 36,             /* quant_or_atom  */
  YYSYMBOL_atom = 37,                      /* atom  */
  YYSYMBOL_term_list_opt = 38,             /* term_list_opt  */
  YYSYMBOL_term_list = 39,                 /* term_list  */
  YYSYMBOL_term = 40                       /* term  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   67

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  25
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  34
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  67

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   279


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    49,    49,    51,    55,    83,    84,    88,   101,   114,
     134,   138,   139,   150,   151,   162,   163,   174,   175,   186,
     187,   197,   203,   209,   210,   230,   250,   254,   281,   282,
     286,   289,   299,   327,   333
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "DECLARE", "PREDICATE",
  "FUNCTION", "VARIABLE", "EXIST", "ALL", "TRUE", "FALSE", "AND", "OR",
  "NOT", "IMPLIES", "EQUIV", "BRACKET_OPEN", "BRACKET_CLOSE",
  "SQUARE_BRACKET_OPEN", "SQUARE_BRACKET_CLOSE", "COMMA", "COLON",
  "SEMICOLON", "INT", "STRING", "$accept", "input", "block",
  "declarations", "declaration", "formula", "equiv_formula",
  "implies_formula", "or_formula", "and_formula", "not_formula",
  "quant_or_atom", "atom", "term_list_opt", "term_list", "term", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-22)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -22,     9,   -22,    23,   -22,    -2,   -22,   -20,   -11,    -9,
      -8,     3,   -22,   -22,    10,    10,     0,   -22,     8,    16,
      11,    20,    22,   -22,   -22,   -22,    14,    15,    17,    13,
      18,   -22,    24,   -21,   -22,    10,    10,    10,    10,    25,
      26,    21,    27,    28,   -22,   -22,    34,    35,    19,   -22,
      11,    20,    22,   -22,   -22,   -22,   -22,    10,    10,   -21,
     -22,   -21,   -22,   -22,    36,   -22,   -22
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     3,     0,     6,     0,     0,     0,
       0,     0,    21,    22,     0,     0,     0,     5,     0,    10,
      11,    13,    15,    17,    19,    23,     0,     0,     0,     0,
       0,    20,     0,    29,     4,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,    33,    32,     0,    28,    30,
      12,    14,    16,    18,     7,     8,     9,     0,     0,    29,
      27,     0,    25,    24,     0,    31,    34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -22,   -22,   -22,   -22,    46,    39,   -22,     5,    29,    30,
     -14,   -22,   -22,    -4,   -22,    -5
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     4,     5,     6,    18,    19,    20,    21,    22,
      23,    24,    25,    47,    48,    49
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      31,     3,    45,    46,    26,    10,    11,    12,    13,     2,
      29,    14,     3,    27,    15,    28,    33,    10,    11,    12,
      13,    30,    16,    14,    53,    36,    15,     7,     8,     9,
      34,    35,    37,    38,    16,    39,    40,    42,    41,    61,
      50,    44,    43,    62,    63,    56,    57,    58,    54,    55,
      59,    17,    60,    66,    32,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,    52
};

static const yytype_int8 yycheck[] =
{
      14,     3,    23,    24,    24,     7,     8,     9,    10,     0,
      18,    13,     3,    24,    16,    24,    16,     7,     8,     9,
      10,    18,    24,    13,    38,    14,    16,     4,     5,     6,
      22,    15,    12,    11,    24,    21,    21,    24,    21,    20,
      35,    17,    24,    57,    58,    24,    19,    19,    23,    23,
      16,     5,    17,    17,    15,    59,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    37
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    26,     0,     3,    27,    28,    29,     4,     5,     6,
       7,     8,     9,    10,    13,    16,    24,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    24,    24,    24,    18,
      18,    35,    30,    16,    22,    15,    14,    12,    11,    21,
      21,    21,    24,    24,    17,    23,    24,    38,    39,    40,
      32,    33,    34,    35,    23,    23,    24,    19,    19,    16,
      17,    20,    35,    35,    38,    40,    17
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    25,    26,    26,    27,    28,    28,    29,    29,    29,
      30,    31,    31,    32,    32,    33,    33,    34,    34,    35,
      35,    36,    36,    36,    36,    36,    36,    37,    38,    38,
      39,    39,    40,    40,    40
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     3,     2,     1,     5,     5,     5,
       1,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       2,     1,     1,     1,     5,     5,     3,     4,     1,     0,
       1,     3,     1,     1,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* block: declarations formula SEMICOLON  */
#line 55 "parser.y"
                                   { 
        fprintf(stderr, "PAR: Formula completed with Semicolon.\n");
        
        fprintf(stderr, "\n----- New Block Parsed -----\n"); 

        fprintf(stderr, "\n----- Start Syntax Tree Printout. -----\n");
        printTree((yyvsp[-1].p), 0);
        fprintf(stderr, "----- End of Syntax Tree Printout. -----\n");

        struct treeNode *opt = eliminate_double_negation((yyvsp[-1].p));
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
#line 1174 "parser.c"
    break;

  case 7: /* declaration: DECLARE PREDICATE STRING COLON INT  */
#line 88 "parser.y"
                                       {
        fprintf(stderr, "PAR: Declaration: Predicate -%s- Arity: %d\n", (yyvsp[-2].str), (yyvsp[0].val));

        struct tableEntry *entry = getSymbolEntry(symbolTable, (yyvsp[-2].str));
        if (entry != NULL) {
            if (strcmp(entry->type, "predicate") != 0 || entry->arity != (yyvsp[0].val)) {
                fprintf(stderr, "ERROR: Identifier %s already declared with different type\n", (yyvsp[-2].str));
            exit(1);
            }
        } else {
            addSymbolEntry(&symbolTable, (yyvsp[-2].str), "predicate", (yyvsp[0].val));  
        }
    }
#line 1192 "parser.c"
    break;

  case 8: /* declaration: DECLARE FUNCTION STRING COLON INT  */
#line 101 "parser.y"
                                        {
        fprintf(stderr, "PAR: Declaration: Function -%s- Arity: %d\n", (yyvsp[-2].str), (yyvsp[0].val));

        struct tableEntry *entry = getSymbolEntry(symbolTable, (yyvsp[-2].str));
        if (entry != NULL) {
            if (strcmp(entry->type, "function") != 0 || entry->arity != (yyvsp[0].val)) {
                fprintf(stderr, "ERROR: Identifier %s already declared with different type\n", (yyvsp[-2].str));
            exit(1);
            }
        } else {
            addSymbolEntry(&symbolTable, (yyvsp[-2].str), "function", (yyvsp[0].val));
        }
    }
#line 1210 "parser.c"
    break;

  case 9: /* declaration: DECLARE VARIABLE STRING COLON STRING  */
#line 114 "parser.y"
                                           {
        fprintf(stderr, "PAR: Declaration: Variable -%s- Type: %s\n", (yyvsp[-2].str), (yyvsp[0].str));

        if(strcmp((yyvsp[0].str), "int") != 0) {
            fprintf(stderr, "ERROR: Variable %s must have type int\n", (yyvsp[-2].str));
            exit(1);
        }
        struct tableEntry *entry = getSymbolEntry(symbolTable, (yyvsp[-2].str));
        if (entry != NULL) {
            if (strcmp(entry->type, "variable") != 0) {
                fprintf(stderr, "ERROR: Identifier %s already declared with different type\n", (yyvsp[-2].str));
            exit(1);
            }
        } else {
            addSymbolEntry(&symbolTable, (yyvsp[-2].str), "variable", 0);
        }
    }
#line 1232 "parser.c"
    break;

  case 11: /* equiv_formula: implies_formula  */
#line 138 "parser.y"
                      { (yyval.p) = (yyvsp[0].p); }
#line 1238 "parser.c"
    break;

  case 12: /* equiv_formula: equiv_formula EQUIV implies_formula  */
#line 139 "parser.y"
                                          { 
        fprintf(stderr,"PAR: Formula reduced - JUNCTOR: EQUIVALENT\n"); 
        (yyval.p) = makeNode(NODE_BINARY_OPERATOR);
        (yyval.p)->treeTypes.binaryType.left = (yyvsp[-2].p);
        (yyval.p)->treeTypes.binaryType.right = (yyvsp[0].p);
        (yyval.p)->treeTypes.binaryType.operatorType = BINOP_IFF;
        fprintf(stderr, "SYT: Binary Node created - Type <->\n");
    }
#line 1251 "parser.c"
    break;

  case 13: /* implies_formula: or_formula  */
#line 150 "parser.y"
                 { (yyval.p) = (yyvsp[0].p); }
#line 1257 "parser.c"
    break;

  case 14: /* implies_formula: implies_formula IMPLIES or_formula  */
#line 151 "parser.y"
                                         { 
        fprintf(stderr,"PAR: Formula reduced - JUNCTOR: IMPLICATION\n"); 
        (yyval.p) = makeNode(NODE_BINARY_OPERATOR);
        (yyval.p)->treeTypes.binaryType.left = (yyvsp[-2].p);
        (yyval.p)->treeTypes.binaryType.right = (yyvsp[0].p);
        (yyval.p)->treeTypes.binaryType.operatorType = BINOP_IMPLIES;
        fprintf(stderr, "SYT: Binary Node created - Type ->\n");
    }
#line 1270 "parser.c"
    break;

  case 15: /* or_formula: and_formula  */
#line 162 "parser.y"
                  { (yyval.p) = (yyvsp[0].p); }
#line 1276 "parser.c"
    break;

  case 16: /* or_formula: or_formula OR and_formula  */
#line 163 "parser.y"
                                { 
        fprintf(stderr,"PAR: Formula reduced - JUNCTOR: OR\n"); 
        (yyval.p) = makeNode(NODE_BINARY_OPERATOR);
        (yyval.p)->treeTypes.binaryType.left = (yyvsp[-2].p);
        (yyval.p)->treeTypes.binaryType.right = (yyvsp[0].p);
        (yyval.p)->treeTypes.binaryType.operatorType = BINOP_OR;
        fprintf(stderr, "SYT: Binary Node created - Type |\n");
    }
#line 1289 "parser.c"
    break;

  case 17: /* and_formula: not_formula  */
#line 174 "parser.y"
                  { (yyval.p) = (yyvsp[0].p); }
#line 1295 "parser.c"
    break;

  case 18: /* and_formula: and_formula AND not_formula  */
#line 175 "parser.y"
                                  { 
        fprintf(stderr,"PAR: Formula reduced - JUNCTOR: AND\n"); 
        (yyval.p) = makeNode(NODE_BINARY_OPERATOR);
        (yyval.p)->treeTypes.binaryType.left = (yyvsp[-2].p);
        (yyval.p)->treeTypes.binaryType.right = (yyvsp[0].p);
        (yyval.p)->treeTypes.binaryType.operatorType = BINOP_AND;
        fprintf(stderr, "SYT: Binary Node created - Type &\n");
    }
#line 1308 "parser.c"
    break;

  case 19: /* not_formula: quant_or_atom  */
#line 186 "parser.y"
                    { (yyval.p) = (yyvsp[0].p); }
#line 1314 "parser.c"
    break;

  case 20: /* not_formula: NOT not_formula  */
#line 187 "parser.y"
                      { 
        fprintf(stderr,"PAR: Formula reduced - JUNCTOR: NEGATION\n"); 
        (yyval.p) = makeNode(NODE_UNARY_OPERATOR);
        (yyval.p)->treeTypes.unaryType.child = (yyvsp[0].p);
        (yyval.p)->treeTypes.unaryType.operatorType = UOP_NOT;
        fprintf(stderr, "SYT: Unary Node created - Type ~\n");
    }
#line 1326 "parser.c"
    break;

  case 21: /* quant_or_atom: TRUE  */
#line 197 "parser.y"
           { 
        (yyval.p) = makeNode(NODE_BOOL);
        (yyval.p)->treeTypes.boolType.value = 1;
        fprintf(stderr, "PAR: Formula reduced - ATOM: TRUE\n");
        fprintf(stderr,"SYT: TRUE Node created\n"); 
      }
#line 1337 "parser.c"
    break;

  case 22: /* quant_or_atom: FALSE  */
#line 203 "parser.y"
            { 
        (yyval.p) = makeNode(NODE_BOOL);
        (yyval.p)->treeTypes.boolType.value = 0;
        fprintf(stderr, "PAR: Formula reduced - ATOM: FALSE\n");
        fprintf(stderr,"SYT: FALSE Node created\n"); 
      }
#line 1348 "parser.c"
    break;

  case 23: /* quant_or_atom: atom  */
#line 209 "parser.y"
           { (yyval.p) = (yyvsp[0].p); }
#line 1354 "parser.c"
    break;

  case 24: /* quant_or_atom: ALL SQUARE_BRACKET_OPEN STRING SQUARE_BRACKET_CLOSE not_formula  */
#line 210 "parser.y"
                                                                      {
            struct tableEntry *entry = getSymbolEntry(symbolTable, (yyvsp[-2].str));
            if(entry == NULL) {
                fprintf(stderr, "ERROR: Variable %s not declared\n", (yyvsp[-2].str));
                exit(1);
            }
            if(strcmp(entry->type, "variable") != 0) {
                fprintf(stderr, "ERROR: %s is not a variable\n", (yyvsp[-2].str));
                exit(1);
            }

            (yyval.p) = makeNode(NODE_QUANTOR);
            (yyval.p)->treeTypes.quantorType.quantorType = FORALL;
            (yyval.p)->treeTypes.quantorType.var = entry;
            (yyval.p)->treeTypes.quantorType.formula = (yyvsp[0].p);

            fprintf(stderr, "PAR: Formula reduced - QUANTOR: ALL %s\n", (yyvsp[-2].str));
            fprintf(stderr, "SYT: Variable Node created\n");
            fprintf(stderr, "SYT: Quantor Node created - Type ALL\n");
        }
#line 1379 "parser.c"
    break;

  case 25: /* quant_or_atom: EXIST SQUARE_BRACKET_OPEN STRING SQUARE_BRACKET_CLOSE not_formula  */
#line 230 "parser.y"
                                                                        {
        struct tableEntry *entry = getSymbolEntry(symbolTable, (yyvsp[-2].str));
        if(entry == NULL) {
            fprintf(stderr, "ERROR: Variable %s not declared\n", (yyvsp[-2].str));
            exit(1);
        }
        
        if(strcmp(entry->type, "variable") != 0) {
            fprintf(stderr, "ERROR: %s is not a variable\n", (yyvsp[-2].str));
            exit(1);
        }
        (yyval.p) = makeNode(NODE_QUANTOR);
        (yyval.p)->treeTypes.quantorType.quantorType = EXISTS;
        (yyval.p)->treeTypes.quantorType.var = entry;
        (yyval.p)->treeTypes.quantorType.formula = (yyvsp[0].p);

        fprintf(stderr,"PAR: Formula reduced - QUANTOR: EXIST %s\n",(yyvsp[-2].str)); 
        fprintf(stderr, "SYT: Variable Node created\n");
        fprintf(stderr,"SYT: Quantor Node created - Type EXIST\n"); 
    }
#line 1404 "parser.c"
    break;

  case 26: /* quant_or_atom: BRACKET_OPEN formula BRACKET_CLOSE  */
#line 250 "parser.y"
                                         { (yyval.p) = (yyvsp[-1].p); }
#line 1410 "parser.c"
    break;

  case 27: /* atom: STRING BRACKET_OPEN term_list_opt BRACKET_CLOSE  */
#line 254 "parser.y"
                                                    {
        struct tableEntry *entry = getSymbolEntry(symbolTable, (yyvsp[-3].str));
        if(entry == NULL) { 
            fprintf(stderr, "ERROR: %s not declared\n", (yyvsp[-3].str)); 
            exit(1); 
            }
        if(strcmp(entry->type, "predicate") != 0) {
            fprintf(stderr, "ERROR: %s is not declared as predicate\n", (yyvsp[-3].str));
            exit(1);
        }

        int argCount = countArguments((yyvsp[-1].p));
        if(argCount != entry->arity) {
            fprintf(stderr, "ERROR: Predicate %s expects %d argument(s), but got %d\n", (yyvsp[-3].str), entry->arity, argCount);
            exit(1);
        }

        (yyval.p) = makeNode(NODE_PREDICATE);
        (yyval.p)->treeTypes.predicatType.entry = entry;
        (yyval.p)->treeTypes.predicatType.arguments = (yyvsp[-1].p);
        fprintf(stderr, "PAR: Formula reduced - ATOM: %s() - %d arguments\n", (yyvsp[-3].str), argCount);
        fprintf(stderr, "SYT: Predicate Node created - %s\n", (yyvsp[-3].str));
        
    }
#line 1439 "parser.c"
    break;

  case 28: /* term_list_opt: term_list  */
#line 281 "parser.y"
              { (yyval.p) = (yyvsp[0].p); }
#line 1445 "parser.c"
    break;

  case 29: /* term_list_opt: %empty  */
#line 282 "parser.y"
      { (yyval.p) = NULL; }
#line 1451 "parser.c"
    break;

  case 30: /* term_list: term  */
#line 286 "parser.y"
         { (yyval.p) = (yyvsp[0].p); 
    fprintf(stderr, "SYT: Argument Node created\n"); 
    }
#line 1459 "parser.c"
    break;

  case 31: /* term_list: term_list COMMA term  */
#line 289 "parser.y"
                           {
        struct treeNode *last = (yyvsp[-2].p);
        while(last->next) last = last->next;
        last->next = (yyvsp[0].p);
        (yyval.p) = (yyvsp[-2].p); 
        fprintf(stderr, "SYT: Argument Node added to list\n");
    }
#line 1471 "parser.c"
    break;

  case 32: /* term: STRING  */
#line 299 "parser.y"
           {
        struct tableEntry *entry = getSymbolEntry(symbolTable, (yyvsp[0].str));
        if(entry == NULL) {
            fprintf(stderr, "ERROR: %s not declared\n", (yyvsp[0].str));
            exit(1);
        }

        if(strcmp(entry->type, "variable") == 0) {
            fprintf(stderr, "PAR: TERM: Variable/Constant %s\n", (yyvsp[0].str));
            (yyval.p) = makeNode(NODE_VARIABLE);
            (yyval.p)->treeTypes.variableType.entry = entry;
            fprintf(stderr, "SYT: Variable Node created - %s\n", (yyvsp[0].str));
        } else if(strcmp(entry->type, "function") == 0) {
            if(entry->arity != 0) {
                fprintf(stderr, "ERROR: Function %s expects %d argument(s), but got 0\n", (yyvsp[0].str), entry->arity);
                exit(1);
            }

            fprintf(stderr, "PAR: TERM: Variable/Constant %s\n", (yyvsp[0].str));
            (yyval.p) = makeNode(NODE_FUNCTION);
            (yyval.p)->treeTypes.functionType.entry = entry;
            (yyval.p)->treeTypes.functionType.arguments = NULL;
            fprintf(stderr, "SYT: Function Node created - %s\n", (yyvsp[0].str));
        } else {
            fprintf(stderr, "ERROR: %s cannot be used as term\n", (yyvsp[0].str));
            exit(1);
        }
    }
#line 1504 "parser.c"
    break;

  case 33: /* term: INT  */
#line 327 "parser.y"
        {
        fprintf(stderr, "PAR: TERM: Number: %d\n", (yyvsp[0].val));
        (yyval.p) = makeNode(NODE_NUMBER);
        (yyval.p)->treeTypes.numberType.value = (yyvsp[0].val);
        fprintf(stderr, "SYT: Number Node created - %d\n", (yyvsp[0].val));
    }
#line 1515 "parser.c"
    break;

  case 34: /* term: STRING BRACKET_OPEN term_list_opt BRACKET_CLOSE  */
#line 333 "parser.y"
                                                    {
        struct tableEntry *entry = getSymbolEntry(symbolTable, (yyvsp[-3].str));
        if(entry == NULL) { 
            fprintf(stderr, "ERROR: %s not declared\n", (yyvsp[-3].str)); 
            exit(1); 
            }
        if(strcmp(entry->type, "function") !=0) {
            fprintf(stderr, "ERROR: %s is not a function\n", (yyvsp[-3].str));
            exit(1);
        }

        int argCount = countArguments((yyvsp[-1].p));
        if(argCount != entry->arity) {
            fprintf(stderr, "ERROR: Function %s expects %d argument(s), but got %d\n", (yyvsp[-3].str), entry->arity, argCount);
            exit(1);
        }

        fprintf(stderr, "PAR: TERM: Function %s() - %d arguments\n", (yyvsp[-3].str), argCount);
        (yyval.p) = makeNode(NODE_FUNCTION);
        (yyval.p)->treeTypes.functionType.entry = entry;
        (yyval.p)->treeTypes.functionType.arguments = (yyvsp[-1].p);
        fprintf(stderr, "SYT: Function Node created - %s(...)\n", (yyvsp[-3].str));
    }
#line 1543 "parser.c"
    break;


#line 1547 "parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 358 "parser.y"


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
