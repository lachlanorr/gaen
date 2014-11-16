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
     HASH = 259,
     STRING_LITERAL = 260,
     INT_LITERAL = 261,
     TRUE = 262,
     FALSE = 263,
     FLOAT_LITERAL = 264,
     VOID = 265,
     BOOL = 266,
     CHAR = 267,
     BYTE = 268,
     SHORT = 269,
     USHORT = 270,
     INT = 271,
     UINT = 272,
     LONG = 273,
     ULONG = 274,
     HALF = 275,
     FLOAT = 276,
     DOUBLE = 277,
     COLOR = 278,
     VEC2 = 279,
     VEC3 = 280,
     VEC4 = 281,
     MAT3 = 282,
     MAT34 = 283,
     MAT4 = 284,
     HANDLE_ = 285,
     ENTITY = 286,
     STRING = 287,
     IF = 288,
     SWITCH = 289,
     CASE = 290,
     DEFAULT = 291,
     FOR = 292,
     WHILE = 293,
     DO = 294,
     BREAK = 295,
     RETURN = 296,
     COMPONENT = 297,
     COMPONENTS = 298,
     USING = 299,
     AS = 300,
     CONST = 301,
     THIS = 302,
     NONE = 303,
     THEN = 304,
     ELSE = 305,
     TRANSFORM = 306,
     OR_ASSIGN = 307,
     XOR_ASSIGN = 308,
     AND_ASSIGN = 309,
     RSHIFT_ASSIGN = 310,
     LSHIFT_ASSIGN = 311,
     MOD_ASSIGN = 312,
     DIV_ASSIGN = 313,
     MUL_ASSIGN = 314,
     SUB_ASSIGN = 315,
     ADD_ASSIGN = 316,
     OR = 317,
     AND = 318,
     NEQ = 319,
     EQ = 320,
     GTE = 321,
     LTE = 322,
     RSHIFT = 323,
     LSHIFT = 324,
     DEC = 325,
     INC = 326,
     UMINUS = 327,
     POSTDEC = 328,
     POSTINC = 329
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
#line 206 "compose_parser.c"
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
#line 255 "compose_parser.c"

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
#define YYFINAL  44
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1170

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  99
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  150
/* YYNRULES -- Number of states.  */
#define YYNSTATES  296

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   329

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    81,     2,     2,    98,    80,    67,     2,
      89,    92,    78,    76,    96,    77,    86,    79,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    95,
      70,    51,    71,     2,    97,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    90,     2,    93,    66,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    91,    65,    94,    82,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    68,
      69,    72,    73,    74,    75,    83,    84,    85,    87,    88
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    13,    19,    21,    23,
      27,    29,    31,    34,    38,    42,    49,    57,    64,    67,
      71,    73,    76,    82,    88,    92,    98,   102,   105,   106,
     109,   114,   117,   121,   123,   126,   129,   135,   136,   138,
     142,   146,   149,   153,   155,   158,   164,   172,   178,   186,
     196,   203,   211,   215,   217,   220,   222,   223,   225,   227,
     231,   234,   239,   241,   243,   247,   251,   255,   259,   263,
     267,   271,   275,   279,   283,   287,   291,   295,   299,   303,
     307,   311,   315,   319,   323,   327,   331,   335,   338,   341,
     344,   346,   349,   352,   355,   358,   360,   365,   370,   375,
     380,   385,   390,   397,   399,   403,   407,   411,   415,   419,
     423,   425,   427,   429,   431,   433,   434,   436,   437,   439,
     440,   442,   446,   449,   451,   454,   456,   458,   461,   463,
     465,   467,   469,   471,   473,   475,   477,   479,   481,   483,
     485,   487,   489,   491,   493,   495,   497,   499,   501,   503,
     505
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     100,     0,    -1,   106,    -1,   101,   106,    -1,   102,    -1,
     101,   102,    -1,    44,   103,    45,   103,    95,    -1,   104,
      -1,   105,    -1,   103,    86,   105,    -1,     3,    -1,   107,
      -1,   106,   107,    -1,    31,     3,   108,    -1,    42,     3,
     108,    -1,   128,     3,    89,   111,    92,   117,    -1,    46,
      31,     3,    89,   111,    92,   117,    -1,    31,     3,    89,
     111,    92,   117,    -1,    91,    94,    -1,    91,   109,    94,
      -1,   110,    -1,   109,   110,    -1,     4,    89,   111,    92,
     117,    -1,   128,     4,    51,   122,    95,    -1,   128,     4,
      95,    -1,   128,     3,    51,   122,    95,    -1,   128,     3,
      95,    -1,    43,   112,    -1,    -1,   129,     3,    -1,   111,
      96,   129,     3,    -1,    91,    94,    -1,    91,   113,    94,
      -1,   114,    -1,   113,   114,    -1,   103,    95,    -1,   103,
      91,   115,    94,    95,    -1,    -1,   116,    -1,   115,    96,
     116,    -1,     3,    51,   122,    -1,    91,    94,    -1,    91,
     118,    94,    -1,   119,    -1,   118,   119,    -1,    33,    89,
     122,    92,   119,    -1,    33,    89,   122,    92,   119,    50,
     119,    -1,    38,    89,   122,    92,   119,    -1,    39,   120,
      38,    89,   122,    92,    95,    -1,    37,    89,   125,    95,
     126,    95,   125,    92,   119,    -1,    97,   121,     4,    51,
     122,    95,    -1,    97,   121,     4,    89,   127,    92,    95,
      -1,    41,   122,    95,    -1,   117,    -1,   122,    95,    -1,
     119,    -1,    -1,     3,    -1,     6,    -1,    89,   122,    92,
      -1,   129,     3,    -1,   129,     3,    51,   122,    -1,     5,
      -1,   123,    -1,   122,    76,   122,    -1,   122,    77,   122,
      -1,   122,    78,   122,    -1,   122,    79,   122,    -1,   122,
      80,   122,    -1,   122,    75,   122,    -1,   122,    74,   122,
      -1,   122,    63,   122,    -1,   122,    64,   122,    -1,   122,
      65,   122,    -1,   122,    66,   122,    -1,   122,    67,   122,
      -1,     3,    51,   122,    -1,     3,    62,   122,    -1,     3,
      61,   122,    -1,     3,    60,   122,    -1,     3,    59,   122,
      -1,     3,    58,   122,    -1,     3,    57,   122,    -1,     3,
      56,   122,    -1,     3,    55,   122,    -1,     3,    54,   122,
      -1,     3,    53,   122,    -1,    81,   122,    -1,    82,   122,
      -1,    77,   122,    -1,     4,    -1,    84,   122,    -1,    83,
     122,    -1,   122,    84,    -1,   122,    83,    -1,   124,    -1,
      23,    91,   127,    94,    -1,    25,    91,   127,    94,    -1,
      28,    91,   127,    94,    -1,    32,    91,   127,    94,    -1,
     103,    91,   127,    94,    -1,   103,    89,   127,    92,    -1,
      98,    86,     3,    89,   127,    92,    -1,    52,    -1,   122,
      69,   122,    -1,   122,    68,   122,    -1,   122,    73,   122,
      -1,   122,    72,   122,    -1,   122,    70,   122,    -1,   122,
      71,   122,    -1,     3,    -1,     6,    -1,     9,    -1,     7,
      -1,     8,    -1,    -1,   122,    -1,    -1,   123,    -1,    -1,
     122,    -1,   127,    96,   122,    -1,    46,   130,    -1,   130,
      -1,    46,   103,    -1,   103,    -1,   128,    -1,    46,    31,
      -1,    31,    -1,    12,    -1,    13,    -1,    14,    -1,    15,
      -1,    16,    -1,    17,    -1,    18,    -1,    19,    -1,    20,
      -1,    21,    -1,    22,    -1,    11,    -1,    23,    -1,    24,
      -1,    25,    -1,    26,    -1,    27,    -1,    28,    -1,    29,
      -1,    10,    -1,    30,    -1,    32,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   115,   115,   116,   120,   121,   125,   129,   133,   134,
     138,   142,   143,   148,   149,   150,   151,   152,   156,   157,
     161,   162,   166,   167,   168,   169,   170,   171,   175,   176,
     177,   181,   182,   186,   187,   191,   192,   196,   197,   198,
     202,   206,   207,   211,   212,   216,   217,   219,   220,   222,
     224,   225,   227,   229,   231,   235,   238,   239,   240,   244,
     246,   247,   249,   251,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   278,   279,   280,
     282,   284,   285,   286,   287,   289,   291,   292,   293,   294,
     296,   297,   299,   301,   305,   306,   307,   308,   309,   310,
     311,   315,   316,   317,   318,   322,   323,   327,   328,   332,
     333,   334,   338,   339,   340,   341,   345,   346,   347,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "HASH", "STRING_LITERAL",
  "INT_LITERAL", "TRUE", "FALSE", "FLOAT_LITERAL", "VOID", "BOOL", "CHAR",
  "BYTE", "SHORT", "USHORT", "INT", "UINT", "LONG", "ULONG", "HALF",
  "FLOAT", "DOUBLE", "COLOR", "VEC2", "VEC3", "VEC4", "MAT3", "MAT34",
  "MAT4", "HANDLE_", "ENTITY", "STRING", "IF", "SWITCH", "CASE", "DEFAULT",
  "FOR", "WHILE", "DO", "BREAK", "RETURN", "COMPONENT", "COMPONENTS",
  "USING", "AS", "CONST", "THIS", "NONE", "THEN", "ELSE", "'='",
  "TRANSFORM", "OR_ASSIGN", "XOR_ASSIGN", "AND_ASSIGN", "RSHIFT_ASSIGN",
  "LSHIFT_ASSIGN", "MOD_ASSIGN", "DIV_ASSIGN", "MUL_ASSIGN", "SUB_ASSIGN",
  "ADD_ASSIGN", "OR", "AND", "'|'", "'^'", "'&'", "NEQ", "EQ", "'<'",
  "'>'", "GTE", "LTE", "RSHIFT", "LSHIFT", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'!'", "'~'", "DEC", "INC", "UMINUS", "'.'", "POSTDEC", "POSTINC",
  "'('", "'['", "'{'", "')'", "']'", "'}'", "';'", "','", "'@'", "'$'",
  "$accept", "cmp_file", "using_list", "using_stmt", "dotted_id",
  "dotted_id_proc", "dotted_id_part", "def_list", "def", "message_block",
  "message_list", "message_prop", "param_list", "component_block",
  "component_member_list", "component_member", "prop_init_list",
  "prop_init", "block", "stmt_list", "stmt", "do_stmt", "target_expr",
  "expr", "cond_expr", "literal", "expr_or_empty", "cond_expr_or_empty",
  "fun_params", "type", "type_ent", "basic_type", YY_NULL
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,    61,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   124,    94,    38,   319,   320,
      60,    62,   321,   322,   323,   324,    43,    45,    42,    47,
      37,    33,   126,   325,   326,   327,    46,   328,   329,    40,
      91,   123,    41,    93,   125,    59,    44,    64,    36
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    99,   100,   100,   101,   101,   102,   103,   104,   104,
     105,   106,   106,   107,   107,   107,   107,   107,   108,   108,
     109,   109,   110,   110,   110,   110,   110,   110,   111,   111,
     111,   112,   112,   113,   113,   114,   114,   115,   115,   115,
     116,   117,   117,   118,   118,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   120,   121,   121,   121,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   123,   123,   123,   123,   123,   123,
     123,   124,   124,   124,   124,   125,   125,   126,   126,   127,
     127,   127,   128,   128,   128,   128,   129,   129,   129,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     5,     1,     1,     3,
       1,     1,     2,     3,     3,     6,     7,     6,     2,     3,
       1,     2,     5,     5,     3,     5,     3,     2,     0,     2,
       4,     2,     3,     1,     2,     2,     5,     0,     1,     3,
       3,     2,     3,     1,     2,     5,     7,     5,     7,     9,
       6,     7,     3,     1,     2,     1,     0,     1,     1,     3,
       2,     4,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       1,     2,     2,     2,     2,     1,     4,     4,     4,     4,
       4,     4,     6,     1,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     0,     1,     0,     1,     0,
       1,     3,     2,     1,     2,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    10,   148,   140,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   141,   142,   143,   144,   145,
     146,   147,   149,     0,   150,     0,     0,     0,     0,     0,
       4,   125,     7,     8,     2,    11,     0,   123,     0,     0,
       0,     0,   124,   122,     1,     5,     3,     0,    12,     0,
      28,     0,    13,    14,     0,     0,     9,    28,   128,     0,
       0,   126,     0,     0,     0,     0,    18,     0,    20,     0,
       0,    28,     0,   127,     0,     0,    29,    28,     0,    27,
      19,    21,     0,     0,     6,     0,     0,     0,    17,     0,
       0,    31,     0,     0,    33,     0,    26,     0,    24,     0,
      15,   110,    90,    62,   111,   113,   114,   112,   141,   143,
     146,   150,     0,     0,     0,     0,     0,   103,     0,     0,
       0,     0,     0,     0,    41,    56,     0,   125,    53,     0,
      43,     0,    63,    95,     0,    30,     0,    37,    35,    32,
      34,     0,     0,    16,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,   119,   119,   119,     0,
     115,     0,    55,     0,     0,    89,    87,    88,    92,    91,
       0,    57,    58,     0,     0,   119,   119,    42,    44,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    93,    54,
      60,    22,     0,     0,    38,    25,    23,    76,    86,    85,
      84,    83,    82,    81,    80,    79,    78,    77,   120,     0,
       0,     0,     0,     0,   116,     0,     0,     0,    52,    59,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   105,
     104,   108,   109,   107,   106,    70,    69,    64,    65,    66,
      67,    68,     0,     0,     0,     0,    96,     0,    97,    98,
      99,     0,   117,     0,     0,     0,   119,   119,   101,   100,
      61,    40,    36,    39,   121,    45,     0,    63,     0,    47,
       0,     0,     0,     0,     0,   115,     0,    50,     0,   102,
      46,     0,    48,    51,     0,    49
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    28,    29,    30,   127,    32,    33,    34,    35,    52,
      67,    68,    60,    79,    93,    94,   203,   204,   128,   129,
     130,   163,   173,   218,   132,   133,   225,   278,   219,    61,
     134,    37
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -154
static const yytype_int16 yypact[] =
{
     345,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,    17,  -154,    34,    85,   845,    52,   345,
    -154,    30,  -154,  -154,   441,  -154,   114,  -154,   -36,    61,
     -38,   150,    30,  -154,  -154,  -154,   441,    85,  -154,    70,
     643,   108,  -154,  -154,    85,    75,  -154,   643,  -154,   875,
     -78,  -154,   169,    97,    91,   905,  -154,   249,  -154,    40,
     -56,   643,    -5,  -154,   100,   643,  -154,   643,    16,  -154,
    -154,  -154,   -50,   -45,  -154,    49,   100,   205,  -154,   185,
      51,  -154,   -74,    21,  -154,   493,  -154,   493,  -154,   100,
    -154,   561,  -154,  -154,  -154,  -154,  -154,  -154,   113,   149,
     158,   163,   166,   167,   191,   397,   493,  -154,   493,   493,
     493,   493,   493,   493,  -154,    65,   119,   -35,  -154,   301,
    -154,   627,  -154,  -154,   255,  -154,   100,   280,  -154,  -154,
    -154,   660,   693,  -154,   493,   493,   493,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   493,  -154,   246,   726,  -154,   -14,   -14,   -14,   -14,
     876,  -154,  -154,   281,   287,   493,   493,  -154,  -154,   493,
     493,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,  -154,  -154,  -154,
     240,  -154,   242,    50,  -154,  -154,  -154,   565,   565,   565,
     565,   565,   565,   565,   565,   565,   565,   565,   565,    93,
     107,   145,   151,   906,   565,   202,   936,   209,  -154,  -154,
     -40,   211,    66,   154,   995,  1015,  1034,  1052,  1069,  1086,
    1086,     6,     6,     6,     6,   116,   116,    77,    77,   -14,
     -14,   -14,   493,   493,   206,   280,  -154,   493,  -154,  -154,
    -154,   397,   493,   397,   493,   493,   493,   493,  -154,  -154,
     565,   565,  -154,  -154,   565,   285,   565,   241,   250,  -154,
     966,   759,    87,    88,   397,   493,   251,  -154,   254,  -154,
    -154,   245,  -154,  -154,   397,  -154
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -154,  -154,  -154,   312,   533,  -154,   297,   321,   -19,   313,
    -154,   284,   -31,  -154,  -154,   261,  -154,   124,   -61,  -154,
    -113,  -154,  -154,   -87,   118,  -154,    96,  -154,  -153,   139,
      92,   -18
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -119
static const yytype_int16 yytable[] =
{
     131,    95,   162,   220,   221,   222,    97,    54,   141,    43,
     142,   265,    47,    88,    74,    48,   178,   137,    75,     1,
      38,   138,   232,   233,     1,   100,    72,    48,   131,   164,
      47,   165,   166,   167,   168,   169,   170,    39,   143,    84,
      85,    43,   131,    82,    83,    96,    90,    43,    47,   266,
      98,    47,    44,    50,   175,    51,   176,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   171,   197,
     198,   172,   223,   224,   226,   201,  -119,  -119,  -119,  -119,
     190,   191,   192,   193,   194,   195,   196,    86,     1,   197,
     198,    75,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
      91,     1,    63,   282,   283,   139,    47,    49,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    36,
      24,    99,    62,   136,   254,    75,   255,    75,   275,    62,
     279,    64,    51,    55,    65,   194,   195,   196,   268,    57,
     197,   198,   257,    62,    71,   270,   271,    89,    36,    62,
     274,   290,    76,    36,   131,   276,   131,   280,   281,   288,
     289,   295,    78,   257,   257,    36,    77,   256,   135,   257,
      69,    87,   192,   193,   194,   195,   196,   131,   224,   197,
     198,   258,    66,   257,   155,   174,    69,   131,   101,   102,
     103,   104,   105,   106,   107,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,   108,    16,
     109,    18,    19,   110,    21,    22,    58,   111,   112,   259,
     156,   257,   113,   114,   115,   260,   116,   257,   269,   157,
     257,    59,     1,    63,   158,   159,   160,   117,   200,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
     161,    24,   118,   202,   227,   230,   119,   120,   121,   122,
     231,   252,    64,   253,   123,    65,    87,   262,   264,   124,
     267,   272,   125,   126,   101,   102,   103,   104,   105,   106,
     107,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,   108,    16,   109,    18,    19,   110,
      21,    22,    58,   111,   112,   284,  -118,   294,   113,   114,
     115,    45,   116,    80,    56,   285,   292,    59,     1,   293,
      46,    81,    53,   117,   140,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   118,   273,
     277,   291,   119,   120,   121,   122,     0,    25,     0,    26,
     123,    27,    87,     0,     0,   177,     0,     0,   125,   126,
     101,   102,   103,   104,   105,   106,   107,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
     108,    16,   109,    18,    19,   110,    21,    22,    58,   111,
     112,     0,     0,     0,   113,   114,   115,     0,   116,     0,
       0,     0,     0,    59,     1,     0,     0,     0,     0,   117,
       0,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,   118,     0,     0,     0,   119,   120,
     121,   122,     0,    25,     0,     0,   123,    27,    87,     0,
       0,     0,     0,     0,   125,   126,   101,   102,   103,   104,
     105,   106,   107,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,   108,    16,   109,    18,
      19,   110,    21,    22,    58,   111,     0,     0,     0,     0,
       0,     0,     0,    31,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,   117,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      42,     0,    31,     0,   -10,     0,     0,    31,     0,     0,
     118,     0,     0,     0,   119,   120,   121,   122,     0,    31,
       0,     0,   123,    31,    31,     0,     0,    70,     0,     0,
      31,   126,    42,     0,     0,     0,     0,     0,    42,     0,
      31,     0,     0,     0,    31,     0,     0,     0,    31,     0,
      31,    92,   144,     0,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,     0,     0,    92,     0,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,     1,   -10,   197,   198,
     -10,     0,   -10,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    58,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,     0,     0,
     197,   198,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   199,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,     0,     0,   197,   198,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   205,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,     0,     0,   197,   198,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   206,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,     0,     0,   197,
     198,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   228,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
       0,     0,   197,   198,     0,     0,     0,     0,     1,     0,
       0,     0,     0,     0,   287,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    41,    24,     1,     0,
       0,     0,     0,     0,     0,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    73,    24,     1,     0,
       0,     0,     0,     0,     0,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     0,    24,     0,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,     0,     0,   197,
     198,     0,     0,     0,     0,     0,     0,     0,   229,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,     0,     0,   197,
     198,     0,     0,     0,     0,     0,     0,     0,   261,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,     0,     0,   197,
     198,     0,     0,     0,     0,     0,     0,     0,   263,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,     0,     0,   197,
     198,     0,     0,     0,     0,     0,     0,     0,   286,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,     0,     0,   197,   198,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,     0,     0,   197,   198,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,     0,     0,   197,   198,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,     0,     0,   197,   198,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
       0,     0,   197,   198,  -119,  -119,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,     0,     0,   197,
     198
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-154)))

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-119)))

static const yytype_int16 yycheck[] =
{
      87,    51,   115,   156,   157,   158,    51,    45,    95,    27,
      97,    51,    86,    74,    92,    34,   129,    91,    96,     3,
       3,    95,   175,   176,     3,    86,    57,    46,   115,   116,
      86,   118,   119,   120,   121,   122,   123,     3,    99,    95,
      71,    59,   129,     3,     4,    95,    77,    65,    86,    89,
      95,    86,     0,    89,    89,    91,    91,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,     3,    83,
      84,     6,   159,   160,   161,   136,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    92,     3,    83,
      84,    96,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
      94,     3,     4,   266,   267,    94,    86,     3,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,     0,
      32,    92,    50,    92,    94,    96,    96,    96,   261,    57,
     263,    43,    91,     3,    46,    78,    79,    80,    92,    89,
      83,    84,    96,    71,    89,   252,   253,    75,    29,    77,
     257,   284,     3,    34,   261,   262,   263,   264,   265,    92,
      92,   294,    91,    96,    96,    46,    89,    94,     3,    96,
      51,    91,    76,    77,    78,    79,    80,   284,   285,    83,
      84,    94,    94,    96,    91,    86,    67,   294,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    94,
      91,    96,    37,    38,    39,    94,    41,    96,    94,    91,
      96,    46,     3,     4,    91,    89,    89,    52,     3,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      89,    32,    77,     3,    38,     4,    81,    82,    83,    84,
       3,    51,    43,    51,    89,    46,    91,    95,    89,    94,
      89,    95,    97,    98,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    50,    95,    92,    37,    38,
      39,    29,    41,    94,    47,    95,    95,    46,     3,    95,
      29,    67,    39,    52,    93,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    77,   255,
     262,   285,    81,    82,    83,    84,    -1,    42,    -1,    44,
      89,    46,    91,    -1,    -1,    94,    -1,    -1,    97,    98,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    -1,
      -1,    -1,    -1,    46,     3,    -1,    -1,    -1,    -1,    52,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    77,    -1,    -1,    -1,    81,    82,
      83,    84,    -1,    42,    -1,    -1,    89,    46,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     0,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    -1,    29,    -1,     3,    -1,    -1,    34,    -1,    -1,
      77,    -1,    -1,    -1,    81,    82,    83,    84,    -1,    46,
      -1,    -1,    89,    50,    51,    -1,    -1,    54,    -1,    -1,
      57,    98,    59,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    75,    -1,
      77,    78,    51,    -1,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    -1,    93,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,     3,    86,    83,    84,
      89,    -1,    91,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    95,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    -1,    83,    84,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    -1,    83,    84,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    -1,    83,    84,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    -1,    83,    84,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    -1,    83,    84,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    -1,    83,
      84
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    42,    44,    46,   100,   101,
     102,   103,   104,   105,   106,   107,   128,   130,     3,     3,
     103,    31,   103,   130,     0,   102,   106,    86,   107,     3,
      89,    91,   108,   108,    45,     3,   105,    89,    31,    46,
     111,   128,   129,     4,    43,    46,    94,   109,   110,   128,
     103,    89,   111,    31,    92,    96,     3,    89,    91,   112,
      94,   110,     3,     4,    95,   111,    92,    91,   117,   129,
     111,    94,   103,   113,   114,    51,    95,    51,    95,    92,
     117,     3,     4,     5,     6,     7,     8,     9,    23,    25,
      28,    32,    33,    37,    38,    39,    41,    52,    77,    81,
      82,    83,    84,    89,    94,    97,    98,   103,   117,   118,
     119,   122,   123,   124,   129,     3,    92,    91,    95,    94,
     114,   122,   122,   117,    51,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    91,    91,    91,    91,    89,
      89,    89,   119,   120,   122,   122,   122,   122,   122,   122,
     122,     3,     6,   121,    86,    89,    91,    94,   119,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    83,    84,    95,
       3,   117,     3,   115,   116,    95,    95,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   127,
     127,   127,   127,   122,   122,   125,   122,    38,    95,    92,
       4,     3,   127,   127,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,    51,    51,    94,    96,    94,    96,    94,    94,
      94,    92,    95,    92,    89,    51,    89,    89,    92,    94,
     122,   122,    95,   116,   122,   119,   122,   123,   126,   119,
     122,   122,   127,   127,    50,    95,    92,    95,    92,    92,
     119,   125,    95,    95,    92,   119
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
#line 116 "compose.y"
    { ast_create_using_list((yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 4:
/* Line 1787 of yacc.c  */
#line 120 "compose.y"
    { (yyval.pAst) = ast_append(kAST_UsingList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 5:
/* Line 1787 of yacc.c  */
#line 121 "compose.y"
    { (yyval.pAst) = ast_append(kAST_UsingList, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 6:
/* Line 1787 of yacc.c  */
#line 125 "compose.y"
    { (yyval.pAst) = ast_create_using_stmt((yyvsp[(2) - (5)].pAst), (yyvsp[(4) - (5)].pAst), pParseData); }
    break;

  case 7:
/* Line 1787 of yacc.c  */
#line 129 "compose.y"
    { (yyval.pAst) = ast_create_dotted_id((yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 8:
/* Line 1787 of yacc.c  */
#line 133 "compose.y"
    { (yyval.pAst) = ast_append(kAST_DottedId, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 9:
/* Line 1787 of yacc.c  */
#line 134 "compose.y"
    { (yyval.pAst) = ast_append(kAST_DottedId, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 10:
/* Line 1787 of yacc.c  */
#line 138 "compose.y"
    { (yyval.pAst) = ast_create_with_str(kAST_DottedIdPart, kASTF_None, (yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 148 "compose.y"
    { (yyval.pAst) = ast_create_entity_def((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 149 "compose.y"
    { (yyval.pAst) = ast_create_component_def((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 150 "compose.y"
    { (yyval.pAst) = ast_create_function_def((yyvsp[(2) - (6)].str), (yyvsp[(1) - (6)].pAst), (yyvsp[(6) - (6)].pAst), pParseData); }
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 151 "compose.y"
    { (yyval.pAst) = ast_create_function_def((yyvsp[(3) - (7)].str), ast_create_with_numi(kAST_DataType, kASTF_Const,CONST_DT((yyvsp[(2) - (7)].dataType)), pParseData), (yyvsp[(7) - (7)].pAst), pParseData); }
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 152 "compose.y"
    { (yyval.pAst) = ast_create_function_def((yyvsp[(2) - (6)].str), ast_create_with_numi(kAST_DataType, kASTF_None, (yyvsp[(1) - (6)].dataType), pParseData), (yyvsp[(6) - (6)].pAst), pParseData); }
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 156 "compose.y"
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 157 "compose.y"
    { (yyval.pAst) = ast_create_block((yyvsp[(2) - (3)].pAst),   pParseData); }
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 161 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 162 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 166 "compose.y"
    { (yyval.pAst) = ast_create_message_def((yyvsp[(1) - (5)].str), (yyvsp[(3) - (5)].pSymTab), (yyvsp[(5) - (5)].pAst), pParseData); }
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 167 "compose.y"
    { (yyval.pAst) = ast_create_property_def((yyvsp[(2) - (5)].str), (yyvsp[(1) - (5)].pAst), (yyvsp[(4) - (5)].pAst), pParseData); }
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 168 "compose.y"
    { (yyval.pAst) = ast_create_property_def((yyvsp[(2) - (3)].str), (yyvsp[(1) - (3)].pAst), NULL, pParseData); }
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 169 "compose.y"
    { (yyval.pAst) = ast_create_field_def((yyvsp[(2) - (5)].str), (yyvsp[(1) - (5)].pAst), (yyvsp[(4) - (5)].pAst), pParseData); }
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 170 "compose.y"
    { (yyval.pAst) = ast_create_field_def((yyvsp[(2) - (3)].str), (yyvsp[(1) - (3)].pAst), NULL, pParseData); }
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 171 "compose.y"
    { (yyval.pAst) = ast_create_component_members((yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 175 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, NULL); }
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 176 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, symrec_create(kSYMT_Param, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].str), NULL, pParseData)); }
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 177 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, (yyvsp[(1) - (4)].pSymTab), symrec_create(kSYMT_Param, (yyvsp[(3) - (4)].pAst), (yyvsp[(4) - (4)].str), NULL, pParseData)); }
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 181 "compose.y"
    { (yyval.pAst) = ast_create_with_child_list(kAST_ComponentMemberList, pParseData); }
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 182 "compose.y"
    { (yyval.pAst) = (yyvsp[(2) - (3)].pAst); }
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 186 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 34:
/* Line 1787 of yacc.c  */
#line 187 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 191 "compose.y"
    { (yyval.pAst) = ast_create_component_member((yyvsp[(1) - (2)].pAst), ast_create(kAST_PropInit, pParseData), pParseData); }
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 192 "compose.y"
    { (yyval.pAst) = ast_create_component_member((yyvsp[(1) - (5)].pAst), (yyvsp[(3) - (5)].pAst), pParseData); }
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 196 "compose.y"
    { (yyval.pAst) = ast_create(kAST_PropInitList, pParseData); }
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 197 "compose.y"
    { (yyval.pAst) = ast_append(kAST_PropInitList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 198 "compose.y"
    { (yyval.pAst) = ast_append(kAST_PropInitList, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 202 "compose.y"
    { (yyval.pAst) = ast_create_prop_init((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 206 "compose.y"
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 207 "compose.y"
    { (yyval.pAst) = ast_create_block((yyvsp[(2) - (3)].pAst),   pParseData); }
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 211 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 212 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 45:
/* Line 1787 of yacc.c  */
#line 216 "compose.y"
    { (yyval.pAst) = ast_create_if((yyvsp[(3) - (5)].pAst), (yyvsp[(5) - (5)].pAst), NULL, pParseData); }
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 217 "compose.y"
    { (yyval.pAst) = ast_create_if((yyvsp[(3) - (7)].pAst), (yyvsp[(5) - (7)].pAst), (yyvsp[(7) - (7)].pAst),   pParseData); }
    break;

  case 47:
/* Line 1787 of yacc.c  */
#line 219 "compose.y"
    { (yyval.pAst) = ast_create_while((yyvsp[(3) - (5)].pAst), (yyvsp[(5) - (5)].pAst), pParseData); }
    break;

  case 48:
/* Line 1787 of yacc.c  */
#line 220 "compose.y"
    { (yyval.pAst) = ast_create_dowhile((yyvsp[(5) - (7)].pAst), (yyvsp[(2) - (7)].pAst), pParseData); }
    break;

  case 49:
/* Line 1787 of yacc.c  */
#line 222 "compose.y"
    { (yyval.pAst) = ast_create_for((yyvsp[(3) - (9)].pAst), (yyvsp[(5) - (9)].pAst), (yyvsp[(7) - (9)].pAst), (yyvsp[(9) - (9)].pAst), pParseData); }
    break;

  case 50:
/* Line 1787 of yacc.c  */
#line 224 "compose.y"
    { (yyval.pAst) = ast_create_property_set((yyvsp[(2) - (6)].pAst), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].pAst), pParseData); }
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 225 "compose.y"
    { (yyval.pAst) = ast_create_message_send((yyvsp[(2) - (7)].pAst), (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].pAst), pParseData); }
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 227 "compose.y"
    { (yyval.pAst) = ast_create_return((yyvsp[(2) - (3)].pAst), pParseData); }
    break;

  case 53:
/* Line 1787 of yacc.c  */
#line 229 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 54:
/* Line 1787 of yacc.c  */
#line 231 "compose.y"
    { (yyval.pAst) = ast_create_simple_stmt((yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 55:
/* Line 1787 of yacc.c  */
#line 235 "compose.y"
    { parsedata_handle_do_scope(pParseData); (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 56:
/* Line 1787 of yacc.c  */
#line 238 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 239 "compose.y"
    { (yyval.pAst) = ast_create_identifier((yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 58:
/* Line 1787 of yacc.c  */
#line 240 "compose.y"
    { (yyval.pAst) = ast_create_int_literal((yyvsp[(1) - (1)].numi), pParseData); }
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 244 "compose.y"
    { (yyval.pAst) = (yyvsp[(2) - (3)].pAst); }
    break;

  case 60:
/* Line 1787 of yacc.c  */
#line 246 "compose.y"
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].str), NULL, pParseData)); }
    break;

  case 61:
/* Line 1787 of yacc.c  */
#line 247 "compose.y"
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[(1) - (4)].pAst), (yyvsp[(2) - (4)].str), (yyvsp[(4) - (4)].pAst), pParseData)); }
    break;

  case 62:
/* Line 1787 of yacc.c  */
#line 249 "compose.y"
    { (yyval.pAst) = ast_create_string_literal((yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 63:
/* Line 1787 of yacc.c  */
#line 251 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 64:
/* Line 1787 of yacc.c  */
#line 253 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Add,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 65:
/* Line 1787 of yacc.c  */
#line 254 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Sub,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 66:
/* Line 1787 of yacc.c  */
#line 255 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Mul,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 67:
/* Line 1787 of yacc.c  */
#line 256 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Div,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 68:
/* Line 1787 of yacc.c  */
#line 257 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Mod,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 69:
/* Line 1787 of yacc.c  */
#line 258 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LShift, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 70:
/* Line 1787 of yacc.c  */
#line 259 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_RShift, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 71:
/* Line 1787 of yacc.c  */
#line 260 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Or,     (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 72:
/* Line 1787 of yacc.c  */
#line 261 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_And,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 73:
/* Line 1787 of yacc.c  */
#line 262 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitOr,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 74:
/* Line 1787 of yacc.c  */
#line 263 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitXor, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 75:
/* Line 1787 of yacc.c  */
#line 264 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitAnd, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 76:
/* Line 1787 of yacc.c  */
#line 266 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_Assign,       (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 77:
/* Line 1787 of yacc.c  */
#line 267 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_AddAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 78:
/* Line 1787 of yacc.c  */
#line 268 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_SubAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 79:
/* Line 1787 of yacc.c  */
#line 269 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_MulAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 80:
/* Line 1787 of yacc.c  */
#line 270 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_DivAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 81:
/* Line 1787 of yacc.c  */
#line 271 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_ModAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 82:
/* Line 1787 of yacc.c  */
#line 272 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_LShiftAssign, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 83:
/* Line 1787 of yacc.c  */
#line 273 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_RShiftAssign, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 84:
/* Line 1787 of yacc.c  */
#line 274 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_AndAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 85:
/* Line 1787 of yacc.c  */
#line 275 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_XorAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 86:
/* Line 1787 of yacc.c  */
#line 276 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_OrAssign,     (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 87:
/* Line 1787 of yacc.c  */
#line 278 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Not,        (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 88:
/* Line 1787 of yacc.c  */
#line 279 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Complement, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 89:
/* Line 1787 of yacc.c  */
#line 280 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Negate,     (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 90:
/* Line 1787 of yacc.c  */
#line 282 "compose.y"
    { (yyval.pAst) = ast_create_hash((yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 91:
/* Line 1787 of yacc.c  */
#line 284 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PreInc, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 92:
/* Line 1787 of yacc.c  */
#line 285 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PreDec, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 93:
/* Line 1787 of yacc.c  */
#line 286 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PostInc, (yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 94:
/* Line 1787 of yacc.c  */
#line 287 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PostDec, (yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 95:
/* Line 1787 of yacc.c  */
#line 289 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 96:
/* Line 1787 of yacc.c  */
#line 291 "compose.y"
    { (yyval.pAst) = ast_create_color_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 97:
/* Line 1787 of yacc.c  */
#line 292 "compose.y"
    { (yyval.pAst) = ast_create_vec3_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 98:
/* Line 1787 of yacc.c  */
#line 293 "compose.y"
    { (yyval.pAst) = ast_create_mat34_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 99:
/* Line 1787 of yacc.c  */
#line 294 "compose.y"
    { (yyval.pAst) = ast_create_string_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 100:
/* Line 1787 of yacc.c  */
#line 296 "compose.y"
    { (yyval.pAst) = ast_create_entity_or_struct_init((yyvsp[(1) - (4)].pAst), (yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 101:
/* Line 1787 of yacc.c  */
#line 297 "compose.y"
    { (yyval.pAst) = ast_create_function_call((yyvsp[(1) - (4)].pAst), (yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 102:
/* Line 1787 of yacc.c  */
#line 299 "compose.y"
    { (yyval.pAst) = ast_create_system_api_call((yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].pAst), pParseData); }
    break;

  case 103:
/* Line 1787 of yacc.c  */
#line 301 "compose.y"
    { (yyval.pAst) = ast_create(kAST_Transform, pParseData); }
    break;

  case 104:
/* Line 1787 of yacc.c  */
#line 305 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Eq,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 105:
/* Line 1787 of yacc.c  */
#line 306 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_NEq, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 106:
/* Line 1787 of yacc.c  */
#line 307 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LTE, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 107:
/* Line 1787 of yacc.c  */
#line 308 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_GTE, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 108:
/* Line 1787 of yacc.c  */
#line 309 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LT,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 109:
/* Line 1787 of yacc.c  */
#line 310 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_GT,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 110:
/* Line 1787 of yacc.c  */
#line 311 "compose.y"
    { (yyval.pAst) = ast_create_symbol_ref((yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 111:
/* Line 1787 of yacc.c  */
#line 315 "compose.y"
    { (yyval.pAst) = ast_create_int_literal((yyvsp[(1) - (1)].numi), pParseData); }
    break;

  case 112:
/* Line 1787 of yacc.c  */
#line 316 "compose.y"
    { (yyval.pAst) = ast_create_float_literal((yyvsp[(1) - (1)].numf), pParseData); }
    break;

  case 113:
/* Line 1787 of yacc.c  */
#line 317 "compose.y"
    { (yyval.pAst) = ast_create_int_literal(1, pParseData); }
    break;

  case 114:
/* Line 1787 of yacc.c  */
#line 318 "compose.y"
    { (yyval.pAst) = ast_create_int_literal(0, pParseData); }
    break;

  case 115:
/* Line 1787 of yacc.c  */
#line 322 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 116:
/* Line 1787 of yacc.c  */
#line 323 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 117:
/* Line 1787 of yacc.c  */
#line 327 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 118:
/* Line 1787 of yacc.c  */
#line 328 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 119:
/* Line 1787 of yacc.c  */
#line 332 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, NULL, pParseData); }
    break;

  case 120:
/* Line 1787 of yacc.c  */
#line 333 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, (yyvsp[(1) - (1)].pAst),   pParseData); }
    break;

  case 121:
/* Line 1787 of yacc.c  */
#line 334 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, (yyvsp[(1) - (3)].pAst),   (yyvsp[(3) - (3)].pAst),   pParseData); }
    break;

  case 122:
/* Line 1787 of yacc.c  */
#line 338 "compose.y"
    { (yyval.pAst) = ast_create_with_numi(kAST_DataType, kASTF_Const,CONST_DT((yyvsp[(2) - (2)].dataType)), pParseData); }
    break;

  case 123:
/* Line 1787 of yacc.c  */
#line 339 "compose.y"
    { (yyval.pAst) = ast_create_with_numi(kAST_DataType, kASTF_None, (yyvsp[(1) - (1)].dataType), pParseData); }
    break;

  case 124:
/* Line 1787 of yacc.c  */
#line 340 "compose.y"
    { (yyval.pAst) = ast_create_custom_type(kASTF_Const, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 125:
/* Line 1787 of yacc.c  */
#line 341 "compose.y"
    { (yyval.pAst) = ast_create_custom_type(kASTF_None, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 126:
/* Line 1787 of yacc.c  */
#line 345 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 127:
/* Line 1787 of yacc.c  */
#line 346 "compose.y"
    { (yyval.pAst) = ast_create_with_numi(kAST_DataType, kASTF_Const,CONST_DT((yyvsp[(2) - (2)].dataType)), pParseData); }
    break;

  case 128:
/* Line 1787 of yacc.c  */
#line 347 "compose.y"
    { (yyval.pAst) = ast_create_with_numi(kAST_DataType, kASTF_None, (yyvsp[(1) - (1)].dataType), pParseData); }
    break;


/* Line 1787 of yacc.c  */
#line 2774 "compose_parser.c"
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
#line 374 "compose.y"



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

