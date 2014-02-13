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

%{

// To generate this file, run: "flex compose.l"

#include <stdio.h>
#include "compose/parser.h"

void count();
int check_type();

%}

%option 8bit reentrant bison-bridge
%option warn nodefault
%option yylineno
%option outfile="scanner.c" header-file="scanner.h"
%option never-interactive nounistd

%x IN_COMMENT

D           [0-9]
L           [a-zA-Z_]
H           [a-fA-F0-9]
E           [Ee][+-]?{D}+

%%
<INITIAL>{
"/*"              BEGIN(IN_COMMENT);
}
<IN_COMMENT>{
"*/"      BEGIN(INITIAL);
[^*\n]+   // eat comment in chunks
"*"       // eat the lone star
\n        yylineno++;
}
"//"[^\n]*      { }

"enum"          { return(ENUM); }
"struct"        { return(STRUCT); }

"message"       { return(MESSAGE); }
"event"         { return(EVENT); }
"property"      { return(PROPERTY); }

"int"           { return(INT); }
"float"         { return(FLOAT); }
"color"         { return(COLOR); }
"vec2"          { return(VEC2); }
"vec3"          { return(VEC3); }
"vec4"          { return(VEC4); }
"mat3"          { return(MAT3); }
"mat34"         { return(MAT34); }
"mat4"          { return(MAT4); }

"void"          { return(VOID); }

"break"         { return(BREAK); }
"case"          { return(CASE); }
"continue"      { return(CONTINUE); }
"default"       { return(DEFAULT); }
"do"            { return(DO); }
"else"          { return(ELSE); }
"for"           { return(FOR); }
"if"            { return(IF); }
"return"        { return(RETURN); }
"switch"        { return(SWITCH); }
"while"         { return(WHILE); }

{L}({L}|{D})*       { return(check_type()); }

0[xX]{H}            { return(CONSTANT); }
0{D}                { return(CONSTANT); }
{D}                 { return(CONSTANT); }

{D}+{E}             { return(CONSTANT); }
{D}*"."{D}+({E})?   { return(CONSTANT); }
{D}+"."{D}*({E})?   { return(CONSTANT); }

\"(\\.|[^\\"])*\" { return(STRING_LITERAL); }

">>="           { return(RIGHT_ASSIGN); }
"<<="           { return(LEFT_ASSIGN); }
"+="            { return(ADD_ASSIGN); }
"-="            { return(SUB_ASSIGN); }
"*="            { return(MUL_ASSIGN); }
"/="            { return(DIV_ASSIGN); }
"%="            { return(MOD_ASSIGN); }
"&="            { return(AND_ASSIGN); }
"^="            { return(XOR_ASSIGN); }
"|="            { return(OR_ASSIGN); }
">>"            { return(RIGHT_OP); }
"<<"            { return(LEFT_OP); }
"++"            { return(INC_OP); }
"--"            { return(DEC_OP); }
"&&"            { return(AND_OP); }
"||"            { return(OR_OP); }
"<="            { return(LE_OP); }
">="            { return(GE_OP); }
"=="            { return(EQ_OP); }
"!="            { return(NE_OP); }
";"             { return(';'); }
("{")           { return('{'); }
("}")           { return('}'); }
","             { return(','); }
":"             { return(':'); }
"="             { return('='); }
"("             { return('('); }
")"             { return(')'); }
("["|"<:")      { return('['); }
("]"|":>")      { return(']'); }
"."             { return('.'); }
"&"             { return('&'); }
"!"             { return('!'); }
"~"             { return('~'); }
"-"             { return('-'); }
"+"             { return('+'); }
"*"             { return('*'); }
"/"             { return('/'); }
"%"             { return('%'); }
"<"             { return('<'); }
">"             { return('>'); }
"^"             { return('^'); }
"|"             { return('|'); }
"?"             { return('?'); }
"#"             { return('#'); }

[ \t\v\n\f]     { }
.               { /* ignore bad characters */ }

%%

yywrap()
{
    return(1);
}

int check_type()
{
/*
* pseudo code --- this is what it should check
*
*   if (yytext == type_name)
*       return(TYPE_NAME);
*
*   return(IDENTIFIER);
*/

/*
*   it actually will only return IDENTIFIER
*/

    return(IDENTIFIER);
}

