//------------------------------------------------------------------------------
// AssetTypes.cpp - Mapping of asset extension to various asset properties
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

#include "assets/AssetTypes.h"

namespace gaen
{

AssetTypes::AssetTypes()
{
    // Initialize mapping between extensions and memory types so when
    // we allocate buffers for assets we can place them in appropriate
    // buckets.
    mExtToMemTypeMap[ext_to_4cc("gatl")] = kMEM_Engine;
    mExtToMemTypeMap[ext_to_4cc("gimg")] = kMEM_Texture;
    mExtToMemTypeMap[ext_to_4cc("gmat")] = kMEM_Renderer;
    mExtToMemTypeMap[ext_to_4cc("gvtx")] = kMEM_Engine;
    mExtToMemTypeMap[ext_to_4cc("gfrg")] = kMEM_Engine;
}

#ifdef IS_GAEN_PROJECT
void AssetTypes::registerProjectAssetTypes()
{
    // If we're compiling gaen as a project, no custom asset types.
}
#endif

MemType AssetTypes::memTypeFromExt(const char * ext) const
{
    // ensure a 3 character (or longer) extension
    if (!ext[0] || !ext[1] || !ext[2])
        return kMEM_Unspecified;

    u32 cc = ext_to_4cc(ext);
    auto it = mExtToMemTypeMap.find(cc);
    if (it != mExtToMemTypeMap.end())
        return it->second;
    else
        return kMEM_Unspecified;
}

} // namespace gaen
