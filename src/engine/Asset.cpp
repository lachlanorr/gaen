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
#include "engine/AssetMgr.h"

#include "engine/Asset.h"

namespace gaen
{

Asset::Asset(const char * path)
  : mpBuffer(nullptr)
  , mRefCount(0)
  , mStatusFlags(kASFL_None)
  , mSize(0)
{
    ASSERT(path);

    size_t pathLen = strlen(path);
    mPath = (char*)GALLOC(kMEM_Engine, pathLen + 1);
    strcpy(mPath, path);
    mPath[pathLen] = '\0'; // sanity

    mPathHash = HASH::hash_func(mPath);
}

void Asset::load()
{
    PANIC_IF(isLoaded(), "load called on already loaded asset: %s", mPath);
    mStatusFlags = kASFL_None;

    FileReader rdr(mPath);
    mStatusFlags = rdr.statusFlags();

    if (rdr.isOk())
    {
        mSize = rdr.size();
        if (mSize > 0)
        {
            ASSERT(!mpBuffer);
            MemType memType = AssetMgr::mem_type_from_ext(get_ext(mPath));
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
    mStatusFlags = kASFL_None;
}

} // namespace gaen
