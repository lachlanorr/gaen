//------------------------------------------------------------------------------
// Gmdl.h - Runtime model format
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

#ifndef GAEN_ASSETS_GMDL_H
#define GAEN_ASSETS_GMDL_H

#include "core/base_defines.h"
#include "core/mem.h"

#include "assets/AssetHeader.h"
#include "assets/Color.h"

namespace gaen
{

#pragma pack(push, 1)
class Gmdl : public AssetHeader4CC<FOURCC("gmdl")>
{
public:
    static bool is_valid(const void * pBuffer, u64 size);
    static Gmdl * instance(void * pBuffer, u64 size);
    static const Gmdl * instance(const void * pBuffer, u64 size);
    
    static u64 required_size(PixelFormat pixelFormat, u32 width, u32 height);

    static Gmdl * create(PixelFormat pixelFormat, u32 width, u32 height);
    static Gmdl * load(const char * rawPath);

    u64 size() const;

private:
    // Class should not be constructed directly.  Use cast and create static methods.
    Gmdl() = default;
    Gmdl(const Gmdl&) = delete;
    Gmdl & operator=(const Gmdl&) = delete;

    char PADDING__[4];
};
#pragma pack(pop)

static_assert(sizeof(Gmdl) == 4, "Gmdl unexpected size");

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_GMDL_H
