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

#define COMP_ERROR(format, ...)   parsedata_formatted_message(pParseData,kMSGT_Error,format,##__VA_ARGS__)
#define COMP_WARNING(format, ...) parsedata_formatted_message(pParseData,kMSGT_Warning,format,##__VA_ARGS__)
#define COMP_INFO(format, ...)    parsedata_formatted_message(pParseData,kMSGT_Info,format,##__VA_ARGS__)

typedef enum
{
    kAST_Undefined = 0,

    kAST_Root,
    kAST_MessageDef,
    kAST_FunctionDef,

    kAST_FunctionCall,
    kAST_SymbolRef,
    
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
    kSYMT_Undefined = 0,
    kSYMT_Message,
    kSYMT_Function,
    kSYMT_Param,
    kSYMT_Local,
    kSYMT_Property
} SymType;

typedef enum
{
    kDT_Undefined = 0,
    kDT_int,
    kDT_uint,
    kDT_float,
    kDT_bool,
    kDT_vec3,
    kDT_void
} DataType;

typedef enum
{
    kMSGT_Info,
    kMSGT_Warning,
    kMSGT_Error,
} MessageType;

typedef void(*MessageHandler)(MessageType messageType, const char * message, const char * filename, int line, int column);

typedef struct SymRec SymRec;
typedef struct SymTab SymTab;
typedef struct AstList AstList;
typedef struct Ast Ast;
typedef struct ParseData ParseData;

int parse_int(const char * pStr, int base);
float parse_float(const char * pStr);

SymRec * symrec_create(SymType symType,
                       DataType type,
                       const char * name,
                       Ast * pAst);

SymTab* symtab_create();
SymTab* symtab_add_symbol(SymTab* pSymTab, SymRec * pSymRec, ParseData * pParseData);
SymRec* symtab_find_symbol(SymTab* pSymTab, const char * name);

AstList * astlist_create();
AstList * astlist_append(AstList * pAstList, Ast * pAst);


Ast * ast_create(AstType astType, ParseData * pParseData);

Ast * ast_create_message_def(const char * name, AstList * pStmtList, ParseData * pParseData);
Ast * ast_create_function_def(const char * name, DataType returnType, AstList * pStmtList, ParseData * pParseData);

Ast * ast_create_unary_op(AstType astType, Ast * pRhs, ParseData * pParseData);
Ast * ast_create_binary_op(AstType astType, Ast * pLhs, Ast * pRhs, ParseData * pParseData);

Ast * ast_create_int_literal(int numi, ParseData * pParseData);
Ast * ast_create_float_literal(float numf, ParseData * pParseData);

Ast * ast_create_function_call(const char * str, AstList * pExpList, ParseData * pParseData);
Ast * ast_create_symbol_ref(const char * str, ParseData * pParseData);


Ast * ast_add_child(Ast * pParent, Ast * pChild);
Ast * ast_add_children(Ast * pParent, AstList * pChildren);

ParseData * parsedata_create(const char * filename, MessageHandler messageHandler);
void *  parsedata_scanner(ParseData * pParseData);
Ast*    parsedata_add_def(ParseData *pParseData, Ast * pAst);

void parsedata_formatted_message(ParseData * pParseData,
                                 MessageType messageType,
                                 const char * format, ...);

// Adds to a specific symbol table, or to a new one if you send NULL
// This is used for parameters of functions, or complex statements
// (like a for loop with variable initialization)
SymTab* parsedata_add_symbol(ParseData * pParseData, SymTab* pSymTab, SymRec * pSymRec);

// Adds to the symbol table on top of the stack currently.
// A new symbol table should already have been added to the stack.
SymTab* parsedata_add_local_symbol(ParseData * pParseData, SymRec * pSymRec);

SymRec* parsedata_find_symbol(ParseData * pParseData, const char * name);

SymTab* parsedata_current_scope(ParseData * pParseData);
SymTab* parsedata_push_scope(ParseData * pParseData, SymTab * pSymTab);
SymTab* parsedata_pop_scope(ParseData * pParseData);
const char * parsedata_add_string(ParseData * pParseData, const char * str);

void parse_init();
ParseData * parse(ParseData * pParseData,
                  const char * source,
                  size_t length,
                  const char * filename,
                  MessageHandler messageHandler);

ParseData * parse_file(ParseData * pParseData,
                       const char * filename,
                       MessageHandler messageHandler);

typedef struct YYLTYPE YYLTYPE;
void yyerror(YYLTYPE * pLoc, ParseData * pParseData, const char * format, ...);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef GAEN_COMPOSE_COMPILER_H

