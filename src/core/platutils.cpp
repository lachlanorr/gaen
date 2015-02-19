//------------------------------------------------------------------------------
// platutils.cpp - Misc platform specific code
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

#include "core/stdafx.h"

#include <cstdio>

#include "core/mem.h"
#include "core/platutils.h"

namespace gaen
{

bool time_to_str(char * str, size_t strLen, f32 timeSecs)
{
    ASSERT(str);
    ASSERT(strLen > 0);
    ASSERT(timeSecs >= 0.0f);

    static const u32 kSecsPerMin  = 60;
    static const u32 kSecsPerHour = kSecsPerMin * 60;

    u32 secs = static_cast<u32>(timeSecs);

    u32 rem;

    u32 hours = secs / kSecsPerHour;
    rem = secs % kSecsPerHour;

    u32 mins = rem / kSecsPerMin;
    rem = rem % kSecsPerMin;

    f32 fSecs = rem + (timeSecs - secs);

    int ret = snprintf(str, strLen-1, "%02u:%02u:%09.6f", hours, mins, fSecs);
    str[strLen-1] = '\0';

    // snprintf tells us if we could insert the whole string
    return ret > 0 && ret < static_cast<int>(strLen-1);
}


i32 read_file(const char * path, char ** output)
{
    *output = nullptr;
    i32 length = 0;
    char * source = nullptr;
    FILE *fp = fopen(path, "r");

    if (!fp)
    {
        ERR("Failed to read file: %s", path);
        return -1;
    }

    // go to end of file
    if (fseek(fp, 0L, SEEK_END) == 0)
    {
        // get sizeof file
        long bufsize = ftell(fp);
        if (bufsize == -1)
        {
            ERR("Invalid file size: %s", path);
            fclose(fp);
            return -1;
        }

        source = static_cast<char*>(GALLOC(kMEM_Unspecified, (bufsize + 1)));

        // seek backto start
        if (fseek(fp, 0L, SEEK_SET) != 0)
        {
            ERR("Unable to seek back to start: %s", path);
            GFREE(source);
            fclose(fp);
            return -1;
        }

        length = static_cast<i32>(fread(source, sizeof(char), bufsize, fp));
        if (length == 0)
        {
            ERR("Error reading file: %s", path);
            GFREE(source);
            fclose(fp);
            return -1;
        }
        else
        {
            source[length+1] = '\0';
        }
    }
    fclose(fp);

    *output = source;
    return length;
}

void parent_directory(char * dirPath, const char * filePath)
{
    normalize_path(dirPath, filePath);
    parent_directory(dirPath);
}

void parent_directory(char * path)
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
    if (pOut[-1] == '/')
        pOut[-1] = '\0';
}

void normalize_path(char * path)
{
    ASSERT(path);
    while (*path)
    {
        if (*path == '\\')
            *path = '/';
        path++;
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


