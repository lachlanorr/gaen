//------------------------------------------------------------------------------
// CookerRegistry.cpp - Registration for cookers
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#include "core/platutils.h"

#include "chef/Chef.h"
#include "chef/CookerRegistry.h"

namespace gaen
{

void CookerRegistry::register_cooker(const char * rawExt, const char * cookedExt, CookCB cook)
{
    ASSERT(rawExt);
    ASSERT(cookedExt);
    ASSERT(cook);

    PANIC_IF(sRawExtToCooker.find(rawExt) != sRawExtToCooker.end(),
             "Multiple cookers registered for same raw extension: %s",
             rawExt);

    PANIC_IF(sCookedExtToCooker.find(cookedExt) != sCookedExtToCooker.end(),
             "Multiple cookers registered for same cooked extension: %s",
             cookedExt);

    sCookers.emplace_back(rawExt, cookedExt, cook);

    sRawExtToCooker[rawExt] = &sCookers.back();
    sCookedExtToCooker[cookedExt] = &sCookers.back();
}

Cooker * CookerRegistry::find_cooker_from_raw(const char * path)
{
    // find our cooker
    const char * ext = get_ext(path);
    auto it = sRawExtToCooker.find(ext);
    PANIC_IF(it == sRawExtToCooker.end(), "No cooker registered for raw extension: %s", ext);
    Cooker * pCooker = it->second;
    return pCooker;
}

Cooker * CookerRegistry::find_cooker_from_cooked(const char * path)
{
    // find our cooker
    const char * ext = get_ext(path);
    auto it = sCookedExtToCooker.find(ext);
    PANIC_IF(it == sCookedExtToCooker.end(), "No cooker registered for cooked extension: %s", ext);
    Cooker * pCooker = it->second;
    return pCooker;
}


// CookerRegistry statics
List<kMEM_Chef, Cooker> CookerRegistry::sCookers;
HashMap<kMEM_Chef, String<kMEM_Chef>, Cooker*> CookerRegistry::sRawExtToCooker;
HashMap<kMEM_Chef, String<kMEM_Chef>, Cooker*> CookerRegistry::sCookedExtToCooker;




} // namespace gaen
