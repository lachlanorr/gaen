//------------------------------------------------------------------------------
// VoxBiped.cpp - Voxel Biped Geometry
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

#include "gaen/voxel/qbt.h"
#include "gaen/voxel/VoxBiped.h"

namespace gaen
{

static const VoxObjType kVoxObjType
{ VoxType::Biped,
  VoxBiped::create,
  {{ "Hips",       kVPF_CenterOfMass },
   { "L_Thigh",    kVPF_CenterOfMass },
   { "L_Calf",     kVPF_CenterOfMass },
   { "L_Heel",     kVPF_CenterOfMass },
   { "L_Toes",     kVPF_CenterOfMass },
   { "R_Thigh",    kVPF_CenterOfMass },
   { "R_Calf",     kVPF_CenterOfMass },
   { "R_Heel",     kVPF_CenterOfMass },
   { "R_Toes",     kVPF_CenterOfMass },
   { "Waist",      kVPF_CenterOfMass },
   { "Chest",      kVPF_CenterOfMass },
   { "Head",       kVPF_CenterOfMass },
   { "L_Upperarm", kVPF_NONE },
   { "L_Forearm",  kVPF_NONE },
   { "L_Hand",     kVPF_NONE },
   { "L_Digit_0",  kVPF_NONE },
   { "L_Digit_1",  kVPF_NONE },
   { "L_Digit_2",  kVPF_NONE },
   { "L_Thumb_0",  kVPF_NONE },
   { "L_Thumb_1",  kVPF_NONE },
   { "R_Upperarm", kVPF_NONE },
   { "R_Forearm",  kVPF_NONE },
   { "R_Hand",     kVPF_NONE },
   { "R_Digit_0",  kVPF_NONE },
   { "R_Digit_1",  kVPF_NONE },
   { "R_Digit_2",  kVPF_NONE },
   { "R_Thumb_0",  kVPF_NONE },
   { "R_Thumb_1",  kVPF_NONE }},

  {{ "HL_Hand_In",   {vec3(0, 0,  90)} },
   { "HL_Hand_Out",  {vec3(0, 0,  90)} },
   { "HR_Hand_In",   {vec3(0, 0, -90)} },
   { "HR_Hand_Out",  {vec3(0, 0, -90)} }}
};


VoxObjUP VoxBiped::create(const std::shared_ptr<QbtNode>& pRootNode)
{
    VoxObjType vot;
    PANIC_IF(!is_of_type(pRootNode, vot), "VoxObjType mismatch");

    VoxObjUP pvo(GNEW(kMEM_Chef, VoxBiped, pRootNode, vot));

    const auto pBaseNode = pRootNode->findChild("Base");

    pvo->processBaseMatrices(*pBaseNode);

    // build skeleton, process nulls
    pvo->pVoxSkel.reset(GNEW(kMEM_Chef, VoxSkel, pvo.get()));

    return pvo;
}

bool VoxBiped::is_of_type(const std::shared_ptr<QbtNode>& pRootNode, VoxObjType & voxObjType)
{
    const auto pBase = pRootNode->findChild("Base");
    if (!pBase)
        return false;
    if (!VoxObjType::do_parts_match(kVoxObjType, pBase))
        return false;
    voxObjType = kVoxObjType;
    return true;
}

void VoxBiped::exportFiles(const ChefString & basePath, f32 voxelSize) const
{

}

} // namespace gaen
