//------------------------------------------------------------------------------
// Gaud.h - Runtime audio format
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

#ifndef GAEN_ASSETS_GAUD_H
#define GAEN_ASSETS_GAUD_H

#include "gaen/core/base_defines.h"
#include "gaen/core/mem.h"

#include "gaen/assets/AssetHeader.h"
#include "gaen/assets/Color.h"

namespace gaen
{

#pragma pack(push, 1)
class Gaud : public AssetHeader4CC<FOURCC("gaud")>
{
public:
    static bool is_valid(const void * pBuffer, u64 size);
    static Gaud * instance(void * pBuffer, u64 size);
    static const Gaud * instance(const void * pBuffer, u64 size);

    static u64 required_size(u32 localMaxCount, u32 sampleCount);

    static void init(Gaud * pGaud, u32 sampleRatio, u32 numChannels, u32 sampleCount);
    static Gaud * create(u32 sampleRatio, u32 numChannels, u32 sampleCount);

    u32 sampleCount() const { return mSampleCount; }
    u32 sampleRatio() const { return mSampleRatio; }
    u32 numChannels() const { return mNumChannels; }

    u32 localMaxIndex(u32 sampleIdx) const
    {
        return sampleIdx / mSamplesPerLocalMax;
    }

    f32 localMax(u32 sampleIdx) const
    {
        return localMaxes()[localMaxIndex(sampleIdx)];
    }

    f32 * localMaxes()
    {
        return reinterpret_cast<f32*>(reinterpret_cast<u8*>(this) + localMaxOffset());
    }

    const f32 * localMaxes() const
    {
        return const_cast<Gaud*>(this)->localMaxes();
    }

    u32 localMaxOffset() const
    {
        return sizeof(Gaud);
    }

    i16 * samples()
    {
        return reinterpret_cast<i16*>(reinterpret_cast<u8*>(this) + sampleOffset());
    }

    const i16 * samples() const
    {
        return const_cast<Gaud*>(this)->samples();
    }

    u32 sampleOffset() const
    {
        return localMaxOffset() + sizeof(f32) * mLocalMaxCount;
    }

private:
    // Class should not be constructed directly.  Use cast and create static methods.
    Gaud() = default;
    Gaud(const Gaud&) = delete;
    Gaud & operator=(const Gaud&) = delete;

    u32 mSampleCount;
    u32 mLocalMaxCount;
    u32 mSamplesPerLocalMax;
    u16 mSampleRatio; // 1 = 44100, 2 = 22050, 4 = 11025
    u16 mNumChannels; // 1 or 2
};
#pragma pack(pop)

static_assert(sizeof(Gaud) == 32, "Gaud unexpected size");
static_assert(sizeof(Gaud) % 16 == 0, "Gaud size not 2 byte aligned");

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_GAUD_H
