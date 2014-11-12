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

TEST(BlockMemoryTest, Usage)
{
    BlockMemory bm;

    // These tests assume this kBlockPerChunk
    EXPECT_EQ(63, Chunk::kBlocksPerChunk);

    Address addr1 = bm.alloc(62);
    EXPECT_EQ(addr1.chunkIdx, 0);
    EXPECT_EQ(addr1.blockIdx, 0);

    Address addr2 = bm.alloc(2);
    EXPECT_EQ(addr2.chunkIdx, 1);
    EXPECT_EQ(addr2.blockIdx, 0);

    Address addr3 = bm.alloc(1);
    EXPECT_EQ(addr3.chunkIdx, 0);
    EXPECT_EQ(addr3.blockIdx, 62);

    bm.free(addr3);
    Address addr4 = bm.alloc(1);
    EXPECT_EQ(addr4.chunkIdx, 0);
    EXPECT_EQ(addr4.blockIdx, 62);

    bm.free(addr1);
    Address addr5 = bm.alloc(30);
    EXPECT_EQ(addr5.chunkIdx, 0);
    EXPECT_EQ(addr5.blockIdx, 0);

    Address addr6 = bm.alloc(32);
    EXPECT_EQ(addr6.chunkIdx, 0);
    EXPECT_EQ(addr6.blockIdx, 30);

    Address addr7 = bm.alloc(62);
    EXPECT_EQ(addr7.chunkIdx, 2);
    EXPECT_EQ(addr7.blockIdx, 0);

    Address addr8 = bm.alloc(10);
    EXPECT_EQ(addr8.chunkIdx, 1);
    EXPECT_EQ(addr8.blockIdx, 2);

    Address addr9 = bm.alloc(10);
    EXPECT_EQ(addr9.chunkIdx, 1);
    EXPECT_EQ(addr9.blockIdx, 12);

    Address addr10 = bm.alloc(41);
    EXPECT_EQ(addr10.chunkIdx, 1);
    EXPECT_EQ(addr10.blockIdx, 22);

    bm.free(addr8);
    Address addr11 = bm.alloc(10);
    EXPECT_EQ(addr11.chunkIdx, 1);
    EXPECT_EQ(addr11.blockIdx, 2);


}

