//------------------------------------------------------------------------------
// cookers.cpp - Cookers for various asset types
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#include "core/base_defines.h"
#include "core/thread_local.h"

#include "assets/Gimg.h"

#include "chef/Chef.h"
#include "chef/CookerRegistry.h"
#include "chef/Tga.h"
#include "chef/cookers.h"

namespace gaen
{

void cook_fnt(std::ifstream & ifs, std::ofstream & ofs, const CookInfo & ci)
{
    Config<kMEM_Chef> conf;
    conf.read(ifs);

}

void cook_tga(std::ifstream & ifs, std::ofstream & ofs, const CookInfo & ci)
{
    char header[sizeof(Tga)];

    ifs.read(header, sizeof(Tga));
    PANIC_IF(!ifs.good() || ifs.gcount() != sizeof(Tga), "Invalid .tga header: %s", ci.rawPath);

    Tga * pTga = reinterpret_cast<Tga*>(header);

    u32 size = pTga->totalSize();
    PANIC_IF(size > 100 * 1024 * 1024, "File too large for cooking: size: %u, path: %s", size, ci.rawPath); // sanity check

    // allocate a buffer
    Scoped_GFREE<char> pBuf((char*)GALLOC(kMEM_Chef, size));
    memcpy(pBuf.get(), header, sizeof(Tga));

    ifs.read(pBuf.get() + sizeof(Tga), size - sizeof(Tga));
    PANIC_IF(!ifs.good() || ifs.gcount() != size - sizeof(Tga), "Bad .tga file: %s", ci.rawPath);

    PANIC_IF(!pTga->is_valid((u8*)pBuf.get(), size), "Invalid .tga file: %s", ci.rawPath);
    pTga = reinterpret_cast<Tga*>(pBuf.get());

    // Convert to a Gimg with same-ish pixel format
    Gimg * pGimgTga;
    pTga->convertToGimg(&pGimgTga);
    Scoped_GFREE<Gimg> pGimg_sp(pGimgTga);

    PixelFormat pixFmt = pixel_format_from_str(ci.recipe.getWithDefault("pixel_format", "RGBA8"));

    // Convert the pixel format if necessary
    Gimg * pGimg;
    pGimgTga->convertFormat(&pGimg, kMEM_Chef, pixFmt);

    // write out file
    ofs.write((const char *)pGimg, pGimg->totalSize());
}

void register_cookers()
{
    CookerRegistry::register_cooker("fnt", "gfnt", cook_fnt);
    CookerRegistry::register_cooker("tga", "gimg", cook_tga);
}

} // namespace gaen
