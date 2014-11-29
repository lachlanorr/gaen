//------------------------------------------------------------------------------
// compiler_structs.h - Compose script compiler structs
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

#ifndef GAEN_COMPOSE_COMPILER_STRUCTS_H
#define GAEN_COMPOSE_COMPILER_STRUCTS_H

#include "core/base_defines.h"
#include "compose/compiler.h"
#include "compose/comp_mem.h"

using namespace gaen;

struct SymRec
{
    SymType type;
    DataType dataType;
    const char * name;
    const char * full_name;
    Ast * pAst;
    SymTab * pSymTab;
    SymTab * pSymTabInternal;
    u32 order;
};

// Custom comparator for our "char*" map
struct StrcmpComp
{
   bool operator()(char const *lhs, char const *rhs)
   {
      return strcmp(lhs, rhs) < 0;
   }
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
    DataType dataType;
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
    AstFlags flags;
    Ast* pParent;
    Scope* pScope;
    SymRec* pSymRec;

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

    // location info
    int line;
    int column;

    const char * fullPath;
    const char * filename;
    const char * scriptsRootPath;
    size_t scriptsRootPathLen;
    MessageHandler messageHandler;

    bool skipNextScope;
    bool hasErrors;
};

#endif // #ifndef GAEN_COMPOSE_COMPILER_STRUCTS_H
