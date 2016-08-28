//------------------------------------------------------------------------------
// Gatl.cpp - Runtime texture atlas format
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

#include "assets/Gimg.h"
#include "assets/Gatl.h"

#include "assets/file_utils.h"

namespace gaen
{
static u16 verts_offset(size_t imagePathLen)
{
    return (u16)align(sizeof(Gatl) + imagePathLen + 1, 8);
}

bool Gatl::is_valid(const void * pBuffer, u64 size)
{
    if (size < sizeof(Gatl))
        return false;

    const Gatl * pAssetData = reinterpret_cast<const Gatl*>(pBuffer);

    if (pAssetData->magic4cc() != kMagic4CC)
        return false;
    if (pAssetData->size() != size)
        return false;

    if (pAssetData->mDefaultIndex >= pAssetData->glyphCount())
        return false;

    size_t imagePathLen = strnlen(pAssetData->imagePath(), MAX_PATH);
    if (pAssetData->mVertsOffset != reinterpret_cast<const char*>(pAssetData) + verts_offset(imagePathLen) - reinterpret_cast<const char*>(pAssetData))
        return false;

    if (required_size(pAssetData->imagePath(), pAssetData->mGlyphCount, pAssetData->mAliasCount) != size)
        return false;

    return true;
}

Gatl * Gatl::instance(void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gatl buffer");
        return nullptr;
    }

    return reinterpret_cast<Gatl*>(pBuffer);
}

const Gatl * Gatl::instance(const void * pBuffer, u64 size)
{
    return instance(const_cast<void*>(pBuffer), size);
}

u64 Gatl::required_size(const char * imagePath, u16 glyphCount, u16 aliasCount)
{
    ASSERT(imagePath);
    return (verts_offset(strnlen(imagePath, kMaxPath)) +
            verts_size_aligned(glyphCount) +
            tris_size_aligned(glyphCount) +
            aliases_size_aligned(aliasCount));
}

Gatl * Gatl::create(const char * imagePath, u16 glyphCount, u16 aliasCount, u16 defaultIndex)
{
    ASSERT(imagePath);
    u64 size = Gatl::required_size(imagePath, glyphCount, aliasCount);
    Gatl * pGatl = alloc_asset<Gatl>(kMEM_Texture, size);

    if (defaultIndex >= glyphCount)
        PANIC("Failed to create Gatl, defaultIndex too large: %u", defaultIndex);
    
    pGatl->mGlyphCount = glyphCount;
    pGatl->mAliasCount = aliasCount;
    pGatl->mDefaultIndex = defaultIndex;
    pGatl->mVertsOffset = verts_offset(strnlen(imagePath, kMaxPath));

    pGatl->mpImage = nullptr;

    strcpy(reinterpret_cast<char*>(pGatl+1), imagePath);
    
    ASSERT(is_valid(pGatl, required_size(imagePath, glyphCount, aliasCount)));
    return pGatl;
}

u16 Gatl::glyphIndexFromAlias(u32 aliasHash) const
{
    const GlyphAlias * pAlias = aliases();
    const GlyphAlias * pAliasesEnd = aliases() + mAliasCount;

    while (pAlias < pAliasesEnd)
    {
        if (aliasHash == pAlias->hash)
        {
            return pAlias->index;
        }
        pAlias++;
    }

    // we didn't find the hash
    ERR("Failed to find alias hash: %u", aliasHash);
    return mDefaultIndex;
}

GlyphTri * Gatl::glyphElemsFromAlias(u32 aliasHash)
{
    return glyphElems(glyphIndexFromAlias(aliasHash));
}

} // namespace gaen

