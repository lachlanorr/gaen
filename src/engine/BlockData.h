//------------------------------------------------------------------------------
// BlockData.h - Block structs used with BlockMemory
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

#ifndef GAEN_ENGINE_BLOCKDATA_H
#define GAEN_ENGINE_BLOCKDATA_H

#include "compose/compiler.h"
#include "engine/Block.h"

namespace gaen
{

enum BlockType
{
    kBKTY_Uninitialized = 0,
    kBKTY_String        = 1
};

// 12 characters left after string header data in first BlockData
static const u32 kCharsInFirstBlock = 12;
static const u32 kBlocksPerChunk = 63; // changing this affects BlockData::blockCount

inline const char * compose_type_to_block_type(DataType dt)
{
    switch (dt)
    {
    case kDT_string:
    case kDT_asset:
        return "kBKTY_String";
    default:
        PANIC("Invalid data type: %d", dt);
        return nullptr;
    }
}

struct BlockString
{
    u16 charCount;
    char chars[kCharsInFirstBlock];

    const char * c_str() const
    {
        ASSERT(chars[charCount-1] == '\0');
        return &chars[0];
    }
private:
};

struct BlockData
{
    static const u16 kMaxRefCount = 15;

    u16 type:4;
    u16 blockCount:6;  // changing this affects kBlocksPerChunk
    u16 isAllocated:1;
    u16 isInitial:1;   // is the initial block of an allocated series of blocks
    u16 refCount:4;

    union
    {
        BlockString string;
      
        // add additional types here, like "list" and "dict"
    } data;

    static BlockData * from_string(BlockString * pString);

    static u16 validate_block_data(const Block * pBlock, BlockType type);

    void init() { *reinterpret_cast<u16*>(this) = 0; }
};

static_assert(sizeof(BlockData) == sizeof(Block), "BlockData must be same size as a Block");
static_assert(sizeof(BlockData) == 16, "BlockData must be same size as a Block");


} // namespace gaen

#endif // #ifndef GAEN_ENGINE_BLOCKDATA_H
