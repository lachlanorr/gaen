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

#include <glm/vec3.hpp>
#include "core/base_defines.h"
#include "core/mem.h"

namespace gaen
{

class Gimg;


struct GlyphVert
{
    glm::vec3 position;
    f32 u;
    f32 v;
};
static_assert(sizeof(GlyphVert) == 20, "SpriteVert unexpected size");

struct GlyphTri
{
    u16 p0;
    u16 p1;
    u16 p2;

    GlyphTri(u16 p0, u16 p1, u16 p2)
      : p0(p0)
      , p1(p1)
      , p2(p2)
    {}
};
static_assert(sizeof(GlyphTri) == 6, "SpriteTri unexpected size");

struct GlyphAlias
{
    u32 hash;
    u32 index;
};

class Gatl
{
public:
    static bool is_valid(const void * pBuffer, u64 size);
    static Gatl * instance(void * pBuffer, u64 size);
    static const Gatl * instance(const void * pBuffer, u64 size);

    static u64 required_size(u32 glyphCount, u32 aliasCount, const Gimg & image);

    static Gatl * create(u32 glyphCount, u32 aliasCount, u32 defaultIndex, const Gimg & image);

    u64 size() const;

    u32 glyphCount() const { return mGlyphCount; }
    u32 aliasCount() const { return mAliasCount; }

    i32 glyphElemCount() const { return 6; } // 2 triangles

    GlyphTri * glyphElems(u32 idx)
    {
        if (idx >= mGlyphCount)
            idx = mDefaultIndex;
        return reinterpret_cast<GlyphTri*>(tris() + 2 * idx); // 2 tris per
    }
    const GlyphTri * glyphElems(u32 idx) const
    {
        Gatl * pGatl = const_cast<Gatl*>(this);
        return pGatl->glyphElems(idx);
    }
    const void * glyphElemsOffset(u32 idx)
    {
        return glyphElemsOffset(glyphElems(idx));
    }

    GlyphTri * defaultGlyphElems() { return glyphElems(mDefaultIndex); }
    const GlyphTri * defaultGlyphElems() const { return glyphElems(mDefaultIndex); }
    const void * defaultGlyphElemsOffset() const { return glyphElemsOffset(glyphElems(mDefaultIndex)); }

    GlyphTri * glyphElemsFromAlias(u32 aliasHash);
    const GlyphTri * glyphElemsFromAlias(u32 aliasHash) const
    {
        Gatl * pGatl = const_cast<Gatl*>(this);
        return pGatl->glyphElemsFromAlias(aliasHash);
    }
    const void * glyphElemsFromAliasOffset(u32 aliasHash) const
    {
        return glyphElemsOffset(glyphElemsFromAlias(aliasHash));
    }

    const void * glyphElemsOffset(const GlyphTri * pTri) const
    {
        return reinterpret_cast<const void*>((pTri - tris()) * sizeof(GlyphTri));
    }
    
    GlyphVert & vert(u32 idx)
    {
        return verts()[idx];
    }
    const GlyphVert & vert(u32 idx) const
    {
        Gatl * pGatl = const_cast<Gatl*>(this);
        return pGatl->vert(idx);
    }

    GlyphTri & tri(u32 idx)
    {
        return tris()[idx];
    }
    const GlyphTri & tri(u32 idx) const
    {
        Gatl * pGatl = const_cast<Gatl*>(this);
        return pGatl->tri(idx);
    }


    u64 vertsSize() const
    {
        return mGlyphCount * sizeof(GlyphVert) * 4; // 4 verts per glyph
    }

    u64 trisSize() const
    {
        return mGlyphCount * sizeof(GlyphTri) * 2; // 2 tris per glyph
    }

    GlyphVert * verts()
    {
        return reinterpret_cast<GlyphVert*>(reinterpret_cast<u8*>(this) +
                                            sizeof(Gatl));
    }

    const GlyphVert * verts() const
    {
        Gatl * pGatl = const_cast<Gatl*>(this);
        return pGatl->verts();
    }

    GlyphTri * tris()
    {
        return reinterpret_cast<GlyphTri*>(reinterpret_cast<u8*>(this) +
                                           sizeof(Gatl) +
                                           verts_size_aligned(mGlyphCount));
    }

    const GlyphTri * tris() const
    {
        Gatl * pGatl = const_cast<Gatl*>(this);
        return pGatl->tris();
    }

    GlyphAlias * aliases()
    {
        return reinterpret_cast<GlyphAlias*>(reinterpret_cast<u8*>(this) +
                                             sizeof(Gatl) +
                                             verts_size_aligned(mGlyphCount) +
                                             tris_size_aligned(mGlyphCount));
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
        return *reinterpret_cast<Gimg*>(reinterpret_cast<u8*>(this) + gimg_offset_aligned(mGlyphCount, mAliasCount));
    }
    const Gimg & image() const
    {
        Gatl * pGatl = const_cast<Gatl*>(this);
        return pGatl->image();
    }
private:
    static u64 verts_size_aligned(u32 glyphCount)
    {
        return align(glyphCount * sizeof(GlyphVert) * 4, 4); // 4 verts per glyph
    }

    static u64 tris_size_aligned(u32 glyphCount)
    {
        return align(glyphCount * sizeof(GlyphTri) * 2, 4); // 2 tris (6 indices) per glyph
    }

    static u64 aliases_size_aligned(u32 aliasCount)
    {
        return align(aliasCount * sizeof(GlyphAlias), 4);
    }

    static u64 gimg_offset_aligned(u32 glyphCount, u32 aliasCount);

    static const char * kMagic;
    static const u32 kMagic4cc;
    char mMagic[4];

    u32 mGlyphCount;
    u32 mAliasCount;

    u32 mDefaultIndex;
};

// If this isn't a multiple of 16, consider adding more alignment
// corrections in size functions
static_assert(sizeof(Gatl) == 16, "Gatl unexpected size");

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_GATL_H
