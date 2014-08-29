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
     CHAR = 263,
     BYTE = 264,
     SHORT = 265,
     USHORT = 266,
     INT = 267,
     UINT = 268,
     LONG = 269,
     ULONG = 270,
     HALF = 271,
     FLOAT = 272,
     DOUBLE = 273,
     BOOL = 274,
     VEC2 = 275,
     VEC3 = 276,
     VEC4 = 277,
     MAT3 = 278,
     MAT34 = 279,
     MAT4 = 280,
     VOID = 281,
     HANDLE = 282,
     IF = 283,
     SWITCH = 284,
     CASE = 285,
     DEFAULT = 286,
     FOR = 287,
     WHILE = 288,
     DO = 289,
     BREAK = 290,
     RETURN = 291,
     ENTITY = 292,
     COMPONENT = 293,
     COMPONENTS = 294,
     IMPORT = 295,
     THEN = 296,
     ELSE = 297,
     OR_ASSIGN = 298,
     XOR_ASSIGN = 299,
     AND_ASSIGN = 300,
     RSHIFT_ASSIGN = 301,
     LSHIFT_ASSIGN = 302,
     MOD_ASSIGN = 303,
     DIV_ASSIGN = 304,
     MUL_ASSIGN = 305,
     SUB_ASSIGN = 306,
     ADD_ASSIGN = 307,
     OR = 308,
     AND = 309,
     NEQ = 310,
     EQ = 311,
     GTE = 312,
     LTE = 313,
     RSHIFT = 314,
     LSHIFT = 315,
     DEC = 316,
     INC = 317,
     UMINUS = 318,
     POSTDEC = 319,
     POSTINC = 320,
     SCOPE = 321
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
#line 198 "compose_parser.c"
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
#line 247 "compose_parser.c"

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
#define YYFINAL  37
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   909

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  91
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  132
/* YYNRULES -- Number of states.  */
#define YYNSTATES  249

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   321

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    72,     2,    74,     2,    71,    58,     2,
      81,    84,    69,    67,    89,    68,    78,    70,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    88,
      61,    43,    62,     2,    90,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    82,     2,    85,    57,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    83,    56,    86,    73,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      59,    60,    63,    64,    65,    66,    75,    76,    77,    79,
      80,    87
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    13,    17,    19,    23,
      25,    27,    30,    34,    38,    45,    48,    52,    54,    57,
      64,    71,    76,    82,    86,    89,    90,    93,    98,   101,
     105,   107,   110,   113,   119,   120,   122,   126,   130,   133,
     137,   139,   142,   148,   156,   162,   170,   180,   188,   197,
     201,   203,   206,   208,   209,   211,   213,   217,   220,   225,
     227,   231,   235,   239,   243,   247,   251,   255,   259,   263,
     267,   271,   275,   279,   283,   287,   291,   295,   299,   303,
     307,   311,   315,   319,   322,   325,   328,   331,   334,   337,
     340,   343,   345,   350,   352,   356,   360,   364,   368,   372,
     376,   378,   380,   382,   384,   385,   387,   388,   390,   391,
     393,   397,   399,   401,   403,   405,   407,   409,   411,   413,
     415,   417,   419,   421,   423,   425,   427,   429,   431,   433,
     435,   437,   439
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      92,     0,    -1,    97,    -1,    93,    97,    -1,    94,    -1,
      93,    94,    -1,    40,    95,    88,    -1,    96,    -1,    95,
      78,    96,    -1,     3,    -1,    98,    -1,    97,    98,    -1,
      37,     3,    99,    -1,    38,     3,    99,    -1,   119,     3,
      81,   102,    84,   108,    -1,    83,    86,    -1,    83,   100,
      86,    -1,   101,    -1,   100,   101,    -1,    74,     3,    81,
     102,    84,   108,    -1,   119,    74,     3,    43,   113,    88,
      -1,   119,    74,     3,    88,    -1,   119,     3,    43,   113,
      88,    -1,   119,     3,    88,    -1,    39,   103,    -1,    -1,
     119,     3,    -1,   102,    89,   119,     3,    -1,    83,    86,
      -1,    83,   104,    86,    -1,   105,    -1,   104,   105,    -1,
       3,    88,    -1,     3,    81,   106,    84,    88,    -1,    -1,
     107,    -1,   106,    89,   107,    -1,     3,    43,   113,    -1,
      83,    86,    -1,    83,   109,    86,    -1,   110,    -1,   109,
     110,    -1,    28,    81,   114,    84,   110,    -1,    28,    81,
     114,    84,   110,    42,   110,    -1,    33,    81,   114,    84,
     110,    -1,    34,   111,    33,    81,   114,    84,    88,    -1,
      32,    81,   116,    88,   117,    88,   116,    84,   110,    -1,
      90,   112,    74,     3,    43,   113,    88,    -1,    90,   112,
      74,     3,    81,   118,    84,    88,    -1,    36,   113,    88,
      -1,   108,    -1,   113,    88,    -1,   110,    -1,    -1,     3,
      -1,     4,    -1,    81,   113,    84,    -1,   119,     3,    -1,
     119,     3,    43,   113,    -1,   114,    -1,   113,    67,   113,
      -1,   113,    68,   113,    -1,   113,    69,   113,    -1,   113,
      70,   113,    -1,   113,    71,   113,    -1,   113,    66,   113,
      -1,   113,    65,   113,    -1,   113,    54,   113,    -1,   113,
      55,   113,    -1,   113,    56,   113,    -1,   113,    57,   113,
      -1,   113,    58,   113,    -1,     3,    43,   113,    -1,     3,
      53,   113,    -1,     3,    52,   113,    -1,     3,    51,   113,
      -1,     3,    50,   113,    -1,     3,    49,   113,    -1,     3,
      48,   113,    -1,     3,    47,   113,    -1,     3,    46,   113,
      -1,     3,    45,   113,    -1,     3,    44,   113,    -1,    72,
     113,    -1,    73,   113,    -1,    68,   113,    -1,    74,     3,
      -1,    76,   113,    -1,    75,   113,    -1,   113,    76,    -1,
     113,    75,    -1,   115,    -1,     3,    81,   118,    84,    -1,
       3,    -1,   113,    60,   113,    -1,   113,    59,   113,    -1,
     113,    64,   113,    -1,   113,    63,   113,    -1,   113,    61,
     113,    -1,   113,    62,   113,    -1,     4,    -1,     7,    -1,
       5,    -1,     6,    -1,    -1,   113,    -1,    -1,   114,    -1,
      -1,   113,    -1,   118,    89,   113,    -1,   120,    -1,     3,
      -1,     8,    -1,     9,    -1,    10,    -1,    11,    -1,    12,
      -1,    13,    -1,    14,    -1,    15,    -1,    16,    -1,    17,
      -1,    18,    -1,    19,    -1,    20,    -1,    21,    -1,    22,
      -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,    27,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   116,   116,   117,   121,   122,   126,   130,   131,   135,
     139,   140,   144,   145,   146,   150,   151,   155,   156,   160,
     161,   162,   163,   164,   165,   169,   170,   171,   175,   176,
     180,   181,   185,   186,   190,   191,   192,   196,   200,   201,
     205,   206,   210,   211,   213,   214,   216,   218,   219,   221,
     223,   225,   229,   232,   233,   234,   238,   240,   241,   243,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   270,   271,   272,   274,   276,   277,   278,
     279,   281,   283,   284,   289,   290,   291,   292,   293,   294,
     298,   299,   300,   301,   305,   306,   310,   311,   315,   316,
     317,   321,   322,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "INT_LITERAL", "TRUE",
  "FALSE", "FLOAT_LITERAL", "CHAR", "BYTE", "SHORT", "USHORT", "INT",
  "UINT", "LONG", "ULONG", "HALF", "FLOAT", "DOUBLE", "BOOL", "VEC2",
  "VEC3", "VEC4", "MAT3", "MAT34", "MAT4", "VOID", "HANDLE", "IF",
  "SWITCH", "CASE", "DEFAULT", "FOR", "WHILE", "DO", "BREAK", "RETURN",
  "ENTITY", "COMPONENT", "COMPONENTS", "IMPORT", "THEN", "ELSE", "'='",
  "OR_ASSIGN", "XOR_ASSIGN", "AND_ASSIGN", "RSHIFT_ASSIGN",
  "LSHIFT_ASSIGN", "MOD_ASSIGN", "DIV_ASSIGN", "MUL_ASSIGN", "SUB_ASSIGN",
  "ADD_ASSIGN", "OR", "AND", "'|'", "'^'", "'&'", "NEQ", "EQ", "'<'",
  "'>'", "GTE", "LTE", "RSHIFT", "LSHIFT", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'!'", "'~'", "'#'", "DEC", "INC", "UMINUS", "'.'", "POSTDEC",
  "POSTINC", "'('", "'['", "'{'", "')'", "']'", "'}'", "SCOPE", "';'",
  "','", "'@'", "$accept", "cmp_file", "import_list", "import_stmt",
  "dotted_id", "dotted_id_part", "def_list", "def", "message_block",
  "message_list", "message_prop", "param_list", "component_block",
  "component_member_list", "component_member", "prop_init_list",
  "prop_init", "block", "stmt_list", "stmt", "do_stmt", "target_expr",
  "expr", "cond_expr", "literal", "expr_or_empty", "cond_expr_or_empty",
  "fun_params", "type", "basic_type", YY_NULL
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
     295,   296,   297,    61,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   124,    94,    38,   310,
     311,    60,    62,   312,   313,   314,   315,    43,    45,    42,
      47,    37,    33,   126,    35,   316,   317,   318,    46,   319,
     320,    40,    91,   123,    41,    93,   125,   321,    59,    44,
      64
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    91,    92,    92,    93,    93,    94,    95,    95,    96,
      97,    97,    98,    98,    98,    99,    99,   100,   100,   101,
     101,   101,   101,   101,   101,   102,   102,   102,   103,   103,
     104,   104,   105,   105,   106,   106,   106,   107,   108,   108,
     109,   109,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   111,   112,   112,   112,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   114,   114,   114,   114,   114,   114,
     115,   115,   115,   115,   116,   116,   117,   117,   118,   118,
     118,   119,   119,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     3,     1,     3,     1,
       1,     2,     3,     3,     6,     2,     3,     1,     2,     6,
       6,     4,     5,     3,     2,     0,     2,     4,     2,     3,
       1,     2,     2,     5,     0,     1,     3,     3,     2,     3,
       1,     2,     5,     7,     5,     7,     9,     7,     8,     3,
       1,     2,     1,     0,     1,     1,     3,     2,     4,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     4,     1,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     0,     1,     0,     1,     0,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,     0,     0,     0,     0,     0,     4,     2,    10,
       0,   111,     0,     0,     9,     0,     7,     1,     5,     3,
      11,     0,     0,    12,    13,     0,     6,    25,     0,     0,
      15,     0,    17,     0,     8,     0,     0,     0,    24,     0,
      16,    18,     0,     0,     0,     0,    26,     0,    28,     0,
      30,    25,     0,    23,     0,     0,    14,     0,    34,    32,
      29,    31,     0,    93,   100,   102,   103,   101,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    91,     0,     0,
      21,     0,     0,     0,     0,     0,    38,    53,    50,     0,
      40,     0,    27,     0,     0,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   108,    85,
      83,    84,    86,    88,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    89,    22,    57,     0,     0,
     104,     0,    52,     0,     0,    54,    55,     0,    39,    41,
      51,     0,     0,     0,    19,    72,    82,    81,    80,    79,
      78,    77,    76,    75,    74,    73,   109,     0,    56,    67,
      68,    69,    70,    71,    95,    94,    98,    99,    97,    96,
      66,    65,    60,    61,    62,    63,    64,     0,    20,     0,
      59,   105,     0,    59,     0,    49,     0,    37,    33,    36,
      92,     0,    58,     0,   106,     0,     0,     0,   110,    42,
      59,     0,    44,    59,     0,   108,     0,   104,     0,     0,
       0,    43,     0,    45,    47,     0,     0,    48,    46
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    25,    26,    27,    35,    36,    28,    29,    43,    51,
      52,    55,    58,    69,    70,   114,   115,   108,   109,   110,
     163,   167,   111,    96,    97,   212,   231,   187,    98,    31
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -134
static const yytype_int16 yypact[] =
{
     203,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,  -134,    31,    37,    38,    61,   203,  -134,   283,  -134,
      59,  -134,     1,     1,  -134,   -63,  -134,  -134,  -134,   283,
    -134,   -18,   371,  -134,  -134,    38,  -134,   767,     9,    80,
    -134,   450,  -134,     8,  -134,   -77,    91,    -1,  -134,    14,
    -134,  -134,   -30,    93,    15,   767,  -134,   -58,  -134,     0,
    -134,   767,   487,  -134,   -29,   163,  -134,    94,   108,  -134,
    -134,  -134,   -60,    57,  -134,  -134,  -134,  -134,   487,   487,
     487,   110,   487,   487,   487,   515,  -134,  -134,   112,   487,
    -134,    40,    46,    53,   339,   487,  -134,     6,  -134,   251,
    -134,   550,  -134,    96,    28,  -134,    15,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,  -134,
     -44,   -44,  -134,   -44,   -44,   690,   487,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   487,  -134,  -134,  -134,    97,   585,   487,
     487,   487,  -134,   103,   620,  -134,  -134,    68,  -134,  -134,
    -134,   487,    56,   108,  -134,   741,   741,   741,   741,   741,
     741,   741,   741,   741,   741,   741,   741,    30,  -134,   482,
     762,   373,   781,   799,   817,   817,   833,   833,   833,   833,
     -32,   -32,    55,    55,   -44,   -44,   -44,   487,  -134,   741,
      62,   741,    60,    63,    64,  -134,   147,   741,  -134,  -134,
    -134,   487,   741,   339,   487,   339,   487,   -39,   741,   109,
      65,    69,  -134,    72,   487,   487,   339,   487,    70,   655,
      39,  -134,    75,  -134,  -134,    76,   339,  -134,  -134
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -134,  -134,  -134,   134,  -134,   148,   166,   -20,   161,  -134,
     149,   127,  -134,  -134,   132,  -134,    29,   -59,  -134,  -103,
    -134,  -134,   -72,  -133,  -134,   -34,  -134,   -31,    90,  -134
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -113
static const yytype_int16 yytable[] =
{
      95,   162,    67,    67,   234,    76,   169,    64,    40,   165,
     166,    62,    65,    72,    99,    45,   129,   130,   131,    40,
     133,   134,   135,    78,   116,    46,   210,   158,   213,    65,
      79,   154,   155,   164,    32,   149,   150,   151,   152,   153,
      33,    34,   235,   154,   155,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   174,    73,   100,
    -112,    37,    41,    47,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,    63,    59,    42,    68,    80,   209,   211,   209,
      30,   230,    57,   233,    66,    71,    74,   112,    75,   217,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   113,   172,   132,   220,   157,    30,   173,    30,   221,
     229,   159,   232,   245,   151,   152,   153,   160,   221,    30,
     154,   155,    53,   241,   161,   222,   214,    56,   128,   171,
     207,    53,   216,   248,   218,   226,   223,   225,   224,   228,
     227,   236,   209,  -107,   209,    77,   238,   237,   243,   246,
      38,    56,   239,   186,   247,   211,    83,    84,    85,    86,
      87,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   101,    39,    54,    44,   102,   103,   104,    82,   105,
      61,    81,   219,   242,   240,     0,     1,     0,     0,     0,
       0,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    88,     0,     0,     0,    89,    90,    91,    92,    93,
      22,    23,     0,    24,    94,     0,    75,     0,     0,   106,
       0,     0,     0,   107,    83,    84,    85,    86,    87,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   101,
       0,     0,     0,   102,   103,   104,     1,   105,     0,     0,
       0,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      22,    23,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,    75,     0,     0,   168,     0,     0,
       0,   107,    83,    84,    85,    86,    87,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   101,     0,     0,
       0,   102,   103,   104,     1,   105,     0,     0,     0,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
      48,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,    75,     0,     0,     0,     0,     0,     0,   107,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,    49,     0,     0,   154,   155,
       0,     0,     0,     1,     0,     0,     0,    50,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      83,    84,    85,    86,    87,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,     0,    88,     0,   154,   155,    89,
      90,    91,    92,    93,     0,     0,     0,     0,    94,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,     0,     0,     0,
     154,   155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   156,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,     0,     0,     0,   154,   155,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   170,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,     0,     0,     0,
     154,   155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   208,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,     0,     0,     0,   154,   155,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,     0,     0,     0,
     154,   155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   244,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,     0,     0,     0,   154,   155,     0,     0,     0,
       1,     0,     0,     0,   188,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,     0,     0,     0,   154,   155,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,     0,     0,     0,   154,   155,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,     0,     0,     0,   154,   155,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,     0,     0,     0,   154,   155,  -113,  -113,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,     0,
       0,     0,   154,   155,  -113,  -113,  -113,  -113,   147,   148,
     149,   150,   151,   152,   153,     0,     0,     0,   154,   155
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-134)))

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-113)))

static const yytype_int16 yycheck[] =
{
      72,   104,     3,     3,    43,    64,   109,    84,    28,     3,
       4,     3,    89,    43,    43,    78,    88,    89,    90,    39,
      92,    93,    94,    81,    84,    88,   159,    99,   161,    89,
      88,    75,    76,   105,     3,    67,    68,    69,    70,    71,
       3,     3,    81,    75,    76,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   116,    88,    88,
       3,     0,     3,    81,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    74,     3,    83,    86,    86,   159,   160,   161,
       0,   224,    83,   226,     3,    81,     3,     3,    83,   171,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,     3,    84,     3,    84,     3,    26,    89,    28,    89,
     223,    81,   225,    84,    69,    70,    71,    81,    89,    39,
      75,    76,    42,   236,    81,   207,    33,    47,    81,    43,
      43,    51,    74,   246,    88,    81,    84,    84,    88,   221,
       3,    42,   224,    88,   226,    65,    84,    88,    88,    84,
      26,    71,   234,   235,    88,   237,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    26,    45,    33,    32,    33,    34,    71,    36,
      51,    69,   173,   237,   235,    -1,     3,    -1,    -1,    -1,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    68,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      37,    38,    -1,    40,    81,    -1,    83,    -1,    -1,    86,
      -1,    -1,    -1,    90,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      -1,    -1,    -1,    32,    33,    34,     3,    36,    -1,    -1,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      37,    38,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    81,    -1,    83,    -1,    -1,    86,    -1,    -1,
      -1,    90,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    -1,    -1,
      -1,    32,    33,    34,     3,    36,    -1,    -1,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      39,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    74,    -1,    -1,    75,    76,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    86,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    68,    -1,    75,    76,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    81,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    84,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    -1,    -1,    75,    76,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    -1,    -1,    75,    76,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    -1,    -1,    75,    76,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    75,    76,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    76,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    -1,    -1,    75,    76
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    37,    38,    40,    92,    93,    94,    97,    98,
     119,   120,     3,     3,     3,    95,    96,     0,    94,    97,
      98,     3,    83,    99,    99,    78,    88,    81,    39,    74,
      86,   100,   101,   119,    96,   102,   119,    83,   103,     3,
      86,   101,     3,    74,    84,    89,     3,     3,    86,   104,
     105,    81,    43,    88,     3,    83,   108,   119,    81,    88,
      86,   105,   102,     3,     4,     5,     6,     7,    68,    72,
      73,    74,    75,    76,    81,   113,   114,   115,   119,    43,
      88,    28,    32,    33,    34,    36,    86,    90,   108,   109,
     110,   113,     3,     3,   106,   107,    84,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    81,   113,
     113,   113,     3,   113,   113,   113,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    75,    76,    88,     3,   113,    81,
      81,    81,   110,   111,   113,     3,     4,   112,    86,   110,
      88,    43,    84,    89,   108,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   118,    84,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,    43,    88,   113,
     114,   113,   116,   114,    33,    88,    74,   113,    88,   107,
      84,    89,   113,    84,    88,    84,    81,     3,   113,   110,
     114,   117,   110,   114,    43,    81,    42,    88,    84,   113,
     118,   110,   116,    88,    88,    84,    84,    88,   110
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
#line 117 "compose.y"
    { ast_create_import_list((yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 4:
/* Line 1787 of yacc.c  */
#line 121 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ImportList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 5:
/* Line 1787 of yacc.c  */
#line 122 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ImportList, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 6:
/* Line 1787 of yacc.c  */
#line 126 "compose.y"
    { (yyval.pAst) = ast_create_import_stmt((yyvsp[(2) - (3)].pAst), pParseData); }
    break;

  case 7:
/* Line 1787 of yacc.c  */
#line 130 "compose.y"
    { (yyval.pAst) = ast_append(kAST_DottedId, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 8:
/* Line 1787 of yacc.c  */
#line 131 "compose.y"
    { (yyval.pAst) = ast_append(kAST_DottedId, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 9:
/* Line 1787 of yacc.c  */
#line 135 "compose.y"
    { (yyval.pAst) = ast_create_with_str(kAST_DottedIdPart, (yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 12:
/* Line 1787 of yacc.c  */
#line 144 "compose.y"
    { (yyval.pAst) = ast_create_entity_def((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 145 "compose.y"
    { (yyval.pAst) = ast_create_component_def((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 146 "compose.y"
    { (yyval.pAst) = ast_create_function_def((yyvsp[(2) - (6)].str), (yyvsp[(1) - (6)].pAst), (yyvsp[(6) - (6)].pAst), pParseData); }
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 150 "compose.y"
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 151 "compose.y"
    { (yyval.pAst) = ast_create_block((yyvsp[(2) - (3)].pAst),   pParseData); }
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 155 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 156 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 160 "compose.y"
    { (yyval.pAst) = ast_create_message_def((yyvsp[(2) - (6)].str), (yyvsp[(6) - (6)].pAst), pParseData); }
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 161 "compose.y"
    { (yyval.pAst) = ast_create_property_def((yyvsp[(3) - (6)].str), (yyvsp[(1) - (6)].pAst), (yyvsp[(5) - (6)].pAst), pParseData); }
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 162 "compose.y"
    { (yyval.pAst) = ast_create_property_def((yyvsp[(3) - (4)].str), (yyvsp[(1) - (4)].pAst), NULL, pParseData); }
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 163 "compose.y"
    { (yyval.pAst) = ast_create_field_def((yyvsp[(2) - (5)].str), (yyvsp[(1) - (5)].pAst), (yyvsp[(4) - (5)].pAst), pParseData); }
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 164 "compose.y"
    { (yyval.pAst) = ast_create_field_def((yyvsp[(2) - (3)].str), (yyvsp[(1) - (3)].pAst), NULL, pParseData); }
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 165 "compose.y"
    { (yyval.pAst) = ast_create_component_members((yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 169 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, NULL); }
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 170 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, symrec_create(kSYMT_Param, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].str), NULL)); }
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 171 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, (yyvsp[(1) - (4)].pSymTab), symrec_create(kSYMT_Param, (yyvsp[(3) - (4)].pAst), (yyvsp[(4) - (4)].str), NULL)); }
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 175 "compose.y"
    { (yyval.pAst) = ast_create_with_child_list(kAST_ComponentMemberList, pParseData); }
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 176 "compose.y"
    { (yyval.pAst) = (yyvsp[(2) - (3)].pAst); }
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 180 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 181 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 185 "compose.y"
    { (yyval.pAst) = ast_create_component_member((yyvsp[(1) - (2)].str), ast_create(kAST_PropInit, pParseData), pParseData); }
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 186 "compose.y"
    { (yyval.pAst) = ast_create_component_member((yyvsp[(1) - (5)].str), (yyvsp[(3) - (5)].pAst), pParseData); }
    break;

  case 34:
/* Line 1787 of yacc.c  */
#line 190 "compose.y"
    { (yyval.pAst) = ast_create(kAST_PropInitList, pParseData); }
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 191 "compose.y"
    { (yyval.pAst) = ast_append(kAST_PropInitList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 192 "compose.y"
    { (yyval.pAst) = ast_append(kAST_PropInitList, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 196 "compose.y"
    { (yyval.pAst) = ast_create_prop_init((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 200 "compose.y"
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 201 "compose.y"
    { (yyval.pAst) = ast_create_block((yyvsp[(2) - (3)].pAst),   pParseData); }
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 205 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 206 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 210 "compose.y"
    { (yyval.pAst) = ast_create_if((yyvsp[(3) - (5)].pAst), (yyvsp[(5) - (5)].pAst), NULL, pParseData); }
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 211 "compose.y"
    { (yyval.pAst) = ast_create_if((yyvsp[(3) - (7)].pAst), (yyvsp[(5) - (7)].pAst), (yyvsp[(7) - (7)].pAst),   pParseData); }
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 213 "compose.y"
    { (yyval.pAst) = ast_create_while((yyvsp[(3) - (5)].pAst), (yyvsp[(5) - (5)].pAst), pParseData); }
    break;

  case 45:
/* Line 1787 of yacc.c  */
#line 214 "compose.y"
    { (yyval.pAst) = ast_create_dowhile((yyvsp[(5) - (7)].pAst), (yyvsp[(2) - (7)].pAst), pParseData); }
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 216 "compose.y"
    { (yyval.pAst) = ast_create_for((yyvsp[(3) - (9)].pAst), (yyvsp[(5) - (9)].pAst), (yyvsp[(7) - (9)].pAst), (yyvsp[(9) - (9)].pAst), pParseData); }
    break;

  case 47:
/* Line 1787 of yacc.c  */
#line 218 "compose.y"
    { (yyval.pAst) = ast_create_property_set((yyvsp[(2) - (7)].pAst), (yyvsp[(4) - (7)].str), (yyvsp[(6) - (7)].pAst), pParseData); }
    break;

  case 48:
/* Line 1787 of yacc.c  */
#line 219 "compose.y"
    { (yyval.pAst) = ast_create_message_send((yyvsp[(2) - (8)].pAst), (yyvsp[(4) - (8)].str), (yyvsp[(6) - (8)].pAst), pParseData); }
    break;

  case 49:
/* Line 1787 of yacc.c  */
#line 221 "compose.y"
    { (yyval.pAst) = ast_create_return((yyvsp[(2) - (3)].pAst), pParseData); }
    break;

  case 50:
/* Line 1787 of yacc.c  */
#line 223 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 225 "compose.y"
    { (yyval.pAst) = ast_create_simple_stmt((yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 229 "compose.y"
    { parsedata_handle_do_scope(pParseData); (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 53:
/* Line 1787 of yacc.c  */
#line 232 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 54:
/* Line 1787 of yacc.c  */
#line 233 "compose.y"
    { (yyval.pAst) = ast_create_identifier((yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 55:
/* Line 1787 of yacc.c  */
#line 234 "compose.y"
    { (yyval.pAst) = ast_create_int_literal((yyvsp[(1) - (1)].numi), pParseData); }
    break;

  case 56:
/* Line 1787 of yacc.c  */
#line 238 "compose.y"
    { (yyval.pAst) = (yyvsp[(2) - (3)].pAst); }
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 240 "compose.y"
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].str), NULL)); }
    break;

  case 58:
/* Line 1787 of yacc.c  */
#line 241 "compose.y"
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[(1) - (4)].pAst), (yyvsp[(2) - (4)].str), (yyvsp[(4) - (4)].pAst))); }
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 243 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 60:
/* Line 1787 of yacc.c  */
#line 245 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Add,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 61:
/* Line 1787 of yacc.c  */
#line 246 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Sub,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 62:
/* Line 1787 of yacc.c  */
#line 247 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Mul,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 63:
/* Line 1787 of yacc.c  */
#line 248 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Div,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 64:
/* Line 1787 of yacc.c  */
#line 249 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Mod,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 65:
/* Line 1787 of yacc.c  */
#line 250 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LShift, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 66:
/* Line 1787 of yacc.c  */
#line 251 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_RShift, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 67:
/* Line 1787 of yacc.c  */
#line 252 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Or,     (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 68:
/* Line 1787 of yacc.c  */
#line 253 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_And,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 69:
/* Line 1787 of yacc.c  */
#line 254 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitOr,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 70:
/* Line 1787 of yacc.c  */
#line 255 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitXor, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 71:
/* Line 1787 of yacc.c  */
#line 256 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitAnd, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 72:
/* Line 1787 of yacc.c  */
#line 258 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_Assign,       (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 73:
/* Line 1787 of yacc.c  */
#line 259 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_AddAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 74:
/* Line 1787 of yacc.c  */
#line 260 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_SubAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 75:
/* Line 1787 of yacc.c  */
#line 261 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_MulAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 76:
/* Line 1787 of yacc.c  */
#line 262 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_DivAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 77:
/* Line 1787 of yacc.c  */
#line 263 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_ModAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 78:
/* Line 1787 of yacc.c  */
#line 264 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_LShiftAssign, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 79:
/* Line 1787 of yacc.c  */
#line 265 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_RShiftAssign, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 80:
/* Line 1787 of yacc.c  */
#line 266 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_AndAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 81:
/* Line 1787 of yacc.c  */
#line 267 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_XorAssign,    (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 82:
/* Line 1787 of yacc.c  */
#line 268 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_OrAssign,     (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 83:
/* Line 1787 of yacc.c  */
#line 270 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Not,        (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 84:
/* Line 1787 of yacc.c  */
#line 271 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Complement, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 85:
/* Line 1787 of yacc.c  */
#line 272 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Complement, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 86:
/* Line 1787 of yacc.c  */
#line 274 "compose.y"
    { (yyval.pAst) = ast_create_hash((yyvsp[(2) - (2)].str), pParseData); }
    break;

  case 87:
/* Line 1787 of yacc.c  */
#line 276 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PreInc, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 88:
/* Line 1787 of yacc.c  */
#line 277 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PreDec, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 89:
/* Line 1787 of yacc.c  */
#line 278 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PostInc, (yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 90:
/* Line 1787 of yacc.c  */
#line 279 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PostDec, (yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 91:
/* Line 1787 of yacc.c  */
#line 281 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 92:
/* Line 1787 of yacc.c  */
#line 283 "compose.y"
    { (yyval.pAst) = ast_create_function_call((yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 93:
/* Line 1787 of yacc.c  */
#line 284 "compose.y"
    { (yyval.pAst) = ast_create_symbol_ref((yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 94:
/* Line 1787 of yacc.c  */
#line 289 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Eq,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 95:
/* Line 1787 of yacc.c  */
#line 290 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_NEq, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 96:
/* Line 1787 of yacc.c  */
#line 291 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LTE, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 97:
/* Line 1787 of yacc.c  */
#line 292 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_GTE, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 98:
/* Line 1787 of yacc.c  */
#line 293 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LT,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 99:
/* Line 1787 of yacc.c  */
#line 294 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_GT,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 100:
/* Line 1787 of yacc.c  */
#line 298 "compose.y"
    { (yyval.pAst) = ast_create_int_literal((yyvsp[(1) - (1)].numi), pParseData); }
    break;

  case 101:
/* Line 1787 of yacc.c  */
#line 299 "compose.y"
    { (yyval.pAst) = ast_create_float_literal((yyvsp[(1) - (1)].numf), pParseData); }
    break;

  case 102:
/* Line 1787 of yacc.c  */
#line 300 "compose.y"
    { (yyval.pAst) = ast_create_int_literal(1, pParseData); }
    break;

  case 103:
/* Line 1787 of yacc.c  */
#line 301 "compose.y"
    { (yyval.pAst) = ast_create_int_literal(0, pParseData); }
    break;

  case 104:
/* Line 1787 of yacc.c  */
#line 305 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 105:
/* Line 1787 of yacc.c  */
#line 306 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 106:
/* Line 1787 of yacc.c  */
#line 310 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 107:
/* Line 1787 of yacc.c  */
#line 311 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 108:
/* Line 1787 of yacc.c  */
#line 315 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, NULL, pParseData); }
    break;

  case 109:
/* Line 1787 of yacc.c  */
#line 316 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, (yyvsp[(1) - (1)].pAst),   pParseData); }
    break;

  case 110:
/* Line 1787 of yacc.c  */
#line 317 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, (yyvsp[(1) - (3)].pAst),   (yyvsp[(3) - (3)].pAst),   pParseData); }
    break;

  case 111:
/* Line 1787 of yacc.c  */
#line 321 "compose.y"
    { (yyval.pAst) = ast_create_with_numi(kAST_DataType, (yyvsp[(1) - (1)].dataType), pParseData); }
    break;

  case 112:
/* Line 1787 of yacc.c  */
#line 322 "compose.y"
    { (yyval.pAst) = ast_create_with_str(kAST_CustomType, (yyvsp[(1) - (1)].str), pParseData); }
    break;


/* Line 1787 of yacc.c  */
#line 2584 "compose_parser.c"
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
#line 347 "compose.y"



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

