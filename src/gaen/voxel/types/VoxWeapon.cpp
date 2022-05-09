//------------------------------------------------------------------------------
// VoxWeapon.cpp - Voxel Weapon Geometry
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
#include "gaen/assets/file_utils.h"

#include "gaen/voxel/qbt.h"
#include "gaen/voxel/VoxExporter.h"
#include "gaen/voxel/types/VoxWeapon.h"

namespace gaen
{

static const VoxObjType kVoxObjType
{ VoxType::Weapon,
  0.0125,
  VoxWeapon::create,
  {{ "Hold",       kVPF_NONE },
   { "Strike",     kVPF_CenterOfGravity }},

  {{ "HL_Handle",   {vec3(0, -90, 0)} },
   { "HR_Handle",   {vec3(0,  90, 0)} }}
};


VoxObjUP VoxWeapon::create(const std::shared_ptr<QbtNode>& pRootNode)
{
    VoxObjType vot;
    PANIC_IF(!is_of_type(pRootNode, vot), "VoxObjType mismatch");

    VoxObjUP pvo(GNEW(kMEM_Chef, VoxWeapon, pRootNode, vot));

    const auto pBaseNode = pRootNode->findChild("Base");

    pvo->processBaseMatrices(*pBaseNode);
    pvo->buildGeometry();

    // build skeleton, process nulls
    pvo->pVoxSkel.reset(GNEW(kMEM_Chef, VoxSkel, pvo.get(), kVSC_ObjOffset));

    return pvo;
}

bool VoxWeapon::is_of_type(const std::shared_ptr<QbtNode>& pRootNode, VoxObjType & voxObjType)
{
    const auto pBase = pRootNode->findChild("Base");
    if (!pBase)
        return false;
    if (!VoxObjType::do_parts_match(kVoxObjType, pBase))
        return false;
    voxObjType = kVoxObjType;
    return true;
}

Vector<kMEM_Chef, ChefString> VoxWeapon::exportFiles(const ChefString & directory) const
{
    return VoxExporter::write_files(*pBaseGeo,
                                    pVoxSkel.get(),
                                    type.voxelSize,
                                    *pGimgDiffuse,
                                    pRootNode->fullName + "-Base",
                                    directory);
}

} // namespace gaen
