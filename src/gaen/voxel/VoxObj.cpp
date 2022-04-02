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
#include <algorithm>

#include "gaen/assets/Gimg.h"
#include "gaen/assets/file_utils.h"
#include "gaen/image/Png.h"
#include "gaen/image/ImagePartition.h"

#include "gaen/voxel/Qbt.h"
#include "gaen/voxel/VoxMatrix.h"

#include "gaen/voxel/VoxObj.h"

namespace gaen
{

static bool face_size_gt(const VoxMatrixFace *lhs, const VoxMatrixFace *rhs)
{
    return lhs->area > rhs->area;
}

static vec2 calc_uv(uvec2 pos, uvec2 imageSize)
{
    return vec2(pos.x / (f32)imageSize.x + 0.5 / imageSize.x,
                pos.y / (f32)imageSize.y + 0.5 / imageSize.y);
}

static void set_uvs(VoxMatrixFace * pMatFace, uvec2 pos, uvec2 size, uvec2 imageSize)
{
    pMatFace->uvs[0] = calc_uv(uvec2(pos.x,              pos.y + size.y - 1), imageSize);
    pMatFace->uvs[1] = calc_uv(uvec2(pos.x + size.x - 1, pos.y + size.y - 1), imageSize);
    pMatFace->uvs[2] = calc_uv(uvec2(pos.x + size.x - 1, pos.y),              imageSize);
    pMatFace->uvs[3] = calc_uv(uvec2(pos.x,              pos.y),              imageSize);
}

static ivec3 face_image_pos_to_voxel(const VoxFace * pFace, const uvec2 size, u32 x, u32 y)
{
    switch(pFace->side)
    {
    case kVSD_Left:
        return ivec3(pFace->start.x,
                     pFace->start.y + size.y - y - 1,
                     pFace->start.z + x);
    case kVSD_Back:
        return ivec3(pFace->start.x + size.x - x - 1,
                     pFace->start.y + size.y - y - 1,
                     pFace->start.z);
    case kVSD_Bottom:
        return ivec3(pFace->start.x + x,
                     pFace->start.y,
                     pFace->start.z + size.y - y - 1);
    case kVSD_Right:
        return ivec3(pFace->start.x,
                     pFace->start.y + size.y - y - 1,
                     pFace->start.z + size.x - x - 1);
    case kVSD_Front:
        return ivec3(pFace->start.x + x,
                     pFace->start.y + size.y - y - 1,
                     pFace->start.z);
    case kVSD_Top:
        return ivec3(pFace->start.x + x,
                     pFace->start.y,
                     pFace->start.z + y);
    }
    PANIC("Invalid side");
    return ivec3(0);
}

static bool build_diffuse(Gimg * pGimg, Vector<kMEM_Chef, VoxMatrixFace*> matrixFaces)
{
    pGimg->clear(0);

    uvec2 imageSize(pGimg->width(), pGimg->height());
    ImagePartition imgPart(imageSize);

    size_t i = 0;
    for (i = 0; i < matrixFaces.size(); i++)
    {
        VoxMatrixFace * pMatFace = matrixFaces[i];
        const VoxFace * pFace = pMatFace->pFace;

        // 1x1 faces are handled below
        if (pMatFace->area == 1)
            break;

        uvec2 size;
        switch(pFace->side)
        {
        case kVSD_Left:
        case kVSD_Right:
            size.x = pFace->size.z;
            size.y = pFace->size.y;
            break;
        case kVSD_Back:
        case kVSD_Front:
            size.x = pFace->size.x;
            size.y = pFace->size.y;
            break;
        case kVSD_Bottom:
        case kVSD_Top:
            size.x = pFace->size.x;
            size.y = pFace->size.z;
            break;
        }

        uvec2 pos;
        if (!imgPart.getEmptyPosition(&pos, size))
        {
            // No more available space in image for requested size.
            // Caller can re-call us with a bigger Gimg.
            return false;
        }

        for (i32 y = 0; y < size.y; y++)
        {
            Color * pScanLine = (Color*)pGimg->scanline(pos.y + y);
            for (i32 x = 0; x < size.x; x++)
            {
                ivec3 matPos = face_image_pos_to_voxel(pFace, size, x, y);
                pScanLine[pos.x+x] = pMatFace->pMatrix->voxel(matPos).color;
            }
        }

        set_uvs(pMatFace, pos, size, imageSize);
    }

    // the only faces left are 1x1 faces, so we fill in any gaps in the image with those
    Color * const pColorStart = (Color*)pGimg->pixels();
    Color * const pColorEnd = pColorStart + imageSize.x * imageSize.y;
    Color * pColor = pColorStart;

    // remember any 1x1 faces we find and make all 1x1 faces of the same voxel use it
    HashMap<kMEM_Chef, const VoxMatrix*, HashMap<kMEM_Chef, ivec3, uvec2>> voxPosToImgPos;
    for (; i < matrixFaces.size(); i++)
    {
        VoxMatrixFace* pMatFace = matrixFaces[i];
        const VoxFace* pFace = pMatFace->pFace;

        uvec2 pos(0);
        // check if we already have a pixel for a different face of this 1x1x1 voxel
        auto voxToImgIt = voxPosToImgPos[pMatFace->pMatrix].find(pFace->start);
        if (voxToImgIt != voxPosToImgPos[pMatFace->pMatrix].end())
        {
            pos = voxToImgIt->second;
        }
        else
        {
            // find a blank space in image
            while(*pColor != 0)
            {
                if (pColor >= pColorEnd)
                {
                    // we've run out of space in the image
                    return false;
                }
                pColor++;
            }
            *pColor = pMatFace->pMatrix->voxel(pFace->start).color;

            // figure out our pos and store it in our lookup table
            u32 offset = pColor - pColorStart;
            pos = uvec2(offset % imageSize.x, offset / imageSize.x);

            voxPosToImgPos[pMatFace->pMatrix].emplace(pFace->start, pos);

            pColor++;
        }
        set_uvs(pMatFace, pos, uvec2(1), imageSize);
    }

    return true;
}

VoxObj::VoxObj(const Qbt& qbt)
  : qbt(qbt)
  , voxSkel(qbt)
{
    const QbtNode * pBaseNode = qbt.findTopLevelCompound("Base");
    if (pBaseNode != nullptr)
    {
        add_matrices_to_map(baseMatrices, *pBaseNode);
    }

    type = VoxObjType::determine_type(*this);

    // calc base matrices mins/maxes
    mins = ivec3(std::numeric_limits<int>::max());
    maxes = ivec3(std::numeric_limits<int>::min());
    for (const auto & part : type.parts)
    {
        if (part.flags & kVPF_CenterOfMass)
        {
            VoxMatrix & mat = *baseMatrices[part.name];
            ivec3 matMins = mat.mins + mat.worldPos;
            ivec3 matMaxes = mat.maxes + mat.worldPos;
            if (matMins.x < mins.x)
                mins.x = matMins.x;
            if (matMins.y < mins.y)
                mins.y = matMins.y;
            if (matMins.z < mins.z)
                mins.z = matMins.z;

            if (matMaxes.x > maxes.x)
                maxes.x = matMaxes.x;
            if (matMaxes.y > maxes.y)
                maxes.y = matMaxes.y;
            if (matMaxes.z > maxes.z)
                maxes.z = matMaxes.z;
        }
    }
    center = vec3(maxes.x + mins.x + 1, maxes.y + mins.y + 1, maxes.z + mins.z + 1) * 0.5f;
    halfExtents = vec3(center.x - mins.x, center.y - mins.y, center.z - mins.z);

    // build master face list
    Vector<kMEM_Chef, VoxMatrixFace*> matrixFaces;
    i32 totalArea = 0;
    for (const auto & part : type.parts)
    {
        VoxMatrix & matrix = *baseMatrices[part.name];
        for (auto & matFace : matrix.faces)
        {
            matrixFaces.push_back(&matFace);
            totalArea += matFace.area;
        }
    }
    std::stable_sort(matrixFaces.begin(), matrixFaces.end(), face_size_gt);

    f32 pixRoot = sqrt(totalArea);
    u32 imgWidth = next_power_of_two((u32)(pixRoot + 0.5));
    u32 imgHeight = imgWidth;

    pGimgDiffuse.reset(Gimg::create(kPXL_RGBA8, imgWidth, imgHeight, 0));

    if (!build_diffuse(pGimgDiffuse.get(), matrixFaces))
    {
        // try one more time with larger image
        imgWidth *= 2;
        imgHeight *= 2;

        pGimgDiffuse.reset(Gimg::create(kPXL_RGBA8, imgWidth, imgHeight, 0));
        if (!build_diffuse(pGimgDiffuse.get(), matrixFaces))
        {
            PANIC("Unable to build_diffuse on second attempt, width=%d height=%d", imgWidth, imgHeight);
        }
    }


    // Now we only have 1x1 faces, so fill in any empty spaces in
    // image we can find

/*
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
*/
}

void VoxObj::exportFiles(const ChefString & basePath, f32 voxelSize) const
{
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

    voxSkel.writeSkl(sklPath, voxelSize);
}

} // namespace gaen
