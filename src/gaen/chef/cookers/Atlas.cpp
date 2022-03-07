//------------------------------------------------------------------------------
// Atlas.cpp - Texture atlas cooker
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

#include "gaen/chef/stdafx.h"

#include "gaen/math/vec2.h"

#include "gaen/assets/file_utils.h"
#include "gaen/assets/Config.h"
#include "gaen/assets/Gatl.h"

#include "gaen/chef/cooker_utils.h"
#include "gaen/chef/CookInfo.h"
#include "gaen/chef/cookers/Atlas.h"


namespace gaen
{
namespace cookers
{

Atlas::Atlas()
{
    setVersion(2);
    addRawExt(kExtAtl);
    addCookedExtExclusive(kExtGatl);
}

struct GlyphQuad
{
    GlyphVert verts[4];
};

// support for HashMap
struct GlyphQuadHash
{
    u64 operator()(const GlyphQuad & val) const
    {
        const u8 * pBuff = reinterpret_cast<const u8*>(&val);
        return fnv1a_32(pBuff, sizeof(GlyphQuad));
    }
};
struct GlyphQuadEquals
{
    u64 operator()(const GlyphQuad & lhs, const GlyphQuad & rhs) const
    {
        return (lhs.verts[0].position == rhs.verts[0].position &&
                lhs.verts[0].u == rhs.verts[0].u &&
                lhs.verts[0].v == rhs.verts[0].v &&
                lhs.verts[1].position == rhs.verts[0].position &&
                lhs.verts[1].u == rhs.verts[0].u &&
                lhs.verts[1].v == rhs.verts[0].v &&
                lhs.verts[2].position == rhs.verts[0].position &&
                lhs.verts[2].u == rhs.verts[0].u &&
                lhs.verts[2].v == rhs.verts[0].v &&
                lhs.verts[3].position == rhs.verts[0].position &&
                lhs.verts[3].u == rhs.verts[0].u &&
                lhs.verts[3].v == rhs.verts[0].v);
    }
};

static GlyphQuad glyph_quad(i32 left, i32 top, i32 width, i32 height, const ImageInfo & imageInfo)
{
    PANIC_IF(left < 0 || top < 0 || left >= imageInfo.width || top >= imageInfo.height ||  width > imageInfo.width || height > imageInfo.height, "Invalid parameters for GlyphInfo");

    // convert to bottom left coords if source image is top left (which is almost always true)
    if (imageInfo.origin == kORIG_TopLeft)
    {
        top = imageInfo.height - top;
    }

    // convert pixel coords to uv space
    GlyphQuad quad;

    // Top Left
    quad.verts[0].position.x = -width / 2.0f;
    quad.verts[0].position.y = height / 2.0f;
    quad.verts[0].u = (f32)left / (imageInfo.width);
    quad.verts[0].v = (f32)top / (imageInfo.height);

    // Bottom Left
    quad.verts[1].position.x = -width / 2.0f;
    quad.verts[1].position.y = -height / 2.0f;
    quad.verts[1].u = (f32)left / (imageInfo.width);
    quad.verts[1].v = (f32)(top - height) / (imageInfo.height);

    // Bottom Right
    quad.verts[2].position.x = width / 2.0f;
    quad.verts[2].position.y = -height / 2.0f;
    quad.verts[2].u = (f32)(left + width) / (imageInfo.width);
    quad.verts[2].v = (f32)(top - height) / (imageInfo.height);

    // Top Right
    quad.verts[3].position.x = width / 2.0f;
    quad.verts[3].position.y = height / 2.0f;
    quad.verts[3].u = (f32)(left + width) / (imageInfo.width);
    quad.verts[3].v = (f32)top / (imageInfo.height);

    return quad;
}

static bool parse_numeric(u32 * pVal, const char * str)
{
    char * end = nullptr;
    u32 val = strtoul(str, &end, 0);
    if (end && end - str == strlen(str))
    {
        *pVal = val;
        return true;
    }
    return false;
}

void Atlas::cook(CookInfo * pCookInfo) const
{
    Config<kMEM_Chef> atl;
    {
        FileReader rdr(pCookInfo->rawPath().c_str());
        PANIC_IF(!rdr.isOk(), "Unable to load file: %s", pCookInfo->rawPath().c_str());
        atl.read(rdr.ifs);
    }

    static const char * kGlyphs = "glyphs";
    static const char * kFixedSize = "fixed_size";
    static const char * kImage = "image";
    static const char * kRenderOffset = "render_offset";

    PANIC_IF(!atl.hasKey(kImage), "Missing image in .atl");

    const DependencyInfo & depInfo = pCookInfo->recordDependency(atl.get(kImage));

    // Grab some info out of the source image, which will be needed
    // when considering coordinates.
    ImageInfo imageInfo = read_image_info(depInfo.rawPath.c_str());
    PANIC_IF(imageInfo.height != imageInfo.width || !is_power_of_two(imageInfo.height), "Image not square or not power of 2 sized: %s", depInfo.rawPath.c_str());

    // Pull out render_offset if present, else use (0.0f, 0.0f)
    vec2 renderOffset{0.0f, 0.0f};
    if (atl.hasKey(kRenderOffset))
    {
        Config<kMEM_Chef>::FloatVec rov = atl.getFloatVec(kRenderOffset);
        PANIC_IF(rov.size() != 2, "%s must be a list of 2 integers", kRenderOffset);
        renderOffset.x = rov[0];
        renderOffset.y = rov[1];
    }

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

    List<kMEM_Chef, GlyphVert> vertsList;
    HashSet<kMEM_Chef, GlyphQuad, GlyphQuadHash, GlyphQuadEquals> quadsSet;

    List<kMEM_Chef, GlyphTri> trisList;

    List<kMEM_Chef, GlyphAlias> aliasesList;
    HashSet<kMEM_Chef, u32> aliasesSet;
    u16 defaultIndex = 0;

    if (atl.hasSection(kGlyphs))
    {
        // Iterate over keys once to find min/max character values
        auto keyIt = atl.keysBegin(kGlyphs);
        auto keyItEnd = atl.keysEnd(kGlyphs);
        while (keyIt != keyItEnd)
        {
            const char * key = *keyIt;
            PANIC_IF(!key || !key[0], "Empyty glyph key(%s) in atl file: %s", pCookInfo->rawPath().c_str(), key);

            GlyphAlias alias;

            u32 numeric = 0;
            if (parse_numeric(&numeric, key))
            {
                // treat key as a literal value (i.e. a font character)
                alias.hash = numeric;
            }
            else
            {
                // hash the key
                alias.hash = gaen_hash(key);
            }

            PANIC_IF(aliasesSet.find(alias.hash) != aliasesSet.end(), "Alias(%u) multiply defined in atl file: %s", alias.hash, pCookInfo->rawPath().c_str());

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

            GlyphQuad quad = glyph_quad(cv[0], cv[1], width, height, imageInfo);

            PANIC_IF(quadsSet.find(quad) != quadsSet.end(), "Coords multiply defined within atl file: %s", pCookInfo->rawPath().c_str());

            alias.index = (u32)quadsSet.size();
            if (alias.hash == gaen_hash("default"))
                defaultIndex = alias.index;


            quadsSet.insert(quad);
            u16 elemIdx = (u16)vertsList.size();
            trisList.push_back(GlyphTri(elemIdx,
                                        elemIdx + 1,
                                        elemIdx + 2));
            trisList.push_back(GlyphTri(elemIdx + 2,
                                        elemIdx + 3,
                                        elemIdx));

            vertsList.push_back(quad.verts[0]);
            vertsList.push_back(quad.verts[1]);
            vertsList.push_back(quad.verts[2]);
            vertsList.push_back(quad.verts[3]);

            aliasesSet.insert(alias.hash);
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
                GlyphQuad quad = glyph_quad(currX, currY, fixedWidth, fixedHeight, imageInfo);

                quadsSet.insert(quad);
                u16 elemIdx = (u16)vertsList.size();
                trisList.push_back(GlyphTri(elemIdx,
                                            elemIdx + 1,
                                            elemIdx + 2));
                trisList.push_back(GlyphTri(elemIdx + 2,
                                            elemIdx + 3,
                                            elemIdx));

                vertsList.push_back(quad.verts[0]);
                vertsList.push_back(quad.verts[1]);
                vertsList.push_back(quad.verts[2]);
                vertsList.push_back(quad.verts[3]);
            }
        }
    }
    else
    {
        PANIC("No glyphs specified, and invalid fixed_size: %s", pCookInfo->rawPath().c_str());
    }

    ASSERT(vertsList.size() % 4 == 0);
    Gatl * pGatl = Gatl::create(depInfo.gamePaths.front().c_str(), (u16)vertsList.size() / 4, (u16)aliasesList.size(), defaultIndex, renderOffset);
    ASSERT(pGatl);

    // Place verts into Gatl buffer
    u32 idx = 0;
    for (const auto & vert : vertsList)
        pGatl->vert(idx++) = vert;

    // Place tris into Gatl buffer
    idx = 0;
    for (const auto & tri : trisList)
        pGatl->tri(idx++) = tri;

    // Place aliases into Gatl buffer
    idx = 0;
    for (const auto & alias : aliasesList)
        pGatl->alias(idx++) = alias;

    pCookInfo->setCookedBuffer(pGatl);
}

}
} // namespace gaen

