//------------------------------------------------------------------------------
// CookerRegistry.h - Registration for cookers
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

#ifndef GAEN_CHEF_COOKER_REGISTRY_H
#define GAEN_CHEF_COOKER_REGISTRY_H

#include <iostream>
#include <fstream>

#include "core/mem.h"
#include "core/HashMap.h"
#include "core/List.h"
#include "core/String.h"
#include "assets/Config.h"

namespace gaen
{

class Chef;
struct CookInfo
{
    Chef * pChef;
    const char * rawPath;
    const char * cookedPath;
    Config<kMEM_Chef> & recipe;
    
    CookInfo(Chef * pChef, const char * rawPath, const char * cookedPath, Config<kMEM_Chef> & recipe)
      : pChef(pChef)
      , rawPath(rawPath)
      , cookedPath(cookedPath)
      , recipe(recipe)
    {}
};

typedef void(*CookCB)(std::ifstream & ifs, std::ofstream & ofs, const CookInfo & ci);

struct Cooker
{
    const char * rawExt;
    const char * cookedExt;
    CookCB cook;

    Cooker(const char * rawExt, const char * cookedExt, CookCB cook)
      : rawExt(rawExt)
      , cookedExt(cookedExt)
      , cook(cook)
    {}
};


class CookerRegistry
{
public:
    static void register_cooker(const char * rawExt, const char * cookedExt, CookCB cook);

    static Cooker * find_cooker_from_raw(const char * path);
    static Cooker * find_cooker_from_cooked(const char * path);

private:
    static List<kMEM_Chef, Cooker> sCookers;
    
    // map for raw extension to cooker
    static HashMap<kMEM_Chef, String<kMEM_Chef>, Cooker*> sRawExtToCooker;

    // map for cooked extension to cooker
    static HashMap<kMEM_Chef, String<kMEM_Chef>, Cooker*> sCookedExtToCooker;
};


} // namespace gaen

#endif // #ifndef GAEN_CHEF_COOKER_REGISTRY_H


