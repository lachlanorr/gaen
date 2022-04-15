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
#include "gaen/assets/file_utils.h"

#include "gaen/voxel/qbt.h"
#include "gaen/voxel/VoxExporter.h"
#include "gaen/voxel/types/VoxBiped.h"

namespace gaen
{

static const VoxObjType kVoxObjType
{ VoxType::Biped,
  0.0125,
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

Vector<kMEM_Chef, ChefString> VoxBiped::exportFiles(const ChefString & directory) const
{
    return VoxExporter::write_files(*pBaseGeo,
                                    pVoxSkel.get(),
                                    type.voxelSize,
                                    *pGimgDiffuse,
                                    pRootNode->fullName + "-Base",
                                    directory);
/*
    std::array<char, 1024> tempStr;

    // mtl file
    FileWriter mtlWrtr(mtlPath.c_str());
    snprintf(tempStr.data(), tempStr.size(), "newmtl %s_Material\n", baseName.c_str());
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "Ka 0.0 0.0 0.0\n");
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "Kd 1.0 1.0 1.0\n");
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "Ks 0.0 0.0 0.0\n");
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "d 1.0\n");
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "illum 1\n");
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "map_Kd %s\n", get_filename(pngPath).c_str());
    mtlWrtr.write(tempStr.data());

    // png file
    Png::write_gimg(pngPath.c_str(), pGimgDiffuse.get(), true);





*/


/*
    HashMap<kMEM_Chef, vec3, u32> pointIndices;
    Vector<kMEM_Chef, vec3> points;

    HashMap<kMEM_Chef, vec2, u32> uvIndices;
    Vector<kMEM_Chef, vec2> uvs;

    static const Vector<kMEM_Chef, vec3> normals{
        vec3(-1,  0,  0), // kVSD_Left
        vec3( 0,  0, -1), // kVSD_Back
        vec3( 0, -1,  0), // kVSD_Bottom
        vec3( 1,  0,  0), // kVSD_Right
        vec3( 0,  0,  1), // kVSD_Front
        vec3( 0,  1,  0)  // kVSD_Top
    };

    struct Face
    {
        u32 points[4];
        u32 uvs[4];
        u32 normal;
    };

    HashMap<kMEM_Chef, ChefString, Vector<kMEM_Chef, Face>> faceMap;

    for (const auto & part : type.parts)
    {
        pointIndices.clear();

        const VoxMatrix & matrix = *baseMatrices.find(part.name)->second;
        // build face
        for (const auto & matFace : matrix.faces)
        {
            auto & faces = faceMap[matrix.node.name];

            Face face;
            //face.uv = quad.pixelIdx;
            face.normal = matFace.pFace->side;
            for (u32 i = 0; i < 4; ++i)
            {
                u32 pointIdx = 0;
                vec3 scaledVert = matFace.points[i] * voxelSize;
                auto itPnt = pointIndices.find(scaledVert);
                if (itPnt != pointIndices.end())
                {
                    pointIdx = itPnt->second;
                }
                else
                {
                    pointIdx = points.size();
                    points.push_back(scaledVert);
                    pointIndices[scaledVert] = pointIdx;
                }
                face.points[i] = pointIdx;

                u32 uvIdx = 0;
                auto itUv = uvIndices.find(matFace.uvs[i]);
                if (itUv != uvIndices.end())
                {
                    uvIdx = itUv->second;
                }
                else
                {
                    uvIdx = uvs.size();
                    uvs.push_back(matFace.uvs[i]);
                    uvIndices[matFace.uvs[i]] = uvIdx;
                }
                face.uvs[i] = uvIdx;
            }
            faces.push_back(face);
        }
    }

    ChefString pathRoot = basePath;
    strip_ext(pathRoot);
    ChefString baseName = get_filename(pathRoot);
    ChefString objPath = pathRoot + ".obj";
    ChefString sklPath = pathRoot + ".skl";
    ChefString mtlPath = pathRoot + ".mtl";
    ChefString pngPath = pathRoot + ".png";

    std::array<char, 1024> tempStr;

    // mtl file
    FileWriter mtlWrtr(mtlPath.c_str());
    snprintf(tempStr.data(), tempStr.size(), "newmtl %s_Material\n", baseName.c_str());
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "Ka 0.0 0.0 0.0\n");
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "Kd 1.0 1.0 1.0\n");
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "Ks 0.0 0.0 0.0\n");
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "d 1.0\n");
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "illum 1\n");
    mtlWrtr.write(tempStr.data());
    snprintf(tempStr.data(), tempStr.size(), "map_Kd %s\n", get_filename(pngPath).c_str());
    mtlWrtr.write(tempStr.data());

    // png file
    Png::write_gimg(pngPath.c_str(), pGimgDiffuse.get(), true);

    // obj file
    FileWriter objWrtr(objPath.c_str());
    snprintf(tempStr.data(), tempStr.size(), "mtllib %s\n", get_filename(mtlPath).c_str());
    objWrtr.write(tempStr.data());
    objWrtr.write("\n");

    for (const auto & norm : normals)
    {
        snprintf(tempStr.data(), tempStr.size(), "vn %0.1f %0.1f %0.1f\n", norm.x, norm.y, norm.z);
        objWrtr.write(tempStr.data());
    }
    objWrtr.write("\n");

    for (const auto & point : points)
    {
        snprintf(tempStr.data(), tempStr.size(), "v %f %f %f\n", point.x, point.y, point.z);
        objWrtr.write(tempStr.data());
    }
    objWrtr.write("\n");

    for (const auto & uv : uvs)
    {
        snprintf(tempStr.data(), tempStr.size(), "vt %f %f\n", uv.x, uv.y);
        objWrtr.write(tempStr.data());
    }
    objWrtr.write("\n");

    for (const auto & part : type.parts)
    {
        snprintf(tempStr.data(), tempStr.size(), "g %s\n", part.name.c_str());
        objWrtr.write(tempStr.data());
        snprintf(tempStr.data(), tempStr.size(), "usemtl %s_Material\n", baseName.c_str());
        objWrtr.write(tempStr.data());

        auto & faces = faceMap[part.name];
        for (const auto & face : faces)
        {
            snprintf(tempStr.data(), tempStr.size(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
                     face.points[0]+1, face.uvs[0]+1, face.normal+1,
                     face.points[1]+1, face.uvs[1]+1, face.normal+1,
                     face.points[2]+1, face.uvs[2]+1, face.normal+1,
                     face.points[3]+1, face.uvs[3]+1, face.normal+1);
            objWrtr.write(tempStr.data());
        }
    }

    if (voxSkel.getRoot())
        voxSkel.writeSkl(sklPath, voxelSize);
*/
}

} // namespace gaen
