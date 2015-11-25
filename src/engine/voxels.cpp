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

void voxel_subspace(AABB_MinMax * pSubSpace, const AABB_MinMax & pSpace, VoxelIndex subIndex)
{
    // voxels are always cubes, so we only need one dimension's half
    f32 dimHalf = 0.5f * (pSpace.maxCorner.x() - pSpace.minCorner.x());

    // utilize the binary values of VoxelIndex to determine xyz offsets
    Vec3 offsets(((subIndex & 4) >> 2) * dimHalf,
                 ((subIndex & 2) >> 1) * dimHalf,
                 (subIndex & 1) * dimHalf);

    pSubSpace->minCorner = pSpace.minCorner + offsets;
    pSubSpace->maxCorner = pSubSpace->minCorner + offsets;
}

bool test_ray_voxel(VoxelRef * pVoxelRef, const Vec3 & rayPos, const Vec3 & rayDir, const VoxelRoot & root, Vec3 & normal)
{
    static const u32 kCubeSides = 6;
    static const Vec3 kNormals[kCubeSides + 1] = { { 0.0f,  0.0f,  0.0f},
                                                   {-1.0f,  0.0f,  0.0f},
                                                   { 1.0f,  0.0f,  0.0f},
                                                   { 0.0f, -1.0f,  0.0f},
                                                   { 0.0f,  1.0f,  0.0f},
                                                   { 0.0f,  0.0f, -1.0f},
                                                   { 0.0f,  0.0f,  1.0f} };


    // put ray into this voxel's space, so voxel is at 0,0,0 and 0,0,0 rotation
    // from ray's perspective.

    // Translate ray
    Vec3 rayPosLoc = rayPos - root.pos;

    // Transpose of rotation matrix is the inverse
    Mat3 rotInv = Mat3::transpose(root.rot);
    Vec3 rayDirLoc = Mat3::multiply(rotInv, rayDir);

    // initialize our box, and start collision detection
    f32 boxRad = root.rad;
    Vec3 boxMin(-boxRad, -boxRad, -boxRad);
    Vec3 boxMax(boxRad, boxRad, boxRad);
    *pVoxelRef = root.children;



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


    // rayLoc is now the ray in local voxel coords
    Vec3 invR = 1.0f / rayDirLoc;

//    for (;;)
    {
        Vec3 tbot = invR * (boxMin - rayPosLoc);
        Vec3 ttop = invR * (boxMax - rayPosLoc);
        Vec3 tmin = min_(ttop, tbot);
        Vec3 tmax = max_(ttop, tbot);
        Vec2 t = max_(Vec2(tmin.x(), tmin.x()), Vec2(tmin.y(), tmin.z()));
        float t0 = maxval(t.x(), t.y());
        t = min_(Vec2(tmax.x(), tmax.x()), Vec2(tmax.y(), tmax.z()));
        float t1 = minval(t.x(), t.y());

        // find index into normals, only one of these is true,
        // and this eliminates all branching.
        // If none are true, we have no collision.
        u32 normalIdx = 0;

        normalIdx += (u32)(t0 <= t1 && t0 == tbot.x()) * 1;
        normalIdx += (u32)(t0 <= t1 && t0 == ttop.x()) * 2;
        normalIdx += (u32)(t0 <= t1 && t0 == tbot.y()) * 3;
        normalIdx += (u32)(t0 <= t1 && t0 == ttop.y()) * 4;
        normalIdx += (u32)(t0 <= t1 && t0 == tbot.z()) * 5;
        normalIdx += (u32)(t0 <= t1 && t0 == ttop.z()) * 6;

        normal = kNormals[normalIdx];
        return normalIdx != 0;
    }

}


} // namespace gaen
