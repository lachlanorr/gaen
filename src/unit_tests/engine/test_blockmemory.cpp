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

#include "engine/CmpString.h"
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

TEST(BlockMemoryTest, CmpString)
{
    BlockMemory bm;

    // These tests assume this kBlockPerChunk
    EXPECT_EQ(63, Chunk::kBlocksPerChunk);

    u32 totalBlocks = bm.totalBlocks();
    u32 availableBlocks = bm.availableBlocks();

    EXPECT_EQ(bm.availableBlocks(), totalBlocks);

    CmpString str1 = bm.stringAlloc(kMaxStringLength);
    EXPECT_EQ(str1.size(), kMaxStringLength);
    EXPECT_EQ(str1.c_str()[kMaxStringLength], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63);

    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks);

    str1 = bm.stringAlloc(kMaxStringLength);
    EXPECT_EQ(str1.size(), kMaxStringLength);
    EXPECT_EQ(str1.c_str()[kMaxStringLength], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63);

    CmpString str2 = bm.stringAlloc(kMaxStringLength-kBlockSize);
    EXPECT_EQ(str2.size(), kMaxStringLength-kBlockSize);
    EXPECT_EQ(str2.c_str()[kMaxStringLength-kBlockSize], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63 - 62);

    CmpString str3 = bm.stringAlloc(kBlockSize-1);
    EXPECT_EQ(str3.size(), kBlockSize-1);
    EXPECT_EQ(str3.c_str()[kBlockSize-1], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63 - 62 - 1);

    bm.addRef(str1);
    bm.addRef(str2);
    bm.addRef(str3);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63 - 62 - 1);

    bm.release(str1);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 62 - 1);

    bm.release(str2);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 1);

    str1 = bm.stringAlloc(kMaxStringLength);
    EXPECT_EQ(str1.size(), kMaxStringLength);
    EXPECT_EQ(str1.c_str()[kMaxStringLength], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63 - 1);

    bm.release(str3);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks);
}

TEST(CmpStringTest, format_next_specifier)
{
    FormatSpecifier fs;
    const char * ret;
    const char * fmt;

    ret = find_next_specifier(&fs, nullptr);
    EXPECT_EQ(ret, nullptr);

    ret = find_next_specifier(&fs, "");
    EXPECT_EQ(ret, nullptr);


    fmt = "%y"; // invalid specifier 
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, nullptr);
    EXPECT_EQ(fs.begin, nullptr);
    EXPECT_EQ(fs.end, nullptr);
    EXPECT_EQ(fs.type, '\0');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);

    fmt = "abc%y"; // invalid specifier 
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, nullptr);
    EXPECT_EQ(fs.begin, nullptr);
    EXPECT_EQ(fs.end, nullptr);
    EXPECT_EQ(fs.type, '\0');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);

    fmt = "%ydef"; // invalid specifier 
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, nullptr);
    EXPECT_EQ(fs.begin, nullptr);
    EXPECT_EQ(fs.end, nullptr);
    EXPECT_EQ(fs.type, '\0');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);

    fmt = "abc%ydef"; // invalid specifier 
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, nullptr);
    EXPECT_EQ(fs.begin, nullptr);
    EXPECT_EQ(fs.end, nullptr);
    EXPECT_EQ(fs.type, '\0');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);

    
    fmt = "%%";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 2);
    EXPECT_EQ(fs.begin, fmt);
    EXPECT_EQ(fs.end, fmt + 2);
    EXPECT_EQ(fs.type, '%');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);

    fmt = "abc%%";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 5);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 5);
    EXPECT_EQ(fs.type, '%');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);

    fmt = "%%def";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 2);
    EXPECT_EQ(fs.begin, fmt);
    EXPECT_EQ(fs.end, fmt + 2);
    EXPECT_EQ(fs.type, '%');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);

    fmt = "abc%%def";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 5);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 5);
    EXPECT_EQ(fs.type, '%');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);




    fmt = "%d";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 2);
    EXPECT_EQ(fs.begin,     fmt);
    EXPECT_EQ(fs.end,       fmt + 2);
    EXPECT_EQ(fs.type,      'd');
    EXPECT_EQ(fs.width,      0);
    EXPECT_EQ(fs.precision,  0);
    EXPECT_EQ(fs.flagMinus,  0);
    EXPECT_EQ(fs.flagPlus,   0);
    EXPECT_EQ(fs.flagSpace,  0);
    EXPECT_EQ(fs.flagHash,   0);
    EXPECT_EQ(fs.flagZero,   0);

    fmt = "abc%d";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 5);
    EXPECT_EQ(fs.begin,     fmt + 3);
    EXPECT_EQ(fs.end,       fmt + 5);
    EXPECT_EQ(fs.type,      'd');
    EXPECT_EQ(fs.width,      0);
    EXPECT_EQ(fs.precision,  0);
    EXPECT_EQ(fs.flagMinus,  0);
    EXPECT_EQ(fs.flagPlus,   0);
    EXPECT_EQ(fs.flagSpace,  0);
    EXPECT_EQ(fs.flagHash,   0);
    EXPECT_EQ(fs.flagZero,   0);

    fmt = "%ddef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 2);
    EXPECT_EQ(fs.begin,     fmt);
    EXPECT_EQ(fs.end,       fmt + 2);
    EXPECT_EQ(fs.type,      'd');
    EXPECT_EQ(fs.width,      0);
    EXPECT_EQ(fs.precision,  0);
    EXPECT_EQ(fs.flagMinus,  0);
    EXPECT_EQ(fs.flagPlus,   0);
    EXPECT_EQ(fs.flagSpace,  0);
    EXPECT_EQ(fs.flagHash,   0);
    EXPECT_EQ(fs.flagZero,   0);

    fmt = "abc%ddef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 5);
    EXPECT_EQ(fs.begin,     fmt + 3);
    EXPECT_EQ(fs.end,       fmt + 5);
    EXPECT_EQ(fs.type,      'd');
    EXPECT_EQ(fs.width,      0);
    EXPECT_EQ(fs.precision,  0);
    EXPECT_EQ(fs.flagMinus,  0);
    EXPECT_EQ(fs.flagPlus,   0);
    EXPECT_EQ(fs.flagSpace,  0);
    EXPECT_EQ(fs.flagHash,   0);
    EXPECT_EQ(fs.flagZero,   0);


    fmt = "abc%0ddef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 6);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 6);
    EXPECT_EQ(fs.type, 'd');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 1);


    fmt = "abc%00ddef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 7);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 7);
    EXPECT_EQ(fs.type, 'd');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 1);

    fmt = "abc%+-00ddef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 9);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 9);
    EXPECT_EQ(fs.type, 'd');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 1);
    EXPECT_EQ(fs.flagPlus, 1);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 1);


    fmt = "abc%+-0 0ddef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 10);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 10);
    EXPECT_EQ(fs.type, 'd');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 1);
    EXPECT_EQ(fs.flagPlus, 1);
    EXPECT_EQ(fs.flagSpace, 1);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 1);

    fmt = "abc%#ddef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 6);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 6);
    EXPECT_EQ(fs.type, 'd');
    EXPECT_EQ(fs.width, 0);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 1);
    EXPECT_EQ(fs.flagZero, 0);



    fmt = "abc%#04fdef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 8);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 8);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 4);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 1);
    EXPECT_EQ(fs.flagZero, 1);

    fmt = "abc%0127fdef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 9);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 9);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 127);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 1);


    fmt = "abc%128.fdef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 9);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 9);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 128);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);

    fmt = "abc%#0127.034fdef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 14);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 14);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 127);
    EXPECT_EQ(fs.precision, 34);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 1);
    EXPECT_EQ(fs.flagZero, 1);

    fmt = "abc%127.34fdef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 11);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 11);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 127);
    EXPECT_EQ(fs.precision, 34);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);

    fmt = "abc%129.34fdef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 11);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 11);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 128);
    EXPECT_EQ(fs.precision, 34);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);

    fmt = "abc%129.1402391fdef";
    ret = find_next_specifier(&fs, fmt);
    EXPECT_EQ(ret, fmt + 16);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 16);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 128);
    EXPECT_EQ(fs.precision, 128);
    EXPECT_EQ(fs.flagMinus, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);

}

