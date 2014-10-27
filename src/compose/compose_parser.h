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
     VOID = 263,
     BOOL = 264,
     CHAR = 265,
     BYTE = 266,
     SHORT = 267,
     USHORT = 268,
     INT = 269,
     UINT = 270,
     LONG = 271,
     ULONG = 272,
     HALF = 273,
     FLOAT = 274,
     DOUBLE = 275,
     COLOR = 276,
     VEC2 = 277,
     VEC3 = 278,
     VEC4 = 279,
     MAT3 = 280,
     MAT34 = 281,
     MAT4 = 282,
     HANDLE_ = 283,
     ENTITY = 284,
     IF = 285,
     SWITCH = 286,
     CASE = 287,
     DEFAULT = 288,
     FOR = 289,
     WHILE = 290,
     DO = 291,
     BREAK = 292,
     RETURN = 293,
     COMPONENT = 294,
     COMPONENTS = 295,
     IMPORT = 296,
     CONST = 297,
     THIS = 298,
     NONE = 299,
     THEN = 300,
     ELSE = 301,
     OR_ASSIGN = 302,
     XOR_ASSIGN = 303,
     AND_ASSIGN = 304,
     RSHIFT_ASSIGN = 305,
     LSHIFT_ASSIGN = 306,
     MOD_ASSIGN = 307,
     DIV_ASSIGN = 308,
     MUL_ASSIGN = 309,
     SUB_ASSIGN = 310,
     ADD_ASSIGN = 311,
     OR = 312,
     AND = 313,
     NEQ = 314,
     EQ = 315,
     GTE = 316,
     LTE = 317,
     RSHIFT = 318,
     LSHIFT = 319,
     DEC = 320,
     INC = 321,
     UMINUS = 322,
     POSTDEC = 323,
     POSTINC = 324,
     SCOPE = 325
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
#line 137 "compose_parser.h"
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
