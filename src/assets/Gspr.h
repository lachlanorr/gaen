//------------------------------------------------------------------------------
// Gspr.h - Sprites with animations, depend on a gatl for images
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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

#ifndef GAEN_ASSETS_GSPR_H
#define GAEN_ASSETS_GSPR_H

#include "assets/AssetHeader.h"

namespace gaen
{

class Gatl;
struct GlyphVert;
struct GlyphTri;

#pragma pack(push, 1)
struct AnimInfo
{
    u32 animHash;
    u32 frameCount:12;
    u32 firstFrame:20;
};
static_assert(sizeof(AnimInfo) == 8, "AnimInfo has unexpected size");

class Gspr : public AssetHeader4CC<FOURCC("gspr")>
{
    template <typename T, typename BT>
    friend class AssetWithDep;
public:
    static bool is_valid(const void * pBuffer, u64 size);
    static Gspr * instance(void * pBuffer, u64 size);
    static const Gspr * instance(const void * pBuffer, u64 size);

    static u64 required_size(const char * atlasPath, u32 animCount, u32 totalFrameCount);

    static Gspr * create(u32 frameWidth,
                         u32 frameHeight,
                         const char * atlasPath,
                         u32 animCount,
                         u32 totalFrameCount);

    u32 frameWidth() const { return mFrameWidth; }
    u32 frameHeight() const { return mFrameHeight; }

    u32 animCount() const { return mAnimCount; }

    const char * atlasPath() const
    {
        // atlasPath is null terminated string immediately after header
        return reinterpret_cast<const char*>(this+1);
    }
    
    const Gatl * atlas() const
    {
        ASSERT(mpAtlas);
        return mpAtlas;
    }

    u32 defaultAnimHash() const;

    const AnimInfo * getAnim(u32 animHash) const;
    const GlyphTri * getFrameElems(const AnimInfo * pAnim, u32 frameIdx) const;
    const void * getFrameElemsOffset(const AnimInfo * pAnim, u32 frameIdx) const;

    AnimInfo * anims();
    const AnimInfo * anims() const;

    u32 * frames();
    const u32 * frames() const;

    u32 * framesEnd();
    const u32 * framesEnd() const;

private:
    // support for templated AssetWithDep class
    const char * dep0Path() const
    {
        return atlasPath();
    }
    const Gatl * dep0() const
    {
        return mpAtlas;
    }
    void setDep0(const Gatl * pAtlas)
    {
        mpAtlas = pAtlas;
    }

    u32 mFrameWidth;
    u32 mFrameHeight;
    
    u32 mAnimCount:12;
    u32 mAnimTocOffset:20;

    const Gatl * mpAtlas;

    char PADDING_[12];

    // What follows header:

    // - null terminated atlasPath (relative to cooked directory, e.g. /foo/bar/baz.atl)
    // - Array of AnimEntries (starting at this address + nAnimTocOffset
    // - Frames referenced by AnimEntries
};
#pragma pack(pop)

static_assert(sizeof(Gspr) == 48, "Gspr unexpected size");
} // namespace


#endif // #ifndef GAEN_ASSETS_GSPR_H
