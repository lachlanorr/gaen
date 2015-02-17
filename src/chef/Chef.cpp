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

#include <string>
#include <set>

#include "core/base_defines.h"
#include "core/thread_local.h"
#include "core/platutils.h"

#include "chef/cookers.h"
#include "chef/Chef.h"

namespace gaen
{

Chef::Chef()
{
    register_cookers();

    // LORRTODO - For projects, include a #ifdef block here that
    // optionally calls the project's register_cookers function as
    // well.
}

Chef::~Chef()
{
    for (auto it : mRawExtToCooker)
        delete it.second;

    // don't need to delete from mCookedExtToCooker since it contains
    // the same items we just deleted.
}

void Chef::cook(const char * platform, const char * rawFile)
{
    return singleton<Chef>().cookAsset(platform, rawFile);
}

void Chef::prep_paths(char * inFile, char * outFile, const char * platform, const char * rawFile, Cooker * pCooker)
{
    const char * ext = get_ext(rawFile);

    // normalize raw_file
    const char * p = rawFile;
    char * pi = inFile;

    // copy the path, replacing '\' with '/'
    while (*p)
    {
        if (*p == '\\')
            *pi = '/';
        else
            *pi = *p;
        ++p;
        ++pi;

        if (p >= ext)
            break; // stop at the 'dot' of the extension
    }
    *pi = '\0';

    // ensure we have an '/assets/raw/' directory in path
    const char * kRawAssetsDir = "/assets/raw/";
    const char * kCookedAssetsDir = "/assets/cooked/";
    const char * assetsRaw = strstr(inFile, kRawAssetsDir);
    if (!assetsRaw)
    {
        PANIC("Raw asset path is not contained within the assets/raw directory: %s", rawFile);
        return;
    }

    // make outFile, replacing '/assets/raw/' with '/assets/cooked/'
    strncpy(outFile, inFile, assetsRaw - inFile);
    strcat(outFile, "/assets/cooked_");
    strcat(outFile, platform);
    strcat(outFile, "/");
    strcat(outFile, assetsRaw + strlen(kRawAssetsDir));

    // now place our cooker's extensions on each file
    strcat(inFile, pCooker->rawExt());
    strcat(outFile, pCooker->cookedExt());
}

void Chef::cookAsset(const char * platform, const char * rawFile)
{
    if (strlen(rawFile) > kMaxFilePath-1)
    {
        PANIC("File path too long, max size allowed: %u, %s", kMaxFilePath-1, rawFile);
    }

    // prepare input and output paths
    TLARRAY(char, inFile, kMaxFilePath);
    TLARRAY(char, outFile, kMaxFilePath);
    TLARRAY(char, outDir, kMaxFilePath);

    // find our cooker
    const char * ext = get_ext(rawFile);
    auto it = mRawExtToCooker.find(ext);
    if (it == mRawExtToCooker.end())
    {
        PANIC("No cooker registered for raw extension: %s", ext);
        return;
    }
    Cooker * pCooker = it->second;

    prep_paths(inFile, outFile, platform, rawFile, pCooker);

    // check if file exists
    if (!file_exists(inFile))
    {
        PANIC("Raw file does not exist: %s", inFile);
        return;
    }

    // make any directories needed in outFile
    parent_directory(outDir, outFile);
    make_directories(outDir);

    // open the input and output streams
    std::ifstream ifs;
    ifs.open(inFile, std::ifstream::in | std::ifstream::binary);
    if (!ifs.good())
    {
        PANIC("Unable to open raw asset for reading: %s", inFile);
        return;
    }

    std::ofstream ofs;
    ofs.open(outFile, std::ifstream::out | std::ifstream::binary);
    if (!ofs.good())
    {
        PANIC("Unable to open cooked asset for writing: %s", outFile);
        return;
    }

    pCooker->cook(platform, ifs, ofs);

    ifs.close();
    ofs.close();
    
}

bool Chef::register_cooker(Cooker * pCooker)
{
    return singleton<Chef>().registerCooker(pCooker);
}

bool Chef::registerCooker(Cooker * pCooker)
{
    if (mRawExtToCooker.find(pCooker->rawExt()) != mRawExtToCooker.end())
    {
        PANIC("Multiple cookers registered for same raw extension: %s", pCooker->rawExt());
        return false;
    }
    if (mCookedExtToCooker.find(pCooker->cookedExt()) != mCookedExtToCooker.end())
    {
        PANIC("Multiple cookers registered for same cooked extension: %s", pCooker->cookedExt());
        return false;
    }

    mRawExtToCooker[pCooker->rawExt()] = pCooker;
    mCookedExtToCooker[pCooker->cookedExt()] = pCooker;

    return true;
}

bool Chef::is_valid_platform(const char * platform)
{
    static std::set<std::string> sPlatforms ={"win", "osx", "ios"};

    return sPlatforms.count(platform) > 0;
}

const char * Chef::get_ext(const char * file)
{
    const char * dotpos = strrchr(file, '.');
    if (!dotpos || !dotpos[1])
    {
        PANIC("Input file has no extension: %s", file);
        return nullptr;
    }
    return dotpos+1;
}


} // namespace gaen
