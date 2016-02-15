//------------------------------------------------------------------------------
// Gimg.h - Runtime image format
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

#ifndef GAEN_ASSETS_GIMG_H
#define GAEN_ASSETS_GIMG_H

#include "core/base_defines.h"
#include "core/mem.h"

namespace gaen
{

// These correspond directly to OpenGL pixel formats
enum PixelFormat
{
    kPXL_R8        = 0x8229, // GL_R8
    kPXL_RGB8      = 0x8051, // GL_RGB8
    kPXL_RGBA8     = 0x8058, // GL_RGBA8
    kPXL_RGB_DXT1  = 0x8C4C, // GL_COMPRESSED_SRGB_S3TC_DXT1_EXT
    kPXL_RGBA_DXT1 = 0x8C4D, // GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT
    kPXL_RGBA_DXT3 = 0x8C4E, // GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT
    kPXL_RGBA_DXT5 = 0x8C4F  // GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT
};

const char * pixel_format_to_str(PixelFormat pixelType);
const PixelFormat pixel_format_from_str(const char * str);

inline u8 luminance(u8 r, u8 g, u8 b)
{
    return static_cast<u8>(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
}

struct Gimg
{
public:
    static bool is_valid(const u8 * pBuffer, u32 size);
    
    static u32 required_size(PixelFormat pixelFormat, u32 width, u32 height);

    static Gimg * create(MemType memType, PixelFormat pixelFormat, u32 width, u32 height);

    u32 totalSize() const;

    u8 * scanline(u32 idx);
    const u8 * scanline(u32 idx) const;

    void convertFormat(Gimg ** pGimg, MemType memType, PixelFormat newPixelFormat) const;

    PixelFormat pixelFormat;
    
    u16 width;
    u16 height;

private:
    // Class should not be constructed directly.  Use cast and create static methods.
    Gimg() = default;
    Gimg(const Gimg&) = delete;
    Gimg & operator=(const Gimg&) = delete;
};

static_assert(sizeof(Gimg) == 8, "Gimg expected to be 8 bytes");

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_GIMG_H
