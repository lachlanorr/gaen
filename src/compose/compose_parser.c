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
     TRUE_ = 262,
     FALSE_ = 263,
     FLOAT_LITERAL = 264,
     VOID_ = 265,
     BOOL_ = 266,
     CHAR_ = 267,
     BYTE_ = 268,
     SHORT_ = 269,
     USHORT_ = 270,
     INT_ = 271,
     UINT_ = 272,
     LONG_ = 273,
     ULONG_ = 274,
     HALF_ = 275,
     FLOAT_ = 276,
     DOUBLE_ = 277,
     COLOR = 278,
     VEC2 = 279,
     VEC3 = 280,
     VEC4 = 281,
     QUAT = 282,
     MAT3 = 283,
     MAT34 = 284,
     MAT4 = 285,
     HANDLE_ = 286,
     ENTITY = 287,
     STRING = 288,
     IF = 289,
     SWITCH = 290,
     CASE = 291,
     DEFAULT = 292,
     FOR = 293,
     WHILE = 294,
     DO = 295,
     BREAK = 296,
     RETURN = 297,
     COMPONENT = 298,
     COMPONENTS = 299,
     USING = 300,
     AS = 301,
     CONST_ = 302,
     THIS__ = 303,
     NONE = 304,
     THEN = 305,
     ELSE = 306,
     TRANSFORM = 307,
     OR_ASSIGN = 308,
     XOR_ASSIGN = 309,
     AND_ASSIGN = 310,
     RSHIFT_ASSIGN = 311,
     LSHIFT_ASSIGN = 312,
     MOD_ASSIGN = 313,
     DIV_ASSIGN = 314,
     MUL_ASSIGN = 315,
     SUB_ASSIGN = 316,
     ADD_ASSIGN = 317,
     OR = 318,
     AND = 319,
     NEQ = 320,
     EQ = 321,
     GTE = 322,
     LTE = 323,
     RSHIFT = 324,
     LSHIFT = 325,
     DEC = 326,
     INC = 327,
     UMINUS = 328,
     POSTDEC = 329,
     POSTINC = 330
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
    const SymDataType*     pSymDataType;


/* Line 387 of yacc.c  */
#line 208 "compose_parser.c"
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
#line 59 "compose.y"

#define YY_NO_UNISTD_H
#include "compose/compose_scanner.h"
#define YYLEX_PARAM parsedata_scanner(pParseData)

#define YYPRINT(file, type, value)   yyprint (file, type, value)
static void yyprint(FILE * file, int type, YYSTYPE value);

/* Line 390 of yacc.c  */
#line 257 "compose_parser.c"

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
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   873

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  128
/* YYNRULES -- Number of states.  */
#define YYNSTATES  279

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   330

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    82,     2,     2,    99,    81,    68,     2,
      90,    93,    79,    77,    97,    78,    87,    80,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    96,
      71,    52,    72,     2,    98,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    91,     2,    94,    67,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    92,    66,    95,    83,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      69,    70,    73,    74,    75,    76,    84,    85,    86,    88,
      89
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
     380,   385,   390,   395,   400,   407,   409,   413,   417,   421,
     425,   429,   433,   435,   437,   439,   441,   443,   444,   446,
     447,   449,   450,   452,   456,   459,   461,   463,   466
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     101,     0,    -1,   107,    -1,   102,   107,    -1,   103,    -1,
     102,   103,    -1,    45,   104,    46,   104,    96,    -1,   105,
      -1,   106,    -1,   104,    87,   106,    -1,     3,    -1,   108,
      -1,   107,   108,    -1,    32,     3,   109,    -1,    43,     3,
     109,    -1,   129,     3,    90,   112,    93,   118,    -1,    47,
      32,     3,    90,   112,    93,   118,    -1,    32,     3,    90,
     112,    93,   118,    -1,    92,    95,    -1,    92,   110,    95,
      -1,   111,    -1,   110,   111,    -1,     4,    90,   112,    93,
     118,    -1,   129,     4,    52,   123,    96,    -1,   129,     4,
      96,    -1,   129,     3,    52,   123,    96,    -1,   129,     3,
      96,    -1,    44,   113,    -1,    -1,   130,     3,    -1,   112,
      97,   130,     3,    -1,    92,    95,    -1,    92,   114,    95,
      -1,   115,    -1,   114,   115,    -1,   104,    96,    -1,   104,
      92,   116,    95,    96,    -1,    -1,   117,    -1,   116,    97,
     117,    -1,     3,    52,   123,    -1,    92,    95,    -1,    92,
     119,    95,    -1,   120,    -1,   119,   120,    -1,    34,    90,
     123,    93,   120,    -1,    34,    90,   123,    93,   120,    51,
     120,    -1,    39,    90,   123,    93,   120,    -1,    40,   121,
      39,    90,   123,    93,    96,    -1,    38,    90,   126,    96,
     127,    96,   126,    93,   120,    -1,    98,   122,     4,    52,
     123,    96,    -1,    98,   122,     4,    90,   128,    93,    96,
      -1,    42,   123,    96,    -1,   118,    -1,   123,    96,    -1,
     120,    -1,    -1,     3,    -1,     6,    -1,    90,   123,    93,
      -1,   130,     3,    -1,   130,     3,    52,   123,    -1,     5,
      -1,   124,    -1,   123,    77,   123,    -1,   123,    78,   123,
      -1,   123,    79,   123,    -1,   123,    80,   123,    -1,   123,
      81,   123,    -1,   123,    76,   123,    -1,   123,    75,   123,
      -1,   123,    64,   123,    -1,   123,    65,   123,    -1,   123,
      66,   123,    -1,   123,    67,   123,    -1,   123,    68,   123,
      -1,   104,    52,   123,    -1,   104,    63,   123,    -1,   104,
      62,   123,    -1,   104,    61,   123,    -1,   104,    60,   123,
      -1,   104,    59,   123,    -1,   104,    58,   123,    -1,   104,
      57,   123,    -1,   104,    56,   123,    -1,   104,    55,   123,
      -1,   104,    54,   123,    -1,    82,   123,    -1,    83,   123,
      -1,    78,   123,    -1,     4,    -1,    85,   123,    -1,    84,
     123,    -1,   123,    85,    -1,   123,    84,    -1,   125,    -1,
      23,    92,   128,    95,    -1,    25,    92,   128,    95,    -1,
      26,    92,   128,    95,    -1,    27,    92,   128,    95,    -1,
      29,    92,   128,    95,    -1,    33,    92,   128,    95,    -1,
     104,    92,   128,    95,    -1,   104,    90,   128,    93,    -1,
      99,    87,     3,    90,   128,    93,    -1,    53,    -1,   123,
      70,   123,    -1,   123,    69,   123,    -1,   123,    74,   123,
      -1,   123,    73,   123,    -1,   123,    71,   123,    -1,   123,
      72,   123,    -1,   104,    -1,     6,    -1,     9,    -1,     7,
      -1,     8,    -1,    -1,   123,    -1,    -1,   124,    -1,    -1,
     123,    -1,   128,    97,   123,    -1,    47,   104,    -1,   104,
      -1,   129,    -1,    47,    32,    -1,    32,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   114,   114,   115,   119,   120,   124,   128,   132,   133,
     137,   141,   142,   147,   148,   149,   150,   151,   155,   156,
     160,   161,   165,   166,   167,   168,   169,   170,   174,   175,
     176,   180,   181,   185,   186,   190,   191,   195,   196,   197,
     201,   205,   206,   210,   211,   215,   216,   218,   219,   221,
     223,   224,   226,   228,   230,   234,   237,   238,   239,   243,
     245,   246,   248,   250,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   277,   278,   279,
     281,   283,   284,   285,   286,   288,   290,   291,   292,   293,
     294,   295,   297,   298,   300,   302,   306,   307,   308,   309,
     310,   311,   312,   316,   317,   318,   319,   323,   324,   328,
     329,   333,   334,   335,   339,   340,   344,   345,   346
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "HASH", "STRING_LITERAL",
  "INT_LITERAL", "TRUE_", "FALSE_", "FLOAT_LITERAL", "VOID_", "BOOL_",
  "CHAR_", "BYTE_", "SHORT_", "USHORT_", "INT_", "UINT_", "LONG_",
  "ULONG_", "HALF_", "FLOAT_", "DOUBLE_", "COLOR", "VEC2", "VEC3", "VEC4",
  "QUAT", "MAT3", "MAT34", "MAT4", "HANDLE_", "ENTITY", "STRING", "IF",
  "SWITCH", "CASE", "DEFAULT", "FOR", "WHILE", "DO", "BREAK", "RETURN",
  "COMPONENT", "COMPONENTS", "USING", "AS", "CONST_", "THIS__", "NONE",
  "THEN", "ELSE", "'='", "TRANSFORM", "OR_ASSIGN", "XOR_ASSIGN",
  "AND_ASSIGN", "RSHIFT_ASSIGN", "LSHIFT_ASSIGN", "MOD_ASSIGN",
  "DIV_ASSIGN", "MUL_ASSIGN", "SUB_ASSIGN", "ADD_ASSIGN", "OR", "AND",
  "'|'", "'^'", "'&'", "NEQ", "EQ", "'<'", "'>'", "GTE", "LTE", "RSHIFT",
  "LSHIFT", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "DEC", "INC",
  "UMINUS", "'.'", "POSTDEC", "POSTINC", "'('", "'['", "'{'", "')'", "']'",
  "'}'", "';'", "','", "'@'", "'$'", "$accept", "cmp_file", "using_list",
  "using_stmt", "dotted_id", "dotted_id_proc", "dotted_id_part",
  "def_list", "def", "message_block", "message_list", "message_prop",
  "param_list", "component_block", "component_member_list",
  "component_member", "prop_init_list", "prop_init", "block", "stmt_list",
  "stmt", "do_stmt", "target_expr", "expr", "cond_expr", "literal",
  "expr_or_empty", "cond_expr_or_empty", "fun_params", "type", "type_ent", YY_NULL
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
     305,   306,    61,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   124,    94,    38,   320,
     321,    60,    62,   322,   323,   324,   325,    43,    45,    42,
      47,    37,    33,   126,   326,   327,   328,    46,   329,   330,
      40,    91,   123,    41,    93,   125,    59,    44,    64,    36
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   100,   101,   101,   102,   102,   103,   104,   105,   105,
     106,   107,   107,   108,   108,   108,   108,   108,   109,   109,
     110,   110,   111,   111,   111,   111,   111,   111,   112,   112,
     112,   113,   113,   114,   114,   115,   115,   116,   116,   116,
     117,   118,   118,   119,   119,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   121,   122,   122,   122,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   124,   124,   124,   124,
     124,   124,   124,   125,   125,   125,   125,   126,   126,   127,
     127,   128,   128,   128,   129,   129,   130,   130,   130
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
       4,     4,     4,     4,     6,     1,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     0,     1,     0,
       1,     0,     1,     3,     2,     1,     1,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    10,     0,     0,     0,     0,     0,     0,     4,   125,
       7,     8,     2,    11,     0,     0,     0,     0,     0,   124,
       1,     5,     3,     0,    12,     0,    28,     0,    13,    14,
       0,     0,     9,    28,   128,     0,     0,   126,     0,     0,
       0,     0,    18,     0,    20,     0,     0,    28,     0,   127,
       0,     0,    29,    28,     0,    27,    19,    21,     0,     0,
       6,     0,     0,     0,    17,     0,     0,    31,     0,     0,
      33,     0,    26,     0,    24,     0,    15,    90,    62,   113,
     115,   116,   114,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,    41,    56,     0,   112,    53,     0,    43,     0,    63,
      95,     0,    30,     0,    37,    35,    32,    34,     0,     0,
      16,   121,   121,   121,   121,   121,   121,     0,   117,     0,
      55,     0,     0,    89,    87,    88,    92,    91,     0,    57,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   121,    42,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    93,    54,    60,
      22,     0,     0,    38,    25,    23,   122,     0,     0,     0,
       0,     0,     0,     0,   118,     0,     0,     0,    52,    59,
       0,     0,    76,    86,    85,    84,    83,    82,    81,    80,
      79,    78,    77,     0,     0,    71,    72,    73,    74,    75,
     107,   106,   110,   111,   109,   108,    70,    69,    64,    65,
      66,    67,    68,     0,     0,     0,     0,    96,     0,    97,
      98,    99,   100,   101,     0,   119,     0,     0,     0,   121,
     121,   103,   102,    61,    40,    36,    39,   123,    45,     0,
      63,     0,    47,     0,     0,     0,     0,     0,   117,     0,
      50,     0,   104,    46,     0,    48,    51,     0,    49
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     6,     7,     8,   104,    10,    11,    12,    13,    28,
      43,    44,    36,    55,    69,    70,   182,   183,   105,   106,
     107,   131,   141,   186,   109,   110,   195,   261,   187,    37,
     111
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -107
static const yytype_int16 yypact[] =
{
      -1,  -107,     3,    57,    69,    25,    77,    -1,  -107,    37,
    -107,  -107,     9,  -107,   153,   -85,    81,   -42,   158,    37,
    -107,  -107,     9,    69,  -107,    79,     8,    23,  -107,  -107,
      69,    84,  -107,     8,  -107,    36,   -35,  -107,   177,    97,
      96,    69,  -107,    47,  -107,    89,   -33,     8,   -22,  -107,
     103,     8,  -107,     8,    18,  -107,  -107,  -107,   -49,   -43,
    -107,    22,   103,   125,  -107,   196,    24,  -107,   -18,    19,
    -107,   387,  -107,   387,  -107,   103,  -107,  -107,  -107,  -107,
    -107,  -107,  -107,   114,   120,   121,   126,   130,   144,   147,
     156,   160,   319,   387,  -107,   387,   387,   387,   387,   387,
     387,  -107,   143,   157,   277,  -107,   222,  -107,   217,  -107,
    -107,   249,  -107,   103,   250,  -107,  -107,  -107,   458,   491,
    -107,   387,   387,   387,   387,   387,   387,   387,   387,   387,
    -107,   218,   524,  -107,    53,    53,    53,    53,   590,  -107,
    -107,   112,   255,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,  -107,  -107,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,  -107,  -107,  -107,   207,
    -107,   211,    28,  -107,  -107,  -107,   710,    65,    71,    82,
     105,   124,   145,   620,   710,   169,   650,   176,  -107,  -107,
     -29,   178,   710,   710,   710,   710,   710,   710,   710,   710,
     710,   710,   710,    29,   146,   731,   751,   770,   788,   304,
     352,   352,   370,   370,   370,   370,   113,   113,   154,   154,
      53,    53,    53,   387,   387,   171,   250,  -107,   387,  -107,
    -107,  -107,  -107,  -107,   319,   387,   319,   387,   387,   387,
     387,  -107,  -107,   710,   710,  -107,  -107,   710,   219,   710,
     175,   180,  -107,   680,   557,    42,    43,   319,   387,   181,
    -107,   182,  -107,  -107,   179,  -107,  -107,   319,  -107
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -107,  -107,  -107,   266,   452,  -107,   251,   272,     2,   283,
    -107,   260,    26,  -107,  -107,   239,  -107,    73,   -37,  -107,
     -91,  -107,  -107,   -63,    66,  -107,    48,  -107,  -106,   189,
      94
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -126
static const yytype_int16 yytable[] =
{
     108,   130,     1,    71,    30,    26,    15,    27,   118,    73,
     119,     1,     1,    64,    24,   157,   188,   189,   190,   191,
     192,     1,     1,   248,    24,    76,     1,    39,     1,   108,
     132,     2,   133,   134,   135,   136,   137,   138,   120,     1,
      34,     2,     3,   108,     4,    23,     5,    72,   213,   214,
       1,    39,     3,    74,    23,    35,     5,    18,    50,    48,
      16,   249,    51,    60,   193,   194,   196,    40,    49,    23,
      41,    62,     1,    61,   114,    51,   180,    20,   115,    66,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,    40,    58,    59,    41,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,    67,   116,    75,   200,   113,    42,    51,
      38,    51,   251,   235,    23,   236,   238,    38,     1,    77,
      78,    79,    80,    81,    82,   271,   272,   176,   177,   238,
     238,    38,    56,   265,   266,    65,   139,    38,    83,   140,
      84,    85,    86,   258,    87,   262,    25,    34,    88,    89,
     237,    31,   238,    90,    91,    92,   239,    93,   238,    33,
     253,   254,    35,    27,    47,   257,   273,   240,    94,   238,
      52,   108,   259,   108,   263,   264,   278,    53,    54,    14,
     171,   172,   173,   174,   175,    63,    14,   176,   177,   112,
     241,    14,   238,    95,   108,   194,   121,    96,    97,    98,
      99,    14,   122,   123,   108,   100,    45,    63,   124,   242,
     101,   238,   125,   102,   103,     1,    77,    78,    79,    80,
      81,    82,    45,   173,   174,   175,   126,   127,   176,   177,
     243,   252,   238,   238,   142,    83,   128,    84,    85,    86,
     129,    87,   179,   181,    34,    88,    89,   197,   201,   233,
      90,    91,    92,   234,    93,   245,   247,   255,   250,    35,
     267,  -120,   277,    21,    32,    94,   268,   275,   276,    22,
    -125,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,    29,
      95,   176,   177,    57,    96,    97,    98,    99,   117,   256,
       0,   260,   100,   178,    63,     0,   274,   156,     0,     0,
     102,   103,     1,    77,    78,    79,    80,    81,    82,   143,
       0,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,     0,    83,     0,    84,    85,    86,     0,    87,     0,
       0,    34,    88,    89,     0,     0,     0,    90,    91,    92,
       0,    93,     0,     0,    23,     0,    35,   154,     0,   155,
       0,     0,    94,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,     0,   176,   177,
       1,    77,    78,    79,    80,    81,    82,    95,     0,     0,
       0,    96,    97,    98,    99,     0,     0,     0,     0,   100,
      83,    63,    84,    85,    86,     0,    87,   102,   103,    34,
      88,  -126,  -126,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,    35,     0,   176,   177,     0,     0,
      94,  -126,  -126,  -126,  -126,   169,   170,   171,   172,   173,
     174,   175,     9,     0,   176,   177,    17,    19,     0,     9,
       0,     0,     0,     0,     9,    95,     0,     0,     0,    96,
      97,    98,    99,     0,     9,     0,     0,   100,     9,     9,
       0,     0,    46,     0,     0,     9,   103,    19,     0,     0,
       0,     0,     0,    19,     0,     9,     0,     0,     0,     9,
       0,     0,     0,     9,     0,     9,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,     0,   176,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,     0,   176,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   185,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,     0,   176,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     198,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,     0,
       0,   176,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,     0,   176,   177,     0,     0,     0,     0,
       0,     0,     0,   199,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,     0,   176,   177,     0,     0,     0,     0,
       0,     0,     0,   244,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,     0,   176,   177,     0,     0,     0,     0,
       0,     0,     0,   246,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,     0,   176,   177,     0,     0,     0,     0,
       0,     0,     0,   269,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,     0,   176,   177,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,     0,   176,   177,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,     0,   176,   177,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,     0,   176,   177,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,     0,   176,   177
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-107)))

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-126)))

static const yytype_int16 yycheck[] =
{
      63,    92,     3,    52,    46,    90,     3,    92,    71,    52,
      73,     3,     3,    50,    12,   106,   122,   123,   124,   125,
     126,     3,     3,    52,    22,    62,     3,     4,     3,    92,
      93,    32,    95,    96,    97,    98,    99,   100,    75,     3,
      32,    32,    43,   106,    45,    87,    47,    96,   154,   155,
       3,     4,    43,    96,    87,    47,    47,    32,    93,    33,
       3,    90,    97,    96,   127,   128,   129,    44,    32,    87,
      47,    93,     3,    47,    92,    97,   113,     0,    96,    53,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    44,     3,     4,    47,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,    95,    95,    93,     4,    93,    95,    97,
      26,    97,    93,    95,    87,    97,    97,    33,     3,     4,
       5,     6,     7,     8,     9,    93,    93,    84,    85,    97,
      97,    47,    95,   249,   250,    51,     3,    53,    23,     6,
      25,    26,    27,   244,    29,   246,     3,    32,    33,    34,
      95,     3,    97,    38,    39,    40,    95,    42,    97,    90,
     233,   234,    47,    92,    90,   238,   267,    95,    53,    97,
       3,   244,   245,   246,   247,   248,   277,    90,    92,     0,
      77,    78,    79,    80,    81,    92,     7,    84,    85,     3,
      95,    12,    97,    78,   267,   268,    92,    82,    83,    84,
      85,    22,    92,    92,   277,    90,    27,    92,    92,    95,
      95,    97,    92,    98,    99,     3,     4,     5,     6,     7,
       8,     9,    43,    79,    80,    81,    92,    90,    84,    85,
      95,    95,    97,    97,    87,    23,    90,    25,    26,    27,
      90,    29,     3,     3,    32,    33,    34,    39,     3,    52,
      38,    39,    40,    52,    42,    96,    90,    96,    90,    47,
      51,    96,    93,     7,    23,    53,    96,    96,    96,     7,
       3,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    16,
      78,    84,    85,    43,    82,    83,    84,    85,    69,   236,
      -1,   245,    90,    96,    92,    -1,   268,    95,    -1,    -1,
      98,    99,     3,     4,     5,     6,     7,     8,     9,    52,
      -1,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    23,    -1,    25,    26,    27,    -1,    29,    -1,
      -1,    32,    33,    34,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    -1,    -1,    87,    -1,    47,    90,    -1,    92,
      -1,    -1,    53,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    -1,    84,    85,
       3,     4,     5,     6,     7,     8,     9,    78,    -1,    -1,
      -1,    82,    83,    84,    85,    -1,    -1,    -1,    -1,    90,
      23,    92,    25,    26,    27,    -1,    29,    98,    99,    32,
      33,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    47,    -1,    84,    85,    -1,    -1,
      53,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,     0,    -1,    84,    85,     4,     5,    -1,     7,
      -1,    -1,    -1,    -1,    12,    78,    -1,    -1,    -1,    82,
      83,    84,    85,    -1,    22,    -1,    -1,    90,    26,    27,
      -1,    -1,    30,    -1,    -1,    33,    99,    35,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    -1,    84,    85,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    -1,    -1,    84,    85,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    -1,    -1,    84,    85,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    -1,    84,    85,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,    -1,    84,    85
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    32,    43,    45,    47,   101,   102,   103,   104,
     105,   106,   107,   108,   129,     3,     3,   104,    32,   104,
       0,   103,   107,    87,   108,     3,    90,    92,   109,   109,
      46,     3,   106,    90,    32,    47,   112,   129,   130,     4,
      44,    47,    95,   110,   111,   129,   104,    90,   112,    32,
      93,    97,     3,    90,    92,   113,    95,   111,     3,     4,
      96,   112,    93,    92,   118,   130,   112,    95,   104,   114,
     115,    52,    96,    52,    96,    93,   118,     4,     5,     6,
       7,     8,     9,    23,    25,    26,    27,    29,    33,    34,
      38,    39,    40,    42,    53,    78,    82,    83,    84,    85,
      90,    95,    98,    99,   104,   118,   119,   120,   123,   124,
     125,   130,     3,    93,    92,    96,    95,   115,   123,   123,
     118,    92,    92,    92,    92,    92,    92,    90,    90,    90,
     120,   121,   123,   123,   123,   123,   123,   123,   123,     3,
       6,   122,    87,    52,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    90,    92,    95,   120,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    84,    85,    96,     3,
     118,     3,   116,   117,    96,    96,   123,   128,   128,   128,
     128,   128,   128,   123,   123,   126,   123,    39,    96,    93,
       4,     3,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   128,   128,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,    52,    52,    95,    97,    95,    97,    95,
      95,    95,    95,    95,    93,    96,    93,    90,    52,    90,
      90,    93,    95,   123,   123,    96,   117,   123,   120,   123,
     124,   127,   120,   123,   123,   128,   128,    51,    96,    93,
      96,    93,    93,   120,   126,    96,    96,    93,   120
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
#line 115 "compose.y"
    { ast_create_using_list((yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 4:
/* Line 1787 of yacc.c  */
#line 119 "compose.y"
    { (yyval.pAst) = ast_append(kAST_UsingList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 5:
/* Line 1787 of yacc.c  */
#line 120 "compose.y"
    { (yyval.pAst) = ast_append(kAST_UsingList, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 6:
/* Line 1787 of yacc.c  */
#line 124 "compose.y"
    { (yyval.pAst) = ast_create_using_stmt((yyvsp[(2) - (5)].pAst), (yyvsp[(4) - (5)].pAst), pParseData); }
    break;

  case 7:
/* Line 1787 of yacc.c  */
#line 128 "compose.y"
    { (yyval.pAst) = ast_create_dotted_id((yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 8:
/* Line 1787 of yacc.c  */
#line 132 "compose.y"
    { (yyval.pAst) = ast_append(kAST_DottedId, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 9:
/* Line 1787 of yacc.c  */
#line 133 "compose.y"
    { (yyval.pAst) = ast_append(kAST_DottedId, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 10:
/* Line 1787 of yacc.c  */
#line 137 "compose.y"
    { (yyval.pAst) = ast_create_with_str(kAST_DottedIdPart, (yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 147 "compose.y"
    { (yyval.pAst) = ast_create_entity_def((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 148 "compose.y"
    { (yyval.pAst) = ast_create_component_def((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 149 "compose.y"
    { (yyval.pAst) = ast_create_function_def((yyvsp[(2) - (6)].str), (yyvsp[(1) - (6)].pSymDataType), (yyvsp[(6) - (6)].pAst), pParseData); }
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 150 "compose.y"
    { (yyval.pAst) = ast_create_function_def((yyvsp[(3) - (7)].str), parsedata_find_type(pParseData, "entity", 1, 0), (yyvsp[(7) - (7)].pAst), pParseData); }
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 151 "compose.y"
    { (yyval.pAst) = ast_create_function_def((yyvsp[(2) - (6)].str), parsedata_find_type(pParseData, "entity", 0, 0), (yyvsp[(6) - (6)].pAst), pParseData); }
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 155 "compose.y"
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 156 "compose.y"
    { (yyval.pAst) = ast_create_block((yyvsp[(2) - (3)].pAst),   pParseData); }
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 160 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 161 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 165 "compose.y"
    { (yyval.pAst) = ast_create_message_def((yyvsp[(1) - (5)].str), (yyvsp[(3) - (5)].pSymTab), (yyvsp[(5) - (5)].pAst), pParseData); }
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 166 "compose.y"
    { (yyval.pAst) = ast_create_property_def((yyvsp[(2) - (5)].str), (yyvsp[(1) - (5)].pSymDataType), (yyvsp[(4) - (5)].pAst), pParseData); }
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 167 "compose.y"
    { (yyval.pAst) = ast_create_property_def((yyvsp[(2) - (3)].str), (yyvsp[(1) - (3)].pSymDataType), NULL, pParseData); }
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 168 "compose.y"
    { (yyval.pAst) = ast_create_field_def((yyvsp[(2) - (5)].str), (yyvsp[(1) - (5)].pSymDataType), (yyvsp[(4) - (5)].pAst), pParseData); }
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 169 "compose.y"
    { (yyval.pAst) = ast_create_field_def((yyvsp[(2) - (3)].str), (yyvsp[(1) - (3)].pSymDataType), NULL, pParseData); }
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 170 "compose.y"
    { (yyval.pAst) = ast_create_component_members((yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 174 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, NULL); }
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 175 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, symrec_create(kSYMT_Param, (yyvsp[(1) - (2)].pSymDataType), (yyvsp[(2) - (2)].str), NULL, pParseData)); }
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 176 "compose.y"
    { (yyval.pSymTab) = parsedata_add_param(pParseData, (yyvsp[(1) - (4)].pSymTab), symrec_create(kSYMT_Param, (yyvsp[(3) - (4)].pSymDataType), (yyvsp[(4) - (4)].str), NULL, pParseData)); }
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 180 "compose.y"
    { (yyval.pAst) = ast_create_with_child_list(kAST_ComponentMemberList, pParseData); }
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 181 "compose.y"
    { (yyval.pAst) = (yyvsp[(2) - (3)].pAst); }
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 185 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 34:
/* Line 1787 of yacc.c  */
#line 186 "compose.y"
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 190 "compose.y"
    { (yyval.pAst) = ast_create_component_member((yyvsp[(1) - (2)].pAst), ast_create(kAST_PropInit, pParseData), pParseData); }
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 191 "compose.y"
    { (yyval.pAst) = ast_create_component_member((yyvsp[(1) - (5)].pAst), (yyvsp[(3) - (5)].pAst), pParseData); }
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 195 "compose.y"
    { (yyval.pAst) = ast_create(kAST_PropInitList, pParseData); }
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 196 "compose.y"
    { (yyval.pAst) = ast_append(kAST_PropInitList, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 197 "compose.y"
    { (yyval.pAst) = ast_append(kAST_PropInitList, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 201 "compose.y"
    { (yyval.pAst) = ast_create_prop_init((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 205 "compose.y"
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 206 "compose.y"
    { (yyval.pAst) = ast_create_block((yyvsp[(2) - (3)].pAst),   pParseData); }
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 210 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 211 "compose.y"
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[(1) - (2)].pAst), (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 45:
/* Line 1787 of yacc.c  */
#line 215 "compose.y"
    { (yyval.pAst) = ast_create_if((yyvsp[(3) - (5)].pAst), (yyvsp[(5) - (5)].pAst), NULL, pParseData); }
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 216 "compose.y"
    { (yyval.pAst) = ast_create_if((yyvsp[(3) - (7)].pAst), (yyvsp[(5) - (7)].pAst), (yyvsp[(7) - (7)].pAst),   pParseData); }
    break;

  case 47:
/* Line 1787 of yacc.c  */
#line 218 "compose.y"
    { (yyval.pAst) = ast_create_while((yyvsp[(3) - (5)].pAst), (yyvsp[(5) - (5)].pAst), pParseData); }
    break;

  case 48:
/* Line 1787 of yacc.c  */
#line 219 "compose.y"
    { (yyval.pAst) = ast_create_dowhile((yyvsp[(5) - (7)].pAst), (yyvsp[(2) - (7)].pAst), pParseData); }
    break;

  case 49:
/* Line 1787 of yacc.c  */
#line 221 "compose.y"
    { (yyval.pAst) = ast_create_for((yyvsp[(3) - (9)].pAst), (yyvsp[(5) - (9)].pAst), (yyvsp[(7) - (9)].pAst), (yyvsp[(9) - (9)].pAst), pParseData); }
    break;

  case 50:
/* Line 1787 of yacc.c  */
#line 223 "compose.y"
    { (yyval.pAst) = ast_create_property_set((yyvsp[(2) - (6)].pAst), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].pAst), pParseData); }
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 224 "compose.y"
    { (yyval.pAst) = ast_create_message_send((yyvsp[(2) - (7)].pAst), (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].pAst), pParseData); }
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 226 "compose.y"
    { (yyval.pAst) = ast_create_return((yyvsp[(2) - (3)].pAst), pParseData); }
    break;

  case 53:
/* Line 1787 of yacc.c  */
#line 228 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 54:
/* Line 1787 of yacc.c  */
#line 230 "compose.y"
    { (yyval.pAst) = ast_create_simple_stmt((yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 55:
/* Line 1787 of yacc.c  */
#line 234 "compose.y"
    { parsedata_handle_do_scope(pParseData); (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 56:
/* Line 1787 of yacc.c  */
#line 237 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 238 "compose.y"
    { (yyval.pAst) = ast_create_identifier((yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 58:
/* Line 1787 of yacc.c  */
#line 239 "compose.y"
    { (yyval.pAst) = ast_create_int_literal((yyvsp[(1) - (1)].numi), pParseData); }
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 243 "compose.y"
    { (yyval.pAst) = (yyvsp[(2) - (3)].pAst); }
    break;

  case 60:
/* Line 1787 of yacc.c  */
#line 245 "compose.y"
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[(1) - (2)].pSymDataType), (yyvsp[(2) - (2)].str), NULL, pParseData)); }
    break;

  case 61:
/* Line 1787 of yacc.c  */
#line 246 "compose.y"
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[(1) - (4)].pSymDataType), (yyvsp[(2) - (4)].str), (yyvsp[(4) - (4)].pAst), pParseData)); }
    break;

  case 62:
/* Line 1787 of yacc.c  */
#line 248 "compose.y"
    { (yyval.pAst) = ast_create_string_literal((yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 63:
/* Line 1787 of yacc.c  */
#line 250 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 64:
/* Line 1787 of yacc.c  */
#line 252 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Add,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 65:
/* Line 1787 of yacc.c  */
#line 253 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Sub,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 66:
/* Line 1787 of yacc.c  */
#line 254 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Mul,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 67:
/* Line 1787 of yacc.c  */
#line 255 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Div,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 68:
/* Line 1787 of yacc.c  */
#line 256 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Mod,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 69:
/* Line 1787 of yacc.c  */
#line 257 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LShift, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 70:
/* Line 1787 of yacc.c  */
#line 258 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_RShift, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 71:
/* Line 1787 of yacc.c  */
#line 259 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Or,     (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 72:
/* Line 1787 of yacc.c  */
#line 260 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_And,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 73:
/* Line 1787 of yacc.c  */
#line 261 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitOr,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 74:
/* Line 1787 of yacc.c  */
#line 262 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitXor, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 75:
/* Line 1787 of yacc.c  */
#line 263 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_BitAnd, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 76:
/* Line 1787 of yacc.c  */
#line 265 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_Assign,       (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 77:
/* Line 1787 of yacc.c  */
#line 266 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_AddAssign,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 78:
/* Line 1787 of yacc.c  */
#line 267 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_SubAssign,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 79:
/* Line 1787 of yacc.c  */
#line 268 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_MulAssign,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 80:
/* Line 1787 of yacc.c  */
#line 269 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_DivAssign,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 81:
/* Line 1787 of yacc.c  */
#line 270 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_ModAssign,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 82:
/* Line 1787 of yacc.c  */
#line 271 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_LShiftAssign, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 83:
/* Line 1787 of yacc.c  */
#line 272 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_RShiftAssign, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 84:
/* Line 1787 of yacc.c  */
#line 273 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_AndAssign,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 85:
/* Line 1787 of yacc.c  */
#line 274 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_XorAssign,    (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 86:
/* Line 1787 of yacc.c  */
#line 275 "compose.y"
    { (yyval.pAst) = ast_create_assign_op(kAST_OrAssign,     (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 87:
/* Line 1787 of yacc.c  */
#line 277 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Not,        (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 88:
/* Line 1787 of yacc.c  */
#line 278 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Complement, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 89:
/* Line 1787 of yacc.c  */
#line 279 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_Negate,     (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 90:
/* Line 1787 of yacc.c  */
#line 281 "compose.y"
    { (yyval.pAst) = ast_create_hash((yyvsp[(1) - (1)].str), pParseData); }
    break;

  case 91:
/* Line 1787 of yacc.c  */
#line 283 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PreInc, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 92:
/* Line 1787 of yacc.c  */
#line 284 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PreDec, (yyvsp[(2) - (2)].pAst), pParseData); }
    break;

  case 93:
/* Line 1787 of yacc.c  */
#line 285 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PostInc, (yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 94:
/* Line 1787 of yacc.c  */
#line 286 "compose.y"
    { (yyval.pAst) = ast_create_unary_op(kAST_PostDec, (yyvsp[(1) - (2)].pAst), pParseData); }
    break;

  case 95:
/* Line 1787 of yacc.c  */
#line 288 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 96:
/* Line 1787 of yacc.c  */
#line 290 "compose.y"
    { (yyval.pAst) = ast_create_color_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 97:
/* Line 1787 of yacc.c  */
#line 291 "compose.y"
    { (yyval.pAst) = ast_create_vec3_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 98:
/* Line 1787 of yacc.c  */
#line 292 "compose.y"
    { (yyval.pAst) = ast_create_vec4_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 99:
/* Line 1787 of yacc.c  */
#line 293 "compose.y"
    { (yyval.pAst) = ast_create_quat_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 100:
/* Line 1787 of yacc.c  */
#line 294 "compose.y"
    { (yyval.pAst) = ast_create_mat34_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 101:
/* Line 1787 of yacc.c  */
#line 295 "compose.y"
    { (yyval.pAst) = ast_create_string_init((yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 102:
/* Line 1787 of yacc.c  */
#line 297 "compose.y"
    { (yyval.pAst) = ast_create_entity_or_struct_init((yyvsp[(1) - (4)].pAst), (yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 103:
/* Line 1787 of yacc.c  */
#line 298 "compose.y"
    { (yyval.pAst) = ast_create_function_call((yyvsp[(1) - (4)].pAst), (yyvsp[(3) - (4)].pAst), pParseData); }
    break;

  case 104:
/* Line 1787 of yacc.c  */
#line 300 "compose.y"
    { (yyval.pAst) = ast_create_system_api_call((yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].pAst), pParseData); }
    break;

  case 105:
/* Line 1787 of yacc.c  */
#line 302 "compose.y"
    { (yyval.pAst) = ast_create(kAST_Transform, pParseData); }
    break;

  case 106:
/* Line 1787 of yacc.c  */
#line 306 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_Eq,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 107:
/* Line 1787 of yacc.c  */
#line 307 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_NEq, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 108:
/* Line 1787 of yacc.c  */
#line 308 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LTE, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 109:
/* Line 1787 of yacc.c  */
#line 309 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_GTE, (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 110:
/* Line 1787 of yacc.c  */
#line 310 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_LT,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 111:
/* Line 1787 of yacc.c  */
#line 311 "compose.y"
    { (yyval.pAst) = ast_create_binary_op(kAST_GT,  (yyvsp[(1) - (3)].pAst), (yyvsp[(3) - (3)].pAst), pParseData); }
    break;

  case 112:
/* Line 1787 of yacc.c  */
#line 312 "compose.y"
    { (yyval.pAst) = ast_create_symbol_ref((yyvsp[(1) - (1)].pAst), pParseData); }
    break;

  case 113:
/* Line 1787 of yacc.c  */
#line 316 "compose.y"
    { (yyval.pAst) = ast_create_int_literal((yyvsp[(1) - (1)].numi), pParseData); }
    break;

  case 114:
/* Line 1787 of yacc.c  */
#line 317 "compose.y"
    { (yyval.pAst) = ast_create_float_literal((yyvsp[(1) - (1)].numf), pParseData); }
    break;

  case 115:
/* Line 1787 of yacc.c  */
#line 318 "compose.y"
    { (yyval.pAst) = ast_create_int_literal(1, pParseData); }
    break;

  case 116:
/* Line 1787 of yacc.c  */
#line 319 "compose.y"
    { (yyval.pAst) = ast_create_int_literal(0, pParseData); }
    break;

  case 117:
/* Line 1787 of yacc.c  */
#line 323 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 118:
/* Line 1787 of yacc.c  */
#line 324 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 119:
/* Line 1787 of yacc.c  */
#line 328 "compose.y"
    { (yyval.pAst) = NULL; }
    break;

  case 120:
/* Line 1787 of yacc.c  */
#line 329 "compose.y"
    { (yyval.pAst) = (yyvsp[(1) - (1)].pAst); }
    break;

  case 121:
/* Line 1787 of yacc.c  */
#line 333 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, NULL, pParseData); }
    break;

  case 122:
/* Line 1787 of yacc.c  */
#line 334 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, (yyvsp[(1) - (1)].pAst),   pParseData); }
    break;

  case 123:
/* Line 1787 of yacc.c  */
#line 335 "compose.y"
    { (yyval.pAst) = ast_append(kAST_FunctionParams, (yyvsp[(1) - (3)].pAst),   (yyvsp[(3) - (3)].pAst),   pParseData); }
    break;

  case 124:
/* Line 1787 of yacc.c  */
#line 339 "compose.y"
    { (yyval.pSymDataType) = parsedata_find_type_from_dotted_id(pParseData, (yyvsp[(2) - (2)].pAst), 1, 0); }
    break;

  case 125:
/* Line 1787 of yacc.c  */
#line 340 "compose.y"
    { (yyval.pSymDataType) = parsedata_find_type_from_dotted_id(pParseData, (yyvsp[(1) - (1)].pAst), 0, 0); }
    break;

  case 126:
/* Line 1787 of yacc.c  */
#line 344 "compose.y"
    { (yyval.pSymDataType) = (yyvsp[(1) - (1)].pSymDataType); }
    break;

  case 127:
/* Line 1787 of yacc.c  */
#line 345 "compose.y"
    { (yyval.pSymDataType) = parsedata_find_type(pParseData, "entity", 1, 0); }
    break;

  case 128:
/* Line 1787 of yacc.c  */
#line 346 "compose.y"
    { (yyval.pSymDataType) = parsedata_find_type(pParseData, "entity", 0, 0); }
    break;


/* Line 1787 of yacc.c  */
#line 2695 "compose_parser.c"
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
#line 348 "compose.y"



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

