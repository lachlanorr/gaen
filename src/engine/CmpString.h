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

struct FormatSpecifier
{
    const char * begin;
    const char * end;

    u32 width;
    u32 precision;

    char type;

    // flags
    u8 flagMinus:1;
    u8 flagPlus:1;
    u8 flagSpace:1;
    u8 flagHash:1;
    u8 flagZero:1;
    u8 PADDING:3;
};

const char * find_next_specifier(FormatSpecifier * pFs, const char * str);


} // namespace gaen

#endif // #ifndef GAEN_ENGINE_CMPSTRING_H

