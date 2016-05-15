//------------------------------------------------------------------------------
// Chef.h - Class to manage asset cookers
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

#ifndef GAEN_CHEF_CHEF_H
#define GAEN_CHEF_CHEF_H

#include "core/mem.h"
#include "core/String.h"
#include "assets/Config.h"

namespace gaen
{

typedef void(*DependencyCB)(u32 chefId, const char * assetRawPath, const char * dependencyRawPath);

struct Cooker;

class Chef
{
public:
    Chef(u32 id, const char * platform, const char * assetsDir, bool force, DependencyCB dependencyCB);
    
    static const char * default_platform();
    static bool is_valid_platform(const char * platform);

    static void assets_raw_dir(char * assetsRawDir, const char * assetsDir);
    static void assets_cooked_dir(char * assetsCookedDir, const char * platform, const char * assetsDir);

    u32 id() { return mId; }
    const char * platform() { return mPlatform.c_str(); }

    void cook(const char * path);

    void reportDependency(char * dependencyRawPath, const char * sourceRawPath, const char * dependencyPath);

    // Path conversion functions
    bool isRawPath(const char * path);
    bool isCookedPath(const char * path);
    bool isGamePath(const char * path);
    
    void getRawPath(char * rawPath, const char * path, Cooker * pCooker = nullptr);
    void getRawRelativePath(char * rawRelativePath, const char * rawPath);
    void getCookedPath(char * cookedPath, const char * path, Cooker * pCooker = nullptr);
    void getGamePath(char * gamePath, const char * path, Cooker * pCooker = nullptr);

private:
    typedef List<kMEM_Chef, String<kMEM_Chef>> RecipeList;
    
    const size_t kMaxPlatform = 4;

    bool shouldCook(const char * rawPath, const char * cookedPath, const RecipeList & recipes);
    RecipeList findRecipes(const char * rawPath);
    void overlayRecipes(Config<kMEM_Chef> & recipe, const RecipeList & recipes);
    bool convertRelativeDependencyPath(char * dependencyRawPath, const char * sourceRawPath, const char * dependencyPath);

	void getDependencyFilePath(char * dependencyFilePath, const char * path);
	void deleteDependencyFile(const char * path);

    u32 mId;
    bool mForce;
    DependencyCB mDependencyCB;

    String<kMEM_Chef> mPlatform;
    String<kMEM_Chef> mAssetsDir;
    String<kMEM_Chef> mAssetsRawDir;
    String<kMEM_Chef> mAssetsCookedDir;
};

} // namespace gaen

#endif // #ifndef GAEN_CHEF_CHEF_H
