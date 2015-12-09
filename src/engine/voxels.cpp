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


static const SubVoxel kVoxelSearchOrder[6 * 4 * 8] = {

    // left
        // LeftBottomBack
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftTopBack,

            SubVoxel::RightBottomBack,
            SubVoxel::RightBottomFront,
            SubVoxel::RightTopFront,
            SubVoxel::RightTopBack,

        // LeftBottomFront
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomBack,

            SubVoxel::RightBottomFront,
            SubVoxel::RightTopFront,
            SubVoxel::RightTopBack,
            SubVoxel::RightBottomBack,

        // LeftTopFront
            SubVoxel::LeftTopFront,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftBottomFront,

            SubVoxel::RightTopFront,
            SubVoxel::RightTopBack,
            SubVoxel::RightBottomBack,
            SubVoxel::RightBottomFront,

        // LeftTopBack
            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftTopFront,

            SubVoxel::RightTopBack,
            SubVoxel::RightBottomBack,
            SubVoxel::RightBottomFront,
            SubVoxel::RightTopFront,

    // right
        // RightBottomFront
            SubVoxel::RightBottomFront,
            SubVoxel::RightBottomBack,
            SubVoxel::RightTopBack,
            SubVoxel::RightTopFront,

            SubVoxel::LeftBottomFront,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftTopFront,

        // RightBottomBack
            SubVoxel::RightBottomBack,
            SubVoxel::RightTopBack,
            SubVoxel::RightTopFront,
            SubVoxel::RightBottomFront,

            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomFront,

        // RightTopBack
            SubVoxel::RightTopBack,
            SubVoxel::RightTopFront,
            SubVoxel::RightBottomFront,
            SubVoxel::RightBottomBack,

            SubVoxel::LeftTopBack,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftBottomBack,

        // RightTopFront
            SubVoxel::RightTopFront,
            SubVoxel::RightBottomFront,
            SubVoxel::RightBottomBack,
            SubVoxel::RightTopBack,

            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopBack,

    // bottom
        // LeftBottomBack
            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomBack,
            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomFront,

            SubVoxel::LeftTopBack,
            SubVoxel::RightTopBack,
            SubVoxel::RightTopFront,
            SubVoxel::LeftTopFront,

        // RightBottomBack
            SubVoxel::RightBottomBack,
            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftBottomBack,

            SubVoxel::RightTopBack,
            SubVoxel::RightTopFront,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftTopBack,

        // RightBottomFront
            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomBack,

            SubVoxel::RightTopFront,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftTopBack,
            SubVoxel::RightTopBack,

        // LeftBottomFront
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomBack,
            SubVoxel::RightBottomFront,

            SubVoxel::LeftTopFront,
            SubVoxel::LeftTopBack,
            SubVoxel::RightTopBack,
            SubVoxel::RightTopFront,

    // top
        // LeftTopFront
            SubVoxel::LeftTopFront,
            SubVoxel::RightTopFront,
            SubVoxel::RightTopBack,
            SubVoxel::LeftTopBack,

            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomFront,
            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomBack,

        // RightTopFront
            SubVoxel::RightTopFront,
            SubVoxel::RightTopBack,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftTopFront,

            SubVoxel::RightBottomFront,
            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftBottomFront,

        // RightTopBack
            SubVoxel::RightTopBack,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftTopFront,
            SubVoxel::RightTopFront,

            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomFront,

        // LeftTopBack
            SubVoxel::LeftTopBack,
            SubVoxel::LeftTopFront,
            SubVoxel::RightTopFront,
            SubVoxel::RightTopBack,

            SubVoxel::LeftBottomBack,
            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomFront,
            SubVoxel::RightBottomBack,

    // back
        // RightBottomBack
            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopBack,
            SubVoxel::RightTopBack,

            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftTopFront,
            SubVoxel::RightTopFront,

        // LeftBottomBack
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopBack,
            SubVoxel::RightTopBack,
            SubVoxel::RightBottomBack,

            SubVoxel::LeftBottomFront,
            SubVoxel::LeftTopFront,
            SubVoxel::RightTopFront,
            SubVoxel::RightBottomFront,

        // LeftTopBack
            SubVoxel::LeftTopBack,
            SubVoxel::RightTopBack,
            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomBack,

            SubVoxel::LeftTopFront,
            SubVoxel::RightTopFront,
            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomFront,

        // RightTopBack
            SubVoxel::RightTopBack,
            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopBack,

            SubVoxel::RightTopFront,
            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftTopFront,

    // front
        // LeftBottomFront
            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomFront,
            SubVoxel::RightTopFront,
            SubVoxel::LeftTopFront,

            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomBack,
            SubVoxel::RightTopBack,
            SubVoxel::LeftTopBack,

        // RightBottomFront
            SubVoxel::RightBottomFront,
            SubVoxel::RightTopFront,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomFront,

            SubVoxel::RightBottomBack,
            SubVoxel::RightTopBack,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomBack,

        // RightTopFront
            SubVoxel::RightTopFront,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomFront,

            SubVoxel::RightTopBack,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomBack,

        // LeftTopFront
            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomFront,
            SubVoxel::RightTopFront,

            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomBack,
            SubVoxel::RightTopBack
};

static_assert(sizeof(kVoxelSearchOrder) == 192, "kVoxelSearchOrder not 192 bytes");

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

inline bool test_ray_box(VoxelFaceHit * pVoxelFaceHit,
                         f32 * pEntryDist,
                         f32 * pExitDist,
                         const Vec3 & rayPos,
                         const Vec3 & invRayDir,
                         const AABB_MinMax & aabb)
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


    Vec3 tbot = invRayDir * (aabb.min - rayPos);
    Vec3 ttop = invRayDir * (aabb.max - rayPos);
    Vec3 tmin = min_(ttop, tbot);
    Vec3 tmax = max_(ttop, tbot);
    Vec2 t = max_(Vec2(tmin.x(), tmin.x()), Vec2(tmin.y(), tmin.z()));
    f32 t0 = maxval(t.x(), t.y());
    t = min_(Vec2(tmax.x(), tmax.x()), Vec2(tmax.y(), tmax.z()));
    f32 t1 = minval(t.x(), t.y());


    // find index into normals, only one of these is true,
    // and this eliminates all branching.
    // If none are true, we have no collision.

    u32 faceHit = 0;
    faceHit += (u32)(t0 <= t1 && t0 == tbot.x()) * 1;
    faceHit += (u32)(t0 <= t1 && t0 == ttop.x()) * 2;
    faceHit += (u32)(t0 <= t1 && t0 == tbot.y()) * 3;
    faceHit += (u32)(t0 <= t1 && t0 == ttop.y()) * 4;
    faceHit += (u32)(t0 <= t1 && t0 == tbot.z()) * 5;
    faceHit += (u32)(t0 <= t1 && t0 == ttop.z()) * 6;

    *pVoxelFaceHit = (VoxelFaceHit)faceHit;
    *pEntryDist = t0;
    *pExitDist = t1;

    return *pVoxelFaceHit != VoxelFaceHit::None;
}

inline bool is_hit_within_voxel(VoxelFaceHit voxelFaceHit,
                               const Vec3 & hitPos,
                               const AABB_MinMax & aabb)
{

    // Compiler should make this a jump table for us,
    // better than our own jump table of functions since this
    // can be inlined.

    bool ret = false;
    switch (voxelFaceHit)
    {
    case VoxelFaceHit::Left:
    case VoxelFaceHit::Right:
        ret = hitPos.y() >= aabb.min.y() &&
              hitPos.y() <= aabb.max.y() &&
              hitPos.z() >= aabb.min.z() &&
              hitPos.z() <= aabb.max.z();
    case VoxelFaceHit::Bottom:
    case VoxelFaceHit::Top:
        ret = hitPos.x() >= aabb.min.x() &&
              hitPos.x() <= aabb.max.x() &&
              hitPos.z() >= aabb.min.z() &&
              hitPos.z() <= aabb.max.z();
    case VoxelFaceHit::Back:
    case VoxelFaceHit::Front:
        ret = hitPos.x() >= aabb.min.x() &&
              hitPos.x() <= aabb.max.x() &&
              hitPos.y() >= aabb.min.y() &&
              hitPos.y() <= aabb.max.y();
    }
    return ret;
}

inline void eval_voxel_hit(const SubVoxel ** ppSearchOrder,
                           Vec3 * pHitPos,
                           VoxelFaceHit voxelFaceHit,
                           f32 entryDist,
                           f32 exitDist,
                           const Vec3 & rayPos,
                           const Vec3 & rayDir,
                           const AABB_MinMax & aabb)
{
    *pHitPos = rayPos + rayDir * entryDist;
    u32 searchOrderIndex = 0;

    // index into the face within kVoxelSearchOrder based on the fase we hit
    const SubVoxel * pSearchBlock = kVoxelSearchOrder + ((u32)voxelFaceHit - 1) * (8 * 4);

    // We can skip the 0th entry, since we default to that if other 3 fail
    // AABB_MinMax subAabb0 = voxel_subspace(aabb, pSearchBlock[0]);
    AABB_MinMax subAabb1 = voxel_subspace(aabb, pSearchBlock[8]);
    AABB_MinMax subAabb2 = voxel_subspace(aabb, pSearchBlock[16]);
    AABB_MinMax subAabb3 = voxel_subspace(aabb, pSearchBlock[24]);

    // zero or one of following expressions are true,
    // if zero are true, it means that subAbb0 was the hit, and we're already initialized to 0.
    // searchOrderIndex += (u32)is_hit_within_voxel(voxelFaceHit, *pHitPos, subAabb0) * 0;
    searchOrderIndex += (u32)is_hit_within_voxel(voxelFaceHit, *pHitPos, subAabb1) * 1;
    searchOrderIndex += (u32)is_hit_within_voxel(voxelFaceHit, *pHitPos, subAabb2) * 2;
    searchOrderIndex += (u32)is_hit_within_voxel(voxelFaceHit, *pHitPos, subAabb3) * 3;

    *ppSearchOrder = pSearchBlock + searchOrderIndex * 8;

    ASSERT(*ppSearchOrder < (&kVoxelSearchOrder[0] + sizeof(kVoxelSearchOrder)));
}

ImageBuffer::ImageBuffer(u16 size, size_t pixelSize)
{
    mSize = size;
    mPixelSize = (u8)pixelSize;
    mPixels = (u8*)GALLOC(kMEM_Engine, bufferSize());
}

ImageBuffer::~ImageBuffer()
{
    GFREE(mPixels);
}

void ImageBuffer::copy(const ImageBuffer & sourceBuffer)
{
    ASSERT(sourceBuffer.bufferSize() == bufferSize());
    memcpy(mPixels, sourceBuffer.buffer(), bufferSize());
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

bool test_ray_voxel(VoxelRef * pVoxelRef, Vec3 * pNormal, f32 * pZDepth, const VoxelWorld & voxelWorld, const Vec3 & rayPos, const Vec3 & rayDir, const VoxelRoot & root, u32 maxDepth)
{
    // put ray into this voxel's space, so voxel is at 0,0,0 and 0,0,0 rotation
    // from ray's perspective.

    // Translate ray
    Vec3 rayPosLoc = rayPos - root.pos;

    // Transpose of rotation matrix is the inverse
    Mat3 rotInv = Mat3::transpose(root.rot);
    Vec3 rayDirLoc = Mat3::multiply(rotInv, rayDir);
    Vec3 invRayDirLoc = 1.0f / rayDirLoc;

    AABB_MinMax rootAabb(root.rad);

    *pVoxelRef = root.children;

    VoxelFaceHit voxelFaceHit;
    f32 entryDist;
    f32 exitDist;
    const SubVoxel * pSearchOrder;
    Vec3 hitPosLoc;

    bool hit = test_ray_box(&voxelFaceHit, &entryDist, &exitDist, rayPosLoc, invRayDirLoc, rootAabb);

    if (hit)
    {
        // don't think this should ever happen, as root voxel should never be terminal
        ASSERT(!pVoxelRef->isTerminalEmpty());

        eval_voxel_hit(&pSearchOrder,
                       &hitPosLoc,
                       voxelFaceHit,
                       entryDist,
                       exitDist,
                       rayPosLoc,
                       rayDirLoc,
                       rootAabb);

        u32 recurseDepth = 0;

        VoxelRecurseInfo * pRi = &sRecurseStack[recurseDepth];
        pRi->searchOrder = pSearchOrder;
        pRi->searchIndex = 0;
        pRi->aabb = rootAabb;
        pRi->hit = hit;

        for (;;)
        {
            while (pRi->searchIndex < 8)
            {
                AABB_MinMax aabb = voxel_subspace(pRi->aabb, pRi->searchOrder[pRi->searchIndex]);

                pRi->hit = test_ray_box(&voxelFaceHit, &entryDist, &exitDist, rayPosLoc, invRayDirLoc, aabb);

                if (pRi->hit)
                {
                    *pVoxelRef = voxelWorld.getVoxelRef(*pVoxelRef, pRi->searchOrder[pRi->searchIndex]);

                    if (pVoxelRef->isTerminalFull())
                    {
                        *pNormal = kNormals[(u32)voxelFaceHit];
                        *pZDepth = entryDist;
                        // now conduct more specific test to find point of intersection


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

                        eval_voxel_hit(&pSearchOrder,
                                       &hitPosLoc,
                                       voxelFaceHit,
                                       entryDist,
                                       exitDist,
                                       rayPosLoc,
                                       rayDirLoc,
                                       aabb);

                        pRi = &sRecurseStack[recurseDepth];

                        pRi->searchOrder = pSearchOrder;
                        pRi->searchIndex = 0;
                        pRi->aabb = aabb;

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

                //return pRi->hit;
                return false;
            }
        } // for (;;)

    }
    return false;
}


} // namespace gaen
