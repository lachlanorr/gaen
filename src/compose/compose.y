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

#define YYDEBUG 1
#include <stdio.h>
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

%token <dataType> INT UINT FLOAT BOOL CHAR VEC3 VEC4 MAT3 MAT34 MAT4 VOID

%token IF SWITCH CASE DEFAULT FOR WHILE DO BREAK RETURN
%right ELSE THEN

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
%left POSTINC POSTDEC
%right '('
%right '['

%left <pAst> SCOPE

%type <dataType> type
%type <pAst>     def stmt expr block stmt_list expr_list
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
    : '{' '}'              { $$ = ast_append(NULL, NULL, pParseData); }
    | '{' stmt_list '}'    { $$ = $2; }
    ;

stmt_list
    : stmt            { $$ = ast_append(NULL, $1, pParseData); }
    | stmt_list stmt  { $$ = ast_append($1, $2, pParseData); }
    ;
 
stmt
    : type IDENTIFIER ';'          { $$ = NULL; parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, $1, $2, NULL)); }
    | type IDENTIFIER '=' expr ';' { $$ = NULL; parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, $1, $2, $4)); }

    | IF '(' expr ')' block_or_stmt %prec THEN         { $$ = NULL; }
    | IF '(' expr ')' block_or_stmt ELSE block_or_stmt { $$ = NULL; }

    | expr ';'                     { $$ = $1; }
    ;

block_or_stmt
    : block
    | stmt
    ;

expr_list
    : /* empty */         { $$ = ast_append(NULL, NULL, pParseData); }
    | expr                { $$ = ast_append(NULL, $1,   pParseData); }
    | expr_list ',' expr  { $$ = ast_append($1,   $3,   pParseData); }
    ;

expr
    : '(' expr ')'   { $$ = $2; }
    
    | expr '+' expr    { $$ = ast_create_binary_op(kAST_Add,    $1, $3, pParseData); }
    | expr '-' expr    { $$ = ast_create_binary_op(kAST_Sub,    $1, $3, pParseData); }
    | expr '*' expr    { $$ = ast_create_binary_op(kAST_Mul,    $1, $3, pParseData); }
    | expr '/' expr    { $$ = ast_create_binary_op(kAST_Div,    $1, $3, pParseData); }
    | expr '%' expr    { $$ = ast_create_binary_op(kAST_Mod,    $1, $3, pParseData); }
    | expr LSHIFT expr { $$ = ast_create_binary_op(kAST_LShift, $1, $3, pParseData); }
    | expr RSHIFT expr { $$ = ast_create_binary_op(kAST_RShift, $1, $3, pParseData); }
    | expr AND expr    { $$ = ast_create_binary_op(kAST_And,    $1, $3, pParseData); }
    | expr OR expr     { $$ = ast_create_binary_op(kAST_Or,     $1, $3, pParseData); }
    | expr '^' expr    { $$ = ast_create_binary_op(kAST_XOr,    $1, $3, pParseData); }

    | expr EQ expr  { $$ = ast_create_binary_op(kAST_Eq,  $1, $3, pParseData); }
    | expr NEQ expr { $$ = ast_create_binary_op(kAST_NEq, $1, $3, pParseData); }
    | expr LT expr  { $$ = ast_create_binary_op(kAST_LT,  $1, $3, pParseData); }
    | expr LTE expr { $$ = ast_create_binary_op(kAST_LTE, $1, $3, pParseData); }
    | expr GT expr  { $$ = ast_create_binary_op(kAST_GT,  $1, $3, pParseData); }
    | expr GTE expr { $$ = ast_create_binary_op(kAST_GTE, $1, $3, pParseData); }

    | IDENTIFIER '=' expr           { $$ = ast_create_assign_op(kAST_Assign,       $1, $3, pParseData); }
    | IDENTIFIER ADD_ASSIGN expr    { $$ = ast_create_assign_op(kAST_AddAssign,    $1, $3, pParseData); }
    | IDENTIFIER SUB_ASSIGN expr    { $$ = ast_create_assign_op(kAST_SubAssign,    $1, $3, pParseData); }
    | IDENTIFIER MUL_ASSIGN expr    { $$ = ast_create_assign_op(kAST_MulAssign,    $1, $3, pParseData); }
    | IDENTIFIER DIV_ASSIGN expr    { $$ = ast_create_assign_op(kAST_DivAssign,    $1, $3, pParseData); }
    | IDENTIFIER MOD_ASSIGN expr    { $$ = ast_create_assign_op(kAST_ModAssign,    $1, $3, pParseData); }
    | IDENTIFIER LSHIFT_ASSIGN expr { $$ = ast_create_assign_op(kAST_LShiftAssign, $1, $3, pParseData); }
    | IDENTIFIER RSHIFT_ASSIGN expr { $$ = ast_create_assign_op(kAST_RShiftAssign, $1, $3, pParseData); }
    | IDENTIFIER AND_ASSIGN expr    { $$ = ast_create_assign_op(kAST_AndAssign,    $1, $3, pParseData); }
    | IDENTIFIER XOR_ASSIGN expr    { $$ = ast_create_assign_op(kAST_XorAssign,    $1, $3, pParseData); }
    | IDENTIFIER OR_ASSIGN expr     { $$ = ast_create_assign_op(kAST_OrAssign,     $1, $3, pParseData); }

    | '!' expr              { $$ = ast_create_unary_op(kAST_Not,        $2, pParseData); }
    | '~' expr              { $$ = ast_create_unary_op(kAST_Complement, $2, pParseData); }
    | '-' expr %prec UMINUS { $$ = ast_create_unary_op(kAST_Complement, $2, pParseData); }

    | INC expr               { $$ = ast_create_unary_op(kAST_PreInc, $2, pParseData); }
    | DEC expr               { $$ = ast_create_unary_op(kAST_PreDec, $2, pParseData); }
    | expr INC %prec POSTINC { $$ = ast_create_unary_op(kAST_PostInc, $1, pParseData); }
    | expr DEC %prec POSTDEC { $$ = ast_create_unary_op(kAST_PostDec, $1, pParseData); }

    | INT_LITERAL   { $$ = ast_create_int_literal($1, pParseData); }
    | FLOAT_LITERAL { $$ = ast_create_float_literal($1, pParseData); }

    | IDENTIFIER '(' expr_list ')'  { $$ = ast_create_function_call($1, $3, pParseData); }
    | IDENTIFIER                    { $$ = ast_create_symbol_ref($1, pParseData); }
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

