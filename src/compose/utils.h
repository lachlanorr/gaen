//------------------------------------------------------------------------------
// utils.h - Compiler utility functions
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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

#ifndef GAEN_COMPOSE_UTILS_H
#define GAEN_COMPOSE_UTILS_H

namespace gaen
{

static const char * kAssetPathSuffix = "__path";
static const char * kRefCountedAssignedSuffix = "__isAssigned";

char * strcat_alloc(const char * lhs, const char * rhs);
char * asset_path_name(const char * assetHandleName);
char * asset_handle_name(const char * assetPathName);

char * refcounted_assigned_name(const char * propFieldName);

const char * path_join(const char * rootDir,
                       const char * filename,
                       ParseData *pParseData);

const char * path_filename(const char * fullPath,
                           ParseData *pParseData);

const char * full_path(const char * path,
                       ParseData* pParseData);

void make_posix_path(char * path);

}

#endif // #ifndef GAEN_COMPOSE_UTILS_H

