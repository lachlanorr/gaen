//------------------------------------------------------------------------------
// Png.cpp - Png image processing
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

#include "chef/stdafx.h"

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
        png_destroy_info_struct(mpPng, &mpInfo);
        mpPng = nullptr;
        mpInfo = nullptr;
    }

    if (mFd)
    {
        fclose(mFd);
        mFd = 0;
    }
}

void Png::readInfo(const char * path)
{
    ASSERT(!mFd && !mpPng && !mpInfo && !mppRows);
    
    mFd = fopen(path, "rb");
    PANIC_IF(mFd == 0, "Unable to read png file: %s", path);

    mpPng = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    PANIC_IF(!mpPng, "png_create_read_struct failed: %s", path);

    mpInfo = png_create_info_struct(mpPng);
    PANIC_IF(!mpInfo, "png_create_info_struct failed: %s", path);
    
    
    if (setjmp(png_jmpbuf(mpPng)))
        PANIC("Unable to setup libpng error handlers: %s", path);

    png_init_io(mpPng, mFd);

    png_read_png(mpPng, mpInfo, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_get_IHDR(mpPng, mpInfo, &mWidth, &mHeight, &mBitDepth, &mColorType, &mInterlaceMethod, &mCompressionMethod, &mFilterMethod);
}

void Png::readData()
{
    ASSERT(mFd && mpPng && mpInfo && !mppRows);
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

u8 * Png::scanline(u32 idx)
{
    ASSERT(mppRows);
    return nullptr;
}

// Callers should GFREE pGimg
void Png::convertToGimg(Gimg ** pGimgOut)
{
    ASSERT(mppRows);
}

} // namespace gaen

