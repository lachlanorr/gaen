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
#include "engine/system_api.h"

#include "compose/compiler.h"
#include "compose/compiler_structs.h"
#include "compose/codegen_utils.h"
#include "compose/utils.h"
#include "compose/system_api_meta.h"

extern "C" {
#define YYDEBUG 1
#include "compose/compose_parser.h"
#define YY_NO_UNISTD_H
#include "compose/compose_scanner.h"
}

using namespace gaen;

static char sEmptyStr[] = { '\0' };

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
                       Ast * pDataType,
                       const char * name,
                       Ast * pAst)
{
    SymRec * pSymRec = COMP_NEW(SymRec);

    if (pDataType && pDataType->type != kAST_DataType)
    {
        COMP_ERROR(pAst->pParseData, "Custom Types Not Supported Yet");
        return nullptr;
    }
    DataType dataType = pDataType ? (DataType)pDataType->numi : kDT_Undefined;

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
        COMP_ERROR(pParseData, "Symbol already defined: %s", pSymRec->name);
        return pSymTab;
    }

    pSymRec->pSymTab = pSymTab;
    pSymRec->order = static_cast<u32>(pSymTab->dict.size());
    pSymTab->dict[pSymRec->name] = pSymRec;
    pSymTab->orderedSymRecs.push_back(pSymRec);
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

    pAst->pParseData = pParseData;
    pAst->type = astType;
    pAst->flags = kASTF_None;
    pAst->pParent = nullptr;
    pAst->pScope = parsedata_current_scope(pParseData);
    pAst->pSymRec = nullptr;

    pAst->pLhs = nullptr;
    pAst->pMid = nullptr;
    pAst->pRhs = nullptr;

    pAst->numi = 0;
    pAst->numf = 0;
    pAst->str = sEmptyStr;

    pAst->pChildren = nullptr;
    pAst->fullPath = pParseData->fullPath;

    pAst->pBlockInfos = 0;

    return pAst;
}

Ast * ast_create_with_child_list(AstType astType, ParseData * pParseData)
{
    Ast * pAst = ast_create(astType, pParseData);
    pAst->pChildren = astlist_create(); 
    return pAst;
}

Ast * ast_create_with_str(AstType astType, AstFlags flags, const char * str, ParseData * pParseData)
{
    Ast * pAst = ast_create(astType, pParseData);
    pAst->flags = flags;
    pAst->str = str;
    return pAst;
}

Ast * ast_create_with_numi(AstType astType, AstFlags flags, int numi, ParseData * pParseData)
{
    Ast * pAst = ast_create(astType, pParseData);
    pAst->flags = flags;
    pAst->numi = numi;
    return pAst;
}

Ast * ast_create_dotted_id(Ast * pItems, ParseData * pParseData)
{
    ASSERT(pItems->type == kAST_DottedId);
    ASSERT(pItems->pChildren);
    ASSERT(pItems->pChildren->nodes.size() > 0);

    // Add the reconstituted name as str member
    
    size_t len = 0;
    for (Ast * pAst : pItems->pChildren->nodes)
    {
        ASSERT(pAst->str);
        len += strlen(pAst->str) + 1; // +1 for dot, or null on last item
    }

    pItems->str = (char*)COMP_ALLOC(len);

    char * pos = const_cast<char*>(pItems->str); // a rare instance of casting away const that I'm not going to hate myself over
    *pos = '\0';
    for (Ast * pAst : pItems->pChildren->nodes)
    {
        ASSERT(pAst->str);
        strcpy(pos, pAst->str);
        pos += strlen(pAst->str);
        strcpy(pos, ".");
        pos++;
    }
    pos--;
    *pos = '\0';

    return pItems;
}

static Ast * ast_create_block_def(const char * name,
                                  AstType astType,
                                  SymType symType,
                                  Ast * pReturnType,
                                  Ast * pBlock,
                                  Ast * pParent,
                                  ParseData * pParseData)
{
    ASSERT(pBlock);
    ASSERT(pBlock->pChildren);
    ASSERT(pParseData);

    Ast * pAst = ast_create(astType, pParseData);
    pAst->str = name;
    ast_add_children(pAst, pBlock->pChildren);
    pAst->pScope = pBlock->pScope;
    pAst->pScope->pSymTab->pAst = pAst;
    
    if (pReturnType && pReturnType->type != kAST_DataType)
    {
        COMP_ERROR(pParseData, "Custom Types Not Supported Yet");
        return nullptr;
    }
    pAst->pSymRec = symrec_create(symType,
                                  pReturnType,
                                  name,
                                  pAst);

    parsedata_add_local_symbol(pParseData, pAst->pSymRec);

    if (pParent)
        ast_add_child(pParent, pAst);

    return pAst;
}

void ast_create_import_list(Ast * pImportList, ParseData * pParseData)
{
    pParseData->pRootAst->pLhs = pImportList;
}

Ast * ast_create_import_stmt(Ast * pDottedId, ParseData * pParseData)
{
    ASSERT(pDottedId->str);
    
    Ast * pAst = ast_create(kAST_ImportStmt, pParseData);
    pAst->pRhs = pDottedId;

    const char * path = parsedata_dotted_to_path(pParseData, pDottedId->str);
    if (!path)
    {
        COMP_ERROR(pParseData, "Failed to find import: %s", pDottedId->str);
    }
    
    // Do the import
    parsedata_parse_import(pParseData, pDottedId->str, path);

    return pAst;
}

Ast * ast_create_function_def(const char * name, Ast * pReturnType, Ast * pBlock, ParseData * pParseData)
{
    Ast * pAst = ast_create_block_def(name,
                                      kAST_FunctionDef,
                                      kSYMT_Function,
                                      pReturnType,
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
                                      nullptr,
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
                                      nullptr,
                                      pBlock,
                                      pParseData->pRootAst,
                                      pParseData);
    pAst->pBlockInfos = block_pack_props_and_fields(pAst);
    return pAst;
}

Ast * ast_create_message_def(const char * name, SymTab * pSymTab, Ast * pBlock, ParseData * pParseData)
{
    Ast * pAst = ast_create_block_def(name,
                                      kAST_MessageDef,
                                      kSYMT_Message,
                                      nullptr,
                                      pBlock,
                                      NULL,
                                      pParseData);
    if (0 != strcmp(name, "update"))
    {
        // Update messages are special cased to run as a direct
        // function call on the task.  Other messages are handled in
        // the message routine, and need block info computed to
        // reference their "parameters".
        pAst->pBlockInfos = block_pack_message_def_params(pSymTab, pParseData);
    }
    return pAst;
}

Ast * ast_create_property_def(const char * name, Ast * pDataType, Ast * pInitVal, ParseData * pParseData)
{
    ASSERT(pParseData);

    Ast * pAst = ast_create(kAST_PropertyDef, pParseData);

    if (pDataType->type != kAST_DataType)
    {
        COMP_ERROR(pParseData, "Custom Types Not Supported Yet");
        return nullptr;
    }
    pAst->pSymRec = symrec_create(kSYMT_Property,
                                  pDataType,
                                  name,
                                  pInitVal);

    Scope * pScope = pParseData->scopeStack.back();
    symtab_add_symbol(pScope->pSymTab, pAst->pSymRec, pParseData);

    return pAst;
}

Ast * ast_create_field_def(const char * name, Ast * pDataType, Ast * pInitVal, ParseData * pParseData)
{
    ASSERT(pParseData);

    Ast * pAst = ast_create(kAST_FieldDef, pParseData);

    if (pDataType->type != kAST_DataType)
    {
        COMP_ERROR(pParseData, "Custom Types Not Supported Yet");
        return nullptr;
    }
    pAst->pSymRec = symrec_create(kSYMT_Field,
                                  pDataType,
                                  name,
                                  pInitVal);

    Scope * pScope = pParseData->scopeStack.back();
    symtab_add_symbol(pScope->pSymTab, pAst->pSymRec, pParseData);

    return pAst;
}

Ast * ast_create_component_members(Ast * pAst, ParseData * pParseData)
{
    parsedata_pop_scope(pParseData);
    return pAst;
}

Ast * ast_create_component_member(const char * name, Ast * pPropInitList, ParseData * pParseData)
{
    SymRec* pCompSymRec = symtab_find_symbol_recursive(pPropInitList->pScope->pSymTab, name);

    if (!pCompSymRec)
    {
        COMP_ERROR(pParseData, "Unknown component: %s, are you missing an import?", name);
        return nullptr;
    }

    Ast * pAst = ast_create(kAST_ComponentMember, pParseData);
    pAst->str = name;
    ast_set_rhs(pAst, pPropInitList);
    return pAst;
}

Ast * ast_create_prop_init(const char * name, Ast * pVal, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_PropInit, pParseData);
    pAst->str = name;
    ast_set_rhs(pAst, pVal);
    return pAst;
}

Ast * ast_create_custom_type(AstFlags flags, Ast * pTypeInfo, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_CustomType, pParseData);
    pAst->flags = flags;
    ast_set_lhs(pAst, pTypeInfo);
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

Ast * ast_create_hash(const char * name, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_Hash, pParseData);
    pAst->str = name;
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
    Ast * pAst = ast_create(astType, pParseData);

    SymRec * pSymRec = parsedata_find_symbol(pParseData, name);
    // We may not have a pSymRec at this point, e.g. entity
    // initialization syntax In those cases, we don't error here when
    // we don't find the symbol, but will have to handle symbol lookup
    // more explicitly further down the pipe.
    if (pSymRec)
    {
        if (pSymRec->type != kSYMT_Param &&
            pSymRec->type != kSYMT_Local &&
            pSymRec->type != kSYMT_Field)
        {
            COMP_ERROR(pParseData, "Invalid use of symbol in assignment: %s", name);
            return pAst;
        }

        pAst->pSymRec = pSymRec;
    }

    ast_set_rhs(pAst, pRhs);
    return pAst;
}

Ast * ast_create_color_init(Ast * pParams, ParseData * pParseData)
{
    // pop scope that got created by the lexer when encountering the '{'
    parsedata_pop_scope(pParseData);

    Ast * pAst = ast_create(kAST_ColorInit, pParseData);
    ast_set_rhs(pAst, pParams);

    switch (pParams->pChildren->nodes.size())
    {
    case 4:
        for (Ast * pParam : pParams->pChildren->nodes)
        {
            DataType paramDt = RAW_DT(ast_data_type(pParam));
            if (paramDt != kDT_int && paramDt != kDT_uint)
                COMP_ERROR(pParseData, "Invalid data type in color initialization");
        }
        break;
    default:
        COMP_ERROR(pParseData, "Invalid parameters for color initialization");
        break;
    }

    return pAst;
}

Ast * ast_create_vec3_init(Ast * pParams, ParseData * pParseData)
{
    // pop scope that got created by the lexer when encountering the '{'
    parsedata_pop_scope(pParseData);

    Ast * pAst = ast_create(kAST_Vec3Init, pParseData);
    ast_set_rhs(pAst, pParams);

    switch (pParams->pChildren->nodes.size())
    {
    case 1:
    {
        Ast * pParam = pParams->pChildren->nodes.front();
        DataType paramDt = RAW_DT(ast_data_type(pParam));
        if (paramDt != kDT_float && paramDt != kDT_vec3)
            COMP_ERROR(pParseData, "Invalid data type in vec3 initialization");
        break;
    }
    case 3:
    {
        for (Ast * pParam : pParams->pChildren->nodes)
        {
            DataType paramDt = RAW_DT(ast_data_type(pParam));
            if (paramDt != kDT_float)
                COMP_ERROR(pParseData, "Invalid data type in vec3 initialization");
        }
        break;
    }
    default:
        COMP_ERROR(pParseData, "Invalid parameters for vec3 initialization");
        break;
    }

    return pAst;
}

Ast * ast_create_mat34_init(Ast * pParams, ParseData * pParseData)
{
    // pop scope that got created by the lexer when encountering the '{'
    parsedata_pop_scope(pParseData);

    Ast * pAst = ast_create(kAST_Mat34Init, pParseData);
    ast_set_rhs(pAst, pParams);

    switch (pParams->pChildren->nodes.size())
    {
    case 1:
    {
        Ast * pParam = pParams->pChildren->nodes.front();
        DataType paramDt = RAW_DT(ast_data_type(pParam));
        if (paramDt != kDT_float && paramDt != kDT_mat34)
            COMP_ERROR(pParseData, "Invalid data type in mat34 initialization");
        break;
    }
    case 12:
    {
        for (Ast * pParam : pParams->pChildren->nodes)
        {
            DataType paramDt = RAW_DT(ast_data_type(pParam));
            if (paramDt != kDT_float)
                COMP_ERROR(pParseData, "Invalid data type in mat34 initialization");
        }
        break;
    }
    default:
        COMP_ERROR(pParseData, "Invalid parameters for mat34 initialization");
        break;
    }

    return pAst;
}

Ast * ast_create_entity_or_struct_init(Ast * pDottedId, Ast * pParams, ParseData * pParseData)
{
    // LORRTEMP
    return nullptr;

    SymRec * pTypeSymRec = parsedata_find_symbol(pParseData, pDottedId->str);

    Ast * pAst = ast_create(kAST_EntityInit, pParseData);

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

Ast * ast_create_function_call(Ast * pDottedId, Ast * pParams, ParseData * pParseData)
{
    Ast * pAst = nullptr;

    SymRec * pSymRec = parsedata_find_symbol(pParseData, pDottedId->str);

    if (pSymRec)
    {
        pAst = ast_create(kAST_FunctionCall, pParseData);

        if (pSymRec->type != kSYMT_Function)
        {
            COMP_ERROR(pParseData, "Call to non-function symbol: %s", "NOT_IMPLEMENTED__NEED_TO_PRINT_pDottedId as string");
        }
        else
        {
            pAst->pSymRec = pSymRec;
            ast_set_rhs(pAst, pParams);
        }
    }
    else
    {
        COMP_ERROR(pParseData, "Unknown function: %s", "NOT_IMPLEMENTED__NEED_TO_PRINT_pDottedId as string");
        return nullptr;
    }

    ASSERT(pAst);
    return pAst;
}

Ast * ast_create_system_api_call(const char * pApiName, Ast * pParams, ParseData * pParseData)
{

    // check to see if this is a syscall
    const ApiSignature * pSig = find_api(pApiName, pParseData);
    if (pSig)
    {
        Ast * pAst = ast_create(kAST_SystemCall, pParseData);
        pAst->str = pApiName;
        ast_set_rhs(pAst, pParams);
        return pAst;
    }
    else
    {
        COMP_ERROR(pParseData, "Unknown system api reference: %s", pApiName);
        return nullptr;
    }

}

Ast * ast_create_symbol_ref(const char * name, ParseData * pParseData)
{
    SymRec * pSymRec = parsedata_find_symbol(pParseData, name);

    Ast * pAst = ast_create(kAST_SymbolRef, pParseData);

    if (!pSymRec)
    {
        COMP_ERROR(pParseData, "Unknown symbol reference: %s", name);
        return pAst;
    }

    if (pSymRec->type != kSYMT_Param &&
        pSymRec->type != kSYMT_Local &&
        pSymRec->type != kSYMT_Field &&
        pSymRec->type != kSYMT_Property)
    {
        COMP_ERROR(pParseData, "Invalid use of symbol: %s", name);
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

Ast * ast_create_property_set(Ast *pTarget, const char * propertyStr, Ast *pRhs, ParseData *pParseData)
{
    Ast * pAst = ast_create(kAST_PropertySet, pParseData);
    pAst->str = propertyStr;
    ast_set_lhs(pAst, pTarget);
    ast_set_rhs(pAst, pRhs);

    return pAst;
}

Ast * ast_create_message_send(Ast *pTarget, const char * messageStr, Ast *pParams, ParseData *pParseData)
{
    Ast * pAst = ast_create(kAST_MessageSend, pParseData);
    pAst->str = messageStr;
    ast_set_lhs(pAst, pTarget);
    ast_set_rhs(pAst, pParams);

    pAst->pBlockInfos = block_pack_message_params(pAst);

    return pAst;
}

Ast * ast_create_return(Ast *pExpr, ParseData *pParseData)
{
    Ast * pAst = ast_create(kAST_Return, pParseData);
    ast_set_rhs(pAst, pExpr);
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
    else if (pAst->type == kAST_Hash)
        return kDT_uint;
    else if (pAst->type == kAST_FloatLiteral)
        return kDT_float;
    else if (pAst->type == kAST_IntLiteral)
        return kDT_int;
    else if (pAst->type == kAST_ColorInit)
        return kDT_color;
    else if (pAst->type == kAST_Vec2Init)
        return kDT_vec2;
    else if (pAst->type == kAST_Vec3Init)
        return kDT_vec3;
    else if (pAst->type == kAST_Vec4Init)
        return kDT_vec4;
    else if (pAst->type == kAST_Mat34Init)
        return kDT_mat34;
    else if (pAst->type == kAST_Negate)
        return ast_data_type(pAst->pRhs);
    else if (pAst->type == kAST_SystemCall)
    {
        const ApiSignature * pSig = find_api(pAst->str, pAst->pParseData);
        return pSig->returnType;
    }
    COMP_ERROR(pAst->pParseData, "Cannot determine datatype for pAst, type: %d", pAst->type);
    return kDT_Undefined;
}

int are_types_compatible(DataType a, DataType b)
{
    // TODO: Support const checking in assignment. This function should be
    // renamed or special cased to handle checks on LHS of assignment consts.
    // We also need to support implicit conversion from int to float, etc.
    DataType rawA = RAW_DT(a);
    DataType rawB = RAW_DT(b);
    if (rawA == rawB)
        return 1;
    if (rawA == kDT_uint && rawB == kDT_int || 
        rawA == kDT_int && rawB == kDT_uint)
        return 1;
    return 0;
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

    pParseData->messageHandler = messageHandler;

    pParseData->pRootScope = scope_create();
    pParseData->scopeStack.push_back(pParseData->pRootScope);

    pParseData->pRootAst = ast_create(kAST_Root, pParseData);

    pParseData->pRootScope->pSymTab->pAst = pParseData->pRootAst;

    parsedata_prep_paths(pParseData, fullPath);

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

const char * parsedata_dotted_to_path(ParseData * pParseData, const char * dottedId)
{
    ASSERT(dottedId);
    ASSERT(pParseData->scriptsRootPath);
    ASSERT(pParseData->scriptsRootPathLen == strlen(pParseData->scriptsRootPath));

    size_t dottedIdLen = strlen(dottedId);
    size_t pathLen = pParseData->scriptsRootPathLen + dottedIdLen + 4 + 1; // +4 for ".cmp", +1 for null
    char * path = (char*)COMP_ALLOC(pathLen);

    strcpy(path, pParseData->scriptsRootPath);
    strcat(path, dottedId);

    // replace dots with slashes
    char * dotted = path + pParseData->scriptsRootPathLen;
    for (int i = 0; i < dottedIdLen; ++i)
    {
        if (dotted[i] == '.')
            dotted[i] = '/';
    }

    strcat(path, ".cmp");

    return path;
}

void parsedata_prep_paths(ParseData * pParseData, const char * fullPath)
{
    pParseData->fullPath = gaen::full_path(fullPath, pParseData);
    pParseData->filename = gaen::path_filename(pParseData->fullPath, pParseData);

    static const char * kScriptsPath = "/scripts/cmp/";

    const char * pLoc = strstr(pParseData->fullPath, kScriptsPath);
    if (!pLoc)
    {
        COMP_ERROR(pParseData, "File doesn't appear to be in a scripts directory. By convention, all compose scripts must be within a .../scripts/cmp/... sub directory. File: %s", pParseData->fullPath);
        return;
    }

    size_t fullPathLen = strlen(pParseData->fullPath);
    size_t scriptsRootLen = (pLoc - pParseData->fullPath) + strlen(kScriptsPath);

    char * scriptsRootPath = (char*)COMP_ALLOC(scriptsRootLen+1);
    strncpy(scriptsRootPath, pParseData->fullPath, scriptsRootLen);
    scriptsRootPath[scriptsRootLen] = '\0';
    pParseData->scriptsRootPathLen = scriptsRootLen;
    pParseData->scriptsRootPath = scriptsRootPath;
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

Scope* parsedata_push_top_level_stmt_scope(ParseData * pParseData)
{
    if (pParseData->scopeStack.size() == 1)
    {
        return parsedata_push_stmt_scope(pParseData);
    }
    return nullptr;
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
        COMP_ERROR(pParseData, "No more scopes to pop");
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
    ASSERT(pParseData);
    
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

void parsedata_parse_import(ParseData * pParseData,
                            const char * namespace_,
                            const char * fullPath)
{
    ParseData * pImportParseData = parse_file(fullPath, pParseData->messageHandler);

    if (!pImportParseData)
    {
        COMP_ERROR(pParseData, "Failed to parse import: %s", fullPath);
        return;
    }
    
    Import imp;
    imp.pParseData = pImportParseData;
    if (!namespace_ || namespace_[0] == '\0')
    {
        imp.namespace_ = "";
    }
    else
    {
        char * pref = (char*)COMP_ALLOC(strlen(namespace_) + 1 + 1); // +1 for '.', +1 for null
        strcpy(pref, namespace_);
        strcat(pref, ".");
        imp.namespace_ = pref;
    }
        
    pParseData->imports.push_back(imp);
}

//------------------------------------------------------------------------------
// ParseData (END)
//------------------------------------------------------------------------------

void parse_init()
{
    comp_reset_mem();

    //yydebug = 1;
}

ParseData * parse(const char * source,
                  size_t length,
                  const char * fullPath,
                  MessageHandler messageHandler)
{
    int ret;

    ParseData * pParseData = parsedata_create(fullPath, messageHandler);

    if (!source)
    {
        char * newSource = nullptr;
        length = read_file(fullPath, &newSource);
        if (length <= 0)
        {
            COMP_ERROR(pParseData, "Unable to read file");
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

ParseData * parse_file(const char * fullPath,
                       MessageHandler messageHandler)
{
    return parse(nullptr, 0, fullPath, messageHandler);
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
