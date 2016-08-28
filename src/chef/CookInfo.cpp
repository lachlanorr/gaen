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
#include "chef/Cooker.h"

#include "chef/CookerRegistry.h"
#include "chef/CookInfo.h"

namespace gaen
{

void CookInfo::addCookResult(const ChefString & cookedExt,
                             const ChefString & cookedPath,
                             const ChefString & gamePath)
{
    mResults.emplace_back(cookedExt, cookedPath, gamePath);
}

const DependencyInfo & CookInfo::recordDependency(const ChefString & relativePath) const
{
    DependencyInfo depInfo(relativePath);
    depInfo.rawPath = mpChef->getRelativeDependencyRawPath(mRawPath, relativePath);

    const Cooker * pDepCooker = CookerRegistry::find_cooker_from_raw(depInfo.rawPath);
    ASSERT(pDepCooker);

    for (const ChefString & cookedExt : pDepCooker->cookedExts())
    {
        depInfo.gamePaths.push_back(mpChef->getGamePath(depInfo.rawPath, cookedExt));
    }

    auto diPair = mDependencies.insert(depInfo);
    return *diPair.first;
}

UniquePtr<CookInfo> CookInfo::cookDependency(const ChefString & relativePath) const
{
    const DependencyInfo & depInfo = recordDependency(relativePath);

    UniquePtr<CookInfo> pCi = mpChef->cookDependency(depInfo.rawPath);

    for (auto & dep : pCi->dependencies())
    {
        mDependencies.insert(dep);
    }
    return pCi;
}

bool CookInfo::isCooked(const char * ext) const
{
    ASSERT(ext);
    for (CookResult & cr : mResults)
    {
        const char * cookedExt = get_ext(cr.cookedPath.c_str());
        if (0 == strcmp(cookedExt, ext))
        {
            return cr.isCooked();
        }
    }
    PANIC("Invalid extension to isCooked: %s", ext);
    return false;
}

void CookInfo::setCookedBuffer(const char * ext, void * pBuffer, u64 size) const
{
    ASSERT(ext);
    for (CookResult & cr : mResults)
    {
        const char * cookedExt = get_ext(cr.cookedPath.c_str());
        if (0 == strcmp(cookedExt, ext))
        {
            cr.setCookedBuffer(pBuffer, size);
            return;
        }
    }
    PANIC("Invalid extension to setCookedBuffer: %s", ext);
}

} // namespace gaen
