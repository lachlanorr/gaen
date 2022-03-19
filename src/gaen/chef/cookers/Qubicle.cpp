//------------------------------------------------------------------------------
// Qubicle.cpp - Qubicle binary file cooker
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

#include "gaen/core/String.h"
#include "gaen/core/Vector.h"
#include "gaen/core/Set.h"

#include "gaen/chef/chef.h"
#include "gaen/chef/Qbt.h"
#include "gaen/chef/Png.h"

#include "gaen/chef/cookers/Model.h"
#include "gaen/chef/cookers/Image.h"
#include "gaen/chef/cookers/Qubicle.h"

namespace gaen
{
namespace cookers
{

Qubicle::Qubicle()
{
    setVersion(1);
    addRawExt(kExtQbt);
    addCookedExt(kExtGmdl);
    addCookedExt(kExtGimg);
    addCookedExtExclusive(kExtGqbt);
}

struct VoxPart
{
    ChefString name;
    ChefString group;
    ChefString parent;
};

typedef Vector<kMEM_Chef, VoxPart> VoxParts;

struct VoxObjType
{
    ChefString name;
    VoxParts parts;
};

static const Vector<kMEM_Chef, VoxObjType> kVoxObjTypes
{
    { "Biped",
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

enum VoxSide : u32
{
    kVSD_Left   = 0x01,
    kVSD_Back   = 0x02,
    kVSD_Bottom = 0x04,
    kVSD_Right  = 0x08,
    kVSD_Front  = 0x10,
    kVSD_Top    = 0x20
};

struct VoxPixel
{
    Color color;
    ivec3 localPos;
    ivec3 worldPos;
    u32 visibleSides; // bitmask of VoxSide's
    vec2 uv;

    VoxPixel()
      : color(0,0,0,0)
      , localPos(0)
      , worldPos(0)
      , visibleSides(0)
    {}
};

struct VoxQuad
{
    vec3 verts[4];
    u32 pixelIdx;
    VoxSide side;
};

typedef Vector<kMEM_Chef, VoxPixel> VoxPixelVec;

struct VoxMatrix
{
    const QbtNode& node;

    i32 pixBegin;
    i32 pixEnd;

    ivec3 worldPos;

    u32 visibleSides;

    Vector<kMEM_Chef, VoxQuad> quads;

    VoxMatrix(const QbtNode &node)
      : node(node)
      , pixBegin(-1)
      , pixEnd(-1)
    {}
};

typedef UniquePtr<VoxMatrix> VoxMatrixUP;
typedef HashMap<kMEM_Chef, ChefString, VoxMatrixUP> VoxMatrixMap;

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

static const VoxObjType * determine_type(const VoxMatrixMap & matrices)
{
    for (const auto & objType : kVoxObjTypes)
    {
        if (matrices.size() != objType.parts.size())
            continue;
        for (const auto & part : objType.parts)
        {
            if (matrices.find(part.name) == matrices.end())
                continue;
        }
        return &objType;
    }
    return nullptr;
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

    for (u32 x = 0; x < matrix.node.size.x; x++)
    {
        for (u32 z = 0; z < matrix.node.size.z; z++)
        {
            for (u32 y = 0; y < matrix.node.size.y; y++)
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

struct VoxObj
{
    const Qbt& qbt;

    VoxMatrixMap matrices;
    const VoxObjType *pType;

    VoxPixelVec pixels;
    UniquePtr<Gimg> pGimgDiffuse;

    VoxObj(const Qbt& qbt)
      : qbt(qbt)
      , pType(nullptr)
    {
        matrices = build_matrix_map(qbt, "Base");
        pType = determine_type(matrices);
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

    void exportFiles(const ChefString & basePath, f32 scaleFactor) const
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
            u32 uvs[4];
            u32 normal;
        };

        HashMap<kMEM_Chef, ChefString, Vector<kMEM_Chef, Face>> faceMap;
        Vector<kMEM_Chef, vec2> uvAdjusted;

        f32 quarterPixel = 0.25 / (f32)pGimgDiffuse->width();
        std::array<vec2, 4> uvOff{
            vec2(-quarterPixel, quarterPixel),
            vec2(quarterPixel, quarterPixel),
            vec2(quarterPixel, -quarterPixel),
            vec2(-quarterPixel, -quarterPixel)
        };

        for (const auto & part : pType->parts)
        {
            const VoxMatrix & matrix = *matrices.find(part.name)->second;
            // build face
            for (const auto & quad : matrix.quads)
            {
                auto & faces = faceMap[matrix.node.name];

                Face face;
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

                    vec2 adjUv = pixels[quad.pixelIdx].uv + uvOff[i];
                    uvAdjusted.push_back(adjUv);
                    face.uvs[i] = uvAdjusted.size()-1;
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

        for (const auto & uvAdj : uvAdjusted)
        {
            snprintf(tempStr.data(), tempStr.size(), "vt %f %f\n", uvAdj.x, uvAdj.y);
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
                         face.points[0]+1, face.uvs[0]+1, face.normal+1,
                         face.points[1]+1, face.uvs[1]+1, face.normal+1,
                         face.points[2]+1, face.uvs[2]+1, face.normal+1,
                         face.points[3]+1, face.uvs[3]+1, face.normal+1);
                objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
            }
        }
    }
};

void Qubicle::cook(CookInfo * pCookInfo) const
{
    QbtUP pQbTree = Qbt::load_from_file(pCookInfo->rawPath().c_str());

    VoxObj vobj(*pQbTree);
    //build_diffuse_front_half(pGimg.get(), *baseMatrices["Head"]);
/*
    // Create an image for our diffuse map
    u32 glyphsPerRow = (u32)(sqrt((f32)utf32Codes.size()) + 0.5f);
    u32 imgHeight = next_power_of_two(glyphsPerRow * glyphHeight);
    Scoped_GFREE<Gimg> pGimg = Gimg::create(kPXL_R8, imgHeight, imgHeight, Image::reference_path_hash(pCookInfo));
*/
    // Make any directories needed to write transitory .png and .atl


    ChefString objTransPath = pCookInfo->chef().getRawTransPath(pCookInfo->rawPath(), kExtObj);
    ChefString pngTransPath = pCookInfo->chef().getRawTransPath(pCookInfo->rawPath(), kExtPng);

    ChefString transDir = parent_dir((const ChefString&)pngTransPath);
    make_dirs(transDir.c_str());

    vobj.exportFiles(objTransPath, 0.0125);

    //Png::write_gimg(pngTransPath.c_str(), vobj.pGimgDiffuse.get(), false);

/*

    // Write .png transitory output file
    Png::write_gimg(pngTransPath.c_str(), pGimg.get());
    UniquePtr<CookInfo> pCiPng = pCookInfo->chef().forceCook(pngTransPath);
    pCookInfo->transferCookResult(*pCiPng, kExtGimg);

    // Write .atl transitory output file
    fontAtl.write(atlTransPath.c_str());
    UniquePtr<CookInfo> pCiAtl = pCookInfo->chef().forceCook(atlTransPath);
    pCookInfo->transferCookResult(*pCiAtl, kExtGatl);
*/
    // Write a AssetHeader only g_qb file so we can track Font cooker version and force recooks.
    AssetHeader * pGqbtHeader = GNEW(kMEM_Chef, AssetHeader, FOURCC(kExtGqbt), sizeof(AssetHeader));
    pCookInfo->setCookedBuffer(pGqbtHeader);
}

} // namespace cookers
} // namespace gaen
