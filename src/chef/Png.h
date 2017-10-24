//------------------------------------------------------------------------------
// Png.h - Png image processing
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

#ifndef GAEN_CHEF_PNG_H
#define GAEN_CHEF_PNG_H

#include <cstdio>

#include "core/mem.h"
#include "assets/Gimg.h"
#include "chef/cooker_utils.h"

namespace gaen
{
class Png
{
public:
    static UniquePtr<Png> read(const char * path);
    static ImageInfo read_image_info(const char * path);
    static void write_gimg(const char * path, const Gimg * pGimg);
    static PixelFormat color_type_to_pixel_format(int colorType);
    static int pixel_format_to_color_type(PixelFormat pixelFormat);

    ~Png();

    u32 width() { return mWidth; }
    u32 height() { return mHeight; }
    u8 colorType() { return mColorType; }
    u8 bitDepth() { return mBitDepth; }
    u32 bytesPerPixel();

    u8 * scanline(u32 idx);

    // Callers should GFREE pGimg
    void convertToGimg(Gimg ** pGimgOut);

private:
    Png();
    Png(const Png&)              = delete;
    Png(Png&&)                   = delete;
    Png & operator=(const Png&)  = delete;
    Png & operator=(Png&&)       = delete;

    void readInfo(const char * path);
    void readData();

    FILE * mFp;
    png_structp mpPng;
    png_infop mpInfo;
    png_bytep * mppRows;

    bool mIsWriting;

    u32 mWidth;
    u32 mHeight;
    i32 mBitDepth;
    i32 mColorType;
    i32 mInterlaceMethod;
    i32 mCompressionMethod;
    i32 mFilterMethod;
};


} // namespace gaen

#endif // #ifndef GAEN_CHEF_PNG_H
