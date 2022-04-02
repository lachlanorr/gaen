//------------------------------------------------------------------------------
// VoxObj.h - Voxel Geometry
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2022 Lachlan Orr
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

#ifndef GAEN_VOXEL_VOX_OBJ_H
#define GAEN_VOXEL_VOX_OBJ_H

#include "gaen/voxel/VoxMatrix.h"
#include "gaen/voxel/VoxSkel.h"
#include "gaen/voxel/vox_types.h"

namespace gaen
{
class Gimg;
struct Qbt;

struct VoxObj
{
    const Qbt& qbt;
    VoxSkel voxSkel;

    VoxMatrixMap baseMatrices;

    ivec3 mins;
    ivec3 maxes;
    vec3 center;
    vec3 halfExtents;

    VoxObjType type;

    UniquePtr<Gimg> pGimgDiffuse;

    VoxObj(const Qbt& qbt);

    void exportFiles(const ChefString & basePath, f32 voxelSize) const;
};

} // namespace gaen

#endif // #ifndef GAEN_VOXEL_OBJ_H
