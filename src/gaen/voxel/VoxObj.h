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

enum VoxSide : i32
{
    kVSD_UNDEFINED = -1,

    kVSD_Left   = 0,
    kVSD_Back   = 1,
    kVSD_Bottom = 2,
    kVSD_Right  = 3,
    kVSD_Front  = 4,
    kVSD_Top    = 5,

    kVSD_COUNT  = 6
};

enum VoxSideFlag : i32
{
    kVSDF_NONE   = 0x00,

    kVSDF_Left   = 0x01,
    kVSDF_Back   = 0x02,
    kVSDF_Bottom = 0x04,
    kVSDF_Right  = 0x08,
    kVSDF_Front  = 0x10,
    kVSDF_Top    = 0x20
};

struct VoxFace
{
    ivec3 start;
    ivec3 size;
    VoxSide side;
};

struct Voxel
{
    Color color;
    ivec3 pos;
    u32 visibleSides; // bitmask of VoxSide's

    VoxFace * faces; // faces voxel owns, could be composite faces with other co-planar-adjacent voxel faces
    VoxFace * pFaces[kVSD_COUNT];     // pointer to actual face, could be pointing to composite face on another voxel

    Voxel(Color color, ivec3 pos)
      : color(color)
      , pos(pos)
      , visibleSides(0)
    {
        faces = (VoxFace*)GALLOC(kMEM_Chef, sizeof(VoxFace)*kVSD_COUNT); // malloc so things don't move around, our pFaces must remain constant
        for (i32 i = 0; i < kVSD_COUNT; ++i)
        {
            faces[i].start = pos;
            faces[i].size = ivec3(1);
            faces[i].side = (VoxSide)i;
            pFaces[i] = nullptr;
        }
    }
    ~Voxel()
    {
        if (faces != nullptr)
            GFREE(faces);
    }

    Voxel(const Voxel&) = delete;
    Voxel& operator=(const Voxel&) = delete;

    // Re-enable move
    Voxel(Voxel&& rhs) noexcept
    {
        color = rhs.color;
        pos = rhs.pos;
        visibleSides = rhs.visibleSides;
        faces = rhs.faces;
        rhs.faces = nullptr;
        for (i32 i = 0; i < kVSD_COUNT; ++i)
        {
            pFaces[i] = rhs.pFaces[i];
            rhs.pFaces[i] = nullptr;
        }
    };
    Voxel& operator=(Voxel&& rhs)
    {
        color = rhs.color;
        pos = rhs.pos;
        visibleSides = rhs.visibleSides;
        faces = rhs.faces;
        rhs.faces = nullptr;
        for (i32 i = 0; i < kVSD_COUNT; ++i)
        {
            pFaces[i] = rhs.pFaces[i];
            rhs.pFaces[i] = nullptr;
        }
        return *this;
    };
};

struct VoxMatrix;
struct VoxMatrixFace
{
    const VoxFace * pFace;
    const VoxMatrix * pMatrix;
    i32 area;
    vec3 points[4];
    vec2 uvs[4];

    VoxMatrixFace(const VoxFace * pFace, const VoxMatrix * pMatrix)
      : pFace(pFace)
      , pMatrix(pMatrix)
      , area(pFace->size.x * pFace->size.y * pFace->size.z)
    {
        for (u32 i = 0; i < 4; ++i)
        {
            points[i] = vec3(0);
            uvs[i] = vec2(0);
        }
    }
};

struct VoxMatrix
{
    const QbtNode& node;

    ivec3 worldPos;

    Vector<kMEM_Chef, Voxel> voxels;
    HashMap<kMEM_Chef, ivec3, size_t> voxelIdMap;

    Vector<kMEM_Chef, VoxMatrixFace> faces;

    VoxMatrix(const QbtNode &node)
      : node(node)
      , worldPos(0)
    {}
};

typedef UniquePtr<VoxMatrix> VoxMatrixUP;
typedef HashMap<kMEM_Chef, ChefString, VoxMatrixUP> VoxMatrixMap;

struct VoxObj
{
    const Qbt& qbt;

    VoxMatrixMap baseMatrices;
    VoxMatrixMap nullsMatrices;

    VoxObjType type;

    UniquePtr<Gimg> pGimgDiffuse;

    VoxObj(const Qbt& qbt);

    void exportFiles(const ChefString & basePath, f32 scaleFactor) const;
};

} // namespace gaen

#endif // #ifndef GAEN_VOXEL_QBT_H
