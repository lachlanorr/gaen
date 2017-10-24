//------------------------------------------------------------------------------
// Chef.cpp - Class to manage asset cookers
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

#include "chef/stdafx.h"

#include "core/base_defines.h"
#include "core/thread_local.h"

#include "assets/file_utils.h"
#include "assets/AssetHeader.h"

#include "chef/CookerRegistry.h"
#include "chef/Chef.h"

namespace gaen
{
static const u16 kChefVersion = 1;

Chef::Chef(u32 id, const char * platform, const char * assetsDir)
  : mId(id)
{
    ASSERT(platform);
    ASSERT(assetsDir);

    mPlatform = platform;

    mAssetsDir = normalize_path(ChefString(assetsDir));

    mAssetsRawDir = assets_raw_dir(mAssetsDir);
    mAssetsRawTransDir = assets_raw_trans_dir(mAssetsDir);
    mAssetsCookedDir = assets_cooked_dir(platform, mAssetsDir);
}

UniquePtr<CookInfo> Chef::cook(const char * rawPath, bool force)
{
    UniquePtr<CookInfo> pCi = prepCookInfo(rawPath, force);

    // verify we should cook
    if (!pCi || !shouldCook(*pCi))
        return UniquePtr<CookInfo>();
    else
        forceCookAndWrite(pCi.get());

    return pCi;
}

UniquePtr<CookInfo> Chef::forceCook(const ChefString & rawPath)
{
    UniquePtr<CookInfo> pCi = prepCookInfo(rawPath.c_str(), true);
    forceCook(pCi.get());
    return pCi;
}

void Chef::forceCook(CookInfo * pCi)
{
    pCi->cooker().cook(pCi);

    // Set versions in each cooked AssetHeader
    for (const CookResult & res : pCi->results())
    {
        if (res.isCooked())
        {
            PANIC_IF(res.pCookedBuffer->magic4cc() != fourcc(res.cookedExt), "Cooked buffer has wrong ext 4CC: %s", res.cookedPath.c_str());
            PANIC_IF(res.pCookedBuffer->size() != res.cookedBufferSize, "Cooked buffer has wrong size: %s", res.cookedPath.c_str());
            res.pCookedBuffer->setChefVersion(kChefVersion);

            // Only set CookerVersion if it hasn't already been set.
            // It may have been set if we're cooking a transitory
            // file, for example when a .fnt results in cooking a
            // .gimg. In cases like this, we want to keep the Image
            // cooker version, and not override it with the Font
            // coooker version.
            if (res.pCookedBuffer->cookerVersion() == 0)
                res.pCookedBuffer->setCookerVersion(pCi->cooker().version());
        }
    }
}

void Chef::forceCookAndWrite(CookInfo * pCi)
{
    forceCook(pCi);

    // make any directories needed in cookedPath (use first cookResult path, all are the same)
    ChefString cookedDir = parent_dir(pCi->results().front().cookedPath);
    make_dirs(cookedDir.c_str());

    // Set versions in each cooked AssetHeader
    for (const CookResult & res : pCi->results())
    {
        if (res.isCooked())
        {
            // write out file
            {
                FileWriter wrtr(res.cookedPath.c_str());
                wrtr.ofs.write((const char *)res.pCookedBuffer.get(), res.cookedBufferSize);
            }

            writeDependencyFile(*pCi);
            printf("Cooked: %s -> %s\n", pCi->rawPath().c_str(), res.cookedPath.c_str());
        }
    }
}

UniquePtr<CookInfo> Chef::prepCookInfo(const char * rawPath, bool force)
{
    ChefString rawPathStr(rawPath);
    ASSERT(isRawPath(rawPathStr));

    const Cooker * pCooker = CookerRegistry::find_cooker_from_raw(rawPathStr);
    if (!pCooker)
    {
        // not a cookable file
        return UniquePtr<CookInfo>();
    }

    // check if file exists
    PANIC_IF(!file_exists(rawPathStr.c_str()), "Raw file does not exist: %s", rawPathStr.c_str());

    RecipeListUP pRecipes = findRecipes(rawPathStr);
    RecipeUP pFullRecipe = overlayRecipes(*pRecipes);

    UniquePtr<CookInfo> pCi(GNEW(kMEM_Chef, CookInfo, this, pCooker, force, rawPathStr, pRecipes, pFullRecipe));

    for (const ChefString & cookedExt : pCooker->cookedExts())
    {
        pCi->addCookResult(cookedExt,
                           getCookedPath(rawPathStr, cookedExt),
                           getGamePath(rawPathStr, cookedExt));
    }

    return pCi;
}

bool Chef::shouldCook(const CookInfo & ci)
{
    if (ci.force())
        return true;

    // If this is a dependent file, we don't cook it as an individual
    // file, but let the asset that is its parent cook it.
    if (ci.fullRecipe().getBool("is_dependent"))
        return false;

    // shouldCook if any cooked path doesn't exist
    // While looping, find the oldest cooked path to use in comparisons below.
    const ChefString * pOldestCookedPath = nullptr;
    for (const CookResult & res : ci.results())
    {
        if (!file_exists(res.cookedPath.c_str()))
        {
            return true;
        }
        else
        {
            // shouldCook if cooked version exists but is cooked with an older version of the cooker

            // Find the result cooker, which may be different from the
            // raw cooker. For example, in the case of .fnt cooking, the
            // results are .gimg and .gatl. We want to use the cooker
            // versions for the .gimg and .gatl cookers rather than
            // the cooker version of the .fnt.
            const Cooker * pResultCooker = CookerRegistry::find_cooker_from_cooked(res.cookedPath);
            PANIC_IF(!pResultCooker, "No cooker registered exclusively for this cooked extension: %s", res.cookedPath.c_str());
        
            FileReader fr(res.cookedPath.c_str());
            AssetHeader ah(0, 0);
            fr.read(&ah, sizeof(AssetHeader));
            if (ah.chefVersion() < kChefVersion ||
                ah.cookerVersion() < pResultCooker->version())
            {
                return true;
            }
            else if (ah.chefVersion() > kChefVersion ||
                     ah.cookerVersion() > pResultCooker->version())
            {
                ERR("Cooked asset was cooked with newer version than chef.exe; are you sure you have latest code?: %s", res.cookedPath.c_str());
                return false;
            }
        }
        if (pOldestCookedPath == nullptr)
            pOldestCookedPath = &res.cookedPath;
        else if (is_file_newer(pOldestCookedPath->c_str(), res.cookedPath.c_str()))
            pOldestCookedPath = &res.cookedPath;
    }            
            
    // shouldCook if raw path is newer than the oldest cooked path
    if (is_file_newer(ci.rawPath().c_str(), pOldestCookedPath->c_str()))
        return true;

    
    // shouldCook if any recipe is newer than the oldest cooked path
    for (const ChefString & recipePath : ci.recipes())
    {
        if (is_file_newer(recipePath.c_str(), pOldestCookedPath->c_str()))
            return true;
    }


    // shouldCook if any dependency is newer than the oldest cooked path
    auto deps = readDependencyFile(ci.rawPath());
    for (auto dep : deps)
    {
        ChefString depRawPath = getRelativeDependencyRawPath(ci.rawPath(), dep);
        UniquePtr<CookInfo> pDepCi = prepCookInfo(depRawPath.c_str(), false);
        if (pDepCi.get() && shouldCook(*pDepCi))
            return true;
        else if (is_file_newer(depRawPath.c_str(), pOldestCookedPath->c_str()))
            return true;
    }

    return false;
}



bool Chef::isRawPath(const ChefString & path)
{
    return is_parent_dir(mAssetsRawDir, path) ||
           is_parent_dir(mAssetsRawTransDir, path);
}

bool Chef::isCookedPath(const ChefString & path)
{
    return is_parent_dir(mAssetsCookedDir, path);
}

bool Chef::isGamePath(const ChefString & path)
{
    return (path.size() > 0 &&
            path[0] == '/' &&
            !isRawPath(path) &&
            !isCookedPath(path));
}

ChefString Chef::getRawPath(const ChefString & path)
{
    ASSERT(path.size() > 0);

    ChefString pathNorm = normalize_path(path);

    // Only valid raw paths should be sent to this function
    ASSERT(isRawPath(pathNorm));
    
    return pathNorm;
}

ChefString Chef::getRawRelativePath(const ChefString & rawPath)
{
    size_t stripSize = 0;
    if (is_parent_dir(mAssetsRawDir, rawPath))
    {
        stripSize = mAssetsRawDir.size();
    }
    else if (is_parent_dir(mAssetsRawTransDir, rawPath))
    {
        stripSize = mAssetsRawTransDir.size();
    }
    else
    {
        PANIC("Not a raw path: %s", rawPath.c_str());
    }
    return ChefString(rawPath, stripSize + 1, ChefString::npos);
}

ChefString Chef::getRawTransPath(const ChefString & rawPath, const ChefString & transExt)
{
    ASSERT(isRawPath(rawPath));

    ChefString transPath(mAssetsRawTransDir);
    transPath += '/';
    transPath += getRawRelativePath(rawPath);
    change_ext(transPath, transExt);
    return transPath;
}

ChefString Chef::getCookedPath(const ChefString & rawPath, const ChefString & cookedExt)
{
    ASSERT(isRawPath(rawPath));

    ChefString cookedPath(mAssetsCookedDir);
    cookedPath += '/';
    cookedPath += getRawRelativePath(rawPath);
    change_ext(cookedPath, cookedExt);
    return cookedPath;
}

ChefString Chef::getGamePath(const ChefString & rawPath, const ChefString & cookedExt)
{
    ASSERT(isRawPath(rawPath));

    ChefString gamePath("/"); // game paths always start with a '/'
    gamePath += getRawRelativePath(rawPath);
    change_ext(gamePath, cookedExt);
    return gamePath;
}

ChefString Chef::getRelativeDependencyRawPath(const ChefString & sourceRawPath, const ChefString & dependencyPath)
{
    ASSERT(isRawPath(sourceRawPath));

    // assume it's relative to sourceRawPaths' directory
    ChefString dependencyRawPath = parent_dir(sourceRawPath);
    dependencyRawPath += '/';
    dependencyRawPath += normalize_path(dependencyPath);
    return dependencyRawPath;
}

ChefString Chef::getDependencyFilePath(const ChefString & rawPath)
{
    ASSERT(isRawPath(rawPath));
    return rawPath + ".deps";
}

void Chef::deleteDependencyFile(const ChefString & rawPath)
{
    ASSERT(isRawPath(rawPath));

    ChefString depFilePath = getDependencyFilePath(rawPath);

    if (file_exists(depFilePath.c_str()))
        delete_file(depFilePath.c_str());
}

void Chef::writeDependencyFile(const CookInfo & ci)
{
	if (ci.dependencies().size() > 0)
	{
        ChefString depFilePath = getDependencyFilePath(ci.rawPath());

        Config<kMEM_Chef> depConf;
        for (const DependencyInfo & dep : ci.dependencies())
        {
            depConf.setValueless(dep.relativePath.c_str());
        }

        depConf.write(depFilePath.c_str());
	}
	else
    {
        // delete dependency file if it exists
		deleteDependencyFile(ci.rawPath());
    }
}

List<kMEM_Chef, ChefString> Chef::readDependencyFile(const ChefString & rawPath)
{
    ASSERT(isRawPath(rawPath));

    ChefString depFilePath = getDependencyFilePath(rawPath);

    List<kMEM_Chef, ChefString> deps;

    if (file_exists(depFilePath.c_str()))
    {
        Config<kMEM_Chef> conf;
        conf.read(depFilePath.c_str());

        for (auto keyIt = conf.keysBegin(); keyIt != conf.keysEnd(); ++keyIt)
        {
            deps.emplace_back(*keyIt);
        }
    }

    return deps;
}

RecipeListUP Chef::findRecipes(const ChefString & rawPath)
{
    ASSERT(isRawPath(rawPath));

    static const char * kRcpExt = ".rcp";

    RecipeListUP pRecipes(GNEW(kMEM_Chef, RecipeList));

    ChefString ext = get_ext(rawPath.c_str());
    ChefString dir = parent_dir(rawPath);

    ChefString rcpFile = rawPath + kRcpExt;
    if (file_exists(rcpFile.c_str()))
        pRecipes->push_front(rcpFile);

    while (is_parent_dir(mAssetsRawDir, dir))
    {
        // check for file type override, e.g. .tga.rcp
        ChefString rcpFile = dir + "/" + ext + kRcpExt;
        if (file_exists(rcpFile.c_str()))
            pRecipes->push_front(rcpFile);

        // check for directory override, e.g. .rcp
        rcpFile = dir + "/" + kRcpExt;
        if (file_exists(rcpFile.c_str()))
            pRecipes->push_front(rcpFile);

        // move on to parent directory
        parent_dir(dir);
    }

    return pRecipes;
}

RecipeUP Chef::overlayRecipes(const RecipeList & recipes)
{
    RecipeUP pRecipe(GNEW(kMEM_Chef, Recipe));
    for (ChefString rcp : recipes)
    {
        if (!pRecipe->read(rcp.c_str()))
            PANIC("Failure reading recipe: %s", rcp.c_str());
    }
    return pRecipe;
}


} // namespace gaen
