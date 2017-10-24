//------------------------------------------------------------------------------
// Tga.cpp - Targa (.tga) image processing
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

#include "chef/stdafx.h"

#include "core/mem.h"
#include "assets/Gimg.h"

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

/*    if (pTga->width != pTga->height)
    {
        ERR("tga doesn't have matching width and height: width=%u, height=%u", pTga->width, pTga->height);
        return false;
    }

    if (!is_power_of_two(pTga->width) || !is_power_of_two(pTga->height))
    {
        ERR("tga doesn't have power of two width or height: width=%u, height=%u", pTga->width, pTga->height);
        return false;
    }
    */
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
    return bytesPerPixel() * width * height;
}

u32 Tga::totalSize()
{
    return sizeof(Tga) + idLength + colorMapSize() + imageDataSize();
}

u32 Tga::bytesPerPixel()
{
    return bitsPerPixel / 8;
}

u8 * Tga::scanline(u32 idx)
{
    ASSERT(idx < height);
    u32 lineSize = bytesPerPixel() * width;
    return imageData() + idx * lineSize;
}

void Tga::convertToGimg(Gimg ** pGimgOut)
{
    PixelFormat pixFmt = kPXL_RGBA8;
    if (imageType == kTGAT_Rgb && bitsPerPixel == 24)
        pixFmt = kPXL_RGB8;
    else if (imageType == kTGAT_Rgb && bitsPerPixel == 32)
        pixFmt = kPXL_RGBA8;
    else
        PANIC("Unable to convert tga to Gimg, invalid format");

    Gimg * pGimg = Gimg::create(pixFmt, width, height);
    // If pGimg is larger since we're not power of two or our width
    // and height differ, go ahead and zero out the image.
    if (width != pGimg->width() || height != pGimg->height())
        pGimg->clear(Color(0, 0, 0, 255));

    for (u32 line = 0; line < height; ++line)
    {
        u8 * tgaLine = scanline(line);
        u8 * gimgLine = pGimg->scanline(pGimg->height() - line - 1); // reverse row order for opengl

        for (u32 pix = 0; pix < width; ++pix)
        {
            u32 ipix = bytesPerPixel() * pix;

            // tga pixels are stored bgr and bgra
            gimgLine[ipix+0] = tgaLine[ipix+2];
            gimgLine[ipix+1] = tgaLine[ipix+1];
            gimgLine[ipix+2] = tgaLine[ipix+0];

            if (pixFmt == kPXL_RGBA8)
                gimgLine[ipix+3] = tgaLine[ipix+3];
        }
    }

    *pGimgOut = pGimg;
}


} // namespace gaen
