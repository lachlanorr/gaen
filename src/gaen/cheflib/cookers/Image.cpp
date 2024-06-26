//------------------------------------------------------------------------------
// Image.cpp - Image cooker
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

#include "gaen/core/hashing.h"

#include "gaen/assets/file_utils.h"
#include "gaen/assets/Config.h"
#include "gaen/assets/Gimg.h"

#include "gaen/image/Png.h"
#include "gaen/image/Tga.h"

#include "gaen/cheflib/CookInfo.h"
#include "gaen/cheflib/Chef.h"
#include "gaen/cheflib/cookers/Image.h"

namespace gaen
{
namespace cookers
{

Image::Image()
{
    setVersion(1);
    addRawExt(kExtPng);
    addRawExt(kExtTga);
    addCookedExtExclusive(kExtGimg);
}

void Image::cook(CookInfo * pCookInfo) const
{
    const char * ext = get_ext(pCookInfo->rawPath().c_str());

    if (0 == strcmp(ext, kExtPng))
    {
        cookPng(pCookInfo);
    }
    else if (0 == strcmp(ext, kExtTga))
    {
        cookTga(pCookInfo);
    }
    else
    {
        PANIC("Unable to cook image: %s", pCookInfo->rawPath().c_str());
    }
}

Gimg * Image::load_png(const char * path, u32 referencePathHash, PixelFormat pixFmt)
{
    UniquePtr<Png> pPng = Png::read(path);
    PANIC_IF(!pPng, "Failed to read png: %s", path);

    // Convert to a Gimg with same-ish pixel format
    Gimg * pGimgPng;
    pPng->convertToGimg(&pGimgPng, referencePathHash, true);
    Scoped_GFREE<Gimg> pGimg_sp(pGimgPng);

    // Convert the pixel format if necessary
    Gimg * pGimg;
    pGimgPng->convertFormat(&pGimg, kMEM_Chef, pixFmt);

    ASSERT(pGimg);
    return pGimg;
}

void Image::cookPng(CookInfo * pCookInfo) const
{
    PixelFormat pixFmt = pixel_format_from_str(pCookInfo->fullRecipe().getWithDefault("pixel_format", "RGBA8"));
    Gimg * pGimg = Image::load_png(pCookInfo->rawPath().c_str(), Image::reference_path_hash(pCookInfo), pixFmt);

    ASSERT(pGimg);
    pCookInfo->setCookedBuffer(pGimg);
}

void Image::cookTga(CookInfo * pCookInfo) const
{
    FileReader rdr(pCookInfo->rawPath().c_str());
    PANIC_IF(!rdr.isOk(), "Unable to load file: %s", pCookInfo->rawPath().c_str());
    Tga header;

    rdr.read(&header);
    PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != sizeof(Tga), "Invalid .tga header: %s", pCookInfo->rawPath().c_str());

    u32 size = header.totalSize();
    PANIC_IF(size > 100 * 1024 * 1024, "File too large for cooking: size: %u, path: %s", size, pCookInfo->rawPath().c_str()); // sanity check

    // allocate a buffer
    Scoped_GFREE<char> pBuf((char*)GALLOC(kMEM_Chef, size));
    memcpy(pBuf.get(), &header, sizeof(Tga));

    rdr.ifs.read(pBuf.get() + sizeof(Tga), size - sizeof(Tga));
    PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != size - sizeof(Tga), "Bad .tga file: %s", pCookInfo->rawPath().c_str());

    PANIC_IF(!header.is_valid((u8*)pBuf.get(), size), "Invalid .tga file: %s", pCookInfo->rawPath().c_str());
    Tga * pTga = reinterpret_cast<Tga*>(pBuf.get());

    // Convert to a Gimg with same-ish pixel format
    Gimg * pGimgTga;
    pTga->convertToGimg(&pGimgTga, gaen_hash(pCookInfo->rawPath().c_str()));
    Scoped_GFREE<Gimg> pGimg_sp(pGimgTga);

    PixelFormat pixFmt = pixel_format_from_str(pCookInfo->fullRecipe().getWithDefault("pixel_format", "RGBA8"));

    // Convert the pixel format if necessary
    Gimg * pGimg;
    pGimgTga->convertFormat(&pGimg, kMEM_Chef, pixFmt);

    ASSERT(pGimg);
    pCookInfo->setCookedBuffer(pGimg);
}

u32 Image::reference_path_hash(const Chef & chef, const ChefString & rawPath)
{
    static ChefString ext(kExtGimg);
    ChefString gamePath = chef.getGamePath(rawPath, ext);
    return gaen_hash(gamePath.c_str());
}

u32 Image::reference_path_hash(const CookInfo *pCookInfo)
{
    return reference_path_hash(pCookInfo->chef(), pCookInfo->rawPath());
}


}
} // namespace gaen


