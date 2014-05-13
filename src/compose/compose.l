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

#define YY_USER_ACTION                   \
  do {                                   \
    yylloc->first_column = yycolumn + 1; \
    yylloc->first_line = yylineno;       \
    yycolumn += (int)yyleng;             \
  } while(0);                             

#define YY_USER_INIT    \
  do {                  \
    yylineno = 1;       \
    yycolumn = 1;       \
  } while(0)

%}

D  [0-9]
H  [a-fA-F0-9]
L  [a-zA-Z_]

%%
%{
ParseData * pParseData = yyextra;    
%}
     
"int"    { yylval->dataType = kDT_int;   return INT;   }
"uint"   { yylval->dataType = kDT_uint;  return UINT;  }
"float"  { yylval->dataType = kDT_float; return FLOAT; }
"bool"   { yylval->dataType = kDT_bool;  return BOOL;  }
"char"   { yylval->dataType = kDT_char;  return CHAR;  }
"vec3"   { yylval->dataType = kDT_vec3;  return VEC3;  }
"vec4"   { yylval->dataType = kDT_vec4;  return VEC4;  }
"mat3"   { yylval->dataType = kDT_mat3;  return MAT3;  }
"mat34"  { yylval->dataType = kDT_mat34; return MAT34; }
"mat4"   { yylval->dataType = kDT_mat4;  return MAT4;  }
"void"   { yylval->dataType = kDT_void;  return VOID; }

"if"       { return IF;      }
"else"     { return ELSE;    }
"switch"   { return SWITCH;  }
"case"     { return CASE;    }
"default"  { return DEFAULT; }
"for"      { return FOR;     }
"while"    { return WHILE;   }
"do"       { return DO;      }

"break"    { return BREAK;   }
"return"   { return RETURN;  }

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
"<" |
">" |
"(" |
")" |
"[" |
"]" |
"{" |
"}"    { return yytext[0]; }

0[xX]{H}+      { yylval->numi = parse_int(yytext, 16); return INT_LITERAL; }
0{D}+          { yylval->numi = parse_int(yytext, 8);  return INT_LITERAL; }
{D}*           { yylval->numi = parse_int(yytext, 10); return INT_LITERAL; }

{D}*"."{D}+    { yylval->numf = parse_float(yytext); return FLOAT_LITERAL; }
{D}+"."{D}*    { yylval->numf = parse_float(yytext); return FLOAT_LITERAL; }

{L}({L}|{D})*  { yylval->str = parsedata_add_string(pParseData, yytext); return IDENTIFIER; }

[ \t\v\r\n\f]   { }
.               { yyerror(yylloc, pParseData, "Invalid character '%c'", *yytext); return 0; }


%%

