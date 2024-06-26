/*------------------------------------------------------------------------------
compose.l - Flex definition for compose lexer

Gaen Concurrency Engine - http://gaen.org
Copyright (c) 2014-2022 Lachlan Orr

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
%option noinput
%option nounput
%option outfile="compose_scanner.c" header-file="compose_scanner.h"
%option extra-type="ParseData*"

%{
#include "gaen/compose/compiler.h"
#include "gaen/compose/compose_parser.h"
#include "gaen/compose/comp_mem.h"

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

%x COMMENT

D  [0-9]
H  [a-fA-F0-9]
L  [a-zA-Z_]
CRLF (\r\n|\r|\n)

%%
%{
ParseData * pParseData = yyextra;
%}

"/*"                       { BEGIN(COMMENT); }
<COMMENT>"*/"              { BEGIN(INITIAL); }
<COMMENT>([^*]|{CRLF})+|.
<COMMENT><<EOF>>           { yyerror(yylloc, pParseData, "Unterminated comment"); return 0; }
"//".*{CRLF}

"int"    { yylval->dataType = kDT_int;    return INT_;    }
"float"  { yylval->dataType = kDT_float;  return FLOAT_;  }
"bool"   { yylval->dataType = kDT_bool;   return BOOL_;   }
"color"  { yylval->dataType = kDT_color;  return COLOR;   }
"vec2"   { yylval->dataType = kDT_vec2;   return VEC2;    }
"vec3"   { yylval->dataType = kDT_vec3;   return VEC3;    }
"vec4"   { yylval->dataType = kDT_vec4;   return VEC4;    }
"ivec2"  { yylval->dataType = kDT_ivec2;  return IVEC2;   }
"ivec3"  { yylval->dataType = kDT_ivec3;  return IVEC3;   }
"ivec4"  { yylval->dataType = kDT_ivec4;  return IVEC4;   }
"quat"   { yylval->dataType = kDT_quat;   return QUAT;    }
"mat3"   { yylval->dataType = kDT_mat3;   return MAT3;    }
"mat43"  { yylval->dataType = kDT_mat43;  return MAT43;   }
"mat4"   { yylval->dataType = kDT_mat4;   return MAT4;    }
"void"   { yylval->dataType = kDT_void;   return VOID_;   }
"handle" { yylval->dataType = kDT_handle; return HANDLE_; }
"string" { yylval->dataType = kDT_string; return STRING;  }
"asset"  { yylval->dataType = kDT_asset;  return ASSET; }

"self"     { return SELF; }
"creator"  { return CREATOR; }
"renderer" { return RENDERER; }
"source"   { return SOURCE; }

"if"       { return IF;     }
"else"     { return ELSE;   }
"while"    { return WHILE;  }
"do"       { return DO;     }
"for"      { parsedata_push_scope(pParseData); return FOR;    }
"switch"   { return SWITCH; }
"case"     { return CASE;    }
"default"  { return DEFAULT; }

"break"    { return BREAK;   }
"return"   { return RETURN;  }

"const"    { return CONST_; }

"using"      { return USING; }
"as"         { return AS; }
"components" { return COMPONENTS; }
"entity"     { yylval->dataType = kDT_entity; return ENTITY; }
"component"  { return COMPONENT; }

"update"     { parsedata_push_update_scope(pParseData); return UPDATE; }
"input"      { parsedata_push_input_scope(pParseData); return INPUT_; }
"any"        { return ANY; }
"none"       { return NONE; }

"pre"        { parsedata_push_prop_prepost_scope(pParseData); return PRE; }
"post"       { parsedata_push_prop_prepost_scope(pParseData); return POST; }

"transform"  { return TRANSFORM; }
"ready"      { return READY; }
"parent"     { return PARENT; }
"visible"    { return VISIBLE; }


"true"   { return TRUE_; }
"false"  { return FALSE_; }

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

"+="   { return ADD_ASSIGN; }
"-="   { return SUB_ASSIGN; }
"*="   { return MUL_ASSIGN; }
"/="   { return DIV_ASSIGN; }
"%="   { return MOD_ASSIGN; }
"<<="  { return LSHIFT_ASSIGN; }
">>="  { return RSHIFT_ASSIGN; }
"&="   { return AND_ASSIGN; }
"^="   { return XOR_ASSIGN; }
"|="   { return OR_ASSIGN; }

"@" |
"$" |
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
"]"    { return yytext[0]; }

"{"    { parsedata_push_scope(pParseData); return yytext[0]; }
"}"    { return yytext[0]; }

#{L}({L}|{D})*    { yylval->str = parse_identifier(yytext+1, pParseData); return HASH; }
\"(\\.|[^"])*\"   { yylval->str = comp_strdup(yytext); return STRING_LITERAL; } 

0[xX]{H}+      { yylval->numi = parse_int(yytext, 16); return INT_LITERAL; }
0{D}+          { yylval->numi = parse_int(yytext, 8);  return INT_LITERAL; }
{D}+           { yylval->numi = parse_int(yytext, 10); return INT_LITERAL; }

{D}*"."{D}+    { yylval->numf = parse_float(yytext); return FLOAT_LITERAL; }
{D}+"."{D}*    { yylval->numf = parse_float(yytext); return FLOAT_LITERAL; }

{L}({L}|{D})*  { yylval->str = parse_identifier(yytext, pParseData); return IDENTIFIER; }

[ \t\v\r\n\f]   { }
.               { yyerror(yylloc, pParseData, "Invalid character '%c'", *yytext); return 0; }


%%

