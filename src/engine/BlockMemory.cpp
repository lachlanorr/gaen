//------------------------------------------------------------------------------
// BlockMemory.cpp - Simple memory manager for entity dynamic data types
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

#include "engine/stdafx.h"

#include "core/mem.h"

#include "engine/BlockMemory.h"

namespace gaen
{

void BlockData::init()
{
    isAllocated = 0;
    isInitial = 0;
}

Chunk::Chunk(u8 chunkIdx)
{
    mHeader.freeCount = kBlocksPerChunk;
    mHeader.chunkIdx = chunkIdx;

    for (u32 i = 0; i < kBlocksPerChunk; ++i)
        mBlocks[i].init();
}

u8 Chunk::alloc(u8 blockCount)
{
    ASSERT(blockCount < kBlocksPerChunk);
    if (blockCount < mHeader.freeCount)
        return -1;

    u16 idx = 0;
    u32 foundBlocks = 0;
    while (idx < kBlocksPerChunk - blockCount)
    {
        BlockData & bd = mBlocks[idx];
        if (!bd.isAllocated)
        {
            foundBlocks++;
        }
        else
        {
            foundBlocks = 0;
        }

        if (foundBlocks >= blockCount)
        {
            ASSERT(foundBlocks == blockCount);
            ASSERT(foundBlocks <= (u16)(idx+1));
            u8 startIdx = idx - foundBlocks + 1;

            mBlocks[startIdx].isInitial = 1;
            for (u32 i = 0; i < foundBlocks; ++i)
            {
                mBlocks[i].isAllocated = 1;
            }
            return startIdx;
        }
    }

    return -1;
}

void Chunk::free(u8 blockIdx)
{
    ASSERT(blockIdx < kBlocksPerChunk);
    BlockData & bd = mBlocks[blockIdx];
    ASSERT(bd.isAllocated && bd.isInitial);
    bd.isInitial = 0; // to pass assert below
    for (u32 i = 0; i < bd.blockCount; ++i)
    {
        ASSERT(mBlocks[i].isAllocated && !mBlocks[i].isInitial);
        mBlocks[i].init();
    }
}

//------------------------------------------------------------------------------

BlockMemory::BlockMemory()
{
    memset(mChunks, 0, sizeof(mChunks));
    
}

Address BlockMemory::alloc(u8 blockCount)
{
    u32 firstEmptyChunk = -1;

    ASSERT(blockCount > 0 && blockCount <= Chunk::kBlocksPerChunk);

    for (u32 chunkIdx = 0; chunkIdx < kChunkCount; ++chunkIdx)
    {
        Chunk * pBc = mChunks[chunkIdx];
        if (!pBc)
        {
            if (firstEmptyChunk == -1)
            {
                // save this for later if we fail to
                // find any available space in non-empty
                // chunks
                firstEmptyChunk = chunkIdx;
            }
        }
        else
        {
            u8 blockIdx = pBc->alloc(blockCount);
            if (blockIdx != -1)
            {
                return Address(chunkIdx, blockIdx);
            }
        }
    }

    // no available space in allocated block chuncks,
    // see if we can allocate a new one
    if (firstEmptyChunk != -1)
    {
        ASSERT(firstEmptyChunk < kChunkCount && !mChunks[firstEmptyChunk]);
        mChunks[firstEmptyChunk] = GNEW(kMEM_Engine, Chunk, firstEmptyChunk);
        u8 blockIdx = mChunks[firstEmptyChunk]->alloc(blockCount);
        ASSERT(blockIdx != -1);
        return Address(firstEmptyChunk, blockIdx);
    }
    else
    {
        // totally out of memory
        PANIC("Out of BlockMemory");
        return Address();
    }
}

void BlockMemory::free(Address addr)
{
    ASSERT(addr.chunkIdx < kChunkCount && addr.blockIdx < Chunk::kBlocksPerChunk);
    Chunk * pChunk = mChunks[addr.chunkIdx];
    ASSERT(pChunk);
    pChunk->free(addr.blockIdx);
}

} // namespace gaen
