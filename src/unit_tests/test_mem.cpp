//------------------------------------------------------------------------------
// test_mem.cpp - Tests for core/mem memory manager
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#include <cstdio>
#include <list>

#include <gtest/gtest.h>

#include "core/mem.h"

#include "unit_tests/BaseFixture.h"

class TestClass
{
public:
    TestClass()
    {
        printf("Constructor called\n");
    }
    ~TestClass()
    {
        printf("Destructor called\n");
    }
};

TEST(MemoryManagementTest, NewDelete)
{
    TestClass * pX = new (GALLOC(gaen::kMEM_Model, sizeof(TestClass))) TestClass;
    TestClass * pXarr = new (GALLOC(gaen::kMEM_Engine, sizeof(TestClass)*5)) TestClass[5];

    GDELETE(pX);
    GDELETE(pXarr);
}

TEST(MemoryManagementTest, InitAndFin)
{
    gaen::MemMgr memMgr;

    EXPECT_FALSE(memMgr.isInit());
    memMgr.init("16:100,64:20");
    EXPECT_TRUE(memMgr.isInit());
    memMgr.fin();
    EXPECT_FALSE(memMgr.isInit());
}

TEST(MemoryManagementTest, TrackAllocator)
{
    // LORRTODO - fix once we have templated typedefs
    //GAEN_LIST(int) l;
}

TEST(MemoryManagementTest, MemInitStrs)
{
    // empty str
    EXPECT_EQ(0, gaen::parse_mem_init_str("", nullptr, 0));


    // valid, 1 entry
    EXPECT_EQ(1, gaen::parse_mem_init_str("16:100", nullptr, 0));

    // allocSize too small
    EXPECT_EQ(0, gaen::parse_mem_init_str("1:100", nullptr, 0));

    // allocSize not a multiple of 16
    EXPECT_EQ(0, gaen::parse_mem_init_str("17:100", nullptr, 0));

    // trailing comma, should fail
    EXPECT_EQ(0, gaen::parse_mem_init_str("16:100,", nullptr, 0));

    // no count, should fail
    EXPECT_EQ(0, gaen::parse_mem_init_str("16:", nullptr, 0));

    // single number, shoudl fail
    EXPECT_EQ(0, gaen::parse_mem_init_str("16", nullptr, 0));


    // valid, 5 entries
    EXPECT_EQ(5, gaen::parse_mem_init_str("16:100,64:100,256:100,1024:100,4096:100", nullptr, 0));

    // allocSize too small
    EXPECT_EQ(0, gaen::parse_mem_init_str("16:100,8:100,256:100,1024:100,4096:100", nullptr, 0));

    // allocSize not a multiple of 16
    EXPECT_EQ(0, gaen::parse_mem_init_str("16:100,64:100,250:100,1024:100,4096:100", nullptr, 0));

    // trailing comma, should fail
    EXPECT_EQ(0, gaen::parse_mem_init_str("16:100,64:100,256:100,1024:100,4096:100,", nullptr, 0));

    // no count, should fail
    EXPECT_EQ(0, gaen::parse_mem_init_str("16:100,64:100,256:100,1024,4096:100", nullptr, 0));

    // missing colon, should fail
    EXPECT_EQ(0, gaen::parse_mem_init_str("16:100,64:100,256:100,1024100,4096:100", nullptr, 0));
}


