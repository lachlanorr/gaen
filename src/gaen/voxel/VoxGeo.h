//------------------------------------------------------------------------------
// VoxGeo.h - Voxel points, polys, uvs, etc
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

#ifndef GAEN_VOXEL_VOX_GEO_H
#define GAEN_VOXEL_VOX_GEO_H

#include <array>

#include "gaen/math/vec3.h"
#include "gaen/math/vec2.h"
#include "gaen/core/String.h"
#include "gaen/core/Vector.h"
#include "gaen/voxel/VoxMatrix.h"

namespace gaen
{

static const u32 kMaxVoxPolyPoints = 5;

struct VoxPoint
{
    vec3 pos;
    vec2 uv;
};

struct VoxPoly
{
    VoxSide side;
    u32 pointCount;
    std::array<VoxPoint, kMaxVoxPolyPoints> points;
};

struct VoxMesh
{
    ChefString name;
    Vector<kMEM_Chef, VoxPoly> polys;

    VoxMesh(const ChefString & name)
      : name(name)
    {}
};

struct VoxGeo
{
    VoxGeo(const Vector<kMEM_Chef, const VoxMatrix*> & matrices);

    Vector<kMEM_Chef, UniquePtr<VoxMesh>> meshes;
};

} // namespace gaen

#endif // #ifndef GAEN_VOXEL_GEO_H
