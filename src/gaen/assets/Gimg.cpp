//------------------------------------------------------------------------------
// Gimg.cpp - Runtime image format
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2022 Lachlan Orr
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

#include "gaen/core/HashMap.h"
#include "gaen/core/String.h"

#include "gaen/math/common.h"

#include "gaen/assets/file_utils.h"
#include "gaen/assets/Gimg.h"

namespace gaen
{
typedef HashMap<kMEM_Const, PixelFormat, String<kMEM_Const>> PixelFormatToStrMap;
typedef HashMap<kMEM_Const, String<kMEM_Const>, PixelFormat> PixelFormatFromStrMap;
typedef HashMap<kMEM_Const, PixelFormat, u32> PixelFormatToU32Map;

const char * pixel_format_to_str(PixelFormat pixelFormat)
{
    static const PixelFormatToStrMap map =
        {{ kPXL_R8,        "R8"        },
         { kPXL_RGB8,      "RGB8"      },
         { kPXL_RGBA8,     "RGBA8"     },
         { kPXL_RGB_DXT1,  "RGB_DXT1"  },
         { kPXL_RGBA_DXT1, "RGBA_DXT1" },
         { kPXL_RGBA_DXT3, "RGBA_DXT3" },
         { kPXL_RGBA_DXT5, "RGBA_DXT5" },
         { kPXL_RGB32F,    "RGB32F"    },
         { kPXL_RGBA32F,   "RGBA32F"   }
        };

    auto it = map.find(pixelFormat);
    PANIC_IF(it == map.end(), "Invalid PixelFormat: %d", pixelFormat);
    return it->second.c_str();
}

const PixelFormat pixel_format_from_str(const char * str)
{
    static const PixelFormatFromStrMap map =
        {{ "R8",        kPXL_R8 },
         { "RGB8",      kPXL_RGB8 },
         { "RGBA8",     kPXL_RGBA8 },
         { "RGB_DXT1",  kPXL_RGB_DXT1 },
         { "RGBA_DXT1", kPXL_RGBA_DXT1 },
         { "RGBA_DXT3", kPXL_RGBA_DXT3 },
         { "RGBA_DXT5", kPXL_RGBA_DXT5 },
         { "RGB32F",    kPXL_RGB32F },
         { "RGBA32F",   kPXL_RGBA32F }
        };

    auto it = map.find(str);
    PANIC_IF(it == map.end(), "Invlaid PixelFormat string: %s", str);
    return it->second;
}

u32 bytes_per_pixel(PixelFormat pixelFormat)
{
    static const PixelFormatToU32Map map =
        {{ kPXL_R8,         1 },
         { kPXL_RGB8,       3 },
         { kPXL_RGBA8,      4 },
         { kPXL_RGB32F,    12 },
         { kPXL_RGBA32F,   16 }
        };

    auto it = map.find(pixelFormat);
    PANIC_IF(it == map.end(), "Invalid PixelFormat: %d", pixelFormat);
    return it->second;
}

bool Gimg::is_valid(const void * pBuffer, u64 size)
{
    if (size < sizeof(Gimg))
        return false;

    const Gimg * pAssetData = reinterpret_cast<const Gimg*>(pBuffer);

    if (pAssetData->magic4cc() != kMagic4CC)
        return false;
    if (pAssetData->size() != size)
        return false;

    switch (pAssetData->mPixelFormat)
    {
    case kPXL_RGB_DXT1:
    case kPXL_RGBA_DXT1:
    case kPXL_RGBA_DXT3:
    case kPXL_RGBA_DXT5:
        // DXT textures should have multiples of 4 for width and height
        if (pAssetData->mWidth % 4 != 0 || pAssetData->mHeight % 4 != 0)
            return false;
    }

    return true;
}

Gimg * Gimg::instance(void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gimg buffer");
        return nullptr;
    }

    return reinterpret_cast<Gimg*>(pBuffer);
}

const Gimg * Gimg::instance(const void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gimg buffer");
        return nullptr;
    }

    return reinterpret_cast<const Gimg*>(pBuffer);
}

u64 Gimg::required_size(PixelFormat pixelFormat, u32 width, u32 height)
{
    u64 size = sizeof(Gimg);

    switch (pixelFormat)
    {
    case kPXL_Reference:
        // just the header
        break;
    case kPXL_RGB_DXT1:
    case kPXL_RGBA_DXT1:
        PANIC_IF(width % 4 != 0 || height % 4 != 0, "DXT1 texture with width or height not a multiple of 4");
        size += (u64)width * height / 2; // every block of 16 pixels becomes 8 bytes
        break;
    case kPXL_RGBA_DXT3:
    case kPXL_RGBA_DXT5:
        PANIC_IF(width % 4 != 0 || height % 4 != 0, "DXT texture with width or height not a multiple of 4");
        size += (u64)width * height; // every block of 16 pixels becomes 16 bytes
        break;
    default:
        size += (u64)width * height * bytes_per_pixel(pixelFormat);
        break;
    }

    return size;
}

void Gimg::init(Gimg * pGimg, PixelFormat pixelFormat, u32 width, u32 height, u32 referencePathHash)
{
    pGimg->mPixelFormat = pixelFormat;
    pGimg->mWidth = width;
    pGimg->mHeight = height;
    pGimg->mReferencePathHash = referencePathHash;
}

Gimg * Gimg::create(PixelFormat pixelFormat, u32 width, u32 height, u32 referencePathHash)
{
    // adjust width and height to ensure power of 2 and equal size
    PANIC_IF(width != next_power_of_two(width), "Width not a power of 2");
    PANIC_IF(height != next_power_of_two(height), "Height not a power of 2");

    u64 size = Gimg::required_size(pixelFormat, width, height);
    Gimg * pGimg = alloc_asset<Gimg>(kMEM_Texture, size);

    init(pGimg, pixelFormat, width, height, referencePathHash);

    ASSERT(is_valid(pGimg, required_size(pixelFormat, width, height)));

    return pGimg;
}

Gimg * Gimg::create(u32 referencePathHash)
{
    return create(kPXL_Reference, 0, 0, referencePathHash);
}

u8 * Gimg::pixels()
{
    return (u8*)this + sizeof(Gimg);
}

const u8 * Gimg::pixels() const
{
    return (const u8*)this + sizeof(Gimg);
}

u8 * Gimg::scanline(u32 idx)
{
    PANIC_IF(idx + 1 > mHeight, "Invalid scanline %d, for image with height %d", idx, mHeight);

    u8 * pScanline = pixels();

    pScanline += (u64)idx * mWidth * bytes_per_pixel(mPixelFormat);

    return pScanline;
}

const u8 * Gimg::scanline(u32 idx) const
{
    Gimg * pGimg = const_cast<Gimg*>(this);
    return pGimg->scanline(idx);
}

void Gimg::convertFormat(Gimg ** pGimgOut, MemType memType, PixelFormat newPixelFormat) const
{
    Gimg *pGimg = Gimg::create(newPixelFormat, mWidth, mHeight, mReferencePathHash);
    *pGimgOut = pGimg;

    if (mPixelFormat == newPixelFormat)
    {
        memcpy(pGimg, this, size());
        return;
    }

    switch (mPixelFormat)
    {
    case kPXL_R8:
    {
        switch (newPixelFormat)
        {
        case kPXL_RGB8:
        {
            for (u32 line = 0; line < mHeight; ++line)
            {
                const u8 * fromLine = scanline(line);
                u8 * toLine = pGimg->scanline(line);
                for (u32 pix = 0; pix < mWidth; ++pix)
                {
                    u32 fpix = pix;
                    u32 tpix = 3 * pix;
                    toLine[tpix+0] = fromLine[fpix];
                    toLine[tpix+1] = fromLine[fpix];
                    toLine[tpix+2] = fromLine[fpix];
                }
            }
            return;
        }
        case kPXL_RGBA8:
        {
            for (u32 line = 0; line < mHeight; ++line)
            {
                const u8 * fromLine = scanline(line);
                u8 * toLine = pGimg->scanline(line);
                for (u32 pix = 0; pix < mWidth; ++pix)
                {
                    u32 fpix = pix;
                    u32 tpix = 4 * pix;
                    toLine[tpix+0] = fromLine[fpix];
                    toLine[tpix+1] = fromLine[fpix];
                    toLine[tpix+2] = fromLine[fpix];
                    toLine[tpix+3] = 255;
                }
            }
            return;
        }
        }
        break;
    }
    case kPXL_RGB8:
    {
        switch (newPixelFormat)
        {
        case kPXL_R8:
        {
            for (u32 line = 0; line < mHeight; ++line)
            {
                const u8 * fromLine = scanline(line);
                u8 * toLine = pGimg->scanline(line);
                for (u32 pix = 0; pix < mWidth; ++pix)
                {
                    u32 fpix = 3 * pix;
                    u32 tpix = pix;

                    u8 lum = Color::luminance(fromLine[fpix+0],
                                              fromLine[fpix+1],
                                              fromLine[fpix+2]);
                    toLine[tpix] = lum;
                }
            }
            return;
        }
        case kPXL_RGBA8:
        {
            for (u32 line = 0; line < mHeight; ++line)
            {
                const u8 * fromLine = scanline(line);
                u8 * toLine = pGimg->scanline(line);
                for (u32 pix = 0; pix < mWidth; ++pix)
                {
                    u32 fpix = 3 * pix;
                    u32 tpix = 4 * pix;

                    toLine[tpix+0] = fromLine[fpix+0];
                    toLine[tpix+1] = fromLine[fpix+1];
                    toLine[tpix+2] = fromLine[fpix+2];
                    toLine[tpix+3] = 255;
                }
            }
            return;
        }
        }
        break;
    }
    case kPXL_RGBA8:
    {
        switch (newPixelFormat)
        {
        case kPXL_R8:
        {
            for (u32 line = 0; line < mHeight; ++line)
            {
                const u8 * fromLine = scanline(line);
                u8 * toLine = pGimg->scanline(line);
                for (u32 pix = 0; pix < mWidth; ++pix)
                {
                    u32 fpix = 4 * pix;
                    u32 tpix = pix;

                    u8 lum = Color::luminance(fromLine[fpix+0],
                                              fromLine[fpix+1],
                                              fromLine[fpix+2]);
                    toLine[tpix] = lum;
                }
            }
            return;
        }
        case kPXL_RGB8:
        {
            for (u32 line = 0; line < mHeight; ++line)
            {
                const u8 * fromLine = scanline(line);
                u8 * toLine = pGimg->scanline(line);
                for (u32 pix = 0; pix < mWidth; ++pix)
                {
                    u32 fpix = 4 * pix;
                    u32 tpix = 3 * pix;

                    toLine[tpix+0] = fromLine[fpix+0];
                    toLine[tpix+1] = fromLine[fpix+1];
                    toLine[tpix+2] = fromLine[fpix+2];
                }
            }
            return;
        }
        }
        break;
    }
    }

    GFREE(pGimg);
    PANIC("Unsupported Gimg conversion from: %d -> %d", mPixelFormat, newPixelFormat);
}

void Gimg::clear(Color col)
{
    switch (mPixelFormat)
    {
    case kPXL_R8:
        for (u32 line = 0; line < mHeight; ++line)
        {
            u8 * pLine = scanline(line);
            for (u32 pix = 0; pix < mWidth; ++pix)
            {
                pLine[pix] = col.luminance();
            }
        }
        return;
    case kPXL_RGB8:
        for (u32 line = 0; line < mHeight; ++line)
        {
            u8 * pLine = scanline(line);
            for (u32 pix = 0; pix < mWidth; ++pix)
            {
                u32 idx = pix * 3;
                pLine[idx] = col.r() * col.a();
                pLine[idx+1] = col.g() * col.a();
                pLine[idx+2] = col.b() * col.a();
            }
        }
        return;
    case kPXL_RGBA8:
        for (u32 line = 0; line < mHeight; ++line)
        {
            u8 * pLine = scanline(line);
            for (u32 pix = 0; pix < mWidth; ++pix)
            {
                u32 idx = pix * 4;
                pLine[idx] = col.r();
                pLine[idx+1] = col.g();
                pLine[idx+2] = col.b();
                pLine[idx+3] = col.a();
            }
        }
        return;
    }

    PANIC("Unsupported PixelFormat to clear: %d", mPixelFormat);
}

} // namespace gaen

