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
    explicit Asset(const char * path);

    bool isLoaded() const
    {
        return mpBuffer != nullptr;
    }

    const char * path() const
    {
        return mPath;
    }

    const u8 * buffer() const
    {
        ASSERT(isLoaded());
        return mpBuffer;
    }

    const u64 size() const
    {
        ASSERT(isLoaded());
        return mSize;
    }

    u32 statusFlags() const
    {
        return mStatusFlags;
    }

    void addRef()
    {
        ASSERT(isLoaded());
        mRefCount++;
    }

    void release()
    {
        ASSERT(isLoaded());
        ASSERT(mRefCount > 0);
        mRefCount--;
        if (mRefCount == 0)
            unload();
    }

    void load();
    void unload();
private:
    u8 * mpBuffer;
    char * mPath;
    u32 mPathHash;
    u32 mRefCount:16;
    u32 mStatusFlags:16;
    u64 mSize;
}; // class Asset

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_H
