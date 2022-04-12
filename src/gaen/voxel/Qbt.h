//------------------------------------------------------------------------------
// Qbt.h - Qubicle binary tree reader
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

#ifndef GAEN_VOXEL_QBT_H
#define GAEN_VOXEL_QBT_H

#include "gaen/math/vec3.h"
#include "gaen/core/String.h"
#include "gaen/core/Vector.h"
#include "gaen/core/Map.h"
#include "gaen/assets/Color.h"

namespace gaen
{

static const u32 kQbtMagic = 0x32204251; // 'QB 2' little endian

enum QbtNodeType : u32
{
    kQBNT_Matrix   = 0,
    kQBNT_Model    = 1,
    kQBNT_Compound = 2
};

struct QbtNode
{
    const QbtNode * pParent;
    Vector<kMEM_Chef, std::shared_ptr<QbtNode>> children;
    Map<kMEM_Chef, ChefString, std::shared_ptr<QbtNode>> childMap;

    QbtNodeType typeId;
    ChefString name;
    ChefString fullName;
    ivec3 position;
    uvec3 localScale;
    vec3 pivot;
    uvec3 size;
    Vector<kMEM_Chef, Color> voxels;

    const Color & voxel(u32 x, u32 y, u32 z) const;
    const Color& voxel(const uvec3 & coord) const;

    const std::shared_ptr<QbtNode> findChild(const ChefString & name) const;

    QbtNode()
      : pParent(nullptr)
      , typeId(kQBNT_Matrix)
      , position(0)
      , localScale(0)
      , pivot(0)
      , size(0)
    {}
};

struct Qbt
{
    Vector<kMEM_Chef, Color> colors;

    std::shared_ptr<QbtNode> pRoot;
    Qbt()
      : pRoot(nullptr)
    {}

    static std::shared_ptr<Qbt> load_from_file(const ChefString & path);
};

} // namespace gaen

#endif // #ifndef GAEN_VOXEL_QBT_H
