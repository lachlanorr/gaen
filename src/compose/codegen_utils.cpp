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

#include "engine/Block.h"
#include "compose/codegen_utils.h"

namespace gaen
{

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


u32 data_type_cell_count(DataType dataType)
{
    switch (dataType)
    {
    case kDT_int:
    case kDT_uint:
    case kDT_float:
    case kDT_bool:
    case kDT_char:
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
    default:
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

static Ast * find_next_fit(const CompVector<Ast*> & items,
                           u32 currCell)
{
    ASSERT(currCell < kCellsPerBlock);

    u32 cellsRemaining = kCellsPerBlock - currCell;
    
    for (Ast * pItem : items)
    {
        if (pItem->pSymRec->isAssigned)
            continue;

        if (pItem->pSymRec->cellCount > 1 && currCell != 0)
            continue;

        return pItem;
    }
    return nullptr;
}



void block_pack_props_and_fields(Ast *pAst)
{
    u32 count = props_and_fields_count(pAst);

    if (count == 0)
    {
        pAst->pScope->pSymTab->blockCount = 0;
    }

    CompVector<Ast*> items;
    items.reserve(count);

    for (Ast *pChild : pAst->pChildren->nodes)
    {
        if (is_prop_or_field(pChild->pSymRec))
        {
            ASSERT(pChild->pSymRec->blockIndex == 0 &&
                   pChild->pSymRec->cellIndex == 0 &&
                   pChild->pSymRec->cellCount == data_type_cell_count(pChild->pSymRec->dataType) &&
                   pChild->pSymRec->isAssigned == false);

            items.push_back(pChild);
        }
    }

    // items vector now holds all props and fields Ast*'s

    std::sort(items.begin(),
              items.end(),
              [](const Ast* a, const Ast* b)
              { return a->pSymRec->cellCount < b->pSymRec->cellCount; });

    u32 unassignedItemCount = count;

    u32 currBlock = 0;
    u32 currCell = 0;

    while (unassignedItemCount > 0)
    {
        Ast * pItem = find_next_fit(items, currCell);

        if (!pItem)
        {
            // no more items will fit in this block
            currBlock++;
            currCell = 0;
        }
        else
        {
            pItem->pSymRec->isAssigned = true;
            unassignedItemCount--;
            pItem->pSymRec->blockIndex = currBlock;
            pItem->pSymRec->cellIndex = currCell;

            currBlock += pItem->pSymRec->cellCount / kCellsPerBlock;
            currCell += pItem->pSymRec->cellCount % kCellsPerBlock;

            ASSERT(currCell <= kCellsPerBlock);
            if (currCell >= kCellsPerBlock)
            {
                currBlock++;
                currCell = 0;
            }
        }
    }

    pAst->pScope->pSymTab->blockCount = currCell == 0 ? currBlock : currBlock + 1;

}



} // namespace gaen


