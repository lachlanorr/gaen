//------------------------------------------------------------------------------
// file_utils_win32.cpp - Various helper reoutines for file system activities
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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "file_utils.h"

namespace gaen
{

bool file_exists(const char * filePath)
{
    DWORD dwAttrib = GetFileAttributesA(filePath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
            !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool dir_exists(const char * filePath)
{
    DWORD dwAttrib = GetFileAttributesA(filePath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void make_dirs(const char * dirPath)
{
    if (!dirPath || *dirPath == '\0')
        return;

    char scratch[kMaxPath+1];

    if (file_exists(dirPath))
    {
        PANIC("Directory name collides with already existing file: %s", dirPath);
        return;
    }
    else if (dir_exists(dirPath))
    {
        return;
    }
    else
    {
        // make any of our necessary parent directories
        parent_dir(scratch, dirPath);
        make_dirs(scratch);

        // make ourselves
        BOOL retval = CreateDirectoryA(dirPath, NULL);
        PANIC_IF(!retval, "Failed to create directory: %s");
    }
}

void process_path(char * path)
{
    DWORD ret = GetModuleFileNameA(NULL, path, kMaxPath);
    normalize_path(path);
}

void full_path(char * outPath, char * path)
{
    DWORD retval = GetFullPathNameA(path, kMaxPath, outPath, NULL);

    PANIC_IF(retval == 0, "GetFullPathNameA failed on %s", path);

    normalize_path(outPath);
}

bool is_file_newer(const char * path, const char * comparePath)
{
    HANDLE pathH = CreateFileA(path, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    PANIC_IF(!pathH, "File not found: %s", path);

    HANDLE comparePathH = CreateFileA(comparePath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (!comparePathH)
    {
        CloseHandle(pathH);
        PANIC("File not found: %s", comparePath);
    }

    FILETIME pathModTime;
    FILETIME comparePathModTime;

    BOOL pr = GetFileTime(pathH, NULL, NULL, &pathModTime);
    BOOL cpr = GetFileTime(comparePathH, NULL, NULL, &comparePathModTime);

    if (!pr || !cpr)
    {
        CloseHandle(pathH);
        CloseHandle(comparePathH);
        PANIC("Unable to read filetimes from either %s or %s", path, comparePath);
    }

    bool retval = CompareFileTime(&pathModTime, &comparePathModTime) >= 0;

    CloseHandle(pathH);
    CloseHandle(comparePathH);

    return retval;
}


void recurse_dir(const char * rootPath, void * context, RecurseDirCB cb)
{
    char searchPath[kMaxPath+1];
    char foundPath[kMaxPath+1];

    // convert any normalized path back to windows '\'
    normalize_path(searchPath, rootPath);
    append_path(searchPath, "*");

    normalize_path(foundPath, rootPath);

    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFile(searchPath, &fd);
    BOOL ret = hFind != INVALID_HANDLE_VALUE;

    while (ret)
    {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
           if (strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0)
           {
               normalize_path(foundPath, rootPath);
               append_path(foundPath, fd.cFileName);
               recurse_dir(foundPath, context, cb);
           }
        }
        else
        {
            normalize_path(foundPath, rootPath);
            append_path(foundPath, fd.cFileName);
            cb(foundPath, context);
        }

        ret = FindNextFile(hFind, &fd);
    }
    FindClose(hFind);
}


} // namespace gaen
