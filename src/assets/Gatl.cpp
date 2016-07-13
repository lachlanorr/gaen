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

#include "assets/AssetTypes.h"
#include "assets/Gimg.h"
#include "assets/Gatl.h"

namespace gaen
{
const char * Gatl::kMagic = "gatl";
const u32 Gatl::kMagic4cc = AssetTypes::ext_to_4cc(Gatl::kMagic);

bool Gatl::is_valid(const void * pBuffer, u64 size)
{
    if (size < sizeof(Gatl))
        return false;

    const Gatl * pGatl = reinterpret_cast<const Gatl*>(pBuffer);

    if (0 != strncmp(kMagic, pGatl->mMagic, 4))
        return false;

    if (pGatl->mDefaultIndex >= pGatl->glyphCount())
        return false;

    if (pGatl->size() != size)
        return false;

    return true;
}

Gatl * Gatl::instance(void * pBuffer, u64 size)
{
    if (!is_valid(pBuffer, size))
    {
        PANIC("Invalid Gimg buffer");
        return nullptr;
    }

    return reinterpret_cast<Gatl*>(pBuffer);
}

const Gatl * Gatl::instance(const void * pBuffer, u64 size)
{
    return instance(const_cast<void*>(pBuffer), size);
}

u64 Gatl::required_size(u32 glyphCount, u32 aliasCount, const Gimg & image)
{
    return sizeof(Gatl) + glyphCount * sizeof(GlyphCoords) + aliasCount * sizeof(GlyphAlias) + image.size();
    return 0;
}

Gatl * Gatl::create(u32 glyphCount, u32 aliasCount, u32 defaultIndex, const Gimg & image)
{
    u64 size = Gatl::required_size(glyphCount, aliasCount, image);
    Gatl * pGatl = (Gatl*)GALLOC(kMEM_Texture, size);

    ASSERT(strlen(kMagic) == 4);
    strcpy(pGatl->mMagic, kMagic);

    if (defaultIndex >= glyphCount)
        PANIC("Failed to create Gatl, defaultIndex too large: %u", defaultIndex);
    
    pGatl->mGlyphCount = glyphCount;
    pGatl->mAliasCount = aliasCount;
    pGatl->mDefaultIndex = defaultIndex;

    memcpy(&pGatl->image(), &image, image.size());

    ASSERT(is_valid(pGatl, required_size(glyphCount, aliasCount, image)));
    return pGatl;
}

u64 Gatl::size() const
{
    return required_size(mGlyphCount, mAliasCount, image());
}

GlyphCoords & Gatl::coordsFromAlias(u32 aliasHash)
{
    GlyphAlias * pAlias = aliases();
    GlyphAlias * pAliasesEnd = aliases() + mAliasCount;

    while (pAlias++ < pAliasesEnd)
    {
        if (aliasHash == pAlias->hash)
        {
            return coords(pAlias->index);
        }
    }

    // we didn't find the hash
    ERR("Failed to find alias hash: %u", aliasHash);
    return defaultCoords();
}

} // namespace gaen

