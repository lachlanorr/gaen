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

#include "chef/CookerRegistry.h"
#include "chef/Chef.h"

namespace gaen
{

Chef::Chef(u32 id, const char * platform, const char * assetsDir, bool force)
  : mId(id)
  , mForce(force)
{
    ASSERT(platform);
    ASSERT(assetsDir);

    mPlatform = platform;
    
    char scratch[kMaxPath+1];
    normalize_path(scratch, assetsDir);
    mAssetsDir = scratch;

    assets_raw_dir(scratch, mAssetsDir.c_str());
    mAssetsRawDir = scratch;

    assets_cooked_dir(scratch, platform, mAssetsDir.c_str());
    mAssetsCookedDir = scratch;
}

UniquePtr<CookInfo> Chef::cook(const char * path, CookFlags flags)
{
    ASSERT(path);
    PANIC_IF(strlen(path) > kMaxPath-1, "File path too long, max size allowed: %u, %s", kMaxPath-1, path);

    char rawPath[kMaxPath+1];
    getRawPath(rawPath, path);

    Cooker * pCooker = CookerRegistry::find_cooker_from_raw(rawPath);
    if (!pCooker)
    {
        // not a cookable file
        return UniquePtr<CookInfo>();
    }

    char cookedPath[kMaxPath+1];
    getCookedPath(cookedPath, rawPath);

    // check if file exists
    PANIC_IF(!file_exists(rawPath), "Raw file does not exist: %s", rawPath);

    RecipeList recipes = findRecipes(rawPath);
    UniquePtr<CookInfo> pCi(GNEW(kMEM_Chef, CookInfo, this, flags, rawPath, cookedPath, recipes));

    // verify we should cook
    if (!shouldCook(*pCi, recipes, mForce))
        return pCi;

    // make any directories needed in cookedPath
    char cookedDir[kMaxPath+1];
    parent_dir(cookedDir, cookedPath);
    make_dirs(cookedDir);

    pCooker->cook(*pCi);

    return pCi;
}

UniquePtr<CookInfo> Chef::cookDependency(const char * path)
{
    return cook(path, kCF_CookingDependency);
}

void Chef::cookAndWrite(const char * path)
{
    UniquePtr<CookInfo> pCi(cook(path, kCF_None));

    if (pCi.get() && pCi->isCooked())
    {
        writeDependencyFile(*pCi);

        if (pCi->isCooked())
        {
            // write out file
            FileWriter wrtr(pCi->cookedPath());
            wrtr.ofs.write((const char *)pCi->cookedBuffer(), pCi->cookedBufferSize());
            printf("Cooked: %s -> %s\n", pCi->rawPath(), pCi->cookedPath());
        }
    }
}

bool Chef::isRawPath(const char * path)
{
    ASSERT(path);
    return 0 == strncmp(path, mAssetsRawDir.c_str(), mAssetsRawDir.size());
}

bool Chef::isCookedPath(const char * path)
{
    ASSERT(path);
    return 0 == strncmp(path, mAssetsCookedDir.c_str(), mAssetsCookedDir.size());
}

bool Chef::isGamePath(const char * path)
{
    ASSERT(path);
    return (*path == '/' &&
            !isRawPath(path) &&
            !isCookedPath(path));
}

void Chef::getRawPath(char * rawPath, const char * path, Cooker * pCooker)
{
    ASSERT(rawPath);
    ASSERT(path);
    
    char pathNorm[kMaxPath+1];
    normalize_path(pathNorm, path);

    if (isCookedPath(pathNorm))
    {
        strcpy(rawPath, mAssetsRawDir.c_str());
        strcat(rawPath, pathNorm + mAssetsCookedDir.size());
        if (!pCooker) pCooker = CookerRegistry::find_cooker_from_cooked(pathNorm);
        PANIC_IF(!pCooker, "No registered cooker for %s", pathNorm);
        change_ext(rawPath, pCooker->rawExt);
    }
    else if (isGamePath(pathNorm))
    {
        strcpy(rawPath, mAssetsRawDir.c_str());
        strcat(rawPath, pathNorm);
        if (!pCooker) pCooker = CookerRegistry::find_cooker_from_cooked(pathNorm);
        PANIC_IF(!pCooker, "No registered cooker for %s", pathNorm);
        change_ext(rawPath, pCooker->rawExt);
    }
    else if (isRawPath(pathNorm))
    {
        strcpy(rawPath, pathNorm);
    }
    else
    {
        PANIC("Invalid path: %s", path);
    }
}

void Chef::getRawRelativePath(char * rawRelativePath, const char * rawPath)
{
    ASSERT(rawRelativePath);
    ASSERT(rawPath);
    PANIC_IF(!isRawPath(rawPath), "Not a raw path: %s", rawPath);
    strcpy(rawRelativePath, rawPath + mAssetsRawDir.size() + 1);
}

void Chef::getCookedPath(char * cookedPath, const char * path, Cooker * pCooker)
{
    ASSERT(cookedPath);
    ASSERT(path);

    char pathNorm[kMaxPath+1];
    normalize_path(pathNorm, path);

    if (isRawPath(pathNorm))
    {
        strcpy(cookedPath, mAssetsCookedDir.c_str());
        strcat(cookedPath, pathNorm + mAssetsRawDir.size());
        if (!pCooker) pCooker = CookerRegistry::find_cooker_from_raw(pathNorm);
        PANIC_IF(!pCooker, "No registered cooker for %s", pathNorm);
        change_ext(cookedPath, pCooker->cookedExt);
    }
    else if (isGamePath(pathNorm))
    {
        if (!pCooker) pCooker = CookerRegistry::find_cooker_from_cooked(pathNorm);
        PANIC_IF(!pCooker, "No registered cooker for %s", pathNorm);
        strcpy(cookedPath, mAssetsCookedDir.c_str());
        strcat(cookedPath, pathNorm);
    }
    else if (isCookedPath(pathNorm))
    {
        strcpy(cookedPath, pathNorm);
    }
    else
    {
        PANIC("Invalid path: %s", path);
    }
}

void Chef::getGamePath(char * gamePath, const char * path, Cooker * pCooker)
{
    ASSERT(gamePath);
    ASSERT(path);

    char pathNorm[kMaxPath+1];
    normalize_path(pathNorm, path);

    if (isRawPath(pathNorm))
    {
        strcpy(gamePath, pathNorm + mAssetsRawDir.size());
        if (!pCooker) pCooker = CookerRegistry::find_cooker_from_raw(pathNorm);
        PANIC_IF(!pCooker, "No registered cooker for %s", pathNorm);
        change_ext(gamePath, pCooker->cookedExt);
    }
    else if (isCookedPath(pathNorm))
    {
        strcpy(gamePath, pathNorm + mAssetsCookedDir.size());
    }
    else if (isGamePath(pathNorm))
    {
        strcpy(gamePath, pathNorm);
    }
    else
    {
        PANIC("Invalid path: %s", path);
    }
}

bool Chef::convertRelativeDependencyPath(char * dependencyRawPath, const char * sourceRawPath, const char * dependencyPath)
{
    ASSERT(sourceRawPath);
    ASSERT(dependencyPath);
    ASSERT(isRawPath(sourceRawPath));

    char normDepPath[kMaxPath+1];

    normalize_path(normDepPath, dependencyPath);

    if (isRawPath(normDepPath))
    {
        strcpy(dependencyRawPath, normDepPath);
        return true;
    }
    else if (isGamePath(normDepPath) || isCookedPath(normDepPath))
    {
        getRawPath(dependencyRawPath, normDepPath);
        return true;
    }
    else if (normDepPath[0] != '/')
    {
        // assume it's relative to sourceRawPaths' directory
        parent_dir(dependencyRawPath, sourceRawPath);
        strcat(dependencyRawPath, "/");
        strcat(dependencyRawPath, normDepPath);
        return true;
    }
    return false;
}

void Chef::getDependencyFilePath(char * dependencyFilePath, const char * rawPath)
{
    ASSERT(dependencyFilePath);
    ASSERT(rawPath);

    strcpy(dependencyFilePath, rawPath);
    strcat(dependencyFilePath, ".deps");
}

void Chef::deleteDependencyFile(const char * rawPath)
{
    ASSERT(rawPath);

    char depFilePath[kMaxPath+1];

    getDependencyFilePath(depFilePath, rawPath);

    if (file_exists(depFilePath))
        delete_file(depFilePath);
}

void Chef::writeDependencyFile(const CookInfo & ci)
{
	if (ci.dependencies().size() > 0)
	{
        char depFilePath[kMaxPath + 1];
        getDependencyFilePath(depFilePath, ci.rawPath());

        Config<kMEM_Chef> depConf;
        for (const String<kMEM_Chef> & dep : ci.dependencies())
        {
            depConf.setValueless(dep.c_str());
        }

        depConf.write(depFilePath);
	}
	else
    {
        // delete dependency file if it exists
		deleteDependencyFile(ci.rawPath());
    }
}

List<kMEM_Chef, String<kMEM_Chef>> Chef::readDependencyFile(const char * rawPath)
{
    char depFilePath[kMaxPath + 1];
    getDependencyFilePath(depFilePath, rawPath);

    List<kMEM_Chef, String<kMEM_Chef>> deps;

    if (file_exists(depFilePath))
    {
        Config<kMEM_Chef> conf;
        conf.read(depFilePath);

        for (auto keyIt = conf.keysBegin(); keyIt != conf.keysEnd(); ++keyIt)
        {
            deps.emplace_back(*keyIt);
        }
    }

    return deps;
}

bool Chef::shouldCook(const CookInfo & ci, const RecipeList & recipes, bool force)
{
    // If this is a dependent file, we don't cook it as an individual
    // file, but let the asset that is its parent cook it.
    if ((ci.flags() & kCF_CookingDependency) == kCF_CookingDependency)
        return true;
    else if (ci.recipe().getBool("is_dependent"))
        return false;

    if (force)
        return true;

    if (!file_exists(ci.cookedPath()))
        return true;

    if (is_file_newer(ci.rawPath(), ci.cookedPath()))
        return true;

    for (const String<kMEM_Chef> & recipePath : recipes)
    {
        if (is_file_newer(recipePath.c_str(), ci.cookedPath()))
            return true;
    }

    auto deps = readDependencyFile(ci.rawPath());

    for (auto dep : deps)
    {
        char depRawPath[kMaxPath + 1];
        if (!convertRelativeDependencyPath(depRawPath, ci.rawPath(), dep.c_str()))
        {
            PANIC("Unable to convert dependency relative path to raw path: %s", dep.c_str());
        }
        if (is_file_newer(depRawPath, ci.cookedPath()))
            return true;
    }

    return false;
}

RecipeList Chef::findRecipes(const char * rawPath)
{
    RecipeList recipes;

    const char * ext = get_ext(rawPath);

    char dir[kMaxPath+1];
    normalize_path(dir, rawPath);
    parent_dir(dir);

    char rcpFile[kMaxPath+1];
    normalize_path(rcpFile, rawPath);
    strcat(rcpFile, ".rcp");
    if (file_exists(rcpFile))
        recipes.push_front(rcpFile);

    while (strstr(dir, mAssetsRawDir.c_str()) == dir)
    {
        // check for file type override, e.g. .tga.rcp
        sprintf(rcpFile, "%s/.%s.rcp", dir, ext);
        if (file_exists(rcpFile))
            recipes.push_front(rcpFile);

        // check for directory override, e.g. .rcp
        sprintf(rcpFile, "%s/.rcp", dir);
        if (file_exists(rcpFile))
            recipes.push_front(rcpFile);

        // move on to parent directory
        parent_dir(dir);
    }

    return recipes;
}

void Chef::overlayRecipes(Config<kMEM_Chef> & recipe, const RecipeList & recipes)
{
    for (String<kMEM_Chef> rcp : recipes)
    {
        if (!recipe.read(rcp.c_str()))
            PANIC("Failure reading recipe: %s", rcp);
    }
}


} // namespace gaen
