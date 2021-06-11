//------------------------------------------------------------------------------
// Gmat.cpp - Material containing multiple textures
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

#include "core/Vector.h"

#include "assets/file_utils.h"
#include "assets/Gimg.h"

#include "assets/Gmat.h"

namespace gaen
{

bool Gmat::is_valid(const void * pBuffer, u64 size)
{
    if (size < sizeof(Gmat))
        return false;

    const Gmat * pAssetData = reinterpret_cast<const Gmat*>(pBuffer);

    if (pAssetData->magic4cc() != kMagic4CC)
        return false;
    if (pAssetData->size() != size)
        return false;

    return true;
}

Gmat * Gmat::instance(void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gmat buffer");
        return nullptr;
    }

    return reinterpret_cast<Gmat*>(pBuffer);
}

const Gmat * Gmat::instance(const void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gmat buffer");
        return nullptr;
    }

    return reinterpret_cast<const Gmat*>(pBuffer);
}

u64 Gmat::required_size(const Vector<kMEM_Chef, Gimg*> & textures)
{
    u64 size = sizeof(Gmat);
    for (auto & t : textures)
    {
        size += align(t->size(), 16);
    }
    return size;
}

Gmat * Gmat::create(const Vector<kMEM_Chef, Gimg*> & textures)
{
    u64 size = required_size(textures);
    Gmat * pGmat = alloc_asset<Gmat>(kMEM_Texture, size);

    PANIC_IF(textures.size() != kTXTY_COUNT, "Invalid number of textures");

    pGmat->mTextureOffsets[kTXTY_Diffuse] = sizeof(Gmat);
    u8 * pDiffuseStart = reinterpret_cast<u8*>(pGmat) + sizeof(Gmat);
    memcpy(pDiffuseStart, textures[kTXTY_Diffuse], textures[kTXTY_Diffuse]->size());

    ASSERT(is_valid(pGmat, size));

    return pGmat;
}

Gimg * Gmat::texture(TextureType textureType)
{
    if (mTextureOffsets[textureType])
    {
        return reinterpret_cast<Gimg*>(reinterpret_cast<u8*>(this) + mTextureOffsets[textureType]);
    }
    return nullptr;
}

const Gimg * Gmat::texture(TextureType textureType) const
{
    return const_cast<Gmat*>(this)->texture(textureType);
}


} // namespace gaen

