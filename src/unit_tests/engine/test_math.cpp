//------------------------------------------------------------------------------
// test_math.cpp - Tests for core/mem memory manager
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

#include "engine/math.h"

using namespace gaen;

TEST(MathTest, Vec3)
{
    Vec3 v0(20.0f, 10.0f, 5.0f);

    f32 len = v0.length();
    EXPECT_TRUE(is_fp_eq(len, 22.9128780f));

    v0.normalize();
    len = v0.length();
    EXPECT_TRUE(is_fp_eq(len, 1.0f));

    EXPECT_TRUE(is_fp_eq(v0.x(), 0.87287158f));
    EXPECT_TRUE(is_fp_eq(v0.y(), 0.43643579f));
    EXPECT_TRUE(is_fp_eq(v0.z(), 0.21821789f));


    v0.x() = 20.0f;
    v0.y() = 10.0f;
    v0.z() = 5.0f;

    len = v0.length();
    EXPECT_TRUE(is_fp_eq(len,22.9128780f));

    v0.normalizeIfNecessary();
    len = v0.length();
    EXPECT_TRUE(is_fp_eq(len,1.0f));

    EXPECT_TRUE(is_fp_eq(v0.x(),0.87287158f));
    EXPECT_TRUE(is_fp_eq(v0.y(),0.43643579f));
    EXPECT_TRUE(is_fp_eq(v0.z(),0.21821789f));


    Vec3 v1(10.0f, -20.0f, 30.0f);
    Vec3 v2(2.0f, 3.0f, -4.0f);
    
    f32 dotResult = Vec3::dot(v1, v2);
    EXPECT_TRUE(is_fp_eq(dotResult, -160.0f));

    Vec3 crossResult = Vec3::cross(v1, v2);
    EXPECT_TRUE(is_fp_eq(crossResult.x(), -10.0f));
    EXPECT_TRUE(is_fp_eq(crossResult.y(), 100.0f));
    EXPECT_TRUE(is_fp_eq(crossResult.z(), 70.0f));

    
}

TEST(MathTest, Vec4)
{
    Vec4 v0(20.0f, 10.0f, 5.0f, 0.0f);

    f32 len = v0.length();
    EXPECT_TRUE(is_fp_eq(len, 22.9128780f));

    v0.normalize();
    len = v0.length();
    EXPECT_TRUE(is_fp_eq(len, 1.0f));

    EXPECT_TRUE(is_fp_eq(v0.x(), 0.87287158f));
    EXPECT_TRUE(is_fp_eq(v0.y(), 0.43643579f));
    EXPECT_TRUE(is_fp_eq(v0.z(), 0.21821789f));


    v0.x() = 20.0f;
    v0.y() = 10.0f;
    v0.z() = 5.0f;

    len = v0.length();
    EXPECT_TRUE(is_fp_eq(len,22.9128780f));

    v0.normalizeIfNecessary();
    len = v0.length();
    EXPECT_TRUE(is_fp_eq(len,1.0f));

    EXPECT_TRUE(is_fp_eq(v0.x(),0.87287158f));
    EXPECT_TRUE(is_fp_eq(v0.y(),0.43643579f));
    EXPECT_TRUE(is_fp_eq(v0.z(),0.21821789f));


    Vec4 v1(10.0f, -20.0f, 30.0f, 0.0f);
    Vec4 v2(2.0f, 3.0f, -4.0f, 0.0f);
    
    f32 dotResult = Vec4::dot(v1, v2);
    EXPECT_TRUE(is_fp_eq(dotResult, -160.0f));

    Vec4 crossResult = Vec4::cross(v1, v2);
    EXPECT_TRUE(is_fp_eq(crossResult.x(), -10.0f));
    EXPECT_TRUE(is_fp_eq(crossResult.y(), 100.0f));
    EXPECT_TRUE(is_fp_eq(crossResult.z(), 70.0f));
    
}

