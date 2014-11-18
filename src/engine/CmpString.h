//------------------------------------------------------------------------------
// CmpString.h - String class used through BlockMemory in Compose scripts
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#ifndef GAEN_ENGINE_CMPSTRING_H
#define GAEN_ENGINE_CMPSTRING_H

#include "engine/BlockData.h"

namespace gaen
{

class BlockMemory;

class CmpString
{
    friend class BlockMemory;
public:
    CmpString(BlockData * pBlockData);

    char * c_str() { return &mpBlockData->data.string.chars[0]; }
    const char * c_str() const { return &mpBlockData->data.string.chars[0]; }
    u16 size() { return mpBlockData->data.string.charCount; }

private:
    BlockData * mpBlockData;
};


//------------------------------------------------------------------------------

struct FormatSize
{
    i16 totalWidth;
    i16 spacePadding;
    i16 zeroPadding;

    FormatSize() { reset(); }
    FormatSize(i16 totalWidth, i16 spacePadding, i16 zeroPadding)
      : totalWidth(totalWidth)
      , spacePadding(spacePadding)
      , zeroPadding(zeroPadding)
    {}

    void reset() { totalWidth = spacePadding = zeroPadding = -1; }
};

struct FormatSpecifier
{
    const char * begin;
    const char * end;

    i16 width;
    i16 precision;

    FormatSize size;

    char type;

    // flags
    u8 flagLeftJustify:1;
    u8 flagPlus:1;
    u8 flagSpace:1;
    u8 flagHash:1;
    u8 flagZero:1;

    u8 flagStarWidth:1;
    u8 flagStarPrecision:1;

    u8 PADDING:1;

    FormatSpecifier() { reset(); }

    void reset()
    {
        memset(this, 0, sizeof(FormatSpecifier));

        width = -1;
        precision = -1;
        size.reset();
    }

    u32 paramCount()
    {
        ASSERT(type != '\0' && begin && end);
        return 1 + flagStarWidth + flagStarPrecision;
    }
};


FormatSpecifier find_next_specifier(const char * str);



u32 float_whole_size(f32 val);

FormatSize * specifier_max_size(FormatSpecifier * pFs, i32 val);
FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, i32 val);
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, i32 val);
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, i32 val);


FormatSize * specifier_max_size(FormatSpecifier * pFs, u32 val);
FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, u32 val);
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, u32 val);
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, u32 val);

FormatSize * specifier_max_size(FormatSpecifier * pFs, f32 val);
FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, f32 val);
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, f32 val);
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, f32 val);

FormatSize * specifier_max_size(FormatSpecifier * pFs, char val);
FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, char val);
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, char val);
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, char val);

FormatSize * specifier_max_size(FormatSpecifier * pFs, bool val);
FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, bool val);
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, bool val);
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, bool val);

FormatSize * specifier_max_size(FormatSpecifier * pFs, const char * val);
FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, const char * val);
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, const char * val);
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, const char * val);


} // namespace gaen

#endif // #ifndef GAEN_ENGINE_CMPSTRING_H

