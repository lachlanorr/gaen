//------------------------------------------------------------------------------
// Asset.cpp - Smart wrapper for raw asset buffers
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

#include "core/mem.h"
#include "engine/hashes.h"
#include "assets/file_utils.h"
#include "assets/AssetTypes.h"
#include "engine/AssetLoader.h"

#include "engine/Asset.h"

namespace gaen
{

Asset::Asset(const char * path,
             const char * fullPath,
             const AssetTypes & assetTypes)
  : mPath(path)
  , mRefCount(0)
  , mpBuffer(nullptr)
  , mSize(0)
  , mIsMutable(false)
{
    ASSERT(path);

    static std::atomic<u64> sUidCounter(0);
    mUid = ++sUidCounter;
    
    mPathHash = HASH::hash_func(path);
    load(fullPath, assetTypes);
}

Asset::~Asset()
{
    if (isLoaded())
        unload();
}

void Asset::load(const char * fullPath,
                 const AssetTypes & assetTypes)
{
    PANIC_IF(isLoaded(), "load called on already loaded asset: %s", mPath);

    FileReader rdr(fullPath);

    if (rdr.isOk())
    {
        mSize = rdr.size();
        if (mSize > 0)
        {
            ASSERT(!mpBuffer);
            MemType memType = assetTypes.memTypeFromExt(get_ext(mPath.c_str()));
            mpBuffer = (u8*)GALLOC(memType, mSize);
            rdr.read(mpBuffer, mSize);
        }
    }
}
    
void Asset::unload()
{
    PANIC_IF(!isLoaded(), "unload called on unloaded asset: %s", mPath);
    GFREE(mpBuffer);
    mpBuffer = nullptr;
}

} // namespace gaen
