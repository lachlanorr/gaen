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

#include "gaen/core/HashMap.h"
#include "gaen/core/Vector.h"
#include "gaen/math/vec3.h"
#include "gaen/math/vec2.h"
#include "gaen/assets/Color.h"

#include "gaen/voxel/Qbt.h"
#include "gaen/voxel/vox_types.h"

namespace gaen
{
class Gimg;

enum VoxSide : u32
{
    kVSD_Left   = 0x01,
    kVSD_Back   = 0x02,
    kVSD_Bottom = 0x04,
    kVSD_Right  = 0x08,
    kVSD_Front  = 0x10,
    kVSD_Top    = 0x20
};

struct VoxPixel
{
    Color color;
    ivec3 localPos;
    ivec3 worldPos;
    u32 visibleSides; // bitmask of VoxSide's
    vec2 uv;

    VoxPixel()
      : color(0,0,0,0)
      , localPos(0)
      , worldPos(0)
      , visibleSides(0)
    {}
};

struct VoxQuad
{
    vec3 verts[4];
    u32 pixelIdx;
    VoxSide side;
};

typedef Vector<kMEM_Chef, VoxPixel> VoxPixelVec;

struct VoxMatrix
{
    const QbtNode& node;

    i32 pixBegin;
    i32 pixEnd;

    ivec3 worldPos;

    u32 visibleSides;

    Vector<kMEM_Chef, VoxQuad> quads;

    VoxMatrix(const QbtNode &node)
      : node(node)
      , pixBegin(-1)
      , pixEnd(-1)
    {}
};

typedef UniquePtr<VoxMatrix> VoxMatrixUP;
typedef HashMap<kMEM_Chef, ChefString, VoxMatrixUP> VoxMatrixMap;

struct VoxObj
{
    const Qbt& qbt;

    VoxMatrixMap matrices;
    const VoxObjType *pType;

    VoxPixelVec pixels;
    UniquePtr<Gimg> pGimgDiffuse;

    VoxObj(const Qbt& qbt);

    void exportFiles(const ChefString & basePath, f32 scaleFactor) const;
};

} // namespace gaen

#endif // #ifndef GAEN_VOXEL_QBT_H
