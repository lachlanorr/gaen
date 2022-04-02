//------------------------------------------------------------------------------
// VoxSkel.cpp - Parses null voxels and builds skeleton heirarchy
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

#include <array>

#include "gaen/math/matrices.h"
#include "gaen/assets/file_utils.h"
#include "gaen/voxel/Qbt.h"
#include "gaen/voxel/VoxSkel.h"

namespace gaen
{

static const vec3 kBoneInitDir(0, 0, -1);
static const vec3 kBoneDownDir(0, -1, 0);
static const mat43 kBonePreTrans = build_rotate(kBoneInitDir, kBoneDownDir);

struct RawNull
{
    vec3 pos;
    ChefString parent;
    ChefString group;
};
typedef HashMap<kMEM_Chef, ChefString, RawNull> RawNullMap;

const ChefString & vox_null_type_str(VoxNullType type)
{
    static const HashMap<kMEM_Chef, VoxNullType, ChefString> sTypeMap{
        { kVNT_Null, "Null" },
        { kVNT_Bone, "Bone" },
        { kVNT_Hardpoint, "Hardpoint" }
    };

    const auto it = sTypeMap.find(type);
    PANIC_IF(it == sTypeMap.end(), "Invalid VoxNullType: %d", type);
    return it->second;
}

static void calc_details(VoxNull * pNull, RawNullMap & rawNulls)
{
    if (pNull->detailsAreAvailable)
        return;

    VoxNull *pParent = nullptr;
    if (pNull->parent != "")
    {
        pParent = pNull->skel.getNull(pNull->parent);
        if (pParent != nullptr)
        {
            calc_details(pParent, rawNulls);
        }
    }

    if (pNull->type == kVNT_Bone)
    {
        ChefString endName = "E" + pNull->name.substr(1);
        auto eIt = rawNulls.find(endName);
        PANIC_IF(eIt == rawNulls.end(), "No end null defined for bone %s", endName.c_str());
        pNull->endPos = eIt->second.pos;
        vec3 dirFull = pNull->endPos - pNull->pos;
        pNull->length = dirFull.length();
        pNull->dir = normalize(dirFull);

        mat43 rotMat = build_rotate(kBoneInitDir, pNull->dir);
        pNull->worldTrans = build_translate(pNull->pos) * rotMat;
    }
    else
    {
        pNull->worldTrans = build_translate(pNull->pos);
    }

    if (pNull->group.empty())
    {
        if (pParent)
            pNull->group = pParent->group;
        else
            pNull->group = "default";
    }

    if (pParent)
    {
        pNull->localTrans = pParent->worldTrans.inverse() * pNull->worldTrans;
        pParent->children.insert(pNull->name);
    }

    pNull->detailsAreAvailable = true;
}

VoxNullType VoxNull::find_type(const ChefString & rawName)
{
    switch(rawName[0])
    {
    case 'N':
        return kVNT_Null;
    case 'B':
        return kVNT_Bone;
    case 'H':
        return kVNT_Hardpoint;
    default:
        return kVNT_UNKNOWN;
    }
}


VoxNull::VoxNull(VoxSkel & skel,
                 VoxNullType type,
                 const ChefString & name,
                 const ChefString & parent,
                 const ChefString & group,
                 vec3 pos)
  : skel(skel)
  , type(type)
  , name(name)
  , shortName(name.substr(1))
  , parent(parent)
  , group(group)
  , pos(pos)
  , detailsAreAvailable(false)
  , endPos(pos)
  , dir(0.0f)
  , length(0.0f)
  , worldTrans(1.0f)
  , localTrans(1.0f)
{}

VoxSkel::VoxSkel(const Qbt & qbt)
{
    VoxMatrixMap skelMatrices;

    const QbtNode * pSkelNode = qbt.findTopLevelCompound("Skeleton");
    if (pSkelNode == nullptr)
    {
        return;
    }

    add_matrices_to_map(skelMatrices, *pSkelNode);

    // Capture all nulls into RawNulls so we can match up start/end bones and such
    RawNullMap rawNulls;
    for (const auto & matPair : skelMatrices)
    {
        const ChefString & name = matPair.first;
        const VoxMatrix & mat = *matPair.second;

        size_t start = 0;
        while (start < name.size())
        {
            size_t delimPos = name.find("__", start);
            if (delimPos == std::string::npos)
            {
                delimPos = name.size();
            }
            ChefString nullName = name.substr(start, delimPos - start);
            // check for parent
            ChefString parent = "";
            ChefString group = "";

            size_t dashPos = nullName.find('-', 0);
            if (dashPos != std::string::npos)
            {
                parent = nullName.substr(dashPos+1);
                nullName = nullName.substr(0, dashPos);

                dashPos = parent.find('-', 0);
                if (dashPos != std::string::npos)
                {
                    group = parent.substr(dashPos+1);
                    parent = parent.substr(0, dashPos);
                }
            }
            if (parent == "" && nullName[0] == 'N')
            {
                PANIC_IF(root != "", "More than one root null: '%s' and '%s'", root.c_str(), nullName.c_str());
                root = nullName;
            }
            PANIC_IF(parent == "" && nullName[0] != 'N' && nullName[0] != 'E', "Null missing parent: %s", nullName.c_str());

            rawNulls.emplace(nullName, RawNull{mat.worldCenter, parent, group});
            start = delimPos + 2;
        }
    }
    PANIC_IF(root == "", "No root null");

    // Make pass through raw nulls and add to mNulls
    for (const auto & nullPair : rawNulls)
    {
        const ChefString & rawName = nullPair.first;
        const RawNull & rawNull = nullPair.second;

        VoxNullType type = VoxNull::find_type(rawName);

        if (type != kVNT_UNKNOWN)
        {
            ChefString name = rawName;
            ChefString parent = rawNull.parent.empty() ? "" : rawNull.parent;
            mNulls.emplace(name, GNEW(kMEM_Chef, VoxNull, *this, type, name, parent, rawNull.group, rawNull.pos));
        }
    }

    // Calc details of all nulls
    for (auto & nullPair : mNulls)
    {
        VoxNull * pNull = nullPair.second.get();
        calc_details(pNull, rawNulls);
    }
}

VoxNull * VoxSkel::getNull(const ChefString & name)
{
    const VoxNull * pNull = const_cast<const VoxSkel*>(this)->getNull(name);
    if (pNull != nullptr)
    {
        return const_cast<VoxNull*>(pNull);
    }
    return nullptr;
}

const VoxNull * VoxSkel::getNull(const ChefString & name) const
{
    auto it = mNulls.find(name);
    if (it == mNulls.end())
        return nullptr;
    return it->second.get();
}

ChefString serialize_transform(const mat43 & trans, f32 voxelSize, const ChefString indent)
{
    ChefString ser;
    std::array<char, 1024> tempStr;
    ChefString indent2 = indent + "  ";

    ser += "[\n";
    snprintf(tempStr.data(), tempStr.size(),
             "[%f, %f, %f, %f],\n", trans.cols[0][0], trans.cols[0][1], trans.cols[0][2], 0.0f);
    ser += indent2 + tempStr.data();
    snprintf(tempStr.data(), tempStr.size(),
             "[%f, %f, %f, %f],\n", trans.cols[1][0], trans.cols[1][1], trans.cols[1][2], 0.0f);
    ser += indent2 + tempStr.data();
    snprintf(tempStr.data(), tempStr.size(),
             "[%f, %f, %f, %f],\n", trans.cols[2][0], trans.cols[2][1], trans.cols[2][2], 0.0f);
    ser += indent2 + tempStr.data();
    vec3 swtrans = trans.cols[3] * voxelSize;
    snprintf(tempStr.data(), tempStr.size(),
             "[%f, %f, %f, %f]\n", swtrans.x, swtrans.y, swtrans.z, 0.0f);
    ser += indent2 + tempStr.data();
    ser += indent + "]";

    return ser;
}

ChefString VoxNull::serialize(f32 voxelSize, const ChefString indent) const
{
    ChefString ser;
    std::array<char, 1024> tempStr;
    ChefString indent2 = indent + "  ";

    ser += indent + "{\n";

    snprintf(tempStr.data(), tempStr.size(),
             "\"name\": \"%s\",\n", name.c_str());
    ser += indent2 + tempStr.data();

    snprintf(tempStr.data(), tempStr.size(),
             "\"shortName\": \"%s\",\n", shortName.c_str());
    ser += indent2 + tempStr.data();

    snprintf(tempStr.data(), tempStr.size(),
             "\"type\": \"%s\",\n", vox_null_type_str(type).c_str());
    ser += indent2 + tempStr.data();

    snprintf(tempStr.data(), tempStr.size(),
             "\"parent\": %s,\n", parent.empty() ? "null" : ("\"" + parent + "\"").c_str());
    ser += indent2 + tempStr.data();

    if (type == kVNT_Bone)
    {
        snprintf(tempStr.data(), tempStr.size(),
                 "\"group\": \"%s\",\n", group.c_str());
        ser += indent2 + tempStr.data();
    }

    vec3 spos = pos * voxelSize;
    snprintf(tempStr.data(), tempStr.size(),
             "\"pos\": [%f, %f, %f],\n", spos.x, spos.y, spos.z);
    ser += indent2 + tempStr.data();
    if (type == kVNT_Bone)
    {
        vec3 sendPos = endPos * voxelSize;
        snprintf(tempStr.data(), tempStr.size(),
                 "\"endPos\": [%f, %f, %f],\n", sendPos.x, sendPos.y, sendPos.z);
        ser += indent2 + tempStr.data();

        snprintf(tempStr.data(), tempStr.size(),
                 "\"dir\": [%f, %f, %f],\n", dir.x, dir.y, dir.z);
        ser += indent2 + tempStr.data();

        f32 slength = length * voxelSize;
        snprintf(tempStr.data(), tempStr.size(),
                 "\"length\": %f,\n", slength);
        ser += indent2 + tempStr.data();
    }

    ser += "    \"worldTrans\": " + serialize_transform(worldTrans, voxelSize, indent2) + ",\n";
    ser += "    \"localTrans\": " + serialize_transform(localTrans, voxelSize, indent2) + ",\n";

    ser += "    \"children\": [";
    for (const auto & child : children)
    {
        snprintf(tempStr.data(), tempStr.size(),
                 "\"%s\"", child.c_str());
        ser += tempStr.data();
        if (&child != &(*children.rbegin()))
        {
            ser += ", ";
        }
    }
    ser += "]\n";

    ser += "  }";

    return ser;
}

void VoxSkel::writeSkl(const ChefString & path, f32 voxelSize) const
{
    Vector<kMEM_Chef, ChefString> nullObjs;

    serializeNulls(nullObjs, root, voxelSize);

    FileWriter sklWrtr(path.c_str());

    sklWrtr.write("[\n");
    for (const auto & obj : nullObjs)
    {
        sklWrtr.write(obj);

        if (&obj != &nullObjs.back())
        {
            sklWrtr.write(",");
        }
        sklWrtr.write("\n");
    }
    sklWrtr.write("]\n");
}

void VoxSkel::serializeNulls(Vector<kMEM_Chef, ChefString> & nullObjs, const ChefString & nullName, f32 voxelSize) const
{
    const auto nullIt = mNulls.find(nullName);
    PANIC_IF(nullIt == mNulls.end(), "Null not found: %s", nullName.c_str());

    const auto & null = *nullIt->second;

    nullObjs.push_back(null.serialize(voxelSize, "  "));

    for (const auto & childName : null.children)
    {
        serializeNulls(nullObjs, childName, voxelSize);
    }
}

} // namespace gaen
