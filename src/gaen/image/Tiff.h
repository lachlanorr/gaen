//------------------------------------------------------------------------------
// Tiff.h - Tiff image processing
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

#ifndef GAEN_IMAGE_TIFF_H
#define GAEN_IMAGE_TIFF_H

#include "gaen/core/mem.h"
#include "gaen/core/String.h"
#include "gaen/core/Vector.h"
#include "gaen/image/ImageInfo.h"

struct tiff;
class Gimg;

namespace gaen
{
class Tiff
{
public:
    struct LayerInfo
    {
        u16 top;
        u16 left;
        u16 bottom;
        u16 right;

        u16 width() const { return right - left; }
        u16 height() const { return bottom - top; }

        ChefString name;
    };

    static UniquePtr<Tiff> read(const char * path);
    static ImageInfo read_image_info(const char * path);

    ~Tiff();

    u32 origin() { return mOrigin; }
    u32 width() { return mWidth; }
    u32 height() { return mHeight; }

    u8 * scanline(u32 idx);

    const Vector<kMEM_Chef, LayerInfo> & layers();
    UniquePtrFr<Color> extractLayerPixels(const LayerInfo & li);

private:
    Tiff();
    Tiff(const Tiff&)              = delete;
    Tiff(Tiff&&)                   = delete;
    Tiff & operator=(const Tiff&)  = delete;
    Tiff & operator=(Tiff&&)       = delete;

    void init(const char * path);

    ChefString mPath;
    tiff * mpTif;

    UniquePtr<Color> mpPixels;

    ImageOrigin mOrigin;
    u32 mWidth;
    u32 mHeight;

    Vector<kMEM_Chef, LayerInfo> mLayers;
};

} // namespace gaen

#endif // #ifndef GAEN_IMAGE_TIFF_H
