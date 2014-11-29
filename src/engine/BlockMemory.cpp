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

BlockData * BlockData::from_string(BlockString * pString)
{
    BlockData * pBd = reinterpret_cast<BlockData*>(reinterpret_cast<u16*>(pString) - 1);
    ASSERT(pBd->type == kBKTY_String);
    return pBd;
}

u16 BlockData::validate_block_data(const Block * pBlock, BlockType type)
{
    const BlockData * pBlockData = reinterpret_cast<const BlockData*>(pBlock);
    if (pBlockData->type != type ||
        !pBlockData->isAllocated ||
        !pBlockData->isInitial ||
        pBlockData->refCount != 0)
        return 0;
    if (pBlockData->type == kBKTY_String &&
        pBlockData->blockCount > Chunk::kBlocksPerChunk)
        return 0;

    return pBlockData->blockCount;
}


Chunk::Chunk(u8 chunkIdx)
{
    mHeader.magic = kMagic;
    mHeader.freeCount = kBlocksPerChunk;
    mHeader.chunkIdx = chunkIdx;
    mHeader.needsCollection = false;

    for (u32 i = 0; i < kBlocksPerChunk; ++i)
        mBlocks[i].init();
}

u8 Chunk::alloc(u8 blockCount)
{
    mHeader.needsCollection = true;

    ASSERT(blockCount <= kBlocksPerChunk);
    if (blockCount > mHeader.freeCount)
        return Address::kInvalidIdx;

    u32 foundBlocks = 0;
    for (u16 idx = 0; idx < kBlocksPerChunk; ++idx)
    {
        BlockData & bd = mBlocks[idx];
        if (!bd.isAllocated)
        {
            foundBlocks++;
        }
        else
        {
            foundBlocks = 0;
            idx += bd.blockCount - 1; // for loop will add the 1 back
            continue;
        }

        if (foundBlocks >= blockCount)
        {
            ASSERT(foundBlocks == blockCount);
            ASSERT(foundBlocks <= (u16)(idx+1));
            u8 startIdx = idx - foundBlocks + 1;

            mBlocks[startIdx].isInitial = 1;
            mBlocks[startIdx].blockCount = blockCount;
            for (u32 i = 0; i < foundBlocks; ++i)
            {
                mBlocks[startIdx+i].isAllocated = 1;
            }
            mHeader.freeCount -= blockCount;
            return startIdx;
        }
    }

    return Address::kInvalidIdx;
}

void Chunk::free(u8 blockIdx)
{
    ASSERT(blockIdx < kBlocksPerChunk);
    BlockData & bd = mBlocks[blockIdx];
    ASSERT(bd.isAllocated && bd.isInitial);
    ASSERT(bd.blockCount <= (kBlocksPerChunk - blockIdx));
    u32 blockCount = bd.blockCount;
    bd.isInitial = 0; // to pass assert below
    for (u32 i = 0; i < blockCount; ++i)
    {
        mBlocks[blockIdx+i].init();
    }
    mHeader.freeCount += blockCount;
}

Chunk * Chunk::from_block_data(BlockData * pBd)
{
    // This math works because we've aligned the Chunk to
    // sizeof(Chunk) when allocating.
    Chunk * pChunk = (Chunk*)((std::intptr_t)pBd - ((std::intptr_t)pBd % sizeof(Chunk)));
    ASSERT(pChunk->mHeader.magic == kMagic); // sanity check our magic number
    return pChunk;
}

u8 Chunk::indexFromBlockData(BlockData * pBd)
{
    ASSERT(pBd >= &mBlocks[0] && pBd <= &mBlocks[kBlocksPerChunk-1]);
    return (u8)(pBd - &mBlocks[0]);
}

void Chunk::addRef(u8 blockIdx)
{
    BlockData & bd = blockData(blockIdx);
    ASSERT(bd.refCount < BlockData::kMaxRefCount);
    bd.refCount++;
}

void Chunk::release(u8 blockIdx)
{
    BlockData & bd = blockData(blockIdx);
    ASSERT(bd.refCount > 0);
    bd.refCount--;
    if (bd.refCount == 0)
        mHeader.needsCollection = true;
}

void Chunk::collect()
{
    if (mHeader.needsCollection)
    {
        for (u32 blockIdx = 0; blockIdx < kBlocksPerChunk; ++blockIdx)
        {
            BlockData & bd = mBlocks[blockIdx];
            if (bd.isInitial && bd.isAllocated && bd.refCount == 0)
                this->free(blockIdx);
        }
        mHeader.needsCollection = false;
    }
}

u32 Chunk::availableBlocks()
{
    return mHeader.freeCount;
}

u32 Chunk::usedBlocks()
{
    return totalBlocks() - mHeader.freeCount;
}

u32 Chunk::totalBlocks()
{
    return kBlocksPerChunk;
}


//------------------------------------------------------------------------------

BlockMemory::BlockMemory()
{
    memset(mChunks, 0, sizeof(mChunks));
    mNeedsCollection = false;
}

BlockMemory::~BlockMemory()
{
    for (u32 chunkIdx = 0; chunkIdx < kChunkCount; ++chunkIdx)
    {
        if (mChunks[chunkIdx])
        {
            GDELETE(mChunks[chunkIdx]);
            mChunks[chunkIdx] = nullptr;
        }
    }
}

CmpString BlockMemory::stringAlloc(u16 charCount)
{
    u16 charCountWithNull = charCount + 1;

    u8 blockCount = 1;

    if (charCountWithNull > kCharsInFirstBlock)
    {
        u16 charCountNextBlocks = charCountWithNull - kCharsInFirstBlock;
        blockCount += (u8)(charCountNextBlocks / kBlockSize + (charCountNextBlocks % kBlockSize ? 1 : 0));
    }

    Address addr = alloc(blockCount);

    if (addr.blockIdx != Address::kInvalidIdx)
    {
        BlockData & bd = blockData(addr);
        bd.type = kBKTY_String;
        bd.data.string.charCount = charCount;
        bd.data.string.chars[charCount] = '\0';
        return CmpString(&bd);
    }
    else
    {
        PANIC("Failed to allocate string of size %d", charCount);
        return CmpString(nullptr);
    }
}

CmpString BlockMemory::stringAlloc(const char * val)
{
    if (!val) // never allow nulls, just empty
    {
        return stringAlloc((u16)0);
    }
    else
    {
        CmpString str = stringAlloc((u16)strlen(val));
        strcpy(str.c_str(), val);
        return str;
    }
}

CmpString BlockMemory::stringFormat(const char* format, ...)
{
    static thread_local char scratch[kMaxCmpStringLength+1];

    va_list ap;

    va_start(ap, format);
    int count = vsnprintf(scratch, kMaxCmpStringLength, format, ap);
    va_end(ap);

    if (count < 0 || count > kMaxCmpStringLength)
    {
        PANIC("Badly formed format string, or too long to format");
        return CmpString(nullptr);
    }

    // we succeeded in formatting the string
    scratch[count] = '\0';

    CmpString str = stringAlloc(count);
    strcpy(str.c_str(), scratch);
    str.c_str()[count] = '\0';

    return str;
}

CmpString BlockMemory::string(Address & addr)
{
    BlockData & bd = blockData(addr);
    return CmpString(&bd);
}

Address BlockMemory::allocCopy(const BlockData * pSrc)
{
    Address addr = alloc(pSrc->blockCount);
    BlockData & bd = blockData(addr);

    BlockData * pDst = &bd;
    for (u32 blockIdx = 0; blockIdx < pSrc->blockCount; ++blockIdx)
        pDst[blockIdx] = pSrc[blockIdx];

    return addr;
}

Address BlockMemory::alloc(u8 blockCount)
{
    mNeedsCollection = true;
    u8 firstEmptyChunk = Address::kInvalidIdx;

    ASSERT(blockCount > 0 && blockCount <= Chunk::kBlocksPerChunk);

    for (u32 chunkIdx = 0; chunkIdx < kChunkCount; ++chunkIdx)
    {
        Chunk * pChunk = mChunks[chunkIdx];
        if (!pChunk)
        {
            if (firstEmptyChunk == Address::kInvalidIdx)
            {
                // save this for later if we fail to
                // find any available space in non-empty
                // chunks
                firstEmptyChunk = chunkIdx;
            }
        }
        else
        {
            u8 blockIdx = pChunk->alloc(blockCount);
            if (blockIdx != Address::kInvalidIdx)
            {
                return Address(chunkIdx, blockIdx);
            }
        }
    }

    // no available space in allocated block chuncks,
    // see if we can allocate a new one
    if (firstEmptyChunk != Address::kInvalidIdx)
    {
        ASSERT(firstEmptyChunk < kChunkCount && !mChunks[firstEmptyChunk]);

        // Alloc aligned to size of Chunk so we can get to start of
        // chunk easily from a block within the chunk without having
        // to store chunk pointer or index at every block.
        mChunks[firstEmptyChunk] = GNEW_ALIGNED(kMEM_Engine, Chunk, sizeof(Chunk), firstEmptyChunk);

        u8 blockIdx = mChunks[firstEmptyChunk]->alloc(blockCount);
        ASSERT(blockIdx != Address::kInvalidIdx);
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

void BlockMemory::addRef(Address addr)
{
    ASSERT(mChunks[addr.chunkIdx]);
    mChunks[addr.chunkIdx]->addRef(addr.blockIdx);
}

void BlockMemory::release(Address addr)
{
    ASSERT(mChunks[addr.chunkIdx]);
    mChunks[addr.chunkIdx]->release(addr.blockIdx);
    mNeedsCollection |= mChunks[addr.chunkIdx]->needsCollection();
}

void BlockMemory::addRef(const CmpString & str)
{
    BlockData * pBd = str.mpBlockData;
    Chunk * pChunk = Chunk::from_block_data(pBd);
    u8 blockIdx = pChunk->indexFromBlockData(pBd);

    addRef(Address(pChunk->chunkIdx(), blockIdx));
}

void BlockMemory::release(const CmpString & str)
{
    BlockData * pBd = str.mpBlockData;
    Chunk * pChunk = Chunk::from_block_data(pBd);
    u8 blockIdx = pChunk->indexFromBlockData(pBd);

    release(Address(pChunk->chunkIdx(), blockIdx));
}

void BlockMemory::collect()
{
    if (mNeedsCollection)
    {
        for (u32 chunkIdx = 0; chunkIdx < kChunkCount; ++chunkIdx)
        {
            Chunk * pChunk = mChunks[chunkIdx];
            if (pChunk)
            {
                pChunk->collect();
            }
        }
        mNeedsCollection = false;
    }
}

u32 BlockMemory::availableBlocks()
{
    u32 available = 0;
    for (u32 chunkIdx = 0; chunkIdx < kChunkCount; ++chunkIdx)
    {
        Chunk * pChunk = mChunks[chunkIdx];
        if (!pChunk)
            available += Chunk::kBlocksPerChunk;
        else
            available += pChunk->availableBlocks();
    }
    return available;
}

u32 BlockMemory::usedBlocks()
{
    return totalBlocks() - availableBlocks();
}

u32 BlockMemory::totalBlocks()
{
    return kChunkCount * Chunk::kBlocksPerChunk;
}

BlockData & BlockMemory::blockData(Address & addr)
{
    ASSERT(addr.chunkIdx < kChunkCount && addr.blockIdx < Chunk::kBlocksPerChunk);
    Chunk * pChunk = mChunks[addr.chunkIdx];
    ASSERT(pChunk);
    return pChunk->blockData(addr.blockIdx);
}

} // namespace gaen
