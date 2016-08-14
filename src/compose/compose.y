/*------------------------------------------------------------------------------
compose.y - Bison definition for compose parser

Gaen Concurrency Engine - http://gaen.org
Copyright (c) 2014-2016 Lachlan Orr

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
//#define YYDEBUG 1
#include "compose/compiler.h"

#include "compose/comp_mem.h"
#define YYMALLOC COMP_ALLOC
#define YYFREE COMP_FREE

#include <stdio.h>
%}

%define api.pure full
%error-verbose

%lex-param { yyscan_t SCANNER_FROM_PARSEDATA }
%parse-param { ParseData * pParseData }

%locations
%output  "compose_parser.c"
%defines "compose_parser.h"

%union
{
    int                 numi;
    float               numf;
    const char*         str;
    DataType            dataType;
    Ast*                pAst;
    SymTab*             pSymTab;
    const SymDataType*  pSymDataType;
}

%{
#define YY_NO_UNISTD_H
#include "compose/compose_scanner.h"

/*
   This is embarrassingly hackish, but Bison 3's deprecation of
   YYLEX_PARAM doesn't give much choice.  Have researched as
   throrougly as I'm interested in doing at the moment, and this is
   the simplest (maybe only???) solution. Flex wants a scanner, and
   Bison wants to send it a variable, not allowing us to process that
   variable before sending through.
*/
#define SCANNER_FROM_PARSEDATA parsedata_scanner(pParseData)

#define YYPRINT(file, type, value)   yyprint (file, type, value)
static void yyprint(FILE * file, int type, YYSTYPE value);
%}

%token <str> IDENTIFIER HASH STRING_LITERAL
%token <numi> INT_LITERAL TRUE_ FALSE_
%token <numf> FLOAT_LITERAL

/* This type list must match the DataType enum in compiler.h */
%token <dataType> VOID_ BOOL_ CHAR_ BYTE_ SHORT_ USHORT_ INT_ UINT_ LONG_ ULONG_ HALF_ FLOAT_ DOUBLE_ COLOR VEC2 VEC3 VEC4 QUAT MAT3 MAT43 MAT4 HANDLE_ ASSET ENTITY STRING
%type <dataType> basic_type

%token IF SWITCH CASE DEFAULT FOR WHILE DO BREAK RETURN COMPONENT COMPONENTS UPDATE INPUT_ ANY NONE USING AS CONST_ SELF PRE POST VALUE
%right ELSE THEN

%right <pAst> '=' ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN TRANSFORM

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

%type <pSymDataType> type type_ent type_ent_handle_asset

%type <pAst> def stmt block stmt_list fun_params expr cond_expr expr_or_empty cond_expr_or_empty literal
%type <pAst> using_list using_stmt dotted_id dotted_id_proc dotted_id_part
%type <pAst> message_block message_list message_prop target_expr message_expr function_def
%type <pAst> prop_init_list prop_init component_block component_member_list component_member property_decl property_block property_def_list property_def input_block input_def_list input_def

%type <pSymTab> param_list

%%

cmp_file
    : def_list
    | using_list def_list  { ast_create_using_list($1, pParseData); }
    ;

using_list
    : using_stmt             { $$ = ast_append(kAST_UsingList, NULL, $1, pParseData); }
    | using_list using_stmt  { $$ = ast_append(kAST_UsingList, $1, $2, pParseData); }
    ;

using_stmt
    : USING dotted_id AS dotted_id ';'  { $$ = ast_create_using_stmt($2, $4, pParseData); }
    ;

dotted_id
    : dotted_id_proc  { $$ = ast_create_dotted_id($1, pParseData); }
    ;

dotted_id_proc
    : dotted_id_part                { $$ = ast_append(kAST_DottedId, NULL, $1, pParseData); }
    | dotted_id '.' dotted_id_part  { $$ = ast_append(kAST_DottedId, $1, $3, pParseData); }
    ;

dotted_id_part
    : IDENTIFIER { $$ = ast_create_with_str(kAST_DottedIdPart, $1, pParseData); }
    ;

def_list
    : def
    | def_list def
    ;

/* Treat "entity" specially since it is overloaded to be used to define entities */
def
    : ENTITY IDENTIFIER message_block                   { $$ = ast_create_entity_def($2, $3, pParseData); }
    | COMPONENT IDENTIFIER message_block                { $$ = ast_create_component_def($2, $3, pParseData); }
    | function_def                                      { $$ = parsedata_add_root_ast(pParseData, $1); }
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
    : HASH '(' param_list ')' block                 { $$ = ast_create_message_def($1, $3, $5, pParseData); }
    | property_decl property_block                  { $$ = ast_create_property_complex_def($1, $2, pParseData); }
    | property_decl '=' expr ';'                    { $$ = ast_create_property_def($1, $3, pParseData); }
    | property_decl ';'                             { $$ = ast_create_property_def($1, NULL, pParseData); }
    | type_ent_handle_asset IDENTIFIER '=' expr ';' { $$ = ast_create_field_def($2, $1, $4, pParseData); }
    | type_ent_handle_asset IDENTIFIER ';'          { $$ = ast_create_field_def($2, $1, NULL, pParseData); }
    | COMPONENTS component_block                    { $$ = ast_create_component_members($2, pParseData); }
	| UPDATE block                                  { $$ = ast_create_update_def($2, pParseData); }
	| INPUT_ HASH input_block                       { $$ = ast_create_input_mode($2, $3, pParseData); }
    | function_def                                  { $$ = $1; }
    ;

function_def
    : type_ent_handle_asset IDENTIFIER '(' param_list ')' block   { $$ = ast_create_function_def($2, $1, $4, $6, pParseData); }
    ;

param_list
    : /* empty */                                      { $$ = parsedata_add_param(pParseData, NULL, NULL); }
    | type_ent_handle_asset IDENTIFIER                 { $$ = parsedata_add_param(pParseData, NULL, symrec_create(kSYMT_Param, $1, $2, NULL, NULL, pParseData)); }
    | param_list ',' type_ent_handle_asset IDENTIFIER  { $$ = parsedata_add_param(pParseData, $1, symrec_create(kSYMT_Param, $3, $4, NULL, NULL, pParseData)); }
    ;

component_block
    : '{' '}'                        { $$ = ast_create_with_child_list(kAST_ComponentMemberList, pParseData); }
    | '{' component_member_list '}'  { $$ = $2; }
    ;

component_member_list
    : component_member                           { $$ = ast_append(kAST_ComponentMemberList, NULL, $1, pParseData); }
    | component_member_list ',' component_member { $$ = ast_append(kAST_ComponentMemberList, $1, $3, pParseData); }
    ;

component_member
    : dotted_id                        { $$ = ast_create_component_member($1, ast_create(kAST_PropInit, pParseData), pParseData); }
    | dotted_id '{' prop_init_list '}' { $$ = ast_create_component_member($1, $3, pParseData); }
    ;

prop_init_list
    : /* empty */                   { $$ = ast_create(kAST_PropInitList, pParseData); }
    | prop_init                     { $$ = ast_append(kAST_PropInitList, NULL, $1, pParseData); }
    | prop_init_list ',' prop_init  { $$ = ast_append(kAST_PropInitList, $1, $3, pParseData); }
    ;

prop_init
    : IDENTIFIER '=' expr  { $$ = ast_create_prop_init($1, $3, pParseData); }
    | TRANSFORM '=' expr   { $$ = ast_create_transform_init($3, pParseData); }
    ;

property_decl
    : type_ent_handle_asset HASH    { $$ = ast_create_property_decl($2, $1, pParseData); }

property_block
    : '{' '}'                    { $$ = ast_create_with_child_list(kAST_PropertyDef, pParseData); }
    | '{' property_def_list '}'  { $$ = $2; }
    ;

property_def_list
    : property_def                    { $$ = ast_append(kAST_PropertyDef, NULL, $1, pParseData); }
    | property_def_list property_def  { $$ = ast_append(kAST_PropertyDef, $1, $2, pParseData); }
    ;

property_def
    : DEFAULT '=' expr ';'  { $$ = ast_create_property_default_assign($3, pParseData); }
    | PRE block             { $$ = ast_create_property_pre($2, pParseData); }
    | POST block            { $$ = ast_create_property_post($2, pParseData); }
    ;

input_block
    : '{' '}'                 { $$ = ast_create_with_child_list(kAST_Input, pParseData); }
    | '{' input_def_list '}'  { $$ = $2; }
    ;

input_def_list
    : input_def                 { $$ = ast_append(kAST_Input, NULL, $1, pParseData); }
    | input_def_list input_def  { $$ = ast_append(kAST_Input, $1, $2, pParseData); }
    ;

input_def
    : HASH block { $$ = ast_create_input_def($1, $2, pParseData); }
    | ANY block  { $$ = ast_create_input_special_def("any", $2, pParseData); }
    | NONE block { $$ = ast_create_input_special_def("none", $2, pParseData); }
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
    : IF '(' expr ')' stmt %prec THEN         { $$ = ast_create_if($3, $5, NULL, pParseData); }
    | IF '(' expr ')' stmt ELSE stmt { $$ = ast_create_if($3, $5, $7,   pParseData); }

    | WHILE '(' expr ')' stmt         { $$ = ast_create_while($3, $5, pParseData); }
    | DO stmt WHILE '(' expr ')' ';'  { $$ = ast_create_dowhile($5, $2, pParseData); }

    | FOR '(' expr_or_empty ';' cond_expr_or_empty ';' expr_or_empty ')' stmt { $$ = ast_create_for($3, $5, $7, $9, pParseData); }

    | '@' target_expr HASH '=' expr ';'            { $$ = ast_create_property_set($2, ast_create_hash($3, pParseData), $5, pParseData); }
    | '@' target_expr HASH '(' fun_params ')' ';'  { $$ = ast_create_message_send($2, ast_create_hash($3, pParseData), $5, pParseData); }

    | '@' target_expr ':' message_expr '=' expr ';'            { $$ = ast_create_property_set($2, $4, $6, pParseData); }
    | '@' target_expr ':' message_expr '(' fun_params ')' ';'  { $$ = ast_create_message_send($2, $4, $6, pParseData); }

    | RETURN expr ';'  { $$ = ast_create_return($2, pParseData); }

    | block     { $$ = $1; }
    
    | expr ';'  { $$ = ast_create_simple_stmt($1, pParseData); }

    | INPUT_ '=' expr ';' { $$ = ast_create_input_assign($3, pParseData); }
    ;

target_expr
    : /* empty */  { $$ = NULL; }
    | IDENTIFIER   { $$ = ast_create_identifier($1, pParseData); }
    | SELF         { $$ = ast_create(kAST_Self, pParseData); }
    | INT_LITERAL  { $$ = ast_create_int_literal($1, pParseData); }
    ;

message_expr
    : IDENTIFIER   { $$ = ast_create_identifier($1, pParseData); }
    | SELF         { $$ = ast_create(kAST_Self, pParseData); }
    | INT_LITERAL  { $$ = ast_create_int_literal($1, pParseData); }
	| '(' expr ')' { $$ = $2; }
    ;

expr
    : '(' expr ')'   { $$ = $2; }

    | type_ent IDENTIFIER          { $$ = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, $1, $2, NULL, NULL, pParseData)); }
    | type_ent IDENTIFIER '=' expr { $$ = parsedata_add_local_symbol(pParseData, symrec_create(kSYMT_Local, $1, $2, NULL, $4, pParseData)); }
    
    | STRING_LITERAL { $$ = ast_create_string_literal($1, pParseData); }

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

    | dotted_id '=' expr           { $$ = ast_create_assign_op(kAST_Assign,       $1, $3, pParseData); }
    | dotted_id ADD_ASSIGN expr    { $$ = ast_create_assign_op(kAST_AddAssign,    $1, $3, pParseData); }
    | dotted_id SUB_ASSIGN expr    { $$ = ast_create_assign_op(kAST_SubAssign,    $1, $3, pParseData); }
    | dotted_id MUL_ASSIGN expr    { $$ = ast_create_assign_op(kAST_MulAssign,    $1, $3, pParseData); }
    | dotted_id DIV_ASSIGN expr    { $$ = ast_create_assign_op(kAST_DivAssign,    $1, $3, pParseData); }
    | dotted_id MOD_ASSIGN expr    { $$ = ast_create_assign_op(kAST_ModAssign,    $1, $3, pParseData); }
    | dotted_id LSHIFT_ASSIGN expr { $$ = ast_create_assign_op(kAST_LShiftAssign, $1, $3, pParseData); }
    | dotted_id RSHIFT_ASSIGN expr { $$ = ast_create_assign_op(kAST_RShiftAssign, $1, $3, pParseData); }
    | dotted_id AND_ASSIGN expr    { $$ = ast_create_assign_op(kAST_AndAssign,    $1, $3, pParseData); }
    | dotted_id XOR_ASSIGN expr    { $$ = ast_create_assign_op(kAST_XorAssign,    $1, $3, pParseData); }
    | dotted_id OR_ASSIGN expr     { $$ = ast_create_assign_op(kAST_OrAssign,     $1, $3, pParseData); }

    | '!' expr              { $$ = ast_create_unary_op(kAST_Not,        $2, pParseData); }
    | '~' expr              { $$ = ast_create_unary_op(kAST_Complement, $2, pParseData); }
    | '-' expr %prec UMINUS { $$ = ast_create_unary_op(kAST_Negate,     $2, pParseData); }

    | HASH                   { $$ = ast_create_hash($1, pParseData); }

    | INC expr               { $$ = ast_create_unary_op(kAST_PreInc, $2, pParseData); }
    | DEC expr               { $$ = ast_create_unary_op(kAST_PreDec, $2, pParseData); }
    | expr INC %prec POSTINC { $$ = ast_create_unary_op(kAST_PostInc, $1, pParseData); }
    | expr DEC %prec POSTDEC { $$ = ast_create_unary_op(kAST_PostDec, $1, pParseData); }

    | literal   { $$ = $1; }

    | basic_type '{' fun_params '}'      { $$ = ast_create_type_init($1, $3, pParseData); }
    | dotted_id  '{' prop_init_list '}'  { $$ = ast_create_entity_or_struct_init($1, $3, pParseData); }
    | dotted_id  '(' fun_params ')'      { $$ = ast_create_function_call($1, $3, pParseData); }

    | '$' '.' IDENTIFIER '(' fun_params ')'  { $$ = ast_create_system_api_call($3, $5, pParseData); }

    | TRANSFORM  { $$ = ast_create(kAST_Transform, pParseData); }
    ;

cond_expr
    : expr EQ expr   { $$ = ast_create_binary_op(kAST_Eq,  $1, $3, pParseData); }
    | expr NEQ expr  { $$ = ast_create_binary_op(kAST_NEq, $1, $3, pParseData); }
    | expr LTE expr  { $$ = ast_create_binary_op(kAST_LTE, $1, $3, pParseData); }
    | expr GTE expr  { $$ = ast_create_binary_op(kAST_GTE, $1, $3, pParseData); }
    | expr '<' expr  { $$ = ast_create_binary_op(kAST_LT,  $1, $3, pParseData); }
    | expr '>' expr  { $$ = ast_create_binary_op(kAST_GT,  $1, $3, pParseData); }
    | dotted_id      { $$ = ast_create_symbol_ref($1, pParseData); }
    ;

literal
    : INT_LITERAL   { $$ = ast_create_int_literal($1, pParseData); }
    | FLOAT_LITERAL { $$ = ast_create_float_literal($1, pParseData); }
    | TRUE_         { $$ = ast_create_int_literal(1, pParseData); }
    | FALSE_        { $$ = ast_create_int_literal(0, pParseData); }
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
    : CONST_ basic_type  { $$ = parsedata_find_basic_type(pParseData, $2, 1, 0); }
    | basic_type         { $$ = parsedata_find_basic_type(pParseData, $1, 0, 0); }
    | CONST_ dotted_id   { $$ = parsedata_find_type_from_dotted_id(pParseData, $2, 1, 0); }
    | dotted_id          { $$ = parsedata_find_type_from_dotted_id(pParseData, $1, 0, 0); }
    ;

basic_type
    : VOID_
    | BOOL_
    | CHAR_
    | BYTE_
    | SHORT_
    | USHORT_
    | INT_
    | UINT_
    | LONG_
    | ULONG_
    | HALF_
    | FLOAT_
    | DOUBLE_
    | COLOR
    | VEC2
    | VEC3
    | VEC4
    | QUAT
    | MAT3
    | MAT43
    | MAT4
    | STRING
    ;

/*
    Treat "entity" type specially since it is overloaded with use of
    defining entities.
*/
type_ent
    : type           { $$ = $1; }
    | CONST_ ENTITY  { $$ = parsedata_find_type(pParseData, "entity", 1, 0); }
    | ENTITY         { $$ = parsedata_find_type(pParseData, "entity", 0, 0); }
    ;

/*
   "asset" is sort of a type, but only applicable as a top level
   statement within a component or entity, so we special case here.
*/
type_ent_handle_asset
    : type_ent   { $$ = $1; }
    | HANDLE_    { $$ = parsedata_find_type(pParseData, "handle", 0, 0); }
    | ASSET      { $$ = parsedata_find_type(pParseData, "asset_handle", 0, 0); }
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

