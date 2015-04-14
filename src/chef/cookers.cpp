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

#include "assets/file_utils.h"
#include "assets/Gimg.h"
#include "assets/Gatl.h"

#include "chef/Chef.h"
#include "chef/CookerRegistry.h"
#include "chef/Tga.h"
#include "chef/cooker_utils.h"
#include "chef/cookers.h"

namespace gaen
{

void cook_fnt(const CookInfo & ci)
{
    FileReader rdr(ci.rawPath);
    Config<kMEM_Chef> fnt;
    fnt.read(rdr.ifs);

    char minChar = 127;
    char maxChar = 0;

    static const char * kGlyphs = "glyphs";
    static const char * kFixedSize = "fixed_size";
    static const char * kImage = "image";
    static const char * kDefaultGlyph = "default_char";

    PANIC_IF(!fnt.hasKey(kImage), "Missing image in .fnt");
    PANIC_IF(!fnt.hasSection(kGlyphs), "Missing [glyphs] section in .fnt");

    // Crack open the image and get the width/height so we can calculate
    // proper u/v coords
    char imageRawPath[kMaxPath+1];
    ci.pChef->reportDependency(imageRawPath, ci.rawPath, fnt.get(kImage));
    ImageInfo imageInfo = read_image_info(imageRawPath);

    // Pull out the fixed width/height if present
    bool hasFixedSize = false;
    f32 fixedWidth = 0.0f;
    f32 fixedHeight = 0.0f;
    if (fnt.hasKey(kFixedSize))
    {
        auto fsv = fnt.getIntVec(kFixedSize);
        PANIC_IF(fsv.size() != 2, "%s must be a list of 2 integers", kFixedSize);
        fixedWidth = (f32)fsv.front();
        fixedHeight = (f32)fsv.back();
        hasFixedSize = true;
        PANIC_IF(fixedWidth < 1.0f || fixedHeight < 1.0f, "Invalid %s: %d,%d", kFixedSize, fixedWidth, fixedHeight);
    }

    // Iterate over keys once to find min/max character values
    auto keyIt = fnt.keysBegin(kGlyphs);
    auto keyItEnd = fnt.keysEnd(kGlyphs);
    while (keyIt != keyItEnd)
    {
        const char * key = *keyIt;
        PANIC_IF(strlen(key) != 1, "Multi character key in .fnt file: %s", key);
        char keyChar = key[0];
        PANIC_IF(keyChar < 0 || keyChar > 128, "Invalid character in .fnt file: %c", keyChar);
        minChar = minval(minChar, keyChar);
        maxChar = maxval(maxChar, keyChar);
        ++keyIt;
    }
    PANIC_IF(minChar > maxChar, "minChar > maxChar in .fnt file, something is horribly wrong");

    char imageGamePath[kMaxPath+1];
    ci.pChef->getGamePath(imageGamePath, imageRawPath);

    // Extract default glyph if it has been provided
    char defaultChar = minChar;
    if (fnt.hasKey(kDefaultGlyph))
    {
        const char * defCharStr = fnt.get(kDefaultGlyph);
        PANIC_IF(strlen(defCharStr) != 1, "Invalid %s: %s", kDefaultGlyph, defCharStr);
        PANIC_IF(!fnt.hasKey(kGlyphs, defCharStr), "Default character not defined");
        defaultChar = defCharStr[0];
        PANIC_IF(defaultChar < minChar || defaultChar > maxChar, "DefaultChar not found");
    }

    Gatl * pGatl = Gatl::create(kMEM_Chef, imageGamePath, minChar, maxChar, defaultChar);

    // Iterate again to set glyphs in our Gatl
    char cKey[2];
    cKey[1] = '\0';
    for (char c = minChar; c < maxChar; ++c)
    {
        GlyphCoords & coords = pGatl->glyphCoords(c);
        cKey[0] = c;

        if (!fnt.hasKey(kGlyphs, cKey))
        {
            cKey[0] = defaultChar;
        }

        auto cv = fnt.getIntVec(kGlyphs, cKey);
        PANIC_IF(cv.size() != 2 && cv.size() != 4, "Invalid glyph coordinates: %s=%s", cKey, fnt.get(kGlyphs, cKey));
        PANIC_IF(cv.size() == 2 && !hasFixedSize, "Glyph without size: %s=%s", cKey, fnt.get(kGlyphs, cKey));

        f32 glyphLeft = (f32)cv[0];
        f32 glyphTop = (f32)cv[1];

        f32 glyphWidth = fixedWidth;
        f32 glyphHeight = fixedHeight;
        if (cv.size() == 4)
        {
            glyphWidth = (f32)cv[2];
            glyphWidth = (f32)cv[3];
        }

        // convert to bottom left coords if source image is top left (which is almost always true)
        if (imageInfo.origin == kORIG_TopLeft)
        {
            glyphTop = imageInfo.height - glyphTop - 1;
        }

        // convert pixel coords to uv space
        coords.topLeftU = glyphLeft / (imageInfo.width - 1);
        coords.topLeftV = glyphTop / (imageInfo.height - 1);
        coords.bottomRightU = (glyphLeft + glyphWidth) / (imageInfo.width - 1);
        coords.bottomRightV = (glyphTop - glyphHeight) / (imageInfo.height - 1);
    }

    // write out file
    FileWriter wrtr(ci.cookedPath);
    wrtr.ofs.write((const char *)pGatl, pGatl->totalSize());
}

void cook_tga(const CookInfo & ci)
{
    FileReader rdr(ci.rawPath);
    Tga header;

    rdr.read(&header);
    PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != sizeof(Tga), "Invalid .tga header: %s", ci.rawPath);

    u32 size = header.totalSize();
    PANIC_IF(size > 100 * 1024 * 1024, "File too large for cooking: size: %u, path: %s", size, ci.rawPath); // sanity check

    // allocate a buffer
    Scoped_GFREE<char> pBuf((char*)GALLOC(kMEM_Chef, size));
    memcpy(pBuf.get(), &header, sizeof(Tga));

    rdr.ifs.read(pBuf.get() + sizeof(Tga), size - sizeof(Tga));
    PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != size - sizeof(Tga), "Bad .tga file: %s", ci.rawPath);

    PANIC_IF(!header.is_valid((u8*)pBuf.get(), size), "Invalid .tga file: %s", ci.rawPath);
    Tga * pTga = reinterpret_cast<Tga*>(pBuf.get());

    // Convert to a Gimg with same-ish pixel format
    Gimg * pGimgTga;
    pTga->convertToGimg(&pGimgTga);
    Scoped_GFREE<Gimg> pGimg_sp(pGimgTga);

    PixelFormat pixFmt = pixel_format_from_str(ci.recipe.getWithDefault("pixel_format", "RGBA8"));

    // Convert the pixel format if necessary
    Gimg * pGimg;
    pGimgTga->convertFormat(&pGimg, kMEM_Chef, pixFmt);

    // write out file
    FileWriter wrtr(ci.cookedPath);
    wrtr.ofs.write((const char *)pGimg, pGimg->totalSize());
}

void cook_passthrough(const CookInfo & ci)
{
    // copy raw to cooked without modifications
    FileReader rdr(ci.rawPath);

    Scoped_GFREE<char> pBuff((char*)GALLOC(kMEM_Chef, rdr.size()));
    rdr.ifs.read(pBuff.get(), rdr.size());
    PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != rdr.size(), "Read failure of: %s", ci.rawPath);

    FileWriter wrtr(ci.cookedPath);
    wrtr.ofs.write(pBuff.get(), rdr.size());
}

void register_cookers()
{
    CookerRegistry::register_cooker("atl", "gatl", cook_fnt);
    CookerRegistry::register_cooker("tga", "gimg", cook_tga);

    CookerRegistry::register_cooker("mat", "gmat", cook_passthrough);
    CookerRegistry::register_cooker("vtx", "gvtx", cook_passthrough);
    CookerRegistry::register_cooker("frg", "gfrg", cook_passthrough);
}

} // namespace gaen
