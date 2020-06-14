//------------------------------------------------------------------------------
// test_math.cpp - Tests for math routines
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

#include "math/vec3.h"

#include "render_support/voxel.h"

using namespace gaen;

TEST(MathTest, VoxelNeighborNormals)
{
    // test that prints out our normals

    vec3 norm;

    norm = normalize(voxel_neighbor_offset(kVN_LeftBottomBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_LeftBottomMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_LeftBottomFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = normalize(voxel_neighbor_offset(kVN_LeftMiddleBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_LeftMiddleMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_LeftMiddleFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = normalize(voxel_neighbor_offset(kVN_LeftTopBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_LeftTopMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_LeftTopFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = normalize(voxel_neighbor_offset(kVN_MiddleBottomBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_MiddleBottomMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_MiddleBottomFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = normalize(voxel_neighbor_offset(kVN_MiddleMiddleBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    //kVN_MiddleMiddleMiddle,
    norm = normalize(voxel_neighbor_offset(kVN_MiddleMiddleFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = normalize(voxel_neighbor_offset(kVN_MiddleTopBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_MiddleTopMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_MiddleTopFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = normalize(voxel_neighbor_offset(kVN_RightBottomBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_RightBottomMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_RightBottomFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = normalize(voxel_neighbor_offset(kVN_RightMiddleBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_RightMiddleMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_RightMiddleFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = normalize(voxel_neighbor_offset(kVN_RightTopBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_RightTopMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = normalize(voxel_neighbor_offset(kVN_RightTopFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

}
