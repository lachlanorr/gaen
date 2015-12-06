//------------------------------------------------------------------------------
// voxels.cpp - CPU side voxel support
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

#include "engine/stdafx.h"

#include "core/logging.h"

#include "engine/voxels.h"

namespace gaen
{

static const Vec3 kNormals[7] = { { 0.0f,  0.0f,  0.0f},
                                  {-1.0f,  0.0f,  0.0f},   // left
                                  { 1.0f,  0.0f,  0.0f},   // right
                                  { 0.0f, -1.0f,  0.0f},   // bottom
                                  { 0.0f,  1.0f,  0.0f},   // top
                                  { 0.0f,  0.0f, -1.0f},   // back
                                  { 0.0f,  0.0f,  1.0f} }; // front

static const SubVoxel kVoxelSearchOrder[6][8] = { // left
                                                    { SubVoxel::LeftBottomBack,
                                                      SubVoxel::LeftBottomFront,
                                                      SubVoxel::LeftTopBack,
                                                      SubVoxel::LeftTopFront,

                                                      SubVoxel::RightBottomBack,
                                                      SubVoxel::RightBottomFront,
                                                      SubVoxel::RightTopBack,
                                                      SubVoxel::RightTopFront
                                                    },

                                                    // right
                                                    { SubVoxel::RightBottomBack,
                                                      SubVoxel::RightBottomFront,
                                                      SubVoxel::RightTopBack,
                                                      SubVoxel::RightTopFront,

                                                      SubVoxel::LeftBottomBack,
                                                      SubVoxel::LeftBottomFront,
                                                      SubVoxel::LeftTopBack,
                                                      SubVoxel::LeftTopFront
                                                    },

                                                    // bottom
                                                    { SubVoxel::LeftBottomBack,
                                                      SubVoxel::LeftBottomFront,
                                                      SubVoxel::RightBottomBack,
                                                      SubVoxel::RightBottomFront,

                                                      SubVoxel::LeftTopBack,
                                                      SubVoxel::LeftTopFront,
                                                      SubVoxel::RightTopBack,
                                                      SubVoxel::RightTopFront
                                                    },

                                                    // top
                                                    { SubVoxel::LeftTopBack,
                                                      SubVoxel::LeftTopFront,
                                                      SubVoxel::RightTopBack,
                                                      SubVoxel::RightTopFront,

                                                      SubVoxel::LeftBottomBack,
                                                      SubVoxel::LeftBottomFront,
                                                      SubVoxel::RightBottomBack,
                                                      SubVoxel::RightBottomFront
                                                    },

                                                    // back
                                                    { SubVoxel::LeftBottomBack,
                                                      SubVoxel::LeftTopBack,
                                                      SubVoxel::RightBottomBack,
                                                      SubVoxel::RightTopBack,

                                                      SubVoxel::LeftBottomFront,
                                                      SubVoxel::LeftTopFront,
                                                      SubVoxel::RightBottomFront,
                                                      SubVoxel::RightTopFront
                                                    },

                                                    // front
                                                    { SubVoxel::LeftBottomFront,
                                                      SubVoxel::LeftTopFront,
                                                      SubVoxel::RightBottomFront,
                                                      SubVoxel::RightTopFront,

                                                      SubVoxel::LeftBottomBack,
                                                      SubVoxel::LeftTopBack,
                                                      SubVoxel::RightBottomBack,
                                                      SubVoxel::RightTopBack
                                                    } };

static_assert(sizeof(kVoxelSearchOrder) == 48, "kVoxelSearchOrder not 48 bytes");

AABB_MinMax voxel_subspace(const AABB_MinMax & pSpace, SubVoxel subIndex)
{
    // voxels are always cubes, so we only need one dimension's half
    f32 dimHalf = 0.5f * (pSpace.max.x() - pSpace.min.x());

    u32 subIdx = static_cast<u32>(subIndex);

    // utilize the binary values of VoxelIndex to determine xyz offsets
    Vec3 offsets(((subIdx & 4) >> 2) * dimHalf,
                 ((subIdx & 2) >> 1) * dimHalf,
                 (subIdx & 1) * dimHalf);

    AABB_MinMax ret;
    ret.min = pSpace.min + offsets;
    ret.max = ret.min + Vec3(dimHalf, dimHalf, dimHalf);
    return ret;
}

bool test_ray_box(u32 * pNormalIndex, const Vec3 & rayPos, const Vec3 & rayDir, const AABB_MinMax & aabb)
{
    // On GPU, intersect box looks something like:
    // See http://prideout.net/blog/?p=64
    /*
      bool IntersectBox(Ray r, AABB aabb, out float t0, out float t1)
      {
      vec3 invR = 1.0 / r.Dir;
      vec3 tbot = invR * (aabb.Min-r.Origin);
      vec3 ttop = invR * (aabb.Max-r.Origin);
      vec3 tmin = min(ttop, tbot);
      vec3 tmax = max(ttop, tbot);
      vec2 t = max(tmin.xx, tmin.yz);
      t0 = max(t.x, t.y);
      t = min(tmax.xx, tmax.yz);
      t1 = min(t.x, t.y);
      return t0 <= t1;
      }
    */


    Vec3 invR = 1.0f / rayDir;

    Vec3 tbot = invR * (aabb.min - rayPos);
    Vec3 ttop = invR * (aabb.max - rayPos);
    Vec3 tmin = min_(ttop, tbot);
    Vec3 tmax = max_(ttop, tbot);
    Vec2 t = max_(Vec2(tmin.x(), tmin.x()), Vec2(tmin.y(), tmin.z()));
    float t0 = maxval(t.x(), t.y());
    t = min_(Vec2(tmax.x(), tmax.x()), Vec2(tmax.y(), tmax.z()));
    float t1 = minval(t.x(), t.y());


    // find index into normals, only one of these is true,
    // and this eliminates all branching.
    // If none are true, we have no collision.
    *pNormalIndex = 0;
    *pNormalIndex += (u32)(t0 <= t1 && t0 == tbot.x()) * 1;
    *pNormalIndex += (u32)(t0 <= t1 && t0 == ttop.x()) * 2;
    *pNormalIndex += (u32)(t0 <= t1 && t0 == tbot.y()) * 3;
    *pNormalIndex += (u32)(t0 <= t1 && t0 == ttop.y()) * 4;
    *pNormalIndex += (u32)(t0 <= t1 && t0 == tbot.z()) * 5;
    *pNormalIndex += (u32)(t0 <= t1 && t0 == ttop.z()) * 6;

    return *pNormalIndex != 0;
}



ImageBuffer::ImageBuffer(u16 size, size_t pixelSize)
{
    mSize = size;
    mPixelSize = (u8)pixelSize;
    mPixels = (u8*)GALLOC(kMEM_Engine, mSize * mSize * pixelSize);
}

ImageBuffer::~ImageBuffer()
{
    GFREE(mPixels);
}



VoxelWorld::VoxelWorld()
{
    mImages.reserve(kImageCount);
    mImages.emplace_back(kImageSize, sizeof(Pix_RG32U));
}



static const u32 kMaxDepth = 32;

struct VoxelRecurseInfo
{
    AABB_MinMax aabb;
    const SubVoxel * searchOrder;
    u8 searchIndex;
    bool hit;
};

static VoxelRecurseInfo sRecurseStack[kMaxDepth];

bool test_ray_voxel(VoxelRef * pVoxelRef, Vec3 * pNormal, const VoxelWorld & voxelWorld, const Vec3 & rayPos, const Vec3 & rayDir, const VoxelRoot & root, u32 maxDepth)
{
    // put ray into this voxel's space, so voxel is at 0,0,0 and 0,0,0 rotation
    // from ray's perspective.

    // Translate ray
    Vec3 rayPosLoc = rayPos - root.pos;

    // Transpose of rotation matrix is the inverse
    Mat3 rotInv = Mat3::transpose(root.rot);
    Vec3 rayDirLoc = Mat3::multiply(rotInv, rayDir);

    AABB_MinMax rootAabb(root.rad);

    *pVoxelRef = root.children;

    u32 normalIndex = 0;
    bool hit = test_ray_box(&normalIndex, rayPosLoc, rayDirLoc, rootAabb);

    if (hit)
    {
        // don't think this should ever happen, as root voxel should never be terminal
        ASSERT(!pVoxelRef->isTerminalEmpty());

        u32 recurseDepth = 0;

        VoxelRecurseInfo * pRi = &sRecurseStack[recurseDepth];
        pRi->searchOrder = kVoxelSearchOrder[normalIndex-1]; // -1 since this array has no dummy 0th item like the kNormals
        pRi->searchIndex = 0;
        pRi->aabb = rootAabb;
        pRi->hit = hit;

        for (;;)
        {
            while (pRi->searchIndex < 8)
            {
                AABB_MinMax aabb = voxel_subspace(pRi->aabb, pRi->searchOrder[pRi->searchIndex]);

                pRi->hit = test_ray_box(&normalIndex, rayPosLoc, rayDirLoc, aabb);

                if (pRi->hit)
                {
                    *pVoxelRef = voxelWorld.getVoxelRef(*pVoxelRef, pRi->searchOrder[pRi->searchIndex]);
                    LOG_INFO("voxelRef: flags:%d, imageIdx:%d, voxelIdx:%d", pVoxelRef->flags, pVoxelRef->imageIdx, pVoxelRef->voxelIdx);

                    if (pVoxelRef->isTerminalFull())
                    {
                        *pNormal = kNormals[normalIndex];
                        return true;
                    }
                    else if (pVoxelRef->isTerminalEmpty())
                    {
                        pRi->searchIndex++;
                        continue;
                    }
                    else
                    {
                        recurseDepth++;

                        // LORRTODO: Either the geometry must prevent further depth, or we need a check here
                        ASSERT(recurseDepth < kMaxDepth);

                        pRi = &sRecurseStack[recurseDepth];

                        pRi->searchOrder = kVoxelSearchOrder[normalIndex-1];
                        pRi->searchIndex = 0;
                        pRi->aabb = rootAabb;

                        break;
                    }
                } // if (pRi->hit)

                pRi->searchIndex++;
            } // while (pRi->searchIndex < 8)


            if (!pRi->hit && recurseDepth > 0)
            {
                // we hit nothing in this voxel, recurse up a level
                recurseDepth--;
                continue; // pop stack and continue search
            }
            else
            {
                *pNormal = kNormals[0];
                return pRi->hit;
            }
        } // for (;;)

    }
    return false;
}


} // namespace gaen
