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
%}

%define api.pure full
%error-verbose

%lex-param { ParseData * pParseData }
%parse-param { ParseData * pParseData }

%locations
%output  "parser.c"
%defines "parser.h"

%union
{
    int      numi;
    double   numf;
    char*    str;
    Ast*     pAst;
    SymRec*  pSymRec;
}

%{
#define YY_NO_UNISTD_H
#include "compose/scanner.h"
#define YYLEX_PARAM parsedata_scanner(pParseData)
%}

%token <str> IDENTIFIER
%token <numi> INT_LITERAL
%token <numf> FLOAT_LITERAL

%token INT FLOAT BOOL VEC3

%right <pAstNode> '=' ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN

%left <pAstNode> OR
%left <pAstNode> AND
%left <pAstNode> '|'
%left <pAstNode> '^'
%left <pAstNode> '&'

%nonassoc <pAstNode> EQ NEQ
%nonassoc <pAstNode> LT GT LTE GTE

%left <pAstNode> LSHIFT RSHIFT

%left <pAstNode> '+' '-'
%left <pAstNode> '*' '/' '%'

%right <pAstNode> INC DEC '!' '~'
%right <pAstNode> UMINUS

%left <pAstNode> '.'

%left <pAstNode> SCOPE


%%

def_list
    : def
    | def_list def
    ;

def
    : message_def
    | function_def
    ;

message_def
    : '#' IDENTIFIER '(' param_list ')' block
    ;

function_def
    : type IDENTIFIER '(' param_list ')' block
    ;

param_list
    : param_decl
    | param_decl ',' param_list
    ;

param_decl
    : type IDENTIFIER
    ;

block
    : '{' '}'
    ;
/*    | '{' statement_list '}'
    | '{' declaration_list '}'
    | '{' declaration_list statement_list '}'
    ;
*/

type
    : INT
    | FLOAT
    | BOOL
    | VEC3
    ;

%%


