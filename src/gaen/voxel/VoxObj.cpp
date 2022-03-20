//------------------------------------------------------------------------------
// VoxObj.cpp - Voxel Geometry
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

#include "gaen/assets/Gimg.h"
#include "gaen/assets/file_utils.h"
#include "gaen/image/Png.h"

#include "gaen/voxel/VoxObj.h"

namespace gaen
{

static void add_matrices_to_map(VoxMatrixMap & matrices, const QbtNode & node)
{
    if (node.typeId == kQBNT_Matrix)
    {
        matrices[node.name] = VoxMatrixUP(GNEW(kMEM_Chef, VoxMatrix, node));
    }
    else
    {
        for (u32 i = 0; i < node.children.size(); ++i)
        {
            add_matrices_to_map(matrices, *node.children[i]);
        }
    }
}

static VoxMatrixMap build_matrix_map(const Qbt & qbt, const char * topLevelCompound)
{
    VoxMatrixMap matrices;

    if (qbt.pRoot)
    {
        for (u32 i = 0; i < qbt.pRoot->children.size(); ++i)
        {
            if (qbt.pRoot->children[i]->name == topLevelCompound && qbt.pRoot->children[i]->typeId == kQBNT_Compound)
            {
                add_matrices_to_map(matrices, *qbt.pRoot->children[i]);
            }
        }
    }

    return matrices;
}

static const ivec3 qbt_node_world_pos(const QbtNode & node)
{
    ivec3 worldPos = node.position;
    if (node.pParent)
    {
        worldPos += qbt_node_world_pos(*node.pParent);
    }
    return worldPos;
}

static const void process_matrix(VoxPixelVec & pixels, VoxMatrix & matrix)
{
    matrix.worldPos = qbt_node_world_pos(matrix.node);
    vec3 worldPos = vec3(matrix.worldPos.x, matrix.worldPos.y, matrix.worldPos.z);

    for (i32 y = matrix.node.size.y-1; y >= 0; y--)
    {
        for (i32 z = matrix.node.size.z-1; z >= 0; z--)
        {
            for (i32 x = 0; x < matrix.node.size.x; x++)
            {
                Color col = matrix.node.voxel(x, y, z);
                if (col.a() > 1) // 1 means core voxel
                {
                    if (matrix.pixBegin == -1)
                        matrix.pixBegin = pixels.size();
                    matrix.pixEnd = pixels.size() + 1;

                    VoxPixel pix;
                    pix.color = col;
                    pix.color.seta(255);
                    pix.localPos = ivec3(x, y, z);
                    pix.worldPos = pix.localPos + matrix.worldPos;
                    pixels.push_back(pix);
                    u32 pixIdx = pixels.size() - 1;

                    // determine visible sides
                    matrix.visibleSides = 0;
                    if (x == 0 || matrix.node.voxel(x-1, y, z).a() == 0)
                    {
                        matrix.visibleSides |= kVSD_Left;
                        matrix.quads.push_back(
                            VoxQuad{
                                // verts
                                {vec3(x, y, z)+worldPos, vec3(x, y, z+1)+worldPos, vec3(x, y+1, z+1)+worldPos, vec3(x, y+1, z)+worldPos},
                                // pointer to pixel we just created
                                pixIdx,
                                kVSD_Left
                            }
                        );
                    }
                    if (z == 0 || matrix.node.voxel(x, y, z-1).a() == 0)
                    {
                        matrix.visibleSides |= kVSD_Back;
                        matrix.quads.push_back(
                            VoxQuad{
                                // verts
                                {vec3(x+1, y, z)+worldPos, vec3(x, y, z)+worldPos, vec3(x, y+1, z)+worldPos, vec3(x+1, y+1, z)+worldPos},
                                // pointer to pixel we just created
                                pixIdx,
                                kVSD_Back
                            }
                        );
                    }
                    if (y == 0 || matrix.node.voxel(x, y-1, z).a() == 0)
                    {
                        matrix.visibleSides |= kVSD_Bottom;
                        matrix.quads.push_back(
                            VoxQuad{
                                // verts
                                {vec3(x, y, z)+worldPos, vec3(x+1, y, z)+worldPos, vec3(x+1, y, z+1)+worldPos, vec3(x, y, z+1)+worldPos},
                                // pointer to pixel we just created
                                pixIdx,
                                kVSD_Bottom
                            }
                        );
                    }
                    if (x == matrix.node.size.x-1 || matrix.node.voxel(x+1, y, z).a() == 0)
                    {
                        matrix.visibleSides |= kVSD_Right;
                        matrix.quads.push_back(
                            VoxQuad{
                                // verts
                                {vec3(x+1, y, z+1)+worldPos, vec3(x+1, y, z)+worldPos, vec3(x+1, y+1, z)+worldPos, vec3(x+1, y+1, z+1)+worldPos},
                                // pointer to pixel we just created
                                pixIdx,
                                kVSD_Right
                            }
                        );
                    }
                    if (z == matrix.node.size.z-1 || matrix.node.voxel(x, y, z+1).a() == 0)
                    {
                        matrix.visibleSides |= kVSD_Front;
                        matrix.quads.push_back(
                            VoxQuad{
                                // verts
                                {vec3(x, y, z+1)+worldPos, vec3(x+1, y, z+1)+worldPos, vec3(x+1, y+1, z+1)+worldPos, vec3(x, y+1, z+1)+worldPos},
                                // pointer to pixel we just created
                                pixIdx,
                                kVSD_Front
                            }
                        );
                    }
                    if (y == matrix.node.size.y-1 || matrix.node.voxel(x, y+1, z).a() == 0)
                    {
                        matrix.visibleSides |= kVSD_Top;
                        matrix.quads.push_back(
                            VoxQuad{
                                // verts
                                {vec3(x, y+1, z+1)+worldPos, vec3(x+1, y+1, z+1)+worldPos, vec3(x+1, y+1, z)+worldPos, vec3(x, y+1, z)+worldPos},
                                // pointer to pixel we just created
                                pixIdx,
                                kVSD_Top
                            }
                        );
                    }
                }
            }
        }
    }
}

VoxObj::VoxObj(const Qbt& qbt)
  : qbt(qbt)
  , pType(nullptr)
{
    matrices = build_matrix_map(qbt, "Base");
    pType = VoxObjType::determine_type(matrices);
    PANIC_IF(pType == nullptr, "Unknown vox obj type");

    for (const auto & part : pType->parts)
    {
        process_matrix(pixels, *matrices[part.name]);
    }

    PANIC_IF(pixels.size() == 0, "No visible voxels");

    f32 pixRoot = sqrt(pixels.size());
    u32 imgWidth = next_power_of_two((u32)(pixRoot + 0.5));
    u32 imgHeight;
    if (pixels.size() <= imgWidth * imgWidth / 2)
        imgHeight = imgWidth / 2;
    else
        imgHeight = imgWidth;

    pGimgDiffuse.reset(Gimg::create(kPXL_RGBA8, imgWidth, imgHeight, 0));
    pGimgDiffuse->clear(0);
    Color * pDiffusePix = (Color*)pGimgDiffuse->pixels();

    for (u32 i = 0; i < pixels.size(); ++i)
    {
        u32 imgX = i % imgWidth;
        u32 imgY = i / imgWidth;

        // For center of pixel we're using:
        //u = x / width + 0.5 / width;
        //v = y / height + 0.5 / height;
        pixels[i].uv.x = imgX / (f32)imgWidth + 0.5 / imgWidth;
        pixels[i].uv.y = imgY / (f32)imgHeight + 0.5 / imgHeight;

        *pDiffusePix++ = pixels[i].color;
    }
}

void VoxObj::exportFiles(const ChefString & basePath, f32 scaleFactor) const
{
    HashMap<kMEM_Chef, vec3, u32> vertToPoint;
    Vector<kMEM_Chef, vec3> points;

    static const Vector<kMEM_Chef, vec3> normals{
        vec3(-1,  0,  0), // kVSD_Left
        vec3( 0,  0, -1), // kVSD_Back
        vec3( 0, -1,  0), // kVSD_Bottom
        vec3( 1,  0,  0), // kVSD_Right
        vec3( 0,  0,  1), // kVSD_Front
        vec3( 0,  1,  0)  // kVSD_Top
    };

    static const HashMap<kMEM_Chef, u32, u32> normalIdxs{
        { kVSD_Left,   0 },
        { kVSD_Back,   1 },
        { kVSD_Bottom, 2 },
        { kVSD_Right,  3 },
        { kVSD_Front,  4 },
        { kVSD_Top,    5 }
    };

    struct Face
    {
        u32 points[4];
        u32 uv;
        u32 normal;
    };

    HashMap<kMEM_Chef, ChefString, Vector<kMEM_Chef, Face>> faceMap;

    for (const auto & part : pType->parts)
    {
        const VoxMatrix & matrix = *matrices.find(part.name)->second;
        // build face
        for (const auto & quad : matrix.quads)
        {
            auto & faces = faceMap[matrix.node.name];

            Face face;
            face.uv = quad.pixelIdx;
            face.normal = normalIdxs.find(quad.side)->second;
            for (u32 i = 0; i < 4; ++i)
            {
                u32 pointIdx = 0;
                vec3 scaledVert = quad.verts[i] * scaleFactor;
                auto itPnt = vertToPoint.find(scaledVert);
                if (itPnt != vertToPoint.end())
                {
                    pointIdx = itPnt->second;
                }
                else
                {
                    pointIdx = points.size();
                    points.push_back(scaledVert);
                    vertToPoint[scaledVert] = pointIdx;
                }
                face.points[i] = pointIdx;
            }
            faces.push_back(face);
        }
    }

    ChefString pathRoot = basePath;
    strip_ext(pathRoot);
    ChefString baseName = get_filename(pathRoot);
    ChefString objPath = pathRoot + ".obj";
    ChefString mtlPath = pathRoot + ".mtl";
    ChefString pngPath = pathRoot + ".png";

    std::array<char, 1024> tempStr;

    // mtl file
    FileWriter mtlWrtr(mtlPath.c_str());
    snprintf(tempStr.data(), tempStr.size(), "newmtl %s_Material\n", baseName.c_str());
    mtlWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    snprintf(tempStr.data(), tempStr.size(), "Ka 0.0 0.0 0.0\n");
    mtlWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    snprintf(tempStr.data(), tempStr.size(), "Kd 1.0 1.0 1.0\n");
    mtlWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    snprintf(tempStr.data(), tempStr.size(), "Ks 0.0 0.0 0.0\n");
    mtlWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    snprintf(tempStr.data(), tempStr.size(), "d 1.0\n");
    mtlWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    snprintf(tempStr.data(), tempStr.size(), "illum 1\n");
    mtlWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    snprintf(tempStr.data(), tempStr.size(), "map_Kd %s\n", get_filename(pngPath).c_str());
    mtlWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));

    // png file
    Png::write_gimg(pngPath.c_str(), pGimgDiffuse.get(), true);

    // obj file
    FileWriter objWrtr(objPath.c_str());
    snprintf(tempStr.data(), tempStr.size(), "mtllib %s\n", get_filename(mtlPath).c_str());
    objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    objWrtr.ofs.write("\n", 1);

    for (const auto & norm : normals)
    {
        snprintf(tempStr.data(), tempStr.size(), "vn %0.1f %0.1f %0.1f\n", norm.x, norm.y, norm.z);
        objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    }
    objWrtr.ofs.write("\n", 1);

    for (const auto & point : points)
    {
        snprintf(tempStr.data(), tempStr.size(), "v %f %f %f\n", point.x, point.y, point.z);
        objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    }
    objWrtr.ofs.write("\n", 1);

    for (const auto & pix : pixels)
    {
        snprintf(tempStr.data(), tempStr.size(), "vt %f %f\n", pix.uv.x, pix.uv.y);
        objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    }
    objWrtr.ofs.write("\n", 1);

    for (const auto & part : pType->parts)
    {
        snprintf(tempStr.data(), tempStr.size(), "g %s\n", part.name.c_str());
        objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
        snprintf(tempStr.data(), tempStr.size(), "usemtl %s_Material\n", baseName.c_str());
        objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));

        auto & faces = faceMap[part.name];
        for (const auto & face : faces)
        {
            snprintf(tempStr.data(), tempStr.size(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
                     face.points[0]+1, face.uv+1, face.normal+1,
                     face.points[1]+1, face.uv+1, face.normal+1,
                     face.points[2]+1, face.uv+1, face.normal+1,
                     face.points[3]+1, face.uv+1, face.normal+1);
            objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
        }
    }
}

} // namespace gaen
