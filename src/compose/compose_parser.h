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
     CHAR = 263,
     BYTE = 264,
     SHORT = 265,
     USHORT = 266,
     INT = 267,
     UINT = 268,
     LONG = 269,
     ULONG = 270,
     HALF = 271,
     FLOAT = 272,
     DOUBLE = 273,
     BOOL = 274,
     VEC2 = 275,
     VEC3 = 276,
     VEC4 = 277,
     MAT3 = 278,
     MAT34 = 279,
     MAT4 = 280,
     VOID = 281,
     HANDLE = 282,
     IF = 283,
     SWITCH = 284,
     CASE = 285,
     DEFAULT = 286,
     FOR = 287,
     WHILE = 288,
     DO = 289,
     BREAK = 290,
     RETURN = 291,
     ENTITY = 292,
     COMPONENT = 293,
     COMPONENTS = 294,
     IMPORT = 295,
     THEN = 296,
     ELSE = 297,
     OR_ASSIGN = 298,
     XOR_ASSIGN = 299,
     AND_ASSIGN = 300,
     RSHIFT_ASSIGN = 301,
     LSHIFT_ASSIGN = 302,
     MOD_ASSIGN = 303,
     DIV_ASSIGN = 304,
     MUL_ASSIGN = 305,
     SUB_ASSIGN = 306,
     ADD_ASSIGN = 307,
     OR = 308,
     AND = 309,
     NEQ = 310,
     EQ = 311,
     GTE = 312,
     LTE = 313,
     RSHIFT = 314,
     LSHIFT = 315,
     DEC = 316,
     INC = 317,
     UMINUS = 318,
     POSTDEC = 319,
     POSTINC = 320,
     SCOPE = 321
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
#line 133 "compose_parser.h"
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
