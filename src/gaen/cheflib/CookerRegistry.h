//------------------------------------------------------------------------------
// CookerRegistry.h - Registration for cookers
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2022 Lachlan Orr
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

#ifndef GAEN_CHEF_COOKER_REGISTRY_H
#define GAEN_CHEF_COOKER_REGISTRY_H

#include "gaen/core/mem.h"
#include "gaen/core/HashMap.h"
#include "gaen/core/List.h"
#include "gaen/core/String.h"
#include "gaen/assets/Config.h"
#include "gaen/cheflib/CookInfo.h"
#include "gaen/cheflib/Cooker.h"

namespace gaen
{
class CookerRegistry
{
public:
    static void register_cooker(UniquePtr<Cooker> pCooker);

    static const Cooker * find_cooker_from_raw(const ChefString & rawPath);
    static const Cooker * find_cooker_from_cooked(const ChefString & cookedPath);

private:
    static List<kMEM_Chef, UniquePtr<Cooker>> sCookers;

    // map for raw extension to cooker
    static HashMap<kMEM_Chef, ChefString, const Cooker*> sRawExtToCooker;

    // map for cooked extension to cooker
    static HashMap<kMEM_Chef, ChefString, const Cooker*> sCookedExtToCooker;
};


} // namespace gaen

#endif // #ifndef GAEN_CHEF_COOKER_REGISTRY_H


