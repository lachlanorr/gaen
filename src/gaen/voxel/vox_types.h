//------------------------------------------------------------------------------
// vox_types.h - Utilities to identify VoxObj types
//
// aen Concurrency Engine - http://gaen.org
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

#ifndef GAEN_VOXEL_VOX_TYPES_H
#define GAEN_VOXEL_VOX_TYPES_H

#include "gaen/core/String.h"
#include "gaen/core/Vector.h"

namespace gaen
{

struct QbtNode;
struct VoxObj;

enum class VoxType
{
    Prop = 0,
    Biped = 1,
    Weapon = 2,
    Bow = 3,
    Missile = 4
};

const ChefString & vox_type_str(VoxType type);

enum VoxPartFlags
{
    kVPF_NONE = 0,

    kVPF_CenterOfGravity = 0x01
};

struct VoxPartDetails
{
    ChefString name;
    u32 flags;
};

struct VoxNullDetails
{
    vec3 preRot;
};

struct VoxObjType
{
    VoxType type;
    f32 voxelSize;
    UniquePtr<VoxObj>(*create)(const std::shared_ptr<QbtNode>&);
    Vector<kMEM_Chef, VoxPartDetails> parts;
    HashMap<kMEM_Chef, ChefString, VoxNullDetails> nulls;

    static bool do_parts_match(const VoxObjType & objType, const std::shared_ptr<QbtNode> & pNode);
    static bool determine_type(const std::shared_ptr<QbtNode> & pNode, VoxObjType & voxObjType);
};

struct VoxObjTypeDelegator
{
    VoxType type;
    bool (*is_of_type)(const std::shared_ptr<QbtNode>&, VoxObjType&);
};

} // namespace gaen

#endif // #ifndef GAEN_VOXEL_TYPES_H
