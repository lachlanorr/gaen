//------------------------------------------------------------------------------
// Gaim.cpp - Animation data stored in F32 gimg
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

#include <cmath>

#include "gaen/core/logging.h"
#include "gaen/core/Vector.h"
#include "gaen/hashes/hashes.h"

#include "gaen/assets/file_utils.h"
#include "gaen/assets/Gimg.h"

#include "gaen/assets/Gaim.h"

namespace gaen
{

bool Gaim::is_valid(const void * pBuffer, u64 size)
{
    if (size < sizeof(Gaim))
        return false;

    const Gaim * pAssetData = reinterpret_cast<const Gaim*>(pBuffer);

    if (pAssetData->magic4cc() != kMagic4CC)
        return false;
    if (pAssetData->size() != size)
        return false;

    return true;
}

Gaim * Gaim::instance(void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gaim buffer");
        return nullptr;
    }

    return reinterpret_cast<Gaim*>(pBuffer);
}

const Gaim * Gaim::instance(const void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gaim buffer");
        return nullptr;
    }

    return reinterpret_cast<const Gaim*>(pBuffer);
}

static uvec2 image_dimensions(u32 totalPixels)
{
    u32 roundedRoot = (u32)(ceil(sqrt((f32)totalPixels)));
    u32 width = next_power_of_two(roundedRoot);
    u32 height = width / 2; // optimistic we don't need the whole square
    if (width * height < totalPixels)
        height *= 2;
    return uvec2(width, height);
}

u64 Gaim::required_size(u32 & boneCount,
                        u32 & animCount,
                        u64 & imageOffset,
                        uvec2 & widthHeight,
                        const AnimsRaw & animsRaw)
{
    u64 size = sizeof(Gaim);
    size += animsRaw.size() * sizeof(AnimInfo);

    PANIC_IF(size % 16 != 0, "AnimImage starting on non 16 byte boundary");
    PANIC_IF(animsRaw.size() == 0, "No animations in animsRaw");

    PANIC_IF(animsRaw.front().transforms.size() % animsRaw.front().info.frameCount != 0, "Transforms not a multiple of frame count");
    boneCount = (u32)animsRaw.front().transforms.size() / animsRaw.front().info.frameCount;
    animCount = (u32)animsRaw.size();
    imageOffset = sizeof(Gaim) + sizeof(AnimInfo) * animCount;

    u32 totalTransforms = 0;
    for (const auto & animRaw : animsRaw)
    {
        PANIC_IF((animRaw.transforms.size() % boneCount != 0) || (animRaw.transforms.size() / boneCount != animRaw.info.frameCount), "Animations with different bone counts");
        totalTransforms += (u32)animRaw.transforms.size();
    }

    u32 totalPixels = totalTransforms * kPixelsPerTransform;
    widthHeight = image_dimensions(totalPixels);
    size += Gimg::required_size(kPXL_RGBA32F, widthHeight.x, widthHeight.y);

    return size;
}

Gaim * Gaim::create(const AnimsRaw & animsRaw, u32 referencePathHash)
{
    u32 boneCount;
    u32 animCount;
    u64 imageOffset;
    uvec2 widthHeight;

    u64 size = required_size(boneCount, animCount, imageOffset, widthHeight, animsRaw);
    Gaim * pGaim = alloc_asset<Gaim>(kMEM_Texture, size);

    pGaim->mBoneCount = boneCount;
    pGaim->mAnimCount = animCount;
    pGaim->mImageOffset = imageOffset;

    AnimInfo * pAnims = pGaim->anims();
    Gimg * pGimg = pGaim->image();
    Gimg::init(pGimg, kPXL_RGBA32F, widthHeight.x, widthHeight.y, referencePathHash);
    mat43 * pTransStart = (mat43*)pGimg->pixels();
    mat43 * pTrans = pTransStart;

    for (const auto & animRaw : animsRaw)
    {
        *pAnims = animRaw.info;
        pAnims->framesOffset = (pTrans - pTransStart) * kPixelsPerTransform;
        pAnims++;
        for (const auto & trans : animRaw.transforms)
        {
            *pTrans = trans;
            pTrans++;
        }
    }

    ASSERT(is_valid(pGaim, size));

    return pGaim;
}

Gimg * Gaim::image()
{
    return reinterpret_cast<Gimg*>(reinterpret_cast<u8*>(this) + mImageOffset);
}

const Gimg * Gaim::image() const
{
    return reinterpret_cast<const Gimg*>(reinterpret_cast<const u8*>(this) + mImageOffset);
}

const AnimInfo * Gaim::anim(u32 nameHash) const
{
    const AnimInfo * pAi = anims();
    const AnimInfo * pAiEnd = pAi + mAnimCount;
    while (pAi < pAiEnd)
    {
        if (pAi->nameHash == nameHash)
            return pAi;
        pAi++;
    }
    LOG_ERROR("Anim not found, using index 0: %u, 0x%x", nameHash, nameHash);
    return animByIndex(0);
}

const AnimInfo * Gaim::animByIndex(u32 animIndex) const
{
    ASSERT(animIndex < mAnimCount);
    return anims() + animIndex;
}

const u32 Gaim::animIndex(u32 nameHash) const
{
    const AnimInfo * pAi = anim(nameHash);
    if (pAi)
        return (u32)(pAi - anims());
    return 0;
}

u32 Gaim::frameOffset(const AnimInfo * pAnimInfo, f32 elapsedTime, bool looped) const
{
    ASSERT(pAnimInfo);

    // quick out for non-looped animations at the end
    // typically caller needs to set a different animation up
    // when this happens, so we return -1 here
    if (!looped)
        if (elapsedTime > pAnimInfo->totalTime)
            return -1;

    f32 moddedTime = looped ? fmod(elapsedTime, pAnimInfo->totalTime) : min(elapsedTime, pAnimInfo->totalTime);
    u32 offset = (u32)(moddedTime / pAnimInfo->frameDuration);
    offset = offset * kPixelsPerTransform * mBoneCount;
    offset += pAnimInfo->framesOffset;
    return offset;
}

u32 Gaim::frameOffset(u32 animInfoIndex, f32 elapsedTime, bool looped) const
{
    ASSERT(animInfoIndex < mAnimCount);
    const AnimInfo * pAi = anims() + animInfoIndex;
    return frameOffset(pAi, elapsedTime, looped);
}

const mat43 & Gaim::boneTransform(const AnimInfo * pAnimInfo, u32 frameOffset, u32 boneIdx) const
{
    ASSERT(pAnimInfo);
    const Gimg * pGimg = image();
    const mat43 * pTrans = (mat43*)((vec4*)pGimg->pixels() + frameOffset);
    pTrans += boneIdx;
    return *pTrans;
}

} // namespace gaen

