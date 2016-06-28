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

namespace gaen
{

class Asset
{
public:
    friend class AssetMgr;

    explicit Asset(const char * path);
    ~Asset();

    Asset(const Asset&)        = delete;
    Asset & operator=(Asset&)  = delete;
    Asset(Asset&&)             = delete;
    Asset & operator=(Asset&&) = delete;

    bool isLoaded() const
    {
        return mpBuffer != nullptr;
    }

    const char * path() const
    {
        return mPath;
    }

    u32 pathHash() const
    {
        return mPathHash;
    }

    const u8 * buffer() const
    {
        ASSERT(isLoaded());
        return mpBuffer;
    }

    u8 * writableBuffer()
    {
        ASSERT(isLoaded());
        PANIC_IF(!isWritable(), "Write access requested to read only Asset: %s", mPath);
        return mpBuffer;
    }

    bool isWritable() const
    {
        // LORRTODO: Base this value on the file extension allowing
        // certain assets to be writable. Thus, we can use asset
        // references to group entities on different TaskMasters based
        // on their use of writable assets.
        return mIsWritable;
    }

    const u64 size() const
    {
        ASSERT(isLoaded());
        return mSize;
    }

private:
    void load();
    void unload();

    u8 * mpBuffer;
    char * mPath;
    u32 mPathHash;
    u32 mIsWritable:1;
    u64 mSize;
}; // class Asset

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_H
