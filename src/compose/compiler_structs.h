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
    Ast * pAst;
    SymTab * pSymTab;
    u32 order;
};

struct SymTab
{
    SymTab * pParent;
    Ast * pAst;
    CompMap<const char*, SymRec*> dict;
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
    u32 blockIndex;
    u32 cellIndex;
    u32 cellCount;
    bool isPayload;
    bool isAssigned;

    BlockInfo(Ast * pAst);
};

struct BlockInfos
{
    u32 blockCount;
    CompVector<BlockInfo> items;

    BlockInfos()
      : blockCount(0)
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
    AstType type;
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

struct ParseData
{
    Ast* pRootAst;
    void * pScanner;
    Scope* pRootScope;
    CompList<Scope*> scopeStack;
    CompSet<CompString> strings;

    // location info
    int line;
    int column;

    const char * fullPath;
    const char * filename;
    MessageHandler messageHandler;

    bool skipNextScope;
    bool hasErrors;
};

#endif // #ifndef GAEN_COMPOSE_COMPILER_STRUCTS_H
