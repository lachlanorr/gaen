//------------------------------------------------------------------------------
// Gatl.cpp - Runtime texture atlas format
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#include "assets/Gatl.h"

namespace gaen
{

u32 Gatl::header_and_path_size(const char * gimgPath)
{
    u32 pathSize = (u32)strlen(gimgPath) + 1; // +1 for null
    u32 headerAndPathSize = pathSize + sizeof(Gatl);

    // align size to 16 bytes so our coords start o 16 byte boundary
    headerAndPathSize = align(headerAndPathSize, 16);
    
    return headerAndPathSize;
}

u32 Gatl::total_size(const char * gimgPath, u8 minChar, u8 maxChar)
{
    u32 charCount = maxChar - minChar + 1;
    u32 coordSize = charCount * sizeof(GlyphCoords);

    u32 headerAndPathSize = header_and_path_size(gimgPath);
    return headerAndPathSize + coordSize;
}



Gatl * Gatl::create(MemType memType, const char * gimgPath, u8 minChar, u8 maxChar, u8 defaultChar)
{
    PANIC_IF(maxChar < minChar, "Invalid maxChar/minChar == %u/%u", minChar, maxChar);
    PANIC_IF(defaultChar < minChar || defaultChar > maxChar, "Invalid defaultChar: %u", defaultChar);

    char * pBuff = (char*)GALLOC(memType, total_size(gimgPath, minChar, maxChar));

    Gatl * pGatl = reinterpret_cast<Gatl*>(pBuff);

    size_t pathSize = strlen(gimgPath) + 1; // +1 for null
    PANIC_IF(pathSize > 255, "GimgPathSize too large: %u", pathSize);
    pGatl->mGimgPathSize = (u8)pathSize;

    char * pPath = pBuff + sizeof(Gatl);
    strcpy(pPath, gimgPath);

    ASSERT(align(pGatl->mGimgPathSize + sizeof(Gatl), 16) == header_and_path_size(gimgPath));
    
    pGatl->mMinChar = minChar;
    pGatl->mMaxChar = maxChar;
    pGatl->mDefaultChar = defaultChar;

    return pGatl;
}

const char * Gatl::gimgPath() const
{
    ASSERT((std::intptr_t)this % 16 == 0);
    return reinterpret_cast<const char*>(this) + sizeof(Gatl);
}

u32 Gatl::headerAndPathSize() const
{
    return align((u32)sizeof(Gatl) + mGimgPathSize, 16);
}

u32 Gatl::totalSize() const
{
    u32 charCount = mMaxChar - mMinChar + 1;
    u32 coordSize = charCount * sizeof(GlyphCoords);
    return headerAndPathSize() + coordSize;
}

GlyphCoords & Gatl::glyphCoords(char c)
{
    const Gatl * pGatlConst = const_cast<const Gatl*>(this);
    return const_cast<GlyphCoords&>(pGatlConst->glyphCoords(c));
}

const GlyphCoords & Gatl::glyphCoords(char c) const
{
    ASSERT((std::intptr_t)this % 16 == 0);
    if (c < mMinChar || c > mMaxChar)
        c = mDefaultChar;

    const GlyphCoords * pCoords = reinterpret_cast<const GlyphCoords*>(reinterpret_cast<const u8*>(this) + headerAndPathSize());
    
    u32 idx = c - mMinChar;
    return pCoords[idx];
}



} // namespace gaen

