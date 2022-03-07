//------------------------------------------------------------------------------
// Gaud.cpp - Runtime audio format
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

#include "gaen/core/HashMap.h"
#include "gaen/core/String.h"

#include "gaen/math/common.h"

#include "gaen/assets/file_utils.h"
#include "gaen/assets/Gaud.h"

namespace gaen
{

static const u32 kFramesPerLocalMax = 100;

// samples per local max
// rate    ratio   chans   samples per localmax
// 44100     1       2     200
// 44100     1       1     100
// 22050     2       2     100
// 22050     2       1      50
// 11025     4       2      50
// 11025     4       1      25

// formula:
// kFramesPerLocalMax / ratio * chans

static u32 samples_per_local_max(u32 sampleRatio, u32 numChannels)
{
    return kFramesPerLocalMax / sampleRatio * numChannels;
}

static u32 local_max_count(u32 sampleRatio, u32 numChannels, u32 sampleCount)
{
    u32 splm = samples_per_local_max(sampleRatio, numChannels);
    u32 lmc = sampleCount / splm;
    if (sampleCount % splm != 0)
        lmc++;
    return lmc;
}

bool Gaud::is_valid(const void * pBuffer, u64 size)
{
    if (size < sizeof(Gaud))
        return false;

    const Gaud * pAssetData = reinterpret_cast<const Gaud*>(pBuffer);

    if (pAssetData->magic4cc() != kMagic4CC)
        return false;
    if (pAssetData->size() != size)
        return false;

    if (pAssetData->mSampleRatio != 1 && pAssetData->mSampleRatio != 2 && pAssetData->mSampleRatio != 4)
        return false;
    if (pAssetData->mNumChannels != 1 && pAssetData->mNumChannels != 2)
        return false;
    if (pAssetData->mSampleCount == 0)
        return false;
    if (pAssetData->mSampleCount % pAssetData->mNumChannels != 0)
        return false;
    if (pAssetData->mLocalMaxCount != local_max_count(pAssetData->mSampleRatio, pAssetData->mNumChannels, pAssetData->mSampleCount))
        return false;
    if (pAssetData->mSamplesPerLocalMax != samples_per_local_max(pAssetData->mSampleRatio, pAssetData->mNumChannels))
        return false;
    if ((pAssetData->mSampleCount-1) / pAssetData->mSamplesPerLocalMax != (pAssetData->mLocalMaxCount-1))
        return false;

    for (u32 i = 0; i < pAssetData->mLocalMaxCount; ++i)
    {
        if (pAssetData->localMax(i) < 0.0f || pAssetData->localMax(i) > 1.0f)
            return false;
    }

    if (required_size(pAssetData->mLocalMaxCount, pAssetData->mSampleCount) != size)
        return false;

    return true;
}

Gaud * Gaud::instance(void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gaud buffer");
        return nullptr;
    }

    return reinterpret_cast<Gaud*>(pBuffer);
}

const Gaud * Gaud::instance(const void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gaud buffer");
        return nullptr;
    }

    return reinterpret_cast<const Gaud*>(pBuffer);
}

u64 Gaud::required_size(u32 localMaxCount, u32 sampleCount)
{
    u64 size = sizeof(Gaud) + localMaxCount * sizeof(f32) + sampleCount * sizeof(i16);

    return size;
}

void Gaud::init(Gaud * pGaud, u32 sampleRatio, u32 numChannels, u32 sampleCount)
{
    pGaud->mSampleCount = sampleCount;
    pGaud->mLocalMaxCount = local_max_count(sampleRatio, numChannels, sampleCount);
    pGaud->mSamplesPerLocalMax = samples_per_local_max(sampleRatio, numChannels);
    pGaud->mSampleRatio = sampleRatio;
    pGaud->mNumChannels = numChannels;

    f32 * pLocalMax = pGaud->localMaxes();
    f32 * pLocalMaxEnd = pLocalMax + pGaud->mLocalMaxCount;
    while (pLocalMax < pLocalMaxEnd)
    {
        *pLocalMax = 0.0f;
        pLocalMax++;
    }
}

Gaud * Gaud::create(u32 sampleRate, u32 numChannels, u32 sampleCount)
{
    PANIC_IF(sampleRate != 44100 && sampleRate != 22050 && sampleRate != 11025, "Invalid sampleRate: %d", sampleRate);
    PANIC_IF(numChannels != 1 && numChannels != 2, "Invalid numChannels: %d", numChannels);
    PANIC_IF(sampleCount % numChannels != 0, "Invalid sampleCount(%d), not multiple of numChannels(%d)", sampleCount, numChannels);

    PANIC_IF((44100 % sampleRate != 0) || (44100 / sampleRate > 4), "Invalid sampleRate(%d)", sampleRate);
    u32 sampleRatio = 44100 / sampleRate;

    u32 localMaxCount = local_max_count(sampleRatio, numChannels, sampleCount);

    Gaud * pGaud = alloc_asset<Gaud>(kMEM_Audio, required_size(localMaxCount, sampleCount));

    init(pGaud, sampleRatio, numChannels, sampleCount);

    ASSERT(is_valid(pGaud, required_size(localMaxCount, sampleCount)));

    return pGaud;
}

} // namespace gaen

