//------------------------------------------------------------------------------
// AssetLoader.cpp - Loads assets from disk and manages lifetimes.
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

#include "engine/stdafx.h"

#include "core/HashMap.h"
#include "engine/MessageQueue.h"

#include "engine/AssetLoader.h"

namespace gaen
{

// 4cc is endian dangerous, but we only do this within a running process,
// these 4 character codes are never persisted between processes.
static inline u32 ext_to_4cc(const char * ext)
{
    ASSERT(strlen(ext) >= 3);
    u32 cc = 0;
    cc |= ext[0] << 3;
    cc |= ext[1] << 2;
    cc |= ext[2] << 1;
    cc |= ext[3];
    return cc;
}

MemType AssetLoader::mem_type_from_ext(const char * ext)
{
    static HashMap<kMEM_Engine, u32, MemType> sMap;
    if (sMap.size() == 0)
    {
        // initialize, first time through
        sMap[ext_to_4cc("gatl")] = kMEM_Engine;
        sMap[ext_to_4cc("gimg")] = kMEM_Texture;
        sMap[ext_to_4cc("gmat")] = kMEM_Renderer;
        sMap[ext_to_4cc("gvtx")] = kMEM_Engine;
        sMap[ext_to_4cc("gfrg")] = kMEM_Engine;
    }

    // ensure a 3 character (or longer) extension
    if (!ext[0] || !ext[1] || !ext[2])
        return kMEM_Unspecified;

    u32 cc = ext_to_4cc(ext);
    auto it= sMap.find(cc);
    if (it != sMap.end())
        return it->second;
    else
        return kMEM_Unspecified;
}

} // namespace gaen
