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
%option outfile="scanner.c" header-file="scanner.h"
%option extra-type="program*"

%{
#include "compose/compiler_utils.h"
#include "compose/parser.h"
%}

D  [0-9]
H  [a-fA-F0-9]
L  [a-zA-Z_]

%%
%{
program * pProg = yyextra;    
%}
     
"message" { return MESSAGE; }

"int"    { return INT; }
"float"  { return FLOAT; }
"bool"   { return BOOL; }
"vec3"   { return VEC3; }


{L}({L}|{D})*  { return IDENTIFIER; }

0[xX]{H}+      { return INT_LITERAL; }
0{D}           { return INT_LITERAL; }
{D}            { return INT_LITERAL; }

{D}*"."{D}+    { return FLOAT_LITERAL; }
{D}+"."{D}*    { return FLOAT_LITERAL; }


[ \t\v\n\f]     { }
.               { /* ignore bad characters */ }


%%

