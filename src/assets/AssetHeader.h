//------------------------------------------------------------------------------
// AssetHeader.h - Struct that appears at head of every cooked asset
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

#ifndef GAEN_ASSETS_ASSETHEADER_H
#define GAEN_ASSETS_ASSETHEADER_H

#include "core/base_defines.h"

namespace gaen
{

class AssetHeader
{
public:
    AssetHeader()
    {
        memset(this, 0, sizeof(AssetHeader));
    }

    AssetHeader(const char * magic, u32 version)
    {
        u32 magicLen = (u32)strlen(magic);
        ASSERT(magic && magicLen > 0 && magicLen <= 4);
        strncpy(mMagic, magic, 4);
        // fill any zeros if magic shorter than 4 characters
        for (u32 i = magicLen; i < 4; ++i)
            mMagic[i] = '\0';

        mVersion = version;
    }
    u32 version() { return mVersion; }
    template<class StrT>
    StrT getMagic()
    {
        char magicNT[5];
        strncpy(magicNT, mMagic, 4);
        magicNT[4] = '\0';
        return StrT(magicNT);
    }
    bool validateBuffer(const void * pBuffer, u64 size)
    {
        ASSERT(pBuffer && size > 0);

        if (!pBuffer)
            return false;
        if (size < sizeof(AssetHeader))
            return false;

        return 0 == memcmp(pBuffer, this, sizeof(AssetHeader));
    }

    char mMagic[4]; // typically the cooked extension
    u32 mVersion;   // cooker version that cooked the file
};

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_ASSETHEADER_H
