//------------------------------------------------------------------------------
// Audio.h - Audio cooker
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

#ifndef GAEN_CHEF_COOKERS_AUDIO_H
#define GAEN_CHEF_COOKERS_AUDIO_H

#include "gaen/assets/Gimg.h"
#include "gaen/chef/Cooker.h"

namespace gaen
{
namespace cookers
{

static const char * kExtWav = "wav";
static const char * kExtGaud = "gaud";

class Audio : public Cooker
{
public:
    Audio();
    virtual void cook(CookInfo * pCookInfo) const;

    static Gimg * load_png(const char * path, u32 referencePathHash, PixelFormat pixFmt = kPXL_RGBA8);

    static u32 reference_path_hash(const Chef & chef, const ChefString & rawPath);
    static u32 reference_path_hash(const CookInfo *pCookInfo);
private:
    void cookWav(CookInfo * pCookInfo) const;
};

}
} // namespace gaen

#endif // #ifndef GAEN_CHEF_COOKERS_AUDIO_H
