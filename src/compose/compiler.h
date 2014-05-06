//------------------------------------------------------------------------------
// compiler.h - Compose script compiler
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.
//------------------------------------------------------------------------------

#ifndef GAEN_COMPOSE_COMPILER_H
#define GAEN_COMPOSE_COMPILER_H

#ifdef __cplusplus
extern "C" {
#endif

// C Declarations, to be used from Bison

typedef enum
{
    kAST_Plus       = '+',
    kAST_Minus      = '-',
    kAST_Times      = '*',
    kAST_Divide     = '/',
    kAST_MessageDef = 256
} AstType;

typedef enum
{
    kSCOPE_Param,
    kSCOPE_Function,
    kSCOPE_Property,
    kSCOPE_Local
} SymbolScope;

typedef enum
{
    kTYPE_Int,
    kTYPE_Float,
    kTYPE_Bool,
    kTYPE_Vec3
} DataType;

typedef struct SymRec SymRec;
typedef struct SymTab SymTab;
typedef struct Ast Ast;
typedef struct ParseData ParseData;

SymRec * symrec_create(const char * name);
void symrec_destroy(SymRec* pSymRec);

SymTab* symtab_create();
void symtab_destroy(SymTab* pSymTab);
void symtab_add_entry(SymTab* pSymTab, SymRec * pSymRec);
void symtab_push_scope(SymTab* pSymTab);
void symtab_pop_scope(SymTab* pSymTab);

Ast * ast_create(AstType astType, const char * strVal, SymRec * pSymRec);
void ast_destroy(Ast * pAstNode);

ParseData * parsedata_create();
void parsedata_destroy(ParseData * pParseData);
void * parsedata_scanner(ParseData * pParseData);

ParseData * parse(const char * source, size_t length);

typedef struct YYLTYPE YYLTYPE;
void yyerror(YYLTYPE * pLoc, ParseData * pParseData, const char * msg);

#ifdef __cplusplus
} // #ifdef __cplusplus
#endif

#endif // #ifndef GAEN_COMPOSE_COMPILER_H

