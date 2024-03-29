//------------------------------------------------------------------------------
// AssetType.h - Abstract class that defines various asset type methods
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

#ifndef GAEN_ENGINE_ASSET_TYPE_H
#define GAEN_ENGINE_ASSET_TYPE_H

#include "gaen/core/base_defines.h"
#include "gaen/core/mem.h"

#include "gaen/engine/Asset.h"

namespace gaen
{

class AssetTypes;

class AssetType
{
public:
    AssetType(const char * extension,
              MemType memType,
              AssetConstructor constructor)
      : mExtension(extension)
      , mMemType(memType)
      , mConstructor(constructor)
    {}
    
    const char * extension() const { return mExtension; }
    MemType memType() const { return mMemType; }

    Asset * construct(const char * path,
                      const char * fullPath) const
    {
        return mConstructor(path, fullPath, mMemType);
    }
    
private:
    const char * mExtension;
    MemType mMemType;
    AssetConstructor mConstructor;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_TYPE_H
