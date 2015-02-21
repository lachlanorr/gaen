//------------------------------------------------------------------------------
// Chef.cpp - Class to manage asset cookers
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

#include "core/base_defines.h"
#include "core/thread_local.h"
#include "core/platutils.h"

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

    // verify we should cook
    if (!shouldCook(rawPath, cookedPath) && !mForce)
        return;

    // make any directories needed in cookedPath
    char cookedDir[kMaxPath+1];
    parent_directory(cookedDir, cookedPath);
    make_directories(cookedDir);

    CookInfo ci(this, rawPath, cookedPath);

    // open the input and output streams
    ci.ifs.open(rawPath, std::ifstream::in | std::ifstream::binary);
    if (!ci.ifs.good())
    {
        PANIC("Unable to open raw asset for reading: %s", rawPath);
        return;
    }

    ci.ofs.open(cookedPath, std::ifstream::out | std::ifstream::binary);
    if (!ci.ofs.good())
    {
        PANIC("Unable to open cooked asset for writing: %s", cookedPath);
        return;
    }

    pCooker->cook(&ci);

    ci.ifs.close();
    ci.ofs.close();
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



void Chef::recordDependency(const char * assetRawPath, const char * dependencyPath)
{
    ASSERT(assetRawPath);
    ASSERT(dependencyPath);
    ASSERT(isRawPath(assetRawPath));
    
    char normDepPath[kMaxPath+1];
    char depRawPath[kMaxPath+1];

    normalize_path(normDepPath, dependencyPath);
    
    if (isRawPath(normDepPath))
    {
        mDependencyCB(mId, assetRawPath, normDepPath);
    }
    else if (isGamePath(normDepPath) || isCookedPath(normDepPath))
    {
        getRawPath(depRawPath, normDepPath);
        mDependencyCB(mId, assetRawPath, depRawPath);
    }
    else if (normDepPath[0] != '/')
    {
        // assume it's relative to assetRawPaths' directory
        parent_directory(depRawPath, assetRawPath);
        strcat(depRawPath, "/");
        strcat(depRawPath, normDepPath);
        mDependencyCB(mId, assetRawPath, depRawPath);
    }
    else
    {
        PANIC("Invalid dependencyPath: %s", dependencyPath);
    }
}

bool Chef::shouldCook(const char * rawPath, const char * cookedPath)
{
    if (!file_exists(cookedPath))
        return true;

    if (is_file_newer(rawPath, cookedPath))
        return true;

    return false;
}


} // namespace gaen
