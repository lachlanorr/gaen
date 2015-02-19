//------------------------------------------------------------------------------
// Chef.h - Class to manage asset cookers
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#ifndef GAEN_CHEF_CHEF_H
#define GAEN_CHEF_CHEF_H

#include "core/mem.h"
#include "core/String.h"

namespace gaen
{

typedef void(*DependencyCB)(u32 chefId, const char * assetRawPath, const char * dependencyRawPath);

struct Cooker;

class Chef
{
public:
    Chef(u32 id, const char * platform, const char * assetsDir, DependencyCB dependencyCB);
    
    static bool is_valid_platform(const char * platform);

    u32 id() { return mId; }
    const char * platform() { return mPlatform.c_str(); }

    void cook(const char * platform, const char * rawPath);

    void recordDependency(const char * assetRawPath, const char * dependencyPath);

    // Path conversion functions
    bool isRawPath(const char * path);
    bool isCookedPath(const char * path);
    bool isGamePath(const char * path);
    
    void getRawPath(char * rawPath, const char * path, Cooker * pCooker = nullptr);
    void getCookedPath(char * cookedPath, const char * path, Cooker * pCooker = nullptr);
    void getGamePath(char * gamePath, const char * path, Cooker * pCooker = nullptr);

private:
    const size_t kMaxPlatform = 4;
    

    u32 mId;
    DependencyCB mDependencyCB;

    String<kMEM_Chef> mPlatform;
    String<kMEM_Chef> mAssetsDir;
    String<kMEM_Chef> mAssetsRawDir;
    String<kMEM_Chef> mAssetsCookedDir;
};

} // namespace gaen

#endif // #ifndef GAEN_CHEF_CHEF_H
