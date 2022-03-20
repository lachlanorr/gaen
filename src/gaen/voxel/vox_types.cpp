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

#include "gaen/voxel/VoxObj.h"
#include "gaen/voxel/vox_types.h"

namespace gaen
{

static const Vector<kMEM_Chef, VoxObjType> kVoxObjTypes
{
    { VoxType::Biped,
        {
            { "Hips", "Lower", "" },
            { "L_Thigh", "Lower", "Hips" },
            { "L_Calf", "Lower", "L_Thigh" },
            { "L_Heel", "Lower", "L_Calf" },
            { "L_Toes", "Lower", "L_Heel" },
            { "R_Thigh", "Lower", "Hips" },
            { "R_Calf", "Lower", "R_Thigh" },
            { "R_Heel", "Lower", "R_Calf" },
            { "R_Toes", "Lower", "R_Heel" },
            { "Waist", "Upper", "" },
            { "Chest", "Upper", "Waist" },
            { "Head", "Upper", "Chest" },
            { "L_Upperarm", "Upper", "Chest" },
            { "L_Forearm", "Upper", "L_Upperarm" },
            { "L_Hand", "Upper", "L_Forearm" },
            { "L_Digit_0", "Upper", "L_Hand" },
            { "L_Digit_1", "Upper", "L_Digit_0" },
            { "L_Digit_2", "Upper", "L_Digit_1" },
            { "L_Thumb_0", "Upper", "L_Hand" },
            { "L_Thumb_1", "Upper", "L_Thumb_0" },
            { "R_Upperarm", "Upper", "Chest" },
            { "R_Forearm", "Upper", "R_Upperarm" },
            { "R_Hand", "Upper", "R_Forearm" },
            { "R_Digit_0", "Upper", "R_Hand" },
            { "R_Digit_1", "Upper", "R_Digit_0" },
            { "R_Digit_2", "Upper", "R_Digit_1" },
            { "R_Thumb_0", "Upper", "R_Hand" },
            { "R_Thumb_1", "Upper", "R_Thumb_0" }
        }
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
