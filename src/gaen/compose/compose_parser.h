/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_COMPOSE_PARSER_H_INCLUDED
# define YY_YY_COMPOSE_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    HASH = 259,                    /* HASH  */
    STRING_LITERAL = 260,          /* STRING_LITERAL  */
    INT_LITERAL = 261,             /* INT_LITERAL  */
    TRUE_ = 262,                   /* TRUE_  */
    FALSE_ = 263,                  /* FALSE_  */
    FLOAT_LITERAL = 264,           /* FLOAT_LITERAL  */
    VOID_ = 265,                   /* VOID_  */
    BOOL_ = 266,                   /* BOOL_  */
    INT_ = 267,                    /* INT_  */
    FLOAT_ = 268,                  /* FLOAT_  */
    COLOR = 269,                   /* COLOR  */
    VEC2 = 270,                    /* VEC2  */
    VEC3 = 271,                    /* VEC3  */
    VEC4 = 272,                    /* VEC4  */
    IVEC2 = 273,                   /* IVEC2  */
    IVEC3 = 274,                   /* IVEC3  */
    IVEC4 = 275,                   /* IVEC4  */
    QUAT = 276,                    /* QUAT  */
    MAT3 = 277,                    /* MAT3  */
    MAT43 = 278,                   /* MAT43  */
    MAT4 = 279,                    /* MAT4  */
    HANDLE_ = 280,                 /* HANDLE_  */
    ASSET = 281,                   /* ASSET  */
    ENTITY = 282,                  /* ENTITY  */
    STRING = 283,                  /* STRING  */
    IF = 284,                      /* IF  */
    SWITCH = 285,                  /* SWITCH  */
    CASE = 286,                    /* CASE  */
    DEFAULT = 287,                 /* DEFAULT  */
    FOR = 288,                     /* FOR  */
    WHILE = 289,                   /* WHILE  */
    DO = 290,                      /* DO  */
    BREAK = 291,                   /* BREAK  */
    RETURN = 292,                  /* RETURN  */
    COMPONENT = 293,               /* COMPONENT  */
    COMPONENTS = 294,              /* COMPONENTS  */
    UPDATE = 295,                  /* UPDATE  */
    INPUT_ = 296,                  /* INPUT_  */
    ANY = 297,                     /* ANY  */
    NONE = 298,                    /* NONE  */
    USING = 299,                   /* USING  */
    AS = 300,                      /* AS  */
    CONST_ = 301,                  /* CONST_  */
    SELF = 302,                    /* SELF  */
    CREATOR = 303,                 /* CREATOR  */
    PRE = 304,                     /* PRE  */
    POST = 305,                    /* POST  */
    VALUE = 306,                   /* VALUE  */
    RENDERER = 307,                /* RENDERER  */
    ELSE = 308,                    /* ELSE  */
    THEN = 309,                    /* THEN  */
    ADD_ASSIGN = 310,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 311,              /* SUB_ASSIGN  */
    MUL_ASSIGN = 312,              /* MUL_ASSIGN  */
    DIV_ASSIGN = 313,              /* DIV_ASSIGN  */
    MOD_ASSIGN = 314,              /* MOD_ASSIGN  */
    LSHIFT_ASSIGN = 315,           /* LSHIFT_ASSIGN  */
    RSHIFT_ASSIGN = 316,           /* RSHIFT_ASSIGN  */
    AND_ASSIGN = 317,              /* AND_ASSIGN  */
    XOR_ASSIGN = 318,              /* XOR_ASSIGN  */
    OR_ASSIGN = 319,               /* OR_ASSIGN  */
    TRANSFORM = 320,               /* TRANSFORM  */
    READY = 321,                   /* READY  */
    PARENT = 322,                  /* PARENT  */
    VISIBLE = 323,                 /* VISIBLE  */
    OR = 324,                      /* OR  */
    AND = 325,                     /* AND  */
    EQ = 326,                      /* EQ  */
    NEQ = 327,                     /* NEQ  */
    LTE = 328,                     /* LTE  */
    GTE = 329,                     /* GTE  */
    LSHIFT = 330,                  /* LSHIFT  */
    RSHIFT = 331,                  /* RSHIFT  */
    INC = 332,                     /* INC  */
    DEC = 333,                     /* DEC  */
    UMINUS = 334,                  /* UMINUS  */
    POSTINC = 335,                 /* POSTINC  */
    POSTDEC = 336                  /* POSTDEC  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 49 "compose.y"

    int                 numi;
    float               numf;
    const char*         str;
    DataType            dataType;
    Ast*                pAst;
    SymTab*             pSymTab;
    const SymDataType*  pSymDataType;

#line 155 "compose_parser.h"

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
