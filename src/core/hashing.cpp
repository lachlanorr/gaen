//------------------------------------------------------------------------------
// hashing.cpp - Various hash functions
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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

#include "core/stdafx.h"

#include "core/hashing.h"


namespace gaen
{

static const u32 kFnv1_32Init  = 0x811c9dc5;
static const u32 kFnv1_32Prime = 0x01000193;

u32 fnv1a_32(const char *str)
{
    ASSERT(str);

    u32 hval = kFnv1_32Init;

    const u8 *s = reinterpret_cast<const u8 *>(str);

    // FNV-1a hash each octet in the buffer
    while (*s)
    {
        // xor the bottom with the current octet
        hval ^= static_cast<u32>(*s++);

        // multiply by the 32 bit FNV magic prime mod 2^32
        hval *= kFnv1_32Prime;
    }
    return hval;
}

u32 fnv1a_32(const u8 *pBuff, const size_t buffSize)
{
    ASSERT(pBuff && buffSize > 0);

    u32 hval = kFnv1_32Init;

    const u8 *p = pBuff;
    const u8 *end = pBuff + buffSize;

    // FNV-1a hash each octet in the buffer
    while (p < end)
    {
        // xor the bottom with the current octet
        hval ^= static_cast<u32>(*p++);

        // multiply by the 32 bit FNV magic prime mod 2^32
        hval *= kFnv1_32Prime;
    }
    return hval;
}


} // namespace gaen

