//------------------------------------------------------------------------------
// CookInfo.cpp - Describes a cooking operation, and its buffer result
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

#include "chef/Chef.h"

#include "chef/CookInfo.h"

namespace gaen
{

CookInfo::CookInfo(Chef * pChef,
         CookFlags flags,
         const char * rawPath,
         const char * cookedPath,
         const RecipeList & recipes)
  : mpChef(pChef)
  , mFlags(flags)
{
    strcpy(mRawPath, rawPath);
    strcpy(mCookedPath, cookedPath);

    mpChef->overlayRecipes(mRecipe, recipes);

    mpCookedBuffer = nullptr;
    mCookedBufferSize = 0;
}

CookInfo::~CookInfo()
{
    if (mpCookedBuffer)
    {
        GFREE(mpCookedBuffer);
        mpCookedBuffer = nullptr;
        mCookedBufferSize = 0;
    }
}

void CookInfo::recordDependency(const char * path) const
{
    mDependencies.emplace(path);
}

UniquePtr<CookInfo> CookInfo::cookDependency(const char * path) const
{
    char depRawPath[kMaxPath+1];

    if (!mpChef->convertRelativeDependencyPath(depRawPath, mRawPath, path))
    {
        PANIC("Unable to convert dependency relative path to raw path: %s", path);
    }

    UniquePtr<CookInfo> pCi = mpChef->cookDependency(depRawPath);
    PANIC_IF(!pCi->cookedBuffer() || pCi->cookedBufferSize() == 0, "Failed to cook dependency: %s", path);

    char rawRelativePath[kMaxPath+1];
    mpChef->getRawRelativePath(rawRelativePath, depRawPath);

    mDependencies.emplace(path);

    for (auto & dep : pCi->dependencies())
    {
        mDependencies.emplace(dep);
    }
    return pCi;
}

void CookInfo::setCookedBuffer(void * pBuffer, u64 size) const
{
    if (mpCookedBuffer)
        GFREE(mpCookedBuffer);
    mpCookedBuffer = pBuffer;
    mCookedBufferSize = size;
}

} // namespace gaen
