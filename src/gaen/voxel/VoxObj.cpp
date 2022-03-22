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

static const QbtNode * find_top_level_compound(const Qbt & qbt, const char * name)
{
    if (qbt.pRoot)
    {
        for (u32 i = 0; i < qbt.pRoot->children.size(); ++i)
        {
            if (qbt.pRoot->children[i]->name == name && qbt.pRoot->children[i]->typeId == kQBNT_Compound)
            {
                return qbt.pRoot->children[i].get();
            }
        }
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

static bool is_voxel_side_visible(VoxMatrix & matrix, const ivec3 & pos, VoxSide side)
{
    switch (side)
    {
    case kVSD_Left:
        return pos.x == 0 || matrix.node.voxel(pos.x-1, pos.y, pos.z).a() == 0;
    case kVSD_Back:
        return pos.z == 0 || matrix.node.voxel(pos.x, pos.y, pos.z-1).a() == 0;
    case kVSD_Bottom:
        return pos.y == 0 || matrix.node.voxel(pos.x, pos.y-1, pos.z).a() == 0;
    case kVSD_Right:
        return pos.x == matrix.node.size.x-1 || matrix.node.voxel(pos.x+1, pos.y, pos.z).a() == 0;
    case kVSD_Front:
        return pos.z == matrix.node.size.z-1 || matrix.node.voxel(pos.x, pos.y, pos.z+1).a() == 0;
    case kVSD_Top:
        return pos.y == matrix.node.size.y-1 || matrix.node.voxel(pos.x, pos.y+1, pos.z).a() == 0;
    default:
        PANIC("Invalid side");
        return false;
    }
}

static VoxSideFlag voxel_side_flag(VoxSide side)
{
    switch(side)
    {
    case kVSD_Left:
        return kVSDF_Left;
    case kVSD_Back:
        return kVSDF_Back;
    case kVSD_Bottom:
        return kVSDF_Bottom;
    case kVSD_Right:
        return kVSDF_Right;
    case kVSD_Front:
        return kVSDF_Front;
    case kVSD_Top:
        return kVSDF_Top;
    default:
        PANIC("Invalid side");
        return kVSDF_NONE;
    }
}

static bool voxel_pos_check(const ivec3 & pos, VoxSide side)
{
    switch(side)
    {
    case kVSD_Left:
    case kVSD_Right:
        return pos.z > 0;
    case kVSD_Back:
    case kVSD_Bottom:
    case kVSD_Front:
    case kVSD_Top:
        return pos.x > 0;
    default:
        PANIC("Invalid side");
        return false;
    }
}

static ivec3 voxel_prev_pos(const ivec3 & pos, VoxSide side)
{
    switch(side)
    {
    case kVSD_Left:
    case kVSD_Right:
        return ivec3(pos.x, pos.y, pos.z-1);
    case kVSD_Back:
    case kVSD_Bottom:
    case kVSD_Front:
    case kVSD_Top:
        return ivec3(pos.x-1, pos.y, pos.z);
    default:
        PANIC("Invalid side");
        return ivec3(0);
    }
}

static void voxel_merge(Voxel & vox, Voxel & prevVox, VoxSide side, VoxSideFlag sideFlag)
{
    if (prevVox.visibleSides & sideFlag)
    {
        VoxFace * pPrevFace = prevVox.pFaces[side];

        bool canMerge = false;
        ivec3 sizeInc(0);
        switch(side)
        {
        case kVSD_Left:
        case kVSD_Right:
            canMerge = pPrevFace->size.y == 1;
            sizeInc = ivec3(0, 0, 1);
            break;
        case kVSD_Back:
        case kVSD_Front:
            canMerge = pPrevFace->size.y == 1;
            sizeInc = ivec3(1, 0, 0);
            break;
        case kVSD_Bottom:
        case kVSD_Top:
            canMerge = pPrevFace->size.z == 1;
            sizeInc = ivec3(1, 0, 0);
            break;
        }

        // make sure prev face hasn't joined into a taller rect
        if (canMerge)
        {
            // increase size to include us, combining our faces
            pPrevFace->size += sizeInc;
            // change our pFace to use previous voxel's
            vox.pFaces[side] = pPrevFace;
        }
    }
}

static bool voxel_row_check(const ivec3 & pos, VoxSide side)
{
    switch(side)
    {
    case kVSD_Bottom:
    case kVSD_Top:
        return pos.z > 0;
    case kVSD_Left:
    case kVSD_Back:
    case kVSD_Right:
    case kVSD_Front:
        return pos.y > 0;
    default:
        PANIC("Invalid side");
        return false;
    }
}

static void voxel_row_merge(VoxMatrix & matrix, Voxel & vox, VoxSide side, HashMap<kMEM_Chef, VoxFace*, VoxFace*> &faceTrans)
{
    ivec3 prevRow;
    switch(side)
    {
    case kVSD_Left:
    case kVSD_Back:
    case kVSD_Right:
    case kVSD_Front:
        prevRow = ivec3(vox.pos.x, vox.pos.y-1, vox.pos.z);
        break;
    case kVSD_Bottom:
    case kVSD_Top:
        prevRow = ivec3(vox.pos.x, vox.pos.y, vox.pos.z-1);
        break;
    default:
        PANIC("Invalid side");
        break;
    }

    auto itIdx = matrix.voxelIdMap.find(prevRow);
    if (itIdx != matrix.voxelIdMap.end())
    {
        Voxel & prevVox = matrix.voxels[itIdx->second];
        VoxFace * pPrevFace = prevVox.pFaces[side];
        if (pPrevFace != nullptr)
        {
            VoxFace * pFace = vox.pFaces[side];

            bool canMerge = false;
            switch(side)
            {
            case kVSD_Left:
            case kVSD_Right:
                canMerge = (pPrevFace->start.z == pFace->start.z && // check if our row start is the same
                            pPrevFace->size.z == pFace->size.z);    // check if width is the same
                break;
            case kVSD_Back:
            case kVSD_Bottom:
            case kVSD_Front:
            case kVSD_Top:
                canMerge = (pPrevFace->start.x == pFace->start.x && // check if our row start is the same
                            pPrevFace->size.x == pFace->size.x);    // check if width is the same
                break;
            default:
                PANIC("Invalid side");
                break;
            }

            if (canMerge)
            {
                // combine with previous face, and fix up
                // all pFaces pointers in our row
                switch(side)
                {
                case kVSD_Left:
                case kVSD_Right:
                case kVSD_Back:
                case kVSD_Front:
                    pPrevFace->size.y++;
                    break;
                case kVSD_Bottom:
                case kVSD_Top:
                    pPrevFace->size.z++;
                    break;
                }

                faceTrans[pFace] = pPrevFace;
                vox.pFaces[side] = pPrevFace;
            }
        }
    }
}

static void process_voxel_side(VoxMatrix & matrix, Voxel & vox, VoxSide side)
{
    VoxSideFlag sideFlag = voxel_side_flag(side);

    if (is_voxel_side_visible(matrix, vox.pos, side))
    {
        vox.pFaces[side] = &vox.faces[side];
        vox.visibleSides |= sideFlag;

        // check if previous voxel is coplanar on this side
        if (voxel_pos_check(vox.pos, side))
        {
            ivec3 prevPos = voxel_prev_pos(vox.pos, side);
            auto itIdx = matrix.voxelIdMap.find(prevPos);
            if (itIdx != matrix.voxelIdMap.end())
            {
                Voxel & prevVox = matrix.voxels[itIdx->second];
                voxel_merge(vox, prevVox, side, sideFlag);
            }
        }
    }
}

static void process_matrix(VoxMatrix & matrix)
{
    matrix.worldPos = qbt_node_world_pos(matrix.node);

    for (i32 z = 0; z < matrix.node.size.z; z++)
    {
        for (i32 y = 0; y < matrix.node.size.y; y++)
        {
            for (i32 x = 0; x < matrix.node.size.x; x++)
            {
                Color col = matrix.node.voxel(x, y, z);
                if (col.a() > 1) // 1 means core voxel
                {
                    col.seta(255);

                    ivec3 pos(x, y, z);
                    matrix.voxels.emplace_back(col, pos);
                    Voxel &vox = matrix.voxels.back();
                    matrix.voxelIdMap.emplace(pos, matrix.voxels.size()-1);

                    for (i32 side = 0; side < kVSD_COUNT; side++)
                    {
                        process_voxel_side(matrix, vox, (VoxSide)side);
                    }
                }
            }
        }
    }

    // merge compatible rows
    HashMap<kMEM_Chef, VoxFace*, VoxFace*> faceTrans;
    for (Voxel & vox : matrix.voxels)
    {
        for (i32 side = 0; side < kVSD_COUNT; side++)
        {
            VoxFace * pFace = vox.pFaces[side];
            if (pFace != nullptr)
            {
                auto pFaceIt = faceTrans.find(pFace);
                if (pFaceIt != faceTrans.end())
                {
                    vox.pFaces[side] = pFaceIt->second;
                }
                else if (voxel_row_check(vox.pos, (VoxSide)side))
                {
                    voxel_row_merge(matrix, vox, (VoxSide)side, faceTrans);
                }
            }
        }
    }

    // All composite faces should be generated now, build the master matrix face vector
    for (i32 z = 0; z < matrix.node.size.z; z++)
    {
        for (i32 y = 0; y < matrix.node.size.y; y++)
        {
            for (i32 x = 0; x < matrix.node.size.x; x++)
            {
                auto voxIdIt = matrix.voxelIdMap.find(ivec3(x, y, z));
                if (voxIdIt != matrix.voxelIdMap.end())
                {
                    Voxel & vox = matrix.voxels[voxIdIt->second];
                    for (u32 i = 0; i < kVSD_COUNT; ++i)
                    {
                        VoxFace * pFace = vox.pFaces[i];
                        // If our pointer matches our face, we are the
                        // "owner" of this potentially composite face
                        if (pFace == &vox.faces[i])
                        {
                            matrix.faces.push_back(VoxMatrixFace(pFace, &matrix));
                            VoxMatrixFace &matFace = matrix.faces.back();

                            vec3 worldPos(matrix.worldPos.x, matrix.worldPos.y, matrix.worldPos.z);
                            vec3 size(pFace->size.x, pFace->size.y, pFace->size.z);

                            switch(pFace->side)
                            {
                            case kVSD_Left:
                                matFace.points[0] = vec3(pFace->start.x, pFace->start.y,        pFace->start.z) + worldPos;
                                matFace.points[1] = vec3(pFace->start.x, pFace->start.y,        pFace->start.z+size.z) + worldPos;
                                matFace.points[2] = vec3(pFace->start.x, pFace->start.y+size.y, pFace->start.z+size.z) + worldPos;
                                matFace.points[3] = vec3(pFace->start.x, pFace->start.y+size.y, pFace->start.z) + worldPos;
                                break;
                            case kVSD_Back:
                                matFace.points[0] = vec3(pFace->start.x+size.x, pFace->start.y,        pFace->start.z) + worldPos;
                                matFace.points[1] = vec3(pFace->start.x,        pFace->start.y,        pFace->start.z) + worldPos;
                                matFace.points[2] = vec3(pFace->start.x,        pFace->start.y+size.y, pFace->start.z) + worldPos;
                                matFace.points[3] = vec3(pFace->start.x+size.x, pFace->start.y+size.y, pFace->start.z) + worldPos;
                                break;
                            case kVSD_Bottom:
                                matFace.points[0] = vec3(pFace->start.x,        pFace->start.y, pFace->start.z) + worldPos;
                                matFace.points[1] = vec3(pFace->start.x+size.x, pFace->start.y, pFace->start.z) + worldPos;
                                matFace.points[2] = vec3(pFace->start.x+size.x, pFace->start.y, pFace->start.z+size.z) + worldPos;
                                matFace.points[3] = vec3(pFace->start.x,        pFace->start.y, pFace->start.z+size.z) + worldPos;
                                break;
                            case kVSD_Right:
                                matFace.points[0] = vec3(pFace->start.x+1, pFace->start.y,        pFace->start.z+size.z) + worldPos;
                                matFace.points[1] = vec3(pFace->start.x+1, pFace->start.y,        pFace->start.z) + worldPos;
                                matFace.points[2] = vec3(pFace->start.x+1, pFace->start.y+size.y, pFace->start.z) + worldPos;
                                matFace.points[3] = vec3(pFace->start.x+1, pFace->start.y+size.y, pFace->start.z+size.z) + worldPos;
                                break;
                            case kVSD_Front:
                                matFace.points[0] = vec3(pFace->start.x,        pFace->start.y,        pFace->start.z+1) + worldPos;
                                matFace.points[1] = vec3(pFace->start.x+size.x, pFace->start.y,        pFace->start.z+1) + worldPos;
                                matFace.points[2] = vec3(pFace->start.x+size.x, pFace->start.y+size.y, pFace->start.z+1) + worldPos;
                                matFace.points[3] = vec3(pFace->start.x,        pFace->start.y+size.y, pFace->start.z+1) + worldPos;
                                break;
                            case kVSD_Top:
                                matFace.points[0] = vec3(pFace->start.x,        pFace->start.y+1, pFace->start.z+size.z) + worldPos;
                                matFace.points[1] = vec3(pFace->start.x+size.x, pFace->start.y+1, pFace->start.z+size.z) + worldPos;
                                matFace.points[2] = vec3(pFace->start.x+size.x, pFace->start.y+1, pFace->start.z) + worldPos;
                                matFace.points[3] = vec3(pFace->start.x,        pFace->start.y+1, pFace->start.z) + worldPos;
                                break;
                            }

                        }
                    }
                }
            }
        }
    }
}

static bool face_size_gt(const VoxMatrixFace *lhs, const VoxMatrixFace *rhs)
{
    return lhs->area > rhs->area;
}

static vec2 calc_uv(uvec2 pos, uvec2 imageSize)
{
    return vec2(pos.x / (f32)imageSize.x + 0.5 / imageSize.x,
                pos.y / (f32)imageSize.y + 0.5 / imageSize.y);
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
                pScanLine[pos.x+x] = pMatFace->pMatrix->node.voxel(matPos.x, matPos.y, matPos.z);
                pScanLine[pos.x+x].seta(255);
            }
        }
    }

    // the only faces left are 1x1 faces, so we fill in any gaps in the image with those
    Color * const pColorStart = (Color*)pGimg->pixels();
    Color * const pColorEnd = pColorStart + imageSize.x * imageSize.y;
    Color * pColor = pColorStart;

    // remember any 1x1 faces we find and make all 1x1 faces of the same voxel use it
    HashMap<kMEM_Chef, ivec3, uvec2> voxPosToImgPos;
    for (; i < matrixFaces.size(); i++)
    {
        VoxMatrixFace* pMatFace = matrixFaces[i];
        const VoxFace* pFace = pMatFace->pFace;

        uvec2 imagePos(0);
        // check if we already have a pixel for a different face of this 1x1x1 voxel
        auto voxToImgIt = voxPosToImgPos.find(pFace->start);
        if (voxToImgIt != voxPosToImgPos.end())
        {
            imagePos = voxToImgIt->second;
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
            *pColor = pMatFace->pMatrix->node.voxel(pFace->start.x, pFace->start.y, pFace->start.z);
            pColor->seta(255);

            // figure out our imagePos and store it in our lookup table
            u32 offset = pColor - pColorStart;
            imagePos = uvec2(offset % imageSize.x, offset / imageSize.x);
            voxPosToImgPos.emplace(pFace->start, imagePos);

            pColor++;
        }
    }

    return true;
}

VoxObj::VoxObj(const Qbt& qbt)
  : qbt(qbt)
{
    if (const QbtNode * pNode = find_top_level_compound(qbt, "Base"))
    {
        add_matrices_to_map(baseMatrices, *pNode);
        if (const QbtNode * pNode = find_top_level_compound(qbt, "Nulls"))
        {
            add_matrices_to_map(nullsMatrices, *pNode);
        }
    }
    else
    {
        // no known type, just grab all other matrices
        add_matrices_to_map(baseMatrices, *qbt.pRoot);
    }
    type = VoxObjType::determine_type(*this);

    for (const auto & part : type.parts)
    {
        process_matrix(*baseMatrices[part.name]);
    }

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

    struct Face
    {
        u32 points[4];
        //u32 uvs[4];
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
                vec3 scaledVert = matFace.points[i] * scaleFactor;
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

    snprintf(tempStr.data(), tempStr.size(), "vt 0.0 0.0\n");
    objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    /*--
    for (const auto & pix : pixels)
    {
        snprintf(tempStr.data(), tempStr.size(), "vt %f %f\n", pix.uv.x, pix.uv.y);
        objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
    }
    --*/
    objWrtr.ofs.write("\n", 1);

    for (const auto & part : type.parts)
    {
        snprintf(tempStr.data(), tempStr.size(), "g %s\n", part.name.c_str());
        objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
        snprintf(tempStr.data(), tempStr.size(), "usemtl %s_Material\n", baseName.c_str());
        objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));

        auto & faces = faceMap[part.name];
        for (const auto & face : faces)
        {
            snprintf(tempStr.data(), tempStr.size(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
                     face.points[0]+1, 1/*face.uv+1*/, face.normal+1,
                     face.points[1]+1, 1/*face.uv+1*/, face.normal+1,
                     face.points[2]+1, 1/*face.uv+1*/, face.normal+1,
                     face.points[3]+1, 1/*face.uv+1*/, face.normal+1);
            objWrtr.ofs.write(tempStr.data(), strlen(tempStr.data()));
        }
    }
}

} // namespace gaen
