//------------------------------------------------------------------------------
// Gspr.h - Sprites with animations, depend on a gatl for images
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

#ifndef GAEN_ASSETS_GSPR_H
#define GAEN_ASSETS_GSPR_H

namespace gaen
{

class Gatl;
struct GlyphCoords;

class Gspr
{
    friend class AssetTypeGspr;
public:
    static bool is_valid(const void * pBuffer, u64 size);
    static Gspr * instance(void * pBuffer, u64 size);
    static const Gspr * instance(const void * pBuffer, u64 size);

    static u64 required_size();

    static Gspr * create();

    u64 size() const;

    u32 glyphWidth();
    u32 glyphHeight();

    u32 animCount();
    u32 frameCount(u32 animHash);
    const GlyphCoords & getFrame(u32 animHash, u32 frameIdx);

    const char * atlasPath() const;
    
    const Gatl * atlas() const
    {
        ASSERT(pAtlas);
        return pAtlas;
    }

private:
    static const char * kMagic;
    static const u32 kMagic4cc;
    char mMagic[4];

    u32 mAnimCount;
    
    
    const Gatl * pAtlas;
};

} // namespace


#endif // #ifndef GAEN_ASSETS_GSPR_H
