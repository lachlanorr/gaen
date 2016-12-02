//------------------------------------------------------------------------------
// test_math.cpp - Tests for math routines
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/glm_ext.h"

#include "render_support/voxel.h"

using namespace gaen;

TEST(MathTest, Mat3x4)
{
    glm::mat4 trans4(1.0f);
    trans4 = glm::translate(trans4, glm::vec3(1.0f, 10.0f, 100.0f));
    trans4 = glm::rotate(trans4, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    trans4 = glm::rotate(trans4, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    trans4 = glm::rotate(trans4, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans4 = glm::scale(trans4, glm::vec3(10.0f, 100.0f, 1000.0f));

    glm::mat4x3 trans43 = glm::to_mat4x3(trans4);
    glm::mat4 trans4Re = glm::to_mat4x4(trans43);

    ASSERT_TRUE(trans4 == trans4Re);

}

TEST(MathTest, Mat3x3)
{
    glm::mat4 trans4(1.0f);
    trans4 = glm::rotate(trans4, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    trans4 = glm::rotate(trans4, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    trans4 = glm::rotate(trans4, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat3 trans3 = glm::to_mat3x3(trans4);
    glm::mat4 trans4Re = glm::to_mat4x4(trans3);

    ASSERT_TRUE(trans4 == trans4Re);

}

TEST(MathTest, VoxelNeighborNormals)
{
    // test that prints out our normals

    glm::vec3 norm;

    norm = glm::normalize(voxel_neighbor_offset(kVN_LeftBottomBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_LeftBottomMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_LeftBottomFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = glm::normalize(voxel_neighbor_offset(kVN_LeftMiddleBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_LeftMiddleMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_LeftMiddleFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = glm::normalize(voxel_neighbor_offset(kVN_LeftTopBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_LeftTopMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_LeftTopFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = glm::normalize(voxel_neighbor_offset(kVN_MiddleBottomBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_MiddleBottomMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_MiddleBottomFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = glm::normalize(voxel_neighbor_offset(kVN_MiddleMiddleBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    //kVN_MiddleMiddleMiddle,
    norm = glm::normalize(voxel_neighbor_offset(kVN_MiddleMiddleFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = glm::normalize(voxel_neighbor_offset(kVN_MiddleTopBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_MiddleTopMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_MiddleTopFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = glm::normalize(voxel_neighbor_offset(kVN_RightBottomBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_RightBottomMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_RightBottomFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = glm::normalize(voxel_neighbor_offset(kVN_RightMiddleBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_RightMiddleMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_RightMiddleFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

    norm = glm::normalize(voxel_neighbor_offset(kVN_RightTopBack));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_RightTopMiddle));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);
    norm = glm::normalize(voxel_neighbor_offset(kVN_RightTopFront));
    printf("%1.8ef, %1.8ef, %1.8ef\n", norm.x, norm.y, norm.z);

}
