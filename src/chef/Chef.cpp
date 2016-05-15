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

Chef::Chef(u32 id, const char * platform, const char * assetsDir, bool force, DependencyCB dependencyCB)
  : mId(id)
  , mForce(force)
  , mDependencyCB(dependencyCB)
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

const char * Chef::default_platform()
{
#if IS_PLATFORM_WIN32
    return "win";
#elif IS_PLATFORM_OSX
    return "osx";
#elif IS_PLATFORM_IOS
    return "ios";
#else
#error Invalid platform for cooking, no default
#endif
}

bool Chef::is_valid_platform(const char * platform)
{
    ASSERT(platform);
    return (0 == strcmp(platform, "win") ||
            0 == strcmp(platform, "osx") ||
            0 == strcmp(platform, "ios"));
}

void Chef::assets_raw_dir(char * assetsRawDir, const char * assetsDir)
{
    ASSERT(assetsRawDir);
    ASSERT(assetsDir);
    normalize_path(assetsRawDir, assetsDir);
    strcat(assetsRawDir, "/raw");
}

void Chef::assets_cooked_dir(char * assetsCookedDir, const char * platform, const char * assetsDir)
{
    ASSERT(assetsCookedDir);
    ASSERT(platform);
    ASSERT(assetsDir);
    ASSERT(is_valid_platform(platform));
    normalize_path(assetsCookedDir, assetsDir);
    strcat(assetsCookedDir, "/cooked_");
    strcat(assetsCookedDir, platform);
}

void Chef::cook(const char * path)
{
    ASSERT(path);
    PANIC_IF(strlen(path) > kMaxPath-1, "File path too long, max size allowed: %u, %s", kMaxPath-1, path);

    // prepare input and output paths
    char rawPath[kMaxPath+1];
    char cookedPath[kMaxPath+1];

    getRawPath(rawPath, path);
    Cooker * pCooker = CookerRegistry::find_cooker_from_raw(rawPath);
    if (!pCooker)
    {
        // not a cookable file
        return;
    }
    getCookedPath(cookedPath, path);

    // check if file exists
    PANIC_IF(!file_exists(rawPath), "Raw file does not exist: %s", rawPath);

    RecipeList recipes = findRecipes(rawPath);

    // verify we should cook
    if (!shouldCook(rawPath, cookedPath, recipes) && !mForce)
        return;

    // make any directories needed in cookedPath
    char cookedDir[kMaxPath+1];
    parent_dir(cookedDir, cookedPath);
    make_dirs(cookedDir);

    Config<kMEM_Chef> recipe;
    overlayRecipes(recipe, recipes);

    CookInfo ci(this, rawPath, cookedPath, recipe);

    pCooker->cook(ci);

    printf("Cooked: %s -> %s\n", ci.rawPath, ci.cookedPath);
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

void Chef::getDependencyFilePath(char * dependencyFilePath, const char * path)
{
    ASSERT(dependencyFilePath);
    ASSERT(path);

    strcpy(dependencyFilePath, path);
    strcat(dependencyFilePath, ".dep");
}

void Chef::deleteDependencyFile(const char * path)
{
    ASSERT(path);

    char depFilePath[kMaxPath+1];

    getDependencyFilePath(depFilePath, path);

    if (file_exists(depFilePath))
        delete_file(depFilePath);
}

void Chef::reportDependency(char * dependencyRawPath, const char * sourceRawPath, const char * dependencyPath)
{
    // If passed in an output path, use it, otherwise us a scratch space
    char scratch[kMaxPath+1];
    char * depRawPath = dependencyRawPath ? dependencyRawPath : scratch;

    if (!convertRelativeDependencyPath(depRawPath, sourceRawPath, dependencyPath))
    {
        PANIC("Unable to convert dependency relative path to raw path: %s", dependencyPath);
    }

    // We only want to record the portion relative to the 
    // raw assets directory.
    char rawRelativePath[kMaxPath+1];
    getRawRelativePath(rawRelativePath, depRawPath);

    char depFilePath[kMaxPath+1];
    getDependencyFilePath(depFilePath, sourceRawPath);

    if (file_exists(depFilePath))
    {
        Config<kMEM_Chef> deps;
        deps.read(depFilePath);

        //deps.

    }

    mDependencyCB(mId, sourceRawPath, rawRelativePath);
}

bool Chef::shouldCook(const char * rawPath, const char * cookedPath, const RecipeList & recipes)
{
    if (!file_exists(cookedPath))
        return true;

    if (is_file_newer(rawPath, cookedPath))
        return true;

    for (const String<kMEM_Chef> & recipePath : recipes)
    {
        if (is_file_newer(recipePath.c_str(), cookedPath))
            return true;
    }

    char depFile[kMaxPath + 1];
    getDependencyFilePath(depFile, rawPath);
    if (file_exists(depFile))
    {
        printf("TODO: Check each dependent file to see if it has changed.\n");
    }

    return false;
}

Chef::RecipeList Chef::findRecipes(const char * rawPath)
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

void Chef::overlayRecipes(Config<kMEM_Chef> & recipe, const Chef::RecipeList & recipes)
{
    for (String<kMEM_Chef> rcp : recipes)
    {
        if (!recipe.read(rcp.c_str()))
            PANIC("Failure reading recipe: %s", rcp);
    }
}


} // namespace gaen
