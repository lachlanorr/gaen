/*------------------------------------------------------------------------------
compose.l - Flex definition for compose lexer

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

%option reentrant
%option bison-bridge bison-locations
%option 8bit
%option warn nodefault
%option yylineno
%option never-interactive nounistd
%option noyywrap
%option noyyalloc noyyrealloc noyyfree
%option outfile="compose_scanner.c" header-file="compose_scanner.h"
%option extra-type="ParseData*"

%{
#include "compose/compiler.h"
#include "compose/compose_parser.h"
%}

D  [0-9]
H  [a-fA-F0-9]
L  [a-zA-Z_]

%%
%{
ParseData * pParseData = yyextra;    
%}
     
"int"    { return INT; }
"float"  { return FLOAT; }
"bool"   { return BOOL; }
"vec3"   { return VEC3; }

"::"   { return SCOPE; }

"=="   { return EQ; }
"!="   { return NEQ; }
"<="   { return LTE; }
">="   { return GTE; }

"++"   { return INC; }
"--"   { return DEC; }

"<<"   { return LSHIFT; }
">>"   { return RSHIFT; }

"&&"   { return AND; }
"||"   { return OR; }

"+="   { ADD_ASSIGN; }
"-="   { SUB_ASSIGN; }
"*="   { MUL_ASSIGN; }
"/="   { DIV_ASSIGN; }
"%="   { MOD_ASSIGN; }
"<<="  { LSHIFT_ASSIGN; }
">>="  { RSHIFT_ASSIGN; }
"&="   { AND_ASSIGN; }
"^="   { XOR_ASSIGN; }
"|="   { OR_ASSIGN; }

"#" |
"@" |
"+" |
"-" |
"*" |
"/" |
"%" |
"!" |
"~" |
"=" |
"," |
";" |
":" |
"." |
"&" |
"^" |
"|" |
"(" |
")" |
"[" |
"]" |
"{" |
"}" |
"<" |
">"    { return yytext[0]; }

0[xX]{H}+      { return INT_LITERAL; }
0{D}           { return INT_LITERAL; }
{D}            { return INT_LITERAL; }

{D}*"."{D}+    { return FLOAT_LITERAL; }
{D}+"."{D}*    { return FLOAT_LITERAL; }

{L}({L}|{D})*  { yylval->str = parsedata_add_string(pParseData, yytext); return IDENTIFIER; }

[ \t\v\r\n\f]   { }
.               { yyerror(yylloc, pParseData, "Invalid character '%c'", *yytext); return 0; }


%%

