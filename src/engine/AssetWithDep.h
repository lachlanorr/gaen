//------------------------------------------------------------------------------
// AssetWithDep.h - Generic class for single dependent asset types
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#ifndef GAEN_ENGINE_ASSET_WITH_DEP_H
#define GAEN_ENGINE_ASSET_WITH_DEP_H

#include "hashes/hashes.h"

#include "engine/Asset.h"
#include "engine/AssetType.h"
#include "engine/AssetMgr.h"

namespace gaen
{

template <typename T, typename DT>
class AssetWithDep : public Asset
{
public:
    AssetWithDep(const char * path,
                 const char * fullPath,
                 MemType memType)
      : Asset(path, fullPath, memType)
      , mpDep0(nullptr)
    {}

    virtual bool isLoaded() const
    {
        return !mHadError && mpDep0 != nullptr;
    }

    virtual DependentVecUP dependents() const
    {
        const T * pAsset = T::instance(mpBuffer, mSize);

        DependentVecUP deps(GNEW(kMEM_Engine, DependentVec));
        deps->reserve(1);
        deps->emplace_back(HASH::dep0, pAsset->dep0Path());
        return deps;
    }

    virtual void setDependent(u32 nameHash,
                              Asset * pDependent)
    {
        T * pAsset = T::instance(mpBuffer, mSize);
        switch (nameHash)
        {
        case HASH::dep0:
        {
            ASSERT(!pAsset->dep0());
            ASSERT(pDependent->isLoaded());
            pAsset->setDep0(DT::instance(pDependent->buffer(), pDependent->size()));
            mpDep0 = pDependent;
            break;
        }
        }
    }

    virtual void addRefDependents() const
    {
        AssetMgr::addref_asset(0, mpDep0);
    }

    virtual void releaseDependents() const
    {
        AssetMgr::release_asset(0, mpDep0);
    }
private:
    Asset * mpDep0;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_WITH_DEP_H


