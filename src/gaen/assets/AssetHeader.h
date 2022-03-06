//------------------------------------------------------------------------------
// AssetHeader.h - Struct that appears at head of every cooked asset
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

#ifndef GAEN_ASSETS_ASSETHEADER_H
#define GAEN_ASSETS_ASSETHEADER_H

#include "gaen/core/mem.h"

namespace gaen
{

class AssetHeader
{
public:
    AssetHeader(u32 magic4cc, u64 size)
    {
        mMagic4cc = magic4cc;
        mChefVersion = 0;
        mCookerVersion = 0;
        mSize = size;
    }

    u32 magic4cc() const { return mMagic4cc; }

    u32 chefVersion() const { return mChefVersion; }
    void setChefVersion(u32 version) { mChefVersion = version; }

    u32 cookerVersion() const { return mCookerVersion; }
    void setCookerVersion(u32 version) { mCookerVersion = version; }

    u64 size() const { return mSize; }
    void setSize(u64 size) { mSize = size; }

    void getExt(char * ext)
    {
        char * p4cc = (char*)&mMagic4cc;
        ext[0] = p4cc[0];
        ext[1] = p4cc[1];
        ext[2] = p4cc[2];
        ext[3] = p4cc[3];
        ext[4] = '\0';
    }

protected:
    u32 mMagic4cc;  // typically the cooked extension
    u16 mChefVersion;     // chef version that cooked the file
    u16 mCookerVersion;   // cooker version that cooked the file
    u64 mSize;      // total asset size, including header

private:
    AssetHeader(const AssetHeader&)              = delete;
    AssetHeader(AssetHeader&&)                   = delete;
    AssetHeader & operator=(const AssetHeader&)  = delete;
    AssetHeader & operator=(AssetHeader&&)       = delete;
};
static_assert(sizeof(AssetHeader) == 16, "AssetHeader unexpected size");
static_assert(sizeof(AssetHeader) % 16 == 0, "AssetHeader size not 16 byte aligned");

template <u32 FCC>
class AssetHeader4CC : public AssetHeader
{
public:
    static const u32 kMagic4CC = FCC;

    template <class T>
    static T * alloc_asset(MemType memType, u64 size)
    {
        AssetHeader4CC * pAH = new (GALLOC(memType, size)) AssetHeader4CC(size);
        memset(pAH+1, 0, size - sizeof(AssetHeader4CC));
        return static_cast<T*>(pAH);
    }

private:
    explicit AssetHeader4CC(u64 size) : AssetHeader(FCC, size) {}
    AssetHeader4CC(const AssetHeader4CC&)              = delete;
    AssetHeader4CC(AssetHeader4CC&&)                   = delete;
    AssetHeader4CC & operator=(const AssetHeader4CC&)  = delete;
    AssetHeader4CC & operator=(AssetHeader4CC&&)       = delete;
};

} // namespace gaen

#endif // #ifndef GAEN_ASSETS_ASSETHEADER_H
