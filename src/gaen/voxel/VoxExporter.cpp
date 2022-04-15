//------------------------------------------------------------------------------
// VoxExporter.cpp - Static functions to write voxel related files
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

#include "gaen/assets/file_utils.h"

#include "gaen/core/HashMap.h"
#include "gaen/core/Vector.h"

#include "gaen/image/Png.h"

#include "gaen/voxel/VoxObj.h"
#include "gaen/voxel/VoxExporter.h"

namespace gaen
{

static const Vector<kMEM_Chef, vec3> kVoxNormals{
    vec3(-1,  0,  0), // kVSD_Left
    vec3( 0,  0, -1), // kVSD_Back
    vec3( 0, -1,  0), // kVSD_Bottom
    vec3( 1,  0,  0), // kVSD_Right
    vec3( 0,  0,  1), // kVSD_Front
    vec3( 0,  1,  0)  // kVSD_Top
};

Vector<kMEM_Chef, ChefString> VoxExporter::write_files(const VoxGeo & voxGeo,
                                                       const VoxSkel * pVoxSkel,
                                                       f32 voxelSize,
                                                       const Gimg & gimg,
                                                       const ChefString & baseName,
                                                       const ChefString & directory)
{
    make_dirs(directory.c_str());

    ChefString pathRoot = directory + "/" + baseName;
    ChefString objPath = pathRoot + ".obj";
    ChefString sklPath = pathRoot + ".skl";
    ChefString mtlPath = pathRoot + ".mtl";
    ChefString mtlName = baseName + "_Material";
    ChefString pngPath = pathRoot + ".png";

    VoxExporter::write_obj_file(voxGeo, voxelSize, objPath, mtlPath, mtlName);
    VoxExporter::write_mtl_file(mtlPath, mtlName, pngPath);
    VoxExporter::write_png_file(pngPath, gimg);

    if (pVoxSkel)
    {
        pVoxSkel->writeSklFile(sklPath, voxelSize);
    }

    Vector<kMEM_Chef, ChefString> outputPaths;
    outputPaths.push_back(objPath);
    outputPaths.push_back(pngPath);

    return outputPaths;
}

void VoxExporter::write_mtl_file(const ChefString & mtlPath,
                                 const ChefString & mtlName,
                                 const ChefString & pngPath)
{
    std::array<char, 1024> tempStr;

    // mtl file
    FileWriter mtlWrtr(mtlPath.c_str());
    snprintf(tempStr.data(), tempStr.size(), "newmtl %s\n", mtlName.c_str());
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
}

void VoxExporter::write_obj_file(const VoxGeo & voxGeo,
                                 f32 voxelSize,
                                 const ChefString & objPath,
                                 const ChefString & mtlPath,
                                 const ChefString & mtlName)
{
    HashMap<kMEM_Chef, vec3, u32> pointIndices;
    Vector<kMEM_Chef, vec3> points;

    HashMap<kMEM_Chef, vec2, u32> uvIndices;
    Vector<kMEM_Chef, vec2> uvs;

    struct Face
    {
        u32 pointCount;
        u32 points[kMaxVoxPolyPoints];
        u32 uvs[kMaxVoxPolyPoints];
        u32 normal;
    };

    HashMap<kMEM_Chef, ChefString, Vector<kMEM_Chef, Face>> faceMap;

    for (const auto & pMesh : voxGeo.meshes)
    {
        pointIndices.clear();

        // build face
        for (const auto & poly : pMesh->polys)
        {
            auto & faces = faceMap[pMesh->name];

            Face face;
            face.normal = poly.side;
            face.pointCount = poly.pointCount;
            for (u32 i = 0; i < poly.pointCount; ++i)
            {
                u32 pointIdx = 0;
                vec3 scaledPos = poly.points[i].pos * voxelSize;
                auto itPnt = pointIndices.find(scaledPos);
                if (itPnt != pointIndices.end())
                {
                    pointIdx = itPnt->second;
                }
                else
                {
                    pointIdx = points.size();
                    points.push_back(scaledPos);
                    pointIndices[scaledPos] = pointIdx;
                }
                face.points[i] = pointIdx;

                u32 uvIdx = 0;
                auto itUv = uvIndices.find(poly.points[i].uv);
                if (itUv != uvIndices.end())
                {
                    uvIdx = itUv->second;
                }
                else
                {
                    uvIdx = uvs.size();
                    uvs.push_back(poly.points[i].uv);
                    uvIndices[poly.points[i].uv] = uvIdx;
                }
                face.uvs[i] = uvIdx;
            }
            faces.push_back(face);
        }
    }

    std::array<char, 1024> tempStr;

    // obj file
    FileWriter objWrtr(objPath.c_str());
    snprintf(tempStr.data(), tempStr.size(), "mtllib %s\n", get_filename(mtlPath).c_str());
    objWrtr.write(tempStr.data());
    objWrtr.write("\n");

    for (const auto & norm : kVoxNormals)
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

    for (const auto & pMesh : voxGeo.meshes)
    {
        snprintf(tempStr.data(), tempStr.size(), "g %s\n", pMesh->name.c_str());
        objWrtr.write(tempStr.data());
        snprintf(tempStr.data(), tempStr.size(), "usemtl %s\n", mtlName.c_str());
        objWrtr.write(tempStr.data());

        auto & faces = faceMap[pMesh->name];
        ChefString faceLine;
        for (const auto & face : faces)
        {
            faceLine = "f ";
            for (u32 i = 0; i < face.pointCount; ++i)
            {
                snprintf(tempStr.data(), tempStr.size(), "%d/%d/%d",
                         face.points[i]+1, face.uvs[i]+1, face.normal+1);
                faceLine += tempStr.data();
                if (i < face.pointCount-1)
                    faceLine += " ";
            }
            faceLine += "\n";
            objWrtr.write(faceLine);
        }
    }
}

void VoxExporter::write_png_file(const ChefString & pngPath, const Gimg & gimg)
{
    Png::write_gimg(pngPath.c_str(), &gimg, true);
}

} // namespace gaen
