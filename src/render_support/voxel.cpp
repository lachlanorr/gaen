//------------------------------------------------------------------------------
// voxel.cpp - CPU side voxel support
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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

#include "render_support/stdafx.h"

#include "core/logging.h"
#include "math/common.h"

#include "render_support/voxel.h"

namespace gaen
{

static const vec3 kNormals[7] = { { 0.0f,  0.0f,  0.0f},
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
            SubVoxel::LeftTopBack,
            SubVoxel::LeftTopFront,

            SubVoxel::RightBottomBack,
            SubVoxel::RightBottomFront,
            SubVoxel::RightTopBack,
            SubVoxel::RightTopFront,

        // LeftBottomFront
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopBack,

            SubVoxel::RightBottomFront,
            SubVoxel::RightTopFront,
            SubVoxel::RightBottomBack,
            SubVoxel::RightTopBack,

        // LeftTopFront
            SubVoxel::LeftTopFront,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftBottomBack,

            SubVoxel::RightTopFront,
            SubVoxel::RightTopBack,
            SubVoxel::RightBottomFront,
            SubVoxel::RightBottomBack,

        // LeftTopBack
            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomFront,

            SubVoxel::RightTopBack,
            SubVoxel::RightBottomBack,
            SubVoxel::RightTopFront,
            SubVoxel::RightBottomFront,

    // right
        // RightBottomFront
            SubVoxel::RightBottomFront,
            SubVoxel::RightBottomBack,
            SubVoxel::RightTopFront,
            SubVoxel::RightTopBack,

            SubVoxel::LeftBottomFront,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftTopBack,

        // RightBottomBack
            SubVoxel::RightBottomBack,
            SubVoxel::RightTopBack,
            SubVoxel::RightBottomFront,
            SubVoxel::RightTopFront,

            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftTopFront,

        // RightTopBack
            SubVoxel::RightTopBack,
            SubVoxel::RightTopFront,
            SubVoxel::RightBottomBack,
            SubVoxel::RightBottomFront,

            SubVoxel::LeftTopBack,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftBottomFront,

        // RightTopFront
            SubVoxel::RightTopFront,
            SubVoxel::RightBottomFront,
            SubVoxel::RightTopBack,
            SubVoxel::RightBottomBack,

            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomBack,

    // bottom
        // LeftBottomBack
            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomFront,

            SubVoxel::LeftTopBack,
            SubVoxel::RightTopBack,
            SubVoxel::LeftTopFront,
            SubVoxel::RightTopFront,

        // RightBottomBack
            SubVoxel::RightBottomBack,
            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftBottomFront,

            SubVoxel::RightTopBack,
            SubVoxel::RightTopFront,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftTopFront,

        // RightBottomFront
            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomBack,

            SubVoxel::RightTopFront,
            SubVoxel::LeftTopFront,
            SubVoxel::RightTopBack,
            SubVoxel::LeftTopBack,

        // LeftBottomFront
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomFront,
            SubVoxel::RightBottomBack,

            SubVoxel::LeftTopFront,
            SubVoxel::LeftTopBack,
            SubVoxel::RightTopFront,
            SubVoxel::RightTopBack,

    // top
        // LeftTopFront
            SubVoxel::LeftTopFront,
            SubVoxel::RightTopFront,
            SubVoxel::LeftTopBack,
            SubVoxel::RightTopBack,

            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomBack,

        // RightTopFront
            SubVoxel::RightTopFront,
            SubVoxel::RightTopBack,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftTopBack,

            SubVoxel::RightBottomFront,
            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftBottomBack,

        // RightTopBack
            SubVoxel::RightTopBack,
            SubVoxel::LeftTopBack,
            SubVoxel::RightTopFront,
            SubVoxel::LeftTopFront,

            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomFront,

        // LeftTopBack
            SubVoxel::LeftTopBack,
            SubVoxel::LeftTopFront,
            SubVoxel::RightTopBack,
            SubVoxel::RightTopFront,

            SubVoxel::LeftBottomBack,
            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomBack,
            SubVoxel::RightBottomFront,

    // back
        // RightBottomBack
            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::RightTopBack,
            SubVoxel::LeftTopBack,

            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::RightTopFront,
            SubVoxel::LeftTopFront,

        // LeftBottomBack
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopBack,
            SubVoxel::RightBottomBack,
            SubVoxel::RightTopBack,

            SubVoxel::LeftBottomFront,
            SubVoxel::LeftTopFront,
            SubVoxel::RightBottomFront,
            SubVoxel::RightTopFront,

        // LeftTopBack
            SubVoxel::LeftTopBack,
            SubVoxel::RightTopBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomBack,

            SubVoxel::LeftTopFront,
            SubVoxel::RightTopFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomFront,

        // RightTopBack
            SubVoxel::RightTopBack,
            SubVoxel::RightBottomBack,
            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomBack,

            SubVoxel::RightTopFront,
            SubVoxel::RightBottomFront,
            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomFront,

    // front
        // LeftBottomFront
            SubVoxel::LeftBottomFront,
            SubVoxel::RightBottomFront,
            SubVoxel::LeftTopFront,
            SubVoxel::RightTopFront,

            SubVoxel::LeftBottomBack,
            SubVoxel::RightBottomBack,
            SubVoxel::LeftTopBack,
            SubVoxel::RightTopBack,

        // RightBottomFront
            SubVoxel::RightBottomFront,
            SubVoxel::RightTopFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::LeftTopFront,

            SubVoxel::RightBottomBack,
            SubVoxel::RightTopBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::LeftTopBack,

        // RightTopFront
            SubVoxel::RightTopFront,
            SubVoxel::LeftTopFront,
            SubVoxel::RightBottomFront,
            SubVoxel::LeftBottomFront,

            SubVoxel::RightTopBack,
            SubVoxel::LeftTopBack,
            SubVoxel::RightBottomBack,
            SubVoxel::LeftBottomBack,

        // LeftTopFront
            SubVoxel::LeftTopFront,
            SubVoxel::LeftBottomFront,
            SubVoxel::RightTopFront,
            SubVoxel::RightBottomFront,

            SubVoxel::LeftTopBack,
            SubVoxel::LeftBottomBack,
            SubVoxel::RightTopBack,
            SubVoxel::RightBottomBack
};

static_assert(sizeof(kVoxelSearchOrder) == 192, "kVoxelSearchOrder not 192 bytes");


static const vec3 kVoxelNeighborOffsets[26] =
{
    vec3(-1.0f, -1.0f, -1.0f), // kVN_LeftBottomBack     =  0,
    vec3(-1.0f, -1.0f,  0.0f), // kVN_LeftBottomMiddle   =  1,
    vec3(-1.0f, -1.0f,  1.0f), // kVN_LeftBottomFront    =  2,

    vec3(-1.0f,  0.0f, -1.0f), // kVN_LeftMiddleBack     =  3,
    vec3(-1.0f,  0.0f,  0.0f), // kVN_LeftMiddleMiddle   =  4,
    vec3(-1.0f,  0.0f,  1.0f), // kVN_LeftMiddleFront    =  5,

    vec3(-1.0f,  1.0f, -1.0f), // kVN_LeftTopBack        =  6,
    vec3(-1.0f,  1.0f,  0.0f), // kVN_LeftTopMiddle      =  7,
    vec3(-1.0f,  1.0f,  1.0f), // kVN_LeftTopFront       =  8,

    vec3( 0.0f, -1.0f, -1.0f), // kVN_MiddleBottomBack   =  9,
    vec3( 0.0f, -1.0f,  0.0f), // kVN_MiddleBottomMiddle = 10,
    vec3( 0.0f, -1.0f,  1.0f), // kVN_MiddleBottomFront  = 11,

    vec3( 0.0f,  0.0f, -1.0f), // kVN_MiddleMiddleBack   = 12,
    //kVN_MiddleMiddleMiddle,
    vec3( 0.0f,  0.0f,  1.0f), // kVN_MiddleMiddleFront  = 13,

    vec3( 0.0f,  1.0f, -1.0f), // kVN_MiddleTopBack      = 14,
    vec3( 0.0f,  1.0f,  0.0f), // kVN_MiddleTopMiddle    = 15,
    vec3( 0.0f,  1.0f,  1.0f), // kVN_MiddleTopFront     = 16,

    vec3( 1.0f, -1.0f, -1.0f), // kVN_RightBottomBack    = 17,
    vec3( 1.0f, -1.0f,  0.0f), // kVN_RightBottomMiddle  = 18,
    vec3( 1.0f, -1.0f,  1.0f), // kVN_RightBottomFront   = 19,

    vec3( 1.0f,  0.0f, -1.0f), // kVN_RightMiddleBack    = 20,
    vec3( 1.0f,  0.0f,  0.0f), // kVN_RightMiddleMiddle  = 21,
    vec3( 1.0f,  0.0f,  1.0f), // kVN_RightMiddleFront   = 22,

    vec3( 1.0f,  1.0f, -1.0f), // kVN_RightTopBack       = 23,
    vec3( 1.0f,  1.0f,  0.0f), // kVN_RightTopMiddle     = 24,
    vec3( 1.0f,  1.0f,  1.0f)  // kVN_RightTopFront      = 25
};


static const vec3 kVoxelNeighborDirections[26] =
{
    vec3(-5.77350259e-001f, -5.77350259e-001f, -5.77350259e-001f), // kVN_LeftBottomBack     =  0,
    vec3(-7.07106769e-001f, -7.07106769e-001f,  0.00000000e+000f), // kVN_LeftBottomMiddle   =  1,
    vec3(-5.77350259e-001f, -5.77350259e-001f,  5.77350259e-001f), // kVN_LeftBottomFront    =  2,

    vec3(-7.07106769e-001f,  0.00000000e+000f, -7.07106769e-001f), // kVN_LeftMiddleBack     =  3,
    vec3(-1.00000000e+000f,  0.00000000e+000f,  0.00000000e+000f), // kVN_LeftMiddleMiddle   =  4,
    vec3(-7.07106769e-001f,  0.00000000e+000f,  7.07106769e-001f), // kVN_LeftMiddleFront    =  5,

    vec3(-5.77350259e-001f,  5.77350259e-001f, -5.77350259e-001f), // kVN_LeftTopBack        =  6,
    vec3(-7.07106769e-001f,  7.07106769e-001f,  0.00000000e+000f), // kVN_LeftTopMiddle      =  7,
    vec3(-5.77350259e-001f,  5.77350259e-001f,  5.77350259e-001f), // kVN_LeftTopFront       =  8,

    vec3( 0.00000000e+000f, -7.07106769e-001f, -7.07106769e-001f), // kVN_MiddleBottomBack   =  9,
    vec3( 0.00000000e+000f, -1.00000000e+000f,  0.00000000e+000f), // kVN_MiddleBottomMiddle = 10,
    vec3( 0.00000000e+000f, -7.07106769e-001f,  7.07106769e-001f), // kVN_MiddleBottomFront  = 11,

    vec3( 0.00000000e+000f,  0.00000000e+000f, -1.00000000e+000f), // kVN_MiddleMiddleBack   = 12,
    //kVN_MiddleMiddleMiddle,
    vec3( 0.00000000e+000f,  0.00000000e+000f,  1.00000000e+000f), // kVN_MiddleMiddleFront  = 13,

    vec3( 0.00000000e+000f,  7.07106769e-001f, -7.07106769e-001f), // kVN_MiddleTopBack      = 14,
    vec3( 0.00000000e+000f,  1.00000000e+000f,  0.00000000e+000f), // kVN_MiddleTopMiddle    = 15,
    vec3( 0.00000000e+000f,  7.07106769e-001f,  7.07106769e-001f), // kVN_MiddleTopFront     = 16,

    vec3( 5.77350259e-001f, -5.77350259e-001f, -5.77350259e-001f), // kVN_RightBottomBack    = 17,
    vec3( 7.07106769e-001f, -7.07106769e-001f,  0.00000000e+000f), // kVN_RightBottomMiddle  = 18,
    vec3( 5.77350259e-001f, -5.77350259e-001f,  5.77350259e-001f), // kVN_RightBottomFront   = 19,

    vec3( 7.07106769e-001f,  0.00000000e+000f, -7.07106769e-001f), // kVN_RightMiddleBack    = 20,
    vec3( 1.00000000e+000f,  0.00000000e+000f,  0.00000000e+000f), // kVN_RightMiddleMiddle  = 21,
    vec3( 7.07106769e-001f,  0.00000000e+000f,  7.07106769e-001f), // kVN_RightMiddleFront   = 22,

    vec3( 5.77350259e-001f,  5.77350259e-001f, -5.77350259e-001f), // kVN_RightTopBack       = 23,
    vec3( 7.07106769e-001f,  7.07106769e-001f,  0.00000000e+000f), // kVN_RightTopMiddle     = 24,
    vec3( 5.77350259e-001f,  5.77350259e-001f,  5.77350259e-001f), // kVN_RightTopFront      = 25
};

vec3 voxel_neighbor_offset(VoxelNeighbor vn)
{
    return kVoxelNeighborOffsets[vn];
}

AABB_MinMax voxel_subspace(const AABB_MinMax & pSpace, SubVoxel subIndex)
{
    // voxels are always cubes, so we only need one dimension's half
    f32 dimHalf = 0.5f * (pSpace.max.x - pSpace.min.x);

    u32 subIdx = static_cast<u32>(subIndex);

    // utilize the binary values of VoxelIndex to determine xyz offsets
    vec3 offsets(((subIdx & 4) >> 2) * dimHalf,
                      ((subIdx & 2) >> 1) * dimHalf,
                      (subIdx & 1) * dimHalf);

    AABB_MinMax ret;
    ret.min = pSpace.min + offsets;
    ret.max = ret.min + vec3(dimHalf, dimHalf, dimHalf);
    return ret;
}


inline bool test_ray_box(VoxelFace * pVoxelFace,
                         f32 * pEntryDist,
                         f32 * pExitDist,
                         const vec3 & rayPos,
                         const vec3 & invRayDir,
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

    vec3 tbot = invRayDir * (aabb.min - rayPos);
    vec3 ttop = invRayDir * (aabb.max - rayPos);
    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);
    vec2 t = max(vec2(tmin.x, tmin.x), vec2(tmin.y, tmin.z));
    f32 t0 = max(t.x, t.y);
    t = min(vec2(tmax.x, tmax.x), vec2(tmax.y, tmax.z));
    f32 t1 = min(t.x, t.y);

    // find the face that was hit, only one condition is true,
    // and this eliminates branching.
    // If none are true, we have no collision.

    u32 faceHit = 0;
    bool isHit = t1 > 0.0f && t0 <= t1;

    faceHit = max(faceHit, (u32)(isHit && !std::isinf(invRayDir.x) && t0 == tbot.x) * 1);
    faceHit = max(faceHit, (u32)(isHit && !std::isinf(invRayDir.x) && t0 == ttop.x) * 2);
    faceHit = max(faceHit, (u32)(isHit && !std::isinf(invRayDir.y) && t0 == tbot.y) * 3);
    faceHit = max(faceHit, (u32)(isHit && !std::isinf(invRayDir.y) && t0 == ttop.y) * 4);
    faceHit = max(faceHit, (u32)(isHit && !std::isinf(invRayDir.z) && t0 == tbot.z) * 5);
    faceHit = max(faceHit, (u32)(isHit && !std::isinf(invRayDir.z) && t0 == ttop.z) * 6);

    ASSERT(!isHit || faceHit != 0);

    *pVoxelFace = (VoxelFace)faceHit;
    *pEntryDist = t0;
    *pExitDist = t1;

    return *pVoxelFace != VoxelFace::None;
}

inline bool is_hit_within_voxel(VoxelFace voxelFace,
                                const vec3 & hitPos,
                                const AABB_MinMax & aabb)
{
    return (((voxelFace == VoxelFace::Left ||
             voxelFace == VoxelFace::Right) &&
            hitPos.y > aabb.min.y &&
            hitPos.y <= aabb.max.y &&
            hitPos.z > aabb.min.z &&
            hitPos.z <= aabb.max.z) ||

            ((voxelFace == VoxelFace::Bottom ||
              voxelFace == VoxelFace::Top) &&
             hitPos.x > aabb.min.x &&
             hitPos.x <= aabb.max.x &&
             hitPos.z > aabb.min.z &&
             hitPos.z <= aabb.max.z) ||

            ((voxelFace == VoxelFace::Back ||
              voxelFace == VoxelFace::Front) &&
             hitPos.x > aabb.min.x &&
             hitPos.x <= aabb.max.x &&
             hitPos.y > aabb.min.y &&
             hitPos.y <= aabb.max.y));
}

inline vec2 calc_face_uv(VoxelFace voxelFace,
                              const vec3 & hitPos,
                              const AABB_MinMax & aabb)
{
    // get the array of 6 floats so we can index easily
    const f32 * pAabbElems = reinterpret_cast<const f32*>(&aabb);

    // Couple asserts to make sure the hackish behavior above is valid
    ASSERT(vec3(pAabbElems[0], pAabbElems[1], pAabbElems[2]) == aabb.min);
    ASSERT(vec3(pAabbElems[3], pAabbElems[4], pAabbElems[5]) == aabb.max);

    u32 uHitElem = 0;
    uHitElem = max(uHitElem, (u32)(voxelFace == VoxelFace::Left   || voxelFace == VoxelFace::Right) * 2); // z()
    uHitElem = max(uHitElem, (u32)(voxelFace == VoxelFace::Bottom || voxelFace == VoxelFace::Top)   * 0); // x()
    uHitElem = max(uHitElem, (u32)(voxelFace == VoxelFace::Back   || voxelFace == VoxelFace::Front) * 0); // x()

    u32 vHitElem = 0;
    vHitElem = max(vHitElem, (u32)(voxelFace == VoxelFace::Left   || voxelFace == VoxelFace::Right) * 1); // y()
    vHitElem = max(vHitElem, (u32)(voxelFace == VoxelFace::Bottom || voxelFace == VoxelFace::Top)   * 2); // z()
    vHitElem = max(vHitElem, (u32)(voxelFace == VoxelFace::Back   || voxelFace == VoxelFace::Front) * 1); // y()


    u32 uAabbElem = 0;
    uAabbElem = max(uAabbElem, (u32)(voxelFace == VoxelFace::Left)   * 2);  // aabb.min.z
    uAabbElem = max(uAabbElem, (u32)(voxelFace == VoxelFace::Right)  * 5);  // aabb.max.z
    uAabbElem = max(uAabbElem, (u32)(voxelFace == VoxelFace::Bottom) * 0);  // aabb.min.x
    uAabbElem = max(uAabbElem, (u32)(voxelFace == VoxelFace::Top)    * 0);  // aabb.min.x
    uAabbElem = max(uAabbElem, (u32)(voxelFace == VoxelFace::Back)   * 3);  // aabb.max.x
    uAabbElem = max(uAabbElem, (u32)(voxelFace == VoxelFace::Front)  * 0);  // aabb.min.x

    u32 vAabbElem = 0;
    vAabbElem = max(vAabbElem, (u32)(voxelFace == VoxelFace::Left)   * 1);  // aabb.min.y
    vAabbElem = max(vAabbElem, (u32)(voxelFace == VoxelFace::Right)  * 1);  // aabb.min.y
    vAabbElem = max(vAabbElem, (u32)(voxelFace == VoxelFace::Bottom) * 2);  // aabb.min.z
    vAabbElem = max(vAabbElem, (u32)(voxelFace == VoxelFace::Top)    * 5);  // aabb.max.z
    vAabbElem = max(vAabbElem, (u32)(voxelFace == VoxelFace::Back)   * 1);  // aabb.min.y
    vAabbElem = max(vAabbElem, (u32)(voxelFace == VoxelFace::Front)  * 1);  // aabb.min.y

    f32 aabbWidth = aabb.max.x - aabb.min.x;

    return vec2(abs(hitPos[uHitElem] - pAabbElems[uAabbElem]) / aabbWidth,
                     abs(hitPos[vHitElem] - pAabbElems[vAabbElem]) / aabbWidth);
}

inline void eval_voxel_hit(const SubVoxel ** ppSearchOrder,
                           vec3 * pHitPos,
                           VoxelFace voxelFace,
                           f32 entryDist,
                           f32 exitDist,
                           const vec3 & rayPos,
                           const vec3 & rayDir,
                           const AABB_MinMax & aabb)
{
    *pHitPos = rayPos + rayDir * entryDist;
    u32 searchOrderIndex = 0;

    // index into the face within kVoxelSearchOrder based on the face we hit
    const SubVoxel * pSearchBlock = kVoxelSearchOrder + ((u32)voxelFace - 1) * (8 * 4);

    // We can skip the 0th entry, since we default to that if other 3 fail
    AABB_MinMax subAabb0 = voxel_subspace(aabb, pSearchBlock[0]);
    AABB_MinMax subAabb1 = voxel_subspace(aabb, pSearchBlock[8]);
    AABB_MinMax subAabb2 = voxel_subspace(aabb, pSearchBlock[16]);
    AABB_MinMax subAabb3 = voxel_subspace(aabb, pSearchBlock[24]);

    // zero or one of following expressions are true,
    // if zero are true, it means that subAbb0 was the hit, and we're already initialized to 0.
    u32 is_hit_within0 = is_hit_within_voxel(voxelFace, *pHitPos, subAabb0);
    u32 is_hit_within1 = is_hit_within_voxel(voxelFace, *pHitPos, subAabb1);
    u32 is_hit_within2 = is_hit_within_voxel(voxelFace, *pHitPos, subAabb2);
    u32 is_hit_within3 = is_hit_within_voxel(voxelFace, *pHitPos, subAabb3);

    EXPECT_MSG(is_hit_within0 || is_hit_within1 || is_hit_within2 || is_hit_within3, "eval_voxel_hit: no hits within sub aabb's");
    EXPECT_MSG((u32)is_hit_within0 + (u32)is_hit_within1 + (u32)is_hit_within2 + (u32)is_hit_within3 == 1, "eval_voxel_hit: more than one hit within sub aabb's");

    searchOrderIndex += (u32)is_hit_within0 * 0;
    searchOrderIndex += (u32)is_hit_within1 * 1;
    searchOrderIndex += (u32)is_hit_within2 * 2;
    searchOrderIndex += (u32)is_hit_within3 * 3;

    *ppSearchOrder = pSearchBlock + searchOrderIndex * 8;

    ASSERT(*ppSearchOrder < (&kVoxelSearchOrder[0] + sizeof(kVoxelSearchOrder)));
}

VoxelWorld::VoxelWorld()
  : mVoxelRoots(512, sizeof(RG32U))
{
    mImages.reserve(kImageCount);
    mImages.emplace_back(kImageSize, sizeof(RG32U));
}


void VoxelWorld::addVoxelRoot(const VoxelRoot & voxelRoot)
{
    VoxelRoot * pVr = reinterpret_cast<VoxelRoot*>(mVoxelRoots.buffer());
    pVr[mVoxelRootCount] = voxelRoot;
    mVoxelRootCount++;
}


static const u32 kMaxDepth = 32;

struct VoxelRecurseInfo
{
    VoxelRef voxelRef;
    AABB_MinMax aabb;
    const SubVoxel * searchOrder;
    u8 searchIndex;
    bool hit;
    VoxelFace hitFace;
    f32 entryDist;
    f32 exitDist;
    vec3 hitPosLoc;
};

static VoxelRecurseInfo sStack[kMaxDepth];

inline VoxelRecurseInfo prep_stack_entry(const VoxelRef & voxelRef, const AABB_MinMax & aabb)
{
    VoxelRecurseInfo stackEntry;
    stackEntry.voxelRef = voxelRef;
    stackEntry.aabb = aabb;
    stackEntry.searchOrder = nullptr;
    stackEntry.searchIndex = 0;
    stackEntry.hit = false;
    stackEntry.hitFace = VoxelFace::None;
    stackEntry.entryDist = 0.0f;
    stackEntry.exitDist = 0.0f;
    stackEntry.hitPosLoc = vec3(0.0f, 0.0f, 0.0f);

    return stackEntry;
}

bool test_ray_voxel(VoxelRef * pVoxelRef, vec3 * pNormal, f32 * pZDepth, VoxelFace * pFace, vec2 * pFaceUv, const VoxelWorld & voxelWorld, const vec3 & rayPos, const vec3 & rayDir, const VoxelRoot & root, u32 maxDepth)
{
    // put ray into this voxel's space, so voxel is at 0,0,0 and 0,0,0 rotation
    // from ray's perspective.

    // Translate ray
    vec3 rayPosLoc = rayPos - root.pos;

    // Transpose of rotation matrix is the inverse
    mat3 rotInv = transpose(root.rot);
    vec3 rayDirLoc = rotInv * rayDir;
    vec3 invRayDirLoc = 1.0f / rayDirLoc;

    AABB_MinMax rootAabb(root.rad);



    // put voxel root on recurse stack before iteration begins
    u32 d = 0;
    sStack[d] = prep_stack_entry(root.children, rootAabb);

    sStack[d].hit = test_ray_box(&sStack[d].hitFace,
                                 &sStack[d].entryDist,
                                 &sStack[d].exitDist,
                                 rayPosLoc,
                                 invRayDirLoc,
                                 sStack[d].aabb);

    if (!sStack[d].hit || sStack[d].voxelRef.isTerminalEmpty())
    {
        return false;
    }

    // else we hit, loop/recurse over children
    while (true)
    {
        eval_voxel_hit(&sStack[d].searchOrder,
                       &sStack[d].hitPosLoc,
                       sStack[d].hitFace,
                       sStack[d].entryDist,
                       sStack[d].exitDist,
                       rayPosLoc,
                       rayDirLoc,
                       sStack[d].aabb);

        if (sStack[d].hit && sStack[d].voxelRef.isTerminalFull())
        {
            *pVoxelRef = sStack[d].voxelRef;
            *pNormal = kNormals[(u32)sStack[d].hitFace];
            *pZDepth = sStack[d].entryDist;
            *pFace = sStack[d].hitFace;
            *pFaceUv = calc_face_uv(sStack[d].hitFace, sStack[d].hitPosLoc, sStack[d].aabb);
            return true;
        }
        else
        {
//            for (;;)
//            {
                if (sStack[d].searchIndex >= 8)
                {
                    if (d > 0)
                    {
                        d--;
                        continue;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    VoxelRef childRef = voxelWorld.voxelRef(sStack[d].voxelRef.imageIdx, sStack[d].voxelRef.voxelIdx, sStack[d].searchOrder[sStack[d].searchIndex]);
                    AABB_MinMax childAabb = voxel_subspace(sStack[d].aabb,
                                                           sStack[d].searchOrder[sStack[d].searchIndex]);
                    sStack[d].searchIndex++; // increment so if we recurse back here to this level we move past this one

                    if (childRef.isTerminalEmpty())
                    {
                        continue;
                    }

                    VoxelRecurseInfo recInf = prep_stack_entry(childRef, childAabb);
                    recInf.hit = test_ray_box(&recInf.hitFace,
                                              &recInf.entryDist,
                                              &recInf.exitDist,
                                              rayPosLoc,
                                              invRayDirLoc,
                                              recInf.aabb);
                    if (recInf.hit)
                    {
                        d++;
                        ASSERT(d < kMaxDepth);

                        sStack[d] = recInf;
                    }
                }
//            }
        }
    }

    return false;
}


bool test_ray_voxel_gpu(VoxelRef & voxelRef, // out
                        vec3 & normal,  // out
                        f32 & zDepth,        // out
                        u32 & face,          // out
                        vec2 & faceUv,  // out
                        const ImageBuffer * voxelData,
                        const vec3 & rayPos,
                        const vec3 & rayDir,
                        const VoxelRoot & root,
                        u32 maxDepth)
{
    // put ray into this voxel's space, so voxel is at 0,0,0 and 0,0,0 rotation
    // from ray's perspective.

    // Translate ray
    vec3 rayPosLoc = rayPos - root.pos;

    // Transpose of rotation matrix is the inverse
    mat3 rotInv = transpose(root.rot);
    vec3 rayDirLoc = rotInv * rayDir;
    vec3 invRayDirLoc = 1.0f / rayDirLoc;

    AABB_MinMax rootAabb(root.rad);

/*

    // put voxel root on recurse stack before iteration begins
    u32 d = 0;
    prep_stack_entry(&sStack[d], root.children, rootAabb);

    sStack[d].hit = test_ray_box(&sStack[d].hitFace,
        &sStack[d].entryDist,
        &sStack[d].exitDist,
        rayPosLoc,
        invRayDirLoc,
        sStack[d].aabb);

    if (!sStack[d].hit || sStack[d].voxelRef.isTerminalEmpty())
    {
        return false;
    }

    // else we hit, loop/recurse over children
    for (;;)
    {
        eval_voxel_hit(&sStack[d].searchOrder,
            &sStack[d].hitPosLoc,
            sStack[d].hitFace,
            sStack[d].entryDist,
            sStack[d].exitDist,
            rayPosLoc,
            rayDirLoc,
            sStack[d].aabb);

        if (sStack[d].voxelRef.isTerminalFull())
        {
            voxelRef = sStack[d].voxelRef;
            normal = kNormals[(u32)sStack[d].hitFace];
            zDepth = sStack[d].entryDist;
            face = sStack[d].hitFace;
            faceUv = calc_face_uv(sStack[d].hitFace, sStack[d].hitPosLoc, sStack[d].aabb);
            return true;
        }
        else
        {
            for (;;)
            {
                if (sStack[d].searchIndex >= 8)
                {
                    if (d > 0)
                    {
                        d--;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    VoxelRef childRef = voxelWorld.voxelRef(sStack[d].voxelRef.imageIdx, sStack[d].voxelRef.voxelIdx, sStack[d].searchOrder[sStack[d].searchIndex]);
                    AABB_MinMax childAabb = voxel_subspace(sStack[d].aabb,
                        sStack[d].searchOrder[sStack[d].searchIndex]);
                    sStack[d].searchIndex++; // increment so if we recurse back here to this level we move past this one

                    if (childRef.isTerminalEmpty())
                    {
                        continue;
                    }

                    VoxelRecurseInfo recInf;
                    prep_stack_entry(&recInf, childRef, childAabb);
                    recInf.hit = test_ray_box(&recInf.hitFace,
                        &recInf.entryDist,
                        &recInf.exitDist,
                        rayPosLoc,
                        invRayDirLoc,
                        recInf.aabb);
                    if (recInf.hit)
                    {
                        d++;
                        ASSERT(d < kMaxDepth);

                        sStack[d] = recInf;
                        break; // restart on parent for loop
                    }
                    else
                    {
                        continue;
                    }
                }
            }
        }
    }
    */
    return false;
}


} // namespace gaen
