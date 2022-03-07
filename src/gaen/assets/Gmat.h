//------------------------------------------------------------------------------
// Gmat.h - Material containing multiple textures
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

#ifndef GAEN_ASSETS_GMAT_H
#define GAEN_ASSETS_GMAT_H

#include "gaen/core/base_defines.h"
#include "gaen/core/mem.h"
#include "gaen/core/Vector.h"

#include "gaen/assets/AssetHeader.h"

namespace gaen
{

class Gimg;

// These correspond directly to OpenGL pixel formats
enum TextureType
{
    kTXTY_Diffuse = 0,

    kTXTY_COUNT
};

#pragma pack(push, 1)
class Gmat : public AssetHeader4CC<FOURCC("gmat")>
{
public:
    static bool is_valid(const void * pBuffer, u64 size);
    static Gmat * instance(void * pBuffer, u64 size);
    static const Gmat * instance(const void * pBuffer, u64 size);

    static u64 required_size(const Vector<kMEM_Chef, Gimg*> & textures);

    static Gmat * create(const Vector<kMEM_Chef, Gimg*> & textures);

    Gimg * texture(TextureType textureType);
    const Gimg * texture(TextureType textureType) const;

private:
    // Class should not be constructed directly.  Use cast and create static methods.
    Gmat() = default;
    Gmat(const Gmat&) = delete;
    Gmat & operator=(const Gmat&) = delete;

    u64 mTextureOffsets[kTXTY_COUNT];

    char PADDING__[8];
};
#pragma pack(pop)

static_assert(sizeof(Gmat) % 16 == 0, "Gmat size not 16 byte aligned");

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_GMAT_H
