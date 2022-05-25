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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 27 "compose.y"

//#define YYDEBUG 1
#include "gaen/compose/compiler.h"

#include "gaen/compose/comp_mem.h"
#define YYMALLOC COMP_ALLOC
#define YYFREE COMP_FREE

#include <stdio.h>

#line 82 "compose_parser.c"

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

#include "compose_parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_HASH = 4,                       /* HASH  */
  YYSYMBOL_STRING_LITERAL = 5,             /* STRING_LITERAL  */
  YYSYMBOL_INT_LITERAL = 6,                /* INT_LITERAL  */
  YYSYMBOL_TRUE_ = 7,                      /* TRUE_  */
  YYSYMBOL_FALSE_ = 8,                     /* FALSE_  */
  YYSYMBOL_FLOAT_LITERAL = 9,              /* FLOAT_LITERAL  */
  YYSYMBOL_VOID_ = 10,                     /* VOID_  */
  YYSYMBOL_BOOL_ = 11,                     /* BOOL_  */
  YYSYMBOL_INT_ = 12,                      /* INT_  */
  YYSYMBOL_FLOAT_ = 13,                    /* FLOAT_  */
  YYSYMBOL_COLOR = 14,                     /* COLOR  */
  YYSYMBOL_VEC2 = 15,                      /* VEC2  */
  YYSYMBOL_VEC3 = 16,                      /* VEC3  */
  YYSYMBOL_VEC4 = 17,                      /* VEC4  */
  YYSYMBOL_IVEC2 = 18,                     /* IVEC2  */
  YYSYMBOL_IVEC3 = 19,                     /* IVEC3  */
  YYSYMBOL_IVEC4 = 20,                     /* IVEC4  */
  YYSYMBOL_QUAT = 21,                      /* QUAT  */
  YYSYMBOL_MAT3 = 22,                      /* MAT3  */
  YYSYMBOL_MAT43 = 23,                     /* MAT43  */
  YYSYMBOL_MAT4 = 24,                      /* MAT4  */
  YYSYMBOL_HANDLE_ = 25,                   /* HANDLE_  */
  YYSYMBOL_ASSET = 26,                     /* ASSET  */
  YYSYMBOL_ENTITY = 27,                    /* ENTITY  */
  YYSYMBOL_STRING = 28,                    /* STRING  */
  YYSYMBOL_IF = 29,                        /* IF  */
  YYSYMBOL_SWITCH = 30,                    /* SWITCH  */
  YYSYMBOL_CASE = 31,                      /* CASE  */
  YYSYMBOL_DEFAULT = 32,                   /* DEFAULT  */
  YYSYMBOL_FOR = 33,                       /* FOR  */
  YYSYMBOL_WHILE = 34,                     /* WHILE  */
  YYSYMBOL_DO = 35,                        /* DO  */
  YYSYMBOL_BREAK = 36,                     /* BREAK  */
  YYSYMBOL_RETURN = 37,                    /* RETURN  */
  YYSYMBOL_COMPONENT = 38,                 /* COMPONENT  */
  YYSYMBOL_COMPONENTS = 39,                /* COMPONENTS  */
  YYSYMBOL_UPDATE = 40,                    /* UPDATE  */
  YYSYMBOL_INPUT_ = 41,                    /* INPUT_  */
  YYSYMBOL_ANY = 42,                       /* ANY  */
  YYSYMBOL_NONE = 43,                      /* NONE  */
  YYSYMBOL_USING = 44,                     /* USING  */
  YYSYMBOL_AS = 45,                        /* AS  */
  YYSYMBOL_CONST_ = 46,                    /* CONST_  */
  YYSYMBOL_SELF = 47,                      /* SELF  */
  YYSYMBOL_CREATOR = 48,                   /* CREATOR  */
  YYSYMBOL_PRE = 49,                       /* PRE  */
  YYSYMBOL_POST = 50,                      /* POST  */
  YYSYMBOL_VALUE = 51,                     /* VALUE  */
  YYSYMBOL_RENDERER = 52,                  /* RENDERER  */
  YYSYMBOL_ELSE = 53,                      /* ELSE  */
  YYSYMBOL_THEN = 54,                      /* THEN  */
  YYSYMBOL_55_ = 55,                       /* '='  */
  YYSYMBOL_ADD_ASSIGN = 56,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 57,                /* SUB_ASSIGN  */
  YYSYMBOL_MUL_ASSIGN = 58,                /* MUL_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 59,                /* DIV_ASSIGN  */
  YYSYMBOL_MOD_ASSIGN = 60,                /* MOD_ASSIGN  */
  YYSYMBOL_LSHIFT_ASSIGN = 61,             /* LSHIFT_ASSIGN  */
  YYSYMBOL_RSHIFT_ASSIGN = 62,             /* RSHIFT_ASSIGN  */
  YYSYMBOL_AND_ASSIGN = 63,                /* AND_ASSIGN  */
  YYSYMBOL_XOR_ASSIGN = 64,                /* XOR_ASSIGN  */
  YYSYMBOL_OR_ASSIGN = 65,                 /* OR_ASSIGN  */
  YYSYMBOL_TRANSFORM = 66,                 /* TRANSFORM  */
  YYSYMBOL_READY = 67,                     /* READY  */
  YYSYMBOL_PARENT = 68,                    /* PARENT  */
  YYSYMBOL_VISIBLE = 69,                   /* VISIBLE  */
  YYSYMBOL_OR = 70,                        /* OR  */
  YYSYMBOL_AND = 71,                       /* AND  */
  YYSYMBOL_72_ = 72,                       /* '|'  */
  YYSYMBOL_73_ = 73,                       /* '^'  */
  YYSYMBOL_74_ = 74,                       /* '&'  */
  YYSYMBOL_EQ = 75,                        /* EQ  */
  YYSYMBOL_NEQ = 76,                       /* NEQ  */
  YYSYMBOL_77_ = 77,                       /* '<'  */
  YYSYMBOL_78_ = 78,                       /* '>'  */
  YYSYMBOL_LTE = 79,                       /* LTE  */
  YYSYMBOL_GTE = 80,                       /* GTE  */
  YYSYMBOL_LSHIFT = 81,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 82,                    /* RSHIFT  */
  YYSYMBOL_83_ = 83,                       /* '+'  */
  YYSYMBOL_84_ = 84,                       /* '-'  */
  YYSYMBOL_85_ = 85,                       /* '*'  */
  YYSYMBOL_86_ = 86,                       /* '/'  */
  YYSYMBOL_87_ = 87,                       /* '%'  */
  YYSYMBOL_INC = 88,                       /* INC  */
  YYSYMBOL_DEC = 89,                       /* DEC  */
  YYSYMBOL_90_ = 90,                       /* '!'  */
  YYSYMBOL_91_ = 91,                       /* '~'  */
  YYSYMBOL_UMINUS = 92,                    /* UMINUS  */
  YYSYMBOL_93_ = 93,                       /* '.'  */
  YYSYMBOL_POSTINC = 94,                   /* POSTINC  */
  YYSYMBOL_POSTDEC = 95,                   /* POSTDEC  */
  YYSYMBOL_96_ = 96,                       /* '('  */
  YYSYMBOL_97_ = 97,                       /* '['  */
  YYSYMBOL_98_ = 98,                       /* '{'  */
  YYSYMBOL_99_ = 99,                       /* ')'  */
  YYSYMBOL_100_ = 100,                     /* ']'  */
  YYSYMBOL_101_ = 101,                     /* '}'  */
  YYSYMBOL_102_ = 102,                     /* ';'  */
  YYSYMBOL_103_ = 103,                     /* ','  */
  YYSYMBOL_104_ = 104,                     /* ':'  */
  YYSYMBOL_105_ = 105,                     /* '@'  */
  YYSYMBOL_106_ = 106,                     /* '$'  */
  YYSYMBOL_YYACCEPT = 107,                 /* $accept  */
  YYSYMBOL_cmp_file = 108,                 /* cmp_file  */
  YYSYMBOL_using_list = 109,               /* using_list  */
  YYSYMBOL_using_stmt = 110,               /* using_stmt  */
  YYSYMBOL_dotted_id = 111,                /* dotted_id  */
  YYSYMBOL_dotted_id_proc = 112,           /* dotted_id_proc  */
  YYSYMBOL_dotted_id_part = 113,           /* dotted_id_part  */
  YYSYMBOL_def_list = 114,                 /* def_list  */
  YYSYMBOL_def = 115,                      /* def  */
  YYSYMBOL_message_block = 116,            /* message_block  */
  YYSYMBOL_message_list = 117,             /* message_list  */
  YYSYMBOL_message_prop = 118,             /* message_prop  */
  YYSYMBOL_function_def = 119,             /* function_def  */
  YYSYMBOL_param_list = 120,               /* param_list  */
  YYSYMBOL_component_block = 121,          /* component_block  */
  YYSYMBOL_component_member_list = 122,    /* component_member_list  */
  YYSYMBOL_component_member = 123,         /* component_member  */
  YYSYMBOL_prop_init_list = 124,           /* prop_init_list  */
  YYSYMBOL_prop_init = 125,                /* prop_init  */
  YYSYMBOL_property_decl = 126,            /* property_decl  */
  YYSYMBOL_property_block = 127,           /* property_block  */
  YYSYMBOL_property_def_list = 128,        /* property_def_list  */
  YYSYMBOL_property_def = 129,             /* property_def  */
  YYSYMBOL_input_block = 130,              /* input_block  */
  YYSYMBOL_input_def_list = 131,           /* input_def_list  */
  YYSYMBOL_input_def = 132,                /* input_def  */
  YYSYMBOL_block = 133,                    /* block  */
  YYSYMBOL_stmt_list = 134,                /* stmt_list  */
  YYSYMBOL_stmt = 135,                     /* stmt  */
  YYSYMBOL_target_expr = 136,              /* target_expr  */
  YYSYMBOL_message_expr = 137,             /* message_expr  */
  YYSYMBOL_expr = 138,                     /* expr  */
  YYSYMBOL_cond_expr = 139,                /* cond_expr  */
  YYSYMBOL_literal = 140,                  /* literal  */
  YYSYMBOL_expr_or_empty = 141,            /* expr_or_empty  */
  YYSYMBOL_cond_expr_or_empty = 142,       /* cond_expr_or_empty  */
  YYSYMBOL_fun_params = 143,               /* fun_params  */
  YYSYMBOL_type = 144,                     /* type  */
  YYSYMBOL_const_type = 145,               /* const_type  */
  YYSYMBOL_basic_type = 146,               /* basic_type  */
  YYSYMBOL_constable_type = 147,           /* constable_type  */
  YYSYMBOL_type_ent = 148,                 /* type_ent  */
  YYSYMBOL_type_ent_handle_asset = 149     /* type_ent_handle_asset  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;


/* Second part of user prologue.  */
#line 59 "compose.y"

#define YY_NO_UNISTD_H
#include "gaen/compose/compose_scanner.h"

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

#line 283 "compose_parser.c"


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
typedef yytype_int16 yy_state_t;

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

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
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
#define YYFINAL  43
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1265

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  107
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  179
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  341

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   336


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
       2,     2,     2,    90,     2,     2,   106,    87,    74,     2,
      96,    99,    85,    83,   103,    84,    93,    86,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   104,   102,
      77,    55,    78,     2,   105,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    97,     2,   100,    73,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    98,    72,   101,    91,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    75,    76,    79,    80,
      81,    82,    88,    89,    92,    94,    95
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   124,   124,   125,   129,   130,   134,   138,   142,   143,
     147,   151,   152,   157,   158,   159,   160,   164,   165,   169,
     170,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   187,   191,   192,   193,   197,   198,   202,   203,   207,
     208,   212,   213,   214,   218,   219,   220,   221,   222,   226,
     229,   230,   234,   235,   239,   240,   241,   245,   246,   250,
     251,   255,   256,   257,   258,   262,   263,   267,   268,   272,
     273,   275,   276,   278,   280,   281,   283,   284,   286,   288,
     290,   294,   295,   296,   297,   298,   299,   300,   304,   305,
     306,   307,   311,   313,   314,   316,   318,   319,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   346,   347,   348,   350,   352,   353,   354,   355,   357,
     358,   359,   361,   362,   364,   365,   366,   370,   371,   372,
     373,   374,   375,   376,   380,   381,   382,   383,   387,   388,
     392,   393,   397,   398,   399,   403,   404,   408,   412,   413,
     414,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   439,   440,   448,   449,   450
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER", "HASH",
  "STRING_LITERAL", "INT_LITERAL", "TRUE_", "FALSE_", "FLOAT_LITERAL",
  "VOID_", "BOOL_", "INT_", "FLOAT_", "COLOR", "VEC2", "VEC3", "VEC4",
  "IVEC2", "IVEC3", "IVEC4", "QUAT", "MAT3", "MAT43", "MAT4", "HANDLE_",
  "ASSET", "ENTITY", "STRING", "IF", "SWITCH", "CASE", "DEFAULT", "FOR",
  "WHILE", "DO", "BREAK", "RETURN", "COMPONENT", "COMPONENTS", "UPDATE",
  "INPUT_", "ANY", "NONE", "USING", "AS", "CONST_", "SELF", "CREATOR",
  "PRE", "POST", "VALUE", "RENDERER", "ELSE", "THEN", "'='", "ADD_ASSIGN",
  "SUB_ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "LSHIFT_ASSIGN",
  "RSHIFT_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN", "OR_ASSIGN", "TRANSFORM",
  "READY", "PARENT", "VISIBLE", "OR", "AND", "'|'", "'^'", "'&'", "EQ",
  "NEQ", "'<'", "'>'", "LTE", "GTE", "LSHIFT", "RSHIFT", "'+'", "'-'",
  "'*'", "'/'", "'%'", "INC", "DEC", "'!'", "'~'", "UMINUS", "'.'",
  "POSTINC", "POSTDEC", "'('", "'['", "'{'", "')'", "']'", "'}'", "';'",
  "','", "':'", "'@'", "'$'", "$accept", "cmp_file", "using_list",
  "using_stmt", "dotted_id", "dotted_id_proc", "dotted_id_part",
  "def_list", "def", "message_block", "message_list", "message_prop",
  "function_def", "param_list", "component_block", "component_member_list",
  "component_member", "prop_init_list", "prop_init", "property_decl",
  "property_block", "property_def_list", "property_def", "input_block",
  "input_def_list", "input_def", "block", "stmt_list", "stmt",
  "target_expr", "message_expr", "expr", "cond_expr", "literal",
  "expr_or_empty", "cond_expr_or_empty", "fun_params", "type",
  "const_type", "basic_type", "constable_type", "type_ent",
  "type_ent_handle_asset", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-161)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-157)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     692,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,
    -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,    10,
    -161,    34,    39,   483,    50,   692,  -161,   -40,  -161,  -161,
     729,  -161,  -161,  -161,    98,  -161,  -161,  -161,   117,    25,
      25,   -36,  -161,  -161,  -161,   729,    39,  -161,    73,    45,
     628,  -161,  -161,    39,  -161,   531,  1180,    49,  -161,    56,
      57,   155,  -161,   660,  -161,  -161,    41,   131,   -83,  -161,
    -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,   531,   531,
     531,   531,   531,   531,    68,   505,   706,  -161,  -161,    67,
     165,   -65,   168,  1180,     3,  -161,   208,  -161,    74,  -161,
    -161,   531,   -10,  -161,  -161,   -44,  -161,  -161,  -161,    60,
      60,    60,    60,   970,   170,   531,   531,   531,   531,   531,
     531,   531,   531,   531,   531,   531,   531,   114,   531,   531,
     531,   531,   531,   531,   531,   531,   531,   531,   531,   531,
     531,   531,   531,   531,   531,   531,  -161,  -161,  -161,   531,
     124,    57,  1180,  -161,   -54,  -161,   -57,    -8,  -161,    88,
      92,    99,   427,   531,  -161,   186,  -161,   323,  -161,   739,
       1,  -161,   772,   138,    57,    57,  -161,    -2,  -161,   531,
    -161,  -161,   101,  1139,  1139,  1139,  1139,  1139,  1139,  1139,
    1139,  1139,  1139,  1139,  1139,    15,   139,   143,   146,   148,
     184,    -3,  -161,  1158,  1176,   202,   298,   315,   401,   401,
     503,   503,   503,   503,   232,   232,   175,   175,    60,    60,
      60,     8,   531,  -161,   207,    57,   114,  -161,    39,   531,
     531,   531,   206,   805,  -161,  -161,  -161,  -161,  -161,  -161,
      -1,  -161,  -161,  -161,   -69,    57,    57,  -161,    12,  -161,
    -161,   531,  -161,  -161,  -161,  -161,   838,   531,  -161,   531,
     531,   531,   531,   531,   531,  -161,   114,  -161,  1139,  -161,
    -161,    43,  -161,  1000,  1139,   149,  1030,   157,  -161,   -37,
       9,   248,  -161,  -161,  -161,  -161,  -161,   871,  -161,    22,
    1139,  1139,  1139,  1139,  1139,  1139,  -161,  -161,   427,   531,
     427,   531,   531,   531,  -161,  -161,  -161,   531,   -24,    57,
    -161,  -161,   212,  1139,   166,   167,  -161,  1060,   904,    27,
    1090,   531,   531,  -161,   427,   531,   169,  -161,   171,  -161,
     937,    30,  -161,   173,  -161,  -161,  -161,   191,   427,  -161,
    -161
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    10,   158,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   178,   179,     0,
     159,     0,     0,     0,     0,     0,     4,   156,     7,     8,
       2,    11,    15,   175,     0,   155,   160,   177,     0,     0,
       0,     0,   157,     1,     5,     3,     0,    12,     0,     0,
       0,    13,    14,     0,     9,     0,    32,     0,   176,     0,
       0,     0,    17,     0,    19,    30,     0,     0,     0,   124,
      96,   144,   146,   147,   145,   135,   136,   134,     0,     0,
       0,     0,     0,     0,     0,   143,     0,    95,    97,   155,
       0,     0,     0,    32,     0,    27,     0,    28,     0,    18,
      20,     0,     0,    24,    22,     0,    49,     6,   123,   125,
     126,   121,   122,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   152,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   127,   128,    16,   152,
      93,     0,     0,    33,     0,    35,    39,     0,    37,     0,
       0,     0,     0,     0,    65,    81,    79,     0,    67,     0,
       0,    29,     0,     0,     0,     0,    50,     0,    52,     0,
      26,    92,   132,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   153,     0,     0,     0,     0,     0,
       0,     0,    42,   105,   106,   107,   108,   109,   137,   138,
     141,   142,   139,   140,   103,   104,    98,    99,   100,   101,
     102,     0,     0,    31,     0,     0,    41,    36,     0,     0,
     148,     0,     0,     0,    82,    86,    83,    84,    85,    87,
       0,    66,    68,    80,     0,     0,     0,    57,     0,    59,
      23,     0,    55,    56,    51,    53,     0,   152,   131,     0,
       0,     0,     0,     0,     0,   130,     0,   129,    94,    34,
      21,     0,    38,     0,   149,     0,     0,     0,    78,     0,
       0,     0,    61,    63,    64,    58,    60,     0,    25,     0,
     154,    44,    45,    46,    47,    48,    43,    40,     0,   150,
       0,     0,     0,   152,    88,    90,    89,     0,     0,     0,
      54,   133,    69,     0,    95,     0,    71,     0,     0,     0,
       0,     0,   152,    62,     0,   148,     0,    74,     0,    91,
       0,     0,    70,     0,    72,    75,    76,     0,     0,    77,
      73
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -161,  -161,  -161,   269,    97,  -161,   249,   275,   -13,   261,
    -161,   239,   -30,   210,  -161,  -161,    77,    81,    42,  -161,
    -161,  -161,   133,  -161,  -161,    63,   -59,  -161,  -160,  -161,
    -161,   -55,    13,  -161,    23,  -161,  -145,  -161,  -161,   106,
     299,   107,   -42
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    24,    25,    26,    85,    28,    29,    30,    31,    51,
      63,    64,    32,    91,    95,   157,   158,   201,   202,    66,
     104,   177,   178,   171,   248,   249,   166,   167,   168,   240,
     308,   169,    87,    88,   275,   315,   195,    33,    34,    89,
      36,    90,    38
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      86,    97,   232,   279,   221,   244,     1,   242,    67,    53,
      46,   179,   304,    39,    92,   305,   244,    47,   302,   107,
      65,    67,   173,   108,   109,   110,   111,   112,   113,    96,
     173,   321,    47,    65,   151,   281,    46,    40,   152,   174,
     175,   226,     1,   245,   246,   225,   172,   174,   175,   152,
      43,    92,    56,    46,   245,   246,   306,    46,   180,   303,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   322,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   176,   223,   227,   194,   228,   101,    27,   265,   254,
     266,    48,   247,   280,   155,   307,    35,    37,   233,   267,
     224,   259,   289,   285,   258,   252,   253,   196,   259,    41,
      49,   311,    27,    50,   256,   259,   328,    27,    55,   337,
     259,    35,    37,   259,   105,   106,    35,    37,   312,   102,
     316,    56,    27,   103,   297,    93,   266,    27,   146,   147,
      68,    35,    37,    27,    94,    96,    35,    37,   319,    98,
      27,   114,    35,    37,   332,   149,   270,   268,   150,    35,
      37,   153,   170,   182,   273,   274,   276,   331,   340,   222,
     197,   198,   199,   200,   229,   282,   283,   284,   230,   234,
      27,   156,   235,   251,   260,   231,   287,   257,   261,    35,
      37,   262,   194,   263,   290,   291,   292,   293,   294,   295,
     269,     1,    69,    70,    71,    72,    73,    74,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,   236,   237,    58,    20,   159,   238,   264,
     277,   160,   161,   162,   313,   163,   317,   318,   194,    27,
     323,   299,   320,   301,   239,    75,    76,   309,    35,    37,
     143,   144,   145,   146,   147,   324,   330,   194,  -151,   325,
     274,   334,   338,   335,    77,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,    78,   339,    44,    54,    79,    80,    81,    82,
      45,    52,   100,   154,    83,   272,    96,   271,   296,   164,
     255,   286,   314,   165,    84,   141,   142,   143,   144,   145,
     146,   147,    42,     0,     0,   156,     1,    69,    70,    71,
      72,    73,    74,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,   333,     0,
      58,    20,   159,     0,     0,     0,   160,   161,   162,     0,
     163,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,     0,    77,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,     0,     0,    78,     0,     0,
       0,    79,    80,    81,    82,     0,     0,     0,     0,    83,
       0,    96,     0,     0,   241,     0,     0,     0,   165,    84,
       1,    69,    70,    71,    72,    73,    74,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,     0,     0,    58,    20,   159,     0,     0,     0,
     160,   161,   162,     0,   163,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,  -157,  -157,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,     0,     0,    77,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,  -156,     0,
       0,    78,     0,     0,     0,    79,    80,    81,    82,     0,
       0,     0,     0,    83,     0,    96,     0,     0,     0,     0,
       0,     0,   165,    84,     1,    69,    70,    71,    72,    73,
      74,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,     0,     0,    58,    20,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,     0,     0,     0,     0,     0,     0,     0,    75,    76,
    -157,  -157,  -157,  -157,   139,   140,   141,   142,   143,   144,
     145,   146,   147,     0,     0,     0,     0,    77,    46,     0,
       0,   126,     0,   127,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    78,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     1,    57,     0,     0,     0,     0,    84,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    58,    20,     0,     0,     0,
       0,     0,     0,     1,    57,     0,     0,    59,    60,    61,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    58,    20,     0,
       0,     0,     0,     0,     0,     1,     0,     0,     0,    59,
      60,    61,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      21,     0,     1,     0,     0,     0,    22,     0,    23,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,    99,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    23,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   148,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   250,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   278,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     288,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   310,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   327,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   336,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   181,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   300,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   326,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
       0,     0,     0,     1,     0,     0,     0,     0,     0,   329,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    58,    20,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147
};

static const yytype_int16 yycheck[] =
{
      55,    60,   162,     4,   149,     4,     3,   167,    50,    45,
      93,    55,     3,     3,    56,     6,     4,    30,    55,   102,
      50,    63,    32,    78,    79,    80,    81,    82,    83,    98,
      32,    55,    45,    63,    99,   104,    93,     3,   103,    49,
      50,    98,     3,    42,    43,    99,   101,    49,    50,   103,
       0,    93,    96,    93,    42,    43,    47,    93,   102,    96,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    96,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   101,   151,   101,   149,   103,    55,     0,   101,   101,
     103,     3,   101,   104,   101,    96,     0,     0,   163,   101,
     152,   103,   257,   101,    99,   174,   175,     3,   103,    22,
       3,    99,    25,    98,   179,   103,    99,    30,    55,    99,
     103,    25,    25,   103,     3,     4,    30,    30,   298,    98,
     300,    96,    45,   102,   101,    96,   103,    50,    88,    89,
      53,    45,    45,    56,    98,    98,    50,    50,   303,     4,
      63,    93,    56,    56,   324,    98,   225,   222,     3,    63,
      63,     3,    98,     3,   229,   230,   231,   322,   338,    55,
      66,    67,    68,    69,    96,   244,   245,   246,    96,     3,
      93,    94,     6,    55,    55,    96,   251,    96,    55,    93,
      93,    55,   257,    55,   259,   260,   261,   262,   263,   264,
       3,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    47,    48,    27,    28,    29,    52,    55,
      34,    33,    34,    35,   299,    37,   301,   302,   303,   152,
     309,   102,   307,    96,    68,    47,    48,     9,   152,   152,
      85,    86,    87,    88,    89,    53,   321,   322,   102,   102,
     325,   102,    99,   102,    66,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    84,   102,    25,    46,    88,    89,    90,    91,
      25,    40,    63,    93,    96,   228,    98,   226,   266,   101,
     177,   248,   299,   105,   106,    83,    84,    85,    86,    87,
      88,    89,    23,    -1,    -1,   228,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   325,    -1,
      27,    28,    29,    -1,    -1,    -1,    33,    34,    35,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    48,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    66,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    -1,    84,    -1,    -1,
      -1,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    96,
      -1,    98,    -1,    -1,   101,    -1,    -1,    -1,   105,   106,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      33,    34,    35,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    48,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    66,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     3,    -1,
      -1,    84,    -1,    -1,    -1,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    96,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    27,    28,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    48,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,    66,    93,    -1,
      -1,    96,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    88,
      89,    90,    91,    -1,    -1,    -1,    -1,    96,    -1,    -1,
      -1,     3,     4,    -1,    -1,    -1,    -1,   106,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,    -1,    -1,    39,    40,    41,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    39,
      40,    41,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
      38,    -1,     3,    -1,    -1,    -1,    44,    -1,    46,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    -1,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    99,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    38,    44,    46,   108,   109,   110,   111,   112,   113,
     114,   115,   119,   144,   145,   146,   147,   148,   149,     3,
       3,   111,   147,     0,   110,   114,    93,   115,     3,     3,
      98,   116,   116,    45,   113,    55,    96,     4,    27,    39,
      40,    41,   101,   117,   118,   119,   126,   149,   111,     4,
       5,     6,     7,     8,     9,    47,    48,    66,    84,    88,
      89,    90,    91,    96,   106,   111,   138,   139,   140,   146,
     148,   120,   149,    96,    98,   121,    98,   133,     4,   101,
     118,    55,    98,   102,   127,     3,     4,   102,   138,   138,
     138,   138,   138,   138,    93,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    96,    98,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,   102,    98,
       3,    99,   103,     3,   120,   101,   111,   122,   123,    29,
      33,    34,    35,    37,   101,   105,   133,   134,   135,   138,
      98,   130,   138,    32,    49,    50,   101,   128,   129,    55,
     102,    99,     3,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   143,     3,    66,    67,    68,
      69,   124,   125,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   143,    55,   133,   149,    99,    98,   101,   103,    96,
      96,    96,   135,   138,     3,     6,    47,    48,    52,    68,
     136,   101,   135,   102,     4,    42,    43,   101,   131,   132,
     102,    55,   133,   133,   101,   129,   138,    96,    99,   103,
      55,    55,    55,    55,    55,   101,   103,   101,   138,     3,
     133,   124,   123,   138,   138,   141,   138,    34,   102,     4,
     104,   104,   133,   133,   133,   101,   132,   138,   102,   143,
     138,   138,   138,   138,   138,   138,   125,   101,    99,   102,
      99,    96,    55,    96,     3,     6,    47,    96,   137,     9,
     102,    99,   135,   138,   139,   142,   135,   138,   138,   143,
     138,    55,    96,   133,    53,   102,    99,   102,    99,    99,
     138,   143,   135,   141,   102,   102,   102,    99,    99,   102,
     135
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   107,   108,   108,   109,   109,   110,   111,   112,   112,
     113,   114,   114,   115,   115,   115,   115,   116,   116,   117,
     117,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   119,   120,   120,   120,   121,   121,   122,   122,   123,
     123,   124,   124,   124,   125,   125,   125,   125,   125,   126,
     127,   127,   128,   128,   129,   129,   129,   130,   130,   131,
     131,   132,   132,   132,   132,   133,   133,   134,   134,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   136,   136,   136,   136,   136,   136,   136,   137,   137,
     137,   137,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   139,   139,   139,
     139,   139,   139,   139,   140,   140,   140,   140,   141,   141,
     142,   142,   143,   143,   143,   144,   144,   145,   146,   146,
     146,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   148,   148,   149,   149,   149
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     5,     1,     1,     3,
       1,     1,     2,     3,     3,     1,     5,     2,     3,     1,
       2,     5,     2,     4,     2,     5,     3,     2,     2,     3,
       1,     6,     0,     2,     4,     2,     3,     1,     3,     1,
       4,     0,     1,     3,     3,     3,     3,     3,     3,     2,
       2,     3,     1,     2,     4,     2,     2,     2,     3,     1,
       2,     2,     4,     2,     2,     2,     3,     1,     2,     5,
       7,     5,     7,     9,     6,     7,     7,     8,     3,     1,
       2,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     4,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     1,     2,     2,     2,     2,     4,
       4,     4,     3,     6,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     0,     1,
       0,     1,     0,     1,     3,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
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
        yyerror (&yylloc, pParseData, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
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

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, pParseData); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParseData * pParseData)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (pParseData);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParseData * pParseData)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, pParseData);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, ParseData * pParseData)
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), pParseData);
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
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
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
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
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
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
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
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
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ParseData * pParseData)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (pParseData);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (ParseData * pParseData)
{
/* Lookahead token kind.  */
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
    int yynerrs = 0;

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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = yylex (&yylval, &yylloc, SCANNER_FROM_PARSEDATA);
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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 3: /* cmp_file: using_list def_list  */
#line 125 "compose.y"
                           { ast_create_using_list((yyvsp[-1].pAst), pParseData); }
#line 2065 "compose_parser.c"
    break;

  case 4: /* using_list: using_stmt  */
#line 129 "compose.y"
                             { (yyval.pAst) = ast_append(kAST_UsingList, NULL, (yyvsp[0].pAst), pParseData); }
#line 2071 "compose_parser.c"
    break;

  case 5: /* using_list: using_list using_stmt  */
#line 130 "compose.y"
                             { (yyval.pAst) = ast_append(kAST_UsingList, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 2077 "compose_parser.c"
    break;

  case 6: /* using_stmt: USING dotted_id AS dotted_id ';'  */
#line 134 "compose.y"
                                        { (yyval.pAst) = ast_create_using_stmt((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2083 "compose_parser.c"
    break;

  case 7: /* dotted_id: dotted_id_proc  */
#line 138 "compose.y"
                      { (yyval.pAst) = ast_create_dotted_id((yyvsp[0].pAst), pParseData); }
#line 2089 "compose_parser.c"
    break;

  case 8: /* dotted_id_proc: dotted_id_part  */
#line 142 "compose.y"
                                    { (yyval.pAst) = ast_append(kAST_DottedId, NULL, (yyvsp[0].pAst), pParseData); }
#line 2095 "compose_parser.c"
    break;

  case 9: /* dotted_id_proc: dotted_id '.' dotted_id_part  */
#line 143 "compose.y"
                                    { (yyval.pAst) = ast_append(kAST_DottedId, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2101 "compose_parser.c"
    break;

  case 10: /* dotted_id_part: IDENTIFIER  */
#line 147 "compose.y"
                 { (yyval.pAst) = ast_create_with_str(kAST_DottedIdPart, (yyvsp[0].str), pParseData); }
#line 2107 "compose_parser.c"
    break;

  case 13: /* def: ENTITY IDENTIFIER message_block  */
#line 157 "compose.y"
                                                        { (yyval.pAst) = ast_create_entity_def((yyvsp[-1].str), (yyvsp[0].pAst), pParseData); }
#line 2113 "compose_parser.c"
    break;

  case 14: /* def: COMPONENT IDENTIFIER message_block  */
#line 158 "compose.y"
                                                        { (yyval.pAst) = ast_create_component_def((yyvsp[-1].str), (yyvsp[0].pAst), pParseData); }
#line 2119 "compose_parser.c"
    break;

  case 15: /* def: function_def  */
#line 159 "compose.y"
                                                        { (yyval.pAst) = parsedata_add_root_ast(pParseData, (yyvsp[0].pAst)); }
#line 2125 "compose_parser.c"
    break;

  case 16: /* def: const_type IDENTIFIER '=' expr ';'  */
#line 160 "compose.y"
                                                        { (yyval.pAst) = ast_create_global_const_def((yyvsp[-3].str), (yyvsp[-4].pSymDataType), (yyvsp[-1].pAst), pParseData); }
#line 2131 "compose_parser.c"
    break;

  case 17: /* message_block: '{' '}'  */
#line 164 "compose.y"
                            { (yyval.pAst) = ast_create_block(NULL, pParseData); }
#line 2137 "compose_parser.c"
    break;

  case 18: /* message_block: '{' message_list '}'  */
#line 165 "compose.y"
                            { (yyval.pAst) = ast_create_block((yyvsp[-1].pAst),   pParseData); }
#line 2143 "compose_parser.c"
    break;

  case 19: /* message_list: message_prop  */
#line 169 "compose.y"
                                { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[0].pAst), pParseData); }
#line 2149 "compose_parser.c"
    break;

  case 20: /* message_list: message_list message_prop  */
#line 170 "compose.y"
                                { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 2155 "compose_parser.c"
    break;

  case 21: /* message_prop: HASH '(' param_list ')' block  */
#line 174 "compose.y"
                                                    { (yyval.pAst) = ast_create_message_def((yyvsp[-4].str), (yyvsp[-2].pSymTab), (yyvsp[0].pAst), pParseData); }
#line 2161 "compose_parser.c"
    break;

  case 22: /* message_prop: property_decl property_block  */
#line 175 "compose.y"
                                                    { (yyval.pAst) = ast_create_property_complex_def((yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 2167 "compose_parser.c"
    break;

  case 23: /* message_prop: property_decl '=' expr ';'  */
#line 176 "compose.y"
                                                    { (yyval.pAst) = ast_create_property_def((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2173 "compose_parser.c"
    break;

  case 24: /* message_prop: property_decl ';'  */
#line 177 "compose.y"
                                                    { (yyval.pAst) = ast_create_property_def((yyvsp[-1].pAst), NULL, pParseData); }
#line 2179 "compose_parser.c"
    break;

  case 25: /* message_prop: type_ent_handle_asset IDENTIFIER '=' expr ';'  */
#line 178 "compose.y"
                                                    { (yyval.pAst) = ast_create_field_def((yyvsp[-3].str), (yyvsp[-4].pSymDataType), (yyvsp[-1].pAst), pParseData); }
#line 2185 "compose_parser.c"
    break;

  case 26: /* message_prop: type_ent_handle_asset IDENTIFIER ';'  */
#line 179 "compose.y"
                                                    { (yyval.pAst) = ast_create_field_def((yyvsp[-1].str), (yyvsp[-2].pSymDataType), NULL, pParseData); }
#line 2191 "compose_parser.c"
    break;

  case 27: /* message_prop: COMPONENTS component_block  */
#line 180 "compose.y"
                                                    { (yyval.pAst) = ast_create_component_members((yyvsp[0].pAst), pParseData); }
#line 2197 "compose_parser.c"
    break;

  case 28: /* message_prop: UPDATE block  */
#line 181 "compose.y"
                                                        { (yyval.pAst) = ast_create_update_def((yyvsp[0].pAst), pParseData); }
#line 2203 "compose_parser.c"
    break;

  case 29: /* message_prop: INPUT_ HASH input_block  */
#line 182 "compose.y"
                                                        { (yyval.pAst) = ast_create_input_mode((yyvsp[-1].str), (yyvsp[0].pAst), pParseData); }
#line 2209 "compose_parser.c"
    break;

  case 30: /* message_prop: function_def  */
#line 183 "compose.y"
                                                    { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2215 "compose_parser.c"
    break;

  case 31: /* function_def: type_ent_handle_asset IDENTIFIER '(' param_list ')' block  */
#line 187 "compose.y"
                                                                  { (yyval.pAst) = ast_create_function_def((yyvsp[-4].str), (yyvsp[-5].pSymDataType), (yyvsp[-2].pSymTab), (yyvsp[0].pAst), pParseData); }
#line 2221 "compose_parser.c"
    break;

  case 32: /* param_list: %empty  */
#line 191 "compose.y"
                                                       { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, NULL); }
#line 2227 "compose_parser.c"
    break;

  case 33: /* param_list: type_ent_handle_asset IDENTIFIER  */
#line 192 "compose.y"
                                                       { (yyval.pSymTab) = parsedata_add_param(pParseData, NULL, symrec_create(kSYMT_Param, (yyvsp[-1].pSymDataType), (yyvsp[0].str), NULL, NULL, pParseData)); }
#line 2233 "compose_parser.c"
    break;

  case 34: /* param_list: param_list ',' type_ent_handle_asset IDENTIFIER  */
#line 193 "compose.y"
                                                       { (yyval.pSymTab) = parsedata_add_param(pParseData, (yyvsp[-3].pSymTab), symrec_create(kSYMT_Param, (yyvsp[-1].pSymDataType), (yyvsp[0].str), NULL, NULL, pParseData)); }
#line 2239 "compose_parser.c"
    break;

  case 35: /* component_block: '{' '}'  */
#line 197 "compose.y"
                                     { (yyval.pAst) = ast_create_with_child_list(kAST_ComponentMemberList, pParseData); }
#line 2245 "compose_parser.c"
    break;

  case 36: /* component_block: '{' component_member_list '}'  */
#line 198 "compose.y"
                                     { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2251 "compose_parser.c"
    break;

  case 37: /* component_member_list: component_member  */
#line 202 "compose.y"
                                                 { (yyval.pAst) = ast_append(kAST_ComponentMemberList, NULL, (yyvsp[0].pAst), pParseData); }
#line 2257 "compose_parser.c"
    break;

  case 38: /* component_member_list: component_member_list ',' component_member  */
#line 203 "compose.y"
                                                 { (yyval.pAst) = ast_append(kAST_ComponentMemberList, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2263 "compose_parser.c"
    break;

  case 39: /* component_member: dotted_id  */
#line 207 "compose.y"
                                       { (yyval.pAst) = ast_create_component_member((yyvsp[0].pAst), ast_create(kAST_PropInit, pParseData), pParseData); }
#line 2269 "compose_parser.c"
    break;

  case 40: /* component_member: dotted_id '{' prop_init_list '}'  */
#line 208 "compose.y"
                                       { (yyval.pAst) = ast_create_component_member((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2275 "compose_parser.c"
    break;

  case 41: /* prop_init_list: %empty  */
#line 212 "compose.y"
                                    { (yyval.pAst) = ast_create(kAST_PropInitList, pParseData); }
#line 2281 "compose_parser.c"
    break;

  case 42: /* prop_init_list: prop_init  */
#line 213 "compose.y"
                                    { (yyval.pAst) = ast_append(kAST_PropInitList, NULL, (yyvsp[0].pAst), pParseData); }
#line 2287 "compose_parser.c"
    break;

  case 43: /* prop_init_list: prop_init_list ',' prop_init  */
#line 214 "compose.y"
                                    { (yyval.pAst) = ast_append(kAST_PropInitList, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2293 "compose_parser.c"
    break;

  case 44: /* prop_init: IDENTIFIER '=' expr  */
#line 218 "compose.y"
                           { (yyval.pAst) = ast_create_prop_init((yyvsp[-2].str), (yyvsp[0].pAst), pParseData); }
#line 2299 "compose_parser.c"
    break;

  case 45: /* prop_init: TRANSFORM '=' expr  */
#line 219 "compose.y"
                           { (yyval.pAst) = ast_create_transform_init((yyvsp[0].pAst), pParseData); }
#line 2305 "compose_parser.c"
    break;

  case 46: /* prop_init: READY '=' expr  */
#line 220 "compose.y"
                           { (yyval.pAst) = ast_create_ready_init((yyvsp[0].pAst), pParseData); }
#line 2311 "compose_parser.c"
    break;

  case 47: /* prop_init: PARENT '=' expr  */
#line 221 "compose.y"
                           { (yyval.pAst) = ast_create_parent_init((yyvsp[0].pAst), pParseData); }
#line 2317 "compose_parser.c"
    break;

  case 48: /* prop_init: VISIBLE '=' expr  */
#line 222 "compose.y"
                           { (yyval.pAst) = ast_create_visible_init((yyvsp[0].pAst), pParseData); }
#line 2323 "compose_parser.c"
    break;

  case 49: /* property_decl: type_ent_handle_asset HASH  */
#line 226 "compose.y"
                                    { (yyval.pAst) = ast_create_property_decl((yyvsp[0].str), (yyvsp[-1].pSymDataType), pParseData); }
#line 2329 "compose_parser.c"
    break;

  case 50: /* property_block: '{' '}'  */
#line 229 "compose.y"
                                 { (yyval.pAst) = ast_create_with_child_list(kAST_PropertyDef, pParseData); }
#line 2335 "compose_parser.c"
    break;

  case 51: /* property_block: '{' property_def_list '}'  */
#line 230 "compose.y"
                                 { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2341 "compose_parser.c"
    break;

  case 52: /* property_def_list: property_def  */
#line 234 "compose.y"
                                      { (yyval.pAst) = ast_append(kAST_PropertyDef, NULL, (yyvsp[0].pAst), pParseData); }
#line 2347 "compose_parser.c"
    break;

  case 53: /* property_def_list: property_def_list property_def  */
#line 235 "compose.y"
                                      { (yyval.pAst) = ast_append(kAST_PropertyDef, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 2353 "compose_parser.c"
    break;

  case 54: /* property_def: DEFAULT '=' expr ';'  */
#line 239 "compose.y"
                            { (yyval.pAst) = ast_create_property_default_assign((yyvsp[-1].pAst), pParseData); }
#line 2359 "compose_parser.c"
    break;

  case 55: /* property_def: PRE block  */
#line 240 "compose.y"
                            { (yyval.pAst) = ast_create_property_pre((yyvsp[0].pAst), pParseData); }
#line 2365 "compose_parser.c"
    break;

  case 56: /* property_def: POST block  */
#line 241 "compose.y"
                            { (yyval.pAst) = ast_create_property_post((yyvsp[0].pAst), pParseData); }
#line 2371 "compose_parser.c"
    break;

  case 57: /* input_block: '{' '}'  */
#line 245 "compose.y"
                              { (yyval.pAst) = ast_create_with_child_list(kAST_Input, pParseData); }
#line 2377 "compose_parser.c"
    break;

  case 58: /* input_block: '{' input_def_list '}'  */
#line 246 "compose.y"
                              { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2383 "compose_parser.c"
    break;

  case 59: /* input_def_list: input_def  */
#line 250 "compose.y"
                                { (yyval.pAst) = ast_append(kAST_Input, NULL, (yyvsp[0].pAst), pParseData); }
#line 2389 "compose_parser.c"
    break;

  case 60: /* input_def_list: input_def_list input_def  */
#line 251 "compose.y"
                                { (yyval.pAst) = ast_append(kAST_Input, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 2395 "compose_parser.c"
    break;

  case 61: /* input_def: HASH block  */
#line 255 "compose.y"
                                   { (yyval.pAst) = ast_create_input_def((yyvsp[-1].str), 0.0, (yyvsp[0].pAst), pParseData); }
#line 2401 "compose_parser.c"
    break;

  case 62: /* input_def: HASH ':' FLOAT_LITERAL block  */
#line 256 "compose.y"
                                   { (yyval.pAst) = ast_create_input_def((yyvsp[-3].str), (yyvsp[-1].numf), (yyvsp[0].pAst), pParseData); }
#line 2407 "compose_parser.c"
    break;

  case 63: /* input_def: ANY block  */
#line 257 "compose.y"
                                   { (yyval.pAst) = ast_create_input_special_def("any", (yyvsp[0].pAst), pParseData); }
#line 2413 "compose_parser.c"
    break;

  case 64: /* input_def: NONE block  */
#line 258 "compose.y"
                                   { (yyval.pAst) = ast_create_input_special_def("none", (yyvsp[0].pAst), pParseData); }
#line 2419 "compose_parser.c"
    break;

  case 65: /* block: '{' '}'  */
#line 262 "compose.y"
                            { (yyval.pAst) = ast_create_block(NULL, pParseData); }
#line 2425 "compose_parser.c"
    break;

  case 66: /* block: '{' stmt_list '}'  */
#line 263 "compose.y"
                            { (yyval.pAst) = ast_create_block((yyvsp[-1].pAst),   pParseData); }
#line 2431 "compose_parser.c"
    break;

  case 67: /* stmt_list: stmt  */
#line 267 "compose.y"
                      { (yyval.pAst) = ast_append(kAST_Block, NULL, (yyvsp[0].pAst), pParseData); }
#line 2437 "compose_parser.c"
    break;

  case 68: /* stmt_list: stmt_list stmt  */
#line 268 "compose.y"
                      { (yyval.pAst) = ast_append(kAST_Block, (yyvsp[-1].pAst), (yyvsp[0].pAst), pParseData); }
#line 2443 "compose_parser.c"
    break;

  case 69: /* stmt: IF '(' expr ')' stmt  */
#line 272 "compose.y"
                                              { (yyval.pAst) = ast_create_if((yyvsp[-2].pAst), (yyvsp[0].pAst), NULL, pParseData); }
#line 2449 "compose_parser.c"
    break;

  case 70: /* stmt: IF '(' expr ')' stmt ELSE stmt  */
#line 273 "compose.y"
                                     { (yyval.pAst) = ast_create_if((yyvsp[-4].pAst), (yyvsp[-2].pAst), (yyvsp[0].pAst),   pParseData); }
#line 2455 "compose_parser.c"
    break;

  case 71: /* stmt: WHILE '(' expr ')' stmt  */
#line 275 "compose.y"
                                      { (yyval.pAst) = ast_create_while((yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2461 "compose_parser.c"
    break;

  case 72: /* stmt: DO stmt WHILE '(' expr ')' ';'  */
#line 276 "compose.y"
                                      { (yyval.pAst) = ast_create_dowhile((yyvsp[-2].pAst), (yyvsp[-5].pAst), pParseData); }
#line 2467 "compose_parser.c"
    break;

  case 73: /* stmt: FOR '(' expr_or_empty ';' cond_expr_or_empty ';' expr_or_empty ')' stmt  */
#line 278 "compose.y"
                                                                              { (yyval.pAst) = ast_create_for((yyvsp[-6].pAst), (yyvsp[-4].pAst), (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2473 "compose_parser.c"
    break;

  case 74: /* stmt: '@' target_expr HASH '=' expr ';'  */
#line 280 "compose.y"
                                                   { (yyval.pAst) = ast_create_property_set((yyvsp[-4].pAst), ast_create_hash((yyvsp[-3].str), pParseData), (yyvsp[-1].pAst), pParseData); }
#line 2479 "compose_parser.c"
    break;

  case 75: /* stmt: '@' target_expr HASH '(' fun_params ')' ';'  */
#line 281 "compose.y"
                                                   { (yyval.pAst) = ast_create_message_send((yyvsp[-5].pAst), ast_create_hash((yyvsp[-4].str), pParseData), (yyvsp[-2].pAst), pParseData); }
#line 2485 "compose_parser.c"
    break;

  case 76: /* stmt: '@' target_expr ':' message_expr '=' expr ';'  */
#line 283 "compose.y"
                                                               { (yyval.pAst) = ast_create_property_set((yyvsp[-5].pAst), (yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2491 "compose_parser.c"
    break;

  case 77: /* stmt: '@' target_expr ':' message_expr '(' fun_params ')' ';'  */
#line 284 "compose.y"
                                                               { (yyval.pAst) = ast_create_message_send((yyvsp[-6].pAst), (yyvsp[-4].pAst), (yyvsp[-2].pAst), pParseData); }
#line 2497 "compose_parser.c"
    break;

  case 78: /* stmt: RETURN expr ';'  */
#line 286 "compose.y"
                       { (yyval.pAst) = ast_create_return((yyvsp[-1].pAst), pParseData); }
#line 2503 "compose_parser.c"
    break;

  case 79: /* stmt: block  */
#line 288 "compose.y"
                { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2509 "compose_parser.c"
    break;

  case 80: /* stmt: expr ';'  */
#line 290 "compose.y"
                { (yyval.pAst) = ast_create_simple_stmt((yyvsp[-1].pAst), pParseData); }
#line 2515 "compose_parser.c"
    break;

  case 81: /* target_expr: %empty  */
#line 294 "compose.y"
                   { (yyval.pAst) = NULL; }
#line 2521 "compose_parser.c"
    break;

  case 82: /* target_expr: IDENTIFIER  */
#line 295 "compose.y"
                   { (yyval.pAst) = ast_create_identifier((yyvsp[0].str), pParseData); }
#line 2527 "compose_parser.c"
    break;

  case 83: /* target_expr: SELF  */
#line 296 "compose.y"
                   { (yyval.pAst) = ast_create(kAST_Self, pParseData); }
#line 2533 "compose_parser.c"
    break;

  case 84: /* target_expr: CREATOR  */
#line 297 "compose.y"
                   { (yyval.pAst) = ast_create(kAST_Creator, pParseData); }
#line 2539 "compose_parser.c"
    break;

  case 85: /* target_expr: RENDERER  */
#line 298 "compose.y"
                   { (yyval.pAst) = ast_create(kAST_Renderer, pParseData); }
#line 2545 "compose_parser.c"
    break;

  case 86: /* target_expr: INT_LITERAL  */
#line 299 "compose.y"
                   { (yyval.pAst) = ast_create_int_literal((yyvsp[0].numi), pParseData); }
#line 2551 "compose_parser.c"
    break;

  case 87: /* target_expr: PARENT  */
#line 300 "compose.y"
                   { (yyval.pAst) = ast_create(kAST_Parent, pParseData); }
#line 2557 "compose_parser.c"
    break;

  case 88: /* message_expr: IDENTIFIER  */
#line 304 "compose.y"
                   { (yyval.pAst) = ast_create_identifier((yyvsp[0].str), pParseData); }
#line 2563 "compose_parser.c"
    break;

  case 89: /* message_expr: SELF  */
#line 305 "compose.y"
                   { (yyval.pAst) = ast_create(kAST_Self, pParseData); }
#line 2569 "compose_parser.c"
    break;

  case 90: /* message_expr: INT_LITERAL  */
#line 306 "compose.y"
                   { (yyval.pAst) = ast_create_int_literal((yyvsp[0].numi), pParseData); }
#line 2575 "compose_parser.c"
    break;

  case 91: /* message_expr: '(' expr ')'  */
#line 307 "compose.y"
                       { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2581 "compose_parser.c"
    break;

  case 92: /* expr: '(' expr ')'  */
#line 311 "compose.y"
                     { (yyval.pAst) = (yyvsp[-1].pAst); }
#line 2587 "compose_parser.c"
    break;

  case 93: /* expr: type_ent IDENTIFIER  */
#line 313 "compose.y"
                                   { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[-1].pSymDataType), (yyvsp[0].str), NULL, NULL, pParseData)); }
#line 2593 "compose_parser.c"
    break;

  case 94: /* expr: type_ent IDENTIFIER '=' expr  */
#line 314 "compose.y"
                                   { (yyval.pAst) = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, (yyvsp[-3].pSymDataType), (yyvsp[-2].str), NULL, (yyvsp[0].pAst), pParseData)); }
#line 2599 "compose_parser.c"
    break;

  case 95: /* expr: cond_expr  */
#line 316 "compose.y"
                       { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2605 "compose_parser.c"
    break;

  case 96: /* expr: STRING_LITERAL  */
#line 318 "compose.y"
                       { (yyval.pAst) = ast_create_string_literal((yyvsp[0].str), pParseData); }
#line 2611 "compose_parser.c"
    break;

  case 97: /* expr: literal  */
#line 319 "compose.y"
                       { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2617 "compose_parser.c"
    break;

  case 98: /* expr: expr '+' expr  */
#line 321 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_Add,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2623 "compose_parser.c"
    break;

  case 99: /* expr: expr '-' expr  */
#line 322 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_Sub,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2629 "compose_parser.c"
    break;

  case 100: /* expr: expr '*' expr  */
#line 323 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_Mul,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2635 "compose_parser.c"
    break;

  case 101: /* expr: expr '/' expr  */
#line 324 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_Div,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2641 "compose_parser.c"
    break;

  case 102: /* expr: expr '%' expr  */
#line 325 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_Mod,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2647 "compose_parser.c"
    break;

  case 103: /* expr: expr LSHIFT expr  */
#line 326 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_LShift, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2653 "compose_parser.c"
    break;

  case 104: /* expr: expr RSHIFT expr  */
#line 327 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_RShift, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2659 "compose_parser.c"
    break;

  case 105: /* expr: expr OR expr  */
#line 328 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_Or,     (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2665 "compose_parser.c"
    break;

  case 106: /* expr: expr AND expr  */
#line 329 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_And,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2671 "compose_parser.c"
    break;

  case 107: /* expr: expr '|' expr  */
#line 330 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_BitOr,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2677 "compose_parser.c"
    break;

  case 108: /* expr: expr '^' expr  */
#line 331 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_BitXor, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2683 "compose_parser.c"
    break;

  case 109: /* expr: expr '&' expr  */
#line 332 "compose.y"
                       { (yyval.pAst) = ast_create_binary_op(kAST_BitAnd, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2689 "compose_parser.c"
    break;

  case 110: /* expr: dotted_id '=' expr  */
#line 334 "compose.y"
                                   { (yyval.pAst) = ast_create_assign_op(kAST_Assign,       (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2695 "compose_parser.c"
    break;

  case 111: /* expr: dotted_id ADD_ASSIGN expr  */
#line 335 "compose.y"
                                   { (yyval.pAst) = ast_create_assign_op(kAST_AddAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2701 "compose_parser.c"
    break;

  case 112: /* expr: dotted_id SUB_ASSIGN expr  */
#line 336 "compose.y"
                                   { (yyval.pAst) = ast_create_assign_op(kAST_SubAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2707 "compose_parser.c"
    break;

  case 113: /* expr: dotted_id MUL_ASSIGN expr  */
#line 337 "compose.y"
                                   { (yyval.pAst) = ast_create_assign_op(kAST_MulAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2713 "compose_parser.c"
    break;

  case 114: /* expr: dotted_id DIV_ASSIGN expr  */
#line 338 "compose.y"
                                   { (yyval.pAst) = ast_create_assign_op(kAST_DivAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2719 "compose_parser.c"
    break;

  case 115: /* expr: dotted_id MOD_ASSIGN expr  */
#line 339 "compose.y"
                                   { (yyval.pAst) = ast_create_assign_op(kAST_ModAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2725 "compose_parser.c"
    break;

  case 116: /* expr: dotted_id LSHIFT_ASSIGN expr  */
#line 340 "compose.y"
                                   { (yyval.pAst) = ast_create_assign_op(kAST_LShiftAssign, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2731 "compose_parser.c"
    break;

  case 117: /* expr: dotted_id RSHIFT_ASSIGN expr  */
#line 341 "compose.y"
                                   { (yyval.pAst) = ast_create_assign_op(kAST_RShiftAssign, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2737 "compose_parser.c"
    break;

  case 118: /* expr: dotted_id AND_ASSIGN expr  */
#line 342 "compose.y"
                                   { (yyval.pAst) = ast_create_assign_op(kAST_AndAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2743 "compose_parser.c"
    break;

  case 119: /* expr: dotted_id XOR_ASSIGN expr  */
#line 343 "compose.y"
                                   { (yyval.pAst) = ast_create_assign_op(kAST_XorAssign,    (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2749 "compose_parser.c"
    break;

  case 120: /* expr: dotted_id OR_ASSIGN expr  */
#line 344 "compose.y"
                                   { (yyval.pAst) = ast_create_assign_op(kAST_OrAssign,     (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2755 "compose_parser.c"
    break;

  case 121: /* expr: '!' expr  */
#line 346 "compose.y"
                            { (yyval.pAst) = ast_create_unary_op(kAST_Not,        (yyvsp[0].pAst), pParseData); }
#line 2761 "compose_parser.c"
    break;

  case 122: /* expr: '~' expr  */
#line 347 "compose.y"
                            { (yyval.pAst) = ast_create_unary_op(kAST_Complement, (yyvsp[0].pAst), pParseData); }
#line 2767 "compose_parser.c"
    break;

  case 123: /* expr: '-' expr  */
#line 348 "compose.y"
                            { (yyval.pAst) = ast_create_unary_op(kAST_Negate,     (yyvsp[0].pAst), pParseData); }
#line 2773 "compose_parser.c"
    break;

  case 124: /* expr: HASH  */
#line 350 "compose.y"
                             { (yyval.pAst) = ast_create_hash((yyvsp[0].str), pParseData); }
#line 2779 "compose_parser.c"
    break;

  case 125: /* expr: INC expr  */
#line 352 "compose.y"
                             { (yyval.pAst) = ast_create_unary_op(kAST_PreInc, (yyvsp[0].pAst), pParseData); }
#line 2785 "compose_parser.c"
    break;

  case 126: /* expr: DEC expr  */
#line 353 "compose.y"
                             { (yyval.pAst) = ast_create_unary_op(kAST_PreDec, (yyvsp[0].pAst), pParseData); }
#line 2791 "compose_parser.c"
    break;

  case 127: /* expr: expr INC  */
#line 354 "compose.y"
                             { (yyval.pAst) = ast_create_unary_op(kAST_PostInc, (yyvsp[-1].pAst), pParseData); }
#line 2797 "compose_parser.c"
    break;

  case 128: /* expr: expr DEC  */
#line 355 "compose.y"
                             { (yyval.pAst) = ast_create_unary_op(kAST_PostDec, (yyvsp[-1].pAst), pParseData); }
#line 2803 "compose_parser.c"
    break;

  case 129: /* expr: basic_type '{' fun_params '}'  */
#line 357 "compose.y"
                                         { (yyval.pAst) = ast_create_type_init((yyvsp[-3].dataType), (yyvsp[-1].pAst), pParseData); }
#line 2809 "compose_parser.c"
    break;

  case 130: /* expr: dotted_id '{' prop_init_list '}'  */
#line 358 "compose.y"
                                         { (yyval.pAst) = ast_create_entity_init((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2815 "compose_parser.c"
    break;

  case 131: /* expr: dotted_id '(' fun_params ')'  */
#line 359 "compose.y"
                                         { (yyval.pAst) = ast_create_function_call((yyvsp[-3].pAst), (yyvsp[-1].pAst), pParseData); }
#line 2821 "compose_parser.c"
    break;

  case 132: /* expr: '$' '.' IDENTIFIER  */
#line 361 "compose.y"
                                             { (yyval.pAst) = ast_create_system_const_ref((yyvsp[0].str), pParseData); }
#line 2827 "compose_parser.c"
    break;

  case 133: /* expr: '$' '.' IDENTIFIER '(' fun_params ')'  */
#line 362 "compose.y"
                                             { (yyval.pAst) = ast_create_system_api_call((yyvsp[-3].str), (yyvsp[-1].pAst), pParseData); }
#line 2833 "compose_parser.c"
    break;

  case 134: /* expr: TRANSFORM  */
#line 364 "compose.y"
                 { (yyval.pAst) = ast_create(kAST_Transform, pParseData); }
#line 2839 "compose_parser.c"
    break;

  case 135: /* expr: SELF  */
#line 365 "compose.y"
                 { (yyval.pAst) = ast_create(kAST_Self, pParseData); }
#line 2845 "compose_parser.c"
    break;

  case 136: /* expr: CREATOR  */
#line 366 "compose.y"
                 { (yyval.pAst) = ast_create(kAST_Creator, pParseData); }
#line 2851 "compose_parser.c"
    break;

  case 137: /* cond_expr: expr EQ expr  */
#line 370 "compose.y"
                     { (yyval.pAst) = ast_create_binary_op(kAST_Eq,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2857 "compose_parser.c"
    break;

  case 138: /* cond_expr: expr NEQ expr  */
#line 371 "compose.y"
                     { (yyval.pAst) = ast_create_binary_op(kAST_NEq, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2863 "compose_parser.c"
    break;

  case 139: /* cond_expr: expr LTE expr  */
#line 372 "compose.y"
                     { (yyval.pAst) = ast_create_binary_op(kAST_LTE, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2869 "compose_parser.c"
    break;

  case 140: /* cond_expr: expr GTE expr  */
#line 373 "compose.y"
                     { (yyval.pAst) = ast_create_binary_op(kAST_GTE, (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2875 "compose_parser.c"
    break;

  case 141: /* cond_expr: expr '<' expr  */
#line 374 "compose.y"
                     { (yyval.pAst) = ast_create_binary_op(kAST_LT,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2881 "compose_parser.c"
    break;

  case 142: /* cond_expr: expr '>' expr  */
#line 375 "compose.y"
                     { (yyval.pAst) = ast_create_binary_op(kAST_GT,  (yyvsp[-2].pAst), (yyvsp[0].pAst), pParseData); }
#line 2887 "compose_parser.c"
    break;

  case 143: /* cond_expr: dotted_id  */
#line 376 "compose.y"
                     { (yyval.pAst) = ast_create_symbol_ref((yyvsp[0].pAst), pParseData); }
#line 2893 "compose_parser.c"
    break;

  case 144: /* literal: INT_LITERAL  */
#line 380 "compose.y"
                    { (yyval.pAst) = ast_create_int_literal((yyvsp[0].numi), pParseData); }
#line 2899 "compose_parser.c"
    break;

  case 145: /* literal: FLOAT_LITERAL  */
#line 381 "compose.y"
                    { (yyval.pAst) = ast_create_float_literal((yyvsp[0].numf), pParseData); }
#line 2905 "compose_parser.c"
    break;

  case 146: /* literal: TRUE_  */
#line 382 "compose.y"
                    { (yyval.pAst) = ast_create_bool_literal(1, pParseData); }
#line 2911 "compose_parser.c"
    break;

  case 147: /* literal: FALSE_  */
#line 383 "compose.y"
                    { (yyval.pAst) = ast_create_bool_literal(0, pParseData); }
#line 2917 "compose_parser.c"
    break;

  case 148: /* expr_or_empty: %empty  */
#line 387 "compose.y"
                   { (yyval.pAst) = NULL; }
#line 2923 "compose_parser.c"
    break;

  case 149: /* expr_or_empty: expr  */
#line 388 "compose.y"
                   { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2929 "compose_parser.c"
    break;

  case 150: /* cond_expr_or_empty: %empty  */
#line 392 "compose.y"
                   { (yyval.pAst) = NULL; }
#line 2935 "compose_parser.c"
    break;

  case 151: /* cond_expr_or_empty: cond_expr  */
#line 393 "compose.y"
                   { (yyval.pAst) = (yyvsp[0].pAst); }
#line 2941 "compose_parser.c"
    break;

  case 152: /* fun_params: %empty  */
#line 397 "compose.y"
                           { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, NULL, pParseData); }
#line 2947 "compose_parser.c"
    break;

  case 153: /* fun_params: expr  */
#line 398 "compose.y"
                           { (yyval.pAst) = ast_append(kAST_FunctionParams, NULL, (yyvsp[0].pAst),   pParseData); }
#line 2953 "compose_parser.c"
    break;

  case 154: /* fun_params: fun_params ',' expr  */
#line 399 "compose.y"
                           { (yyval.pAst) = ast_append(kAST_FunctionParams, (yyvsp[-2].pAst),   (yyvsp[0].pAst),   pParseData); }
#line 2959 "compose_parser.c"
    break;

  case 155: /* type: basic_type  */
#line 403 "compose.y"
                         { (yyval.pSymDataType) = parsedata_find_basic_type(pParseData, (yyvsp[0].dataType), 0, 0); }
#line 2965 "compose_parser.c"
    break;

  case 156: /* type: dotted_id  */
#line 404 "compose.y"
                         { (yyval.pSymDataType) = parsedata_find_type_from_dotted_id(pParseData, (yyvsp[0].pAst), 0, 0); }
#line 2971 "compose_parser.c"
    break;

  case 157: /* const_type: CONST_ constable_type  */
#line 408 "compose.y"
                            { (yyval.pSymDataType) = parsedata_find_basic_type(pParseData, (yyvsp[0].dataType), 1, 0); }
#line 2977 "compose_parser.c"
    break;

  case 175: /* type_ent: type  */
#line 439 "compose.y"
                     { (yyval.pSymDataType) = (yyvsp[0].pSymDataType); }
#line 2983 "compose_parser.c"
    break;

  case 176: /* type_ent: ENTITY  */
#line 440 "compose.y"
                     { (yyval.pSymDataType) = parsedata_find_type(pParseData, "entity", 0, 0); }
#line 2989 "compose_parser.c"
    break;

  case 177: /* type_ent_handle_asset: type_ent  */
#line 448 "compose.y"
                 { (yyval.pSymDataType) = (yyvsp[0].pSymDataType); }
#line 2995 "compose_parser.c"
    break;

  case 178: /* type_ent_handle_asset: HANDLE_  */
#line 449 "compose.y"
                 { (yyval.pSymDataType) = parsedata_find_type(pParseData, "handle", 0, 0); }
#line 3001 "compose_parser.c"
    break;

  case 179: /* type_ent_handle_asset: ASSET  */
#line 450 "compose.y"
                 { (yyval.pSymDataType) = parsedata_find_type(pParseData, "asset_handle", 0, 0); }
#line 3007 "compose_parser.c"
    break;


#line 3011 "compose_parser.c"

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
  *++yylsp = yyloc;

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
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, pParseData, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, pParseData);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
  yyerror (&yylloc, pParseData, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, pParseData);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, pParseData);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 453 "compose.y"



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

