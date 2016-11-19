//------------------------------------------------------------------------------
// Gmdl.cpp - Runtime model format
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

#include <glm/common.hpp>

#include "core/HashMap.h"
#include "core/String.h"

#include "assets/file_utils.h"
#include "assets/Gmdl.h"

namespace gaen
{

bool Gmdl::is_valid(const void * pBuffer, u64 size)
{
    if (size < sizeof(Gmdl))
        return false;

    const Gmdl * pAssetData = reinterpret_cast<const Gmdl*>(pBuffer);

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

Gmdl * Gmdl::instance(void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gmdl buffer");
        return nullptr;
    }

    return reinterpret_cast<Gmdl*>(pBuffer);
}

const Gmdl * Gmdl::instance(const void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gmdl buffer");
        return nullptr;
    }

    return reinterpret_cast<const Gmdl*>(pBuffer);
}

u64 Gmdl::required_size(PixelFormat pixelFormat, u32 width, u32 height)
{
    u64 size = sizeof(Gmdl);
    
    switch (pixelFormat)
    {
    case kPXL_R8:
        size += width * height;
        break;
    case kPXL_RGB8:
        size += width * height * 3;
        break;
    case kPXL_RGBA8:
        size += width * height * 4;
        break;
    case kPXL_RGB_DXT1:
    case kPXL_RGBA_DXT1:
        PANIC_IF(width % 4 != 0 || height % 4 != 0, "DXT1 texture with width or height not a multiple of 4");
        size += width * height / 2; // every block of 16 pixels becomes 8 bytes
        break;
    case kPXL_RGBA_DXT3:
    case kPXL_RGBA_DXT5:
        PANIC_IF(width % 4 != 0 || height % 4 != 0, "DXT texture with width or height not a multiple of 4");
        size += width * height; // every block of 16 pixels becomes 16 bytes
        break;
    }

    return size;
}

Gmdl * Gmdl::create(PixelFormat pixelFormat, u32 width, u32 height)
{
    // adjust width and height to ensure power of 2 and equal size
    width = height = next_power_of_two(glm::max(width, height));

    u64 size = Gmdl::required_size(pixelFormat, width, height);
    Gmdl * pGmdl = alloc_asset<Gmdl>(kMEM_Texture, size);

    pGmdl->mPixelFormat = pixelFormat;
    pGmdl->mWidth = width;
    pGmdl->mHeight = height;

    ASSERT(is_valid(pGmdl, required_size(pixelFormat, width, height)));

    return pGmdl;
}

u64 Gmdl::size() const
{
    return required_size(mPixelFormat, mWidth, mHeight);
}


} // namespace gaen

