//------------------------------------------------------------------------------
// VoxSkel.h - Parses null voxels and builds skeleton heirarchy
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

#ifndef GAEN_VOXEL_VOX_SKEL_H
#define GAEN_VOXEL_VOX_SKEL_H

#include "gaen/core/String.h"
#include "gaen/core/Map.h"
#include "gaen/core/Set.h"
#include "gaen/math/mat43.h"
#include "gaen/voxel/VoxMatrix.h"

namespace gaen
{
struct Qbt;

enum VoxNullType
{
    kVNT_UNKNOWN = 0,

    kVNT_Null = 1,
    kVNT_Bone = 2,
    kVNT_Hardpoint = 3
};

const ChefString & vox_null_type_str(VoxNullType type);

class VoxSkel;
struct VoxObj;
struct VoxNull
{
    VoxNull(VoxSkel & skel,
            VoxNullType type,
            const ChefString & name,
            const ChefString & parent,
            const ChefString & group,
            vec3 pos);

    ChefString serialize(f32 voxelSize, const VoxObj * pVoxObj, const ChefString indent) const;

    VoxSkel & skel;
    VoxNullType type;
    ChefString name;
    ChefString parent;
    ChefString group;
    vec3 pos;

    bool detailsAreAvailable;
    vec3 endPos;
    vec3 dir;
    f32 length;

    mat43 worldTrans;
    mat43 localTrans;

    Set<kMEM_Chef, ChefString> children;

    static VoxNullType find_type(const ChefString & rawName);
};

typedef UniquePtr<VoxNull> VoxBoneUP;
struct FileWriter;
class VoxSkel
{
public:
    VoxSkel() {}
    VoxSkel(const VoxObj * pVoxObj);

    const VoxNull * getNull(const ChefString & name) const;
    VoxNull * getNull(const ChefString & name);

    const VoxNull * getRoot() { return mNulls.find(root)->second.get(); }

    void writeSkl(const ChefString & path, f32 voxelSize) const;
    void serializeNulls(Vector<kMEM_Chef, ChefString> & nullObjs, const ChefString & nullName, f32 voxelSize, const ChefString indent) const;

private:
    const VoxObj * pVoxObj;
    ChefString root;
    Map<kMEM_Chef, ChefString, VoxBoneUP> mNulls;
};

} // namespace gaen

#endif // #ifndef GAEN_VOXEL_SKEL_H
