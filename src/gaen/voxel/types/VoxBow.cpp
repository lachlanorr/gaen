//------------------------------------------------------------------------------
// VoxBow.cpp - Voxel Bow Geometry
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
#include "gaen/voxel/types/VoxBow.h"

namespace gaen
{

static const VoxObjType kVoxObjType
{ VoxType::Bow,
  0.0125,
  VoxBow::create,
  {{ "Hold",       kVPF_CenterOfGravity },

   { "L_String_0", kVPF_NONE },
   { "R_String_0", kVPF_NONE },

   { "L_Seg_0",    kVPF_CenterOfGravity },
   { "L_Seg_1",    kVPF_CenterOfGravity },
   { "L_Seg_2",    kVPF_CenterOfGravity },
   { "L_String_1", kVPF_NONE },

   { "R_Seg_0",    kVPF_CenterOfGravity },
   { "R_Seg_1",    kVPF_CenterOfGravity },
   { "R_Seg_2",    kVPF_CenterOfGravity },
   { "R_String_1", kVPF_NONE }},

  {{ "HL_Handle",   {vec3(0, -90, 0)} },
   { "HR_Handle",   {vec3(0,  90, 0)} }}
};


VoxObjUP VoxBow::create(const std::shared_ptr<QbtNode>& pRootNode)
{
    VoxObjType vot;
    PANIC_IF(!is_of_type(pRootNode, vot), "VoxObjType mismatch");

    VoxObjUP pvo(GNEW(kMEM_Chef, VoxBow, pRootNode, vot));

    const auto pBaseNode = pRootNode->findChild("Base");

    pvo->processBaseMatrices(*pBaseNode);

    // build skeleton, process nulls
    pvo->pVoxSkel.reset(GNEW(kMEM_Chef, VoxSkel, pvo.get()));

    return pvo;
}

bool VoxBow::is_of_type(const std::shared_ptr<QbtNode>& pRootNode, VoxObjType & voxObjType)
{
    const auto pBase = pRootNode->findChild("Base");
    if (!pBase)
        return false;
    if (!VoxObjType::do_parts_match(kVoxObjType, pBase))
        return false;
    voxObjType = kVoxObjType;
    return true;
}

Vector<kMEM_Chef, ChefString> VoxBow::exportFiles(const ChefString & directory) const
{
    return VoxExporter::write_files(*pBaseGeo,
                                    pVoxSkel.get(),
                                    type.voxelSize,
                                    *pGimgDiffuse,
                                    pRootNode->fullName + "-Base",
                                    directory);
}

} // namespace gaen
