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
     INT_LITERAL = 260,
     TRUE = 261,
     FALSE = 262,
     FLOAT_LITERAL = 263,
     VOID = 264,
     BOOL = 265,
     CHAR = 266,
     BYTE = 267,
     SHORT = 268,
     USHORT = 269,
     INT = 270,
     UINT = 271,
     LONG = 272,
     ULONG = 273,
     HALF = 274,
     FLOAT = 275,
     DOUBLE = 276,
     COLOR = 277,
     VEC2 = 278,
     VEC3 = 279,
     VEC4 = 280,
     MAT3 = 281,
     MAT34 = 282,
     MAT4 = 283,
     HANDLE_ = 284,
     ENTITY = 285,
     IF = 286,
     SWITCH = 287,
     CASE = 288,
     DEFAULT = 289,
     FOR = 290,
     WHILE = 291,
     DO = 292,
     BREAK = 293,
     RETURN = 294,
     COMPONENT = 295,
     COMPONENTS = 296,
     USING = 297,
     AS = 298,
     CONST = 299,
     THIS = 300,
     NONE = 301,
     THEN = 302,
     ELSE = 303,
     TRANSFORM = 304,
     OR_ASSIGN = 305,
     XOR_ASSIGN = 306,
     AND_ASSIGN = 307,
     RSHIFT_ASSIGN = 308,
     LSHIFT_ASSIGN = 309,
     MOD_ASSIGN = 310,
     DIV_ASSIGN = 311,
     MUL_ASSIGN = 312,
     SUB_ASSIGN = 313,
     ADD_ASSIGN = 314,
     OR = 315,
     AND = 316,
     NEQ = 317,
     EQ = 318,
     GTE = 319,
     LTE = 320,
     RSHIFT = 321,
     LSHIFT = 322,
     DEC = 323,
     INC = 324,
     UMINUS = 325,
     POSTDEC = 326,
     POSTINC = 327,
     SCOPE = 328
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
#line 140 "compose_parser.h"
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
