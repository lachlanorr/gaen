//------------------------------------------------------------------------------
// test_blockmemory.cpp - Tests for BlockMemory
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

#include <gtest/gtest.h>

#include "engine/BlockMemory.h"

using namespace gaen;

TEST(BlockMemoryTest, AllocCollect)
{
    BlockMemory bm;

    // These tests assume this kBlockPerChunk
    EXPECT_EQ(63, Chunk::kBlocksPerChunk);

    u32 totalBlocks = bm.totalBlocks();
    u32 availableBlocks = bm.availableBlocks();

    EXPECT_EQ(totalBlocks, availableBlocks);
    EXPECT_EQ(totalBlocks, 1008);

    Address addr1 = bm.alloc(62);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62);
    EXPECT_EQ(addr1.chunkIdx, 0);
    EXPECT_EQ(addr1.blockIdx, 0);

    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks);

    addr1 = bm.alloc(62);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62);
    EXPECT_EQ(addr1.chunkIdx, 0);
    EXPECT_EQ(addr1.blockIdx, 0);

    bm.addRef(addr1);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62);
    bm.addRef(addr1);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62);
    bm.release(addr1);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62);
    bm.release(addr1);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks);

    addr1 = bm.alloc(62);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62);
    EXPECT_EQ(addr1.chunkIdx, 0);
    EXPECT_EQ(addr1.blockIdx, 0);

    Address addr2 = bm.alloc(2);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62 - 2);
    EXPECT_EQ(addr2.chunkIdx, 1);
    EXPECT_EQ(addr2.blockIdx, 0);

    Address addr3 = bm.alloc(1);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62 - 2 - 1);
    EXPECT_EQ(addr3.chunkIdx, 0);
    EXPECT_EQ(addr3.blockIdx, 62);

    bm.free(addr3);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62 - 2);
    Address addr4 = bm.alloc(1);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62 - 2 - 1);
    EXPECT_EQ(addr4.chunkIdx, 0);
    EXPECT_EQ(addr4.blockIdx, 62);

    bm.free(addr1);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 2 - 1);
    Address addr5 = bm.alloc(30);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 2 - 1 - 30);
    EXPECT_EQ(addr5.chunkIdx, 0);
    EXPECT_EQ(addr5.blockIdx, 0);

    Address addr6 = bm.alloc(32);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 2 - 1 - 30 - 32);
    EXPECT_EQ(addr6.chunkIdx, 0);
    EXPECT_EQ(addr6.blockIdx, 30);

    Address addr7 = bm.alloc(62);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 2 - 1 - 30 - 32 - 62);
    EXPECT_EQ(addr7.chunkIdx, 2);
    EXPECT_EQ(addr7.blockIdx, 0);

    Address addr8 = bm.alloc(10);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 2 - 1 - 30 - 32 - 62 - 10);
    EXPECT_EQ(addr8.chunkIdx, 1);
    EXPECT_EQ(addr8.blockIdx, 2);

    Address addr9 = bm.alloc(10);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 2 - 1 - 30 - 32 - 62 - 10 - 10);
    EXPECT_EQ(addr9.chunkIdx, 1);
    EXPECT_EQ(addr9.blockIdx, 12);

    Address addr10 = bm.alloc(41);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 2 - 1 - 30 - 32 - 62 - 10 - 10 - 41);
    EXPECT_EQ(addr10.chunkIdx, 1);
    EXPECT_EQ(addr10.blockIdx, 22);

    bm.free(addr8);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 2 - 1 - 30 - 32 - 62 - 10 - 41);
    Address addr11 = bm.alloc(10);
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 2 - 1 - 30 - 32 - 62 - 10 - 10 - 41);
    EXPECT_EQ(addr11.chunkIdx, 1);
    EXPECT_EQ(addr11.blockIdx, 2);

    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks);
}

TEST(BlockMemoryTest, String)
{
    BlockMemory bm;

    // These tests assume this kBlockPerChunk
    EXPECT_EQ(63, Chunk::kBlocksPerChunk);

    u32 totalBlocks = bm.totalBlocks();
    u32 availableBlocks = bm.availableBlocks();

    EXPECT_EQ(bm.availableBlocks(), totalBlocks);

    String * str1 = bm.stringAlloc(kMaxStringLength);
    EXPECT_EQ(str1->charCount, kMaxStringLength);
    EXPECT_EQ(str1->chars[kMaxStringLength], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63);

    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks);

    str1 = bm.stringAlloc(kMaxStringLength);
    EXPECT_EQ(str1->charCount, kMaxStringLength);
    EXPECT_EQ(str1->chars[kMaxStringLength], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63);

    String * str2 = bm.stringAlloc(kMaxStringLength-kBlockSize);
    EXPECT_EQ(str2->charCount, kMaxStringLength-kBlockSize);
    EXPECT_EQ(str2->chars[kMaxStringLength-kBlockSize], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63 - 62);

    String * str3 = bm.stringAlloc(kBlockSize-1);
    EXPECT_EQ(str3->charCount, kBlockSize-1);
    EXPECT_EQ(str3->chars[kBlockSize-1], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63 - 62 - 1);

    bm.stringAddRef(str1);
    bm.stringAddRef(str2);
    bm.stringAddRef(str3);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63 - 62 - 1);

    bm.stringRelease(str1);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62 - 1);

    bm.stringRelease(str2);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 1);

    str1 = bm.stringAlloc(kMaxStringLength);
    EXPECT_EQ(str1->charCount, kMaxStringLength);
    EXPECT_EQ(str1->chars[kMaxStringLength], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63 - 1);

    bm.stringRelease(str3);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks);
}