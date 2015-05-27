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
/* Line 2053 of yacc.c  */
#line 49 "compose.y"

    int         numi;
    float       numf;
    const char* str;
    DataType    dataType;
    Ast*        pAst;
    SymTab*     pSymTab;
    SymRec*     pSymRec;


/* Line 2053 of yacc.c  */
#line 143 "compose_parser.h"
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
