//------------------------------------------------------------------------------
// compiler.cpp - Compose script compiler
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#include "core/hashing.h"
#include "core/thread_local.h"
#include "engine/system_api.h"

#include "compose/compiler.h"
#include "compose/comp_string.h"
#include "compose/compiler_structs.h"
#include "compose/codegen_utils.h"
#include "compose/utils.h"

extern "C" {
// LORRNOTE: Uncomment below to enable stderr based debug messages from parser
#define YYDEBUG 1
#include "compose/compose_parser.h"
#define YY_NO_UNISTD_H
#include "compose/compose_scanner.h"
}

using namespace gaen;

static_assert(kDT_COUNT == 26, "Make sure DataType enum ids look right... seems they have changed");

static const char * kScriptsPath = "/src/scripts/cmp/";

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

const char * parse_identifier(const char * str, ParseData * pParseData)
{
    if (!str || !str[0])
    {
        COMP_ERROR(pParseData, "Invalid empty identifier");
    }
    else if (str[0] == '_')
    {
        COMP_ERROR(pParseData, "Invalid idenfier with leading underscore: %s", str);
    }
    else
    {
        for (const char * c = str+1; *c; ++c)
        {
            if (*(c-1) == '_' && *c == '_')
                COMP_ERROR(pParseData, "Invalid identifier with double underscores: %s", str);
            if (*c == '_' && !*(c+1))
                COMP_ERROR(pParseData, "Invalid identifier with trailing underscore: %s", str);
        }
    }
    
    return parsedata_add_string(pParseData, str);
}

size_t mangle_function_len(const char * name, const AstList * pParamList)
{
    size_t len = 3; // "f__"
    if (pParamList)
    {
        for (const Ast * pParam : pParamList->nodes)
        {
            const SymDataType * pSdt = ast_data_type(pParam);
            len += strlen(pSdt->mangledParam) + 2; // mangled name + "__"
        }
    }
    len += strlen(name);
    return len;
}

void mangle_function(char * mangledName, i32 mangledNameSize, const char * name, const AstList * pParamList)
{
    ASSERT(mangledNameSize > mangle_function_len(name, pParamList));

    char * p = mangledName;

    strcpy(p, "f__");
    p += 3;

    if (pParamList)
    {
        for (const Ast * pParam : pParamList->nodes)
        {
            const SymDataType * pSdt = ast_data_type(pParam);

            u32 typeStrLen = (u32)strlen(pSdt->mangledParam);
            strcpy(p, pSdt->mangledParam);
            strcat(p, "__");
            p += typeStrLen + 2;
        }
    }

    strcpy(p, name);
}

const char * unmangle_function(const char * mangledName)
{
    const char * lastDoubleDash = strstr(mangledName, "__");

    if (lastDoubleDash)
        lastDoubleDash += 2;
    else
        return mangledName;

    while (const char * ldd = strstr(lastDoubleDash, "__"))
    {
        lastDoubleDash = ldd + 2;
    }

    return lastDoubleDash;
}

size_t mangle_type_len(const char * name)
{
    return strlen(name) + 5; // 3 for abbrev + 2 underscores
}

void mangle_type(char * mangledName, size_t mangledNameSize, const char * name, int isConst, int isReference)
{
    ASSERT(mangledNameSize > mangle_type_len(name));
    
    char * p = mangledName;

    strcpy(p, "t");
    p++;

    if (isConst)
        strcpy(p, "c");
    else
        strcpy(p, "n");
    p++;

    if (isReference)
        strcpy(p, "r");
    else
        strcpy(p, "v");
    p++;

    strcpy(p, "__");
    p += 2;
        
    strcpy(p, name);
}


size_t mangle_param_len(const char * name)
{
    return strlen(name) + 3; // 1 for abbrev + 2 underscores
}

void mangle_param(char * mangledName, size_t mangledNameSize, const char * name, int isConst, int isReference)
{
    ASSERT(mangledNameSize > mangle_param_len(name));

    char * p = mangledName;

    strcpy(p, "t");
    p++;

    strcpy(p, "__");
    p += 2;

    strcpy(p, name);
}



//------------------------------------------------------------------------------
// SymDataType
//------------------------------------------------------------------------------

static const char * cpp_type_str(const char * cppName, int isConst, int isReference, ParseData * pParseData)
{
    ASSERT(cppName);
    size_t typeLen = strlen(cppName);
    if (isConst)
        typeLen += 6; // "const " 6 chars
    if (isReference)
        typeLen += 2;

    char * ret = (char*)COMP_ALLOC(typeLen+1);
    ret[0] = '\0';
    if (isConst)
        strcat(ret, "const ");

    if (isReference)
        strcat(ret, "& ");

    strcat(ret, cppName);

    return ret;

}

SymDataType * symdatatype_create(DataType dataType,
                                 const char * name,
                                 const char * cppName,
                                 u32 cellCount,
                                 int isConst,
                                 int isReference,
                                 ParseData * pParseData)
{
    SymDataType * pSdt = COMP_NEW(SymDataType);

    pSdt->typeDesc.dataType = dataType;
    pSdt->typeDesc.isConst = isConst != 0;
    pSdt->typeDesc.isReference = isReference != 0;
    pSdt->cellCount = cellCount;

    pSdt->name = name;

    size_t mangledTypeSize = mangle_type_len(name) + 1;
    char * mangledType = (char*)COMP_ALLOC(mangledTypeSize);
    mangle_type(mangledType, mangledTypeSize, name, isConst, isReference);
    pSdt->mangledType = mangledType;

    size_t mangledParamSize = mangle_param_len(name) + 1;
    char * mangledParam = (char*)COMP_ALLOC(mangledParamSize);
    mangle_param(mangledParam, mangledParamSize, name, isConst, isReference);
    pSdt->mangledParam = mangledParam;


    pSdt->cppTypeStr = cpp_type_str(cppName, isConst, isReference, pParseData);

    return pSdt;
}

void symdatatype_add_field(SymDataType * pSdt, SymDataType * pFieldSdt, const char * fieldName)
{
    SymStructField * pField = (SymStructField*)COMP_ALLOC(sizeof(SymStructField));
    pField->pSymDataType = pFieldSdt;
    pField->name = fieldName;
    pSdt->fields.push_back(pField);
}

void symdatatype_add_field_related(RelatedTypes * pRelatedTypes, SymDataType * pFieldSdt, const char * fieldName)
{
    symdatatype_add_field(pRelatedTypes->pNormal, pFieldSdt, fieldName);
    symdatatype_add_field(pRelatedTypes->pConst, pFieldSdt, fieldName);
    symdatatype_add_field(pRelatedTypes->pReference, pFieldSdt, fieldName);
    symdatatype_add_field(pRelatedTypes->pConstReference, pFieldSdt, fieldName);
}

const SymStructField * symdatatype_find_field(const SymDataType * pSdt, const char * fieldName)
{
    for (SymStructField * pField : pSdt->fields)
    {
        if (0 == strcmp(pField->name, fieldName))
            return pField;
    }
    return nullptr;
}
//------------------------------------------------------------------------------
// SymDataType (END)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// SymRec
//------------------------------------------------------------------------------
SymRec * symrec_create(SymType symType,
                       const SymDataType * pSdt,
                       const char * name,
                       Ast * pAst,
                       ParseData * pParseData)
{
    SymRec * pSymRec = COMP_NEW(SymRec);

    pSymRec->type = symType;
    pSymRec->pSymDataType = pSdt;
    pSymRec->name = name;
    pSymRec->pAst = pAst;

    pSymRec->pSymTab = nullptr;

    // For components and entities, fill in the "internal" symtab
    // so we can easily track which properties are valid within.
    // This is necessary when components or entities are being
    // initialized with property values, we want to be able to
    // error on invalid property names.
    if (symType == kSYMT_Entity || 
        symType == kSYMT_Component)
    {
        ASSERT(pParseData->scopeStack.size() > 0);
        ASSERT(pParseData->scopeStack.back()->pSymTab->children.size() > 0);
        pSymRec->pSymTabInternal = pParseData->scopeStack.back()->pSymTab->children.back();
    }
    else
    {
        pSymRec->pSymTabInternal = nullptr;
    }

    pSymRec->order = 0;
    pSymRec->flags = 0;

    // For top level symbols, prepare the full_name, as it will
    // be used to register the symbol.
    switch (symType)
    {
    case kSYMT_Function:
    case kSYMT_Entity:
    case kSYMT_Component:
    case kSYMT_Type:
    {

        const char * ns = "";
        if (pAst && pAst->pParseData && pAst->pParseData->namespace_)
        {
            ns = pAst->pParseData->namespace_;
        }

        ASSERT(name);

        // count the dots, since we'll be changing them to __
        u32 dotCount = 0;
        const char * p = ns;
        for (const char * c = ns; *c; c++)
            if (*c == '.') 
                dotCount++;
        char * fname = (char*)COMP_ALLOC(strlen(ns) + strlen(name) + dotCount + 1);

        // replace all '.' with "__"
        p = ns;
        char * dest = fname;
        for (const char * c = ns; *c; c++)
            if (*c != '.')
                *dest++ = *c;
            else
            {
                *dest++ = '_';
                *dest++ = '_';
            }
        *dest = '\0';

        strcat(fname, name);
        pSymRec->fullName = fname;
        break;
    }
    default:
        pSymRec->fullName = nullptr;
        break;
    }


    return pSymRec;
}
//------------------------------------------------------------------------------
// SymRec (END)
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// SymTab
//------------------------------------------------------------------------------
SymTab* symtab_create(ParseData * pParseData)
{
    SymTab* pSymTab = COMP_NEW(SymTab);
    pSymTab->pParent = nullptr;
    pSymTab->pAst = nullptr;
    pSymTab->pParseData = pParseData;
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

SymTab* symtab_add_symbol_with_fields(SymTab* pSymTab, SymRec * pSymRec, ParseData * pParseData)
{
    symtab_add_symbol(pSymTab, pSymRec, pParseData);

    if (pSymRec->pSymDataType)
    {
        for (const SymStructField * pField : pSymRec->pSymDataType->fields)
        {
            size_t nameLen = strlen(pSymRec->name) + 1 + strlen(pField->name); // +1 for '.'
            char * qualifiedName = (char*)COMP_ALLOC(nameLen+1);
            strcpy(qualifiedName, pSymRec->name);
            strcat(qualifiedName, ".");
            strcat(qualifiedName, pField->name);

            SymRec * pFieldSymRec = symrec_create(pSymRec->type,
                                                  pField->pSymDataType,
                                                  qualifiedName,
                                                  nullptr,
                                                  pParseData);

            pFieldSymRec->flags |= (kSRFL_Member | kSRFL_NeedsCppParens);

            symtab_add_symbol_with_fields(pSymTab, pFieldSymRec, pParseData);
        }
    }

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

// check if name matches this using
const char * namespace_match(const char * name, const Using & using_)
{
    const char * dotPos = strrchr(name, '.');

    // If there's no '.', this identifier for sure doesn't match this using
    if (!dotPos)
        return nullptr;

    const char * unqualified = dotPos+1;
    size_t unqualifiedLen = strlen(unqualified);

    // Id ends with a '.', bad juju
    if (unqualifiedLen == 0)
        return nullptr;

    size_t namespaceLen = dotPos - name;
    bool namespaceMatch = (strncmp(using_.namespace_, name, namespaceLen) == 0 ||
                           strncmp(using_.pParseData->namespace_, name, namespaceLen) == 0);

    if (!namespaceMatch)
        return nullptr;

    // We have a match, return the unqualified name
    return unqualified;
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
    else
    {
        // We're in the root and haven't found the symbol.
        // Try to find it in an using'ed symbol list.
        for (const Using & using_ : pSymTab->pParseData->usings)
        {
            const char * unqualifiedName = namespace_match(name, using_);
            if (unqualifiedName) // if name starts with "using as" namespace, or the literal pParseData namespace
            {
                SymRec * pSymRec = symtab_find_symbol(using_.pParseData->pRootScope->pSymTab, unqualifiedName);
                if (pSymRec)
                    return pSymRec;
            }
        }
    }

    // Ok, we haven't found the symbol anywhere, including explicit usings.
    // Attempt to implicitly using the containing file.
    {
        const char * dotPos = strrchr(name, '.');
        if (dotPos)
        {
            size_t usingNameLen = dotPos - name;
            if (usingNameLen <= 1)
                return nullptr; // seems like bad id, like it starts with a '.'

            char * usingName = (char*)COMP_ALLOC(usingNameLen+1);
            strncpy(usingName, name, usingNameLen);
            usingName[usingNameLen] = '\0';

            // we have a dot, attempt to load the file
            const char * path = parsedata_dotted_to_path(pSymTab->pParseData, usingName);
            if (path)
            {
                // path seems possibly valid, continue with the using
                const Using * pUsing = parsedata_parse_using(pSymTab->pParseData, nullptr, path); // null for namespace, using will use implicit namespace of the file
                if (pUsing)
                {
                    const char * unqualifiedName = namespace_match(name, *pUsing);
                    SymRec * pSymRec = symtab_find_symbol(pUsing->pParseData->pRootScope->pSymTab, unqualifiedName);
                    if (pSymRec)
                        return pSymRec;
                }
            }
        }
    }

    COMP_ERROR(pSymTab->pParseData, "Symbol not found: %s", name);
    return nullptr;
}

SymRec* symtab_find_type(SymTab* pSymTab, const char * name)
{
    // LORRTEMP
    return nullptr;
}

const SymDataType* symtab_find_type_recursive(SymTab* pSymTab, DataType dataType, int isConst, int isReference)
{
    ASSERT(pSymTab);

    // LORRTEMP
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
    pAst->pParent = nullptr;
    pAst->pScope = parsedata_current_scope(pParseData);
    pAst->pSymRec = nullptr;
    pAst->pSymDataType = nullptr;

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

Ast * ast_create_with_str(AstType astType, const char * str, ParseData * pParseData)
{
    Ast * pAst = ast_create(astType, pParseData);
    pAst->str = str;
    return pAst;
}

Ast * ast_create_with_numi(AstType astType, int numi, ParseData * pParseData)
{
    Ast * pAst = ast_create(astType, pParseData);
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
                                  const SymDataType * pReturnType,
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

    pAst->pSymRec = symrec_create(symType,
                                  pReturnType,
                                  name,
                                  pAst,
                                  pParseData);

    parsedata_add_local_symbol(pParseData, pAst->pSymRec);

    if (pParent)
        ast_add_child(pParent, pAst);

    return pAst;
}

static Ast * ast_register_system_api(const char * name,
                                     AstType astType,
                                     SymType symType,
                                     SymRec * pReturnTypeSymRec,
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

    pAst->pSymRec = symrec_create(symType,
                                  pReturnTypeSymRec->pSymDataType,
                                  name,
                                  pAst,
                                  pParseData);

    parsedata_add_local_symbol(pParseData, pAst->pSymRec);

    if (pParent)
        ast_add_child(pParent, pAst);

    return pAst;
}

void ast_create_using_list(Ast * pUsingList, ParseData * pParseData)
{
    pParseData->pRootAst->pLhs = pUsingList;
}

Ast * ast_create_using_stmt(Ast * pUsingDottedId, Ast * pAsDottedId, ParseData * pParseData)
{
    ASSERT(pUsingDottedId->str);
    
    Ast * pAst = ast_create(kAST_UsingStmt, pParseData);
    ast_set_lhs(pAst, pUsingDottedId);
    ast_set_rhs(pAst, pAsDottedId);

    const char * path = parsedata_dotted_to_path(pParseData, pUsingDottedId->str);
    if (!path)
    {
        COMP_ERROR(pParseData, "Failed to find using: %s", pUsingDottedId->str);
    }
    
    // Do the using
    parsedata_parse_using(pParseData, pAsDottedId->str, path);

    return pAst;
}

Ast * ast_create_function_def(const char * name, const SymDataType * pReturnType, Ast * pBlock, ParseData * pParseData)
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

Ast * ast_create_property_def(const char * name, const SymDataType * pDataType, Ast * pInitVal, ParseData * pParseData)
{
    ASSERT(pParseData);

    Ast * pAst = ast_create(kAST_PropertyDef, pParseData);

    pAst->pSymRec = symrec_create(kSYMT_Property,
                                  pDataType,
                                  name,
                                  pInitVal,
                                  pParseData);
    pAst->pSymRec->flags |= kSRFL_NeedsCppParens;

    Scope * pScope = pParseData->scopeStack.back();
    symtab_add_symbol_with_fields(pScope->pSymTab, pAst->pSymRec, pParseData);

    return pAst;
}

Ast * ast_create_field_def(const char * name, const SymDataType * pDataType, Ast * pInitVal, ParseData * pParseData)
{
    ASSERT(pParseData);

    Ast * pAst = ast_create(kAST_FieldDef, pParseData);

    pAst->pSymRec = symrec_create(kSYMT_Field,
                                  pDataType,
                                  name,
                                  pInitVal,
                                  pParseData);
    pAst->pSymRec->flags |= kSRFL_NeedsCppParens;

    Scope * pScope = pParseData->scopeStack.back();
    symtab_add_symbol_with_fields(pScope->pSymTab, pAst->pSymRec, pParseData);

    return pAst;
}

Ast * ast_create_function_arg(const char * name, SymRec * pDataTypeSymRec, ParseData * pParseData)
{
    ASSERT(pParseData);
    Ast * pAst = ast_create(kAST_FunctionArg, pParseData);
    pAst->str = name;
    pAst->pSymRec = pDataTypeSymRec;
    return pAst;
}

Ast * ast_create_component_members(Ast * pAst, ParseData * pParseData)
{
    parsedata_pop_scope(pParseData);
    return pAst;
}

Ast * ast_create_component_member(Ast * pDottedId, Ast * pPropInitList, ParseData * pParseData)
{
    if (pPropInitList->type == kAST_PropInitList)
    {
        // implies there was a '{' '}' provided, so we don't need to pop the scope
        parsedata_pop_scope(pParseData);
    }

    SymRec* pCompSymRec = symtab_find_symbol_recursive(pPropInitList->pScope->pSymTab, pDottedId->str);

    if (!pCompSymRec)
    {
        COMP_ERROR(pParseData, "Unknown component: %s", pDottedId->str);
        return nullptr;
    }

    Ast * pAst = ast_create(kAST_ComponentMember, pParseData);
    pAst->str = pDottedId->str;
    pAst->pSymRec = pCompSymRec;
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

Ast * ast_create_assign_op(AstType astType, Ast * pDottedId, Ast * pRhs, ParseData * pParseData)
{
    Ast * pAst = ast_create(astType, pParseData);

    SymRec * pSymRec = parsedata_find_symbol(pParseData, pDottedId);

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
            COMP_ERROR(pParseData, "Invalid use of symbol in assignment: %s", pDottedId->str);
            return pAst;
        }

        pAst->pSymRec = pSymRec;
    }

    ast_set_lhs(pAst, pDottedId);
    ast_set_rhs(pAst, pRhs);
    return pAst;
}

Ast * ast_create_scalar_init(DataType dataType, Ast * pParams, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_ScalarInit, pParseData);

    const SymDataType * pSdt = parsedata_find_basic_type(pParseData, dataType, 0, 0);
    ASSERT(pSdt);

    pAst->pSymDataType = pSdt;

    if (pParams->pChildren->nodes.size() == 1)
    {
        const SymDataType * pParamSdt = ast_data_type(pParams->pChildren->nodes.front());

        // LORRTODO: Compare types (pSdt and pParamSdt) and make sure conversion is possible

        ast_set_rhs(pAst, pParams);
    }
    else
    {
        COMP_ERROR(pParseData, "Invalid parameters for %s initialization", pSdt->name);
    }

    return pAst;
}

Ast * ast_create_color_init(Ast * pParams, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_ColorInit, pParseData);
    ast_set_rhs(pAst, pParams);

    switch (pParams->pChildren->nodes.size())
    {
    case 4:
        for (Ast * pParam : pParams->pChildren->nodes)
        {
            const SymDataType * pSdt = ast_data_type(pParam);
            if (pSdt->typeDesc.dataType != kDT_int && pSdt->typeDesc.dataType != kDT_uint)
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
    Ast * pAst = ast_create(kAST_Vec3Init, pParseData);
    ast_set_rhs(pAst, pParams);

    switch (pParams->pChildren->nodes.size())
    {
    case 1:
    {
        Ast * pParam = pParams->pChildren->nodes.front();
        const SymDataType * pSdt = ast_data_type(pParam);
        if (pSdt->typeDesc.dataType != kDT_float && pSdt->typeDesc.dataType != kDT_vec3)
            COMP_ERROR(pParseData, "Invalid data type in vec3 initialization");
        break;
    }
    case 3:
    {
        for (Ast * pParam : pParams->pChildren->nodes)
        {
            const SymDataType * pSdt = ast_data_type(pParam);
            if (pSdt->typeDesc.dataType != kDT_float)
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

Ast * ast_create_vec4_init(Ast * pParams, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_Vec4Init, pParseData);
    ast_set_rhs(pAst, pParams);

    switch (pParams->pChildren->nodes.size())
    {
    case 4:
    {
        for (Ast * pParam : pParams->pChildren->nodes)
        {
            const SymDataType * pSdt = ast_data_type(pParam);
            if (pSdt->typeDesc.dataType != kDT_float)
                COMP_ERROR(pParseData, "Invalid data type in vec4 initialization");
        }
        break;
    }
    default:
        COMP_ERROR(pParseData, "Invalid parameters for vec4 initialization");
        break;
    }

    return pAst;
}

Ast * ast_create_quat_init(Ast * pParams, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_QuatInit, pParseData);
    ast_set_rhs(pAst, pParams);

    switch (pParams->pChildren->nodes.size())
    {
    case 4:
    {
        for (Ast * pParam : pParams->pChildren->nodes)
        {
            const SymDataType * pSdt = ast_data_type(pParam);
            if (pSdt->typeDesc.dataType != kDT_float)
                COMP_ERROR(pParseData, "Invalid data type in quat initialization");
        }
        break;
    }
    default:
        COMP_ERROR(pParseData, "Invalid parameters for quat initialization");
        break;
    }

    return pAst;
}

Ast * ast_create_mat34_init(Ast * pParams, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_Mat34Init, pParseData);
    ast_set_rhs(pAst, pParams);

    switch (pParams->pChildren->nodes.size())
    {
    case 1:
    {
        Ast * pParam = pParams->pChildren->nodes.front();
        const SymDataType * pSdt = ast_data_type(pParam);
        if (pSdt->typeDesc.dataType != kDT_float && pSdt->typeDesc.dataType != kDT_mat34)
            COMP_ERROR(pParseData, "Invalid data type in mat34 initialization");
        break;
    }
    case 12:
    {
        for (Ast * pParam : pParams->pChildren->nodes)
        {
            const SymDataType * pSdt = ast_data_type(pParam);
            if (pSdt->typeDesc.dataType != kDT_float)
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

Ast * ast_create_string_init(Ast * pParams, ParseData * pParseData)
{
    if (!pParams->pChildren || pParams->pChildren->nodes.size() == 0)
    {
        // no params, just a blank string
        Ast * pAst = ast_create(kAST_StringInit, pParseData);
        return pAst;
    }
    else if (pParams->pChildren->nodes.size() == 1)
    {
        Ast * pChild = pParams->pChildren->nodes.front();
        const SymDataType * pSdt = ast_data_type(pChild);
        switch (pSdt->typeDesc.dataType)
        {
        case kDT_bool:
        case kDT_char:
        case kDT_byte:
        case kDT_short:
        case kDT_ushort:
        case kDT_int:
        case kDT_uint:
        case kDT_long:
        case kDT_ulong:
        case kDT_half:
        case kDT_float:
        case kDT_double:
        case kDT_color:
        case kDT_vec2:
        case kDT_vec3:
        case kDT_vec4:
        case kDT_quat:
        case kDT_mat3:
        case kDT_mat34:
        case kDT_mat4:
        case kDT_string:
        {
            Ast * pAst = ast_create(kAST_StringInit, pParseData);
            ast_set_rhs(pAst, pChild);
            return pAst;
        }
        default:
            COMP_ERROR(pParseData, "Unable to convert data type %d to string", pSdt->typeDesc.dataType);
            return nullptr;
        }
    }
    else
    {
        // multiple children, only valid if first is a string
        Ast * pFirstChild = pParams->pChildren->nodes.front();
        if (pFirstChild->type != kAST_StringLiteral)
        {
            COMP_ERROR(pParseData, "First parameter expected to be a format string literal");
            return nullptr;
        }

        // Sanity check the format string and make sure it appears valid
        // for the provided parameters. Incidentally, this is why we
        // require a string literal, and don't accept an arbitrary string
        // Ast. It's not possible to codegen reasonable C++ that runtime
        // checks the validity of the parameters.
        const char * format = pFirstChild->str;
        const char * formatEnd = format + strlen(format);

        u32 paramCount = (u32)(pParams->pChildren->nodes.size() - 1);
        auto paramStart = pParams->pChildren->nodes.begin();
        paramStart++; // to skip past format string
        auto paramEnd = pParams->pChildren->nodes.end();
        auto paramIt = paramStart;

        const char * p = format;

        // Iterate once to collect the FormatSpecifiers and make sure
        // format string and parameter counts are compatible.
        u32 i = 0;
        while (i < paramCount)
        {
            FormatSpecifier fspec = find_next_specifier(p);

            if (!fspec.begin)
            {
                COMP_ERROR(pParseData, "Too few arguments provided in format string: %s", format);
                return nullptr;
            }
            ASSERT(fspec.end && fspec.end > fspec.begin);
            if (fspec.end > formatEnd)
            {
                COMP_ERROR(pParseData, "Error processing format string: %s", format);
                return nullptr;
            }

            if (fspec.flagStarWidth)
            {
                if (!is_integral_type(ast_data_type(*paramIt)))
                {
                    COMP_ERROR(pParseData, "integral type expected in position %u for %s", i, format);
                    return nullptr;
                }
                i++;
                paramIt++;
                if (i > paramCount || (i < paramCount && paramIt == paramEnd))
                {
                    COMP_ERROR(pParseData, "Too few arguments provided in format string: %s", format);
                    return nullptr;
                }
            }

            if (fspec.flagStarPrecision)
            {
                if (!is_integral_type(ast_data_type(*paramIt)))
                {
                    COMP_ERROR(pParseData, "integral type expected in position %u for %s", i, format);
                    return nullptr;
                }
                i++;
                paramIt++;
                if (i > paramCount || (i < paramCount && paramIt == paramEnd))
                {
                    COMP_ERROR(pParseData, "Too few arguments provided in format string: %s", format);
                    return nullptr;
                }
            }

            switch (fspec.type)
            {
            case 'd':
            case 'i':
            case 'u':
            case 'x':
            case 'X':
            case 'o':
                if (!is_integral_type(ast_data_type(*paramIt)))
                {
                    COMP_ERROR(pParseData, "integral parameter expected in position %u for %s", i, format);
                    return nullptr;
                }
                break;

            case 'c':
                if (ast_data_type(*paramIt)->typeDesc.dataType != kDT_char)
                {
                    COMP_ERROR(pParseData, "char parameter expected in position %u for %s", i, format);
                    return nullptr;
                }
                break;

            case 'f':
            case 'F':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
                if (ast_data_type(*paramIt)->typeDesc.dataType != kDT_float)
                {
                    COMP_ERROR(pParseData, "float parameter expected in position %u for %s", i, format);
                    return nullptr;
                }
                break;
            }

            i++;
            paramIt++;
            if (i > paramCount || (i < paramCount && paramIt == paramEnd))
            {
                COMP_ERROR(pParseData, "Too few arguments provided in format string: %s", format);
                return nullptr;
            }

            p = fspec.end;
        }

        if (i != paramCount || paramIt != paramEnd)
        {
            COMP_ERROR(pParseData, "Invalid number of arguments provided in format string: %s", format);
            return nullptr;
        }

        // Ok, we've sanity checked about as well as we can at compile time.

        Ast * pAst = ast_create(kAST_StringFormat, pParseData);
        ast_add_children(pAst, pParams->pChildren);

        return pAst;
    }
}

Ast * ast_create_type_init(DataType dataType, Ast * pParams, ParseData * pParseData)
{
    // pop scope that got created by the lexer when encountering the '{'
    parsedata_pop_scope(pParseData);

    switch (dataType)
    {
    case kDT_color:
        return ast_create_color_init(pParams, pParseData);
    case kDT_vec3:
        return ast_create_vec3_init(pParams, pParseData);
    case kDT_vec4:
        return ast_create_vec4_init(pParams, pParseData);
    case kDT_quat:
        return ast_create_quat_init(pParams, pParseData);
    case kDT_mat34:
        return ast_create_mat34_init(pParams, pParseData);
    case kDT_string:
        return ast_create_string_init(pParams, pParseData);
    default:
        return ast_create_scalar_init(dataType, pParams, pParseData);
    }
}

Ast * ast_create_entity_or_struct_init(Ast * pDottedId, Ast * pParams, ParseData * pParseData)
{
    // pop scope that got created by the lexer when encountering the '{'
    parsedata_pop_scope(pParseData);

    SymRec * pSymRec = parsedata_find_symbol(pParseData, pDottedId);

    Ast * pAst = nullptr;

    if (!pSymRec)
    {
        COMP_ERROR(pParseData, "Unknown entity: %s", pDottedId->str);
        return pAst;
    }

    if (pSymRec->type == kSYMT_Entity)
    {
       pAst = ast_create(kAST_EntityInit, pParseData);
       pAst->str = pDottedId->str;
       pAst->pSymRec = pSymRec;
       ast_set_rhs(pAst, pParams);

       // Build a unique hash for this entity init.
       // We'll use this hash to name a function in codegen to create/init this entity.
       // We use filename+col+line as a basis for the hash.
       {
           size_t initNameStrLen = strlen(pParseData->namespace_) + strlen(pDottedId->str) + 16;
           char * initNameStr = (char*)COMP_ALLOC(initNameStrLen+1);
           snprintf(initNameStr, initNameStrLen, "%s_%s__%d_%d", pParseData->namespace_, pDottedId->str, pParseData->line, pParseData->column);
           // replace any '.' with '_'
           char * p = initNameStr;
           while (*p++) 
               if (*p == '.')
                   *p = '_';
           pAst->str = initNameStr;
       }
    }
    else
    {
        COMP_ERROR(pParseData, "Support for structs not yet implemented: %s", pDottedId->str);
        return pAst;
    }

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

Ast * ast_create_string_literal(const char * str, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_StringLiteral, pParseData);
    size_t strLength = strlen(str);
    ASSERT(str[0] == '"' && str[strLength-1] == '"');
    const char * s = str + 1;
    char * d = (char*)COMP_ALLOC(strLength - 2 + 1); // -2 for leading/trailing '"', +1 for null termination
    pAst->str = d;

    while (*s)
    {
        if (*s == '\\')
        {
            switch(s[1])
            {
            case 'a':
                *d = 0x07; // Alarm (Beep, Bell)
                break;
            case 'b':
                *d = 0x08; // Backspace
                break;
            case 'f':
                *d = 0x0c; // Formfeed
                break;
            case 'n':
                *d = 0x0a; // Newline
                break;
            case 'r':
                *d = 0x0a; // Newline
                // \r\n get converted to just a single \n
                if (s[2] == '\n')
                    s++;
                break;
            case 't':
                *d = 0x09; // Horizontal Tab
                break;
            case 'v':
                *d = 0x0B; // Vertical Tab
                break;
            case '\\':
                *d = '\\'; // Backslash
                break;
            case '\'':
                *d = '\''; // Single quotation mark
                break;
            case '"':
                *d = '"'; // Double quotation mark
                break;
            case '?':
                *d = '?'; // Question mark
                break;
            default:
                *d = '\\';
                d++;
                *d = s[1]; 
                break;
            }
            s++; // extra ++ to get past '\'
        }
        else
        {
            *d = *s;
        }
        s++;
        d++;
    }

    // null terminate over the trailing '"'
    d--;
    ASSERT(*d == '"');
    *d = '\0';

    return pAst;
}

Ast * ast_create_function_call(Ast * pDottedId, Ast * pParams, ParseData * pParseData)
{
    Ast * pAst = nullptr;

    SymRec * pSymRec = parsedata_find_function_symbol(pParseData, pDottedId->str, pParams);

    if (pSymRec)
    {
        pAst = ast_create(kAST_FunctionCall, pParseData);

        if (pSymRec->type != kSYMT_Function)
        {
            ASSERT(pDottedId && pDottedId->str);
            COMP_ERROR(pParseData, "Call to non-function symbol: %s", pDottedId->str);
            return nullptr;
        }
        else
        {
            pAst->pSymRec = pSymRec;
            ast_set_rhs(pAst, pParams);
        }
    }
    else
    {
        ASSERT(pDottedId && pDottedId->str);
        COMP_ERROR(pParseData, "Unknown function: %s", pDottedId->str);
        return nullptr;
    }

    ASSERT(pAst);
    return pAst;
}

Ast * ast_create_system_api_call(const char * pApiName, Ast * pParams, ParseData * pParseData)
{
    Ast * pAst = nullptr;

    SymRec * pSymRec = parsedata_find_function_symbol(pParseData, pApiName, pParams);

    if (pSymRec && pSymRec->type == kSYMT_SystemApi)
    {
        pAst = ast_create(kAST_SystemCall, pParseData);

        if (pSymRec->type != kSYMT_SystemApi)
        {
            ASSERT(pApiName);
            COMP_ERROR(pParseData, "Call to non-system api symbol: %s", pApiName);
            return nullptr;
        }
        else
        {
            pAst->pSymRec = pSymRec;
            ast_set_rhs(pAst, pParams);
            pAst->str = unmangle_function(pSymRec->name);
        }
    }
    else
    {
        COMP_ERROR(pParseData, "Unknown system api: %s", pApiName);
        return nullptr;
    }

    ASSERT(pAst);
    return pAst;
}

Ast * ast_create_symbol_ref(Ast * pDottedId, ParseData * pParseData)
{
    SymRec * pSymRec = parsedata_find_symbol(pParseData, pDottedId);

    Ast * pAst = ast_create(kAST_SymbolRef, pParseData);
    ast_set_lhs(pAst, pDottedId);

    if (!pSymRec)
    {
        COMP_ERROR(pParseData, "Unknown symbol reference: %s", pDottedId->str);
        return pAst;
    }

    if (pSymRec->type != kSYMT_Param &&
        pSymRec->type != kSYMT_Local &&
        pSymRec->type != kSYMT_Field &&
        pSymRec->type != kSYMT_Property)
    {
        COMP_ERROR(pParseData, "Invalid use of symbol: %s", pDottedId->str);
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
    // "@tid#prop = 5;" is really just sugar for "@tid#set_property(#prop, 5);"
    // So, we mimic this set_property form and pass along through normal
    // asg_create_message_send processing.

    Ast * pParams = ast_create(kAST_FunctionParams, pParseData);
    Ast * pMsgType = ast_create_hash(propertyStr, pParseData);

    ast_add_child(pParams, pMsgType);
    ast_add_child(pParams, pRhs);

    return ast_create_message_send(pTarget, "set_property", pParams, pParseData);
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

const SymDataType * ast_data_type(const Ast * pAst)
{
    // LORRTODO: This should be simplified now that we have
    // SymDataType, most o fthese should be set at parse time on the
    // ast instead of having to go through this case statement.

    if (pAst->pSymRec)
        return pAst->pSymRec->pSymDataType;

    SymRec * pSymRec = nullptr;

    if (pAst->pSymDataType)
        return pAst->pSymDataType;

    switch (pAst->type)
    {
    case kAST_Hash:
        pSymRec = parsedata_find_type_symbol(pAst->pParseData, "uint", 0, 0);
        break;
    case kAST_FloatLiteral:
        pSymRec = parsedata_find_type_symbol(pAst->pParseData, "float", 0, 0);
        break;
    case kAST_IntLiteral:
        pSymRec = parsedata_find_type_symbol(pAst->pParseData, "int", 0, 0);
        break;
    case kAST_StringLiteral:
    case kAST_StringInit:
    case kAST_StringFormat:
        pSymRec = parsedata_find_type_symbol(pAst->pParseData, "string", 0, 0);
        break;
    case kAST_ColorInit:
        pSymRec = parsedata_find_type_symbol(pAst->pParseData, "color", 0, 0);
        break;
    case kAST_Vec2Init:
        pSymRec = parsedata_find_type_symbol(pAst->pParseData, "vec2", 0, 0);
        break;
    case kAST_Vec3Init:
        pSymRec = parsedata_find_type_symbol(pAst->pParseData, "vec3", 0, 0);
        break;
    case kAST_Vec4Init:
        pSymRec = parsedata_find_type_symbol(pAst->pParseData, "vec4", 0, 0);
        break;
    case kAST_QuatInit:
        pSymRec = parsedata_find_type_symbol(pAst->pParseData, "quat", 0, 0);
        break;
    case kAST_Mat34Init:
    case kAST_Transform:
        pSymRec = parsedata_find_type_symbol(pAst->pParseData, "mat34", 0, 0);
        break;
    case kAST_Negate:
        return ast_data_type(pAst->pRhs);
    case kAST_SystemCall:
    case kAST_FunctionArg:
        return pAst->pSymRec->pSymDataType;
    }

    if (!pSymRec)
    {
        COMP_ERROR(pAst->pParseData, "Cannot determine datatype for pAst, type: %d", pAst->type);
        return nullptr;
    }

    return pSymRec->pSymDataType;
}

const SymDataType * const_data_type(ParseData * pParseData, const SymDataType * pSdt)
{
    if (pSdt->typeDesc.isConst)
        return pSdt;
    else
    {
        SymRec * pSymRec = parsedata_find_type_symbol(pParseData, pSdt->name, true, pSdt->typeDesc.isReference);
        ASSERT(pSymRec);
        return pSymRec->pSymDataType;
    }
}

const SymDataType * non_const_data_type(ParseData * pParseData, const SymDataType * pSdt)
{
    if (!pSdt->typeDesc.isConst)
        return pSdt;
    else
    {
        SymRec * pSymRec = parsedata_find_type_symbol(pParseData, pSdt->name, false, pSdt->typeDesc.isReference);
        ASSERT(pSymRec);
        return pSymRec->pSymDataType;
    }
}

int is_block_memory_type(const SymDataType * pSdt)
{
    return (pSdt->typeDesc.dataType == kDT_string ? 1 : 0);
}

int is_integral_type(const SymDataType * pSdt)
{
    DataType dt = pSdt->typeDesc.dataType;
    return (dt == kDT_int || dt== kDT_uint || dt == kDT_short || dt == kDT_ushort) ? 1 : 0;
}


//------------------------------------------------------------------------------
// Ast (END)
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Scope
//------------------------------------------------------------------------------
Scope * scope_create(ParseData * pParseData)
{
    Scope * pScope = COMP_NEW(Scope);
    pScope->pAstList = astlist_create();
    pScope->pSymTab = symtab_create(pParseData);

    return pScope;
}
//------------------------------------------------------------------------------
// Scope (END)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ParseData
//------------------------------------------------------------------------------
ParseData * parsedata_create(const char * fullPath,
                             CompList<CompString> * pApiIncludes,
                             MessageHandler messageHandler)
{
    ParseData * pParseData = COMP_NEW(ParseData);

    pParseData->skipNextScope = false;
    pParseData->hasErrors = false;
    pParseData->pScanner = nullptr;

    pParseData->messageHandler = messageHandler;

    pParseData->pRootScope = scope_create(pParseData);
    pParseData->scopeStack.push_back(pParseData->pRootScope);

    pParseData->pRootAst = ast_create(kAST_Root, pParseData);

    pParseData->pRootScope->pSymTab->pAst = pParseData->pRootAst;

    pParseData->pApiIncludes = pApiIncludes;

    parsedata_prep_paths(pParseData, fullPath);

    register_basic_types(pParseData);
    register_system_apis(pParseData);

    return pParseData;
}

void parsedata_destroy(ParseData * pParseData)
{
    ASSERT(pParseData);
    COMP_FREE(pParseData);
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
    for (size_t i = 0; i < dottedIdLen; ++i)
    {
        if (dotted[i] == '.')
            dotted[i] = '/';
    }

    strcat(path, ".cmp");

    return path;
}

static bool has_invalid_underscore(const char * str)
{
    if (!str || !str[0])
        return false;
    if (*str == '_')                          // "_" at start
        return true;
    for (const char * c = str+1; *c; c++)
        if (*(c-1) == '_' && *c == '_')       // "__"
            return true;
        else if (*(c-1) == '.' && *c == '_')  // "._"
            return true;
        else if (*(c-1) == '_' && *c == '.')  // "_."
            return true;
        else if (*c == '_' && !*(c+1))        // "_" at end
            return true;
    return false;
}

void parsedata_prep_paths(ParseData * pParseData, const char * fullPath)
{
    pParseData->fullPath = gaen::full_path(fullPath, pParseData);
    pParseData->filename = gaen::path_filename(pParseData->fullPath, pParseData);

    if (has_invalid_underscore(pParseData->filename))
    {
        COMP_ERROR(pParseData, "Some portion of .cmp path has invalid underscores (double, leading, or trailing) on a directory or filename: %s", pParseData->fullPath);
        return;
    }

    const char * dotPos = strrchr(pParseData->filename, '.');
    if (!dotPos ||
        strcmp(dotPos, ".cmp") != 0 ||
        dotPos <= pParseData->filename) // ensure there's something before the '.'
    {
        COMP_ERROR(pParseData, "Invalid extension for Compose script, must be .cmp: %s", fullPath);
        return;
    }

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

    size_t namespaceLen = fullPathLen - scriptsRootLen;
    ASSERT(namespaceLen > 3);
    namespaceLen -= 3; // for "cmp", leave the '.' as we want our namespace to have a trailing '.'
    char * namespace_ = (char*)COMP_ALLOC(namespaceLen + 1);
    strncpy(namespace_, pParseData->fullPath + scriptsRootLen, namespaceLen);
    namespace_[namespaceLen] = '\0';

    // The only '.' character in the namespace should be at the end.
    if (strchr(namespace_, '.') != namespace_ + namespaceLen - 1)
    {
        COMP_ERROR(pParseData, "'.' characters are invalid in file or directory names in .cmp paths: %s", pParseData->fullPath);
        return;
    }

    char * p = namespace_;
    while (*p++)
        if (*p == '/')
            *p = '.';
    pParseData->namespace_ = namespace_;

    // Check for double underscores or trailing/leading underscores
    if (has_invalid_underscore(pParseData->namespace_))
    {
        COMP_ERROR(pParseData, "Some portion of .cmp path has invalid underscores (double, leading, or trailing) on a directory or filename: %s", pParseData->fullPath);
        return;
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
        symtab_add_symbol_with_fields(pSymTab, pSymRec, pParseData);
    }
    return pSymTab;
}

SymRec* parsedata_find_symbol(ParseData * pParseData, Ast * pAst)
{
    ASSERT(pParseData);
    ASSERT(pAst);
    return symtab_find_symbol_recursive(pParseData->scopeStack.back()->pSymTab, pAst->str);
}

SymRec* parsedata_find_function_symbol(ParseData * pParseData, const char * name, Ast * pParams)
{
    char mangledName[kMaxCmpId+1];
    mangle_function(mangledName, kMaxCmpId, name, pParams->pChildren);
    return symtab_find_symbol_recursive(pParseData->scopeStack.back()->pSymTab, mangledName);
}

SymRec* parsedata_find_type_symbol(ParseData * pParseData, const char * name, int isConst, int isReference)
{
    char mangledName[kMaxCmpId+1];
    mangle_type(mangledName, kMaxCmpId, name, isConst, isReference);
    return symtab_find_symbol_recursive(pParseData->scopeStack.back()->pSymTab, mangledName);
}

const SymDataType* parsedata_find_type(ParseData * pParseData, const char * name, int isConst, int isReference)
{
    return parsedata_find_type_symbol(pParseData, name, isConst, isReference)->pSymDataType;
}

SymRec* parsedata_find_type_symbol_from_dotted_id(ParseData * pParseData, const Ast * pAstId, int isConst, int isReference)
{
    return parsedata_find_type_symbol(pParseData, pAstId->str, isConst, isReference);
}

const SymDataType* parsedata_find_type_from_dotted_id(ParseData * pParseData, const Ast * pAstId, int isConst, int isReference)
{
    return parsedata_find_type(pParseData, pAstId->str, isConst, isReference);
}

Ast * parsedata_add_symbol(ParseData * pParseData, SymRec * pSymRec, Scope * pScope)
{
    ASSERT(pParseData);
    ASSERT(pParseData->scopeStack.size() >= 1);
    ASSERT(pSymRec);

    Ast * pAst = ast_create(kAST_SymbolDecl, pParseData);
    pAst->pSymRec = pSymRec;
    ast_set_rhs(pAst, pSymRec->pAst);

    symtab_add_symbol_with_fields(pScope->pSymTab, pSymRec, pParseData);
    return pAst;
}

Ast* parsedata_add_local_symbol(ParseData * pParseData, SymRec * pSymRec)
{
    return parsedata_add_symbol(pParseData, pSymRec, pParseData->scopeStack.back());
}

Ast* parsedata_add_root_symbol(ParseData * pParseData, SymRec * pSymRec)
{
    return parsedata_add_symbol(pParseData, pSymRec, pParseData->pRootScope);
}

const SymDataType * parsedata_find_basic_type(ParseData * pParseData, DataType dataType, int isConst, int isReference)
{
    TypeDesc td;
    td.dataType = dataType;
    td.isConst = isConst != 0;
    td.isReference = isReference != 0;

    return parsedata_find_basic_type_desc(pParseData, &td);
}

const SymDataType * parsedata_find_basic_type_desc(ParseData * pParseData, const TypeDesc * pTypeDesc)
{
    auto it = pParseData->basicTypes.find(*pTypeDesc);

    if (it != pParseData->basicTypes.end())
        return it->second;
    return nullptr;
}

void parsedata_register_basic_type(ParseData * pParseData, SymDataType * pSymDataType)
{
    if (!parsedata_find_basic_type_desc(pParseData, &pSymDataType->typeDesc))
    {
        pParseData->basicTypes[pSymDataType->typeDesc] = pSymDataType;
    }
    else
    {
        COMP_ERROR(pParseData,
                   "Basic type already registered: DataType=%d, IsConst=%d, IsReference=%d",
                   pSymDataType->typeDesc.dataType,
                   pSymDataType->typeDesc.isConst,
                   pSymDataType->typeDesc.isReference);
    }
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
        pScope = scope_create(pParseData);

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
    TLARRAY(char, tMessage, kMessageMax);

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

const Using * parsedata_parse_using(ParseData * pParseData,
                                    const char * namespace_,
                                    const char * fullPath)
{
    ParseData * pUsingParseData = parse_file(fullPath, pParseData->pApiIncludes, pParseData->messageHandler);

    if (!pUsingParseData)
    {
        COMP_ERROR(pParseData, "Failed to parse using: %s", fullPath);
        return nullptr;
    }
    
    Using imp;
    imp.pParseData = pUsingParseData;
    if (!namespace_ || namespace_[0] == '\0')
    {
        imp.namespace_ = pUsingParseData->namespace_; // if no namespace provided, use namespace of parsed using
    }
    else
    {
        char * pref = (char*)COMP_ALLOC(strlen(namespace_) + 1 + 1); // +1 for '.', +1 for null
        strcpy(pref, namespace_);
        strcat(pref, ".");
        imp.namespace_ = pref;
    }
        
    pParseData->usings.push_back(imp);

    return &pParseData->usings.back();
}

//------------------------------------------------------------------------------
// ParseData (END)
//------------------------------------------------------------------------------

void parse_init()
{
    comp_reset_mem();

    // LORRNOTE: Uncomment below to enable stderr based debug messages from parser
    //yydebug = 1;
}

i32 read_file(const char * path, char ** output)
{
    *output = nullptr;
    i32 length = 0;
    char * source = nullptr;
    FILE *fp = fopen(path, "r");

    if (!fp)
    {
        ERR("Failed to read file: %s", path);
        return -1;
    }

    // go to end of file
    if (fseek(fp, 0L, SEEK_END) == 0)
    {
        // get sizeof file
        long bufsize = ftell(fp);
        if (bufsize == -1)
        {
            ERR("Invalid file size: %s", path);
            fclose(fp);
            return -1;
        }

        source = static_cast<char*>(GALLOC(kMEM_Unspecified, (bufsize + 1)));

        // seek backto start
        if (fseek(fp, 0L, SEEK_SET) != 0)
        {
            ERR("Unable to seek back to start: %s", path);
            GFREE(source);
            fclose(fp);
            return -1;
        }

        length = static_cast<i32>(fread(source, sizeof(char), bufsize, fp));
        if (length == 0)
        {
            ERR("Error reading file: %s", path);
            GFREE(source);
            fclose(fp);
            return -1;
        }
        else
        {
            source[length+1] = '\0';
        }
    }
    fclose(fp);

    *output = source;
    return length;
}

ParseData * parse(const char * source,
                  size_t length,
                  const char * fullPath,
                  CompList<CompString> * pApiIncludes,
                  MessageHandler messageHandler)
{
    int ret;

    ParseData * pParseData = parsedata_create(fullPath, pApiIncludes, messageHandler);

    if (!source)
    {
        char * newSource = nullptr;
        i32 len = read_file(pParseData->fullPath, &newSource);
        if (len <= 0)
        {
            COMP_ERROR(pParseData, "Unable to read file: %s", pParseData->fullPath);
            return nullptr;
        }
        length = len;
        source = newSource;
    }

    ret = yylex_init_extra(pParseData, &pParseData->pScanner);
    if (ret != 0)
    {
        return nullptr;
    }

    yy_scan_bytes(source, length, pParseData->pScanner);

    ret = yyparse(pParseData);

    yylex_destroy(pParseData->pScanner);

    if (ret != 0)
    {
        return nullptr;
    }

    return pParseData;
}   

void yyerror(YYLTYPE * pLoc, ParseData * pParseData, const char * format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    pParseData->line = pLoc->first_line;
    pParseData->column = pLoc->first_column;
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

namespace gaen
{
    RelatedTypes register_basic_type(DataType dt,
                                     const char * name,
                                     const char * cppName,
                                     u32 cellCount,
                                     ParseData * pParseData)
    {
        RelatedTypes rt;

        // normal
        rt.pNormal = symdatatype_create(dt, name, cppName, cellCount, 0, 0, pParseData);
        parsedata_register_basic_type(pParseData, rt.pNormal);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_Type, rt.pNormal, rt.pNormal->mangledType, nullptr, pParseData));

        // const
        rt.pConst = symdatatype_create(dt, name, cppName, cellCount, 1, 0, pParseData);
        parsedata_register_basic_type(pParseData, rt.pConst);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_Type, rt.pConst, rt.pConst->mangledType, nullptr, pParseData));

        // reference
        rt.pReference = symdatatype_create(dt, name, cppName, cellCount, 0, 1, pParseData);
        parsedata_register_basic_type(pParseData, rt.pReference);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_Type, rt.pReference, rt.pReference->mangledType, nullptr, pParseData));

        // const reference
        rt.pConstReference = symdatatype_create(dt, name, cppName, cellCount, 1, 1, pParseData);
        parsedata_register_basic_type(pParseData, rt.pConstReference);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_Type, rt.pConstReference, rt.pConstReference->mangledType, nullptr, pParseData));

        return rt;
    }

    void register_basic_types(ParseData * pParseData)
    {
        register_basic_type(kDT_void,  "void", "void", 0, pParseData);
        register_basic_type(kDT_bool,  "bool", "bool", 1, pParseData);
        register_basic_type(kDT_int,   "int",  "i32",  1, pParseData);
        register_basic_type(kDT_uint,  "uint", "u32", 1, pParseData);
        register_basic_type(kDT_color, "color", "Color", 1, pParseData);

        // Save float related types since we need them to register fields of each
        // composite type.
        RelatedTypes floatRt = register_basic_type(kDT_float, "float", "f32", 1, pParseData);

        RelatedTypes rt;
        rt = register_basic_type(kDT_vec2, "vec2", "Vec2", 2, pParseData);
        symdatatype_add_field_related(&rt, floatRt.pNormal, "x");
        symdatatype_add_field_related(&rt, floatRt.pNormal, "y");

        rt = register_basic_type(kDT_vec3, "vec3", "Vec3", 3, pParseData);
        symdatatype_add_field_related(&rt, floatRt.pNormal, "x");
        symdatatype_add_field_related(&rt, floatRt.pNormal, "y");
        symdatatype_add_field_related(&rt, floatRt.pNormal, "z");

        rt = register_basic_type(kDT_vec4, "vec4", "Vec4", 4, pParseData);
        symdatatype_add_field_related(&rt, floatRt.pNormal, "x");
        symdatatype_add_field_related(&rt, floatRt.pNormal, "y");
        symdatatype_add_field_related(&rt, floatRt.pNormal, "z");
        symdatatype_add_field_related(&rt, floatRt.pNormal, "w");

        rt = register_basic_type(kDT_quat, "quat", "Quat", 4, pParseData);
        symdatatype_add_field_related(&rt, floatRt.pNormal, "x");
        symdatatype_add_field_related(&rt, floatRt.pNormal, "y");
        symdatatype_add_field_related(&rt, floatRt.pNormal, "z");
        symdatatype_add_field_related(&rt, floatRt.pNormal, "w");

        register_basic_type(kDT_mat3,  "mat3",  "Mat3",   9, pParseData);
        register_basic_type(kDT_mat34, "mat34", "Mat34", 12, pParseData);
        register_basic_type(kDT_mat4,  "mat4",  "Mat4",  16, pParseData);

        register_basic_type(kDT_handle, "handle", "Handle", 2, pParseData);
        register_basic_type(kDT_entity, "entity", "task_id", 1, pParseData);
        register_basic_type(kDT_string, "string", "CmpString", 2, pParseData);
    }

    ParseData * parse_file(const char * fullPath,
                           CompList<CompString> * pApiIncludes,
                           MessageHandler messageHandler)
    {
        return parse(nullptr, 0, fullPath, pApiIncludes, messageHandler);
    }

} // namespace gaen
