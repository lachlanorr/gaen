//------------------------------------------------------------------------------
// vox_types.cpp - Utilities to identify VoxObj types
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

#include "gaen/assets/Gimg.h"
#include "gaen/voxel/Qbt.h"
#include "gaen/voxel/VoxObj.h"
#include "gaen/voxel/types/VoxBiped.h"
#include "gaen/voxel/types/VoxWeapon.h"
#include "gaen/voxel/types/VoxProp.h"
#include "gaen/voxel/vox_types.h"

namespace gaen
{

static const Vector<kMEM_Chef, VoxObjTypeDelegator> kVoxObjTypeDelegators
{
    { VoxType::Biped, VoxBiped::is_of_type },
    { VoxType::Weapon, VoxWeapon::is_of_type }
};

const ChefString & vox_type_str(VoxType type)
{
    static const HashMap<kMEM_Chef, VoxType, ChefString> sTypeMap{
        { VoxType::Biped, "biped" },
        { VoxType::Weapon, "weapon" },
        { VoxType::Prop, "prop" }
    };

    const auto it = sTypeMap.find(type);
    PANIC_IF(it == sTypeMap.end(), "Invalid VoxType: %d", type);
    return it->second;
}

bool VoxObjType::do_parts_match(const VoxObjType & objType, const std::shared_ptr<QbtNode> & pNode)
{
    for (const auto & part : objType.parts)
    {
        const auto pChild = pNode->findChild(part.name);
        if (!pChild || pChild->typeId != kQBNT_Matrix)
            return false;
    }
    return true;
}

bool VoxObjType::determine_type(const std::shared_ptr<QbtNode> & pNode, VoxObjType & voxObjType)
{
    for (const auto & objTypeDelegator : kVoxObjTypeDelegators)
    {
        if (objTypeDelegator.is_of_type(pNode, voxObjType))
        {
            return true;
        }
    }
    return false;
}

} // namespace gaen
