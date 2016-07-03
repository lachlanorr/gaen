//------------------------------------------------------------------------------
// file_utils.cpp - Various helper routines for file system activities
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

#include "core/mem.h"

#include "file_utils.h"

namespace gaen
{

void parent_dir(char * dirPath, const char * filePath)
{
    normalize_path(dirPath, filePath);
    parent_dir(dirPath);
}

void parent_dir(char * path)
{
    char * lastSep = strrchr(path, '/');
    if (!lastSep)
        path[0] = '\0';
    else
        *lastSep = '\0';
}


const char * get_ext(const char * path)
{
    ASSERT(path);
    const char * dotpos = strrchr(path, '.');
    PANIC_IF(!dotpos || !dotpos[1], "Input file has no extension: %s");
    return dotpos+1;
}

char * get_ext(char * path)
{
    ASSERT(path);
    char * dotpos = strrchr(path, '.');
    PANIC_IF(!dotpos || !dotpos[1], "Input file has no extension: %s");
    return dotpos+1;
}

void strip_ext(char * path)
{
    char * dotpos = strrchr(path, '.');
    if (dotpos)
        *dotpos = '\0';
}

void normalize_path(char * outPath, const char * inPath)
{
    ASSERT(outPath);
    ASSERT(inPath);

    const char * pIn = inPath;
    char * pOut = outPath;
    
    // copy the path, replacing '\' with '/'
    while (*pIn)
    {
        if (*pIn == '\\')
            *pOut = '/';
        else
            *pOut = *pIn;
        ++pIn;
        ++pOut;
    }
    *pOut = '\0';

    // strip of trailing '/' if present
    if (pOut > outPath && pOut[-1] == '/')
        pOut[-1] = '\0';
}

void normalize_path(char * path)
{
    ASSERT(path);
    char * p = path;
    while (*p)
    {
        if (*p == '\\')
            *p = '/';
        p++;
    }

    // strip of trailing '/' if present
    if (p > path && p[-1] == '/')
        p[-1] = '\0';
}

void append_path(char * path, const char * append)
{
    ASSERT(path);
    ASSERT(append);
    if (*path)
    {
        size_t pathLen = strlen(path);
        if (path[pathLen-1] != '/')
            strcat(path, "/");
        strcat(path, append);
    }
    else
    {
        strcpy(path, append);
    }
}

void change_ext(char * path, const char * ext)
{
    ASSERT(path);
    ASSERT(ext);
    char * extPos = get_ext(path);
    strcpy(extPos, ext);
}

const char * get_filename(const char * path)
{
    ASSERT(path);
    const char * lastSlash = strrchr(path, '/');
    if (!lastSlash)
    {
        return path;
    }
    else
    {
        return lastSlash + 1;
    }
}

void get_filename(char * filename, const char * path)
{
    ASSERT(filename);
    ASSERT(path);
    const char * lastSlash = strrchr(path, '/');
    if (!lastSlash)
    {
        strcpy(filename, path);
    }
    else
    {
        strcpy(filename, lastSlash + 1);
    }
}

void get_filename_root(char * filename, const char * path)
{
    ASSERT(filename);
    ASSERT(path);
    const char * lastSlash = strrchr(path, '/');
    if (!lastSlash)
    {
        strcpy(filename, path);
    }
    else
    {
        strcpy(filename, lastSlash + 1);
    }

    char * lastdot = const_cast<char*>(strrchr(filename, '.'));
    if (lastdot)
        *lastdot = '\0';
}

void upper(char * str)
{
    ASSERT(str);
    char * c = str;
    while (*c)
    {
        if (*c >= 'a' && *c <= 'z')
            *c = 'A' + (*c - 'a');
        c++;
    }
}

//------------------------------------------------------------------------------
// Asset cooking path manipulation
//------------------------------------------------------------------------------
const char * default_platform()
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

bool is_valid_platform(const char * platform)
{
    ASSERT(platform);
    return (0 == strcmp(platform, "win") ||
            0 == strcmp(platform, "osx") ||
            0 == strcmp(platform, "ios"));
}

void assets_raw_dir(char * assetsRawDir, const char * assetsDir)
{
    ASSERT(assetsRawDir);
    ASSERT(assetsDir);
    normalize_path(assetsRawDir, assetsDir);
    strcat(assetsRawDir, "/raw");
}

void assets_cooked_dir(char * assetsCookedDir, const char * platform, const char * assetsDir)
{
    ASSERT(assetsCookedDir);
    ASSERT(platform);
    ASSERT(assetsDir);
    ASSERT(is_valid_platform(platform));
    normalize_path(assetsCookedDir, assetsDir);
    strcat(assetsCookedDir, "/cooked_");
    strcat(assetsCookedDir, platform);
}

void find_assets_cooking_dir(char * assetsDir)
{
    char path[kMaxPath+1];
    char checkPath[kMaxPath+1];

    process_path(path);
    parent_dir(path);

    for(;;)
    {
        PANIC_IF(!*path, "Unable to find assets dir, make sure chef executable is located under a gaen or gaen project directory tree.");

        snprintf(checkPath, kMaxPath, "%s/assets/raw/", path);
        if (dir_exists(checkPath))
        {
            // get rid of /raw/ at end
            size_t checkPathLen = strlen(checkPath);
            checkPath[checkPathLen - 5] = '\0';
            strcpy(assetsDir, checkPath);
            return;
        }
        parent_dir(path);
    }
}

void find_assets_runtime_dir(char * assetsDir)
{
    char path[kMaxPath+1];
    char checkPath[kMaxPath+1];
    char assetToFind[kMaxPath+1];

    // There should always be profont, since our engine
    // depends on it for some debugging output.
    const char * kAssetToFind = "/gaen/fonts/profont.gatl";
    strcpy(assetToFind, kAssetToFind);

    // Get current process path
    process_path(path);
    parent_dir(path);

    // By default, check if it looks like our process is
    // within the assets directory, which should be true
    // in a packaged release.
    snprintf(checkPath, kMaxPath, "%s%s", path, assetToFind);
    if (file_exists(checkPath))
    {
        strcpy(assetsDir, path);
        return;
    }

    // If we haven't found the assets, start looking in parent
    // directories for the cooked_<platform> based file path. This would be the case for a development machine
    // where gaen.exe exists in somewhere in the build hierarchy.
    snprintf(assetToFind, kMaxPath, "/assets/cooked_%s%s", default_platform(), kAssetToFind);

    for(;;)
    {
        PANIC_IF(!*path, "Unable to find assets dir, make sure assets are cooked.");

        snprintf(checkPath, kMaxPath, "%s%s", path, assetToFind);
        if (file_exists(checkPath))
        {
            // get rid of kAssetToFind at end
            size_t checkPathLen = strlen(checkPath);
            checkPath[checkPathLen - strlen(kAssetToFind)] = '\0';
            strcpy(assetsDir, checkPath);
            return;
        }
        parent_dir(path);
    }
}

//------------------------------------------------------------------------------
// Asset cooking path manipulation (END)
//------------------------------------------------------------------------------


bool write_file_if_contents_differ(const char * path, const char * contents)
{
    ASSERT(path);
    ASSERT(contents);

    FileReader rdr(path);
    Scoped_GFREE<char> oldContents((char*)GALLOC(kMEM_Renderer, rdr.size()+1)); // +1 for null we'll add to end
    rdr.read(oldContents.get(), rdr.size());
    oldContents.get()[rdr.size()] = '\0';
    rdr.ifs.close();

    if (strcmp(contents, oldContents.get()) != 0)
    {
        FileWriter wrt(path);
        wrt.ofs.write(contents, strlen(contents));
        return true;
    }
    return false;
}

} // namespace gaen

