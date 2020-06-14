//------------------------------------------------------------------------------
// Png.cpp - Png image processing
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

#include "assets/Gimg.h"

#include "chef/Png.h"

namespace gaen
{
Png::Png()
{
    memset(this, 0, sizeof(Png));
}

Png::~Png()
{
    if (mpPng)
    {
        ASSERT(mpInfo);

        if (!mIsWriting)
            png_destroy_read_struct(&mpPng, &mpInfo, nullptr);
        else
            png_destroy_write_struct(&mpPng, &mpInfo);

        mpPng = nullptr;
        mpInfo = nullptr;
    }

    if (mFp)
    {
        fclose(mFp);
        mFp = 0;
    }
}

void Png::readInfo(const char * path)
{
    ASSERT(!mFp && !mpPng && !mpInfo && !mppRows);

    mFp = fopen(path, "rb");
    PANIC_IF(mFp == 0, "Unable to read png file: %s", path);

    mpPng = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    PANIC_IF(!mpPng, "png_create_read_struct failed");

    mpInfo = png_create_info_struct(mpPng);
    PANIC_IF(!mpInfo, "png_create_info_struct failed");

    if (setjmp(png_jmpbuf(mpPng)))
        PANIC("Unable to setup libpng error handlers");

    png_init_io(mpPng, mFp);

    png_read_png(mpPng, mpInfo, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_get_IHDR(mpPng, mpInfo, &mWidth, &mHeight, &mBitDepth, &mColorType, &mInterlaceMethod, &mCompressionMethod, &mFilterMethod);
}

void Png::readData()
{
    ASSERT(mFp && mpPng && mpInfo && !mppRows);
    mppRows = png_get_rows(mpPng, mpInfo);
    PANIC_IF(!mppRows, "Failed to png_get_rows");
}

UniquePtr<Png> Png::read(const char * path)
{
    UniquePtr<Png> pPng(GNEW(kMEM_Chef, Png));

    pPng->readInfo(path);
    pPng->readData();

    return pPng;
}

ImageInfo Png::read_image_info(const char * path)
{
    Png png;
    png.readInfo(path);
    ImageInfo imgInf(kORIG_TopLeft, (u16)png.width(), (u16)png.height());
    return imgInf;
}

void Png::write_gimg(const char * path, const Gimg * pGimg)
{
    ASSERT(path);
    ASSERT(pGimg);

    Png png;

    png.mFp = fopen(path, "wb");
    PANIC_IF(!png.mFp, "Unable to open file for writing png: %s", path);

    png.mpPng = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png.mIsWriting = true;
    PANIC_IF(!png.mpPng, "png_create_read_struct failed");

    png.mpInfo = png_create_info_struct(png.mpPng);
    PANIC_IF(!png.mpInfo, "png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png.mpPng)))
        PANIC("Unable to setup libpng error handlers");

    png_init_io(png.mpPng, png.mFp);

    int colorType = pixel_format_to_color_type(pGimg->pixelFormat());

    png_set_IHDR(png.mpPng,
                 png.mpInfo,
                 pGimg->width(),
                 pGimg->height(),
                 8,
                 colorType,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    png_write_info(png.mpPng, png.mpInfo);

    for (u32 line = 0; line < pGimg->height(); ++line)
    {
        const u8 * gimgLine = pGimg->scanline(pGimg->height() - line - 1); // reverse row order for opengl
        png_write_row(png.mpPng, gimgLine);
    }

    png_write_end(png.mpPng, NULL);
}

u8 * Png::scanline(u32 idx)
{
    ASSERT(mppRows);
    PANIC_IF(idx >= mHeight, "Invalid scanline idx: %d", idx);

    return mppRows[idx];
}

u32 Png::bytesPerPixel()
{
    ASSERT(mpPng);
    PANIC_IF(mBitDepth != 8, "Invalid png BitDepth: %d", mBitDepth);
    switch(mColorType)
    {
    case PNG_COLOR_TYPE_RGB:
        return 3;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        return 4;
    case PNG_COLOR_TYPE_GRAY:
        return 1;
    default:
        PANIC("Invalid png ColorType: %d", mColorType);
        return 0;
    }
}

PixelFormat Png::color_type_to_pixel_format(int colorType)
{
    switch(colorType)
    {
    case PNG_COLOR_TYPE_RGB:
        return kPXL_RGB8;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        return kPXL_RGBA8;
    case PNG_COLOR_TYPE_GRAY:
        return kPXL_R8;
    default:
        PANIC("Invalid png ColorType: %d", colorType);
        return kPXL_R8;
    }
}

int Png::pixel_format_to_color_type(PixelFormat pixelFormat)
{
    switch(pixelFormat)
    {
    case kPXL_RGB8:
        return PNG_COLOR_TYPE_RGB;
    case kPXL_RGBA8:
        return PNG_COLOR_TYPE_RGB_ALPHA;
    case kPXL_R8:
        return PNG_COLOR_TYPE_GRAY;
    default:
        PANIC("Invalid png pixelFormat: %d", pixelFormat);
        return PNG_COLOR_TYPE_GRAY;
    }
}


// Callers should GFREE pGimg
void Png::convertToGimg(Gimg ** pGimgOut, u32 referencePathHash)
{
    ASSERT(mppRows);

    PixelFormat pixFmt = color_type_to_pixel_format(mColorType);

    Gimg * pGimg = Gimg::create(pixFmt, mWidth, mHeight, referencePathHash);
    // If pGimg is larger since we're not power of two or our width
    // and height differ, go ahead and zero out the image.
    if (mWidth != pGimg->width() || mHeight != pGimg->height())
        pGimg->clear(Color(0, 0, 0, 255));

    u32 bpp = bytesPerPixel();

    for (u32 line = 0; line < mHeight; ++line)
    {
        u8 * pngLine = scanline(line);
        u8 * gimgLine = pGimg->scanline(pGimg->height() - line - 1); // reverse row order for opengl

        for (u32 pix = 0; pix < mWidth; ++pix)
        {
            u32 ipix = bpp * pix;

            for (u32 ch = 0; ch < bpp; ++ch)
            {
                gimgLine[ipix + ch] = pngLine[ipix + ch];
            }
        }
    }

    *pGimgOut = pGimg;

}

} // namespace gaen

