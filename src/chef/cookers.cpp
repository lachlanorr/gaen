//------------------------------------------------------------------------------
// cookers.cpp - Cookers for various asset types
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

#include <glm/common.hpp>

#include "core/base_defines.h"
#include "core/thread_local.h"
#include "core/hashing.h"

#include "assets/file_utils.h"
#include "assets/Gimg.h"
#include "assets/Gatl.h"
#include "assets/Gspr.h"

#include "chef/Chef.h"
#include "chef/CookerRegistry.h"
#include "chef/Tga.h"
#include "chef/cooker_utils.h"
#include "chef/cookers.h"

namespace gaen
{

static GlyphCoords glyph_coords(i32 left, i32 top, i32 width, i32 height, const ImageInfo & imageInfo)
{
    PANIC_IF(left < 0 || top < 0 || left >= imageInfo.width || top >= imageInfo.height ||  width > imageInfo.width || height > imageInfo.height, "Invalid parameters for GlyphInfo");

    // convert to bottom left coords if source image is top left (which is almost always true)
    if (imageInfo.origin == kORIG_TopLeft)
    {
        top = imageInfo.height - top - 1;
    }

    // convert pixel coords to uv space
    GlyphCoords coords;
    coords.topLeftU = (f32)left / (imageInfo.width - 1);
    coords.topLeftV = (f32)top / (imageInfo.height - 1);
    coords.bottomRightU = (f32)(left + width) / (imageInfo.width - 1);
    coords.bottomRightV = (f32)(top - height) / (imageInfo.height - 1);

    return coords;
}

void cook_spr(const CookInfo & ci)
{
    Config<kMEM_Chef> spr;
    {
        FileReader rdr(ci.rawPath());
        PANIC_IF(!rdr.isOk(), "Unable to load file: %s", ci.rawPath());
        spr.read(rdr.ifs);
    }

    static const char * kAtlas = "atlas";
    static const char * kFrameSize = "size";
    static const char * kFrames = "frames";

    PANIC_IF(!spr.hasKey(kAtlas), "Missing atlas in .spr: %s", ci.rawPath());
    PANIC_IF(!spr.hasKey(kFrameSize), "Missing frame_size in .spr: %s", ci.rawPath());

    u32 animCount = 0;
    u32 totalFrameCount = 0;

    // Iterate over sections to discover how many animations and total
    // frames there are.
    for (auto secIt = spr.sectionsBegin();
         secIt != spr.sectionsEnd();
         secIt++)
    {
        if (!spr.isGlobalSection(*secIt) && spr.hasKey(*secIt, kFrames))
        {
            Config<kMEM_Chef>::IntVec frameVec = spr.getIntVec(*secIt, kFrames);
            PANIC_IF(frameVec.size() == 0, "No frames specified in .spr animation: %s, %s", *secIt, ci.rawPath());
            animCount++;
            totalFrameCount += (u32)frameVec.size();
        }
    }

    PANIC_IF(animCount == 0, "No animations in .spr: %s", ci.rawPath());
    PANIC_IF(totalFrameCount == 0, "No frames specified in .spr: %s", ci.rawPath());

    Config<kMEM_Chef>::IntVec frameSizeVec = spr.getIntVec(kFrameSize);
    PANIC_IF(frameSizeVec.size() != 2, "frame_size doesn't contain 2 ints in .spr: %s", ci.rawPath());

    const char * atlasPathSpr = spr.get(kAtlas);
    char atlasPathGame[kMaxPath+1];
    ci.recordDependency(atlasPathGame, atlasPathSpr);

    Gspr * pGspr = Gspr::create(frameSizeVec[0],
                                frameSizeVec[1],
                                atlasPathGame,
                                animCount,
                                totalFrameCount);
    ASSERT(pGspr);

    // Iterate over sections again to insert animation frame data into
    // the gspr.
    u32 currFrame = 0;
    AnimInfo * pAnimInfo = pGspr->anims();
    u32 * pFrames = pGspr->frames();
    for (auto secIt = spr.sectionsBegin();
         secIt != spr.sectionsEnd();
         secIt++)
    {
        if (!spr.isGlobalSection(*secIt) && spr.hasKey(*secIt, kFrames))
        {
            Config<kMEM_Chef>::IntVec frameVec = spr.getIntVec(*secIt, kFrames);
            PANIC_IF(frameVec.size() == 0, "No frames specified in .spr animation: %s, %s", ci.rawPath(), *secIt);

            pAnimInfo->animHash = gaen_hash(*secIt);
            pAnimInfo->frameCount = (u32)frameVec.size();
            pAnimInfo->firstFrame = currFrame;

            for (i32 i : frameVec)
            {
                *pFrames++ = i;
            }

            currFrame += pAnimInfo->frameCount;
            pAnimInfo++;
        }
    }

    ASSERT(Gspr::is_valid(pGspr, pGspr->size()));
    ci.setCookedBuffer(pGspr, pGspr->size());
}

void cook_atl(const CookInfo & ci)
{
    Config<kMEM_Chef> atl;
    {
        FileReader rdr(ci.rawPath());
        PANIC_IF(!rdr.isOk(), "Unable to load file: %s", ci.rawPath());
        atl.read(rdr.ifs);
    }

    static const char * kGlyphs = "glyphs";
    static const char * kFixedSize = "fixed_size";
    static const char * kImage = "image";

    PANIC_IF(!atl.hasKey(kImage), "Missing image in .atl");

    // Cook our dependent image
    UniquePtr<CookInfo> pCiImage = ci.cookDependency(atl.get(kImage));
    PANIC_IF(!pCiImage.get() || !pCiImage->isCooked(), "Failed to cook dependent file: %s", atl.get(kImage));
    const Gimg * pImage = Gimg::instance(pCiImage->cookedBuffer(), pCiImage->cookedBufferSize());

    // Grab some info out of the source image, which will be needed
    // when considering coordinates.
    ImageInfo imageInfo = read_image_info(pCiImage->rawPath());
    ImageInfo imageInfoSized(imageInfo.origin, pImage->width(), pImage->height());

    // Pull out the fixed width/height if present
    bool hasFixedSize = false;
    i32 fixedWidth = 0;
    i32 fixedHeight = 0;
    if (atl.hasKey(kFixedSize))
    {
        auto fsv = atl.getIntVec(kFixedSize);
        PANIC_IF(fsv.size() != 2, "%s must be a list of 2 integers", kFixedSize);
        fixedWidth = fsv.front();
        fixedHeight = fsv.back();
        hasFixedSize = true;
        PANIC_IF(fixedWidth > imageInfo.width || fixedHeight > imageInfo.height, "Invalid %s: %d,%d", kFixedSize, fixedWidth, fixedHeight);
    }

    List<kMEM_Chef, GlyphCoords> coordsList;
    HashSet<kMEM_Chef, GlyphCoords, GlyphCoordsHash, GlyphCoordsEquals> coordsSet;
    List<kMEM_Chef, GlyphAlias> aliasesList;
    HashSet<kMEM_Chef, u32> aliasesSet;
    u32 defaultIndex = 0;

    if (atl.hasSection(kGlyphs))
    {
        // Iterate over keys once to find min/max character values
        auto keyIt = atl.keysBegin(kGlyphs);
        auto keyItEnd = atl.keysEnd(kGlyphs);
        while (keyIt != keyItEnd)
        {
            const char * key = *keyIt;
            PANIC_IF(!key || !key[0], "Empyty glyph key(%s) in atl file: %s", ci.rawPath(), key);

            GlyphAlias alias;

            if (key[1] == '\0')
            {
                // treat key as a literal value (i.e. a font character)
                alias.hash = key[0];
            }
            else
            {
                // hash the key
                alias.hash = gaen_hash(key);
            }

            PANIC_IF(aliasesSet.find(alias.hash) != aliasesSet.end(), "Alias(%u) multiply defined in atl file: %s", alias.hash, ci.rawPath());

            auto cv = atl.getIntVec(kGlyphs, key);
            PANIC_IF(cv.size() != 2 && cv.size() != 4, "Invalid glyph coordinates: %s=%s", key, atl.get(kGlyphs, key));
            PANIC_IF(cv.size() == 2 && !hasFixedSize, "Glyph without size: %s=%s", key, atl.get(kGlyphs, key));

            i32 width = fixedWidth;
            i32 height = fixedHeight;
            if (cv.size() == 4)
            {
                width = cv[2];
                height = cv[3];
            }

            GlyphCoords coords = glyph_coords(cv[0], cv[1], width, height, imageInfoSized);

            PANIC_IF(coordsSet.find(coords) != coordsSet.end(), "Coords multiply defined within atl file: %s", ci.rawPath());

            alias.index = (u32)coordsList.size();
            if (alias.hash == gaen_hash("default"))
                defaultIndex = alias.index;

            aliasesSet.insert(alias.hash);
            coordsSet.insert(coords);
            coordsList.push_back(coords);
            aliasesList.push_back(alias);

            ++keyIt;
        }
    }
    else if (hasFixedSize &&
             (imageInfo.width % fixedWidth == 0) &&
             (imageInfo.height % fixedHeight == 0))
    {
        // No explicit glyphs, so see if we can infer them based on sizes
        i32 currX = 0;
        i32 currY = 0;

        for (i32 currY = 0; currY < imageInfo.height; currY+=fixedHeight)
        {
            for (i32 currX = 0; currX < imageInfo.width; currX+=fixedWidth)
            {
                GlyphCoords coords = glyph_coords(currX, currY, fixedWidth, fixedHeight, imageInfoSized);
                coordsList.push_back(coords);
            }
        }
    }
    else
    {
        PANIC("No glyphs specified, and invalid fixed_size: %s", ci.rawPath());
    }

    Gatl * pGatl = Gatl::create((u32)coordsList.size(), (u32)aliasesList.size(), defaultIndex, *pImage);
    ASSERT(pGatl);

    // Place coords into Gatl buffer
    u32 idx = 0;
    for (const auto & coord : coordsList)
        pGatl->coords(idx++) = coord;

    // Place aliases into Gatl buffer
    idx = 0;
    for (const auto & alias : aliasesList)
        pGatl->alias(idx++) = alias;

    ci.setCookedBuffer(pGatl, pGatl->size());
}

void cook_tga(const CookInfo & ci)
{
    FileReader rdr(ci.rawPath());
    PANIC_IF(!rdr.isOk(), "Unable to load file: %s", ci.rawPath());
    Tga header;

    rdr.read(&header);
    PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != sizeof(Tga), "Invalid .tga header: %s", ci.rawPath());

    u32 size = header.totalSize();
    PANIC_IF(size > 100 * 1024 * 1024, "File too large for cooking: size: %u, path: %s", size, ci.rawPath()); // sanity check

    // allocate a buffer
    Scoped_GFREE<char> pBuf((char*)GALLOC(kMEM_Chef, size));
    memcpy(pBuf.get(), &header, sizeof(Tga));

    rdr.ifs.read(pBuf.get() + sizeof(Tga), size - sizeof(Tga));
    PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != size - sizeof(Tga), "Bad .tga file: %s", ci.rawPath());

    PANIC_IF(!header.is_valid((u8*)pBuf.get(), size), "Invalid .tga file: %s", ci.rawPath());
    Tga * pTga = reinterpret_cast<Tga*>(pBuf.get());

    // Convert to a Gimg with same-ish pixel format
    Gimg * pGimgTga;
    pTga->convertToGimg(&pGimgTga);
    Scoped_GFREE<Gimg> pGimg_sp(pGimgTga);

    PixelFormat pixFmt = pixel_format_from_str(ci.recipe().getWithDefault("pixel_format", "RGBA8"));

    // Convert the pixel format if necessary
    Gimg * pGimg;
    pGimgTga->convertFormat(&pGimg, kMEM_Chef, pixFmt);

    ASSERT(pGimg);
    ci.setCookedBuffer(pGimg, pGimg->size());
}

void cook_passthrough(const CookInfo & ci)
{
    // copy raw to cooked without modifications
    FileReader rdr(ci.rawPath());
    PANIC_IF(!rdr.isOk(), "Unable to load file: %s", ci.rawPath());

    char * pBuf = (char*)GALLOC(kMEM_Chef, rdr.size());

    rdr.ifs.read(pBuf, rdr.size());
    PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != rdr.size(), "Read failure of: %s", ci.rawPath());

    ci.setCookedBuffer(pBuf, rdr.size());
}

void register_cookers()
{
    CookerRegistry::register_cooker("atl", "gatl", cook_atl);
    CookerRegistry::register_cooker("spr", "gspr", cook_spr);
    CookerRegistry::register_cooker("tga", "gimg", cook_tga);

    CookerRegistry::register_cooker("mat", "gmat", cook_passthrough);
    CookerRegistry::register_cooker("vtx", "gvtx", cook_passthrough);
    CookerRegistry::register_cooker("frg", "gfrg", cook_passthrough);

    register_project_cookers();
}

#ifdef IS_GAEN_PROJECT
void register_project_cookers()
{
    // If we're compiling gaen as a project, no custom asset cookers.
}
#endif

} // namespace gaen
