/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
    PARENT = 320,
    OR = 321,
    AND = 322,
    EQ = 323,
    NEQ = 324,
    LTE = 325,
    GTE = 326,
    LSHIFT = 327,
    RSHIFT = 328,
    INC = 329,
    DEC = 330,
    UMINUS = 331,
    POSTINC = 332,
    POSTDEC = 333
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 49 "compose.y" /* yacc.c:1909  */

    int                 numi;
    float               numf;
    const char*         str;
    DataType            dataType;
    Ast*                pAst;
    SymTab*             pSymTab;
    const SymDataType*  pSymDataType;

#line 143 "compose_parser.h" /* yacc.c:1909  */
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
