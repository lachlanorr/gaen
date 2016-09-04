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

#include "chef/cooker_utils.h"
#include "chef/CookInfo.h"
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

    static const char * kSource = "source";
    static const char * kSizes = "sizes";
    static const char * kCharacters = "characters";

    PANIC_IF(!fnt.hasKey(kSource), "Missing 'source' in .fnt");
    PANIC_IF(!fnt.hasKey(kSizes), "Missing 'sizes' in .fnt");
    PANIC_IF(!fnt.hasSection(kCharacters), "Missing [characters] section in .fnt");

    ChefString sourcePath = fnt.get(kSource);
    auto sizes = fnt.getIntVec(kSizes);

    Vector<kMEM_Chef, u32> utf32Chars;
    utf32Chars.reserve(fnt.sectionKeyCount(kCharacters));

    // Parse all characters, accounting for utf8 encoded chars
    auto charsEnd = fnt.keysEnd(kCharacters);
    for (auto it = fnt.keysBegin(kCharacters);
         it != charsEnd;
         ++it)
    {
        utf32Chars.push_back(utf8_to_utf32(*it));
    }

    const DependencyInfo & sourceInfo = pCookInfo->recordDependency(sourcePath);

    // Load font
    FT_Error err;
    FT_Face face;

    for (int size : sizes)
    {
        err = FT_New_Face(ft_library(), sourceInfo.rawPath.c_str(), 0, &face);
        PANIC_IF(err != 0, "Unable to FT_New_Face: %s, err: %d", sourceInfo.rawPath.c_str(), err);

        // Set the pixel size... we don't set point sizes because then we deal with DPI.
        // For game fonts, it's easier to think of pixel sizes.
        err = FT_Set_Pixel_Sizes(face, 0, size);
        PANIC_IF(err != 0, "Unable to FT_Set_Pixel_Sizes: %s, size: %d, err: %d", sourceInfo.rawPath.c_str(), size, err);

        u32 charCode = (u32)'A';
        FT_UInt glyphIndex = FT_Get_Char_Index(face, charCode);
        ERR_IF(glyphIndex == 0, "Unsupported char index: %s, char code: %u", sourceInfo.rawPath.c_str(), charCode);

        err = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
        PANIC_IF(err != 0, "Unable to FT_Load_Glyph: %s, glyphIndex: %u, err: %d", sourceInfo.rawPath.c_str(), glyphIndex, err);

        err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        PANIC_IF(err != 0, "Unable to FT_Render_Glyph: %s, glyphIndex: %u, err: %d", sourceInfo.rawPath.c_str(), glyphIndex, err);
    }

}

}
} // namespace gaen

