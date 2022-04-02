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

#include "gaen/voxel/Qbt.h"
#include "gaen/voxel/VoxObj.h"
#include "gaen/voxel/vox_types.h"

namespace gaen
{

static const Vector<kMEM_Chef, VoxObjType> kVoxObjTypes
{
    { VoxType::Biped,
      {{ "Hips",       "Lower", "",           kVPF_CenterOfMass },
       { "L_Thigh",    "Lower", "Hips",       kVPF_CenterOfMass },
       { "L_Calf",     "Lower", "L_Thigh",    kVPF_CenterOfMass },
       { "L_Heel",     "Lower", "L_Calf",     kVPF_CenterOfMass },
       { "L_Toes",     "Lower", "L_Heel",     kVPF_CenterOfMass },
       { "R_Thigh",    "Lower", "Hips",       kVPF_CenterOfMass },
       { "R_Calf",     "Lower", "R_Thigh",    kVPF_CenterOfMass },
       { "R_Heel",     "Lower", "R_Calf",     kVPF_CenterOfMass },
       { "R_Toes",     "Lower", "R_Heel",     kVPF_CenterOfMass },
       { "Waist",      "Upper", "",           kVPF_CenterOfMass },
       { "Chest",      "Upper", "Waist",      kVPF_CenterOfMass },
       { "Head",       "Upper", "Chest",      kVPF_CenterOfMass },
       { "L_Upperarm", "Upper", "Chest",      kVPF_NONE },
       { "L_Forearm",  "Upper", "L_Upperarm", kVPF_NONE },
       { "L_Hand",     "Upper", "L_Forearm",  kVPF_NONE },
       { "L_Digit_0",  "Upper", "L_Hand",     kVPF_NONE },
       { "L_Digit_1",  "Upper", "L_Digit_0",  kVPF_NONE },
       { "L_Digit_2",  "Upper", "L_Digit_1",  kVPF_NONE },
       { "L_Thumb_0",  "Upper", "L_Hand",     kVPF_NONE },
       { "L_Thumb_1",  "Upper", "L_Thumb_0",  kVPF_NONE },
       { "R_Upperarm", "Upper", "Chest",      kVPF_NONE },
       { "R_Forearm",  "Upper", "R_Upperarm", kVPF_NONE },
       { "R_Hand",     "Upper", "R_Forearm",  kVPF_NONE },
       { "R_Digit_0",  "Upper", "R_Hand",     kVPF_NONE },
       { "R_Digit_1",  "Upper", "R_Digit_0",  kVPF_NONE },
       { "R_Digit_2",  "Upper", "R_Digit_1",  kVPF_NONE },
       { "R_Thumb_0",  "Upper", "R_Hand",     kVPF_NONE },
       { "R_Thumb_1",  "Upper", "R_Thumb_0",  kVPF_NONE }}
    }
};

const VoxObjType VoxObjType::determine_type(const VoxObj & vobj)
{
    for (const auto & objType : kVoxObjTypes)
    {
        if (vobj.baseMatrices.size() != objType.parts.size())
            continue;
        for (const auto & part : objType.parts)
        {
            if (vobj.baseMatrices.find(part.name) == vobj.baseMatrices.end())
                continue;
        }
        return objType;
    }

    VoxObjType otherType{VoxType::Other, {}};
    for (const auto & matrix : vobj.baseMatrices)
    {
        otherType.parts.push_back({matrix.second->node.name, "", ""});
    }
    return otherType;
}

} // namespace gaen
