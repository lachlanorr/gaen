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

#include "assets/file_utils.h"
#include "assets/Config.h"
#include "assets/Gatl.h"
#include "assets/Gimg.h"

#include "chef/cooker_utils.h"
#include "chef/CookInfo.h"
#include "chef/Chef.h"
#include "chef/Png.h"
#include "chef/cookers/Image.h"
#include "chef/cookers/Font.h"


namespace gaen
{
namespace cookers
{


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

Font::Font()
{
    mVersion = 1;
    mRawExts.push_back(kExtFnt);
    mCookedExts.push_back(kExtGfnt);
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
    int size = fnt.getInt(kSize);
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

    // Create an image large enough to hold all glyphs
    u32 glyphsPerRow = (u32)(glm::sqrt((f32)utf32Codes.size()) + 0.5f);
    u32 imgHeight = next_power_of_two(glyphsPerRow * size);
    Scoped_GFREE<Gimg> pGimg = Gimg::create(kPXL_R8, imgHeight, imgHeight);

    u32 glyphLeft = 0;
    u32 glyphBottom = imgHeight;

    for (u32 i = 0; i < utf32Codes.size(); ++i)
    {
        if (i % glyphsPerRow == 0)
        {
            glyphLeft = 0;
            glyphBottom -= size;
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
            u32 ystart = glyphBottom + face->glyph->bitmap_top;
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
        glyphLeft += advPixX;
    }

    // Write .png transitory output file
    ChefString pngTransPath = pCookInfo->chef().getRawTransPath(pCookInfo->rawPath(), kExtPng);

    // make any directories needed to write .png
    ChefString transDir = parent_dir((const ChefString&)pngTransPath);
    make_dirs(transDir.c_str());

    Png::write_gimg(pngTransPath.c_str(), pGimg.get());

}

}
} // namespace gaen

