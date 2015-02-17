//------------------------------------------------------------------------------
// cookers.cpp - Cookers for various asset types
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

#include "core/base_defines.h"
#include "core/Config.h"

#include "chef/Chef.h"
#include "chef/cookers.h"

namespace gaen
{

class CookerFnt : public Cooker
{
public:    
    const char * rawExt() { return "fnt"; }
    const char * cookedExt() { return "gfnt"; }
    
    void cook(const char * platform, std::istream & input, std::ostream & output)
    {
        Config<kMEM_Chef> conf;
        conf.read(input);

        // cook the image file dependency
        Chef::cook(platform, conf.get("image"));
    }
};
    
void register_cookers()
{
    REGISTER_COOKER(CookerFnt);
}

} // namespace gaen
