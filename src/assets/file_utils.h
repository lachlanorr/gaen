//------------------------------------------------------------------------------
// file_utils.h - Various helper routines for file system activities
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

#ifndef GAEN_ASSETS_FILE_UTILS_H
#define GAEN_ASSETS_FILE_UTILS_H

#include <iostream>
#include <fstream>

#include "core/base_defines.h"

namespace gaen
{

static const u32 kMaxPath = 255;

typedef void(*RecurseDirCB)(const char * path, void * context);

void recurse_dir(const char * root_path, void * context, RecurseDirCB cb);

// All of these file system functions assume a path with '/' separators, 
// even on windows.
bool file_exists(const char * filePath);
bool dir_exists(const char * dirPath);
void process_path(char * path);

// Get parent directory of file_path
void parent_dir(char * dirPath, const char * filePath);

// Set path to parent
void parent_dir(char * path);

// Make all directories specified in path
void make_dirs(const char * dirPath);

const char * get_ext(const char * path);
char * get_ext(char * path);
void strip_ext(char * path);
void change_ext(char * path, const char * ext);

const char * get_filename(const char * path);
void get_filename(char * filename, const char * path);
void get_filename_root(char * filename, const char * path);
void upper(char * str);

// copy inPath to outPath, converting all '\' to '/'
void normalize_path(char * outPath, const char * inPath);
void normalize_path(char * path);

void append_path(char * path, const char * append);

void full_path(char * outPath, char * path);

bool is_file_newer(const char * path, const char * comparePath);

bool write_file_if_contents_differ(const char * path, const char * contents);

struct FileReader
{
    FileReader(const char * path)
    {
        ifs.open(path, std::ifstream::in | std::ifstream::binary);
        auto flags = ifs.flags();
        bool fail = ifs.fail();
        if (!ifs.good())
        {
            PANIC("Unable to open FileReader: %s", path);
        }
        ifs.seekg(0, ifs.end);
        mSize = (u64)ifs.tellg();
        ifs.seekg(0, ifs.beg);
    }
    ~FileReader()
    {
        if (ifs.is_open())
            ifs.close();
    }

    template <typename T>
    void read(T* pDst, u64 size)
    {
        ifs.read(reinterpret_cast<char*>(pDst), size);
        PANIC_IF(!ifs.good() || ifs.gcount() != size, "Read failure");
    }

    template <typename T>
    void read(T* pDst)
    {
        ifs.read(reinterpret_cast<char*>(pDst), sizeof(T));
        PANIC_IF(!ifs.good() || ifs.gcount() != sizeof(T), "Read failure");
    }


    u64 size() { return mSize; }

    std::ifstream ifs;

private:
    u64 mSize;
};

struct FileWriter
{
    FileWriter(const char * path)
    {
        ofs.open(path, std::ofstream::out | std::ofstream::binary);
        if (!ofs.good())
        {
            PANIC("Unable to open FileWriter: %s", path);
        }
    }
    ~FileWriter()
    {
        if (ofs.is_open())
            ofs.close();
    }

    std::ofstream ofs;
};

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_FILE_UTILS_H
