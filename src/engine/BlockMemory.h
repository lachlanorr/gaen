//------------------------------------------------------------------------------
// BlockMemory.h - Simple memory manager for entity dynamic data types
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

#ifndef GAEN_ENGINE_BLOCKMEMORY_H
#define GAEN_ENGINE_BLOCKMEMORY_H

#include "core/base_defines.h"
#include "engine/Block.h"

namespace gaen
{

enum BlockType
{
    kBKTY_String,

    kBKTY_COUNT
};

// 12 characters left after string header data in first BlockData
static const u32 kCharsInFirstBlock = 12;

struct String
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
    u16 type:4;
    u16 blockCount:6;  // changing this affects kBlocksPerChunk
    u16 isAllocated:1;
    u16 isInitial:1;   // is the initial block of an allocated series of blocks
    u16 PADDING:4;

    union
    {
        String string;
      
        // add additional types here, like "list" and "dict"
    } data;

    void init();
};

static_assert(sizeof(BlockData) == sizeof(Block), "BlockData must be same size as a Block");
static_assert(sizeof(BlockData) == 16, "BlockData must be same size as a Block");

//------------------------------------------------------------------------------

struct ChunkHeader
{
    u8 freeCount;
    u8 chunkIdx;
    u8 PADDING[14];
};

static_assert(sizeof(ChunkHeader) == sizeof(Block), "ChunkHeader must be same size as a Block");

class Chunk
{
public:
    static const u32 kBlocksPerChunk = 63; // changing this affects BlockData::blockCount

    Chunk(u8 chunkIdx);

    // Allocate and return u16 block data index.
    // Returns Address::kInvalidIdx if no space.
    u8 alloc(u8 blockCount);
    void free(u8 blockIdx);

    BlockData & blockData(u8 blockIdx) { return mBlocks[blockIdx]; }
private:
    ChunkHeader mHeader;
    BlockData mBlocks[kBlocksPerChunk];
};

// max string is length of chunk
static const u32 kMaxStringLength = (Chunk::kBlocksPerChunk - 1) * kBlockSize + kCharsInFirstBlock - 1; // -1 for null terminator

//------------------------------------------------------------------------------

struct Address
{
    static const u8 kInvalidIdx = 255;

    u8 chunkIdx;
    u8 blockIdx;

    Address()
      : chunkIdx(kInvalidIdx)
      , blockIdx(kInvalidIdx) {}

    Address(u8 chunkIdx, u8 blockIdx)
      : chunkIdx(chunkIdx)
      , blockIdx(blockIdx) {}

};

class BlockMemory
{
public:
    static const u32 kChunkCount = 16;

    BlockMemory();
    ~BlockMemory();

    String * allocString(u16 charCount);
    
    Address alloc(u8 blockCount);
    void free(Address addr);
private:
    BlockData & blockData(Address & addr);

    Chunk * mChunks[kChunkCount];
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_BLOCKMEMORY_H
