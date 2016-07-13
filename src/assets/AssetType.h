//------------------------------------------------------------------------------
// AssetType.h - Abstract class that defines various asset type methods
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

#ifndef GAEN_ENGINE_ASSET_TYPE_H
#define GAEN_ENGINE_ASSET_TYPE_H

#include "core/base_defines.h"
#include "core/mem.h"
#include "core/Vector.h"

namespace gaen
{

struct Dependent
{
    Dependent(u32 nameHash, const char * path)
      : nameHash(nameHash)
      , path(path)
    {}

    u32 nameHash;

    // We don't copy the path string into this struct, but just
    // maintain a pointer.  The reason this works is that this struct
    // is only a temporary list used by AssetMgr when it's wanting the
    // dependent paths from an asset, and they will always be defined
    // within the asset buffer that is building this list.
    const char * path;
};

typedef Vector<kMEM_Engine, Dependent> DependentVec;
typedef UniquePtr<DependentVec> DependentVecUP;
    
class AssetType
{
public:
    AssetType(const char * extension, MemType memType)
      : mExtension(extension)
      , mMemType(memType)
    {}
    
    const char * extension() const { return mExtension; }
    MemType memType() const { return mMemType; }

    virtual bool isFullyLoaded(const void * pBuffer,
                               u64 size) const
    {
        return pBuffer != nullptr && size > 0;
    }
    
    virtual DependentVecUP dependents(const void * pBuffer,
                                      u64 size) const
    {
        return DependentVecUP();
    }
    
    virtual void setDependent(u32 nameHash,
                              void * pBuffer,
                              u64 size,
                              const void * pDependentBuffer,
                              u64 dependentSize) const
    {}

private:
    const char * mExtension;
    MemType mMemType;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_TYPE_H
