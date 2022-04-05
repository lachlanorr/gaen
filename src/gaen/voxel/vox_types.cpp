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
       { "R_Thumb_1",  kVPF_NONE }}
    }
};

const ChefString & vox_type_str(VoxType type)
{
    static const HashMap<kMEM_Chef, VoxType, ChefString> sTypeMap{
        { VoxType::Other, "other" },
        { VoxType::Biped, "biped" }
    };

    const auto it = sTypeMap.find(type);
    PANIC_IF(it == sTypeMap.end(), "Invalid VoxType: %d", type);
    return it->second;
}

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
        otherType.parts.push_back({matrix.second->node.name, kVPF_CenterOfMass});
    }
    return otherType;
}

} // namespace gaen
