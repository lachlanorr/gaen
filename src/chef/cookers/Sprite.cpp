//------------------------------------------------------------------------------
// Sprite.cpp - Sprite cooker
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

#include "chef/stdafx.h"

#include "assets/file_utils.h"
#include "assets/Config.h"
#include "assets/Gspr.h"
#include "assets/Gatl.h"

#include "chef/cooker_utils.h"
#include "chef/CookInfo.h"
#include "chef/cookers/Sprite.h"

namespace gaen
{
namespace cookers
{

Sprite::Sprite()
{
    mVersion = 1;
    mRawExts.push_back(kExtSpr);
    mCookedExts.push_back(kExtGspr);
}

void Sprite::cook(CookInfo * pCookInfo) const
{
    Config<kMEM_Chef> spr;
    {
        FileReader rdr(pCookInfo->rawPath().c_str());
        PANIC_IF(!rdr.isOk(), "Unable to load file: %s", pCookInfo->rawPath().c_str());
        spr.read(rdr.ifs);
    }

    static const char * kAtlas = "atlas";
    static const char * kFrameSize = "size";
    static const char * kFrames = "frames";

    PANIC_IF(!spr.hasKey(kAtlas), "Missing atlas in .spr: %s", pCookInfo->rawPath().c_str());
    PANIC_IF(!spr.hasKey(kFrameSize), "Missing frame_size in .spr: %s", pCookInfo->rawPath().c_str());

    u32 animCount = 0;
    u32 totalFrameCount = 0;

    // Iterate over sections to discover how many animations and total
    // frames there are.
    for (auto secIt = spr.sectionsBegin();
         secIt != spr.sectionsEnd();
         secIt++)
    {
        if (!spr.isGlobalSection(*secIt) && spr.hasKey(*secIt, kFrames))
        {
            Config<kMEM_Chef>::IntVec frameVec = spr.getIntVec(*secIt, kFrames);
            PANIC_IF(frameVec.size() == 0, "No frames specified in .spr animation: %s, %s", *secIt, pCookInfo->rawPath().c_str());
            animCount++;
            totalFrameCount += (u32)frameVec.size();
        }
    }

    PANIC_IF(animCount == 0, "No animations in .spr: %s", pCookInfo->rawPath().c_str());
    PANIC_IF(totalFrameCount == 0, "No frames specified in .spr: %s", pCookInfo->rawPath().c_str());

    Config<kMEM_Chef>::IntVec frameSizeVec = spr.getIntVec(kFrameSize);
    PANIC_IF(frameSizeVec.size() != 2, "frame_size doesn't contain 2 ints in .spr: %s", pCookInfo->rawPath().c_str());

    const char * atlasPathSpr = spr.get(kAtlas);

    // Cook our dependent atlas so we can use it to lookup frames
    UniquePtr<CookInfo> pCiAtl = pCookInfo->cookDependency(atlasPathSpr);
    PANIC_IF(!pCiAtl.get() || !pCiAtl->results().front().isCooked(), "Failed to cook dependent file: %s", atlasPathSpr);
    const Gatl * pGatl = Gatl::instance(pCiAtl->results().front().pCookedBuffer.get(), pCiAtl->results().front().cookedBufferSize);

    Gspr * pGspr = Gspr::create(frameSizeVec[0],
                                frameSizeVec[1],
                                pCiAtl->results().front().gamePath.c_str(),
                                animCount,
                                totalFrameCount);
    ASSERT(pGspr);

    // Iterate over sections again to insert animation frame data into
    // the gspr.
    u32 currFrame = 0;
    AnimInfo * pAnimInfo = pGspr->anims();
    u32 * pFrames = pGspr->frames();
    for (auto secIt = spr.sectionsBegin();
         secIt != spr.sectionsEnd();
         secIt++)
    {
        if (!spr.isGlobalSection(*secIt) && spr.hasKey(*secIt, kFrames))
        {
            Config<kMEM_Chef>::StringVec frameVec = spr.getVec(*secIt, kFrames);
            PANIC_IF(frameVec.size() == 0, "No frames specified in .spr animation: %s, %s", pCookInfo->rawPath().c_str(), *secIt);

            pAnimInfo->animHash = gaen_hash(*secIt);
            pAnimInfo->frameCount = (u32)frameVec.size();
            pAnimInfo->firstFrame = currFrame;

            for (const char * fr : frameVec)
            {
                // Determine if it looks like a number or a name
                if (is_int(fr))
                {
                    // Verify frame is valid within atlas
                    i32 frameIdx = to_int(fr);
                    PANIC_IF(frameIdx < 0 || frameIdx >= (i32)pGatl->glyphCount(), "Invalid frame %d for .atl %s", frameIdx, atlasPathSpr);
                    *pFrames = (u32)frameIdx;
                }
                else
                {
                    // Lookup the index for this alias so we don't
                    // have to do that at runtime
                    u32 aliasHash = gaen_hash(fr);
                    u32 frameIdx = pGatl->glyphIndexFromAlias(aliasHash);
                    *pFrames = frameIdx;
                }

                pFrames++;
            }

            currFrame += pAnimInfo->frameCount;
            pAnimInfo++;
        }
    }

    ASSERT(Gspr::is_valid(pGspr, pGspr->size()));
    pCookInfo->setCookedBuffer(kExtGspr, pGspr, pGspr->size());
}

}
} // namespace gaen

