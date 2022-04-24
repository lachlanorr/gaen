//------------------------------------------------------------------------------
// VoxWeapon.h - Voxel Weapon Geometry
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

#ifndef GAEN_VOXEL_VOX_WEAPON_H
#define GAEN_VOXEL_VOX_WEAPON_H

#include "gaen/voxel/VoxObj.h"

namespace gaen
{
struct QbtNode;

struct VoxWeapon : public VoxObj
{
    VoxWeapon(const std::shared_ptr<QbtNode> & pRootNode, const VoxObjType & type)
      : VoxObj(pRootNode, type)
    {}

    static VoxObjUP create(const std::shared_ptr<QbtNode>& pRootNode);
    static bool is_of_type(const std::shared_ptr<QbtNode>& pRootNode, VoxObjType & voxObjType);

    Vector<kMEM_Chef, ChefString> exportFiles(const ChefString & directory) const override;
};

} // namespace gaen

#endif // #ifndef GAEN_VOXEL_WEAPON_H
