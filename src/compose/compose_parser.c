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
    MAT43 = 284,
    MAT4 = 285,
    HANDLE_ = 286,
    ASSET = 287,
    ENTITY = 288,
    STRING = 289,
    IF = 290,
    SWITCH = 291,
    CASE = 292,
    DEFAULT = 293,
    FOR = 294,
    WHILE = 295,
    DO = 296,
    BREAK = 297,
    RETURN = 298,
    COMPONENT = 299,
    COMPONENTS = 300,
    USING = 301,
    AS = 302,
    CONST_ = 303,
    SELF = 304,
    ELSE = 305,
    THEN = 306,
    ADD_ASSIGN = 307,
    SUB_ASSIGN = 308,
    MUL_ASSIGN = 309,
    DIV_ASSIGN = 310,
    MOD_ASSIGN = 311,
    LSHIFT_ASSIGN = 312,
    RSHIFT_ASSIGN = 313,
    AND_ASSIGN = 314,
    XOR_ASSIGN = 315,
    OR_ASSIGN = 316,
    TRANSFORM = 317,
    OR = 318,
    AND = 319,
    EQ = 320,
    NEQ = 321,
    LTE = 322,
    GTE = 323,
    LSHIFT = 324,
    RSHIFT = 325,
    INC = 326,
    DEC = 327,
    UMINUS = 328,
    POSTINC = 329,
    POSTDEC = 330
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

#line 203 "compose_parser.c" /* yacc.c:355  */
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

#line 250 "compose_parser.c" /* yacc.c:358  */

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
#define YYFINAL  44
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1331

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  101
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  157
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  302

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   330

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    84,     2,     2,   100,    81,    68,     2,
      90,    93,    79,    77,    97,    78,    87,    80,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    99,    96,
      71,    52,    72,     2,    98,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    91,     2,    94,    67,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    92,    66,    95,    85,     2,     2,     2,
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
      69,    70,    73,    74,    75,    76,    82,    83,    86,    88,
      89
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   124,   124,   125,   129,   130,   134,   138,   142,   143,
     147,   151,   152,   157,   158,   159,   160,   161,   165,   166,
     170,   171,   175,   176,   177,   178,   179,   180,   184,   185,
     186,   190,   191,   195,   196,   200,   201,   205,   206,   207,
     211,   212,   216,   217,   221,   222,   226,   227,   229,   230,
     232,   234,   235,   237,   238,   240,   242,   244,   248,   249,
     250,   251,   255,   256,   257,   258,   262,   264,   265,   267,
     269,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   296,   297,   298,   300,   302,   303,
     304,   305,   307,   309,   310,   311,   313,   315,   319,   320,
     321,   322,   323,   324,   325,   329,   330,   331,   332,   336,
     337,   341,   342,   346,   347,   348,   352,   353,   354,   355,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   388,   389,   390,   398,   399,   400
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
  "QUAT", "MAT3", "MAT43", "MAT4", "HANDLE_", "ASSET", "ENTITY", "STRING",
  "IF", "SWITCH", "CASE", "DEFAULT", "FOR", "WHILE", "DO", "BREAK",
  "RETURN", "COMPONENT", "COMPONENTS", "USING", "AS", "CONST_", "SELF",
  "ELSE", "THEN", "'='", "ADD_ASSIGN", "SUB_ASSIGN", "MUL_ASSIGN",
  "DIV_ASSIGN", "MOD_ASSIGN", "LSHIFT_ASSIGN", "RSHIFT_ASSIGN",
  "AND_ASSIGN", "XOR_ASSIGN", "OR_ASSIGN", "TRANSFORM", "OR", "AND", "'|'",
  "'^'", "'&'", "EQ", "NEQ", "'<'", "'>'", "LTE", "GTE", "LSHIFT",
  "RSHIFT", "'+'", "'-'", "'*'", "'/'", "'%'", "INC", "DEC", "'!'", "'~'",
  "UMINUS", "'.'", "POSTINC", "POSTDEC", "'('", "'['", "'{'", "')'", "']'",
  "'}'", "';'", "','", "'@'", "':'", "'$'", "$accept", "cmp_file",
  "using_list", "using_stmt", "dotted_id", "dotted_id_proc",
  "dotted_id_part", "def_list", "def", "message_block", "message_list",
  "message_prop", "param_list", "component_block", "component_member_list",
  "component_member", "prop_init_list", "prop_init", "block", "stmt_list",
  "stmt", "target_expr", "message_expr", "expr", "cond_expr", "literal",
  "expr_or_empty", "cond_expr_or_empty", "fun_params", "type",
  "basic_type", "type_ent", "type_handle_asset", YY_NULLPTR
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
     305,   306,    61,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   124,    94,    38,   320,
     321,    60,    62,   322,   323,   324,   325,    43,    45,    42,
      47,    37,   326,   327,    33,   126,   328,    46,   329,   330,
      40,    91,   123,    41,    93,   125,    59,    44,    64,    58,
      36
};
# endif

#define YYPACT_NINF -110

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-110)))

#define YYTABLE_NINF -130

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-130)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     684,  -110,  -110,  -110,  -110,  -110,  -110,  -110,  -110,  -110,
    -110,  -110,  -110,  -110,  -110,  -110,  -110,  -110,  -110,  -110,
    -110,  -110,  -110,    40,  -110,    45,    46,   999,     8,   684,
    -110,    11,  -110,  -110,   762,  -110,    92,  -110,   -34,    12,
     -33,    97,    11,  -110,  -110,  -110,   762,    46,  -110,    16,
     723,    98,  -110,  -110,    46,    64,  -110,   723,  -110,  -110,
    -110,  1027,   -64,  -110,  -110,   144,    78,    61,  -110,   598,
    -110,    28,   -59,   723,    48,  -110,    79,   723,  -110,   723,
      -2,  -110,  -110,  -110,   -41,   -39,  -110,    55,    79,   213,
    -110,   176,    63,  -110,   -57,    10,  -110,   507,  -110,   507,
    -110,    79,  -110,  -110,  -110,  -110,  -110,  -110,  -110,   104,
     105,   116,   409,   507,  -110,   507,   507,   507,   507,   507,
     507,  -110,     3,   100,   492,  -110,   311,  -110,   747,  -110,
    -110,   118,   210,  -110,    79,     9,  -110,    46,   780,   813,
    -110,   507,   507,   507,   209,   846,  -110,    51,    51,    51,
      51,   998,  -110,  -110,  -110,     0,   247,   507,   507,   507,
     507,   507,   507,   507,   507,   507,   507,   507,   507,     9,
    -110,  -110,   507,   507,   507,   507,   507,   507,   507,   507,
     507,   507,   507,   507,   507,   507,   507,   507,   507,   507,
    -110,  -110,  -110,   507,   207,  -110,   208,   212,   110,  -110,
    -110,  -110,  -110,  1028,  1148,   162,  1058,   172,  -110,  -110,
     -16,     4,   175,  1148,  1148,  1148,  1148,  1148,  1148,  1148,
    1148,  1148,  1148,  1148,  1148,    77,   114,  1167,  1185,  1202,
    1218,  1233,  1248,  1248,   206,   206,   206,   206,   120,   120,
      57,    57,    51,    51,    51,   160,   507,   507,   507,  -110,
       9,   409,   507,   409,   507,   507,   507,  -110,  -110,  -110,
     507,     7,   507,  -110,   507,  -110,  -110,  1148,  1148,  1148,
    -110,   216,  1148,   173,   174,  -110,  1088,   879,    80,  1118,
     507,   507,    83,  1148,   409,   507,   178,  -110,   179,  -110,
     912,    85,  -110,  -110,   197,  -110,  -110,  -110,   196,   409,
    -110,  -110
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    10,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,     0,   151,     0,     0,     0,     0,     0,
       4,   129,     7,     8,     2,    11,     0,   127,     0,     0,
       0,     0,   128,   126,     1,     5,     3,     0,    12,     0,
      28,     0,    13,    14,     0,     0,     9,    28,   156,   157,
     154,     0,     0,   152,   155,     0,     0,     0,    18,     0,
      20,     0,     0,    28,     0,   153,     0,     0,    29,    28,
       0,    27,    19,    21,     0,     0,     6,     0,     0,     0,
      17,     0,     0,    31,    35,     0,    33,     0,    26,     0,
      24,     0,    15,    97,    69,   115,   117,   118,   116,     0,
       0,     0,     0,     0,   107,     0,     0,     0,     0,     0,
       0,    42,    58,     0,   114,    56,     0,    44,     0,    70,
     102,   127,     0,    30,     0,    37,    32,     0,     0,     0,
      16,     0,   119,     0,     0,     0,    96,    98,    99,    94,
      95,     0,    59,    61,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   123,    37,
      43,    45,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     100,   101,    57,   123,    67,    22,     0,     0,     0,    38,
      34,    25,    23,     0,   120,     0,     0,     0,    55,    66,
       0,     0,     0,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,   124,     0,     0,    78,    79,    80,
      81,    82,   108,   109,   112,   113,   110,   111,    76,    77,
      71,    72,    73,    74,    75,     0,     0,     0,     0,    36,
       0,     0,   121,     0,     0,     0,   123,    62,    64,    63,
       0,     0,   123,   105,     0,   104,   103,    68,    40,    41,
      39,    46,     0,    70,     0,    48,     0,     0,     0,     0,
       0,   123,     0,   125,     0,   119,     0,    51,     0,    65,
       0,     0,   106,    47,     0,    49,    52,    53,     0,     0,
      54,    50
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -110,  -110,  -110,   239,   135,  -110,   246,   265,    -8,   260,
    -110,   231,   -32,  -110,  -110,   164,   133,    54,   -61,  -110,
    -109,  -110,  -110,   -97,    58,  -110,    21,  -110,   -90,     5,
     606,   194,   -27
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    28,    29,    30,   124,    32,    33,    34,    35,    52,
      69,    70,    62,    81,    95,    96,   198,   199,   125,   126,
     127,   155,   261,   128,   129,   130,   205,   274,   225,    63,
     131,   132,    65
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     138,     1,   139,   144,   210,    36,   152,   257,    44,   153,
     258,    97,   196,    99,    54,    90,   145,   171,   146,   147,
     148,   149,   150,   151,    71,    74,    48,   102,    47,    76,
      47,    84,    85,    77,    36,   135,   255,    86,    48,    36,
     140,    87,    71,    38,   203,   204,   206,    92,    39,     1,
      91,    36,   154,   259,    47,    98,    50,   100,    51,   280,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   197,   195,   256,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,    93,   260,    49,   224,   281,    47,   211,
      55,     1,    66,   245,    51,   136,    57,   137,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    58,
      59,    60,    24,   190,   191,    31,   187,   188,   189,   190,
     191,    88,   271,    67,   275,    77,    61,    78,   101,   267,
     268,   269,    77,    80,    73,   272,   134,   276,   277,   224,
      77,    40,    42,   279,    31,   224,   278,   283,    79,    31,
     263,    89,   282,   288,   264,   293,   292,   264,   298,   133,
     264,    31,   264,   290,   224,    31,    31,   156,   204,    72,
     301,   291,    31,    68,   141,   142,    42,   185,   186,   187,
     188,   189,   190,   191,    31,   249,   143,   250,    31,   265,
     193,   250,    31,   194,    31,    94,     1,   103,   104,   105,
     106,   107,   108,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    64,    64,    60,    24,   109,   207,
     212,    64,   110,   111,   112,   266,   113,   264,   252,   246,
     247,    61,   254,    64,   248,   262,   284,    64,    45,  -122,
     285,    64,    94,    64,   295,   296,   114,  -130,  -130,  -130,
    -130,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     299,   115,   300,    56,    46,   116,   117,   118,   119,    53,
      83,   200,   226,   120,   270,    89,   294,     0,   121,     0,
     273,   122,     0,   123,     1,   103,   104,   105,   106,   107,
     108,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,     0,     0,    60,    24,   109,     0,     0,     0,
     110,   111,   112,     0,   113,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   115,
       0,     0,     0,   116,   117,   118,   119,     0,     0,     0,
       0,   120,     0,    89,     0,     0,   170,     0,     0,   122,
       0,   123,     1,   103,   104,   105,   106,   107,   108,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
       0,     0,    60,    24,   109,     0,     0,     0,   110,   111,
     112,     0,   113,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   115,     0,     0,
       0,   116,   117,   118,   119,  -129,     0,     0,     0,   120,
       0,    89,     0,     0,     0,     0,     0,   122,     0,   123,
       1,   103,   104,   105,   106,   107,   108,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,     0,     0,
      60,    24,     0,     0,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,     0,     0,     0,     0,     0,     0,     0,    47,
       0,     0,   168,     0,   169,   115,     0,     0,     0,   116,
     117,   118,   119,     0,     0,     0,     0,   120,     0,     0,
       0,     1,    66,     0,     0,     0,    37,   123,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    58,
      59,    60,    24,    43,     0,    37,     0,     0,     0,     0,
      37,     0,     0,    67,     0,     0,    61,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    37,    37,     0,     0,
       0,     0,     0,    37,     0,     0,     0,    43,     0,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     0,    37,
       0,     0,     0,    37,     0,    37,     0,     1,     0,     0,
       0,     0,     0,    82,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,     0,     0,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     1,     0,    25,     0,
      26,     0,    27,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    58,    59,    60,    24,     0,     0,
       0,     0,     0,     0,     0,     1,     0,     0,     0,     0,
       0,    61,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,     0,     0,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
      27,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   192,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   201,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   202,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   208,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   287,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,     0,     0,     0,     0,
       0,     0,     1,     0,     0,     0,     0,     0,   297,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
       1,     0,    41,    24,     0,     0,     0,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,     0,     0,
      75,    24,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   209,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   251,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   253,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   286,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   289,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,  -130,  -130,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191
};

static const yytype_int16 yycheck[] =
{
      97,     3,    99,   112,     4,     0,     3,     3,     0,     6,
       6,    52,     3,    52,    47,    76,   113,   126,   115,   116,
     117,   118,   119,   120,    51,    57,    34,    88,    87,    93,
      87,     3,     4,    97,    29,    92,    52,    96,    46,    34,
     101,    73,    69,     3,   141,   142,   143,    79,     3,     3,
      77,    46,    49,    49,    87,    96,    90,    96,    92,    52,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    63,   134,    90,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,    95,    90,     3,   193,    90,    87,    99,
       3,     3,     4,   193,    92,    95,    90,    97,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    82,    83,     0,    79,    80,    81,    82,
      83,    93,   251,    45,   253,    97,    48,     3,    93,   246,
     247,   248,    97,    92,    90,   252,    93,   254,   255,   256,
      97,    26,    27,   260,    29,   262,   256,   264,    90,    34,
      93,    92,   262,    93,    97,   284,    93,    97,    93,     3,
      97,    46,    97,   280,   281,    50,    51,    87,   285,    54,
     299,   281,    57,    95,    90,    90,    61,    77,    78,    79,
      80,    81,    82,    83,    69,    95,    90,    97,    73,    95,
      92,    97,    77,     3,    79,    80,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    50,    51,    33,    34,    35,    40,
       3,    57,    39,    40,    41,    95,    43,    97,    96,    52,
      52,    48,    90,    69,    52,    90,    50,    73,    29,    96,
      96,    77,   137,    79,    96,    96,    63,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      93,    78,    96,    47,    29,    82,    83,    84,    85,    39,
      69,   137,   169,    90,   250,    92,   285,    -1,    95,    -1,
     252,    98,    -1,   100,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    -1,    33,    34,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    82,    83,    84,    85,    -1,    -1,    -1,
      -1,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    98,
      -1,   100,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    -1,    33,    34,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    82,    83,    84,    85,     3,    -1,    -1,    -1,    90,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
      33,    34,    -1,    -1,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    90,    -1,    92,    78,    -1,    -1,    -1,    82,
      83,    84,    85,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,     3,     4,    -1,    -1,    -1,     0,   100,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    27,    -1,    29,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    77,    -1,    79,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    95,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    44,    -1,
      46,    -1,    48,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    48,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      48,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    96,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
       3,    -1,    33,    34,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
      33,    34,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    33,    34,    44,    46,    48,   102,   103,
     104,   105,   106,   107,   108,   109,   130,   131,     3,     3,
     105,    33,   105,   131,     0,   104,   108,    87,   109,     3,
      90,    92,   110,   110,    47,     3,   107,    90,    31,    32,
      33,    48,   113,   130,   132,   133,     4,    45,    95,   111,
     112,   133,   105,    90,   113,    33,    93,    97,     3,    90,
      92,   114,    95,   112,     3,     4,    96,   113,    93,    92,
     119,   133,   113,    95,   105,   115,   116,    52,    96,    52,
      96,    93,   119,     4,     5,     6,     7,     8,     9,    35,
      39,    40,    41,    43,    63,    78,    82,    83,    84,    85,
      90,    95,    98,   100,   105,   119,   120,   121,   124,   125,
     126,   131,   132,     3,    93,    92,    95,    97,   124,   124,
     119,    90,    90,    90,   121,   124,   124,   124,   124,   124,
     124,   124,     3,     6,    49,   122,    87,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    90,    92,
      95,   121,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    96,    92,     3,   119,     3,    63,   117,   118,
     116,    96,    96,   124,   124,   127,   124,    40,    96,    93,
       4,    99,     3,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   129,   117,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   129,    52,    52,    52,    95,
      97,    93,    96,    93,    90,    52,    90,     3,     6,    49,
      90,   123,    90,    93,    97,    95,    95,   124,   124,   124,
     118,   121,   124,   125,   128,   121,   124,   124,   129,   124,
      52,    90,   129,   124,    50,    96,    93,    96,    93,    93,
     124,   129,    93,   121,   127,    96,    96,    96,    93,    93,
      96,   121
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   101,   102,   102,   103,   103,   104,   105,   106,   106,
     107,   108,   108,   109,   109,   109,   109,   109,   110,   110,
     111,   111,   112,   112,   112,   112,   112,   112,   113,   113,
     113,   114,   114,   115,   115,   116,   116,   117,   117,   117,
     118,   118,   119,   119,   120,   120,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   122,   122,
     122,   122,   123,   123,   123,   123,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   125,   125,
     125,   125,   125,   125,   125,   126,   126,   126,   126,   127,
     127,   128,   128,   129,   129,   129,   130,   130,   130,   130,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   132,   132,   132,   133,   133,   133
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     5,     1,     1,     3,
       1,     1,     2,     3,     3,     6,     7,     6,     2,     3,
       1,     2,     5,     5,     3,     5,     3,     2,     0,     2,
       4,     2,     3,     1,     3,     1,     4,     0,     1,     3,
       3,     3,     2,     3,     1,     2,     5,     7,     5,     7,
       9,     6,     7,     7,     8,     3,     1,     2,     0,     1,
       1,     1,     1,     1,     1,     3,     3,     2,     4,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     1,     2,     2,
       2,     2,     1,     4,     4,     4,     6,     1,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     0,
       1,     0,     1,     0,     1,     3,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1
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
#line 1862 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 129 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_UsingList, NULL, (yyvsp[0].pAst), pParseData); }
#line 1868 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 130 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_UsingList, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 1874 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 134 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_using_stmt((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 1880 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 138 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_dotted_id((yyvsp[0].pAst), pParseData); }
#line 1886 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 142 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_DottedId, NULL, (yyvsp[0].pAst), pParseData); }
#line 1892 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 143 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_DottedId, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 1898 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 147 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_with_str(kAST_DottedIdPart, (yyvsp[0].str), pParseData); }
#line 1904 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 157 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_entity_def((yyvsp[-1].str), (yyvsp[0].pAst), pParseData); }
#line 1910 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 158 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_component_def((yyvsp[-1].str), (yyvsp[0].pAst), pParseData); }
#line 1916 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 159 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_function_def((yyvsp[-4].str), (yyvsp[-5].pSymDataType), (yyvsp[0].pAst), pParseData); }
#line 1922 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 160 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_function_def((yyvsp[-4].str), parsedata_find_type(pParseData, "entity", 1, 0), (yyvsp[0].pAst), pParseData); }
#line 1928 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 161 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_function_def((yyvsp[-4].str), parsedata_find_type(pParseData, "entity", 0, 0), (yyvsp[0].pAst), pParseData); }
#line 1934 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 165 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
#line 1940 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 166 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_block((yyvsp[-1].pAst),   pParseData); }
#line 1946 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 170 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[0].pAst), pParseData); }
#line 1952 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 171 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 1958 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 175 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_message_def((yyvsp[-4].str), (yyvsp[-2].pSymTab), (yyvsp[0].pAst), pParseData); }
#line 1964 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 176 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_def((yyvsp[-3].str), (yyvsp[-4].pSymDataType), (yyvsp[-1].pAst), pParseData); }
#line 1970 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 177 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_def((yyvsp[-1].str), (yyvsp[-2].pSymDataType), NULL, pParseData); }
#line 1976 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 178 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_field_def((yyvsp[-3].str), (yyvsp[-4].pSymDataType), (yyvsp[-1].pAst), pParseData); }
#line 1982 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 179 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_field_def((yyvsp[-1].str), (yyvsp[-2].pSymDataType), NULL, pParseData); }
#line 1988 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 180 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_component_members((yyvsp[0].pAst), pParseData); }
#line 1994 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 184 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, NULL); }
#line 2000 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 185 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, symrec_create(kSYMT_Param, (yyvsp[-1].pSymDataType), (yyvsp[0].str), NULL, pParseData)); }
#line 2006 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 186 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymTab) = parsedata_add_param(pParseData, (yyvsp[-3].pSymTab), symrec_create(kSYMT_Param, (yyvsp[-1].pSymDataType), (yyvsp[0].str), NULL, pParseData)); }
#line 2012 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 190 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_with_child_list(kAST_ComponentMemberList, pParseData); }
#line 2018 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 191 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2024 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 195 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, NULL, (yyvsp[0].pAst), pParseData); }
#line 2030 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 196 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_ComponentMemberList, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2036 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 200 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_component_member((yyvsp[0].pAst), ast_create(kAST_PropInit, pParseData), pParseData); }
#line 2042 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 201 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_component_member((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2048 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 205 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create(kAST_PropInitList, pParseData); }
#line 2054 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 206 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_PropInitList, NULL, (yyvsp[0].pAst), pParseData); }
#line 2060 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 207 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_PropInitList, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2066 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 211 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_prop_init((yyvsp[-2].str), (yyvsp[0].pAst), pParseData); }
#line 2072 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 41:
#line 212 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_transform_init((yyvsp[0].pAst), pParseData); }
#line 2078 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 42:
#line 216 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_block(NULL, pParseData); }
#line 2084 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 217 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_block((yyvsp[-1].pAst),   pParseData); }
#line 2090 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 44:
#line 221 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[0].pAst), pParseData); }
#line 2096 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 45:
#line 222 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 2102 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 46:
#line 226 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_if((yyvsp[-2].pAst), (yyvsp[0].pAst), NULL, pParseData); }
#line 2108 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 47:
#line 227 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_if((yyvsp[-4].pAst), (yyvsp[-2].pAst), (yyvsp[0].pAst),   pParseData); }
#line 2114 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 48:
#line 229 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_while((yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2120 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 230 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_dowhile((yyvsp[-2].pAst), (yyvsp[-5].pAst), pParseData); }
#line 2126 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 232 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_for((yyvsp[-6].pAst), (yyvsp[-4].pAst), (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2132 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 234 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_set((yyvsp[-4].pAst), ast_create_hash((yyvsp[-3].str), pParseData), (yyvsp[-1].pAst), pParseData); }
#line 2138 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 235 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_message_send((yyvsp[-5].pAst), ast_create_hash((yyvsp[-4].str), pParseData), (yyvsp[-2].pAst), pParseData); }
#line 2144 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 237 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_property_set((yyvsp[-5].pAst), (yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2150 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 238 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_message_send((yyvsp[-6].pAst), (yyvsp[-4].pAst), (yyvsp[-2].pAst), pParseData); }
#line 2156 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 240 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_return((yyvsp[-1].pAst), pParseData); }
#line 2162 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 242 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2168 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 244 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_simple_stmt((yyvsp[-1].pAst), pParseData); }
#line 2174 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 248 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = NULL; }
#line 2180 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 249 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_identifier((yyvsp[0].str), pParseData); }
#line 2186 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 250 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create(kAST_Self, pParseData); }
#line 2192 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 251 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_int_literal((yyvsp[0].numi), pParseData); }
#line 2198 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 255 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_identifier((yyvsp[0].str), pParseData); }
#line 2204 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 256 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create(kAST_Self, pParseData); }
#line 2210 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 257 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_int_literal((yyvsp[0].numi), pParseData); }
#line 2216 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 258 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2222 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 262 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2228 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 264 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[-1].pSymDataType), (yyvsp[0].str), NULL, pParseData)); }
#line 2234 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 265 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[-3].pSymDataType), (yyvsp[-2].str), (yyvsp[0].pAst), pParseData)); }
#line 2240 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 267 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_string_literal((yyvsp[0].str), pParseData); }
#line 2246 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 269 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2252 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 271 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Add,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2258 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 272 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Sub,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2264 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 273 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Mul,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2270 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 274 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Div,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2276 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 275 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Mod,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2282 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 276 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_LShift, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2288 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 277 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_RShift, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2294 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 278 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Or,     (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2300 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 279 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_And,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2306 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 280 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_BitOr,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2312 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 281 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_BitXor, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2318 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 282 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_BitAnd, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2324 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 284 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_Assign,       (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2330 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 285 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_AddAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2336 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 286 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_SubAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2342 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 287 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_MulAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2348 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 288 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_DivAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2354 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 289 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_ModAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2360 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 290 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_LShiftAssign, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2366 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 291 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_RShiftAssign, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2372 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 292 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_AndAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2378 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 293 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_XorAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2384 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 294 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_assign_op(kAST_OrAssign,     (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2390 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 296 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_Not,        (yyvsp[0].pAst), pParseData); }
#line 2396 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 297 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_Complement, (yyvsp[0].pAst), pParseData); }
#line 2402 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 298 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_Negate,     (yyvsp[0].pAst), pParseData); }
#line 2408 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 300 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_hash((yyvsp[0].str), pParseData); }
#line 2414 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 302 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_PreInc, (yyvsp[0].pAst), pParseData); }
#line 2420 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 303 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_PreDec, (yyvsp[0].pAst), pParseData); }
#line 2426 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 304 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_PostInc, (yyvsp[-1].pAst), pParseData); }
#line 2432 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 305 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_unary_op(kAST_PostDec, (yyvsp[-1].pAst), pParseData); }
#line 2438 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 307 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2444 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 309 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_type_init((yyvsp[-3].dataType), (yyvsp[-1].pAst), pParseData); }
#line 2450 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 310 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_entity_or_struct_init((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2456 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 311 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_function_call((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2462 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 313 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_system_api_call((yyvsp[-3].str), (yyvsp[-1].pAst), pParseData); }
#line 2468 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 315 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create(kAST_Transform, pParseData); }
#line 2474 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 319 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_Eq,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2480 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 320 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_NEq, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2486 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 321 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_LTE, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2492 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 322 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_GTE, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2498 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 323 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_LT,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2504 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 324 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_binary_op(kAST_GT,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2510 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 325 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_symbol_ref((yyvsp[0].pAst), pParseData); }
#line 2516 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 329 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_int_literal((yyvsp[0].numi), pParseData); }
#line 2522 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 330 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_float_literal((yyvsp[0].numf), pParseData); }
#line 2528 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 331 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_int_literal(1, pParseData); }
#line 2534 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 332 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_create_int_literal(0, pParseData); }
#line 2540 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 336 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = NULL; }
#line 2546 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 337 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2552 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 341 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = NULL; }
#line 2558 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 342 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2564 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 346 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, NULL, pParseData); }
#line 2570 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 347 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, (yyvsp[0].pAst),   pParseData); }
#line 2576 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 348 "compose.y" /* yacc.c:1646  */
    { (yyval.pAst) = ast_append(kAST_FunctionParams, (yyvsp[-2].pAst),   (yyvsp[0].pAst),   pParseData); }
#line 2582 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 352 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_basic_type(pParseData, (yyvsp[0].dataType), 1, 0); }
#line 2588 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 353 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_basic_type(pParseData, (yyvsp[0].dataType), 0, 0); }
#line 2594 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 354 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type_from_dotted_id(pParseData, (yyvsp[0].pAst), 1, 0); }
#line 2600 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 355 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type_from_dotted_id(pParseData, (yyvsp[0].pAst), 0, 0); }
#line 2606 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 388 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = (yyvsp[0].pSymDataType); }
#line 2612 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 389 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type(pParseData, "entity", 1, 0); }
#line 2618 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 390 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type(pParseData, "entity", 0, 0); }
#line 2624 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 398 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = (yyvsp[0].pSymDataType); }
#line 2630 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 399 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type(pParseData, "handle", 0, 0); }
#line 2636 "compose_parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 400 "compose.y" /* yacc.c:1646  */
    { (yyval.pSymDataType) = parsedata_find_type(pParseData, "asset_handle", 0, 0); }
#line 2642 "compose_parser.c" /* yacc.c:1646  */
    break;


#line 2646 "compose_parser.c" /* yacc.c:1646  */
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
#line 403 "compose.y" /* yacc.c:1906  */



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

