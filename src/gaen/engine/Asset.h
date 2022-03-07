//------------------------------------------------------------------------------
// Asset.h - Smart wrapper for raw asset buffers
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

#ifndef GAEN_ENGINE_ASSET_H
#define GAEN_ENGINE_ASSET_H

#include "gaen/core/base_defines.h"
#include "gaen/core/mem.h"
#include "gaen/core/String.h"
#include "gaen/core/Vector.h"

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


class Asset
{
    friend class AssetMgr;
public:

    Asset(const char * path,
          const char * fullPath,
          MemType memType);
    ~Asset();

    Asset(const Asset&)        = delete;
    Asset & operator=(Asset&)  = delete;
    Asset(Asset&&)             = delete;
    Asset & operator=(Asset&&) = delete;

    bool operator==(const Asset & rhs)
    {
        // Use mPathHash for fast comparisons
        return (mPathHash == rhs.mPathHash &&
                mPath == rhs.mPath);
    }

    virtual bool isLoaded() const
    {
        return !mHadError && mpBuffer != nullptr && mSize > 0;
    }

    bool hadError() const
    {
        return mHadError;
    }

    const String<kMEM_Engine> & path() const
    {
        return mPath;
    }

    u32 pathHash() const
    {
        return mPathHash;
    }

    template <typename T>
    const T * buffer() const
    {
        ASSERT(isLoaded());
        return reinterpret_cast<const T*>(mpBuffer);
    }

    const void * buffer() const
    {
        ASSERT(isLoaded());
        return mpBuffer;
    }

    template <typename T>
    T * mutableBuffer()
    {
        ASSERT(isLoaded());
        PANIC_IF(!isMutable(), "Write access requested to read only Asset: %s", mPath);
        return const_cast<T*>(reinterpret_cast<const T*>(mpBuffer));
    }

    bool isMutable() const
    {
        // LORRTODO: Base this value on the file extension allowing
        // certain assets to be mutable. Thus, we can use asset
        // references to group entities on different TaskMasters based
        // on their use of mutable assets.
        return mIsMutable;
    }

    const u64 size() const
    {
        ASSERT(isLoaded());
        return mSize;
    }

    const u64 uid() const
    {
        return mUid;
    }

    template <class T>
    static Asset * construct(const char * path,
                             const char * fullPath,
                             MemType memType)
    {
        return GNEW(kMEM_Engine, T, path, fullPath, memType);
    }

protected:
    void addRef()
    {
        ASSERT(hadError() || isLoaded());
        mRefCount++;
        addRefDependents();
    }

    bool release()
    {
        ASSERT(hadError() || isLoaded());
        ASSERT(mRefCount > 0);
        releaseDependents();
        mRefCount--;

        return mRefCount == 0;
    }

    virtual DependentVecUP dependents() const
    {
        return DependentVecUP();
    }

    virtual void setDependent(u32 nameHash,
                              Asset * pDependent)
    {
        PANIC("setDependent called on an Asset type that has no dependents");
    }

    virtual void addRefDependents() const
    {}

    virtual void releaseDependents() const
    {}

    u32 refCount()
    {
        return mRefCount;
    }

    void load(const char * fullPath,
              MemType memType);
    void unload();

    String<kMEM_Engine> mPath;
    u32 mPathHash;
    u32 mRefCount;

    void * mpBuffer;
    u64 mSize;

    u64 mUid;

    bool mIsMutable;
    bool mHadError;

}; // class Asset

typedef Asset*(*AssetConstructor)(const char * path,
                                  const char * fullPath,
                                  MemType memType);

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_H
