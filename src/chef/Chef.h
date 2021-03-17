//------------------------------------------------------------------------------
// Chef.h - Class to manage asset cookers
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

    u32 id() const { return mId; }
    const ChefString & platform() const { return mPlatform; }

    UniquePtr<CookInfo> cook(const char * rawPath, bool force) const;
    UniquePtr<CookInfo> forceCook(const ChefString & rawPath) const;
    void forceCook(CookInfo * pCi) const;
    void forceCookAndWrite(CookInfo * pCi) const;

    UniquePtr<CookInfo> prepCookInfo(const char * rawPath, bool force, Cooker * pCookerOverride = nullptr) const;
    bool shouldCook(const CookInfo & ci) const;

    ChefString getRawPath(const ChefString & path) const;
    ChefString getRawRelativePath(const ChefString & rawPath) const;
    ChefString getRawTransPath(const ChefString & rawPath, const ChefString & transExt) const;
    ChefString getCookedPath(const ChefString & path, const ChefString & cookedExt) const;
    ChefString getGamePath(const ChefString & path, const ChefString & cookedExt) const;
    ChefString getRelativeDependencyRawPath(const ChefString & sourceRawPath, const ChefString & dependencyPath) const;
    ChefString getDependencyFilePath(const ChefString & rawPath) const;

private:
    const size_t kMaxPlatform = 4;

    // Path conversion functions
    bool isRawPath(const ChefString & path) const;
    bool isCookedPath(const ChefString & path) const;
    bool isGamePath(const ChefString & path) const;

    RecipeListUP findRecipes(const ChefString & rawPath) const;
    RecipeUP overlayRecipes(const RecipeList & recipes) const;

    void writeDependencyFile(const CookInfo & ci) const;
    List<kMEM_Chef, ChefString> readDependencyFile(const ChefString & rawPath) const;
	void deleteDependencyFile(const ChefString & rawPath) const;

    u32 mId;

    ChefString mPlatform;
    ChefString mAssetsDir;
    ChefString mAssetsRawDir;
    ChefString mAssetsRawTransDir;
    ChefString mAssetsCookedDir;
};

} // namespace gaen

#endif // #ifndef GAEN_CHEF_CHEF_H
