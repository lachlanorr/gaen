//------------------------------------------------------------------------------
// CmpString.h - String class used through BlockMemory in Compose scripts
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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

#ifndef GAEN_ENGINE_CMPSTRING_H
#define GAEN_ENGINE_CMPSTRING_H

#include "engine/BlockData.h"

namespace gaen
{

class BlockMemory;

class CmpString
{
    friend class BlockMemory;
public:
    CmpString()
      : mpBlockData(nullptr)
    {}

    CmpString(BlockData * pBlockData);

    char * c_str()             { ASSERT(mpBlockData); return &mpBlockData->data.string.chars[0]; }
    const char * c_str() const { ASSERT(mpBlockData); return &mpBlockData->data.string.chars[0]; }
    u16 size() const           { ASSERT(mpBlockData); return mpBlockData->data.string.charCount; }
    u16 blockCount() const     { ASSERT(mpBlockData); return mpBlockData->blockCount; }

    template <typename T>
    void writeMessage(T & msgAcc, u32 startIndex) const
    {
        ASSERT(mpBlockData);

        for (u32 i = 0; i < mpBlockData->blockCount; ++i)
        {
            static_assert(sizeof(Block) == sizeof(BlockData), "Block and BlockData must have same size");
            msgAcc[startIndex + i] = *reinterpret_cast<Block*>(&mpBlockData[i]);
        }

        // fix up header
        BlockData * pBlockData = reinterpret_cast<BlockData*>(&msgAcc[startIndex]);
        pBlockData->refCount = 0;
    }

    template <typename T>
    static bool is_string_message(const T & msgAcc, u32 startIndex)
    {
        const BlockData * pBd = reinterpret_cast<const BlockData*>(&msgAcc[startIndex]);
        return (pBd->type == kBKTY_String &&
                pBd->blockCount <= msgAcc.available() &&
                pBd->blockCount < kBlocksPerChunk);
    }

private:
    BlockData * mpBlockData;
};

// Typedef to differentiate assets from strings (which are internally both CmpString's)
typedef CmpString CmpStringAsset;

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_CMPSTRING_H

