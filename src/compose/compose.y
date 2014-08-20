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
    SymTab*     pSymTab;
}

%{
#define YY_NO_UNISTD_H
#include "compose/compose_scanner.h"
#define YYLEX_PARAM parsedata_scanner(pParseData)

#define YYPRINT(file, type, value)   yyprint (file, type, value)
static void yyprint(FILE * file, int type, YYSTYPE value);
%}

%token <str> IDENTIFIER
%token <numi> INT_LITERAL TRUE FALSE
%token <numf> FLOAT_LITERAL

%token <dataType> INT UINT FLOAT BOOL CHAR VEC3 VEC4 MAT3 MAT34 MAT4 VOID

%token IF SWITCH CASE DEFAULT FOR WHILE DO BREAK RETURN ENTITY COMPONENT
%right ELSE THEN

%right <pAst> '=' ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN

%left <pAst> OR
%left <pAst> AND
%left <pAst> '|'
%left <pAst> '^'
%left <pAst> '&'

%nonassoc <pAst> EQ NEQ
%nonassoc <pAst> '<' '>' LTE GTE

%left <pAst> LSHIFT RSHIFT

%left <pAst> '+' '-'
%left <pAst> '*' '/' '%'

%right <pAst> INC DEC '!' '~'
%right <pAst> UMINUS

%left <pAst> '.'
%left POSTINC POSTDEC
%right '(' '[' '{'
%left  ')' ']' '}'

%left <pAst> SCOPE

%type <dataType> type

%type <pAst> def stmt do_stmt block stmt_list fun_params expr cond_expr expr_or_empty cond_expr_or_empty literal
%type <pAst> message_block message_list message_prop target_expr component_expr

%type <pSymTab> param_list

%%

def_list
    : def
    | def_list def
    ;

def
    : ENTITY IDENTIFIER message_block          { $$ = ast_create_entity_def($2, $3, pParseData); }
    | COMPONENT IDENTIFIER message_block       { $$ = ast_create_component_def($2, $3, pParseData); }
    | type IDENTIFIER '(' param_list ')' block { $$ = ast_create_function_def($2, $1, $6, pParseData); }
    ;

message_block
    : '{' '}'               { $$ = ast_create_block(NULL, pParseData); }
    | '{' message_list '}'  { $$ = ast_create_block($2,   pParseData); }
    ;

message_list
    : message_prop              { $$ = ast_append(kAST_Block, NULL, $1, pParseData); }
    | message_list message_prop { $$ = ast_append(kAST_Block, $1, $2, pParseData); }
    ;
 
message_prop
    : '#' IDENTIFIER '(' param_list ')' block { $$ = ast_create_message_def($2, $6, pParseData); }
    | type '#' IDENTIFIER '=' expr ';'        { $$ = ast_create_property_def($3, $1, $5, pParseData); }
    | type '#' IDENTIFIER ';'                 { $$ = ast_create_property_def($3, $1, NULL, pParseData); }
    | type IDENTIFIER '=' expr ';'            { $$ = ast_create_field_def($2, $1, $4, pParseData); }
    | type IDENTIFIER ';'                     { $$ = ast_create_field_def($2, $1, NULL, pParseData); }
    ;

param_list
    : /* empty */                       { $$ = parsedata_add_param(pParseData, NULL, NULL); }
    | type IDENTIFIER                   { $$ = parsedata_add_param(pParseData, NULL, symrec_create(kSYMT_Param, $1, $2, NULL)); }
    | param_list ',' type IDENTIFIER    { $$ = parsedata_add_param(pParseData, $1, symrec_create(kSYMT_Param, $3, $4, NULL)); }
    ;

block
    : '{' '}'               { $$ = ast_create_block(NULL, pParseData); }
    | '{' stmt_list '}'     { $$ = ast_create_block($2,   pParseData); }
    ;

stmt_list
    : stmt            { $$ = ast_append(kAST_Block, NULL, $1, pParseData); }
    | stmt_list stmt  { $$ = ast_append(kAST_Block, $1, $2, pParseData); }
    ;
 
stmt
    : IF '(' cond_expr ')' stmt %prec THEN         { $$ = ast_create_if($3, $5, NULL, pParseData); }
    | IF '(' cond_expr ')' stmt ELSE stmt { $$ = ast_create_if($3, $5, $7,   pParseData); }

    | WHILE '(' cond_expr ')' stmt         { $$ = ast_create_while($3, $5, pParseData); }
    | DO do_stmt WHILE '(' cond_expr ')' ';'  { $$ = ast_create_dowhile($5, $2, pParseData); }

    | FOR '(' expr_or_empty ';' cond_expr_or_empty ';' expr_or_empty ')' stmt { $$ = ast_create_for($3, $5, $7, $9, pParseData); }

    | '@' target_expr ':' component_expr '#' IDENTIFIER '=' expr ';'            { $$ = ast_create_property_set($2, $4, $6, $8, pParseData); }
    | '@' target_expr ':' component_expr '#' IDENTIFIER '(' fun_params ')' ';'  { $$ = ast_create_message_send($2, $4, $6, $8, pParseData); }

    | block     { $$ = $1; }
    
    | expr ';'  { $$ = ast_create_simple_stmt($1, pParseData); }
    ;

do_stmt
    : stmt { parsedata_handle_do_scope(pParseData); $$ = $1; }

target_expr
    : /* empty */  { $$ = NULL; }
    | IDENTIFIER   { $$ = ast_create_identifier($1, pParseData); }
    | INT_LITERAL  { $$ = ast_create_int_literal($1, pParseData); }
    ;

component_expr
    : /* empty */  { $$ = NULL; }
    | IDENTIFIER   { $$ = ast_create_identifier($1, pParseData); }
    ;

expr
    : '(' expr ')'   { $$ = $2; }

    | type IDENTIFIER          { $$ = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, $1, $2, NULL)); }
    | type IDENTIFIER '=' expr { $$ = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, $1, $2, $4)); }
    
    | cond_expr        { $$ = $1; }
    
    | expr '+' expr    { $$ = ast_create_binary_op(kAST_Add,    $1, $3, pParseData); }
    | expr '-' expr    { $$ = ast_create_binary_op(kAST_Sub,    $1, $3, pParseData); }
    | expr '*' expr    { $$ = ast_create_binary_op(kAST_Mul,    $1, $3, pParseData); }
    | expr '/' expr    { $$ = ast_create_binary_op(kAST_Div,    $1, $3, pParseData); }
    | expr '%' expr    { $$ = ast_create_binary_op(kAST_Mod,    $1, $3, pParseData); }
    | expr LSHIFT expr { $$ = ast_create_binary_op(kAST_LShift, $1, $3, pParseData); }
    | expr RSHIFT expr { $$ = ast_create_binary_op(kAST_RShift, $1, $3, pParseData); }
    | expr OR expr     { $$ = ast_create_binary_op(kAST_Or,     $1, $3, pParseData); }
    | expr AND expr    { $$ = ast_create_binary_op(kAST_And,    $1, $3, pParseData); }
    | expr '|' expr    { $$ = ast_create_binary_op(kAST_BitOr,  $1, $3, pParseData); }
    | expr '^' expr    { $$ = ast_create_binary_op(kAST_BitXor, $1, $3, pParseData); }
    | expr '&' expr    { $$ = ast_create_binary_op(kAST_BitAnd, $1, $3, pParseData); }

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

    | literal   { $$ = $1; }

    | IDENTIFIER '(' fun_params ')'  { $$ = ast_create_function_call($1, $3, pParseData); }
    | IDENTIFIER                    { $$ = ast_create_symbol_ref($1, pParseData); }
    
    ;

cond_expr
    : expr EQ expr   { $$ = ast_create_binary_op(kAST_Eq,  $1, $3, pParseData); }
    | expr NEQ expr  { $$ = ast_create_binary_op(kAST_NEq, $1, $3, pParseData); }
    | expr LTE expr  { $$ = ast_create_binary_op(kAST_LTE, $1, $3, pParseData); }
    | expr GTE expr  { $$ = ast_create_binary_op(kAST_GTE, $1, $3, pParseData); }
    | expr '<' expr  { $$ = ast_create_binary_op(kAST_LT,  $1, $3, pParseData); }
    | expr '>' expr  { $$ = ast_create_binary_op(kAST_GT,  $1, $3, pParseData); }
    ;

literal
    : INT_LITERAL   { $$ = ast_create_int_literal($1, pParseData); }
    | FLOAT_LITERAL { $$ = ast_create_float_literal($1, pParseData); }
    | TRUE          { $$ = ast_create_int_literal(1, pParseData); }
    | FALSE         { $$ = ast_create_int_literal(0, pParseData); }
    ;

expr_or_empty
    : /* empty */  { $$ = NULL; }
    | expr         { $$ = $1; }
    ;

cond_expr_or_empty
    : /* empty */  { $$ = NULL; }
    | cond_expr    { $$ = $1; }
    ;

fun_params
    : /* empty */          { $$ = ast_append(kAST_FunctionParams, NULL, NULL, pParseData); }
    | expr                 { $$ = ast_append(kAST_FunctionParams, NULL, $1,   pParseData); }
    | fun_params ',' expr  { $$ = ast_append(kAST_FunctionParams, $1,   $3,   pParseData); }
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

