//------------------------------------------------------------------------------
// Gaim.h - Animation data stored in F32 gimg
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

#ifndef GAEN_ASSETS_GAIM_H
#define GAEN_ASSETS_GAIM_H

#include "core/base_defines.h"
#include "core/mem.h"
#include "core/Vector.h"
#include "core/List.h"

#include "math/mat43.h"
#include "math/vec2.h"

#include "assets/AssetHeader.h"

namespace gaen
{

class Gimg;

#pragma pack(push, 1)
struct AnimInfo
{
    u32 nameHash;
    f32 totalTime;
    f32 frameDuration;
    u32 frameCount:11;
    u32 framesOffset:20;
    u32 isLoopable:1;
};

class Gaim : public AssetHeader4CC<FOURCC("gaim")>
{
public:
    static const u32 kPixelsPerTransform = 3; // kBytesPerBone / bytes_per_pixel(kPXL_RGBA32F);

    typedef Vector<kMEM_Chef, mat43> Transforms;
    struct AnimRaw
    {
        AnimInfo info;
        Transforms transforms;
    };
    typedef List<kMEM_Chef, AnimRaw> AnimsRaw;

    static bool is_valid(const void * pBuffer, u64 size);
    static Gaim * instance(void * pBuffer, u64 size);
    static const Gaim * instance(const void * pBuffer, u64 size);

    static u64 required_size(u32 & boneCount,
                             u32 & animCount,
                             u64 & imageOffset,
                             uvec2 & widthHeight,
                             const AnimsRaw & animsRaw);

    static Gaim * create(const AnimsRaw & animsRaw, u32 referencePathHash);

    Gimg * image();
    const Gimg * image() const;

    const AnimInfo * anim(u32 nameHash) const;
    const AnimInfo * animByIndex(u32 nameHash) const;
    const u32 animIndex(u32 animIndex) const;
    u32 frameOffset(const AnimInfo * pAnimInfo, f32 elapsedTime) const;
    u32 frameOffset(u32 animInfoIndex, f32 elapsedTime) const;
    const mat43 & boneTransform(const AnimInfo * pAnimInfo, u32 frameOffset, u32 boneIdx) const;

private:
    // Class should not be constructed directly.  Use cast and create static methods.
    Gaim() = delete;
    Gaim(const Gaim&) = delete;
    Gaim & operator=(const Gaim&) = delete;

    static const u32 kBytesPerBone = 48;
    u32 frame_stride() const
    {
        return mBoneCount * kBytesPerBone;
    }

    AnimInfo * anims()
    {
        return reinterpret_cast<AnimInfo*>(reinterpret_cast<u8*>(this) + sizeof(Gaim));
    }

    const AnimInfo * anims() const
    {
        return reinterpret_cast<const AnimInfo*>(reinterpret_cast<const u8*>(this) + sizeof(Gaim));
    }

    u64 imageOffset() const
    {
        return mImageOffset;
    }

    u32 mBoneCount;
    u32 mAnimCount;
    u64 mImageOffset;
};
#pragma pack(pop)

static_assert(sizeof(AnimInfo) == 16, "AnimInfo unexpected size");
static_assert(sizeof(Gaim) % 16 == 0, "Gaim size not 16 byte aligned");

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_GAIM_H
