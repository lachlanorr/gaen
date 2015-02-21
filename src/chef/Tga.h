//------------------------------------------------------------------------------
// Tga.h - Targa (.tga) image processing
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

#ifndef GAEN_CHEF_TGA_H
#define GAEN_CHEF_TGA_H

#include "core/base_defines.h"

namespace gaen
{

enum TgaImageType
{
    kTGAT_NoImageData = 0,
    kTGAT_ColorMapped = 1,
    kTGAT_Rgb = 2,
    kTGAT_Grayscale = 3,
    kTGAT_RleColorMapped = 9,
    kTGAT_RleRgb = 10,
    kTGAT_RleGrayscale = 11
};

#pragma pack(push, 1)
struct Tga
{
    static bool is_valid(u8 * pBuffer, u32 size);

    u8 * id();
    u32 idSize();
    u8 * colorMap();
    u32 colorMapSize();
    u8 * imageData();
    u32 imageDataSize();
    u32 totalSize();

    u8 * scanline(u32 idx);

    u8 idLength;
    u8 colorMapType;
    u8 imageType;

    // color map
    u16 firstColorMapEntry;
    u16 colorMapEntryCount;
    u8 colorMapEntrySize;

    // image specification
    u16 xOrigin;
    u16 yOrigin;
    u16 width;
    u16 height;

    u8 bitsPerPixel;

    u8 interleaving:2;
    u8 screenOrigin:1;
    u8 RESERVED:1;
    u8 attribBitsPerPixel:4;

private:
    u8 * postHeader() { return reinterpret_cast<u8*>(this) + sizeof(Tga); }
};
#pragma pack(pop)

static_assert(sizeof(Tga) == 18, "Tga should be 18 bytes");

} // namespace gaen

#endif // #ifndef GAEN_CHEF_TGA_H
