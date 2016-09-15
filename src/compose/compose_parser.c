/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 27 "compose.y" /* yacc.c:339  */

//#define YYDEBUG 1
#include "compose/compiler.h"

#include "compose/comp_mem.h"
#define YYMALLOC COMP_ALLOC
#define YYFREE COMP_FREE

#include <stdio.h>

#line 77 "compose_parser.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
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
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    HASH = 259,
    STRING_LITERAL = 260,
    INT_LITERAL = 261,
    TRUE_ = 262,
    FALSE_ = 263,
    FLOAT_LITERAL = 264,
    VOID_ = 265,
    BOOL_ = 266,
    INT_ = 267,
    FLOAT_ = 268,
    COLOR = 269,
    VEC2 = 270,
    VEC3 = 271,
    VEC4 = 272,
    IVEC2 = 273,
    IVEC3 = 274,
    IVEC4 = 275,
    QUAT = 276,
    MAT3 = 277,
    MAT43 = 278,
    MAT4 = 279,
    HANDLE_ = 280,
    ASSET = 281,
    ENTITY = 282,
    STRING = 283,
    IF = 284,
    SWITCH = 285,
    CASE = 286,
    DEFAULT = 287,
    FOR = 288,
    WHILE = 289,
    DO = 290,
    BREAK = 291,
    RETURN = 292,
    COMPONENT = 293,
    COMPONENTS = 294,
    UPDATE = 295,
    INPUT_ = 296,
    ANY = 297,
    NONE = 298,
    USING = 299,
    AS = 300,
    CONST_ = 301,
    SELF = 302,
    PRE = 303,
    POST = 304,
    VALUE = 305,
    ELSE = 306,
    THEN = 307,
    ADD_ASSIGN = 308,
    SUB_ASSIGN = 309,
    MUL_ASSIGN = 310,
    DIV_ASSIGN = 311,
    MOD_ASSIGN = 312,
    LSHIFT_ASSIGN = 313,
    RSHIFT_ASSIGN = 314,
    AND_ASSIGN = 315,
    XOR_ASSIGN = 316,
    OR_ASSIGN = 317,
    TRANSFORM = 318,
    READY = 319,
    OR = 320,
    AND = 321,
    EQ = 322,
    NEQ = 323,
    LTE = 324,
    GTE = 325,
    LSHIFT = 326,
    RSHIFT = 327,
    INC = 328,
    DEC = 329,
    UMINUS = 330,
    POSTINC = 331,
    POSTDEC = 332
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 49 "compose.y" /* yacc.c:355  */

    int                 numi;
    float               numf;
    const char*         str;
    DataType            dataType;
    Ast*                pAst;
    SymTab*             pSymTab;
    const SymDataType*  pSymDataType;

#line 205 "compose_parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (ParseData * pParseData);

#endif /* !YY_YY_COMPOSE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */
#line 59 "compose.y" /* yacc.c:358  */

#define YY_NO_UNISTD_H
#include "compose/compose_scanner.h"

/*
   This is embarrassingly hackish, but Bison 3's deprecation of
   YYLEX_PARAM doesn't give much choice.  Have researched as
   throrougly as I'm interested in doing at the moment, and this is
   the simplest (maybe only???) solution. Flex wants a scanner, and
   Bison wants to send it a variable, not allowing us to process that
   variable before sending through.
*/
#define SCANNER_FROM_PARSEDATA parsedata_scanner(pParseData)

#define YYPRINT(file, type, value)   yyprint (file, type, value)
static void yyprint(FILE * file, int type, YYSTYPE value);

#line 252 "compose_parser.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

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
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  43
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1343

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  103
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  172
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  328

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   332

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    86,     2,     2,   102,    83,    70,     2,
      92,    95,    81,    79,    99,    80,    89,    82,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   101,    98,
      73,    53,    74,     2,   100,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    93,     2,    96,    69,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    94,    68,    97,    87,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    71,    72,    75,    76,    77,    78,    84,    85,
      88,    90,    91
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   124,   124,   125,   129,   130,   134,   138,   142,   143,
     147,   151,   152,   157,   158,   159,   163,   164,   168,   169,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     186,   190,   191,   192,   196,   197,   201,   202,   206,   207,
     211,   212,   213,   217,   218,   219,   223,   226,   227,   231,
     232,   236,   237,   238,   242,   243,   247,   248,   252,   253,
     254,   258,   259,   263,   264,   268,   269,   271,   272,   274,
     276,   277,   279,   280,   282,   284,   286,   288,   292,   293,
     294,   295,   299,   300,   301,   302,   306,   308,   309,   311,
     313,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   340,   341,   342,   344,   346,   347,
     348,   349,   351,   353,   354,   355,   357,   359,   360,   364,
     365,   366,   367,   368,   369,   370,   374,   375,   376,   377,
     381,   382,   386,   387,   391,   392,   393,   397,   398,   399,
     400,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,   427,   428,   429,
     437,   438,   439
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "HASH", "STRING_LITERAL",
  "INT_LITERAL", "TRUE_", "FALSE_", "FLOAT_LITERAL", "VOID_", "BOOL_",
  "INT_", "FLOAT_", "COLOR", "VEC2", "VEC3", "VEC4", "IVEC2", "IVEC3",
  "IVEC4", "QUAT", "MAT3", "MAT43", "MAT4", "HANDLE_", "ASSET", "ENTITY",
  "STRING", "IF", "SWITCH", "CASE", "DEFAULT", "FOR", "WHILE", "DO",
  "BREAK", "RETURN", "COMPONENT", "COMPONENTS", "UPDATE", "INPUT_", "ANY",
  "NONE", "USING", "AS", "CONST_", "SELF", "PRE", "POST", "VALUE", "ELSE",
  "THEN", "'='", "ADD_ASSIGN", "SUB_ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN",
  "MOD_ASSIGN", "LSHIFT_ASSIGN", "RSHIFT_ASSIGN", "AND_ASSIGN",
  "XOR_ASSIGN", "OR_ASSIGN", "TRANSFORM", "READY", "OR", "AND", "'|'",
  "'^'", "'&'", "EQ", "NEQ", "'<'", "'>'", "LTE", "GTE", "LSHIFT",
  "RSHIFT", "'+'", "'-'", "'*'", "'/'", "'%'", "INC", "DEC", "'!'", "'~'",
  "UMINUS", "'.'", "POSTINC", "POSTDEC", "'('", "'['", "'{'", "')'", "']'",
  "'}'", "';'", "','", "'@'", "':'", "'$'", "$accept", "cmp_file",
  "using_list", "using_stmt", "dotted_id", "dotted_id_proc",
  "dotted_id_part", "def_list", "def", "message_block", "message_list",
  "message_prop", "function_def", "param_list", "component_block",
  "component_member_list", "component_member", "prop_init_list",
  "prop_init", "property_decl", "property_block", "property_def_list",
  "property_def", "input_block", "input_def_list", "input_def", "block",
  "stmt_list", "stmt", "target_expr", "message_expr", "expr", "cond_expr",
  "literal", "expr_or_empty", "cond_expr_or_empty", "fun_params", "type",
  "basic_type", "type_ent", "type_ent_handle_asset", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,    61,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   124,    94,
      38,   322,   323,    60,    62,   324,   325,   326,   327,    43,
      45,    42,    47,    37,   328,   329,    33,   126,   330,    46,
     331,   332,    40,    91,   123,    41,    93,   125,    59,    44,
      64,    58,    36
};
# endif

#define YYPACT_NINF -160

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-160)))

#define YYTABLE_NINF -151

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-151)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     752,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,    16,
    -160,    25,    43,   271,    37,   752,  -160,   -37,  -160,  -160,
     789,  -160,  -160,  -160,  -160,  -160,    60,   -15,   -15,   -34,
    -160,   -37,  -160,  -160,  -160,   789,    43,  -160,     2,   625,
    -160,  -160,    43,  -160,   826,    30,  -160,    36,    49,   128,
    -160,   664,  -160,  -160,   -50,   149,   -80,   -21,   154,   826,
      17,  -160,   232,  -160,    66,  -160,  -160,   532,   -27,  -160,
    -160,   -45,  -160,  -160,    49,   826,  -160,    38,  -160,   -44,
     -26,  -160,  -160,  -160,  -160,  -160,  -160,  -160,    72,    87,
      91,   432,   532,   133,  -160,  -160,   532,   532,   532,   532,
     532,   532,  -160,     7,   102,   508,  -160,   332,  -160,   807,
    -160,  -160,   101,   193,    -2,  -160,   840,   146,    49,    49,
    -160,   -25,  -160,   532,  -160,  -160,   198,    49,     3,  -160,
      43,   532,   532,   532,   170,   873,   532,  -160,   -69,   -69,
     -69,   -69,  1071,  -160,  -160,  -160,    23,   203,   532,   532,
     532,   532,   532,   532,   532,   532,   532,   532,   532,   532,
       3,  -160,  -160,   532,   532,   532,   532,   532,   532,   532,
     532,   532,   532,   532,   532,   532,   532,   532,   532,   532,
     532,  -160,  -160,  -160,   532,   155,    49,    49,    49,  -160,
      34,  -160,  -160,   532,  -160,  -160,  -160,  -160,   906,  -160,
    -160,   159,   160,   161,    59,  -160,  -160,  1101,  1221,   117,
    1131,   124,  -160,   939,  -160,   -49,    33,   125,  1221,  1221,
    1221,  1221,  1221,  1221,  1221,  1221,  1221,  1221,  1221,  1221,
      40,    78,  1240,  1258,   786,   310,   326,   409,   409,   424,
     424,   424,   424,    88,    88,   -23,   -23,   -69,   -69,   -69,
      83,   532,  -160,  -160,  -160,  -160,  -160,   972,  -160,   532,
     532,   532,  -160,     3,   432,   532,   432,   532,  -160,   532,
     532,  -160,  -160,  -160,   532,   -41,   532,  -160,   532,  -160,
    -160,  1221,  -160,  1221,  1221,  1221,  -160,   167,  1221,   121,
     122,  -160,  1161,  1005,    45,  1191,   532,   532,    51,  1221,
     432,   532,   126,  -160,   129,  -160,  1038,    56,  -160,  -160,
     127,  -160,  -160,  -160,   130,   432,  -160,  -160
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    10,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   171,   172,     0,
     166,     0,     0,     0,     0,     0,     4,   150,     7,     8,
       2,    11,    15,   167,   148,   170,     0,     0,     0,     0,
     168,   149,   147,     1,     5,     3,     0,    12,     0,     0,
      13,    14,     0,     9,    31,     0,   169,     0,     0,     0,
      16,     0,    18,    29,     0,     0,     0,     0,     0,    31,
       0,    26,     0,    27,     0,    17,    19,     0,     0,    23,
      21,     0,    46,     6,     0,     0,    32,     0,    34,    38,
       0,    36,   117,    89,   136,   138,   139,   137,     0,     0,
       0,     0,     0,     0,   128,   127,     0,     0,     0,     0,
       0,     0,    61,    78,     0,   135,    75,     0,    63,     0,
      90,   122,   148,     0,     0,    28,     0,     0,     0,     0,
      47,     0,    49,     0,    25,    30,     0,     0,    40,    35,
       0,     0,   140,     0,     0,     0,     0,   116,   118,   119,
     114,   115,     0,    79,    81,    80,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   144,
      40,    62,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   120,   121,    76,   144,    87,     0,     0,     0,    54,
       0,    56,    22,     0,    52,    53,    48,    50,     0,    33,
      20,     0,     0,     0,     0,    41,    37,     0,   141,     0,
       0,     0,    74,     0,    86,     0,     0,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   145,
       0,     0,    98,    99,   100,   101,   102,   129,   130,   133,
     134,   131,   132,    96,    97,    91,    92,    93,    94,    95,
       0,     0,    58,    59,    60,    55,    57,     0,    24,     0,
       0,     0,    39,     0,     0,   142,     0,     0,    77,     0,
     144,    82,    84,    83,     0,     0,   144,   125,     0,   124,
     123,    88,    51,    43,    44,    45,    42,    65,     0,    90,
       0,    67,     0,     0,     0,     0,     0,   144,     0,   146,
       0,   140,     0,    70,     0,    85,     0,     0,   126,    66,
       0,    68,    71,    72,     0,     0,    73,    69
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -160,  -160,  -160,   201,    93,  -160,   177,   206,    12,   194,
    -160,   196,   -35,   189,  -160,  -160,   123,    92,    -5,  -160,
    -160,  -160,   139,  -160,  -160,    64,    -9,  -160,  -100,  -160,
    -160,   -77,    -4,  -160,   -39,  -160,  -159,  -160,   136,   669,
      80
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    24,    25,    26,   115,    28,    29,    30,    31,    50,
      61,    62,    32,    67,    71,    90,    91,   214,   215,    64,
      80,   131,   132,   125,   200,   201,   116,   117,   118,   156,
     285,   119,   120,   121,   219,   300,   240,    33,   122,   123,
      36
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     126,   144,   196,    77,   279,   127,   211,   127,   133,    46,
     153,    52,   306,   154,    63,   191,   192,   172,    83,    37,
       1,   128,   129,   128,   129,   145,    63,   225,    38,   147,
     148,   149,   150,   151,   152,   260,   281,    43,   196,   282,
     197,   198,    47,   280,    78,    46,     1,    54,    79,    73,
     138,   307,    46,   134,   155,    46,   208,    47,   188,   189,
     190,   191,   192,    48,   217,   218,   220,   212,   213,   223,
     130,   139,   206,   140,    84,   135,   197,   198,    85,    49,
     283,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,    27,    54,   199,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,    88,    39,    41,   239,    27,   204,
     205,   304,    69,    27,   226,   284,   267,   308,   210,    65,
      70,   265,    74,   137,    68,   287,    34,    85,    27,   288,
     314,    65,    27,    72,   288,    66,   318,    27,   317,    68,
     288,   324,    81,    82,    27,   288,   272,    86,   273,    42,
     124,    34,    27,    89,   141,   136,    34,   186,   187,   188,
     189,   190,   191,   192,   297,   289,   301,   273,    27,   142,
     290,    34,   288,   143,   291,    34,   146,   262,   263,   264,
      34,   157,   293,   294,   295,   194,   195,    34,   298,   203,
     302,   209,   303,   239,   221,    34,   227,   305,   261,   239,
     319,   309,   269,   270,   271,   275,   277,   286,   310,  -143,
     311,    34,   325,    53,   321,   327,    44,   322,   326,   316,
     239,    45,    51,    89,   218,     1,    92,    93,    94,    95,
      96,    97,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    76,    87,    56,
      20,    98,   241,   216,   266,    99,   100,   101,   296,   102,
     207,   299,   320,   103,     1,     0,     0,     0,    23,   104,
       0,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,   105,     0,    40,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   106,     0,     0,     0,   107,   108,   109,   110,
       0,     0,     0,     0,   111,     0,    72,     0,     0,   112,
       0,     0,   113,     0,   114,     1,    92,    93,    94,    95,
      96,    97,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,     0,     0,    56,
      20,    98,     0,     0,     0,    99,   100,   101,     0,   102,
       0,     0,     0,   103,     0,     0,     0,     0,    23,   104,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   105,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   106,     0,     0,     0,   107,   108,   109,   110,
       0,     0,     0,     0,   111,     0,    72,     0,     0,   171,
       0,     0,   113,     0,   114,     1,    92,    93,    94,    95,
      96,    97,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,     0,     0,    56,
      20,    98,     0,     0,     0,    99,   100,   101,     0,   102,
       0,     0,     0,   103,     0,     0,     0,     0,    23,   104,
    -151,  -151,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,   105,  -151,  -151,  -151,
    -151,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,  -150,   106,     0,     0,     0,   107,   108,   109,   110,
       0,     0,     0,     0,   111,     0,    72,     0,     0,     0,
       0,     0,   113,     0,   114,     1,    92,    93,    94,    95,
      96,    97,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,     0,     0,    56,
      20,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,     0,     0,     0,     0,     0,     0,    23,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   105,    46,     0,     0,
     169,     0,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   106,     0,     0,     0,   107,   108,   109,   110,
       0,     0,     0,     0,   111,     0,     0,     0,     1,    55,
       0,     0,     0,     0,   114,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    56,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,    59,     1,    55,    35,
       0,    23,     0,     0,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    56,    20,     0,    35,     0,     0,     0,     0,    35,
       0,     0,     0,    57,    58,    59,     0,     0,     0,     0,
      23,     0,     0,     0,    35,     0,     0,     0,    35,     0,
       0,     0,    60,    35,     0,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    35,     1,     0,     0,     0,     0,
       0,    75,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     1,     0,     0,     0,    22,     0,    23,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     1,
       0,     0,     0,     0,     0,    23,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    56,    20,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,    23,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   193,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   202,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   222,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   278,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     292,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   276,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192
};

static const yytype_int16 yycheck[] =
{
      77,   101,     4,    53,    53,    32,     3,    32,    53,    89,
       3,    45,    53,     6,    49,    84,    85,   117,    98,     3,
       3,    48,    49,    48,    49,   102,    61,     4,     3,   106,
     107,   108,   109,   110,   111,   194,     3,     0,     4,     6,
      42,    43,    30,    92,    94,    89,     3,    92,    98,    58,
      94,    92,    89,    98,    47,    89,   133,    45,    81,    82,
      83,    84,    85,     3,   141,   142,   143,    64,    65,   146,
      97,    97,    97,    99,    95,    84,    42,    43,    99,    94,
      47,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,     0,    92,    97,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,    97,    22,    23,   194,    25,   128,
     129,   280,    92,    30,   101,    92,   203,   286,   137,    49,
      94,    97,     4,    95,    54,    95,     0,    99,    45,    99,
      95,    61,    49,    94,    99,    52,    95,    54,   307,    69,
      99,    95,     3,     4,    61,    99,    97,     3,    99,    23,
      94,    25,    69,    70,    92,    85,    30,    79,    80,    81,
      82,    83,    84,    85,   274,    97,   276,    99,    85,    92,
      97,    45,    99,    92,   261,    49,    53,   196,   197,   198,
      54,    89,   269,   270,   271,    94,     3,    61,   275,    53,
     277,     3,   279,   280,    34,    69,     3,   284,    53,   286,
     310,   288,    53,    53,    53,    98,    92,    92,    51,    98,
      98,    85,    95,    46,    98,   325,    25,    98,    98,   306,
     307,    25,    38,   140,   311,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    61,    69,    27,
      28,    29,   170,   140,   200,    33,    34,    35,   273,    37,
     131,   275,   311,    41,     3,    -1,    -1,    -1,    46,    47,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    64,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    -1,    -1,    92,    -1,    94,    -1,    -1,    97,
      -1,    -1,   100,    -1,   102,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    33,    34,    35,    -1,    37,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    46,    47,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    64,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    80,    -1,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    -1,    -1,    92,    -1,    94,    -1,    -1,    97,
      -1,    -1,   100,    -1,   102,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    33,    34,    35,    -1,    37,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    46,    47,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    64,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,     3,    80,    -1,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    -1,    -1,    92,    -1,    94,    -1,    -1,    -1,
      -1,    -1,   100,    -1,   102,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    27,
      28,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    89,    -1,    -1,
      92,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,     3,     4,
      -1,    -1,    -1,    -1,   102,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    40,    41,     3,     4,     0,
      -1,    46,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    25,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    97,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,     3,    -1,    -1,    -1,    -1,
      -1,    97,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,     3,    -1,    -1,    -1,    44,    -1,    46,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    46,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    46,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    38,    44,    46,   104,   105,   106,   107,   108,   109,
     110,   111,   115,   140,   141,   142,   143,     3,     3,   107,
      27,   107,   141,     0,   106,   110,    89,   111,     3,    94,
     112,   112,    45,   109,    92,     4,    27,    39,    40,    41,
      97,   113,   114,   115,   122,   143,   107,   116,   143,    92,
      94,   117,    94,   129,     4,    97,   114,    53,    94,    98,
     123,     3,     4,    98,    95,    99,     3,   116,    97,   107,
     118,   119,     4,     5,     6,     7,     8,     9,    29,    33,
      34,    35,    37,    41,    47,    64,    80,    84,    85,    86,
      87,    92,    97,   100,   102,   107,   129,   130,   131,   134,
     135,   136,   141,   142,    94,   126,   134,    32,    48,    49,
      97,   124,   125,    53,    98,   129,   143,    95,    94,    97,
      99,    92,    92,    92,   131,   134,    53,   134,   134,   134,
     134,   134,   134,     3,     6,    47,   132,    89,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    92,
      94,    97,   131,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    98,    94,     3,     4,    42,    43,    97,
     127,   128,    98,    53,   129,   129,    97,   125,   134,     3,
     129,     3,    64,    65,   120,   121,   119,   134,   134,   137,
     134,    34,    98,   134,    95,     4,   101,     3,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     139,   120,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     139,    53,   129,   129,   129,    97,   128,   134,    98,    53,
      53,    53,    97,    99,    95,    98,    95,    92,    98,    53,
      92,     3,     6,    47,    92,   133,    92,    95,    99,    97,
      97,   134,    98,   134,   134,   134,   121,   131,   134,   135,
     138,   131,   134,   134,   139,   134,    53,    92,   139,   134,
      51,    98,    95,    98,    95,    95,   134,   139,    95,   131,
     137,    98,    98,    98,    95,    95,    98,   131
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   103,   104,   104,   105,   105,   106,   107,   108,   108,
     109,   110,   110,   111,   111,   111,   112,   112,   113,   113,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     115,   116,   116,   116,   117,   117,   118,   118,   119,   119,
     120,   120,   120,   121,   121,   121,   122,   123,   123,   124,
     124,   125,   125,   125,   126,   126,   127,   127,   128,   128,
     128,   129,   129,   130,   130,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   132,   132,
     132,   132,   133,   133,   133,   133,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   135,
     135,   135,   135,   135,   135,   135,   136,   136,   136,   136,
     137,   137,   138,   138,   139,   139,   139,   140,   140,   140,
     140,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   142,   142,   142,
     143,   143,   143
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     5,     1,     1,     3,
       1,     1,     2,     3,     3,     1,     2,     3,     1,     2,
       5,     2,     4,     2,     5,     3,     2,     2,     3,     1,
       6,     0,     2,     4,     2,     3,     1,     3,     1,     4,
       0,     1,     3,     3,     3,     3,     2,     2,     3,     1,
       2,     4,     2,     2,     2,     3,     1,     2,     2,     2,
       2,     2,     3,     1,     2,     5,     7,     5,     7,     9,
       6,     7,     7,     8,     3,     1,     2,     4,     0,     1,
       1,     1,     1,     1,     1,     3,     3,     2,     4,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     1,     2,     2,
       2,     2,     1,     4,     4,     4,     6,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       0,     1,     0,     1,     0,     1,     3,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
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
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, pParseData); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParseData * pParseData)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (pParseData);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParseData * pParseData)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, pParseData);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, ParseData * pParseData)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , pParseData);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, pParseData); \
} while (0)

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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
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

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ParseData * pParseData)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (pParseData);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (ParseData * pParseData)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

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
      yychar = yylex (&yylval, &yylloc, SCANNER_FROM_PARSEDATA);
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
     '$$ = $1'.

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
#line 125 "compose.y" /* yacc.c:1646  */
    { ast_create_using_list((yyvsp[-1].pAst), pParseData); }
#line 1882 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 129 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_UsingList, NULL, (yyvsp[0].pAst), pParseData); }
#line 1888 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 130 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_UsingList, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 1894 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 134 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_using_stmt((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 1900 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 138 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_dotted_id((yyvsp[0].pAst), pParseData); }
#line 1906 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 142 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_DottedId, NULL, (yyvsp[0].pAst), pParseData); }
#line 1912 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 143 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_DottedId, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 1918 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 147 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_with_str(kAST_DottedIdPart, (yyvsp[0].str), pParseData); }
#line 1924 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 157 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_entity_def((yyvsp[-1].str), (yyvsp[0].pAst), pParseData); }
#line 1930 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 158 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_component_def((yyvsp[-1].str), (yyvsp[0].pAst), pParseData); }
#line 1936 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 159 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = parsedata_add_root_ast(pParseData, (yyvsp[0].pAst)); }
#line 1942 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 163 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
#line 1948 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 164 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_block((yyvsp[-1].pAst),   pParseData); }
#line 1954 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 168 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[0].pAst), pParseData); }
#line 1960 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 169 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 1966 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 173 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_message_def((yyvsp[-4].str), (yyvsp[-2].pSymTab), (yyvsp[0].pAst), pParseData); }
#line 1972 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 174 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_complex_def((yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 1978 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 175 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_def((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 1984 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 176 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_def((yyvsp[-1].pAst), NULL, pParseData); }
#line 1990 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 177 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_field_def((yyvsp[-3].str), (yyvsp[-4].pSymDataType), (yyvsp[-1].pAst), pParseData); }
#line 1996 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 178 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_field_def((yyvsp[-1].str), (yyvsp[-2].pSymDataType), NULL, pParseData); }
#line 2002 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 179 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_component_members((yyvsp[0].pAst), pParseData); }
#line 2008 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 180 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_update_def((yyvsp[0].pAst), pParseData); }
#line 2014 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 181 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_input_mode((yyvsp[-1].str), (yyvsp[0].pAst), pParseData); }
#line 2020 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 182 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2026 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 186 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_function_def((yyvsp[-4].str), (yyvsp[-5].pSymDataType), (yyvsp[-2].pSymTab), (yyvsp[0].pAst), pParseData); }
#line 2032 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 190 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, NULL); }
#line 2038 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 191 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, symrec_create(kSYMT_Param, (yyvsp[-1].pSymDataType), (yyvsp[0].str), NULL, NULL, pParseData)); }
#line 2044 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 192 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymTab) = parsedata_add_param(pParseData, (yyvsp[-3].pSymTab), symrec_create(kSYMT_Param, (yyvsp[-1].pSymDataType), (yyvsp[0].str), NULL, NULL, pParseData)); }
#line 2050 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 196 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_with_child_list(kAST_ComponentMemberList, pParseData); }
#line 2056 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 197 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2062 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 201 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, NULL, (yyvsp[0].pAst), pParseData); }
#line 2068 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 202 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2074 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 206 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_component_member((yyvsp[0].pAst), ast_create(kAST_PropInit, pParseData), pParseData); }
#line 2080 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 207 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_component_member((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2086 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 211 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create(kAST_PropInitList, pParseData); }
#line 2092 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 41:
#line 212 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_PropInitList, NULL, (yyvsp[0].pAst), pParseData); }
#line 2098 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 42:
#line 213 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_PropInitList, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2104 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 217 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_prop_init((yyvsp[-2].str), (yyvsp[0].pAst), pParseData); }
#line 2110 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 44:
#line 218 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_transform_init((yyvsp[0].pAst), pParseData); }
#line 2116 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 45:
#line 219 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_ready_init((yyvsp[0].pAst), pParseData); }
#line 2122 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 46:
#line 223 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_decl((yyvsp[0].str), (yyvsp[-1].pSymDataType), pParseData); }
#line 2128 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 47:
#line 226 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_with_child_list(kAST_PropertyDef, pParseData); }
#line 2134 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 48:
#line 227 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2140 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 231 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_PropertyDef, NULL, (yyvsp[0].pAst), pParseData); }
#line 2146 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 232 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_PropertyDef, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 2152 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 236 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_default_assign((yyvsp[-1].pAst), pParseData); }
#line 2158 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 237 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_pre((yyvsp[0].pAst), pParseData); }
#line 2164 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 238 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_post((yyvsp[0].pAst), pParseData); }
#line 2170 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 242 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_with_child_list(kAST_Input, pParseData); }
#line 2176 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 243 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2182 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 247 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_Input, NULL, (yyvsp[0].pAst), pParseData); }
#line 2188 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 248 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_Input, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 2194 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 252 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_input_def((yyvsp[-1].str), (yyvsp[0].pAst), pParseData); }
#line 2200 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 253 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_input_special_def("any", (yyvsp[0].pAst), pParseData); }
#line 2206 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 254 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_input_special_def("none", (yyvsp[0].pAst), pParseData); }
#line 2212 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 258 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
#line 2218 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 259 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_block((yyvsp[-1].pAst),   pParseData); }
#line 2224 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 263 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[0].pAst), pParseData); }
#line 2230 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 264 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 2236 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 268 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_if((yyvsp[-2].pAst), (yyvsp[0].pAst), NULL, pParseData); }
#line 2242 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 269 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_if((yyvsp[-4].pAst), (yyvsp[-2].pAst), (yyvsp[0].pAst),   pParseData); }
#line 2248 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 271 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_while((yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2254 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 272 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_dowhile((yyvsp[-2].pAst), (yyvsp[-5].pAst), pParseData); }
#line 2260 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 274 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_for((yyvsp[-6].pAst), (yyvsp[-4].pAst), (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2266 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 276 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_set((yyvsp[-4].pAst), ast_create_hash((yyvsp[-3].str), pParseData), (yyvsp[-1].pAst), pParseData); }
#line 2272 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 277 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_message_send((yyvsp[-5].pAst), ast_create_hash((yyvsp[-4].str), pParseData), (yyvsp[-2].pAst), pParseData); }
#line 2278 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 279 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_set((yyvsp[-5].pAst), (yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2284 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 280 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_message_send((yyvsp[-6].pAst), (yyvsp[-4].pAst), (yyvsp[-2].pAst), pParseData); }
#line 2290 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 282 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_return((yyvsp[-1].pAst), pParseData); }
#line 2296 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 284 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2302 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 286 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_simple_stmt((yyvsp[-1].pAst), pParseData); }
#line 2308 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 288 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_input_assign((yyvsp[-1].pAst), pParseData); }
#line 2314 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 292 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = NULL; }
#line 2320 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 293 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_identifier((yyvsp[0].str), pParseData); }
#line 2326 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 294 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create(kAST_Self, pParseData); }
#line 2332 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 295 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_int_literal((yyvsp[0].numi), pParseData); }
#line 2338 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 299 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_identifier((yyvsp[0].str), pParseData); }
#line 2344 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 300 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create(kAST_Self, pParseData); }
#line 2350 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 301 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_int_literal((yyvsp[0].numi), pParseData); }
#line 2356 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 302 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2362 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 306 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2368 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 308 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[-1].pSymDataType), (yyvsp[0].str), NULL, NULL, pParseData)); }
#line 2374 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 309 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[-3].pSymDataType), (yyvsp[-2].str), NULL, (yyvsp[0].pAst), pParseData)); }
#line 2380 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 311 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_string_literal((yyvsp[0].str), pParseData); }
#line 2386 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 313 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2392 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 315 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Add,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2398 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 316 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Sub,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2404 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 317 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Mul,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2410 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 318 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Div,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2416 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 319 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Mod,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2422 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 320 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_LShift, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2428 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 321 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_RShift, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2434 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 322 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Or,     (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2440 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 323 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_And,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2446 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 324 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_BitOr,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2452 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 325 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_BitXor, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2458 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 326 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_BitAnd, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2464 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 328 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_Assign,       (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2470 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 329 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_AddAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2476 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 330 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_SubAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2482 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 331 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_MulAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2488 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 332 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_DivAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2494 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 333 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_ModAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2500 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 334 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_LShiftAssign, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2506 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 335 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_RShiftAssign, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2512 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 336 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_AndAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2518 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 337 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_XorAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2524 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 338 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_OrAssign,     (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2530 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 340 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_Not,        (yyvsp[0].pAst), pParseData); }
#line 2536 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 341 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_Complement, (yyvsp[0].pAst), pParseData); }
#line 2542 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 342 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_Negate,     (yyvsp[0].pAst), pParseData); }
#line 2548 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 344 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_hash((yyvsp[0].str), pParseData); }
#line 2554 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 346 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_PreInc, (yyvsp[0].pAst), pParseData); }
#line 2560 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 347 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_PreDec, (yyvsp[0].pAst), pParseData); }
#line 2566 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 348 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_PostInc, (yyvsp[-1].pAst), pParseData); }
#line 2572 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 349 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_PostDec, (yyvsp[-1].pAst), pParseData); }
#line 2578 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 351 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2584 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 353 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_type_init((yyvsp[-3].dataType), (yyvsp[-1].pAst), pParseData); }
#line 2590 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 354 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_entity_init((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2596 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 355 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_function_call((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2602 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 357 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_system_api_call((yyvsp[-3].str), (yyvsp[-1].pAst), pParseData); }
#line 2608 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 359 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create(kAST_Transform, pParseData); }
#line 2614 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 360 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create(kAST_Self, pParseData); }
#line 2620 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 364 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Eq,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2626 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 365 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_NEq, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2632 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 366 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_LTE, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2638 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 367 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_GTE, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2644 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 368 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_LT,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2650 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 369 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_GT,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2656 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 370 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_symbol_ref((yyvsp[0].pAst), pParseData); }
#line 2662 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 374 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_int_literal((yyvsp[0].numi), pParseData); }
#line 2668 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 375 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_float_literal((yyvsp[0].numf), pParseData); }
#line 2674 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 376 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_bool_literal(1, pParseData); }
#line 2680 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 377 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_bool_literal(0, pParseData); }
#line 2686 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 381 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = NULL; }
#line 2692 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 382 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2698 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 386 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = NULL; }
#line 2704 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 387 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2710 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 391 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, NULL, pParseData); }
#line 2716 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 392 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, (yyvsp[0].pAst),   pParseData); }
#line 2722 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 393 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_FunctionParams, (yyvsp[-2].pAst),   (yyvsp[0].pAst),   pParseData); }
#line 2728 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 397 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_basic_type(pParseData, (yyvsp[0].dataType), 1, 0); }
#line 2734 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 398 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_basic_type(pParseData, (yyvsp[0].dataType), 0, 0); }
#line 2740 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 399 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type_from_dotted_id(pParseData, (yyvsp[0].pAst), 1, 0); }
#line 2746 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 400 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type_from_dotted_id(pParseData, (yyvsp[0].pAst), 0, 0); }
#line 2752 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 427 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = (yyvsp[0].pSymDataType); }
#line 2758 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 428 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type(pParseData, "entity", 1, 0); }
#line 2764 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 429 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type(pParseData, "entity", 0, 0); }
#line 2770 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 437 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = (yyvsp[0].pSymDataType); }
#line 2776 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 438 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type(pParseData, "handle", 0, 0); }
#line 2782 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 439 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type(pParseData, "asset_handle", 0, 0); }
#line 2788 "compose_parser.c" /* yacc.c:1646  */
    break;


#line 2792 "compose_parser.c" /* yacc.c:1646  */
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

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
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
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 442 "compose.y" /* yacc.c:1906  */



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

