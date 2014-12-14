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
#include "compose/comp_string.h"

using namespace gaen;

TEST(BlockMemoryTest, AllocCollect)
{
    BlockMemory bm;

    // These tests assume this kBlockPerChunk
    EXPECT_EQ(63, kBlocksPerChunk);

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
    EXPECT_EQ(63, kBlocksPerChunk);

    u32 totalBlocks = bm.totalBlocks();

    EXPECT_EQ(bm.availableBlocks(), totalBlocks);

    CmpString str1 = bm.stringAlloc(kMaxCmpStringLength);
    EXPECT_EQ(str1.size(), kMaxCmpStringLength);
    EXPECT_EQ(str1.c_str()[kMaxCmpStringLength], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63);

    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks);

    str1 = bm.stringAlloc(kMaxCmpStringLength);
    EXPECT_EQ(str1.size(), kMaxCmpStringLength);
    EXPECT_EQ(str1.c_str()[kMaxCmpStringLength], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63);

    CmpString str2 = bm.stringAlloc(kMaxCmpStringLength-kBlockSize);
    EXPECT_EQ(str2.size(), kMaxCmpStringLength-kBlockSize);
    EXPECT_EQ(str2.c_str()[kMaxCmpStringLength-kBlockSize], '\0');
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

    str1 = bm.stringAlloc(kMaxCmpStringLength);
    EXPECT_EQ(str1.size(), kMaxCmpStringLength);
    EXPECT_EQ(str1.c_str()[kMaxCmpStringLength], '\0');
    EXPECT_EQ(bm.availableBlocks(), totalBlocks - 63 - 1);

    bm.release(str3);
    bm.collect();
    EXPECT_EQ(bm.availableBlocks(), totalBlocks);
}

TEST(CmpStringTest, format_next_specifier)
{
    FormatSpecifier fs;
    const char * fmt;

    fs = find_next_specifier(nullptr);
    EXPECT_EQ(fs.begin, nullptr);
    EXPECT_EQ(fs.end, nullptr);
    EXPECT_EQ(fs.type, '\0');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fs = find_next_specifier("");
    EXPECT_EQ(fs.begin, nullptr);
    EXPECT_EQ(fs.end, nullptr);
    EXPECT_EQ(fs.type, '\0');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);


    fmt = "%y"; // invalid specifier 
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, nullptr);
    EXPECT_EQ(fs.end, nullptr);
    EXPECT_EQ(fs.type, '\0');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%y"; // invalid specifier 
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, nullptr);
    EXPECT_EQ(fs.end, nullptr);
    EXPECT_EQ(fs.type, '\0');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "%ydef"; // invalid specifier 
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, nullptr);
    EXPECT_EQ(fs.end, nullptr);
    EXPECT_EQ(fs.type, '\0');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%ydef"; // invalid specifier 
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, nullptr);
    EXPECT_EQ(fs.end, nullptr);
    EXPECT_EQ(fs.type, '\0');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    
    fmt = "%%";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt);
    EXPECT_EQ(fs.end, fmt + 2);
    EXPECT_EQ(fs.type, '%');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%%";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 5);
    EXPECT_EQ(fs.type, '%');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "%%def";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt);
    EXPECT_EQ(fs.end, fmt + 2);
    EXPECT_EQ(fs.type, '%');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%%def";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 5);
    EXPECT_EQ(fs.type, '%');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);




    fmt = "%d";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin,     fmt);
    EXPECT_EQ(fs.end,       fmt + 2);
    EXPECT_EQ(fs.type,      'd');
    EXPECT_EQ(fs.width,      -1);
    EXPECT_EQ(fs.precision,  -1);
    EXPECT_EQ(fs.flagLeftJustify,  0);
    EXPECT_EQ(fs.flagPlus,   0);
    EXPECT_EQ(fs.flagSpace,  0);
    EXPECT_EQ(fs.flagHash,   0);
    EXPECT_EQ(fs.flagZero,   0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%d";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin,     fmt + 3);
    EXPECT_EQ(fs.end,       fmt + 5);
    EXPECT_EQ(fs.type,      'd');
    EXPECT_EQ(fs.width,      -1);
    EXPECT_EQ(fs.precision,  -1);
    EXPECT_EQ(fs.flagLeftJustify,  0);
    EXPECT_EQ(fs.flagPlus,   0);
    EXPECT_EQ(fs.flagSpace,  0);
    EXPECT_EQ(fs.flagHash,   0);
    EXPECT_EQ(fs.flagZero,   0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "%ddef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin,     fmt);
    EXPECT_EQ(fs.end,       fmt + 2);
    EXPECT_EQ(fs.type,      'd');
    EXPECT_EQ(fs.width,      -1);
    EXPECT_EQ(fs.precision,  -1);
    EXPECT_EQ(fs.flagLeftJustify,  0);
    EXPECT_EQ(fs.flagPlus,   0);
    EXPECT_EQ(fs.flagSpace,  0);
    EXPECT_EQ(fs.flagHash,   0);
    EXPECT_EQ(fs.flagZero,   0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%ddef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin,     fmt + 3);
    EXPECT_EQ(fs.end,       fmt + 5);
    EXPECT_EQ(fs.type,      'd');
    EXPECT_EQ(fs.width,      -1);
    EXPECT_EQ(fs.precision,  -1);
    EXPECT_EQ(fs.flagLeftJustify,  0);
    EXPECT_EQ(fs.flagPlus,   0);
    EXPECT_EQ(fs.flagSpace,  0);
    EXPECT_EQ(fs.flagHash,   0);
    EXPECT_EQ(fs.flagZero,   0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);


    fmt = "abc%0ddef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 6);
    EXPECT_EQ(fs.type, 'd');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 1);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);


    fmt = "abc%00ddef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 7);
    EXPECT_EQ(fs.type, 'd');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 1);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%+-00ddef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 9);
    EXPECT_EQ(fs.type, 'd');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 1);
    EXPECT_EQ(fs.flagPlus, 1);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 1);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);


    fmt = "abc%+-0 0ddef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 10);
    EXPECT_EQ(fs.type, 'd');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 1);
    EXPECT_EQ(fs.flagPlus, 1);
    EXPECT_EQ(fs.flagSpace, 1);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 1);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%#ddef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 6);
    EXPECT_EQ(fs.type, 'd');
    EXPECT_EQ(fs.width, -1);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 1);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);



    fmt = "abc%#04fdef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 8);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 4);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 1);
    EXPECT_EQ(fs.flagZero, 1);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%0127fdef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 9);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 127);
    EXPECT_EQ(fs.precision, -1);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 1);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);


    fmt = "abc%128.fdef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 9);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 128);
    EXPECT_EQ(fs.precision, 0);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%#0127.034fdef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 14);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 127);
    EXPECT_EQ(fs.precision, 34);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 1);
    EXPECT_EQ(fs.flagZero, 1);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%127.34fdef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 11);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 127);
    EXPECT_EQ(fs.precision, 34);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%129.34fdef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 11);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 128);
    EXPECT_EQ(fs.precision, 34);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

    fmt = "abc%129.1402391fdef";
    fs = find_next_specifier(fmt);
    EXPECT_EQ(fs.begin, fmt + 3);
    EXPECT_EQ(fs.end, fmt + 16);
    EXPECT_EQ(fs.type, 'f');
    EXPECT_EQ(fs.width, 128);
    EXPECT_EQ(fs.precision, 128);
    EXPECT_EQ(fs.flagLeftJustify, 0);
    EXPECT_EQ(fs.flagPlus, 0);
    EXPECT_EQ(fs.flagSpace, 0);
    EXPECT_EQ(fs.flagHash, 0);
    EXPECT_EQ(fs.flagZero, 0);
    EXPECT_EQ(fs.flagStarWidth, 0);
    EXPECT_EQ(fs.flagStarPrecision, 0);
    EXPECT_EQ(fs.size.totalWidth, -1);
    EXPECT_EQ(fs.size.spacePadding, -1);
    EXPECT_EQ(fs.size.zeroPadding, -1);

}

TEST(CmpStringTest, format_size_helpers)
{
    FormatSize * pSize;
    FormatSpecifier spec;

    // float_whole_size - measures size of whole number portion of
    // floating point value

    u32 size;
    size = float_whole_size(0.0f);

    size = float_whole_size(-0.0f);
    EXPECT_EQ(size, 1);

    size = float_whole_size(1.0f);
    EXPECT_EQ(size, 1);

    size = float_whole_size(-1.0f);
    EXPECT_EQ(size, 1);

    size = float_whole_size(10.f);
    EXPECT_EQ(size, 2);

    size = float_whole_size(-10.f);
    EXPECT_EQ(size, 2);

    size = float_whole_size(99.99999f);
    EXPECT_EQ(size, 2);

    size = float_whole_size(100.99999f);
    EXPECT_EQ(size, 3);

    size = float_whole_size(999.99999f);
    EXPECT_EQ(size, 4); // fp rounding error gets us here, should be 3

    size = float_whole_size(1000.f);
    EXPECT_EQ(size, 4);



    // specifier_max_size, i32

    spec = find_next_specifier("%d");
    pSize = specifier_max_size(&spec, (i32)0);
    EXPECT_EQ(pSize->totalWidth, 1);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%20d");
    pSize = specifier_max_size(&spec, (i32)0);
    EXPECT_EQ(pSize->totalWidth, 20);
    EXPECT_EQ(pSize->spacePadding, 19);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%20.10d");
    pSize = specifier_max_size(&spec, (i32)0);
    EXPECT_EQ(pSize->totalWidth, 20);
    EXPECT_EQ(pSize->spacePadding, 10);
    EXPECT_EQ(pSize->zeroPadding, 9);

    spec = find_next_specifier("%.10d");
    pSize = specifier_max_size(&spec, (i32)0);
    EXPECT_EQ(pSize->totalWidth, 10);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 9);

    spec = find_next_specifier("%d");
    pSize = specifier_max_size(&spec, (i32)200);
    EXPECT_EQ(pSize->totalWidth, 3);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%1d");
    pSize = specifier_max_size(&spec, (i32)200);
    EXPECT_EQ(pSize->totalWidth, 3);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%1d");
    pSize = specifier_max_size(&spec, (i32)123456789);
    EXPECT_EQ(pSize->totalWidth, 9);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%1d");
    pSize = specifier_max_size(&spec, (i32)-123456789);
    EXPECT_EQ(pSize->totalWidth, 10);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%+1d");
    pSize = specifier_max_size(&spec, (i32)123456789);
    EXPECT_EQ(pSize->totalWidth, 10);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%+1d");
    pSize = specifier_max_size(&spec, (i32)-123456789);
    EXPECT_EQ(pSize->totalWidth, 10);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%+.0d");
    pSize = specifier_max_size(&spec, 0);
    EXPECT_EQ(pSize->totalWidth, 1);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%.0d");
    pSize = specifier_max_size(&spec, 0);
    EXPECT_EQ(pSize->totalWidth, 0);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%2.0d");
    pSize = specifier_max_size(&spec, 0);
    EXPECT_EQ(pSize->totalWidth, 2);
    EXPECT_EQ(pSize->spacePadding, 2);
    EXPECT_EQ(pSize->zeroPadding, 0);


    // specifier_max_size, u32
    spec = find_next_specifier("%u");
    pSize = specifier_max_size(&spec, (u32)0);
    EXPECT_EQ(pSize->totalWidth, 1);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%20u");
    pSize = specifier_max_size(&spec, (u32)0);
    EXPECT_EQ(pSize->totalWidth, 20);
    EXPECT_EQ(pSize->spacePadding, 19);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%20.10u");
    pSize = specifier_max_size(&spec, (u32)0);
    EXPECT_EQ(pSize->totalWidth, 20);
    EXPECT_EQ(pSize->spacePadding, 10);
    EXPECT_EQ(pSize->zeroPadding, 9);

    spec = find_next_specifier("%.10u");
    pSize = specifier_max_size(&spec, (u32)0);
    EXPECT_EQ(pSize->totalWidth, 10);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 9);

    spec = find_next_specifier("%u");
    pSize = specifier_max_size(&spec, (u32)200);
    EXPECT_EQ(pSize->totalWidth, 3);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%1u");
    pSize = specifier_max_size(&spec, (u32)200);
    EXPECT_EQ(pSize->totalWidth, 3);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%1u");
    pSize = specifier_max_size(&spec, (u32)123456789);
    EXPECT_EQ(pSize->totalWidth, 9);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%+1u");
    pSize = specifier_max_size(&spec, (u32)123456789);
    EXPECT_EQ(pSize->totalWidth, 10);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%+.0u");
    pSize = specifier_max_size(&spec, 0);
    EXPECT_EQ(pSize->totalWidth, 1);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%.0u");
    pSize = specifier_max_size(&spec, 0);
    EXPECT_EQ(pSize->totalWidth, 0);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%2.0u");
    pSize = specifier_max_size(&spec, 0);
    EXPECT_EQ(pSize->totalWidth, 2);
    EXPECT_EQ(pSize->spacePadding, 2);
    EXPECT_EQ(pSize->zeroPadding, 0);



    // specifier_max_size, f32
    spec = find_next_specifier("%f");
    pSize = specifier_max_size(&spec, 0.0f);
    EXPECT_EQ(pSize->totalWidth, 8);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%f");
    pSize = specifier_max_size(&spec, -0.0f);
    EXPECT_EQ(pSize->totalWidth, 9);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%.1f");
    pSize = specifier_max_size(&spec, 0.0f);
    EXPECT_EQ(pSize->totalWidth, 3);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%.1f");
    pSize = specifier_max_size(&spec, -0.0f);
    EXPECT_EQ(pSize->totalWidth, 4);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%.0f");
    pSize = specifier_max_size(&spec, 1.23124f);
    EXPECT_EQ(pSize->totalWidth, 1);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%#.0f");
    pSize = specifier_max_size(&spec, 1.23124f);
    EXPECT_EQ(pSize->totalWidth, 2);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%#+.0f");
    pSize = specifier_max_size(&spec, 1.23124f);
    EXPECT_EQ(pSize->totalWidth, 3);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%#+.0f");
    pSize = specifier_max_size(&spec, 9.7f);
    EXPECT_EQ(pSize->totalWidth, 4); // should be rounded to 10.
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%.1f");
    pSize = specifier_max_size(&spec, 1.23124f);
    EXPECT_EQ(pSize->totalWidth, 3);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%10.0f");
    pSize = specifier_max_size(&spec, 1.23124f);
    EXPECT_EQ(pSize->totalWidth, 10);
    EXPECT_EQ(pSize->spacePadding, 9);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%20f");
    pSize = specifier_max_size(&spec, 0.0f);
    EXPECT_EQ(pSize->totalWidth, 20);
    EXPECT_EQ(pSize->spacePadding, 12);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%3.2f");
    pSize = specifier_max_size(&spec, 4.1234f);
    EXPECT_EQ(pSize->totalWidth, 4);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%3.2f");
    pSize = specifier_max_size(&spec, -4.1234f);
    EXPECT_EQ(pSize->totalWidth, 5);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%+3.2f");
    pSize = specifier_max_size(&spec, 4.1234f);
    EXPECT_EQ(pSize->totalWidth, 5);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%+3.2f");
    pSize = specifier_max_size(&spec, -4.1234f);
    EXPECT_EQ(pSize->totalWidth, 5);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%+3.10f");
    pSize = specifier_max_size(&spec, -4.1234f);
    EXPECT_EQ(pSize->totalWidth, 13);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%+3.10f");
    pSize = specifier_max_size(&spec, 4.1234f);
    EXPECT_EQ(pSize->totalWidth, 13);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);


    // %x
    spec = find_next_specifier("%x");
    pSize = specifier_max_size(&spec, 0x22222);
    EXPECT_EQ(pSize->totalWidth, 5);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%X");
    pSize = specifier_max_size(&spec, 0x22222);
    EXPECT_EQ(pSize->totalWidth, 5);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%.7X");
    pSize = specifier_max_size(&spec, 0x22222);
    EXPECT_EQ(pSize->totalWidth, 7);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 2);

    spec = find_next_specifier("%0.7X");
    pSize = specifier_max_size(&spec, 0x22222);
    EXPECT_EQ(pSize->totalWidth, 7);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 2);

    spec = find_next_specifier("%0X");
    pSize = specifier_max_size(&spec, 0x22222);
    EXPECT_EQ(pSize->totalWidth, 5);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%7X");
    pSize = specifier_max_size(&spec, 0x22222);
    EXPECT_EQ(pSize->totalWidth, 7);
    EXPECT_EQ(pSize->spacePadding, 2);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%#7X");
    pSize = specifier_max_size(&spec, 0xffff);
    EXPECT_EQ(pSize->totalWidth, 7);
    EXPECT_EQ(pSize->spacePadding, 1);
    EXPECT_EQ(pSize->zeroPadding, 0);

    spec = find_next_specifier("%#.7X");
    pSize = specifier_max_size(&spec, 0xffff);
    EXPECT_EQ(pSize->totalWidth, 9);
    EXPECT_EQ(pSize->spacePadding, 0);
    EXPECT_EQ(pSize->zeroPadding, 3);

}
