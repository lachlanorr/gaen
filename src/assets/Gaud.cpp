//------------------------------------------------------------------------------
// Gaud.cpp - Runtime audio format
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#include "math/common.h"

#include "assets/file_utils.h"
#include "assets/Gaud.h"

namespace gaen
{

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
    if (required_size(pAssetData->mSampleCount) != size)
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

u64 Gaud::required_size(u32 sampleCount)
{
    u64 size = sizeof(Gaud) + sampleCount * sizeof(i16);

    return size;
}

void Gaud::init(Gaud * pGaud, u32 sampleRatio, u32 numChannels, u32 sampleCount)
{
    pGaud->mSampleRatio = sampleRatio;
    pGaud->mNumChannels = numChannels;
    pGaud->mSampleCount = sampleCount;
}

Gaud * Gaud::create(u32 sampleRatio, u32 numChannels, u32 sampleCount)
{
    PANIC_IF(sampleRatio != 1 && sampleRatio != 2 && sampleRatio != 4, "Invalid sampleRatio: %d", sampleRatio);
    PANIC_IF(numChannels != 1 && numChannels != 2, "Invalid numChannels: %d", numChannels);
    PANIC_IF(sampleCount % numChannels != 0, "Invalid sampleCount(%d), not multiple of numChannels(%d)", sampleCount, numChannels);

    Gaud * pGaud = alloc_asset<Gaud>(kMEM_Audio, required_size(sampleCount));

    init(pGaud, sampleRatio, numChannels, sampleCount);

    ASSERT(is_valid(pGaud, required_size(sampleCount)));

    return pGaud;
}

i16 * Gaud::samples()
{
    return (i16*)((u8*)this + sizeof(Gaud));
}

const i16 * Gaud::samples() const
{
    return (const i16*)((u8*)this + sizeof(Gaud));
}

} // namespace gaen

