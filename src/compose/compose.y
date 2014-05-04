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
#include "compose/compiler_utils.h"
%}

%define api.pure full
%error-verbose

%lex-param { program * pProg }
%parse-param { program * pProg }

%locations
%output  "parser.c"
%defines "parser.h"

%union
{
    int          numi;
    double       numf;
    char*        str;
    ast_node*    pAstNode;
    sym_record*  pSymRec;
}

%{
#define YY_NO_UNISTD_H
#include "compose/scanner.h"
#define YYLEX_PARAM prog_scanner(pProg)
%}

%token <str> IDENTIFIER
%token <numi> INT_LITERAL
%token <numf> FLOAT_LITERAL

%token INT FLOAT BOOL VEC3
%token <ast_node*> MESSAGE

%%

decl_list
    : decl
    | decl_list decl
    ;

decl
    : message_decl
    ;

message_decl
    : MESSAGE IDENTIFIER '<' param_decl_list '>' block
    ;

param_decl_list
    : param_decl
    | param_decl_list ',' param_decl
    ;

param_decl
    : /* empty */
    | type IDENTIFIER
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


