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
     INT_LITERAL = 259,
     TRUE = 260,
     FALSE = 261,
     FLOAT_LITERAL = 262,
     INT = 263,
     UINT = 264,
     FLOAT = 265,
     BOOL = 266,
     CHAR = 267,
     VEC3 = 268,
     VEC4 = 269,
     MAT3 = 270,
     MAT34 = 271,
     MAT4 = 272,
     VOID = 273,
     IF = 274,
     SWITCH = 275,
     CASE = 276,
     DEFAULT = 277,
     FOR = 278,
     WHILE = 279,
     DO = 280,
     BREAK = 281,
     RETURN = 282,
     ENTITY = 283,
     COMPONENT = 284,
     COMPONENTS = 285,
     THEN = 286,
     ELSE = 287,
     OR_ASSIGN = 288,
     XOR_ASSIGN = 289,
     AND_ASSIGN = 290,
     RSHIFT_ASSIGN = 291,
     LSHIFT_ASSIGN = 292,
     MOD_ASSIGN = 293,
     DIV_ASSIGN = 294,
     MUL_ASSIGN = 295,
     SUB_ASSIGN = 296,
     ADD_ASSIGN = 297,
     OR = 298,
     AND = 299,
     NEQ = 300,
     EQ = 301,
     GTE = 302,
     LTE = 303,
     RSHIFT = 304,
     LSHIFT = 305,
     DEC = 306,
     INC = 307,
     UMINUS = 308,
     POSTDEC = 309,
     POSTINC = 310,
     SCOPE = 311
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
#line 123 "compose_parser.h"
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
