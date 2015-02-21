//------------------------------------------------------------------------------
// Tga.cpp - Targa (.tga) image processing
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

#include "chef/Tga.h"

namespace gaen
{


bool Tga::is_valid(u8 * pBuffer, u32 size)
{
    if (size < sizeof(Tga))
    {
        ERR("tga buffer smaller than header, size: %u", size);
        return false;
    }

    Tga * pTga = reinterpret_cast<Tga*>(pBuffer);

    if (size < pTga->totalSize())
    {
        ERR("tga buffer smaller than required, size: %u", size);
        return false;
    }

    if (pTga->bitsPerPixel != 32 && pTga->bitsPerPixel != 24)
    {
        ERR("only 24 or 32 bit RGB .tga files are supported, bitsPerPixel: %u", pTga->bitsPerPixel);
        return false;
    }

    if (pTga->imageType != kTGAT_Rgb)
    {
        ERR("Only RGB, non RLE .tga files are supported, imageType: %u", pTga->imageType);
        return false;
    }


    switch (pTga->imageType)
    {
    case kTGAT_Rgb:
    case kTGAT_Grayscale:
        // we're ok with these
        break;
    default:
        ERR("Only RGB and Grayscale, non RLE .tga files are supported, imageType: %u", pTga->imageType);
        return false;
    };

    return true;
}

u8 * Tga::id()
{
    if (idLength > 0)
        return postHeader();
    else
        return nullptr;
}

u8 * Tga::colorMap()
{
    return postHeader() + idLength;
}

u32 Tga::colorMapSize()
{
    return colorMapEntryCount * colorMapEntrySize;
}

u8 * Tga::imageData()
{
    return colorMap() + colorMapSize();
}

u32 Tga::imageDataSize()
{
    return bitsPerPixel * width * height / 8;
}

u32 Tga::totalSize()
{
    return sizeof(Tga) + idLength + colorMapSize() + imageDataSize();
}

u8 * Tga::scanline(u32 idx)
{
    ASSERT(idx < height);
    u32 lineSize = bitsPerPixel * width / 8;
    return imageData() + idx * lineSize;
}


} // namespace gaen
