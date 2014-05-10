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

#ifndef NULL
#define NULL 0    
#endif

// Macro so we can call YYABORT if any of our funcs fail.
#define CHECK(val) \
do \
{ \
    if (!(val)) \
    { \
        YYABORT; \
    } \
} while (0)


typedef enum
{
    kAST_Root,
    kAST_MessageDef,

    kAST_Add,
    kAST_Subtract,
    kAST_Multiply,
    kAST_Divide,
    kAST_Modulus,

    kAST_Not,
    kAST_Complement,
    kAST_Negate,

    kAST_IntLiteral,
    kAST_FloatLiteral,
    kAST_StringLiteral
} AstType;

typedef enum
{
    kSCOPE_Function,
    kSCOPE_Param,
    kSCOPE_Local,
    kSCOPE_Property
} SymbolScope;

typedef enum
{
    kDT_int,
    kDT_uint,
    kDT_float,
    kDT_bool,
    kDT_vec3
} DataType;

typedef struct SymRec SymRec;
typedef struct SymTab SymTab;
typedef struct AstList AstList;
typedef struct Ast Ast;
typedef struct ParseData ParseData;


int parse_int(const char * pStr, int base);
float parse_float(const char * pStr);

SymRec * symrec_create(SymbolScope scope,
                       enum yytokentype type,
                       const char * name,
                       Ast * pValue);

SymTab* symtab_create();
SymTab* symtab_add_record(SymTab* pSymTab, SymRec * pSymRec);

AstList * astlist_create();
AstList * astlist_append(AstList * pAstList, Ast * pAst);

Ast * ast_create(AstType astType, ParseData * pParseData);
Ast * ast_create_message_def(AstList * pStmtList, ParseData * pParseData);
Ast * ast_create_unary_op(AstType astType, Ast * pRhs, ParseData * pParseData);
Ast * ast_create_binary_op(AstType astType, Ast * pLhs, Ast * pRhs, ParseData * pParseData);

Ast * ast_add_list(Ast * pAst, AstList * pAstList);
Ast * ast_add_child(Ast * pAst, Ast * pChild);
Ast * ast_add_children(Ast * pAst, AstList * pChildren);

ParseData * parsedata_create();
void *  parsedata_scanner(ParseData * pParseData);
Ast*    parsedata_add_def(ParseData *pParseData, Ast * pAst);

// Adds to a specific symbol table, or to a new one if you send NULL
// This is used for parameters of functions, or complex statements
// (like a for loop with variable initialization)
SymTab* parsedata_add_symbol(ParseData * pParseData, SymTab* pSymTab, SymRec * pSymRec);

// Adds to the symbol table on top of the stack currently.
// A new symbol table should already have been added to the stack.
SymTab* parsedata_add_local_symbol(ParseData * pParseData, SymRec * pSymRec);

SymTab* parsedata_current_scope(ParseData * pParseData);
SymTab* parsedata_push_scope(ParseData * pParseData, SymTab * pSymTab);
SymTab* parsedata_pop_scope(ParseData * pParseData);
const char * parsedata_add_string(ParseData * pParseData, const char * str);

void parse_init();
ParseData * parse(const char * source, size_t length);

typedef struct YYLTYPE YYLTYPE;
void yyerror(YYLTYPE * pLoc, ParseData * pParseData, const char * format, ...);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef GAEN_COMPOSE_COMPILER_H

