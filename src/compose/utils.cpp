//------------------------------------------------------------------------------
// utils.cpp - Compiler utility functions
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "core/base_defines.h"

#include "compose/compiler.h"

#include "compose/utils.h"

#if IS_PLATFORM_WIN32
#include <windows.h>
#endif

namespace gaen
{


const char * path_join(const char * rootDir,
                       const char * filename,
                       ParseData *pParseData)
{
    ASSERT(rootDir);
    ASSERT(filename);
    
    static thread_local char fullPath[FILENAME_MAX];

    size_t rootLen = strlen(rootDir);
    size_t filenameLen = strlen(filename);

    if (rootLen + filenameLen + 2 >= FILENAME_MAX)
        COMP_ERROR(pParseData, "Filename too long");

    char * dest = fullPath;

    strcpy(dest, rootDir);
    dest += rootLen;

    if (*(dest-1) != '/')
    {
        *dest = '/';
        dest++;
    }

    strcpy(dest, filename);
    dest += rootLen;
    *dest = '\0';

    return parsedata_add_string(pParseData, fullPath);
}


const char * path_filename(const char * fullPath,
                           ParseData *pParseData)
{
    const char * lastSlash = strrchr(fullPath,'/');
    if (!lastSlash)
    {
        return parsedata_add_string(pParseData, fullPath);
    }
    else
    {
        if (strlen(lastSlash) <= 1)
        {
            COMP_ERROR(pParseData, "fullPath ends with '/', that's no bueno!");
            return nullptr;
        }
        return parsedata_add_string(pParseData, lastSlash+1);
    }
}


const char * full_path(const char * path,
                       ParseData* pParseData)
{
    ASSERT(path);
    static thread_local char fullPath[FILENAME_MAX];
#if IS_PLATFORM_WIN32
    DWORD ret = GetFullPathNameA(path, FILENAME_MAX, fullPath, nullptr);

    if (ret >= FILENAME_MAX)
    {
        COMP_ERROR(pParseData, "Path too long");
        return nullptr;
    }

    make_posix_path(fullPath);

    return parsedata_add_string(pParseData, fullPath);
#else
    if (!realpath(path, fullPath))
    {
        COMP_ERROR(pParseData, "Failed to find the realpath of %s", path);
        return nullptr;
    }

    return parsedata_add_string(pParseData, fullPath);
#endif
        
}

void make_posix_path(char * path)
{
    ASSERT(path);
    char * p = path;
    while ((p = strchr(p, '\\')))
        *p = '/';
}


}
