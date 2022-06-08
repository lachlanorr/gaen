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

#include <glm/gtx/euler_angles.hpp>


#include "gaen/math/matrices.h"
#include "gaen/math/mat3.h"
#include "gaen/assets/file_utils.h"
#include "gaen/voxel/Qbt.h"
#include "gaen/voxel/VoxObj.h"
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
        { kVNT_Null, "null" },
        { kVNT_Bone, "bone" },
        { kVNT_Hardpoint, "hardpoint" }
    };

    const auto it = sTypeMap.find(type);
    PANIC_IF(it == sTypeMap.end(), "Invalid VoxNullType: %d", type);
    return it->second;
}

static void calc_details(VoxNull * pNull, const vec3 & voxObjCenter, const vec3 & offset, RawNullMap & rawNulls)
{
    if (pNull->detailsAreAvailable)
        return;

    VoxNull *pParent = nullptr;
    if (pNull->parent != "")
    {
        pParent = pNull->skel.getNull(pNull->parent);
        if (pParent != nullptr)
        {
            calc_details(pParent, voxObjCenter, offset, rawNulls);
        }
    }

    if (pNull->type == kVNT_Bone)
    {
        ChefString endName = "E" + pNull->name.substr(1);
        auto eIt = rawNulls.find(endName);
        PANIC_IF(eIt == rawNulls.end(), "No end null defined for bone %s", endName.c_str());
        pNull->endPos = eIt->second.pos + offset;
        vec3 dirFull = pNull->endPos - pNull->pos;
        pNull->length = length(dirFull);
        pNull->dir = normalize(dirFull);

        mat43 rotMat = build_rotate(kBoneInitDir, pNull->dir) * pNull->preRot;
        pNull->worldTrans = build_translate(pNull->pos) * rotMat;
    }
    else
    {
        pNull->worldTrans = build_translate(pNull->pos) * pNull->preRot;
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
        if (pNull->type == kVNT_Bone)
        {
            // bones just have rotates
            mat3 rot = (mat3)pNull->worldTrans;
            mat3 prot = (mat3)pParent->worldTrans;
            mat3 locRot = prot.inverse() * rot;
            pNull->localTrans = locRot;
        }
        else
        {
            pNull->localTrans = pParent->worldTrans.inverse() * pNull->worldTrans;
        }

        pParent->children.insert(pNull->name);
    }
    else
    {
        pNull->localTrans = mat43(voxObjCenter).inverse() * pNull->worldTrans;
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
                 vec3 pos,
                 const mat43 & preRot)
  : skel(skel)
  , type(type)
  , name(name)
  , parent(parent)
  , group(group)
  , pos(pos)
  , preRot(preRot)
  , detailsAreAvailable(false)
  , endPos(pos)
  , dir(0.0f)
  , length(0.0f)
  , worldTrans(1.0f)
  , localTrans(1.0f)
{}

VoxSkel::VoxSkel(const VoxObj* pVoxObj, VoxSkelCenter skelCenter)
  : pVoxObj(pVoxObj)
{
    VoxMatrixMap skelMatrices;

    const auto pSkelNode = pVoxObj->pRootNode->findChild("Skeleton");
    if (!pSkelNode)
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

            rawNulls.emplace(nullName, RawNull{mat.worldPivot, parent, group});
            start = delimPos + 2;
        }
    }
    PANIC_IF(root == "", "No root null");

    switch(skelCenter)
    {
    case kVSC_ObjOffset:
        offset = pVoxObj->offset;
        break;
    case kVSC_RootNull:
        offset = -rawNulls[root].pos;
        break;
    default:
        PANIC("Invalid value for skelCenter: %d", skelCenter);
    };

    // duplicate any L* nulls into R* versions
    RawNullMap addtlNulls;
    for (const auto & nullPair : rawNulls)
    {
        const ChefString & rawName = nullPair.first;
        const RawNull & rawNull = nullPair.second;

        ChefString rhs = "";
        if (rawName.find("BL_") == 0)
        {
            rhs = "BR_" + rawName.substr(3);
        }
        else if (rawName.find("EL_") == 0)
        {
            rhs = "ER_" + rawName.substr(3);
        }
        else if (rawName.find("NL_") == 0)
        {
            rhs = "NR_" + rawName.substr(3);
        }
        else if (rawName.find("HL_") == 0)
        {
            rhs = "HR_" + rawName.substr(3);
        }

        if (!rhs.empty())
        {
            // check to see if rhs already exists
            if (rawNulls.find(rhs) == rawNulls.end())
            {
                ChefString rhsParent;
                if (rawNull.parent.find("BL_") == 0)
                {
                    rhsParent = "BR_" + rawNull.parent.substr(3);
                }
                else if (rawNull.parent.find("EL_") == 0)
                {
                    rhsParent = "ER_" + rawNull.parent.substr(3);
                }
                else if (rawNull.parent.find("NL_") == 0)
                {
                    rhsParent = "NR_" + rawNull.parent.substr(3);
                }
                else if (rawNull.parent.find("HL_") == 0)
                {
                    rhsParent = "HR_" + rawNull.parent.substr(3);
                }
                else
                {
                    rhsParent = rawNull.parent;
                }

                RawNull rhsNull;
                rhsNull.pos = vec3(rawNulls[root].pos.x - (rawNull.pos.x - rawNulls[root].pos.x), rawNull.pos.y, rawNull.pos.z);
                rhsNull.parent = rhsParent;
                rhsNull.group = rawNull.group;
                addtlNulls[rhs] = rhsNull;
            }
        }
    }
    // Add our additional rhs nulls to rawNulls
    for (const auto & addtlNullPair : addtlNulls)
    {
        rawNulls[addtlNullPair.first] = addtlNullPair.second;
    }

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

            mat43 preRot(1.0f);
            mat43 preRotX, preRotY, preRotZ;
            const auto nullDetailsIt = pVoxObj->type.nulls.find(name);
            if (nullDetailsIt != pVoxObj->type.nulls.end())
            {
                vec3 radPreRot = radians(nullDetailsIt->second.preRot);

                preRotX = mat43(glm::eulerAngleX(radPreRot.x));
                preRotY = mat43(glm::eulerAngleY(radPreRot.y));
                preRotZ = mat43(glm::eulerAngleZ(radPreRot.z));
                preRot = preRotZ * preRotY * preRotX;
            }

            mNulls.emplace(name, GNEW(kMEM_Chef, VoxNull, *this, type, name, parent, rawNull.group, rawNull.pos + offset, preRot));
        }
    }

    // Calc details of all nulls
    for (auto & nullPair : mNulls)
    {
        VoxNull * pNull = nullPair.second.get();
        calc_details(pNull, pVoxObj->cogCenter + offset, offset, rawNulls);
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
             "[%f, %f, %f, %f]\n", swtrans.x, swtrans.y, swtrans.z, 1.0f);
    ser += indent2 + tempStr.data();
    ser += indent + "]";

    return ser;
}

ChefString VoxNull::serialize(f32 voxelSize, const VoxObj * pVoxObj, const ChefString indent) const
{
    ChefString ser;
    std::array<char, 1024> tempStr;
    ChefString indent2 = indent + "  ";

    ser += indent + "{\n";

    snprintf(tempStr.data(), tempStr.size(),
             "\"name\": \"%s\",\n", name.c_str());
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

        // find matrix to get center and extents
        const auto matIt = pVoxObj->baseMatrices.find(name.substr(1));
        if (matIt != pVoxObj->baseMatrices.end())
        {
            ser += indent2 + "\"hasGeometry\": true,\n";

            const auto & mat = *matIt->second;
            vec3 scenter = (mat.worldCenter + pVoxObj->offset) * voxelSize;
            snprintf(tempStr.data(), tempStr.size(),
                     "\"center\": [%f, %f, %f],\n", scenter.x, scenter.y, scenter.z);
            ser += indent2 + tempStr.data();

            vec3 shalfExtents = mat.halfExtents * voxelSize;
            snprintf(tempStr.data(), tempStr.size(),
                     "\"halfExtents\": [%f, %f, %f],\n", shalfExtents.x, shalfExtents.y, shalfExtents.z);
            ser += indent2 + tempStr.data();
        }
        else
        {
            ser += indent2 + "\"hasGeometry\": false,\n";
        }
    }

    ser += indent2 + "\"worldTransform\": " + serialize_transform(worldTrans, voxelSize, indent2) + ",\n";
    ser += indent2 + "\"localTransform\": " + serialize_transform(localTrans, voxelSize, indent2) + ",\n";

    ser += indent2 + "\"children\": [";
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

    ser += indent + "}";

    return ser;
}

void VoxSkel::writeSklFile(const ChefString & path, f32 voxelSize) const
{
    std::array<char, 1024> tempStr;

    Vector<kMEM_Chef, ChefString> nullObjs;

    serializeNulls(nullObjs, root, voxelSize, "    ");

    FileWriter sklWrtr(path.c_str());

    sklWrtr.write("{\n");

    snprintf(tempStr.data(), tempStr.size(),
             "  \"type\": \"%s\",\n", vox_type_str(pVoxObj->type.type).c_str());
    sklWrtr.write(tempStr.data());

    vec3 scenter = (pVoxObj->cogCenter + pVoxObj->offset) * voxelSize;
    snprintf(tempStr.data(), tempStr.size(),
             "  \"center\": [%f, %f, %f],\n", scenter.x, scenter.y, scenter.z);
    sklWrtr.write(tempStr.data());

    vec3 shalfExtents = pVoxObj->cogHalfExtents * voxelSize;
    snprintf(tempStr.data(), tempStr.size(),
             "  \"halfExtents\": [%f, %f, %f],\n", shalfExtents.x, shalfExtents.y, shalfExtents.z);
    sklWrtr.write(tempStr.data());

    sklWrtr.write("  \"skeleton\": [\n");
    for (const auto & obj : nullObjs)
    {
        sklWrtr.write(obj);

        if (&obj != &nullObjs.back())
        {
            sklWrtr.write(",");
        }
        sklWrtr.write("\n");
    }
    sklWrtr.write("  ]\n");
    sklWrtr.write("}\n");
}

void VoxSkel::serializeNulls(Vector<kMEM_Chef, ChefString> & nullObjs, const ChefString & nullName, f32 voxelSize, const ChefString indent) const
{
    const auto nullIt = mNulls.find(nullName);
    PANIC_IF(nullIt == mNulls.end(), "Null not found: %s", nullName.c_str());

    const auto & null = *nullIt->second;

    nullObjs.push_back(null.serialize(voxelSize, pVoxObj, indent));

    for (const auto & childName : null.children)
    {
        serializeNulls(nullObjs, childName, voxelSize, indent);
    }
}

} // namespace gaen
