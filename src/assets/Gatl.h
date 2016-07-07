//------------------------------------------------------------------------------
// Gatl.h - Runtime texture atlas format
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

#ifndef GAEN_ASSETS_GATL_H
#define GAEN_ASSETS_GATL_H

#include "core/base_defines.h"
#include "core/mem.h"

namespace gaen
{

class Gimg;

struct GlyphCoords
{
    f32 topLeftU;
    f32 topLeftV;
    f32 bottomRightU;
    f32 bottomRightV;

    static GlyphCoords zero()
    {
        GlyphCoords coords;
        coords.topLeftU = coords.topLeftV = coords.bottomRightU = coords.bottomRightV = 0.0f;
        return coords;
    }
};

// support for HashMap
struct GlyphCoordsHash
{
    u64 operator()(const GlyphCoords & val) const
    {
        const u8 * pBuff = reinterpret_cast<const u8*>(&val);
        return fnv1a_32(pBuff, sizeof(GlyphCoords));
    }
};
struct GlyphCoordsEquals
{
    u64 operator()(const GlyphCoords & lhs, const GlyphCoords & rhs) const
    {
        return (lhs.topLeftU == rhs.topLeftU &&
                lhs.topLeftV == rhs.topLeftV &&
                lhs.bottomRightU == rhs.bottomRightU &&
                lhs.bottomRightV == rhs.bottomRightV);
    }
};

struct GlyphAlias
{
    u32 hash;
    u32 index;
};

struct Gatl
{
public:
    static bool is_valid(const void * pBuffer, u64 size);
    static Gatl * instance(void * pBuffer,u64 size);

    static u64 required_size(u32 glyphCount, u32 aliasCount, const Gimg & image);

    static Gatl * create(u32 glyphCount, u32 aliasCount, u32 defaultIndex, const Gimg & image);

    u64 size() const;

    u32 glyphCount() const { return mGlyphCount; }
    u32 aliasCount() const { return mAliasCount; }

    GlyphCoords & coords(u32 idx)
    {
        if (idx >= mGlyphCount)
            idx = mDefaultIndex;
        return glyphs()[idx];
    }
    const GlyphCoords & coords(u32 idx) const
    {
        Gatl * pGatl = const_cast<Gatl*>(this);
        return pGatl->coords(idx);
    }

    GlyphCoords & defaultCoords() { return coords(mDefaultIndex); }
    const GlyphCoords & defaultCoords() const { return coords(mDefaultIndex); }

    GlyphCoords & coordsFromAlias(u32 aliasHash);
    const GlyphCoords & coordsFromAlias(u32 aliasHash) const
    {
        Gatl * pGatl = const_cast<Gatl*>(this);
        return pGatl->coordsFromAlias(aliasHash);
    }


    GlyphAlias & alias(u32 idx)
    {
        idx %= mAliasCount; // treat glyphs as a circular buffer
        return aliases()[idx];
    }
    const GlyphAlias & alias(u32 idx) const
    {
        Gatl * pGatl = const_cast<Gatl*>(this);
        return pGatl->alias(idx);
    }


    Gimg & image()
    {
        return *reinterpret_cast<Gimg*>(aliases() + mAliasCount);
    }
    const Gimg & image() const
    {
        Gatl * pGatl = const_cast<Gatl*>(this);
        return pGatl->image();
    }
private:
    GlyphCoords * glyphs()
    {
        return reinterpret_cast<GlyphCoords*>(reinterpret_cast<u8*>(this) + sizeof(*this));
    }

    GlyphAlias * aliases()
    {
        return reinterpret_cast<GlyphAlias*>(glyphs() + mGlyphCount);
    }

    static const char * kMagic;
    static const u32 kMagic4cc;
    char mMagic[4];

    u32 mGlyphCount;
    u32 mAliasCount;

    u32 mDefaultIndex;
};

static_assert(sizeof(Gatl) == 16, "Gatl unexpected size");

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_GATL_H
