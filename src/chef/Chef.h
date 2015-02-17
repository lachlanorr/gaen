//------------------------------------------------------------------------------
// Chef.h - Class to manage asset cookers
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

#ifndef GAEN_CHEF_CHEF_H
#define GAEN_CHEF_CHEF_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "core/mem.h"

#define REGISTER_COOKER(x) Chef::register_cooker(GNEW(kMEM_Chef, x))

namespace gaen
{

class Cooker
{
public:
    virtual const char * rawExt() = 0;
    virtual const char * cookedExt() = 0;
    virtual void cook(const char * platform, std::istream & input, std::ostream & output) = 0;
};


class Chef
{
public:
    Chef();
    ~Chef();
    
    static void cook(const char * platform, const char * rawFile);
    static bool register_cooker(Cooker * pCooker);
    static bool is_valid_platform(const char * platform);
    static const char * get_ext(const char * file);

private:
    void cookAsset(const char * platform, const char * rawFile);
    bool registerCooker(Cooker * pCooker);

    static void prep_paths(char * inFile, char * outFile, const char * platform, const char * rawFile, Cooker * pCooker);

    // map for raw extension to cooker
    std::map<std::string, Cooker*> mRawExtToCooker;

    // map for cooked extension to cooker
    std::map<std::string, Cooker*> mCookedExtToCooker;
};

} // namespace gaen

#endif // #ifndef GAEN_CHEF_CHEF_H
