//------------------------------------------------------------------------------
// test_ringbuffers.cpp - Test core/multi_queues
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

#include "core/threading.h"
#include "core/SpscRingBuffer.h"

TEST(RingBuffers, SpscRingBuffer)
{
    using namespace gaen;

    struct Data
    {
        i32 i;
        f32 f;
        u64 u;
    };

    init_threading(1);

    const size_t kQSize = 4;

    SpscRingBuffer<Data> q(kQSize, kMEM_Unspecified);

    // Try to pop... should be nothing
    SpscRingBuffer<Data>::Accessor accPop1;
    q.popBegin(&accPop1);
    EXPECT_EQ(accPop1.available(), 0);

    SpscRingBuffer<Data>::Accessor accPush1;
    q.pushBegin(&accPush1, 2);
    EXPECT_EQ(accPush1.available(), kQSize);

    Data & d0 = accPush1[0];
    Data & d1 = accPush1[1];
    Data & d2 = accPush1[2];
    Data & d3 = accPush1[3];

    accPush1[0].i = 123;
    accPush1[0].f = 1.23f;
    accPush1[0].u = 123123;

    accPush1[1].i = 234;
    accPush1[1].f = 2.34f;
    accPush1[1].u = 234234;
    q.pushCommit(2);

    
    SpscRingBuffer<Data>::Accessor accPop2;
    q.popBegin(&accPop2);
    EXPECT_EQ(accPop2.available(), 2);

    Data & dd0 = accPush1[0];
    Data & dd1 = accPush1[1];

    EXPECT_EQ(dd0.i, 123);
    EXPECT_EQ(dd0.f, 1.23f);
    EXPECT_EQ(dd0.u, 123123);

    EXPECT_EQ(dd1.i, 234);
    EXPECT_EQ(dd1.f, 2.34f);
    EXPECT_EQ(dd1.u, 234234);

    q.popCommit(1);


    SpscRingBuffer<Data>::Accessor accPush2;
    q.pushBegin(&accPush2, 2);
    EXPECT_EQ(accPush2.available(), kQSize - 1);

    Data & ddd2 = accPush2[0];
    Data & ddd3 = accPush2[1];
    Data & ddd0 = accPush2[2];


    SpscRingBuffer<Data>::Accessor accPop3;
    q.popBegin(&accPop3);
    EXPECT_EQ(accPop3.available(), 1);

    Data & dddd1 = accPop3[0];

    EXPECT_EQ(dddd1.i, 234);
    EXPECT_EQ(dddd1.f, 2.34f);
    EXPECT_EQ(dddd1.u, 234234);

    q.popCommit(1);


    SpscRingBuffer<Data>::Accessor accPop4;
    q.popBegin(&accPop4);
    EXPECT_EQ(accPop4.available(), 0);


    EXPECT_EQ(&d0, &dd0);
    EXPECT_EQ(&d0, &ddd0);
    EXPECT_EQ(&d1, &dd1);
    EXPECT_EQ(&d2, &ddd2);
    EXPECT_EQ(&d3, &ddd3);


}
