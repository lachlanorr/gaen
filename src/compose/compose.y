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
#include "compose/compiler.h"

#include "compose/comp_mem.h"
#define YYMALLOC COMP_ALLOC
#define YYFREE COMP_FREE

//#define YYDEBUG 1
//#include <stdio.h>
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
    float       numf;
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

#define YYPRINT(file, type, value)   yyprint (file, type, value)
void yyprint(FILE * file, int type, YYSTYPE value);
%}

%token <str> IDENTIFIER
%token <numi> INT_LITERAL
%token <numf> FLOAT_LITERAL

%token <dataType> INT UINT FLOAT BOOL VEC3 VOID

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
%type <pAst>     def stmt exp
%type <pAstList> block stmt_list exp_list
%type <pSymTab>  param_list

%%

def_list
    : def
    | def_list def
    ;

def
    : '#' IDENTIFIER '(' param_list ')' block  { $$ = ast_create_message_def($2, $6, pParseData); }
    | type IDENTIFIER '(' param_list ')' block { $$ = ast_create_function_def($2, $1, $6, pParseData); }
    ;

param_list
    : /* empty */                       { $$ = parsedata_add_symbol(pParseData, NULL, NULL); }
    | type IDENTIFIER                   { $$ = parsedata_add_symbol(pParseData, NULL, symrec_create(kSYMT_Param, $1, $2, NULL)); }
    | param_list ',' type IDENTIFIER    { $$ = parsedata_add_symbol(pParseData, $1, symrec_create(kSYMT_Param, $3, $4, NULL)); }
    ;

block
    : '{' '}'              { $$ = astlist_append(NULL, NULL); }
    | '{' stmt_list '}'    { $$ = $2; }
    ;

stmt_list
    : stmt            { $$ = astlist_append(NULL, $1); }
    | stmt_list stmt  { $$ = astlist_append($1, $2); }
    ;
 
stmt
    : type IDENTIFIER ';'         { $$ = NULL; parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, $1, $2, NULL)); }
    | type IDENTIFIER '=' exp ';' { $$ = NULL; parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, $1, $2, $4)); }
    | exp ';'                     { $$ = $1; }
    ;

exp_list
    : /* empty */       { $$ = astlist_append(NULL, NULL); }
    | exp               { $$ = astlist_append(NULL, $1); }
    | exp_list ',' exp  { $$ = astlist_append($1, $3); }
    ;

exp
    : '(' exp ')'   { $$ = $2; }
    | exp '+' exp   { $$ = ast_create_binary_op(kAST_Add,      $1, $3, pParseData); }
    | exp '-' exp   { $$ = ast_create_binary_op(kAST_Subtract, $1, $3, pParseData); }
    | exp '*' exp   { $$ = ast_create_binary_op(kAST_Multiply, $1, $3, pParseData); }
    | exp '/' exp   { $$ = ast_create_binary_op(kAST_Divide,   $1, $3, pParseData); }
    | exp '%' exp   { $$ = ast_create_binary_op(kAST_Modulus,  $1, $3, pParseData); }

    | '!' exp              { $$ = ast_create_unary_op(kAST_Not,        $1, pParseData); }
    | '~' exp              { $$ = ast_create_unary_op(kAST_Complement, $1, pParseData); }
    | '-' exp %prec UMINUS { $$ = ast_create_unary_op(kAST_Complement, $1, pParseData); }

    | INT_LITERAL   { $$ = ast_create_int_literal($1, pParseData); }
    | FLOAT_LITERAL { $$ = ast_create_float_literal($1, pParseData); }

    | IDENTIFIER '(' exp_list ')'  { $$ = ast_create_function_call($1, $3, pParseData); }
    | IDENTIFIER                   { $$ = ast_create_symbol_ref($1, pParseData); }
    ;

type
    : INT
    | UINT
    | FLOAT
    | BOOL
    | VEC3
    | VOID
    ;

%%


#if 0
static void yyprint (FILE * file, int type, YYSTYPE value)
{
    if (type < 128)
    {
        fprintf(file, "'%c'", type);
    }
    else
    {
        switch(type)
        {
        case IDENTIFIER:
            fprintf(file, "\"%s\"", value.str);
            break;
        case INT_LITERAL:
            fprintf(file, "%d", value.numi);
            break;
        case FLOAT_LITERAL:
            fprintf(file, "%f", value.numf);
            break;
        default:
            fprintf(file, "** yyprint unknown: %d", type);
            break;
        }
    }
}
#endif

