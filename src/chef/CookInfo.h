//------------------------------------------------------------------------------
// CookInfo.h - Describes a cooking operation, and its buffer result
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

#ifndef GAEN_CHEF_COOK_INFO_H
#define GAEN_CHEF_COOK_INFO_H

#include "core/mem.h"
#include "core/Set.h"
#include "core/List.h"
#include "core/String.h"

#include "assets/Config.h"
#include "assets/file_utils.h"

namespace gaen
{
class Cooker;

enum CookFlags
{
    kCF_None              = 0x00,
    kCF_CookingDependency = 0x01
};

typedef Config<kMEM_Chef> Recipe;
typedef List<kMEM_Chef, ChefString> RecipeList;
class Chef;

struct CookResult
{
    CookResult(const ChefString & cookedExt, const ChefString & cookedPath, const ChefString & gamePath)
      : cookedExt(cookedExt)
      , cookedPath(cookedPath)
      , gamePath(gamePath)
    {}

    ChefString cookedExt;
    ChefString cookedPath;
    ChefString gamePath;
    mutable UniquePtr<void> pCookedBuffer;
    mutable u64 cookedBufferSize = 0;

    bool isCooked() const
    {
        return pCookedBuffer.get() && cookedBufferSize > 0;
    }

    void setCookedBuffer(void * pBuffer, u64 size) const
    {
        pCookedBuffer.reset(pBuffer);
        cookedBufferSize = size;
    }
};
typedef List<kMEM_Chef, CookResult> CookResultList;

struct DependencyInfo
{
    DependencyInfo(const ChefString & relativePath)
      : relativePath(relativePath)
    {}

    ChefString relativePath;
    ChefString rawPath;
    List<kMEM_Chef, ChefString> gamePaths;
};
struct DependencyInfoHash
{
    u64 operator()(const DependencyInfo & val) const
    {
        return fnv1a_32(val.relativePath.c_str());
    }
};
struct DependencyInfoEquals
{
    u64 operator()(const DependencyInfo & lhs, const DependencyInfo & rhs) const
    {
        return lhs.relativePath == rhs.relativePath;
    }
};
typedef HashSet<kMEM_Chef, DependencyInfo, DependencyInfoHash, DependencyInfoEquals> DependencySet;

class CookInfo
{
public:
    CookInfo(Chef * pChef,
             const Cooker * pCooker,
             CookFlags flags,
             const ChefString & rawPath,
             const Recipe & recipe)
      : mpChef(pChef)
      , mpCooker(pCooker)
      , mFlags(flags)
      , mRawPath(rawPath)
      , mRecipe(recipe)
    {}

    Chef & chef() { return *mpChef; }
    const Cooker & cooker() const { return *mpCooker; }
    CookFlags flags() const { return mFlags; }

    const ChefString & rawPath() const { return mRawPath; }
    const Recipe & recipe() const { return mRecipe; }

    const CookResultList & results() const { return mResults; }
    const DependencySet & dependencies() const { return mDependencies; }

    void addCookResult(const ChefString & cookedExt,
                       const ChefString & cookedPath,
                       const ChefString & gamePath);

    // Record a dependency, but don't cook it to include in the parent
    // asset.
    const DependencyInfo & recordDependency(const ChefString & relativePath) const;

    // Cook and record a dependency
    UniquePtr<CookInfo> cookDependency(const ChefString & relativePath) const;

    bool isCooked(const char * ext) const;
    void setCookedBuffer(const char * ext, void * pBuffer, u64 size) const;
private:
    Chef * mpChef;
    const Cooker * mpCooker;
    CookFlags mFlags;

    ChefString mRawPath;
    Recipe mRecipe;

    mutable CookResultList mResults;
    mutable DependencySet mDependencies;
};

} // namespace gaen

#endif // #ifndef GAEN_CHEF_COOK_INFO_H
