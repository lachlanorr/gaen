//------------------------------------------------------------------------------
// hashing.h - Various hash functions
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

#ifndef GAEN_CORE_HASHING_H
#define GAEN_CORE_HASHING_H

#include "core/base_defines.h"

namespace gaen
{

u32 fnv1a_32(const char *str);
u32 fnv1a_32(const u8 *pBuff, const size_t buffSize);

// Use this everywhere, avoid using fnv1a_32 direectly since we may
// change the algorithm some day.
inline u32 gaen_hash(const char * str)
{
    return fnv1a_32(str);
}

} // namespace gaen


#endif // #ifndef GAEN_CORE_HASHING_H
