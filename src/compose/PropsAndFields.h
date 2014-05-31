//------------------------------------------------------------------------------
// PropsAndFields.h - Manage and codegen property and field access
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

#ifndef GAEN_COMPOSE_PROPSANDFIELDS_H
#define GAEN_COMPOSE_PROPSANDFIELDS_H

#include "core/base_defines.h"
#include "compose/comp_mem.h"
#include "compose/compiler.h"

namespace gaen
{

u32 data_type_cell_count(DataType dataType);
char * property_block_accessor(char * output, u32 outputSize, DataType dataType, u32 cellIndex);

class PropsAndFields
{
public:
    enum ItemType
    {
        kITMT_Property = 0,
        kITMT_Field    = 1
    };

    struct Item
    {
        const char * name;
        ItemType type;
        DataType dataType;
        u8 blockIndex;
        u8 cellIndex;
        u8 cellCount;
        bool isAssigned;

        Item(const char * name, ItemType type, DataType dataType);
    };

    PropsAndFields(u32 initSize);
    
    void addField(const char * name, DataType dataType);
    void addProperty(const char * name, DataType dataType);

    void blockPackItems();

    u32 blockCount();

    const Item * findItem(const char * name);

    const Item & operator[](size_t i);

private:
    Item * findNextItemToPack(u32 currCell);

    CompVector<Item> mItems;
    CompVector<Item*> mSortedItems;
    CompHashMap<const char *, Item*> mItemMap;

    u32 mBlockCount;
};

} // namespace gaen

#endif // #ifndef GAEN_COMPOSE_PROPSANDFIELDS_H
