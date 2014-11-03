/* A Bison parser, made by GNU Bison 2.7.1.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 27 "compose.y"

#include "compose/compiler.h"

#include "compose/comp_mem.h"
#define YYMALLOC COMP_ALLOC
#define YYFREE COMP_FREE

#define YYDEBUG 1
#include <stdio.h>

/* Line 371 of yacc.c  */
#line 79 "compose_parser.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "compose_parser.h".  */
#ifndef YY_YY_COMPOSE_PARSER_H_INCLUDED
# define YY_YY_COMPOSE_PARSER_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     INT_LITERAL = 259,
     TRUE = 260,
     FALSE = 261,
     FLOAT_LITERAL = 262,
     VOID = 263,
     BOOL = 264,
     CHAR = 265,
     BYTE = 266,
     SHORT = 267,
     USHORT = 268,
     INT = 269,
     UINT = 270,
     LONG = 271,
     ULONG = 272,
     HALF = 273,
     FLOAT = 274,
     DOUBLE = 275,
     COLOR = 276,
     VEC2 = 277,
     VEC3 = 278,
     VEC4 = 279,
     MAT3 = 280,
     MAT34 = 281,
     MAT4 = 282,
     HANDLE_ = 283,
     ENTITY = 284,
     IF = 285,
     SWITCH = 286,
     CASE = 287,
     DEFAULT = 288,
     FOR = 289,
     WHILE = 290,
     DO = 291,
     BREAK = 292,
     RETURN = 293,
     COMPONENT = 294,
     COMPONENTS = 295,
     IMPORT = 296,
     AS = 297,
     CONST = 298,
     THIS = 299,
     NONE = 300,
     THEN = 301,
     ELSE = 302,
     OR_ASSIGN = 303,
     XOR_ASSIGN = 304,
     AND_ASSIGN = 305,
     RSHIFT_ASSIGN = 306,
     LSHIFT_ASSIGN = 307,
     MOD_ASSIGN = 308,
     DIV_ASSIGN = 309,
     MUL_ASSIGN = 310,
     SUB_ASSIGN = 311,
     ADD_ASSIGN = 312,
     OR = 313,
     AND = 314,
     NEQ = 315,
     EQ = 316,
     GTE = 317,
     LTE = 318,
     RSHIFT = 319,
     LSHIFT = 320,
     DEC = 321,
     INC = 322,
     UMINUS = 323,
     POSTDEC = 324,
     POSTINC = 325,
     SCOPE = 326
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 49 "compose.y"

    int         numi;
    float       numf;
    const char* str;
    DataType    dataType;
    Ast*        pAst;
    SymTab*     pSymTab;


/* Line 387 of yacc.c  */
#line 203 "compose_parser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (ParseData * pParseData);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_COMPOSE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */
/* Line 390 of yacc.c  */
#line 58 "compose.y"

#define YY_NO_UNISTD_H
#include "compose/compose_scanner.h"
#define YYLEX_PARAM parsedata_scanner(pParseData)

#define YYPRINT(file, type, value)   yyprint (file, type, value)
static void yyprint(FILE * file, int type, YYSTYPE value);

/* Line 390 of yacc.c  */
#line 252 "compose_parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif


/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  43
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1110

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  97
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  146
/* YYNRULES -- Number of states.  */
#define YYNSTATES  293

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   326

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    77,     2,    79,    96,    76,    63,     2,
      86,    89,    74,    72,    94,    73,    83,    75,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    93,
      66,    48,    67,     2,    95,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    87,     2,    90,    62,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    88,    61,    91,    78,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    64,    65,    68,    69,    70,
      71,    80,    81,    82,    84,    85,    92
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    13,    19,    21,    23,
      27,    29,    31,    34,    38,    42,    49,    57,    64,    67,
      71,    73,    76,    83,    90,    95,   101,   105,   108,   109,
     112,   117,   120,   124,   126,   129,   132,   138,   139,   141,
     145,   149,   152,   156,   158,   161,   167,   175,   181,   189,
     199,   207,   216,   220,   222,   225,   227,   228,   230,   232,
     236,   239,   244,   246,   250,   254,   258,   262,   266,   270,
     274,   278,   282,   286,   290,   294,   298,   302,   306,   310,
     314,   318,   322,   326,   330,   334,   338,   341,   344,   347,
     350,   353,   356,   359,   362,   364,   369,   374,   379,   384,
     389,   396,   400,   404,   408,   412,   416,   420,   422,   424,
     426,   428,   430,   431,   433,   434,   436,   437,   439,   443,
     446,   448,   451,   453,   455,   458,   460,   462,   464,   466,
     468,   470,   472,   474,   476,   478,   480,   482,   484,   486,
     488,   490,   492,   494,   496,   498,   500
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      98,     0,    -1,   104,    -1,    99,   104,    -1,   100,    -1,
      99,   100,    -1,    41,   101,    42,   101,    93,    -1,   102,
      -1,   103,    -1,   101,    83,   103,    -1,     3,    -1,   105,
      -1,   104,   105,    -1,    29,     3,   106,    -1,    39,     3,
     106,    -1,   126,     3,    86,   109,    89,   115,    -1,    43,
      29,     3,    86,   109,    89,   115,    -1,    29,     3,    86,
     109,    89,   115,    -1,    88,    91,    -1,    88,   107,    91,
      -1,   108,    -1,   107,   108,    -1,    79,     3,    86,   109,
      89,   115,    -1,   126,    79,     3,    48,   120,    93,    -1,
     126,    79,     3,    93,    -1,   126,     3,    48,   120,    93,
      -1,   126,     3,    93,    -1,    40,   110,    -1,    -1,   127,
       3,    -1,   109,    94,   127,     3,    -1,    88,    91,    -1,
      88,   111,    91,    -1,   112,    -1,   111,   112,    -1,   101,
      93,    -1,   101,    88,   113,    91,    93,    -1,    -1,   114,
      -1,   113,    94,   114,    -1,     3,    48,   120,    -1,    88,
      91,    -1,    88,   116,    91,    -1,   117,    -1,   116,   117,
      -1,    30,    86,   121,    89,   117,    -1,    30,    86,   121,
      89,   117,    47,   117,    -1,    35,    86,   121,    89,   117,
      -1,    36,   118,    35,    86,   121,    89,    93,    -1,    34,
      86,   123,    93,   124,    93,   123,    89,   117,    -1,    95,
     119,    79,     3,    48,   120,    93,    -1,    95,   119,    79,
       3,    86,   125,    89,    93,    -1,    38,   120,    93,    -1,
     115,    -1,   120,    93,    -1,   117,    -1,    -1,     3,    -1,
       4,    -1,    86,   120,    89,    -1,   127,     3,    -1,   127,
       3,    48,   120,    -1,   121,    -1,   120,    72,   120,    -1,
     120,    73,   120,    -1,   120,    74,   120,    -1,   120,    75,
     120,    -1,   120,    76,   120,    -1,   120,    71,   120,    -1,
     120,    70,   120,    -1,   120,    59,   120,    -1,   120,    60,
     120,    -1,   120,    61,   120,    -1,   120,    62,   120,    -1,
     120,    63,   120,    -1,     3,    48,   120,    -1,     3,    58,
     120,    -1,     3,    57,   120,    -1,     3,    56,   120,    -1,
       3,    55,   120,    -1,     3,    54,   120,    -1,     3,    53,
     120,    -1,     3,    52,   120,    -1,     3,    51,   120,    -1,
       3,    50,   120,    -1,     3,    49,   120,    -1,    77,   120,
      -1,    78,   120,    -1,    73,   120,    -1,    79,     3,    -1,
      81,   120,    -1,    80,   120,    -1,   120,    81,    -1,   120,
      80,    -1,   122,    -1,    21,    88,   125,    91,    -1,    23,
      88,   125,    91,    -1,    26,    88,   125,    91,    -1,   101,
      88,   125,    91,    -1,   101,    86,   125,    89,    -1,    96,
      83,     3,    86,   125,    89,    -1,   120,    65,   120,    -1,
     120,    64,   120,    -1,   120,    69,   120,    -1,   120,    68,
     120,    -1,   120,    66,   120,    -1,   120,    67,   120,    -1,
       3,    -1,     4,    -1,     7,    -1,     5,    -1,     6,    -1,
      -1,   120,    -1,    -1,   121,    -1,    -1,   120,    -1,   125,
      94,   120,    -1,    43,   128,    -1,   128,    -1,    43,   101,
      -1,   101,    -1,   126,    -1,    43,    29,    -1,    29,    -1,
      10,    -1,    11,    -1,    12,    -1,    13,    -1,    14,    -1,
      15,    -1,    16,    -1,    17,    -1,    18,    -1,    19,    -1,
      20,    -1,     9,    -1,    21,    -1,    22,    -1,    23,    -1,
      24,    -1,    25,    -1,    26,    -1,    27,    -1,     8,    -1,
      28,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   117,   117,   118,   122,   123,   127,   131,   135,   136,
     140,   144,   145,   150,   151,   152,   153,   154,   158,   159,
     163,   164,   168,   169,   170,   171,   172,   173,   177,   178,
     179,   183,   184,   188,   189,   193,   194,   198,   199,   200,
     204,   208,   209,   213,   214,   218,   219,   221,   222,   224,
     226,   227,   229,   231,   233,   237,   240,   241,   242,   246,
     248,   249,   251,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   278,   279,   280,   282,
     284,   285,   286,   287,   289,   291,   292,   293,   295,   296,
     298,   302,   303,   304,   305,   306,   307,   308,   312,   313,
     314,   315,   319,   320,   324,   325,   329,   330,   331,   335,
     336,   337,   338,   342,   343,   344,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "INT_LITERAL", "TRUE",
  "FALSE", "FLOAT_LITERAL", "VOID", "BOOL", "CHAR", "BYTE", "SHORT",
  "USHORT", "INT", "UINT", "LONG", "ULONG", "HALF", "FLOAT", "DOUBLE",
  "COLOR", "VEC2", "VEC3", "VEC4", "MAT3", "MAT34", "MAT4", "HANDLE_",
  "ENTITY", "IF", "SWITCH", "CASE", "DEFAULT", "FOR", "WHILE", "DO",
  "BREAK", "RETURN", "COMPONENT", "COMPONENTS", "IMPORT", "AS", "CONST",
  "THIS", "NONE", "THEN", "ELSE", "'='", "OR_ASSIGN", "XOR_ASSIGN",
  "AND_ASSIGN", "RSHIFT_ASSIGN", "LSHIFT_ASSIGN", "MOD_ASSIGN",
  "DIV_ASSIGN", "MUL_ASSIGN", "SUB_ASSIGN", "ADD_ASSIGN", "OR", "AND",
  "'|'", "'^'", "'&'", "NEQ", "EQ", "'<'", "'>'", "GTE", "LTE", "RSHIFT",
  "LSHIFT", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "'#'", "DEC",
  "INC", "UMINUS", "'.'", "POSTDEC", "POSTINC", "'('", "'['", "'{'", "')'",
  "']'", "'}'", "SCOPE", "';'", "','", "'@'", "'$'", "$accept", "cmp_file",
  "import_list", "import_stmt", "dotted_id", "dotted_id_proc",
  "dotted_id_part", "def_list", "def", "message_block", "message_list",
  "message_prop", "param_list", "component_block", "component_member_list",
  "component_member", "prop_init_list", "prop_init", "block", "stmt_list",
  "stmt", "do_stmt", "target_expr", "expr", "cond_expr", "literal",
  "expr_or_empty", "cond_expr_or_empty", "fun_params", "type", "type_ent",
  "basic_type", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,    61,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   124,    94,    38,   315,   316,    60,    62,   317,   318,
     319,   320,    43,    45,    42,    47,    37,    33,   126,    35,
     321,   322,   323,    46,   324,   325,    40,    91,   123,    41,
      93,   125,   326,    59,    44,    64,    36
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    97,    98,    98,    99,    99,   100,   101,   102,   102,
     103,   104,   104,   105,   105,   105,   105,   105,   106,   106,
     107,   107,   108,   108,   108,   108,   108,   108,   109,   109,
     109,   110,   110,   111,   111,   112,   112,   113,   113,   113,
     114,   115,   115,   116,   116,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   118,   119,   119,   119,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   121,   121,   121,   121,   121,   121,   121,   122,   122,
     122,   122,   123,   123,   124,   124,   125,   125,   125,   126,
     126,   126,   126,   127,   127,   127,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     5,     1,     1,     3,
       1,     1,     2,     3,     3,     6,     7,     6,     2,     3,
       1,     2,     6,     6,     4,     5,     3,     2,     0,     2,
       4,     2,     3,     1,     2,     2,     5,     0,     1,     3,
       3,     2,     3,     1,     2,     5,     7,     5,     7,     9,
       7,     8,     3,     1,     2,     1,     0,     1,     1,     3,
       2,     4,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     4,     4,     4,     4,     4,
       6,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     0,     1,     0,     1,     0,     1,     3,     2,
       1,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    10,   145,   137,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   138,   139,   140,   141,   142,
     143,   144,   146,     0,     0,     0,     0,     0,     0,     4,
     122,     7,     8,     2,    11,     0,   120,     0,     0,     0,
       0,   121,   119,     1,     5,     3,     0,    12,     0,    28,
       0,    13,    14,     0,     0,     9,    28,   125,     0,     0,
     123,     0,     0,     0,     0,    18,     0,    20,     0,     0,
      28,     0,   124,     0,     0,    29,     0,    27,     0,    19,
      21,     0,     0,     6,     0,     0,     0,    17,     0,    31,
       0,     0,    33,    28,     0,    26,     0,     0,    15,   107,
     108,   110,   111,   109,   138,   140,   143,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    41,
      56,     0,   122,    53,     0,    43,     0,    62,    94,     0,
      30,    37,    35,    32,    34,     0,     0,     0,    24,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,   116,   116,     0,   112,     0,    55,     0,     0,
      88,    86,    87,    89,    91,    90,     0,    57,    58,     0,
       0,   116,   116,    42,    44,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    92,    54,    60,     0,     0,    38,
       0,    25,     0,    75,    85,    84,    83,    82,    81,    80,
      79,    78,    77,    76,   117,     0,     0,     0,     0,    62,
     113,     0,    62,     0,    52,    59,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   102,   101,   105,   106,   104,
     103,    69,    68,    63,    64,    65,    66,    67,     0,     0,
       0,     0,    22,    23,    95,     0,    96,    97,     0,   114,
       0,     0,     0,   116,    99,    98,    61,    40,    36,    39,
     118,    45,    62,     0,    47,    62,     0,   116,     0,     0,
     112,     0,     0,     0,   100,    46,     0,    48,    50,     0,
       0,    51,    49
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    27,    28,    29,   122,    31,    32,    33,    34,    51,
      66,    67,    59,    77,    91,    92,   198,   199,   123,   124,
     125,   158,   169,   126,   127,   128,   221,   273,   215,    60,
     129,    36
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -152
static const yytype_int16 yypact[] =
{
     625,  -152,  -152,  -152,  -152,  -152,  -152,  -152,  -152,  -152,
    -152,  -152,  -152,  -152,  -152,  -152,  -152,  -152,  -152,  -152,
    -152,  -152,  -152,    13,    33,   104,   871,    41,   625,  -152,
      29,  -152,  -152,   664,  -152,   124,  -152,   -23,    44,    61,
     136,    29,  -152,  -152,  -152,   664,   104,  -152,    74,   223,
     541,  -152,  -152,   104,    76,  -152,   223,  -152,   898,    47,
    -152,   153,    88,   925,   181,  -152,   579,  -152,     1,   -54,
     223,    51,  -152,    97,   223,  -152,     8,  -152,   129,  -152,
    -152,   -14,   213,  -152,    69,    97,   184,  -152,   214,  -152,
     -53,     9,  -152,   223,   413,  -152,    16,    97,  -152,    18,
    -152,  -152,  -152,  -152,   133,   135,   137,   138,   142,   143,
     372,   413,   413,   413,   413,   227,   413,   413,   413,  -152,
     171,   170,    38,  -152,   278,  -152,   267,  -152,  -152,   251,
    -152,   252,  -152,  -152,  -152,    70,   649,   413,  -152,  -152,
     413,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     413,   413,   413,   413,   413,   413,   413,  -152,   221,   684,
    -152,    99,    99,  -152,    99,    99,   789,  -152,  -152,   179,
     256,   413,   413,  -152,  -152,   413,   413,   413,   413,   413,
     413,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     413,   413,   413,  -152,  -152,  -152,   212,   219,   -84,  -152,
      97,  -152,   719,   895,   895,   895,   895,   895,   895,   895,
     895,   895,   895,   895,   895,   -49,    37,    52,   895,   180,
     895,   175,   182,   187,  -152,  -152,   271,   190,    77,    59,
     917,   938,   958,   977,   995,  1013,  1013,  1029,  1029,  1029,
    1029,    42,    42,   -43,   -43,    99,    99,    99,   413,   413,
     185,   252,  -152,  -152,  -152,   413,  -152,  -152,   372,   413,
     372,   413,   -42,   413,  -152,  -152,   895,   895,  -152,  -152,
     895,   230,   216,   217,  -152,   222,   413,   413,    83,   372,
     413,   224,   754,    84,  -152,  -152,   226,  -152,  -152,   225,
     372,  -152,  -152
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -152,  -152,  -152,   291,   455,  -152,   274,   294,   -20,   285,
    -152,   258,    55,  -152,  -152,   234,  -152,    93,   -71,  -152,
    -109,  -152,  -152,   -94,  -151,  -152,    65,  -152,  -144,   102,
      64,    79
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -116
static const yytype_int16 yytable[] =
{
     136,   157,    87,   219,    81,   222,   276,   250,   216,   217,
     251,     1,     1,    47,    98,   174,    37,   159,   160,   161,
     162,   -10,   164,   165,   166,    47,   139,   228,   229,    46,
      46,   190,   191,   192,    94,   131,    38,   193,   194,    83,
     132,    43,   254,   202,   277,   255,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   214,   214,
     218,   220,   218,    49,   137,    50,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   214,   214,    95,
      82,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,    89,
     133,   -10,    35,    53,   -10,    42,   -10,     1,   272,   138,
     275,    71,    46,    61,   188,   189,   190,   191,   192,   278,
      61,    46,   193,   194,   171,    84,   172,    48,   256,   252,
      35,   255,    50,   283,    61,    35,    73,    42,    88,    54,
      85,    74,    42,   257,    46,    74,   255,    35,   135,   271,
     265,   274,    68,   255,   266,   267,    75,    61,    97,   200,
      56,   270,    70,    74,    74,   218,   264,   218,    68,   214,
     285,   255,   284,   289,   167,   168,    76,   255,   255,   193,
     194,   292,   282,   214,    78,    86,   220,    99,   100,   101,
     102,   103,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,   104,    16,   105,    18,    19,
     106,    21,    22,    57,   107,    93,    96,   130,   108,   109,
     110,   151,   111,   152,   154,   153,     1,    58,   155,   156,
     163,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    57,   170,   196,   197,   223,   112,   226,   227,
     248,   113,   114,   115,   116,   117,    58,   249,   259,   258,
     118,   260,    86,   261,   262,   119,   263,   279,   268,   120,
     121,    99,   100,   101,   102,   103,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,   104,
      16,   105,    18,    19,   106,    21,    22,    57,   107,  -115,
     280,   281,   108,   109,   110,   290,   111,   287,   291,    44,
      55,    58,    45,    52,    80,   134,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   269,   286,     0,   193,   194,     0,
       0,   112,     0,     0,     0,   113,   114,   115,   116,   117,
     195,     0,     0,     0,   118,     0,    86,     0,     0,   173,
       0,     0,     0,   120,   121,    99,   100,   101,   102,   103,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,   104,    16,   105,    18,    19,   106,    21,
      22,    57,   107,     0,     0,     0,   108,   109,   110,     0,
     111,     0,     0,     0,     0,    58,    99,   100,   101,   102,
     103,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,   104,    16,   105,    18,    19,   106,
      21,    22,    57,     0,     0,   112,     0,     0,     0,   113,
     114,   115,   116,   117,     0,    30,    58,     0,   118,     0,
      86,     0,     0,     0,     0,     0,     0,   120,   121,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      39,    41,     0,    30,     0,     0,   112,     0,    30,     0,
     113,   114,   115,   116,   117,     0,     0,     0,     0,   118,
      30,     0,     0,     0,    30,    30,     0,     0,    69,   121,
       0,    30,     0,    41,     0,     0,     0,     0,    41,     0,
       0,    30,     0,     0,     0,    30,     0,     0,     0,    30,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     1,     0,    90,     0,    30,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     1,     0,    63,     0,     0,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      64,     0,    63,     0,     0,     0,     0,     0,     1,     0,
       0,     0,    65,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     0,     0,     0,    64,     0,
       0,     0,     0,     0,    24,     0,    25,     1,    26,     0,
      79,     0,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,     0,     0,     0,    26,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,     0,     0,   193,
     194,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   201,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,     0,     0,   193,   194,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,     0,     0,   193,
     194,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   253,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,     0,     0,   193,   194,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   288,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,     0,     0,   193,
     194,     0,     0,     0,     1,     0,     0,     0,   225,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      40,     1,     0,     0,     0,     0,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    72,     1,     0,
       0,     0,     0,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,     0,     0,   193,   194,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,     0,     0,   193,   194,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,     0,     0,   193,   194,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,     0,     0,   193,   194,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,     0,     0,   193,   194,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,     0,     0,   193,   194,  -116,  -116,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,     0,     0,   193,   194,  -116,  -116,  -116,  -116,   186,
     187,   188,   189,   190,   191,   192,     0,     0,     0,   193,
     194
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-152)))

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-116)))

static const yytype_int16 yycheck[] =
{
      94,   110,    73,   154,     3,   156,    48,    91,   152,   153,
      94,     3,     3,    33,    85,   124,     3,   111,   112,   113,
     114,     3,   116,   117,   118,    45,    97,   171,   172,    83,
      83,    74,    75,    76,    48,    88,     3,    80,    81,    93,
      93,     0,    91,   137,    86,    94,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,    86,    48,    88,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,   171,   172,    93,
      79,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,    91,
      91,    83,     0,    42,    86,    26,    88,     3,   259,    93,
     261,    56,    83,    49,    72,    73,    74,    75,    76,   263,
      56,    83,    80,    81,    86,    70,    88,     3,    91,   200,
      28,    94,    88,   277,    70,    33,    89,    58,    74,     3,
      89,    94,    63,    91,    83,    94,    94,    45,    93,   258,
      91,   260,    50,    94,   248,   249,     3,    93,    89,    89,
      86,   255,    86,    94,    94,   259,    89,   261,    66,   263,
     279,    94,    89,    89,     3,     4,    88,    94,    94,    80,
      81,   290,   276,   277,     3,    88,   280,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    86,     3,     3,    34,    35,
      36,    88,    38,    88,    86,    88,     3,    43,    86,    86,
       3,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    83,     3,     3,    35,    73,    79,     3,
      48,    77,    78,    79,    80,    81,    43,    48,    93,    89,
      86,    89,    88,    86,     3,    91,    86,    47,    93,    95,
      96,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    93,
      93,    89,    34,    35,    36,    89,    38,    93,    93,    28,
      46,    43,    28,    38,    66,    91,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,   251,   280,    -1,    80,    81,    -1,
      -1,    73,    -1,    -1,    -1,    77,    78,    79,    80,    81,
      93,    -1,    -1,    -1,    86,    -1,    88,    -1,    -1,    91,
      -1,    -1,    -1,    95,    96,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    -1,    -1,    34,    35,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    73,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    -1,     0,    43,    -1,    86,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    28,    -1,    -1,    73,    -1,    33,    -1,
      77,    78,    79,    80,    81,    -1,    -1,    -1,    -1,    86,
      45,    -1,    -1,    -1,    49,    50,    -1,    -1,    53,    96,
      -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    -1,    -1,    -1,    70,    -1,    -1,    -1,    74,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    91,    -1,    93,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,     3,    -1,    43,    -1,    -1,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      79,    -1,    43,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    91,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    41,     3,    43,    -1,
      91,    -1,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    43,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,     3,    -1,    -1,    -1,    89,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,     3,    -1,    -1,    -1,    -1,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,     3,    -1,
      -1,    -1,    -1,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    80,    81,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    80,    81,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    80,    81,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    80,    81,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    80,    81,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    80,    81,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    80,    81,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    -1,    -1,    80,
      81
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    39,    41,    43,    98,    99,   100,
     101,   102,   103,   104,   105,   126,   128,     3,     3,   101,
      29,   101,   128,     0,   100,   104,    83,   105,     3,    86,
      88,   106,   106,    42,     3,   103,    86,    29,    43,   109,
     126,   127,    40,    43,    79,    91,   107,   108,   126,   101,
      86,   109,    29,    89,    94,     3,    88,   110,     3,    91,
     108,     3,    79,    93,   109,    89,    88,   115,   127,    91,
     101,   111,   112,    86,    48,    93,     3,    89,   115,     3,
       4,     5,     6,     7,    21,    23,    26,    30,    34,    35,
      36,    38,    73,    77,    78,    79,    80,    81,    86,    91,
      95,    96,   101,   115,   116,   117,   120,   121,   122,   127,
       3,    88,    93,    91,   112,   109,   120,    48,    93,   115,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    88,    88,    88,    86,    86,    86,   117,   118,   120,
     120,   120,   120,     3,   120,   120,   120,     3,     4,   119,
      83,    86,    88,    91,   117,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    80,    81,    93,     3,     3,   113,   114,
      89,    93,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   125,   125,   125,   120,   121,
     120,   123,   121,    35,    93,    89,    79,     3,   125,   125,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,    48,    48,
      91,    94,   115,    93,    91,    94,    91,    91,    89,    93,
      89,    86,     3,    86,    89,    91,   120,   120,    93,   114,
     120,   117,   121,   124,   117,   121,    48,    86,   125,    47,
      93,    89,   120,   125,    89,   117,   123,    93,    93,    89,
      89,    93,   117
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (&yylloc, pParseData, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (YYID (N))                                                     \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (YYID (0))
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

__attribute__((__unused__))
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
#else
static unsigned
yy_location_print_ (yyo, yylocp)
    FILE *yyo;
    YYLTYPE const * const yylocp;
#endif
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += fprintf (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += fprintf (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += fprintf (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += fprintf (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += fprintf (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, pParseData)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, pParseData); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParseData * pParseData)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, pParseData)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    ParseData * pParseData;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (pParseData);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParseData * pParseData)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, pParseData)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    ParseData * pParseData;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, pParseData);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, ParseData * pParseData)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, pParseData)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    ParseData * pParseData;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , pParseData);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, pParseData); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ParseData * pParseData)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, pParseData)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    ParseData * pParseData;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (pParseData);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YYUSE (yytype);
}




/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (ParseData * pParseData)
#else
int
yyparse (pParseData)
    ParseData * pParseData;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;


#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
static YYSTYPE yyval_default;
# define YY_INITIAL_VALUE(Value) = Value
#endif
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc = yyloc_default;


    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
/* Line 1787 of yacc.c  */
#line 118 "compose.y"
    { ast_create_import_list((yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 4:
/* Line 1787 of yacc.c  */
#line 122 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ImportList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 5:
/* Line 1787 of yacc.c  */
#line 123 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ImportList, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 6:
/* Line 1787 of yacc.c  */
#line 127 "compose.y"
    { (yyval.pAst) = ast_create_import_stmt((yyvsp[(2) - (5)].pAst), (yyvsp[(4) - (5)].pAst), pParseData); }
    break;

  case 7:
/* Line 1787 of yacc.c  */
#line 131 "compose.y"
    { (yyval.pAst) = ast_create_dotted_id((yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 8:
/* Line 1787 of yacc.c  */
#line 135 "compose.y"
    { (yyval.pAst) = ast_append(kAST_DottedId, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 9:
/* Line 1787 of yacc.c  */
#line 136 "compose.y"
    { (yyval.pAst) = ast_append(kAST_DottedId, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 10:
/* Line 1787 of yacc.c  */
#line 140 "compose.y"
    { (yyval.pAst) = ast_create_with_str(kAST_DottedIdPart, kASTF_None, (yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 150 "compose.y"
    { (yyval.pAst) = ast_create_entity_def((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 151 "compose.y"
    { (yyval.pAst) = ast_create_component_def((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 152 "compose.y"
    { (yyval.pAst) = ast_create_function_def((yyvsp[(2) - (6)].str), (yyvsp[(1) - (6)].pAst), (yyvsp[(6) - (6)].pAst), pParseData); }
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 153 "compose.y"
    { (yyval.pAst) = ast_create_function_def((yyvsp[(3) - (7)].str), ast_create_with_numi(kAST_DataType, kASTF_Const,CONST_DT((yyvsp[(2) - (7)].dataType)), pParseData), (yyvsp[(7) - (7)].pAst), pParseData); }
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 154 "compose.y"
    { (yyval.pAst) = ast_create_function_def((yyvsp[(2) - (6)].str), ast_create_with_numi(kAST_DataType, kASTF_None, (yyvsp[(1) - (6)].dataType), pParseData), (yyvsp[(6) - (6)].pAst), pParseData); }
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 158 "compose.y"
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 159 "compose.y"
    { (yyval.pAst) = ast_create_block((yyvsp[(2) - (3)].pAst),   pParseData); }
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 163 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 164 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 168 "compose.y"
    { (yyval.pAst) = ast_create_message_def((yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].pSymTab), (yyvsp[(6) - (6)].pAst), pParseData); }
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 169 "compose.y"
    { (yyval.pAst) = ast_create_property_def((yyvsp[(3) - (6)].str), (yyvsp[(1) - (6)].pAst), (yyvsp[(5) - (6)].pAst), pParseData); }
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 170 "compose.y"
    { (yyval.pAst) = ast_create_property_def((yyvsp[(3) - (4)].str), (yyvsp[(1) - (4)].pAst), NULL, pParseData); }
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 171 "compose.y"
    { (yyval.pAst) = ast_create_field_def((yyvsp[(2) - (5)].str), (yyvsp[(1) - (5)].pAst), (yyvsp[(4) - (5)].pAst), pParseData); }
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 172 "compose.y"
    { (yyval.pAst) = ast_create_field_def((yyvsp[(2) - (3)].str), (yyvsp[(1) - (3)].pAst), NULL, pParseData); }
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 173 "compose.y"
    { (yyval.pAst) = ast_create_component_members((yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 177 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, NULL); }
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 178 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, symrec_create(kSYMT_Param, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].str), NULL, pParseData)); }
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 179 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, (yyvsp[(1) - (4)].pSymTab), symrec_create(kSYMT_Param, (yyvsp[(3) - (4)].pAst), (yyvsp[(4) - (4)].str), NULL, pParseData)); }
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 183 "compose.y"
    { (yyval.pAst) = ast_create_with_child_list(kAST_ComponentMemberList, pParseData); }
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 184 "compose.y"
    { (yyval.pAst) = (yyvsp[(2) - (3)].pAst); }
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 188 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 34:
/* Line 1787 of yacc.c  */
#line 189 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 193 "compose.y"
    { (yyval.pAst) = ast_create_component_member((yyvsp[(1) - (2)].pAst), ast_create(kAST_PropInit, pParseData), pParseData); }
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 194 "compose.y"
    { (yyval.pAst) = ast_create_component_member((yyvsp[(1) - (5)].pAst), (yyvsp[(3) - (5)].pAst), pParseData); }
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 198 "compose.y"
    { (yyval.pAst) = ast_create(kAST_PropInitList, pParseData); }
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 199 "compose.y"
    { (yyval.pAst) = ast_append(kAST_PropInitList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 200 "compose.y"
    { (yyval.pAst) = ast_append(kAST_PropInitList, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 204 "compose.y"
    { (yyval.pAst) = ast_create_prop_init((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 208 "compose.y"
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 209 "compose.y"
    { (yyval.pAst) = ast_create_block((yyvsp[(2) - (3)].pAst),   pParseData); }
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 213 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 214 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 45:
/* Line 1787 of yacc.c  */
#line 218 "compose.y"
    { (yyval.pAst) = ast_create_if((yyvsp[(3) - (5)].pAst), (yyvsp[(5) - (5)].pAst), NULL, pParseData); }
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 219 "compose.y"
    { (yyval.pAst) = ast_create_if((yyvsp[(3) - (7)].pAst), (yyvsp[(5) - (7)].pAst), (yyvsp[(7) - (7)].pAst),   pParseData); }
    break;

  case 47:
/* Line 1787 of yacc.c  */
#line 221 "compose.y"
    { (yyval.pAst) = ast_create_while((yyvsp[(3) - (5)].pAst), (yyvsp[(5) - (5)].pAst), pParseData); }
    break;

  case 48:
/* Line 1787 of yacc.c  */
#line 222 "compose.y"
    { (yyval.pAst) = ast_create_dowhile((yyvsp[(5) - (7)].pAst), (yyvsp[(2) - (7)].pAst), pParseData); }
    break;

  case 49:
/* Line 1787 of yacc.c  */
#line 224 "compose.y"
    { (yyval.pAst) = ast_create_for((yyvsp[(3) - (9)].pAst), (yyvsp[(5) - (9)].pAst), (yyvsp[(7) - (9)].pAst), (yyvsp[(9) - (9)].pAst), pParseData); }
    break;

  case 50:
/* Line 1787 of yacc.c  */
#line 226 "compose.y"
    { (yyval.pAst) = ast_create_property_set((yyvsp[(2) - (7)].pAst), (yyvsp[(4) - (7)].str), (yyvsp[(6) - (7)].pAst), pParseData); }
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 227 "compose.y"
    { (yyval.pAst) = ast_create_message_send((yyvsp[(2) - (8)].pAst), (yyvsp[(4) - (8)].str), (yyvsp[(6) - (8)].pAst), pParseData); }
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 229 "compose.y"
    { (yyval.pAst) = ast_create_return((yyvsp[(2) - (3)].pAst), pParseData); }
    break;

  case 53:
/* Line 1787 of yacc.c  */
#line 231 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 54:
/* Line 1787 of yacc.c  */
#line 233 "compose.y"
    { (yyval.pAst) = ast_create_simple_stmt((yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 55:
/* Line 1787 of yacc.c  */
#line 237 "compose.y"
    { parsedata_handle_do_scope(pParseData); (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 56:
/* Line 1787 of yacc.c  */
#line 240 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 241 "compose.y"
    { (yyval.pAst) = ast_create_identifier((yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 58:
/* Line 1787 of yacc.c  */
#line 242 "compose.y"
    { (yyval.pAst) = ast_create_int_literal((yyvsp[(1) - (1)].numi), pParseData); }
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 246 "compose.y"
    { (yyval.pAst) = (yyvsp[(2) - (3)].pAst); }
    break;

  case 60:
/* Line 1787 of yacc.c  */
#line 248 "compose.y"
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].str), NULL, pParseData)); }
    break;

  case 61:
/* Line 1787 of yacc.c  */
#line 249 "compose.y"
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[(1) - (4)].pAst), (yyvsp[(2) - (4)].str), (yyvsp[(4) - (4)].pAst), pParseData)); }
    break;

  case 62:
/* Line 1787 of yacc.c  */
#line 251 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 63:
/* Line 1787 of yacc.c  */
#line 253 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Add,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 64:
/* Line 1787 of yacc.c  */
#line 254 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Sub,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 65:
/* Line 1787 of yacc.c  */
#line 255 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Mul,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 66:
/* Line 1787 of yacc.c  */
#line 256 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Div,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 67:
/* Line 1787 of yacc.c  */
#line 257 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Mod,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 68:
/* Line 1787 of yacc.c  */
#line 258 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LShift, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 69:
/* Line 1787 of yacc.c  */
#line 259 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_RShift, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 70:
/* Line 1787 of yacc.c  */
#line 260 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Or,     (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 71:
/* Line 1787 of yacc.c  */
#line 261 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_And,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 72:
/* Line 1787 of yacc.c  */
#line 262 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitOr,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 73:
/* Line 1787 of yacc.c  */
#line 263 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitXor, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 74:
/* Line 1787 of yacc.c  */
#line 264 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitAnd, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 75:
/* Line 1787 of yacc.c  */
#line 266 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_Assign,       (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 76:
/* Line 1787 of yacc.c  */
#line 267 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_AddAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 77:
/* Line 1787 of yacc.c  */
#line 268 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_SubAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 78:
/* Line 1787 of yacc.c  */
#line 269 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_MulAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 79:
/* Line 1787 of yacc.c  */
#line 270 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_DivAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 80:
/* Line 1787 of yacc.c  */
#line 271 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_ModAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 81:
/* Line 1787 of yacc.c  */
#line 272 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_LShiftAssign, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 82:
/* Line 1787 of yacc.c  */
#line 273 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_RShiftAssign, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 83:
/* Line 1787 of yacc.c  */
#line 274 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_AndAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 84:
/* Line 1787 of yacc.c  */
#line 275 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_XorAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 85:
/* Line 1787 of yacc.c  */
#line 276 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_OrAssign,     (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 86:
/* Line 1787 of yacc.c  */
#line 278 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Not,        (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 87:
/* Line 1787 of yacc.c  */
#line 279 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Complement, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 88:
/* Line 1787 of yacc.c  */
#line 280 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Negate,     (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 89:
/* Line 1787 of yacc.c  */
#line 282 "compose.y"
    { (yyval.pAst) = ast_create_hash((yyvsp[(2) - (2)].str), pParseData); }
    break;

  case 90:
/* Line 1787 of yacc.c  */
#line 284 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PreInc, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 91:
/* Line 1787 of yacc.c  */
#line 285 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PreDec, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 92:
/* Line 1787 of yacc.c  */
#line 286 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PostInc, (yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 93:
/* Line 1787 of yacc.c  */
#line 287 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PostDec, (yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 94:
/* Line 1787 of yacc.c  */
#line 289 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 95:
/* Line 1787 of yacc.c  */
#line 291 "compose.y"
    { (yyval.pAst) = ast_create_color_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 96:
/* Line 1787 of yacc.c  */
#line 292 "compose.y"
    { (yyval.pAst) = ast_create_vec3_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 97:
/* Line 1787 of yacc.c  */
#line 293 "compose.y"
    { (yyval.pAst) = ast_create_mat34_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 98:
/* Line 1787 of yacc.c  */
#line 295 "compose.y"
    { (yyval.pAst) = ast_create_entity_or_struct_init((yyvsp[(1) - (4)].pAst), (yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 99:
/* Line 1787 of yacc.c  */
#line 296 "compose.y"
    { (yyval.pAst) = ast_create_function_call((yyvsp[(1) - (4)].pAst), (yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 100:
/* Line 1787 of yacc.c  */
#line 298 "compose.y"
    { (yyval.pAst) = ast_create_system_api_call((yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].pAst), pParseData); }
    break;

  case 101:
/* Line 1787 of yacc.c  */
#line 302 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Eq,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 102:
/* Line 1787 of yacc.c  */
#line 303 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_NEq, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 103:
/* Line 1787 of yacc.c  */
#line 304 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LTE, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 104:
/* Line 1787 of yacc.c  */
#line 305 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_GTE, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 105:
/* Line 1787 of yacc.c  */
#line 306 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LT,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 106:
/* Line 1787 of yacc.c  */
#line 307 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_GT,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 107:
/* Line 1787 of yacc.c  */
#line 308 "compose.y"
    { (yyval.pAst) = ast_create_symbol_ref((yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 108:
/* Line 1787 of yacc.c  */
#line 312 "compose.y"
    { (yyval.pAst) = ast_create_int_literal((yyvsp[(1) - (1)].numi), pParseData); }
    break;

  case 109:
/* Line 1787 of yacc.c  */
#line 313 "compose.y"
    { (yyval.pAst) = ast_create_float_literal((yyvsp[(1) - (1)].numf), pParseData); }
    break;

  case 110:
/* Line 1787 of yacc.c  */
#line 314 "compose.y"
    { (yyval.pAst) = ast_create_int_literal(1, pParseData); }
    break;

  case 111:
/* Line 1787 of yacc.c  */
#line 315 "compose.y"
    { (yyval.pAst) = ast_create_int_literal(0, pParseData); }
    break;

  case 112:
/* Line 1787 of yacc.c  */
#line 319 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 113:
/* Line 1787 of yacc.c  */
#line 320 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 114:
/* Line 1787 of yacc.c  */
#line 324 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 115:
/* Line 1787 of yacc.c  */
#line 325 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 116:
/* Line 1787 of yacc.c  */
#line 329 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, NULL, pParseData); }
    break;

  case 117:
/* Line 1787 of yacc.c  */
#line 330 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, (yyvsp[(1) - (1)].pAst),   pParseData); }
    break;

  case 118:
/* Line 1787 of yacc.c  */
#line 331 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, (yyvsp[(1) - (3)].pAst),   (yyvsp[(3) - (3)].pAst),   pParseData); }
    break;

  case 119:
/* Line 1787 of yacc.c  */
#line 335 "compose.y"
    { (yyval.pAst) = ast_create_with_numi(kAST_DataType, kASTF_Const,CONST_DT((yyvsp[(2) - (2)].dataType)), pParseData); }
    break;

  case 120:
/* Line 1787 of yacc.c  */
#line 336 "compose.y"
    { (yyval.pAst) = ast_create_with_numi(kAST_DataType, kASTF_None, (yyvsp[(1) - (1)].dataType), pParseData); }
    break;

  case 121:
/* Line 1787 of yacc.c  */
#line 337 "compose.y"
    { (yyval.pAst) = ast_create_custom_type(kASTF_Const, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 122:
/* Line 1787 of yacc.c  */
#line 338 "compose.y"
    { (yyval.pAst) = ast_create_custom_type(kASTF_None, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 123:
/* Line 1787 of yacc.c  */
#line 342 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 124:
/* Line 1787 of yacc.c  */
#line 343 "compose.y"
    { (yyval.pAst) = ast_create_with_numi(kAST_DataType, kASTF_Const,CONST_DT((yyvsp[(2) - (2)].dataType)), pParseData); }
    break;

  case 125:
/* Line 1787 of yacc.c  */
#line 344 "compose.y"
    { (yyval.pAst) = ast_create_with_numi(kAST_DataType, kASTF_None, (yyvsp[(1) - (1)].dataType), pParseData); }
    break;


/* Line 1787 of yacc.c  */
#line 2737 "compose_parser.c"
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, pParseData, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, pParseData, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
		      yytoken, &yylval, &yylloc, pParseData);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, pParseData);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, pParseData, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, pParseData);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, pParseData);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2050 of yacc.c  */
#line 370 "compose.y"



static void yyprint (FILE * file, int type, YYSTYPE value)
{
    if (type < 128)
    {
        fprintf(file, "'%c'", type);
    }
    else
    {
        switch(type)
        {
        case IDENTIFIER:
            fprintf(file, "\"%s\"", value.str);
            break;
        case INT_LITERAL:
            fprintf(file, "%d", value.numi);
            break;
        case FLOAT_LITERAL:
            fprintf(file, "%f", value.numf);
            break;
        default:
            fprintf(file, "** yyprint unknown: %d", type);
            break;
        }
    }
}

