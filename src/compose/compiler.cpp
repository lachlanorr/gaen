//------------------------------------------------------------------------------
// compiler.cpp - Compose script compiler
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

#include <cstdarg>
#include <cstdlib>

#include "core/platutils.h"

#include "compose/compiler.h"
#include "compose/compiler_structs.h"
#include "compose/codegen_utils.h"
#include "compose/utils.h"

extern "C" {
#define YYDEBUG 1
#include "compose/compose_parser.h"
#define YY_NO_UNISTD_H
#include "compose/compose_scanner.h"
}

using namespace gaen;


int parse_int(const char * pStr, int base)
{
    i64 val = strtol(pStr, nullptr, base);
    // LORRTODO - check for over/underflow
    return static_cast<i32>(val);
}

float parse_float(const char * pStr)
{
    f32 val = strtof(pStr, nullptr);
    // LORRTODO - check for over/underflow
    return val;
}

//------------------------------------------------------------------------------
// SymRec
//------------------------------------------------------------------------------
SymRec * symrec_create(SymType symType,
                       DataType dataType,
                       const char * name,
                       Ast * pAst)
{
    SymRec * pSymRec = COMP_NEW(SymRec);

    pSymRec->type = symType;
    pSymRec->dataType = dataType;
    pSymRec->name = name;
    pSymRec->pAst = pAst;

    pSymRec->pSymTab = nullptr;

    return pSymRec;
}
//------------------------------------------------------------------------------
// SymRec (END)
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// SymTab
//------------------------------------------------------------------------------
SymTab* symtab_create()
{
    SymTab* pSymTab = COMP_NEW(SymTab);
    pSymTab->pParent = nullptr;
    pSymTab->pAst = nullptr;
    return pSymTab;
}

SymTab* symtab_add_symbol(SymTab* pSymTab, SymRec * pSymRec, ParseData * pParseData)
{
    ASSERT(pSymTab && pSymRec);
    ASSERT(pSymRec->pSymTab == nullptr);
    ASSERT(pSymRec->name);

    SymRec * pSymRecSearch = symtab_find_symbol(pSymTab, pSymRec->name);

    if (pSymRecSearch)
    {
        COMP_ERROR("Symbol already defined: %s", pSymRec->name);
        return pSymTab;
    }

    pSymRec->pSymTab = pSymTab;
    pSymRec->order = static_cast<u32>(pSymTab->dict.size());
    pSymTab->dict[pSymRec->name] = pSymRec;
    return pSymTab;
}

SymRec* symtab_find_symbol(SymTab* pSymTab, const char * name)
{
    ASSERT(pSymTab);
    ASSERT(name);

    auto it = pSymTab->dict.find(name);

    if (it != pSymTab->dict.end())
    {
        return it->second;
    }
    return nullptr;
}

SymRec* symtab_find_symbol_recursive(SymTab* pSymTab, const char * name)
{
    ASSERT(pSymTab);
    ASSERT(name);

    SymRec * pSymRec = symtab_find_symbol(pSymTab, name);

    if (pSymRec)
    {
        return pSymRec;
    }
    else if (pSymTab->pParent)
    {
        return symtab_find_symbol_recursive(pSymTab->pParent, name);
    }

    return nullptr;
}

SymTab* symtab_transfer(SymTab* pDest, SymTab* pSrc, ParseData* pParseData)
{
    for (auto it : pSrc->dict)
    {
        SymRec * pSymRec = it.second;
        pSymRec->pSymTab = nullptr;
        symtab_add_symbol(pDest, it.second, pParseData);
    }
    pSrc->pAst = nullptr;
    pSrc->dict.clear();
    return pDest;
}

//------------------------------------------------------------------------------
// SymTab (END)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// AstList
//------------------------------------------------------------------------------
AstList * astlist_create()
{
    AstList * pAstList = COMP_NEW(AstList);
    return pAstList;
}

AstList * astlist_append(AstList * pAstList, Ast * pAst)
{
    if (!pAstList)
    {
        pAstList = astlist_create();
    }
    if (pAst)
    {
        pAstList->nodes.push_back(pAst);
    }
    return pAstList;
}
//------------------------------------------------------------------------------
// AstList (END)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Ast
//------------------------------------------------------------------------------
Ast * ast_create(AstType astType, ParseData * pParseData)
{
    ASSERT(pParseData);

    Ast * pAst = COMP_NEW(Ast);

    pAst->type = astType;
    pAst->pParent = nullptr;
    pAst->pScope = parsedata_current_scope(pParseData);
    pAst->pSymRec = nullptr;

    pAst->pLhs = nullptr;
    pAst->pMid = nullptr;
    pAst->pRhs = nullptr;

    pAst->numi = 0;
    pAst->numf = 0;
    pAst->str = nullptr;

    pAst->pChildren = nullptr;
    pAst->fullPath = pParseData->fullPath;

    pAst->pBlockInfos = 0;

    return pAst;
}



static Ast * ast_create_block_def(const char * name,
                                  AstType astType,
                                  SymType symType,
                                  DataType returnType,
                                  Ast * pBlock,
                                  Ast * pParent,
                                  ParseData * pParseData)
{
    ASSERT(pBlock);
    ASSERT(pBlock->pChildren);
    ASSERT(pParseData);

    Ast * pAst = ast_create(astType, pParseData);
    ast_add_children(pAst, pBlock->pChildren);
    pAst->pScope = pBlock->pScope;
    pAst->pScope->pSymTab->pAst = pAst;
    
    pAst->pSymRec = symrec_create(symType,
                                  returnType,
                                  name,
                                  pAst);

    parsedata_add_local_symbol(pParseData, pAst->pSymRec);

    if (pParent)
        ast_add_child(pParent, pAst);

    return pAst;
}

Ast * ast_create_function_def(const char * name, DataType returnType, Ast * pBlock, ParseData * pParseData)
{
    Ast * pAst = ast_create_block_def(name,
                                      kAST_FunctionDef,
                                      kSYMT_Function,
                                      returnType,
                                      pBlock,
                                      pParseData->pRootAst,
                                      pParseData);

    return pAst;
}

Ast * ast_create_entity_def(const char * name, Ast * pBlock, ParseData * pParseData)
{
    Ast * pAst = ast_create_block_def(name,
                                      kAST_EntityDef,
                                      kSYMT_Entity,
                                      kDT_Undefined,
                                      pBlock,
                                      pParseData->pRootAst,
                                      pParseData);
    pAst->pBlockInfos = block_pack_props_and_fields(pAst);
    return pAst;
}

Ast * ast_create_component_def(const char * name, Ast * pBlock, ParseData * pParseData)
{
    Ast * pAst = ast_create_block_def(name,
                                      kAST_ComponentDef,
                                      kSYMT_Component,
                                      kDT_Undefined,
                                      pBlock,
                                      pParseData->pRootAst,
                                      pParseData);
    pAst->pBlockInfos = block_pack_props_and_fields(pAst);
    return pAst;
}

Ast * ast_create_message_def(const char * name, Ast * pBlock, ParseData * pParseData)
{
    Ast * pAst = ast_create_block_def(name,
                                      kAST_MessageDef,
                                      kSYMT_Message,
                                      kDT_Undefined,
                                      pBlock,
                                      NULL,
                                      pParseData);
    return pAst;
}

Ast * ast_create_property_def(const char * name, DataType dataType, Ast * pInitVal, ParseData * pParseData)
{
    ASSERT(pParseData);

    Ast * pAst = ast_create(kAST_PropertyDef, pParseData);
    pAst->pSymRec = symrec_create(kSYMT_Property,
                                  dataType,
                                  name,
                                  pInitVal);

    Scope * pScope = pParseData->scopeStack.back();
    symtab_add_symbol(pScope->pSymTab, pAst->pSymRec, pParseData);

    return pAst;
}

Ast * ast_create_field_def(const char * name, DataType dataType, Ast * pInitVal, ParseData * pParseData)
{
    ASSERT(pParseData);

    Ast * pAst = ast_create(kAST_FieldDef, pParseData);
    pAst->pSymRec = symrec_create(kSYMT_Field,
                                  dataType,
                                  name,
                                  pInitVal);

    Scope * pScope = pParseData->scopeStack.back();
    symtab_add_symbol(pScope->pSymTab, pAst->pSymRec, pParseData);

    return pAst;
}

Ast * ast_create_simple_stmt(Ast * pExpr, ParseData * pParseData)
{
    ASSERT(pParseData);

    Ast * pAst = ast_create(kAST_SimpleStmt, pParseData);
    pAst->pLhs = pExpr;

    return pAst;
}

Ast * ast_create_unary_op(AstType astType, Ast * pRhs, ParseData * pParseData)
{
    Ast * pAst = ast_create(astType, pParseData);

    ast_set_rhs(pAst, pRhs);

    return pAst;
}

Ast * ast_create_binary_op(AstType astType, Ast * pLhs, Ast * pRhs, ParseData * pParseData)
{
    Ast * pAst = ast_create(astType, pParseData);

    ast_set_lhs(pAst, pLhs);
    ast_set_rhs(pAst, pRhs);

    return pAst;
}

Ast * ast_create_assign_op(AstType astType, const char * name, Ast * pRhs, ParseData * pParseData)
{
    SymRec * pSymRec = parsedata_find_symbol(pParseData, name);

    Ast * pAst = ast_create(astType, pParseData);

    if (!pSymRec)
    {
        COMP_ERROR("Unknown symbol reference in assignment: %s", name);
        return pAst;
    }

    if (pSymRec->type != kSYMT_Param &&
        pSymRec->type != kSYMT_Local &&
        pSymRec->type != kSYMT_Field)
    {
        COMP_ERROR("Invalid use of symbol in assignment: %s", name);
        return pAst;
    }   

    pAst->pSymRec = pSymRec;
    ast_set_rhs(pAst, pRhs);
    return pAst;
}

Ast * ast_create_int_literal(int numi, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_IntLiteral, pParseData);
    pAst->numi = numi;
    return pAst;
}

Ast * ast_create_float_literal(float numf, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_FloatLiteral, pParseData);
    pAst->numf = numf;
    return pAst;
}

Ast * ast_create_function_call(const char * name, Ast * pParams, ParseData * pParseData)
{
    SymRec * pSymRec = parsedata_find_symbol(pParseData, name);

    Ast * pAst = ast_create(kAST_FunctionCall, pParseData);

    if (!pSymRec)
    {
        COMP_ERROR("Unknown symbol reference: %s", name);
        return pAst;
    }

    if (pSymRec->type != kSYMT_Function)
    {
        COMP_ERROR("Call to non-function symbol: %s", name);
        return pAst;
    }   

    pAst->pSymRec = pSymRec;
    ast_set_rhs(pAst, pParams);
   
    return pAst;
}

Ast * ast_create_symbol_ref(const char * name, ParseData * pParseData)
{
    SymRec * pSymRec = parsedata_find_symbol(pParseData, name);

    Ast * pAst = ast_create(kAST_SymbolRef, pParseData);

    if (!pSymRec)
    {
        COMP_ERROR("Unknown symbol reference: %s", name);
        return pAst;
    }

    if (pSymRec->type != kSYMT_Param &&
        pSymRec->type != kSYMT_Local &&
        pSymRec->type != kSYMT_Field &&
        pSymRec->type != kSYMT_Property)
    {
        COMP_ERROR("Invalid use of symbol: %s", name);
        return pAst;
    }   

    pAst->pSymRec = pSymRec;
    
    return pAst;
}

Ast * ast_create_if(Ast * pCondition, Ast * pIfBody, Ast * pElseBody, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_If, pParseData);

    ast_set_lhs(pAst, pCondition);
    ast_set_mid(pAst, pIfBody);
    ast_set_rhs(pAst, pElseBody);
    
    // We pushed scopes in the lexer
    if (pElseBody && pElseBody->type != kAST_Block)
        parsedata_pop_scope(pParseData);

    if (pIfBody && pIfBody->type != kAST_Block)
        parsedata_pop_scope(pParseData);
    
    return pAst;
}

Ast * ast_create_while(Ast * pCondition, Ast * pBody, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_While, pParseData);

    ast_set_lhs(pAst, pCondition);

    if (pBody->type == kAST_Block)
    {
        ast_add_children(pAst, pBody->pChildren);
        pAst->pScope = pBody->pScope;
    }
    else
    {
        ast_add_child(pAst, pBody);
        pAst->pScope = parsedata_pop_scope(pParseData); // wasn't popped with '}'
    }
    
    return pAst;
}

Ast * ast_create_dowhile(Ast * pCondition, Ast * pBody, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_DoWhile, pParseData);

    ast_set_lhs(pAst, pCondition);

    // The "while" keyword pushes a stack in the lexer.
    // In the case of a "do while", we don't want this
    // stack, so merge its contents with the parent stack
    // and discard it.
    Scope * pWhileScope = parsedata_pop_scope(pParseData);
    pParseData->skipNextScope = false;
    ASSERT(pWhileScope->pAstList->nodes.size() == 0); // while scope should never contain children

    if (pBody->type == kAST_Block)
    {
        ast_add_children(pAst, pBody->pChildren);
        pAst->pScope = pBody->pScope;
    }
    else
    {
        ast_add_child(pAst, pBody);
        pAst->pScope = parsedata_pop_scope(pParseData); // wasn't popped with '}'
    }

    // And now merge the scope created with the "while" keyword
    symtab_transfer(pAst->pScope->pSymTab, pWhileScope->pSymTab, pParseData);
    
    return pAst;
}

Ast * ast_create_for(Ast * pInit, Ast * pCondition, Ast * pUpdate, Ast * pBody, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_For, pParseData);

    ast_set_lhs(pAst, pInit);
    ast_set_mid(pAst, pCondition);
    ast_set_rhs(pAst, pUpdate);

    if (pBody->type == kAST_Block)
    {
        ast_add_children(pAst, pBody->pChildren);
        pAst->pScope = pBody->pScope;
    }
    else
    {
        ast_add_child(pAst, pBody);
        pAst->pScope = parsedata_pop_scope(pParseData); // wasn't popped with '}'
    }
    
    return pAst;
}

Ast * ast_create_block(Ast* pBlock, ParseData * pParseData)
{
    if (!pBlock)
    {
        pBlock = ast_create(kAST_Block, pParseData);
        pBlock->pChildren = astlist_create();
    }

    pBlock->pScope = parsedata_pop_scope(pParseData);

    return pBlock;
}

Ast * ast_create_identifier(const char * name, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_Identifier, pParseData);
    pAst->str = name;
    return pAst;
}

Ast * ast_create_property_set(Ast *pTarget, Ast *pComponent, const char * propertyStr, Ast *pRhs, ParseData *pParseData)
{
    Ast * pAst = ast_create(kAST_PropertySet, pParseData);
    pAst->str = propertyStr;
    ast_set_lhs(pAst, pTarget);
    ast_set_mid(pAst, pComponent);
    ast_set_rhs(pAst, pRhs);

    return pAst;
}

Ast * ast_create_message_send(Ast *pTarget, Ast *pComponent, const char * messageStr, Ast *pParams, ParseData *pParseData)
{
    Ast * pAst = ast_create(kAST_MessageSend, pParseData);
    pAst->str = messageStr;
    ast_set_lhs(pAst, pTarget);
    ast_set_mid(pAst, pComponent);
    ast_set_rhs(pAst, pParams);

    pAst->pBlockInfos = block_pack_message_params(pParams);

    return pAst;
}

Ast * ast_append(AstType astType, Ast * pAst, Ast * pAstNew, ParseData * pParseData)
{
    if (!pAst)
    {
        pAst = ast_create(astType, pParseData);
    }
    pAst->pChildren = astlist_append(pAst->pChildren, pAstNew);
    return pAst;
}

Ast * ast_add_child(Ast * pParent, Ast * pChild)
{
    ASSERT(pParent);
    ASSERT(pChild);
    pParent->pChildren = astlist_append(pParent->pChildren, pChild);
    pChild->pParent = pParent;
    return pParent;
}

Ast * ast_add_children(Ast * pParent, AstList * pChildren)
{
    ASSERT(pParent);
    ASSERT(pChildren);
    ASSERT(pParent->pChildren == nullptr);

    for (Ast * pAstIt : pChildren->nodes)
    {
        pAstIt->pParent = pParent;
    }
    pParent->pChildren = pChildren;
    return pParent;
}

void ast_set_lhs(Ast * pParent, Ast * pLhs)
{
    pParent->pLhs = pLhs;
    if (pLhs)
        pLhs->pParent = pParent;
}

void ast_set_mid(Ast * pParent, Ast * pMid)
{
    pParent->pMid = pMid;
    if (pMid)
        pMid->pParent = pParent;
}

void ast_set_rhs(Ast * pParent, Ast * pRhs)
{
    pParent->pRhs = pRhs;
    if (pRhs)
        pRhs->pParent = pParent;    
}

DataType ast_data_type(const Ast * pAst)
{
    if (pAst->pSymRec)
        return pAst->pSymRec->dataType;
    PANIC("Cannot determine datatype for pAst, type: %d", pAst->type);
    return kDT_Undefined;
}

//------------------------------------------------------------------------------
// Ast (END)
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Scope
//------------------------------------------------------------------------------
Scope * scope_create()
{
    Scope * pScope = COMP_NEW(Scope);
    pScope->pAstList = astlist_create();
    pScope->pSymTab = symtab_create();

    return pScope;
}
//------------------------------------------------------------------------------
// Scope (END)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ParseData
//------------------------------------------------------------------------------
ParseData * parsedata_create(const char * fullPath,
                             MessageHandler messageHandler)
{
    ParseData * pParseData = COMP_NEW(ParseData);

    pParseData->skipNextScope = false;
    pParseData->hasErrors = false;
    pParseData->pScanner = nullptr;

    pParseData->fullPath = gaen::full_path(fullPath, pParseData);
    pParseData->filename = gaen::path_filename(pParseData->fullPath, pParseData);
    
    pParseData->messageHandler = messageHandler;

    pParseData->pRootScope = scope_create();
    pParseData->scopeStack.push_back(pParseData->pRootScope);

    pParseData->pRootAst = ast_create(kAST_Root, pParseData);

    pParseData->pRootScope->pSymTab->pAst = pParseData->pRootAst;

    return pParseData;
}

void parsedata_destroy(ParseData * pParseData)
{
    ASSERT(pParseData);
    if (pParseData->pScanner)
    {
        yylex_destroy(pParseData->pScanner);
    }
}

void * parsedata_scanner(ParseData * pParseData)
{
    ASSERT(pParseData);
    return pParseData->pScanner;
}

SymTab* parsedata_add_param(ParseData * pParseData, SymTab* pSymTab, SymRec * pSymRec)
{
    ASSERT(pParseData);
    if (pSymTab == nullptr)
    {
        Scope * pScope = parsedata_push_stmt_scope(pParseData);
        pSymTab = pScope->pSymTab;
    }
    if (pSymRec)
    {
        symtab_add_symbol(pSymTab, pSymRec, pParseData);
    }
    return pSymTab;
}

SymRec* parsedata_find_symbol(ParseData * pParseData, const char * name)
{
    ASSERT(pParseData);
    ASSERT(name);
    return symtab_find_symbol_recursive(pParseData->scopeStack.back()->pSymTab, name);
}

Ast* parsedata_add_local_symbol(ParseData * pParseData, SymRec * pSymRec)
{
    ASSERT(pParseData);
    ASSERT(pParseData->scopeStack.size() >= 1);
    ASSERT(pSymRec);

    Ast * pAst = ast_create(kAST_SymbolDecl, pParseData);
    pAst->pSymRec = pSymRec;

    Scope * pScope = pParseData->scopeStack.back();

    symtab_add_symbol(pScope->pSymTab, pSymRec, pParseData);
    return pAst;
}

Scope* parsedata_current_scope(ParseData * pParseData)
{
    ASSERT(pParseData);
    ASSERT(pParseData->scopeStack.size() >= 1);

    return pParseData->scopeStack.back();
}


Scope* parsedata_push_scope(ParseData * pParseData)
{
    ASSERT(pParseData->scopeStack.size() >= 1);

    Scope * pScope = nullptr;
    if (!pParseData->skipNextScope)
    {
        pScope = scope_create();

        // make pParseData a child of the top of the stack
        pScope->pSymTab->pParent = pParseData->scopeStack.back()->pSymTab;
        pScope->pSymTab->pParent->children.push_back(pScope->pSymTab);

        pParseData->scopeStack.push_back(pScope);
    }
    else
    {
        pScope = parsedata_current_scope(pParseData);
    }

    pParseData->skipNextScope = false;
    return pScope;
}

Scope* parsedata_push_stmt_scope(ParseData * pParseData)
{
    ASSERT(!pParseData->skipNextScope);
    Scope * pScope = parsedata_push_scope(pParseData);
    pParseData->skipNextScope = true;
    return pScope;
}

Scope * parsedata_pop_scope(ParseData * pParseData)
{
    ASSERT(pParseData->scopeStack.size() >= 1);

    Scope * pScope = pParseData->scopeStack.back();
    if (pParseData->scopeStack.size() > 1)
    {
        pParseData->scopeStack.pop_back();
    }
    else
    {
        COMP_ERROR("No more scopes to pop");
    }

    pParseData->skipNextScope = false;

    return pScope;
}

void parsedata_handle_do_scope(ParseData * pParseData)
{
    pParseData->skipNextScope = false;
}

const char * parsedata_add_string(ParseData * pParseData, const char * str)
{
    ASSERT(pParseData);
    return pParseData->strings.emplace(str).first->c_str();
}

void parsedata_formatted_message(ParseData * pParseData,
                                 MessageType messageType,
                                 const char * format, ...)
{
    static const size_t kMessageMax = 1024;
    static thread_local char tMessage[kMessageMax];

    pParseData->hasErrors = true;

    va_list argptr;
    va_start(argptr, format);

    if (format && format[0] != '\0')
    {
        int ret = vsnprintf(tMessage, kMessageMax-1, format, argptr);

        if (ret <= 0)
        {
            strncpy(tMessage, "yyerror was unable to format message", kMessageMax-1);
        }
    }
    else
        tMessage[0] = '\0';

    va_end(argptr);

    tMessage[kMessageMax-1] = '\0';

    pParseData->messageHandler(messageType,
                               tMessage,
                               pParseData->filename,
                               pParseData->line,
                               pParseData->column);
}

void parsedata_set_location(ParseData * pParseData,
                            int line,
                            int column)
{
    pParseData->line = line;
    pParseData->column = column;
}

//------------------------------------------------------------------------------
// ParseData (END)
//------------------------------------------------------------------------------

void parse_init()
{
    comp_reset_mem();

    //yydebug = 1;
}

ParseData * parse(ParseData * pParseData,
                  const char * source,
                  size_t length,
                  const char * fullPath,
                  MessageHandler messageHandler)
{
    int ret;

    if (!pParseData)
        pParseData = parsedata_create(fullPath, messageHandler);

    if (!source)
    {
        char * newSource = nullptr;
        length = read_file(pParseData->fullPath, &newSource);
        if (length <= 0)
        {
            PANIC("Unable to read file");
            return nullptr;
        }
        source = newSource;
    }

    ret = yylex_init_extra(pParseData, &pParseData->pScanner);
    if (ret != 0)
    {
        return nullptr;
    }

    yy_scan_bytes(source, length, pParseData->pScanner);

    ret = yyparse(pParseData);
    if (ret != 0)
    {
        return nullptr;
    }

    return pParseData;
}   

ParseData * parse_file(ParseData * pParseData,
                       const char * fullPath,
                       MessageHandler messageHandler)
{
    return parse(nullptr, nullptr, 0, fullPath, messageHandler);
}

void yyerror(YYLTYPE * pLoc, ParseData * pParseData, const char * format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    parsedata_formatted_message(pParseData, kMSGT_Error, format, argptr);
    va_end(argptr);
}


void *yyalloc(yy_size_t size, yyscan_t yyscanner)
{
	return COMP_ALLOC(size);
}

void *yyrealloc(void * ptr, yy_size_t size, yyscan_t yyscanner)
{
	return COMP_REALLOC(ptr, size);
}

void yyfree (void * ptr , yyscan_t yyscanner)
{
    // no freeing with comp_mem.h manager
}
