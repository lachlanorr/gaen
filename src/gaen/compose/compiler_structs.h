//------------------------------------------------------------------------------
// compiler_structs.h - Compose script compiler structs
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#ifndef GAEN_COMPOSE_COMPILER_STRUCTS_H
#define GAEN_COMPOSE_COMPILER_STRUCTS_H

#include "gaen/core/base_defines.h"
#include "gaen/core/mem.h"
#include "gaen/compose/compiler.h"
#include "gaen/compose/comp_mem.h"

using namespace gaen;

static const u32 kMaxCmpId = 128;

struct SymRec;
struct SymDataType;
struct SymStructField
{
    const SymDataType * pSymDataType;
    const char * name;
    u32 flags;
};

struct TypeDesc
{
    DataType dataType;
    bool isConst;
    bool isReference;

    // to support maps
    bool operator< (const TypeDesc & rhs) const
    {
        if (dataType == rhs.dataType)
        {
            if (isConst == rhs.isConst)
            {
                return isReference < rhs.isReference;
            }
            else
                return isConst < rhs.isConst;
        }
        else
            return dataType < rhs.dataType;
    }
};

struct SymDataType
{
    TypeDesc typeDesc;
    u32 cellCount;
    const char * name;
    const char * mangledType;
    const char * mangledParam;
    const char * cppName;
    const char * cppTypeStr;

    CompList<SymStructField*> fields;
};

struct RelatedTypes
{
    SymDataType * pNormal;
    SymDataType * pConst;
    SymDataType * pReference;
    SymDataType * pConstReference;
};

enum SymRecFlag
{
    kSRFL_None            = 0x0000,
    kSRFL_Member          = 0x0001,
    kSRFL_NeedsCppParens  = 0x0002,
    kSRFL_AssetRelated    = 0x0004,
    kSRFL_BuiltInFunction = 0x0008,
    kSRFL_BuiltInConst    = 0x0010
};

struct SymRec
{
    SymType type;
    const SymDataType * pSymDataType;
    const char * name;
    const char * fullName;
    Ast * pAst;
    Ast * pInitVal;
    SymTab * pSymTab;
    SymTab * pSymTabInternal;
    SymRec * pStructSymRec;
    u32 order;
    u32 flags;
};

struct SymTab
{
    SymTab * pParent;
    Ast * pAst;
    ParseData * pParseData;
    CompMap<const char*, SymRec*, StrcmpComp> dict;
    CompList<SymRec*> orderedSymRecs;
    CompList<SymTab*> children;
};

struct AstList
{
    CompList<Ast*> nodes;
};

// Block storage info for properties, fields, and message parameters
struct BlockInfo
{
    Ast * pAst;
    const SymDataType * pSymDataType;
    u32 blockIndex;
    u32 blockMemoryIndex;
    u32 cellIndex;
    u32 cellCount;
    bool isPayload;
    bool isAssigned;
    bool isBlockMemoryType;

    BlockInfo(Ast * pAst);
};

struct BlockInfos
{
    u32 blockCount;
    u32 blockMemoryItemCount;
    CompVector<BlockInfo> items;

    BlockInfos()
      : blockCount(0)
      , blockMemoryItemCount(0)
    {}

    const BlockInfo * find(const Ast * pAst) const
    {
        for (auto it = items.begin(); it != items.end(); ++it)
        {
            if (it->pAst == pAst)
                return &*it;
        }
        return nullptr;
    }

    const BlockInfo * find_payload() const
    {
        for (auto it = items.begin(); it != items.end(); ++it)
        {
            if (it->isPayload)
                return &*it;
        }
        return nullptr;
    }
};

struct Ast
{
    ParseData * pParseData;
    AstType type;
    Ast* pParent;
    Scope* pScope;
    SymRec* pSymRec;
    SymRec* pSymRecRef;
    const SymDataType * pSymDataType;

    Ast* pLhs;
    Ast* pMid;
    Ast* pRhs;

    int numi;
    float numf;
    const char * str;

    AstList* pChildren;
    const char * fullPath;

    BlockInfos * pBlockInfos;
};

struct Scope
{
    AstList * pAstList;
    SymTab * pSymTab;
};

struct Using
{
    const char * namespace_;
    const ParseData * pParseData;
};

struct ParseData
{
    Ast* pRootAst;
    void * pScanner;
    Scope* pRootScope;
    const char * namespace_;
    CompList<Scope*> scopeStack;
    CompSet<CompString> strings;

    // ParseData structs from compilations of usings
    CompList<Using> usings;

    CompMap<TypeDesc, SymDataType*> basicTypes;

    // location info
    int line;
    int column;

    const char * fullPath;
    const char * filename;
    const char * scriptsRootPath;
    size_t scriptsRootPathLen;
    MessageHandler messageHandler;

    bool hasErrors;

    CompList<CompString> * pSystemIncludes;
    CompSet<CompString> scriptIncludes;
};

#endif // #ifndef GAEN_COMPOSE_COMPILER_STRUCTS_H
