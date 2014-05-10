/*------------------------------------------------------------------------------
compose.y - Bison definition for compose parser

Gaen Concurrency Engine - http://gaen.org
Copyright (c) 2014 Lachlan Orr

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
  claim that you wrote the original software. If you use this software
  in a product, an acknowledgment in the product documentation would be
  appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be
  misrepresented as being the original software.

  3. This notice may not be removed or altered from any source
  distribution.
------------------------------------------------------------------------------*/

%{
#include <stdio.h> // LORRTEMP

#include "compose/compiler.h"

#include "compose/comp_mem.h"
#define YYMALLOC COMP_ALLOC
#define YYFREE COMP_FREE
%}

%define api.pure full
%error-verbose

%lex-param { ParseData * pParseData }
%parse-param { ParseData * pParseData }

%locations
%output  "compose_parser.c"
%defines "compose_parser.h"

%union
{
    int         numi;
    double      numf;
    const char* str;
    DataType    dataType;
    Ast*        pAst;
    AstList*    pAstList;
    SymRec*     pSymRec;
    SymTab*     pSymTab;
}

%{
#define YY_NO_UNISTD_H
#include "compose/compose_scanner.h"
#define YYLEX_PARAM parsedata_scanner(pParseData)
%}

%token <str> IDENTIFIER
%token <numi> INT_LITERAL
%token <numf> FLOAT_LITERAL

%token <dataType> INT UINT FLOAT BOOL VEC3

%right <pAst> '=' ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN

%left <pAst> OR
%left <pAst> AND
%left <pAst> '|'
%left <pAst> '^'
%left <pAst> '&'

%nonassoc <pAst> EQ NEQ
%nonassoc <pAst> LT GT LTE GTE

%left <pAst> LSHIFT RSHIFT

%left <pAst> '+' '-'
%left <pAst> '*' '/' '%'

%right <pAst> INC DEC '!' '~'
%right <pAst> UMINUS

%left <pAst> '.'

%left <pAst> SCOPE

%type <dataType> type
%type <pAst>     message_def stmt exp
%type <pAstList> block stmt_list
%type <pSymTab>  param_list

%%

def_list
    : def
    | def_list def
    ;

def
    : message_def   { printf("def:message_def\n"); }
    | function_def  { }
    ;

message_def
    : '#' IDENTIFIER '(' param_list ')' block
       {
           $$ = ast_create_message_def($6, pParseData);
           printf("message_def:'#' IDENTIFIER '(' param_list ')' block\n");
       }
    ;

function_def
    : type IDENTIFIER '(' param_list ')' block
      {
          /*$$ = ast_create_function_def($2*/
      }
    ;

param_list
    : /* empty */                       { $$ = parsedata_add_symbol(pParseData, NULL, NULL);                             printf("param_list:/* empty */\n"); }
    | type IDENTIFIER                   { $$ = parsedata_add_symbol(pParseData, NULL, symrec_create(kSCOPE_Param, $1, $2, NULL));  printf("param_list:type IDENTIFIER\n"); }
    | param_list ',' type IDENTIFIER    { $$ = parsedata_add_symbol(pParseData, $1, symrec_create(kSCOPE_Param, $3, $4, NULL));    printf("param_list:type IDENTIFIER ',' param_list\n"); }
    ;

block
    : '{' '}'              { $$ = astlist_append(NULL, NULL); printf("block:'{' '}'\n"); }
    | '{' stmt_list '}'    { $$ = $2; printf("block:'{' stmt_list '}'\n"); }
    ;

stmt_list
    : stmt            { $$ = astlist_append(NULL, $1); printf("stmt_list:stmt\n"); }
    | stmt_list stmt  { $$ = astlist_append($1, $2); printf("stmt_list:stmt stmt_list\n"); }
    ;
 
stmt
    : type IDENTIFIER ';'  { $$ = NULL; parsedata_add_local_symbol(pParseData, symrec_create(kSCOPE_Local, $1, $2, NULL)); printf("stmt:type IDENTIFIER\n"); printf("stmt:type IDENTIFIER ';'\n"); }
    | exp ';'              { $$ = $1; printf("stmt:exp\n"); printf("stmt:exp ';'\n"); }
    ;

exp
    : exp '+' exp   { $$ = ast_create_binary_op(kAST_Add,      $1, $3, pParseData); }
    | exp '-' exp   { $$ = ast_create_binary_op(kAST_Subtract, $1, $3, pParseData); }
    | exp '*' exp   { $$ = ast_create_binary_op(kAST_Multiply, $1, $3, pParseData); }
    | exp '/' exp   { $$ = ast_create_binary_op(kAST_Divide,   $1, $3, pParseData); }
    | exp '%' exp   { $$ = ast_create_binary_op(kAST_Modulus,  $1, $3, pParseData); }

    | '!' exp              { $$ = ast_create_unary_op(kAST_Not,        $1, pParseData); }
    | '~' exp              { $$ = ast_create_unary_op(kAST_Complement, $1, pParseData); }
    | '-' exp %prec UMINUS { $$ = ast_create_unary_op(kAST_Complement, $1, pParseData); }

    | INT_LITERAL   { printf("INT_LITERAL\n"); }
    | FLOAT_LITERAL { printf("FLOAT_LITERAL\n"); }
    | IDENTIFIER    { printf("IDENTIFIER\n"); }
    ;

type
    : INT
    | UINT
    | FLOAT
    | BOOL
    | VEC3
    ;

%%


