//------------------------------------------------------------------------------
// CookerRegistry.cpp - Registration for cookers
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

#include "assets/file_utils.h"

#include "chef/Chef.h"
#include "chef/CookerRegistry.h"

namespace gaen
{

void CookerRegistry::register_cooker(UniquePtr<Cooker> pCooker)
{
    for (const ChefString & rawExt : pCooker->rawExts())
    {
        PANIC_IF(sRawExtToCooker.find(rawExt) != sRawExtToCooker.end(),
                 "Multiple cookers registered for same raw extension: %s",
                 rawExt);

        sRawExtToCooker[rawExt] = pCooker.get();
    }

    for (const ChefString & cookedExt : pCooker->cookedExtsExclusive())
    {
        PANIC_IF(sCookedExtToCooker.find(cookedExt) != sCookedExtToCooker.end(),
                 "Multiple cookers registered for same cooked extension: %s",
                 cookedExt);

        sCookedExtToCooker[cookedExt] = pCooker.get();
    }

    sCookers.emplace_back(std::move(pCooker));
}

const Cooker * CookerRegistry::find_cooker_from_raw(const ChefString & rawPath)
{
    // find our cooker
    ChefString ext = get_ext(rawPath.c_str());
    auto it = sRawExtToCooker.find(ext);
    if (it == sRawExtToCooker.end())
        return nullptr;
    const Cooker * pCooker = it->second;
    return pCooker;
}

const Cooker * CookerRegistry::find_cooker_from_cooked(const ChefString & cookedPath)
{
    // find our cooker
    ChefString ext = get_ext(cookedPath.c_str());
    auto it = sCookedExtToCooker.find(ext);
    if (it == sCookedExtToCooker.end())
        return nullptr;
    const Cooker * pCooker = it->second;
    return pCooker;
}


// CookerRegistry statics
List<kMEM_Chef, UniquePtr<Cooker>> CookerRegistry::sCookers;
HashMap<kMEM_Chef, ChefString, const Cooker*> CookerRegistry::sRawExtToCooker;
HashMap<kMEM_Chef, ChefString, const Cooker*> CookerRegistry::sCookedExtToCooker;




} // namespace gaen
