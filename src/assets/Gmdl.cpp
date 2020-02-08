//------------------------------------------------------------------------------
// Gmdl.cpp - Runtime model format
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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

#include "core/HashMap.h"
#include "core/String.h"

#include "assets/file_utils.h"
#include "assets/Gmdl.h"

namespace gaen
{

bool Gmdl::is_valid(const void * pBuffer, u64 size)
{
    if (size < sizeof(Gmdl))
        return false;

    const Gmdl * pAssetData = reinterpret_cast<const Gmdl*>(pBuffer);

    if (pAssetData->magic4cc() != kMagic4CC)
        return false;
    if (pAssetData->size() != size)
        return false;

    if (!is_valid_vert_type(pAssetData->mVertType))
        return false;

    if (!is_valid_prim_type(pAssetData->mPrimType))
        return false;

    u64 reqSize = required_size(pAssetData->vertType(),
                                pAssetData->mVertCount,
                                pAssetData->primType(),
                                pAssetData->mPrimCount,
                                pAssetData->mBoneCount,
                                pAssetData->mHardpointCount,
                                pAssetData->mat());
    if (reqSize != size)
        return false;

    u32 vertStride = pAssetData->vertStride();

    u32 tot = pAssetData->totalSize();
    if (size != pAssetData->totalSize())
        return false;

    if (pAssetData->primOffset() != sizeof(Gmdl) + size_aligned(vertStride, pAssetData->mVertCount))
        return false;

    return true;
}

Gmdl * Gmdl::instance(void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gmdl buffer");
        return nullptr;
    }

    return reinterpret_cast<Gmdl*>(pBuffer);
}

const Gmdl * Gmdl::instance(const void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gmdl buffer");
        return nullptr;
    }

    return reinterpret_cast<const Gmdl*>(pBuffer);
}

u64 Gmdl::required_size(VertType vertType,
                        u32 vertCount,
                        PrimType primType,
                        u32 primCount,
                        u32 boneCount,
                        u32 hardpointCount,
                        const Gmat * pMat)
{
    u32 vertStride = vert_stride(vertType);
    u32 primStride = prim_stride(primType);

    u32 matSize = pMat ? (u32)pMat->size() : 0;
    return (sizeof(Gmdl) +
            size_aligned(vertStride, vertCount) +
            size_aligned(primStride, primCount) +
            size_aligned(sizeof(Bone), boneCount) +
            size_aligned(sizeof(Hardpoint), hardpointCount) +
            matSize);
}

Gmdl * Gmdl::create(VertType vertType,
                    u32 vertCount,
                    PrimType primType,
                    u32 primCount,
                    u32 shaderHash,
                    u32 boneCount,
                    u32 hardpointCount,
                    const Gmat * pMat)
{
    u64 size = Gmdl::required_size(vertType,
                                   vertCount,
                                   primType,
                                   primCount,
                                   boneCount,
                                   hardpointCount,
                                   pMat);

    Gmdl * pGmdl = alloc_asset<Gmdl>(kMEM_Model, size);

    PANIC_IF(!is_valid_vert_type(vertType), "Invalid vertType, %d", vertType);
    PANIC_IF(!is_valid_prim_type(primType), "Invalid primTYpe, %d", primType);

    u32 vertStride = vert_stride(vertType);
    u32 primStride = prim_stride(primType);

    pGmdl->mVertType = vertType;
    pGmdl->mPrimType = primType;
    pGmdl->mVertCount = vertCount;
    pGmdl->mPrimCount = primCount;
    pGmdl->mPrimOffset = pGmdl->vertOffset() + size_aligned(vertStride, vertCount);
    pGmdl->mShaderHash = shaderHash;
    pGmdl->mBoneCount = boneCount;
    pGmdl->mBoneOffset = pGmdl->primOffset() + size_aligned(primStride, primCount);
    pGmdl->mHardpointCount = hardpointCount;
    pGmdl->mHardpointOffset = pGmdl->boneOffset() + size_aligned(sizeof(Bone), boneCount);
    pGmdl->mMatOffset = pMat ? pGmdl->hardpointOffset() + size_aligned(sizeof(Hardpoint), hardpointCount) : 0;

    pGmdl->mHas32BitIndices = 0;
    pGmdl->mMorphTargetCount = 0; // no targets, just one set of verts

    if (pMat)
    {
        memcpy(pGmdl->mat(), pMat, pMat->size());
    }

    return pGmdl;
}

u32 Gmdl::boneIndex(u32 nameHash) const
{
    ASSERT(boneCount() > 0);
    const Bone * pBone = bones();
    for (u32 i = 0; i < boneCount(); ++i)
    {
        if (pBone->nameHash == nameHash)
            return i;
        pBone++;
    }
    PANIC("Unknown nameHash, cannot find bone: %u", nameHash);
    return 0;
}

const Bone * Gmdl::boneByIndex(u32 idx) const
{
    ASSERT(idx < boneCount());
    return &bones()[idx];
}

u32 Gmdl::hardpointIndex(u32 nameHash) const
{
    ASSERT(hardpointCount() > 0);
    const Hardpoint * pHardpoint = hardpoints();
    for (u32 i = 0; i < hardpointCount(); ++i)
    {
        if (pHardpoint->nameHash == nameHash)
            return i;
        pHardpoint++;
    }
    PANIC("Unknown nameHash, cannot find hardpoint: %u", nameHash);
    return 0;
}

const Hardpoint * Gmdl::hardpointByIndex(u32 idx) const
{
    ASSERT(idx < hardpointCount());
    return &hardpoints()[idx];
}

const Hardpoint * Gmdl::hardpointByName(u32 nameHash) const
{
    u32 idx = hardpointIndex(nameHash);
    return hardpointByIndex(idx);
}

void Gmdl::compact(u32 newVertCount, u32 newPrimCount)
{
    ASSERT(mBoneCount == 0 && mHardpointCount == 0); // shouldn't ever need to support these in compact, but sanity checking here
    ASSERT(newVertCount < mVertCount);
    ASSERT(newPrimCount < mPrimCount);

    // Get current start of prims before we muck up our counts, etc.
    index * oldPrims = prims();
    Gmat * oldMat = mat();

    u32 vertStride = vert_stride((VertType)mVertType);
    u32 primStride = prim_stride((PrimType)mPrimType);

    mSize = required_size((VertType)mVertType, newVertCount, (PrimType)mPrimType, newPrimCount, 0, 0, oldMat);;
    mVertCount = newVertCount;
    mPrimCount = newPrimCount;
    mPrimOffset = vertOffset() + size_aligned(vertStride, mVertCount);
    mMatOffset = oldMat ? primOffset() + size_aligned(primStride, mPrimCount) : 0;

    index * newPrims = prims();

    // Shift prims to correct new location
    memmove(newPrims, oldPrims, size_aligned(primStride, newPrimCount) + (size_t)size_aligned(sizeof(Bone), mBoneCount));

    // Shift material to correct new location
    if (oldMat)
    {
        Gmat * newMat = mat();
        memmove(newMat, oldMat, oldMat->size());
    }

    ASSERT(is_valid(this, mSize));
}

void Gmdl::updateHalfExtents()
{
    vec3 maxPos = vec3(std::numeric_limits<f32>::lowest());
    vec3 minPos = vec3(std::numeric_limits<f32>::max());

    const u8 * pVert = reinterpret_cast<const u8*>(verts());
    for (u32 i = 0; i < vertCount(); ++i)
    {
        const vec3 * pPos = reinterpret_cast<const vec3*>(pVert);

        maxPos = max(maxPos, *pPos);
        minPos = min(minPos, *pPos);

        pVert += vertStride();
    }

    mHalfExtents = max(abs(minPos), abs(maxPos));
}

const Bone * Gmdl::findBone(u32 nameHash) const
{
    const Bone * pBone = bones();
    for (u32 i = 0; i < boneCount(); ++i)
    {
        if (pBone->nameHash == nameHash)
            return pBone;
        pBone++;
    }
    return nullptr;
}

const Hardpoint * Gmdl::findHardpoint(u32 nameHash) const
{
    const Hardpoint * pHardpoint = hardpoints();
    for (u32 i = 0; i < hardpointCount(); ++i)
    {
        if (pHardpoint->nameHash == nameHash)
            return pHardpoint;
        pHardpoint++;
    }
    return nullptr;
}

} // namespace gaen

