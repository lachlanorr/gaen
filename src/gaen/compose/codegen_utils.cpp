//------------------------------------------------------------------------------
// codegen_utils.cpp - Shared utilities used during code generation
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

#include <algorithm>

#include "gaen/core/HashMap.h"
#include "gaen/core/hashing.h"
#include "gaen/engine/Block.h"
#include "gaen/compose/codegen_utils.h"

// Define BlockInfo constructor here to avoid issues including this file from compiler_structs.h
BlockInfo::BlockInfo(Ast * pAst)
  : pAst(pAst)
  , blockIndex(0)
  , blockMemoryIndex(-1)
  , cellIndex(0)
  , cellCount(0)
  , isPayload(false)
  , isAssigned(false)
{
    cellCount = gaen::calc_cell_count(pAst);
    pSymDataType = non_const_data_type(pAst->pParseData, ast_data_type(pAst));
    isBlockMemoryType = is_block_memory_type(pSymDataType) ? true : false;
}

namespace gaen
{

const Ast * find_update(const Ast * pSearchAst)
{
    for (Ast * pChild : pSearchAst->pChildren->nodes)
    {
        if (pChild->type == kAST_UpdateDef)
        {
            return pChild;
        }
    }
    return nullptr;
}

const Ast * find_input(const Ast * pSearchAst, u32 idx)
{
    u32 i = 0;

    for (Ast * pChild : pSearchAst->pChildren->nodes)
    {
        if (pChild->type == kAST_Input)
        {
            if (i >= idx)
                return pChild;
            else
                ++i;
        }
    }
    return nullptr;
}

const Ast * find_component_members(const Ast * pSearchAst)
{
    Ast * pCompMembers = nullptr;
    for (Ast * pChild : pSearchAst->pChildren->nodes)
    {
        if (pChild->type == kAST_ComponentMemberList)
        {
            if (!pCompMembers)
                pCompMembers = pChild;
            else
            {
                // Based on grammar, we currently allow multiple
                // components sections in an entity.  We check for
                // this error here for simplicity.
                COMP_ERROR(pSearchAst->pParseData, "More than one components section defined in entity");
                return nullptr;
            }
        }
    }
    return pCompMembers;
}

u32 calc_cell_count(const Ast * pAst)
{
    const SymDataType * pSdt = pAst->pSymRecRef ? pAst->pSymRecRef->pSymDataType : ast_data_type(pAst);
    if (!pSdt)
        COMP_ERROR(pAst->pParseData, "Null cell count");
    return pSdt->cellCount;
}

const Ast * find_property_dependency(const Ast * pAst)
{
    if (pAst == nullptr)
        return nullptr;

    if (is_prop(pAst) || pAst->type == kAST_Transform)
        return pAst;

    const Ast * pCheck;
    if (pAst->pSymRecRef)
    {
        pCheck = find_property_dependency(pAst->pSymRecRef->pAst);
        if (pCheck)
            return pCheck;
    }

    pCheck = find_property_dependency(pAst->pLhs);
    if (pCheck)
        return pCheck;

    pCheck = find_property_dependency(pAst->pMid);
    if (pCheck)
        return pCheck;

    pCheck = find_property_dependency(pAst->pRhs);
    if (pCheck)
        return pCheck;

    if (pAst->pChildren)
    {
        for (const Ast *pChild : pAst->pChildren->nodes)
        {
            pCheck = find_property_dependency(pChild);
            if (pCheck)
                return pCheck;
        }
    }    
    return nullptr;
}

ScriptDataCategory data_category(const Ast * pAst)
{
    const SymRec * pSymRec = pAst->pSymRec;

    if (pSymRec)
    {
        if (is_asset_prop_or_field(pSymRec))
            return kSDC_Asset;
        else if (is_prop(pSymRec)) 
            return kSDC_Independent;
        else if (is_field(pSymRec))
        {
            if (find_property_dependency(pAst))
                return kSDC_Dependent;
            return kSDC_Independent;
        }
    }

    return kSDC_None;
}

u32 props_and_fields_count(const Ast * pAst)
{
    u32 count = 0;
    for (Ast *pChild : pAst->pChildren->nodes)
    {
        if (is_prop_or_field(pChild->pSymRec))
            count++;
    }
    return count;
}

static BlockInfo * find_next_fit(CompVector<BlockInfo> & items,
                                 u32 currCell,
                                 bool packForMessage)
{
    ASSERT(currCell < kCellsPerBlock);
    
    for (BlockInfo & item : items)
    {
        if (item.isAssigned)
            continue;

        if (packForMessage && item.isBlockMemoryType)
            continue;

        if (item.cellCount > 2 && currCell != 0)
            continue;
        else if (item.cellCount == 2 && currCell >= 3)
            continue;

        return &item;
    }
    return nullptr;
}

void block_pack_items(BlockInfos * pBlockInfos, bool packForMessage)
{
    if (packForMessage)
    {
        // for messages, ensure block memory items come at the end
        std::stable_sort(pBlockInfos->items.begin(),
                         pBlockInfos->items.end(),
                         [](const BlockInfo & a, const BlockInfo & b)
                         {
                             if (a.isBlockMemoryType && !b.isBlockMemoryType)
                                 return false;
                             else if (!a.isBlockMemoryType && b.isBlockMemoryType)
                                 return true;
                             else
                                 return a.cellCount > b.cellCount;
                         });
    }
    else
    {
        // for non-messages, pack block memory items with the rest
        std::stable_sort(pBlockInfos->items.begin(),
                         pBlockInfos->items.end(),
                         [](const BlockInfo & a, const BlockInfo & b)
                         { return a.cellCount > b.cellCount; });
    }

    u32 unassignedItemCount = (u32)pBlockInfos->items.size();
    if (packForMessage)
    {
        // remove block memory items from unassignedItemCount
        pBlockInfos->blockMemoryItemCount = 0;
        for (BlockInfo & bi : pBlockInfos->items)
        {
            if (bi.isBlockMemoryType)
            {
                --unassignedItemCount;
                bi.blockMemoryIndex = pBlockInfos->blockMemoryItemCount;
                ++pBlockInfos->blockMemoryItemCount;
            }
        }
    }

    u32 currBlock = 0;
    u32 currCell = 0;

    if (packForMessage)
    {
        // Messages use the payload of the first block for a 1 cell
        // value if possible.
        for (BlockInfo & bi : pBlockInfos->items)
        {
            if (bi.cellCount == 1)
            {
                bi.isAssigned = true;
                bi.isPayload = true;
                unassignedItemCount--;
                break;
            }
        }
    }

    while (unassignedItemCount > 0)
    {
        BlockInfo * pItem = find_next_fit(pBlockInfos->items, currCell, packForMessage);

        if (!pItem)
        {
            // no more items will fit in this block
            currBlock++;
            currCell = 0;
        }
        else
        {
            pItem->isAssigned = true;
            unassignedItemCount--;
            pItem->cellCount = calc_cell_count(pItem->pAst);
            pItem->blockIndex = currBlock;
            pItem->cellIndex = currCell;

            currBlock += pItem->cellCount / kCellsPerBlock;
            currCell += pItem->cellCount % kCellsPerBlock;

            ASSERT(currCell <= kCellsPerBlock);
            if (currCell >= kCellsPerBlock)
            {
                currBlock++;
                currCell = 0;
            }
        }
    }

    pBlockInfos->blockCount = currCell == 0 ? currBlock : currBlock + 1;;
}

BlockInfos * block_pack_props_and_fields(Ast *pAst)
{
    ASSERT(pAst->type == kAST_ComponentDef ||
           pAst->type == kAST_EntityDef);

    u32 count = props_and_fields_count(pAst);

    BlockInfos * pBlockInfos = COMP_NEW(BlockInfos);

    if (count > 0)
    {
        pBlockInfos->items.reserve(count);

        for (Ast *pChild : pAst->pChildren->nodes)
            if (is_prop_or_field(pChild->pSymRec))
                pBlockInfos->items.emplace_back(pChild);

        block_pack_items(pBlockInfos, false);
    }

    return pBlockInfos;
}

BlockInfos * block_pack_message_params(Ast * pAst)
{
    ASSERT(pAst->type == kAST_MessageSend);

    Ast * pParams = pAst->pRhs;
    ASSERT(pParams->type == kAST_FunctionParams);

    u32 count = (u32)pParams->pChildren->nodes.size();

    BlockInfos * pBlockInfos = COMP_NEW(BlockInfos);

    if (count > 0)
    {
        pBlockInfos->items.reserve(count);

        for (Ast *pChild : pParams->pChildren->nodes)
            pBlockInfos->items.emplace_back(pChild);

        block_pack_items(pBlockInfos, true);
    }

    return pBlockInfos;
}

u32 param_count(const CompList<SymRec*> & symRecs)
{
    u32 count = 0;
    for (SymRec * pSymRec : symRecs)
    {
        if (pSymRec->type == kSYMT_Param)
            ++count;
    }
    return count;
}

BlockInfos * block_pack_message_def_params(SymTab * pSymTab, ParseData * pParseData)
{
    u32 count = (u32)param_count(pSymTab->orderedSymRecs);

    BlockInfos * pBlockInfos = COMP_NEW(BlockInfos);

    if (count > 0)
    {
        pBlockInfos->items.reserve(count);

        for (SymRec * pSymRec : pSymTab->orderedSymRecs)
        {
            if (pSymRec->type == kSYMT_Param && !(pSymRec->flags & kSRFL_Member))
            {
                // Till now, parsing is ambiguous as to the param list.
                // Now we know this is a message param, so set it as such.
                // We have to know if it's a message param since it will have
                // to be pulled out of the message blocks when referenced in
                // the message definition.
                pSymRec->type = kSYMT_MessageParam;
                Ast * pAst = ast_create(kAST_SymbolDecl, pParseData);
                pAst->pSymRec = pSymRec;
                pAst->str = pAst->pSymRec->name;
                pAst->pBlockInfos = pBlockInfos;
                pSymRec->pAst = pAst;

                pBlockInfos->items.emplace_back(pAst);
            }
        }
        block_pack_items(pBlockInfos, true);
    }

    return pBlockInfos;
}


} // namespace gaen


