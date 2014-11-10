//------------------------------------------------------------------------------
// codegen_utils.cpp - Shared utilities used during code generation
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

#include <algorithm>

#include "core/HashMap.h"
#include "core/hashing.h"
#include "engine/Block.h"
#include "compose/codegen_utils.h"
#include "compose/system_api_meta.h"

// Define BlockInfo constructor here to avoid issues including this file from compiler_structs.h
BlockInfo::BlockInfo(Ast * pAst)
  : pAst(pAst)
  , blockIndex(0)
  , cellIndex(0)
  , cellCount(0)
  , isPayload(false)
  , isAssigned(false)
{
    cellCount = gaen::calc_cell_count(pAst);
}

namespace gaen
{

extern ApiSignature gApiSignatures[];
const ApiSignature * find_api(const char * name, ParseData * pParseData)
{
    static bool isInit = false;
    static HashMap<kMEM_Compose, u32, const ApiSignature*> apiMap;
    if (!isInit)
    {
        ApiSignature * pSig = &gApiSignatures[0];
        while (pSig->nameHash != 0)
        {
            apiMap[pSig->nameHash] = pSig;
            pSig++;
        }
        isInit = true;
    }

    u32 nameHash = gaen_hash(name);

    auto it = apiMap.find(nameHash);
    if (it == apiMap.end())
    {
        COMP_ERROR(pParseData, "Cannot find Compose system api: %u", nameHash);
        return nullptr;
    }
    return it->second;
}


bool is_update_message_def(const Ast * pAst)
{
    return (pAst->type == kAST_MessageDef &&
            pAst->pSymRec &&
            0 == strcmp(pAst->pSymRec->name, "update"));
}

const Ast * find_update_message_def(const Ast * pAst)
{
    for (Ast * pChild : pAst->pChildren->nodes)
    {
        if (is_update_message_def(pChild))
        {
            return pChild;
        }
    }
    return nullptr;
}

const Ast * find_component_members(const Ast * pAst)
{
    Ast * pCompMembers = nullptr;
    for (Ast * pChild : pAst->pChildren->nodes)
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
                COMP_ERROR(pAst->pParseData, "More than one components section defined in entity");
                return nullptr;
            }
        }
    }
    return pCompMembers;
}

u32 calc_cell_count(const Ast * pAst)
{
    if (pAst->pSymRec)
        return data_type_cell_count(pAst->pSymRec->dataType, pAst->pParseData);
    else if (pAst->type == kAST_Hash ||
             pAst->type == kAST_IntLiteral)
        return 1;
    parsedata_formatted_message(pAst->pParseData, kMSGT_Error, "Unable to calculate cell count for Ast type: %d", pAst->type);
    return -1;
}

u32 data_type_cell_count(DataType dataType, ParseData * pParseData)
{
    switch (RAW_DT(dataType))
    {
    case kDT_int:
    case kDT_uint:
    case kDT_float:
    case kDT_bool:
    case kDT_char:
    case kDT_color:
        return 1;
    case kDT_vec3:
        return 3;
    case kDT_vec4:
        return 4;
    case kDT_mat3:
        return 9;
    case kDT_mat34:
        return 12;
    case kDT_mat4:
        return 16;
    case kDT_handle:
        return 8;
    default:
        COMP_ERROR(pParseData, "Unknown cell count for datatype: %d", dataType);
        return 0;
    }
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
                                 u32 currCell)
{
    ASSERT(currCell < kCellsPerBlock);

    u32 cellsRemaining = kCellsPerBlock - currCell;
    
    for (BlockInfo & item : items)
    {
        if (item.isAssigned)
            continue;

        if (item.cellCount > 1 && currCell != 0)
            continue;

        return &item;
    }
    return nullptr;
}

void block_pack_items(BlockInfos * pBlockInfos, bool usePayload)
{
    std::stable_sort(pBlockInfos->items.begin(),
                     pBlockInfos->items.end(),
                     [](const BlockInfo & a, const BlockInfo & b)
                     { return a.cellCount > b.cellCount; });

    u32 unassignedItemCount = (u32)pBlockInfos->items.size();

    u32 currBlock = 0;
    u32 currCell = 0;

    if (usePayload)
    {
        // last item should have smallest cell count
        BlockInfo & bi = pBlockInfos->items[pBlockInfos->items.size()-1];
        if (bi.cellCount == 1)
        {
            bi.isAssigned = true;
            bi.isPayload = true;
            unassignedItemCount--;
        }
    }

    while (unassignedItemCount > 0)
    {
        BlockInfo * pItem = find_next_fit(pBlockInfos->items, currCell);

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
            pItem->cellCount = data_type_cell_count(pItem->pAst->pSymRec->dataType, pItem->pAst->pParseData);
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
            if (pSymRec->type == kSYMT_Param)
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


