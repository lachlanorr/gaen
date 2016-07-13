//------------------------------------------------------------------------------
// AssetTypeGspr.h - Custom asset type for Gspr
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

#ifndef GAEN_ASSETS_ASSET_TYPE_GSPR_H
#define GAEN_ASSETS_ASSET_TYPE_GSPR_H

#include "assets/AssetType.h"

namespace gaen
{

class AssetTypeGspr : public AssetType
{
public:
    AssetTypeGspr();

    virtual bool isFullyLoaded(const void * pBuffer,
                               u64 size) const;

    virtual DependentVecUP dependents(const void * pBuffer,
                                      u64 size) const;
    
    virtual void setDependent(u32 nameHash,
                              void * pBuffer,
                              u64 size,
                              const void * pDependentBuffer,
                              u64 dependentSize) const;

};

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_ASSET_TYPE_GSPR_H
