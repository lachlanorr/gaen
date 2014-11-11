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
     TRUE = 262,
     FALSE = 263,
     FLOAT_LITERAL = 264,
     VOID = 265,
     BOOL = 266,
     CHAR = 267,
     BYTE = 268,
     SHORT = 269,
     USHORT = 270,
     INT = 271,
     UINT = 272,
     LONG = 273,
     ULONG = 274,
     HALF = 275,
     FLOAT = 276,
     DOUBLE = 277,
     COLOR = 278,
     VEC2 = 279,
     VEC3 = 280,
     VEC4 = 281,
     MAT3 = 282,
     MAT34 = 283,
     MAT4 = 284,
     HANDLE_ = 285,
     ENTITY = 286,
     IF = 287,
     SWITCH = 288,
     CASE = 289,
     DEFAULT = 290,
     FOR = 291,
     WHILE = 292,
     DO = 293,
     BREAK = 294,
     RETURN = 295,
     COMPONENT = 296,
     COMPONENTS = 297,
     USING = 298,
     AS = 299,
     CONST = 300,
     THIS = 301,
     NONE = 302,
     THEN = 303,
     ELSE = 304,
     TRANSFORM = 305,
     OR_ASSIGN = 306,
     XOR_ASSIGN = 307,
     AND_ASSIGN = 308,
     RSHIFT_ASSIGN = 309,
     LSHIFT_ASSIGN = 310,
     MOD_ASSIGN = 311,
     DIV_ASSIGN = 312,
     MUL_ASSIGN = 313,
     SUB_ASSIGN = 314,
     ADD_ASSIGN = 315,
     OR = 316,
     AND = 317,
     NEQ = 318,
     EQ = 319,
     GTE = 320,
     LTE = 321,
     RSHIFT = 322,
     LSHIFT = 323,
     DEC = 324,
     INC = 325,
     UMINUS = 326,
     POSTDEC = 327,
     POSTINC = 328,
     SCOPE = 329
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
