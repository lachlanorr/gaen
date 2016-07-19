//------------------------------------------------------------------------------
// AssetGspr.cpp - Custom asset type for Gspr to handle Gatl dependent
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

#include "assets/Gspr.h"
#include "assets/Gatl.h"
#include "hashes/hashes.h"
#include "engine/AssetMgr.h"
#include "engine/AssetGspr.h"

namespace gaen
{

AssetGspr::AssetGspr(const char * path,
                     const char * fullPath,
                     MemType memType)
  : Asset(path, fullPath, memType)
  , mpGatlAsset(nullptr)
{}

bool AssetGspr::isLoaded() const
{
    return !mHadError && mpGatlAsset != nullptr;
}
    
DependentVecUP AssetGspr::dependents() const
{
    const Gspr * pGspr = Gspr::instance(mpBuffer, mSize);
        
    DependentVecUP deps(GNEW(kMEM_Engine, DependentVec));
    deps->reserve(1);
    deps->emplace_back(HASH::atlas, pGspr->atlasPath());
    return deps;
}
    
void AssetGspr::setDependent(u32 nameHash,
                             Asset * pDependent)
{
    Gspr * pGspr = Gspr::instance(mpBuffer, mSize);
    switch (nameHash)
    {
    case HASH::atlas:
    {
        ASSERT(!pGspr->mpAtlas);
        ASSERT(pDependent->isLoaded());
        pGspr->mpAtlas = Gatl::instance(pDependent->buffer(), pDependent->size());
        mpGatlAsset = pDependent;
        break;
    }
    }
}

void AssetGspr::addRefDependents() const
{
    AssetMgr::addref_asset(0, mpGatlAsset);
}

void AssetGspr::releaseDependents() const
{
    AssetMgr::release_asset(0, mpGatlAsset);
}

} // namespace gaen



