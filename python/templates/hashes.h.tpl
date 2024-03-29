//------------------------------------------------------------------------------
// hashes.h - Precalculated hashes, generated by update_hashes.py
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

#ifndef GAEN_ENGINE_HASHES_H
#define GAEN_ENGINE_HASHES_H

#include "gaen/core/HashMap.h"
#include "gaen/core/base_defines.h"

// Determines if string collisions are detected in hashes
#define TRACK_HASHES WHEN(HAS(DEV_BUILD))

namespace gaen
{

class HASH
{
public:
    static u32 hash_func(const char * str);
    static const char * reverse_hash(u32 hash);

    // Pre calculated hashes.
    // Generated with update_hashes.py, which gets run
    // during the build.
${hashes_const_declarations}
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_HASHES_H
