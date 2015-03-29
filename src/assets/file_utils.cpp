//------------------------------------------------------------------------------
// file_utils.cpp - Various helper reoutines for file system activities
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

} // namespace gaen

