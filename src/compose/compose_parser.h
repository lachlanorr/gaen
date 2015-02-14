/* A Bison parser, made by GNU Bison 2.7.1.  */

/* Bison interface for Yacc-like parsers in C
   
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
     CONST_ = 301,
     THIS__ = 302,
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
/* Line 2053 of yacc.c  */
#line 49 "compose.y"

    int         numi;
    float       numf;
    const char* str;
    DataType    dataType;
    Ast*        pAst;
    SymTab*     pSymTab;


/* Line 2053 of yacc.c  */
#line 141 "compose_parser.h"
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
