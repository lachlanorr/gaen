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

#include "gaen/voxel/Qbt.h"

#include "gaen/voxel/VoxMatrix.h"

namespace gaen
{

void add_matrices_to_map(VoxMatrixMap & matrices, const QbtNode & node)
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

static bool is_voxel_side_visible(VoxMatrix & matrix, const vec3 & pos, VoxSide side)
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

static bool voxel_pos_check(const vec3 & pos, VoxSide side)
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

static vec3 voxel_prev_pos(const vec3 & pos, VoxSide side)
{
    switch(side)
    {
    case kVSD_Left:
    case kVSD_Right:
        return vec3(pos.x, pos.y, pos.z-1);
    case kVSD_Back:
    case kVSD_Bottom:
    case kVSD_Front:
    case kVSD_Top:
        return vec3(pos.x-1, pos.y, pos.z);
    default:
        PANIC("Invalid side");
        return vec3(0);
    }
}

static void voxel_merge(Voxel & vox, Voxel & prevVox, VoxSide side, VoxSideFlag sideFlag)
{
    if (prevVox.visibleSides & sideFlag)
    {
        VoxFace * pPrevFace = prevVox.pFaces[side];

        bool canMerge = false;
        vec3 sizeInc(0);
        switch(side)
        {
        case kVSD_Left:
        case kVSD_Right:
            canMerge = pPrevFace->size.y == 1;
            sizeInc = vec3(0, 0, 1);
            break;
        case kVSD_Back:
        case kVSD_Front:
            canMerge = pPrevFace->size.y == 1;
            sizeInc = vec3(1, 0, 0);
            break;
        case kVSD_Bottom:
        case kVSD_Top:
            canMerge = pPrevFace->size.z == 1;
            sizeInc = vec3(1, 0, 0);
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

static bool voxel_row_check(const vec3 & pos, VoxSide side)
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
    vec3 prevRow;
    switch(side)
    {
    case kVSD_Left:
    case kVSD_Back:
    case kVSD_Right:
    case kVSD_Front:
        prevRow = vec3(vox.pos.x, vox.pos.y-1, vox.pos.z);
        break;
    case kVSD_Bottom:
    case kVSD_Top:
        prevRow = vec3(vox.pos.x, vox.pos.y, vox.pos.z-1);
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
            vec3 prevPos = voxel_prev_pos(vox.pos, side);
            auto itIdx = matrix.voxelIdMap.find(prevPos);
            if (itIdx != matrix.voxelIdMap.end())
            {
                Voxel & prevVox = matrix.voxels[itIdx->second];
                voxel_merge(vox, prevVox, side, sideFlag);
            }
        }
    }
}

static const vec3 qbt_node_world_pos(const QbtNode & node)
{
    vec3 worldPos = vec3(node.position.x, node.position.y, node.position.z);
    if (node.pParent)
    {
        worldPos += qbt_node_world_pos(*node.pParent);
    }
    return worldPos;
}

VoxMatrix::VoxMatrix(const QbtNode &node)
  : node(node)
{
    worldPos = qbt_node_world_pos(node);
    mins = vec3(std::numeric_limits<int>::max());
    maxes = vec3(std::numeric_limits<int>::min());

    for (i32 z = 0; z < node.size.z; z++)
    {
        for (i32 y = 0; y < node.size.y; y++)
        {
            for (i32 x = 0; x < node.size.x; x++)
            {
                Color col = node.voxel(x, y, z);
                if (col.a() > 1) // 1 means core voxel
                {
                    col.seta(255);
                    vec3 pos(x, y, z);
                    addVoxel(col, pos);
                }
            }
        }
    }

    vec3 worldMaxes = maxes + worldPos;
    vec3 worldMins = mins + worldPos;
    worldCenter = vec3(worldMaxes.x + worldMins.x + 1, worldMaxes.y + worldMins.y + 1, worldMaxes.z + worldMins.z + 1) * 0.5f;
    halfExtents = vec3(worldCenter.x - worldMins.x, worldCenter.y - worldMins.y, worldCenter.z - worldMins.z);

    // merge compatible rows
    HashMap<kMEM_Chef, VoxFace*, VoxFace*> faceTrans;
    for (Voxel & vox : voxels)
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
                    voxel_row_merge(*this, vox, (VoxSide)side, faceTrans);
                }
            }
        }
    }

    // All composite faces should be generated now, build the master matrix face vector
    for (i32 z = 0; z < node.size.z; z++)
    {
        for (i32 y = 0; y < node.size.y; y++)
        {
            for (i32 x = 0; x < node.size.x; x++)
            {
                auto voxIdIt = voxelIdMap.find(vec3(x, y, z));
                if (voxIdIt != voxelIdMap.end())
                {
                    Voxel & vox = voxels[voxIdIt->second];
                    for (u32 i = 0; i < kVSD_COUNT; ++i)
                    {
                        VoxFace * pFace = vox.pFaces[i];
                        // If our pointer matches our face, we are the
                        // "owner" of this potentially composite face
                        if (pFace == &vox.faces[i])
                        {
                            faces.push_back(VoxMatrixFace(pFace, this));
                            VoxMatrixFace &matFace = faces.back();

                            vec3 worldPos(worldPos.x, worldPos.y, worldPos.z);
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

const Voxel & VoxMatrix::voxel(vec3 pos) const
{
    auto voxIdIt = voxelIdMap.find(pos);
    PANIC_IF(voxIdIt == voxelIdMap.end(), "Voxel not found: (%d, %d, %d)", pos.x, pos.y, pos.z);
    return voxels[voxIdIt->second];
}

void VoxMatrix::addVoxel(Color col, vec3 pos)
{
    voxels.emplace_back(col, pos);
    Voxel &vox = voxels.back();
    voxelIdMap.emplace(pos, voxels.size()-1);

    if (pos.x < mins.x)
        mins.x = pos.x;
    if (pos.y < mins.y)
        mins.y = pos.y;
    if (pos.z < mins.z)
        mins.z = pos.z;

    if (pos.x > maxes.x)
        maxes.x = pos.x;
    if (pos.y > maxes.y)
        maxes.y = pos.y;
    if (pos.z > maxes.z)
        maxes.z = pos.z;

    for (i32 side = 0; side < kVSD_COUNT; side++)
    {
        process_voxel_side(*this, vox, (VoxSide)side);
    }
}

} // namespace gaen
