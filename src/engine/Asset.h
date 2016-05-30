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

#ifndef GAEN_ENGINE_ASSET_MGR_H
#define GAEN_ENGINE_ASSET_MGR_H

#include "core/base_defines.h"
#include "core/mem.h"

namespace gaen
{
/*
class Asset
{
public:
    explicit Asset(const char * path);
    ~Asset()
    {
		if (mpParts.get())
            mpParts.reset();
    }

    bool isLoaded()
    {

		ASSERT((pParts && pParts->path && pParts->pBuffer && pParts->refCount.load() > 0) ||
               !pParts);
        return pParts && pParts->path && pParts->pBuffer;
    }

    const char * path() const
    {
        ASSERT(isLoaded());
        return pParts->path;
    }

    const u8 * buffer() const
    {
        ASSERT(isLoaded());
        return pParts->pBuffer;
    }

    bool operator==(const Asset & rhs) const
    {
        if (!isLoaded() || !rhs.isLoaded())
            return false;
		return 0 == strcmp(path(), rhs.path());
    }
private:
    void release()
    {
        ASSERT(isLoaded());
        int oldVal = pParts->refCount.fetch_sub(1);
    }

    static const u8 * load(const char * path);
    
    struct AssetParts
    {
        const char * path = nullptr;
        u8 * pBuffer      = nullptr;
    };

    //SharedPtr<AssetParts> mpParts;
}; // class Asset
*/
} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_MGR_H
