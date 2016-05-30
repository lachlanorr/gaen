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

#include "engine/Asset.h"

namespace gaen
{
/*
Asset::Asset(const char * path)
  : refCount(0)
{
    ASSERT(path);

    const u8 * pBuffer = load(path);

    if (pBuffer != nullptr)
    {
        pParts = GNEW(kMEM_Engine, Asset::AssetParts);

        u32 pathLen = strlen(path);
        pParts->path = GALLOC(kMEM_Engine, pathLen+1);
        strncpy(pParts->path, path, pathLen);
        pParts->path[pathLen] = '\0';

        pParts->pBuffer = pBuffer;
        pParts->refCount++;

        ASSERT((pParts && pParts->path && pParts->pBuffer && refCount > 0) ||
               !pParts);
    }
}

const u8 * Asset::load(const char * path)
{
    
}
*/
} // namespace gaen
