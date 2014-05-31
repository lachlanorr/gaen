//------------------------------------------------------------------------------
// PropsAndFields.cpp - Manage and codegen property and field access
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

#include "compose/PropsAndFields.h"

namespace gaen
{

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
        PANIC("Invalid dataType: %d", dataType);
        return 0;
    }
}

char * property_block_accessor(char * output, u32 outputSize, DataType dataType, u32 cellIndex)
{
    switch (dataType)
    {
    case kDT_int:
        snprintf(output, outputSize-1, "cells[%u].i", cellIndex);
    case kDT_uint:
        snprintf(output, outputSize-1, "cells[%u].u", cellIndex);
    case kDT_float:
        snprintf(output, outputSize-1, "cells[%u].f", cellIndex);
    case kDT_bool:
        snprintf(output, outputSize-1, "cells[%u].b", cellIndex);
    case kDT_char:
        snprintf(output, outputSize-1, "cells[%u].c", cellIndex);
    case kDT_color:
        snprintf(output, outputSize-1, "cells[%u].color", cellIndex);
    case kDT_vec3:
    case kDT_vec4:
    case kDT_mat3:
    case kDT_mat34:
    case kDT_mat4:
        ASSERT(cellIndex == 0);
        strncpy(output, "qcell", outputSize);
    default:
        PANIC("Invalid dataType: %d", dataType);
        return "";
    }
    output[outputSize-1] = '\0'; // ensure null terminated
    return output;
}


PropsAndFields::PropsAndFields(u32 initSize)
  : mBlockCount(0)
{
    mItems.reserve(initSize);
    mSortedItems.reserve(initSize);
}


void PropsAndFields::addField(const char * name, DataType dataType)
{
    ASSERT(mItemMap.find(name) != mItemMap.end());

    mItems.emplace_back(Item(name, kITMT_Field, dataType));
    mSortedItems.push_back(&mItems.back());
}

void PropsAndFields::addProperty(const char * name, DataType dataType)
{
    ASSERT(mItemMap.find(name) != mItemMap.end());

    mItems.emplace_back(Item(name, kITMT_Property, dataType));
    mSortedItems.push_back(&mItems.back());
}

PropsAndFields::Item * PropsAndFields::findNextItemToPack(u32 currCell)
{
//    u32 cellsRemaining = kCellsPerBlock - currCell;
    
    for (Item * pItem : mSortedItems)
    {
        
    }
    return nullptr;
}

void PropsAndFields::blockPackItems()
{
    if (mSortedItems.size() == 0)
        return;

    for (Item * pItem : mSortedItems)
    {
        pItem->blockIndex = 0;
        pItem->cellIndex = 0;
        pItem->isAssigned = false;
    }
    
    std::sort(mSortedItems.begin(),
              mSortedItems.end(),
              [](const Item* a, const Item*b)
              { return a->cellCount < b->cellCount; });

    u32 unassignedItemCount = (u32)mSortedItems.size();

    mBlockCount = 1;

    u32 currBlock = 0;
    u32 currCell = 0;

    while (unassignedItemCount > 0)
    {
        
    }
    
}

u32 PropsAndFields::blockCount()
{
    return mBlockCount;
}

const PropsAndFields::Item & PropsAndFields::operator[](size_t i)
{
    ASSERT(i < mItems.size());
    return mItems[i];
}

PropsAndFields::Item::Item(const char * name, ItemType type, DataType dataType)
  : name(name)
  , type(type)
  , dataType(dataType)
  , blockIndex(0)
  , cellIndex(0)
{
    cellCount = data_type_cell_count(dataType);
}

} // namespace gaen
