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

#include "engine/BlockData.h"
#include "engine/CmpString.h"

namespace gaen
{

//------------------------------------------------------------------------------

struct ChunkHeader
{
    u32 magic;
    u8 freeCount;
    u8 chunkIdx;
    bool needsCollection;
    u8 PADDING[9];
};

static_assert(sizeof(ChunkHeader) == sizeof(Block), "ChunkHeader must be same size as a Block");

class Chunk
{
public:
    static const u32 kBlocksPerChunk = 63; // changing this affects BlockData::blockCount

    Chunk(u8 chunkIdx);

    u8 chunkIdx() { return mHeader.chunkIdx; }

    // Allocate and return u16 block data index.
    // Returns Address::kInvalidIdx if no space.
    u8 alloc(u8 blockCount);
    void free(u8 blockIdx);

    BlockData & blockData(u8 blockIdx)
    {
        ASSERT(blockIdx < kBlocksPerChunk);
        return mBlocks[blockIdx];
    }

    static Chunk * from_block_data(BlockData * pBd);
    u8 indexFromBlockData(BlockData * pBd);

    void addRef(u8 blockIdx);
    void release(u8 blockIdx);

    bool needsCollection() { return mHeader.needsCollection; }
    void collect();

    u32 availableBlocks();
    u32 usedBlocks();
    u32 totalBlocks();
private:
    static const u32 kMagic = 0xb70cb70c;

    ChunkHeader mHeader;
    BlockData mBlocks[kBlocksPerChunk];
};

// max string is length of chunk
static const u32 kMaxCmpStringLength = (Chunk::kBlocksPerChunk - 1) * kBlockSize + kCharsInFirstBlock - 1; // -1 for null terminator

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

//------------------------------------------------------------------------------

class BlockMemory
{
public:
    static const u32 kChunkCount = 16;

    BlockMemory();
    ~BlockMemory();

    CmpString stringAlloc(u16 charCount);
    CmpString stringAlloc(const char * val);
    CmpString stringFormat(const char* format, ...);
    CmpString string(Address & addr);

    template <typename T>
    CmpString stringReadMessage(const T & msgAcc, u32 startIndex, u32 blockMemoryIndex)
    {
        for (u32 i = 0; i < blockMemoryIndex; ++i)
        {
            const BlockData * pBd = reinterpret_cast<const BlockData*>(&msgAcc[startIndex]);
            startIndex += pBd->blockCount;
            ASSERT(startIndex < msgAcc.available());
        }

        ASSERT(CmpString::is_string_message(msgAcc, startIndex));
        const BlockData * pBd = reinterpret_cast<const BlockData*>(&msgAcc[startIndex]);

        Address addr = alloc(pBd->blockCount);
        BlockData * pBdDest = &mChunks[addr.chunkIdx]->blockData(addr.blockIdx);
        for (u32 i = 0; i < pBd->blockCount; ++i)
        {
            pBdDest[i] = *reinterpret_cast<const BlockData*>(&msgAcc[startIndex + i]);
        }

        return string(addr);
    }


    
    Address allocCopy(const BlockData * pSrc);
    Address alloc(u8 blockCount);
    void free(Address addr);

    void addRef(Address addr);
    void release(Address addr);
    void addRef(const CmpString & str);
    void release(const CmpString & str);

    void collect();

    u32 availableBlocks();
    u32 usedBlocks();
    u32 totalBlocks();
private:
    BlockData & blockData(Address & addr);

    bool mNeedsCollection;
    Chunk * mChunks[kChunkCount];
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_BLOCKMEMORY_H
