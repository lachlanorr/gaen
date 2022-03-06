//------------------------------------------------------------------------------
// Gimg.h - Runtime image format
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#ifndef GAEN_ASSETS_GIMG_H
#define GAEN_ASSETS_GIMG_H

#include "gaen/core/base_defines.h"
#include "gaen/core/mem.h"

#include "gaen/assets/AssetHeader.h"
#include "gaen/assets/Color.h"

namespace gaen
{

// These correspond directly to OpenGL pixel formats
enum PixelFormat
{
    kPXL_Reference = 0x0000, // Not a real image, but asset path reference
    kPXL_R8        = 0x8229, // GL_R8
    kPXL_RGB8      = 0x8051, // GL_RGB8
    kPXL_RGBA8     = 0x8058, // GL_RGBA8
    kPXL_RGB_DXT1  = 0x8C4C, // GL_COMPRESSED_SRGB_S3TC_DXT1_EXT
    kPXL_RGBA_DXT1 = 0x8C4D, // GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT
    kPXL_RGBA_DXT3 = 0x8C4E, // GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT
    kPXL_RGBA_DXT5 = 0x8C4F, // GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT
    kPXL_RGB32F    = 0x8815, // GL_RGB32F
    kPXL_RGBA32F   = 0x8814  // GL_RGBA32F
};

const char * pixel_format_to_str(PixelFormat pixelFormat);
const PixelFormat pixel_format_from_str(const char * str);
u32 bytes_per_pixel(PixelFormat pixelFormat);

#pragma pack(push, 1)
class Gimg : public AssetHeader4CC<FOURCC("gimg")>
{
public:
    static bool is_valid(const void * pBuffer, u64 size);
    static Gimg * instance(void * pBuffer, u64 size);
    static const Gimg * instance(const void * pBuffer, u64 size);

    static u64 required_size(PixelFormat pixelFormat, u32 width, u32 height);

    static void init(Gimg * pGimg, PixelFormat pixelFormat, u32 width, u32 height, u32 referencePathHash);
    static Gimg * create(PixelFormat pixelFormat, u32 width, u32 height, u32 referencePathHash);
    static Gimg * create(u32 referencePathHash);

    u8 * pixels();
    const u8 * pixels() const;
    u8 * scanline(u32 idx);
    const u8 * scanline(u32 idx) const;

    void convertFormat(Gimg ** pGimg, MemType memType, PixelFormat newPixelFormat) const;

    void clear(Color col);

    u32 referencePathHash() const { return mReferencePathHash; }

    PixelFormat pixelFormat() const { return mPixelFormat; }
    u32 width() const { return mWidth; }
    u32 height() const { return mHeight; }

private:
    // Class should not be constructed directly.  Use cast and create static methods.
    Gimg() = default;
    Gimg(const Gimg&) = delete;
    Gimg & operator=(const Gimg&) = delete;

    PixelFormat mPixelFormat;

    u32 mWidth;
    u32 mHeight;

    u32 mReferencePathHash;
};
#pragma pack(pop)

static_assert(sizeof(Gimg) == 32, "Gimg unexpected size");
static_assert(sizeof(Gimg) % 16 == 0, "Gimg size not 16 byte aligned");

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_GIMG_H
