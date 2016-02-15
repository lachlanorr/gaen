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

struct Gatl
{
public:
    static u32 header_and_path_size(const char * gimgPath);
    static u32 total_size(const char * gimgPath, u8 minChar, u8 maxChar);

    static Gatl * create(MemType memType, const char * gimgPath, u8 minChar, u8 maxChar, u8 defaultChar);
    
    const char * gimgPath() const;

    u32 headerAndPathSize() const;
    u32 totalSize() const;
    GlyphCoords & glyphCoords(char c);
    const GlyphCoords & glyphCoords(char c) const;

    char minChar();
    char maxChar();

private:
    u8 mGimgPathSize;
    u8 mMinChar;
    u8 mMaxChar;
    u8 mDefaultChar; // character to use if requested char is not present
};

static_assert(sizeof(Gatl) == 4, "Gatl unexpected size");

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_GATL_H
