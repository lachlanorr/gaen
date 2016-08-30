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
#include "chef/CookInfo.h"

namespace gaen
{

class Cooker;
class CookInfo;

class Chef
{
    friend class CookInfo;
public:
    Chef(u32 id, const char * platform, const char * assetsDir);
    
    u32 id() { return mId; }
    const ChefString & platform() { return mPlatform; }

    UniquePtr<CookInfo> cook(const char * rawPath, bool force);
    void forceCook(CookInfo * pCi);
    void forceCookAndWrite(CookInfo * pCi);

    UniquePtr<CookInfo> prepCookInfo(const char * rawPath, bool force);
    bool shouldCook(const CookInfo & ci);

private:
    const size_t kMaxPlatform = 4;

    // Path conversion functions
    bool isRawPath(const ChefString & path);
    bool isCookedPath(const ChefString & path);
    bool isGamePath(const ChefString & path);

    ChefString getRawPath(const ChefString & path);
    ChefString getRawRelativePath(const ChefString & rawPath);
    ChefString getCookedPath(const ChefString & path, const ChefString & cookedExt);
    ChefString getGamePath(const ChefString & path, const ChefString & cookedExt);
    ChefString getRelativeDependencyRawPath(const ChefString & sourceRawPath, const ChefString & dependencyPath);
	ChefString getDependencyFilePath(const ChefString & rawPath);

    RecipeList findRecipes(const ChefString & rawPath);
    Recipe overlayRecipes(const RecipeList & recipes);

    void writeDependencyFile(const CookInfo & ci);
    List<kMEM_Chef, ChefString> readDependencyFile(const ChefString & rawPath);
	void deleteDependencyFile(const ChefString & rawPath);

    u32 mId;

    ChefString mPlatform;
    ChefString mAssetsDir;
    ChefString mAssetsRawDir;
    ChefString mAssetsRawTransDir;
    ChefString mAssetsCookedDir;
};

} // namespace gaen

#endif // #ifndef GAEN_CHEF_CHEF_H
