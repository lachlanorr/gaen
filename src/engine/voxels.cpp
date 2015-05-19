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

bool test_ray_voxel(VoxelRef * pVoxelRef, const Vec3 & rayPos, const Vec3 & rayDir, const VoxelRoot & root)
{
    // put ray into this voxel's space, so voxel is at 0,0,0 and 0,0,0 rotation
    // from ray's perspective.

    // Translate ray
    Vec3 rayPosLoc = rayPos - root.pos;

    // Transpose of rotation matrix is the inverse
    Mat3 rotInv = Mat3::transpose(root.rot);
    Vec3 rayDirLoc = Mat3::multiply(rotInv, rayDir);

    // rayLoc is now the ray in local voxel coords
    Vec3 rayInv(1.0f / rayDirLoc.x(), 1.0f / rayDirLoc.y(), 1.0f / rayDirLoc.z());

    // initialize our box, and start collision detection
    f32 boxRad = root.rad;
    Vec3 boxMin(-boxRad, -boxRad, -boxRad);
    Vec3 boxMax(boxRad, boxRad, boxRad);
    VoxelRef voxelRef = root.children;

//    for (;;)
    {

        // On GPU, intesect box looks something like:
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

        f32 tx1 = (boxMin.x() - rayPosLoc.x()) * rayInv.x();
        f32 tx2 = (boxMax.x() - rayPosLoc.x()) * rayInv.x();

        f32 tmin = minval(tx1, tx2);
        f32 tmax = maxval(tx1, tx2);

        f32 ty1 = (boxMin.y() - rayPosLoc.y()) * rayInv.y();
        f32 ty2 = (boxMax.y() - rayPosLoc.y()) * rayInv.y();

        tmin = maxval(tmin, minval(ty1, ty2));
        tmax = minval(tmax, maxval(ty1, ty2));

        f32 tz1 = (boxMin.z() - rayPosLoc.z()) * rayInv.z();
        f32 tz2 = (boxMax.z() - rayPosLoc.z()) * rayInv.z();

        tmin = maxval(tmin, minval(tz1, tz2));
        tmax = minval(tmax, maxval(tz1, tz2));

        if (tmax >= maxval(0.0f, tmin))// && tmin < t)
        {
            *pVoxelRef = voxelRef;
            return true;
        }
        else
        {
            return false;
        }
    }
}


} // namespace gaen
