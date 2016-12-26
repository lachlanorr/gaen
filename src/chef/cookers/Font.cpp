//------------------------------------------------------------------------------
// Font.cpp - Font cooker
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

#include "core/thread_local.h"

#include "math/vec2.h"

#include "assets/file_utils.h"
#include "assets/Config.h"
#include "assets/Gatl.h"
#include "assets/Gimg.h"

#include "chef/cooker_utils.h"
#include "chef/CookInfo.h"
#include "chef/Chef.h"
#include "chef/Png.h"
#include "chef/cookers/Atlas.h"
#include "chef/cookers/Image.h"
#include "chef/cookers/Font.h"


namespace gaen
{
namespace cookers
{

Font::Font()
{
    setVersion(1);
    addRawExt(kExtFnt);
    addCookedExt(kExtGatl);
    addCookedExt(kExtGimg);
    addCookedExtExclusive(kExtGfnt);
}

class FontAtl
{
public:
    FontAtl(const char * image, vec2 renderOffset)
      : mImage(image)
      , mRenderOffset(renderOffset)
    {}

    void addGlyph(const char * key, const uvec4 & coords)
    {
        mGlyphs.emplace_back(key, coords);
    }

    void addGlyph(const u32 key, const uvec4 & coords)
    {
        char scratch[16];
        sprintf(scratch, "0x%08x", key);
        addGlyph(scratch, coords);
    }

    void write(const char * path)
    {
        FileWriter fw(path);

        fw.ofs << "image = " << mImage << std::endl;
        fw.ofs << std::endl;

        fw.ofs << "render_offset = " << mRenderOffset.x << "," << mRenderOffset.y << std::endl;
        fw.ofs << std::endl;

        fw.ofs << "[glyphs]" << std::endl;
        for (Glyph & g : mGlyphs)
        {
            fw.ofs << g.key << " = " << g.coords.x << "," << g.coords.y << "," << g.coords.z << "," << g.coords.w << std::endl;
        }
    }

private:
    struct Glyph
    {
        String<kMEM_Chef> key;
        uvec4 coords;

        Glyph(const char * key, const uvec4 & coords)
          : key(key)
          , coords(coords)
        {}
    };
    
    String<kMEM_Chef> mImage;
    vec2 mRenderOffset;
    List<kMEM_Chef, Glyph> mGlyphs;
};

TL(FT_Library*, tpFtLibrary) = nullptr;
static FT_Library & ft_library()
{
    if (tpFtLibrary == nullptr)
    {
        tpFtLibrary = (FT_Library*)GALLOC(kMEM_Chef, sizeof(FT_Library));
        memset(tpFtLibrary, 0, sizeof(FT_Library));
        FT_Error err = FT_Init_FreeType(tpFtLibrary);
        PANIC_IF(err != 0, "FT_Init_FreeType failure: %d", err);
    }
    return *tpFtLibrary;
}

static u32 utf8_to_utf32(const char * from)
{
    const char * fromEnd = from + strlen(from);
    const char * fromNext;

    u32 out;
    u32 * to = &out;
    u32 * toEnd = to + sizeof(u32);
    u32 * toNext;

    auto &f = std::codecvt_utf8<u32>();
    std::mbstate_t mb = std::mbstate_t();

    auto res = f.in(mb, from, fromEnd, fromNext, to, toEnd, toNext);
    if (res == std::codecvt_base::ok)
    {
        return out;
    }
    else
    {
        PANIC("Failed to parse utf8 character: '%s', res: %d", from, res);
        return 0;
    }
}

void Font::cook(CookInfo * pCookInfo) const
{
    Config<kMEM_Chef> fnt;
    {
        FileReader rdr(pCookInfo->rawPath().c_str());
        PANIC_IF(!rdr.isOk(), "Unable to load file: %s", pCookInfo->rawPath().c_str());
        fnt.read(rdr.ifs);
    }

    static const char * kFont = "font";
    static const char * kSize = "size";
    static const char * kCharcodes = "charcodes";

    PANIC_IF(!fnt.hasKey(kFont), "Missing 'font' in .fnt");
    PANIC_IF(!fnt.hasKey(kSize), "Missing 'size' in .fnt");
    PANIC_IF(!fnt.hasSection(kCharcodes), "Missing [charcodes] section in .fnt");

    ChefString sourcePath = fnt.get(kFont);
    u32 size = (u32)fnt.getInt(kSize);
    PANIC_IF(size < 10 || size > 100, "Invalid size in .fnt: %d", size);

    Vector<kMEM_Chef, u32> utf32Codes;
    utf32Codes.reserve(fnt.sectionKeyCount(kCharcodes));

    // Parse all characters, accounting for utf8 encoded chars
    auto codesEnd = fnt.keysEnd(kCharcodes);
    for (auto it = fnt.keysBegin(kCharcodes);
         it != codesEnd;
         ++it)
    {
        utf32Codes.push_back(utf8_to_utf32(*it));
    }

    const DependencyInfo & sourceInfo = pCookInfo->recordDependency(sourcePath);

    // Load font
    FT_Error err;
    FT_Face face;

    err = FT_New_Face(ft_library(), sourceInfo.rawPath.c_str(), 0, &face);
    PANIC_IF(err != 0, "Unable to FT_New_Face: %s, err: %d", sourceInfo.rawPath.c_str(), err);

    // Set the pixel size... we don't set point sizes because then we deal with DPI.
    // For game fonts, it's easier to think of pixel sizes.
    err = FT_Set_Pixel_Sizes(face, 0, size);
    PANIC_IF(err != 0, "Unable to FT_Set_Pixel_Sizes: %s, size: %d, err: %d", sourceInfo.rawPath.c_str(), size, err);

    ASSERT(face->size->metrics.height % 64 == 0);
    u32 glyphHeight = size;
    i32 glyphYOffset = (u32)(-face->bbox.yMin / 64.0f + 0.5);
    vec2 glyphRenderOffset{0.0f, (f32)-glyphYOffset};

    // Create an image large enough to hold all glyphs
    u32 glyphsPerRow = (u32)(sqrt((f32)utf32Codes.size()) + 0.5f);
    u32 imgHeight = next_power_of_two(glyphsPerRow * glyphHeight);
    Scoped_GFREE<Gimg> pGimg = Gimg::create(kPXL_R8, imgHeight, imgHeight);

    ChefString pngTransPath = pCookInfo->chef().getRawTransPath(pCookInfo->rawPath(), kExtPng);
    ChefString atlTransPath = pCookInfo->chef().getRawTransPath(pCookInfo->rawPath(), kExtAtl);

    FontAtl fontAtl(get_filename(pngTransPath.c_str()), glyphRenderOffset);

    u32 glyphLeft = 0;
    u32 glyphBottom = imgHeight;

    for (u32 i = 0; i < utf32Codes.size(); ++i)
    {
        if (i % glyphsPerRow == 0)
        {
            glyphLeft = 0;
            glyphBottom -= glyphHeight;
        }

        u32 charCode = utf32Codes[i];

        FT_UInt glyphIndex = FT_Get_Char_Index(face, charCode);
        ERR_IF(glyphIndex == 0, "Unsupported char index: %s, char code: %u", sourceInfo.rawPath.c_str(), charCode);

        err = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
        PANIC_IF(err != 0, "Unable to FT_Load_Glyph: %s, glyphIndex: %u, err: %d", sourceInfo.rawPath.c_str(), glyphIndex, err);

        err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        PANIC_IF(err != 0, "Unable to FT_Render_Glyph: %s, glyphIndex: %u, err: %d", sourceInfo.rawPath.c_str(), glyphIndex, err);

        if (face->glyph->bitmap.buffer != nullptr)
        {
            // Copy bitmap into gimg.
            // Whitespace chars won't have a buffer to copy, but we're
            // already zero'd out when the Gimg was created.

            PANIC_IF(face->glyph->bitmap.pixel_mode != 2 || face->glyph->bitmap.num_grays != 256 || face->glyph->bitmap.palette_mode != 0, "glyph bitmap doesn't have 8 bit gray data");

            u32 xoffset = face->glyph->bitmap_left;
            u32 ystart = glyphBottom + (face->glyph->bitmap_top + glyphYOffset);
            u8 * gimgLine;
            for (u32 gbmH = 0; gbmH < face->glyph->bitmap.rows; ++gbmH)
            {
                gimgLine = pGimg.get()->scanline(ystart - gbmH);
                for (u32 gbmW = 0; gbmW < face->glyph->bitmap.width; ++gbmW)
                {
                    gimgLine[glyphLeft + xoffset + gbmW] = face->glyph->bitmap.buffer[gbmH * face->glyph->bitmap.width + gbmW];
                }
            }
        }

        u32 advPixX = (u32)((face->glyph->advance.x * (1.0f / 64.0f)) + 0.5f);

        // Atlas wants coords in non-opengl, 0,0 == top left coords
        u32 atlasTop = imgHeight - (glyphBottom + glyphHeight);
        fontAtl.addGlyph(charCode, uvec4(glyphLeft, atlasTop, advPixX, glyphHeight));

        glyphLeft += advPixX;
    }

    // Make any directories needed to write transitory .png and .atl
    ChefString transDir = parent_dir((const ChefString&)pngTransPath);
    make_dirs(transDir.c_str());

    // Write .png transitory output file
    Png::write_gimg(pngTransPath.c_str(), pGimg.get());
    UniquePtr<CookInfo> pCiPng = pCookInfo->chef().forceCook(pngTransPath);
    pCookInfo->transferCookResult(*pCiPng, kExtGimg);

    // Write .atl transitory output file
    fontAtl.write(atlTransPath.c_str());
    UniquePtr<CookInfo> pCiAtl = pCookInfo->chef().forceCook(atlTransPath);
    pCookInfo->transferCookResult(*pCiAtl, kExtGatl);

    // Write a AssetHeader only gfnt file so we can track Font cooker version and force recooks.
    AssetHeader * pGfntHeader = GNEW(kMEM_Chef, AssetHeader, FOURCC(kExtGfnt), sizeof(AssetHeader));
    pCookInfo->setCookedBuffer(pGfntHeader);
}

}
} // namespace gaen

