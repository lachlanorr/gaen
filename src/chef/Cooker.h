//------------------------------------------------------------------------------
// Cooker.h - Pure abstract base class for cookers
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

#ifndef GAEN_CHEF_COOKER_H
#define GAEN_CHEF_COOKER_H

#include "core/List.h"
#include "core/String.h"

namespace gaen
{

class CookInfo;
class Chef;

class Cooker
{
public:
    virtual ~Cooker() {}

    u32 version() const { return mVersion; }

    typedef List<kMEM_Chef, ChefString> ExtList;
    const ExtList & rawExts() const { ASSERT(mCookedExts.size() > 0); return mRawExts; }
    const ExtList & cookedExts() const { ASSERT(mCookedExts.size() > 0); return mCookedExts; }

    virtual void cook(CookInfo * pCookInfo) const = 0;

protected:
    // Increase version in subclasses to force a recook of those asset types.
    u32 mVersion = 0;

    ExtList mRawExts;
    ExtList mCookedExts;
};

} // namespace gaen

#endif // #ifndef GAEN_CHEF_COOKER_H
