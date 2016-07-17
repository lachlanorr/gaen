//------------------------------------------------------------------------------
// Asset.h - Smart wrapper for raw asset buffers
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

#ifndef GAEN_ENGINE_ASSET_H
#define GAEN_ENGINE_ASSET_H

#include "core/base_defines.h"
#include "core/mem.h"
#include "core/String.h"

#include "assets/AssetType.h"

namespace gaen
{

class AssetTypes;

class Asset
{
    friend class AssetMgr;
public:

    explicit Asset(const char * path,
                   const char * fullPath,
                   const AssetTypes & assetTypes);
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
    
    bool isLoaded() const
    {
        return !mHadError && mpAssetType->isFullyLoaded(mpBuffer, mSize);
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
        return mpBuffer;
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
        return mpBuffer;
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

private:
    void addRef()
    {
        mRefCount++;
    }

    bool release()
    {
        ASSERT(mRefCount > 0);
        mRefCount--;

        return mRefCount == 0;
    }

    u32 refCount()
    {
        return mRefCount;
    }

    void load(const char * fullPath,
              const AssetTypes & assetTypes);
    void unload();

    String<kMEM_Engine> mPath;
    u32 mPathHash;
    u32 mRefCount;

    u8 * mpBuffer;
    u64 mSize;

    u64 mUid;

    const AssetType * mpAssetType;
    
    bool mIsMutable;
    bool mHadError;

}; // class Asset

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_H
