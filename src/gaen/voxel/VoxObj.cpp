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

static vec3 face_image_pos_to_voxel(const VoxFace * pFace, const uvec2 size, u32 x, u32 y)
{
    switch(pFace->side)
    {
    case kVSD_Left:
        return vec3(pFace->start.x,
                     pFace->start.y + size.y - y - 1,
                     pFace->start.z + x);
    case kVSD_Back:
        return vec3(pFace->start.x + size.x - x - 1,
                     pFace->start.y + size.y - y - 1,
                     pFace->start.z);
    case kVSD_Bottom:
        return vec3(pFace->start.x + x,
                     pFace->start.y,
                     pFace->start.z + size.y - y - 1);
    case kVSD_Right:
        return vec3(pFace->start.x,
                     pFace->start.y + size.y - y - 1,
                     pFace->start.z + size.x - x - 1);
    case kVSD_Front:
        return vec3(pFace->start.x + x,
                     pFace->start.y + size.y - y - 1,
                     pFace->start.z);
    case kVSD_Top:
        return vec3(pFace->start.x + x,
                     pFace->start.y,
                     pFace->start.z + y);
    }
    PANIC("Invalid side");
    return vec3(0);
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
                vec3 matPos = face_image_pos_to_voxel(pFace, size, x, y);
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
    HashMap<kMEM_Chef, const VoxMatrix*, HashMap<kMEM_Chef, vec3, uvec2>> voxPosToImgPos;
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

VoxObj::VoxObj(const std::shared_ptr<QbtNode>& pRootNode, const VoxObjType& type)
  : pRootNode(pRootNode)
  , type(type)
  , mins(0)
  , maxes(0)
  , cogCenter(0)
  , cogHalfExtents(0)
  , pGimgDiffuse(nullptr)
  , pVoxSkel(nullptr)
{}

void VoxObj::processBaseMatrices(const QbtNode & baseNode)
{
    add_matrices_to_map(baseMatrices, baseNode);

    // calc base matrices mins/maxes
    mins = cogMins = vec3(std::numeric_limits<int>::max());
    maxes = cogMaxes = vec3(std::numeric_limits<int>::min());
    for (const auto & part : type.parts)
    {
        VoxMatrix & mat = *baseMatrices[part.name];
        vec3 matMins = mat.mins + mat.worldPos;
        vec3 matMaxes = mat.maxes + mat.worldPos;

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

        if (part.flags & kVPF_CenterOfGravity)
        {
            if (matMins.x < cogMins.x)
                cogMins.x = matMins.x;
            if (matMins.y < cogMins.y)
                cogMins.y = matMins.y;
            if (matMins.z < cogMins.z)
                cogMins.z = matMins.z;

            if (matMaxes.x > cogMaxes.x)
                cogMaxes.x = matMaxes.x;
            if (matMaxes.y > cogMaxes.y)
                cogMaxes.y = matMaxes.y;
            if (matMaxes.z > cogMaxes.z)
                cogMaxes.z = matMaxes.z;
        }
    }
    center = vec3(maxes.x + mins.x + 1, maxes.y + mins.y + 1, maxes.z + mins.z + 1) * 0.5f;
    halfExtents = vec3(center.x - mins.x, center.y - mins.y, center.z - mins.z);
    cogCenter = vec3(cogMaxes.x + cogMins.x + 1, cogMaxes.y + cogMins.y + 1, cogMaxes.z + cogMins.z + 1) * 0.5f;
    cogHalfExtents = vec3(cogCenter.x - cogMins.x, cogCenter.y - cogMins.y, cogCenter.z - cogMins.z);

    // offset so we are centered in the end
    offset = vec3(-center.x, -mins.y, -center.z);

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

    // build VoxGeo
    Vector<kMEM_Chef, const VoxMatrix*> orderedMatrices(type.parts.size());
    for (size_t i = 0; i < type.parts.size(); ++i)
    {
        orderedMatrices[i] = baseMatrices.find(type.parts[i].name)->second.get();
    }
    pBaseGeo.reset(GNEW(kMEM_Chef, VoxGeo, orderedMatrices, offset));
}

VoxObjVec build_voxobjs_from_qbt(const std::shared_ptr<Qbt> & pQbt)
{
    VoxObjVec vovec;

    if (!pQbt->pRoot)
        return vovec;

    // check to see if root node matches a type
    VoxObjType voType;
    if (VoxObjType::determine_type(pQbt->pRoot, voType))
    {
        vovec.emplace_back(voType.create(pQbt->pRoot));
    }
    else
    {
        // go through all top level nodes and add them in
        for (const auto pChild : pQbt->pRoot->children)
        {
            if (VoxObjType::determine_type(pChild, voType))
            {
                vovec.emplace_back(voType.create(pChild));
            }
            else
            {
                printf("Unknown VoxObjType, node name: %s", pChild->name.c_str());
            }
        }
    }
    if (vovec.size() == 0)
    {
        printf("No known VoxObjTypes in qbt");
    }

    return vovec;
}

} // namespace gaen
