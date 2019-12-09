//------------------------------------------------------------------------------
// AssetTypes.cpp - Mapping of asset extension to various asset properties
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

#include "engine/stdafx.h"

#include "assets/file_utils.h"

#include "assets/Gatl.h"
#include "assets/Gimg.h"
#include "assets/Gspr.h"

#include "engine/Asset.h"
#include "engine/AssetWithDep.h"
#include "engine/AssetTypes.h"

namespace gaen
{

AssetTypes::AssetTypes()
{
    // Register the built in gaen asset types

    registerAssetType("gfrg", kMEM_Engine,   Asset::construct<Asset>);
    registerAssetType("gimg", kMEM_Texture,  Asset::construct<Asset>);
    registerAssetType("gmat", kMEM_Renderer, Asset::construct<Asset>);
    registerAssetType("gvtx", kMEM_Engine,   Asset::construct<Asset>);
    registerAssetType("gmdl", kMEM_Engine,   Asset::construct<Asset>);
    registerAssetType("gaim", kMEM_Engine,   Asset::construct<Asset>);

    registerAssetType("gatl", kMEM_Engine,   Asset::construct<AssetWithDep<Gatl,Gimg>>);
    registerAssetType("gspr", kMEM_Engine,   Asset::construct<AssetWithDep<Gspr,Gatl>>);

    registerProjectAssetTypes();
}

void AssetTypes::registerAssetType(const char * extension,
                                   MemType memType,
                                   AssetConstructor constructor)
{
    u32 ext4cc = ext_to_4cc(extension);

    ASSERT(mExtToAssetTypeMap.find(ext4cc) == mExtToAssetTypeMap.end());

    mExtToAssetTypeMap.emplace(ext4cc, GNEW(kMEM_Engine, AssetType, extension, memType, constructor));
}


#ifdef IS_GAEN_PROJECT
void AssetTypes::registerProjectAssetTypes()
{
    // If we're compiling gaen as a project, no custom asset types.
}
#endif

const AssetType * AssetTypes::assetTypeFromExt(const char * ext) const
{
    // ensure a 3 character (or longer) extension
    if (!ext || !ext[0] || !ext[1] || !ext[2])
    {
        PANIC("Invalid type extension: %s", ext == nullptr ? "<nullptr>" : ext);
        return nullptr;
    }

    u32 cc = ext_to_4cc(ext);
    auto it = mExtToAssetTypeMap.find(cc);
    if (it != mExtToAssetTypeMap.end())
    {
        return it->second.get();
    }
    else
    {
        PANIC("Unknown asset type extension: %s", ext);
        return nullptr;
    }
}

} // namespace gaen
