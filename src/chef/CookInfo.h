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
enum CookFlags
{
    kCF_None              = 0x00,
    kCF_CookingDependency = 0x01
};

typedef List<kMEM_Chef, String<kMEM_Chef>> RecipeList;
class Chef;

class CookInfo
{
public:
    CookInfo(Chef * pChef,
             CookFlags flags,
             const char * rawPath,
             const char * cookedPath,
             const char * gamePath,
             const RecipeList & recipes);
    ~CookInfo();

    // Record a dependency, but don't cook it to include in the parent
    // asset.
    void recordDependency(char * depRawPath, char * gamePath, const char * path) const;

    // Cook a dependency with the intention of including the cooked
    // buffer in the parent asset.
    UniquePtr<CookInfo> cookDependency(const char * path) const;

    CookFlags flags() const { return mFlags; }
    const char * rawPath() const { return mRawPath; }
    const char * cookedPath() const { return mCookedPath; }
    const char * gamePath() const { return mGamePath; }

    const void * cookedBuffer() const { return mpCookedBuffer; }
    u64 cookedBufferSize() const { return mCookedBufferSize; }
    
    bool isCooked() const
    {
        return mpCookedBuffer != nullptr && mCookedBufferSize > 0;
    }

    void setCookedBuffer(void * pBuffer, u64 size) const;

    const Config<kMEM_Chef> & recipe() const { return mRecipe; }
    const Set<kMEM_Chef, String<kMEM_Chef>> & dependencies() const { return mDependencies; }

private:
    Chef * mpChef;
    CookFlags mFlags;
    char mRawPath[kMaxPath+1];
    char mCookedPath[kMaxPath+1];
    char mGamePath[kMaxPath+1];

    mutable void * mpCookedBuffer;
    mutable u64 mCookedBufferSize;

    Config<kMEM_Chef> mRecipe;
    mutable Set<kMEM_Chef, String<kMEM_Chef>> mDependencies;
};

} // namespace gaen

#endif // #ifndef GAEN_CHEF_COOK_INFO_H
