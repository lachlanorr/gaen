//------------------------------------------------------------------------------
// Cooker.h - Pure abstract base class for cookers
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

    u16 version() const { return mVersion; }

    typedef List<kMEM_Chef, ChefString> ExtList;
    const ExtList & rawExts() const { ASSERT(mCookedExts.size() > 0); return mRawExts; }

    // Output extensions of this cooker, not necessarily unique.
    // E.g. Font->(gimg,gatl)
    const ExtList & cookedExts() const { ASSERT(mCookedExts.size() > 0); return mCookedExts; }

    // Output extensions exclusive to this cooker, allowing us to lookup cookers from cooked extensions.
    // E.g. Image->(gimg), Atlas->(gatl)
    const ExtList & cookedExtsExclusive() const { return mCookedExtsExclusive; }

    virtual void cook(CookInfo * pCookInfo) const = 0;

protected:
    void setVersion(u16 version) { mVersion = version; }
    void addRawExt(const ChefString & ext) { mRawExts.push_back(ext); }

    void addCookedExt(const ChefString & ext) { mCookedExts.push_back(ext); }
    void addCookedExtExclusive(const ChefString & ext)
    {
        mCookedExts.push_back(ext);
        mCookedExtsExclusive.push_back(ext);
    }

private:
    // Increase version in subclasses to force a recook of those asset types.
    u16 mVersion = 0;

    ExtList mRawExts;
    ExtList mCookedExts;
    ExtList mCookedExtsExclusive;
};

} // namespace gaen

#endif // #ifndef GAEN_CHEF_COOKER_H
