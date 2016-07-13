//------------------------------------------------------------------------------
// AssetTypeGspr.cpp - Custom asset type for Gspr
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

#include "hashes/hashes.h"

#include "assets/Gatl.h"
#include "assets/Gspr.h"
#include "assets/AssetTypeGspr.h"

namespace gaen
{

AssetTypeGspr::AssetTypeGspr()
  : AssetType("gspr", kMEM_Engine)
{}

bool AssetTypeGspr::isFullyLoaded(const void * pBuffer,
                                  u64 size) const
{
    if (!pBuffer || size == 0)
        return false;

    const Gspr * pGspr = Gspr::instance(pBuffer, size);
    return pGspr->pAtlas != nullptr;
}
    
DependentVecUP AssetTypeGspr::dependents(const void * pBuffer,
                                         u64 size) const
{
    const Gspr * pGspr = Gspr::instance(pBuffer, size);
        
    DependentVecUP deps(GNEW(kMEM_Engine, DependentVec));
    deps->reserve(1);
    deps->emplace_back(HASH::atlas, pGspr->atlasPath());
    return deps;
}
    
void AssetTypeGspr::setDependent(u32 nameHash,
                                 void * pBuffer,
                                 u64 size,
                                 const void * pDependentBuffer,
                                 u64 dependentSize) const
{
    Gspr * pGspr = Gspr::instance(pBuffer, size);
    ASSERT(!pGspr->pAtlas);
        
    switch (nameHash)
    {
    case HASH::atlas:
        pGspr->pAtlas = Gatl::instance(pDependentBuffer, dependentSize);
    }
}

} // namespace gaen

