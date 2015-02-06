//------------------------------------------------------------------------------
// FrameTime.h - Calculates timee deltas ans tracks frame rate stats
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#ifndef GAEN_ENGINE_FRAME_TIME_H
#define GAEN_ENGINE_FRAME_TIME_H

#include "core/base_defines.h"
#include "core/platutils.h"

namespace gaen
{

struct FrameInfo
{
    f32 last10;
    f32 last100;
    f32 last1000;
    f32 last10000;
};

template <u32 count>
class FrameStats
{
public:
    void init()
    {
        for (u32 i = 0; i < count; ++i)
        {
            mDeltas[i] = 0.0f;
        }
        mSum = 0.0f;
        mFrameCount = 0;
    }

    void addFrame(f32 deltaSecs)
    {
        u32 index = mFrameCount % count;

        mSum -= mDeltas[index];
        mDeltas[index] = deltaSecs;
        mSum += deltaSecs;

        mFrameCount++;
    }

    f32 deltaMean()
    {
        if (mFrameCount >= count)
            return mSum / count;
        else
            return mSum / mFrameCount;
    }
    
    f32 fps()
    {
        return 1.0f / deltaMean();
    }

private:
    f32 mDeltas[count];
    f32 mSum = 0.0f;
    u32 mFrameCount = 0;
};



class FrameTime
{
public:
    void init()
    {
        mFrameCount = 0;
        mLastFrameTime = now();

        mFrameStats10.init();
        mFrameStats100.init();
        mFrameStats1000.init();
        mFrameStats10000.init();

        mIsInit = true;
    }

    u32 frameCount() { return mFrameCount; }
        
    f32 calcDelta()
    {
        ASSERT(mIsInit);
        
        f32 startFrameTime = now();
        f32 deltaSecs = startFrameTime - mLastFrameTime;
        mLastFrameTime = startFrameTime;

        mFrameStats10.addFrame(deltaSecs);
        mFrameStats100.addFrame(deltaSecs);
        mFrameStats1000.addFrame(deltaSecs);
        mFrameStats10000.addFrame(deltaSecs);

        mFrameCount++;

        return deltaSecs;
    }

    f32 deltaMeanLast10()
    {
        return mFrameStats10.deltaMean();
    }

    f32 deltaMeanLast100()
    {
        return mFrameStats100.deltaMean();
    }

    f32 deltaMeanLast1000()
    {
        return mFrameStats1000.deltaMean();
    }

    f32 deltaMeanLast10000()
    {
        return mFrameStats10000.deltaMean();
    }

    void deltaMean(FrameInfo * pDeltas)
    {
        pDeltas->last10   = mFrameStats10.deltaMean();
        pDeltas->last100  = mFrameStats100.deltaMean();
        pDeltas->last1000 = mFrameStats1000.deltaMean();
        pDeltas->last10000 = mFrameStats10000.deltaMean();
    }

    f32 fpsLast10()
    {
        return mFrameStats10.fps();
    }

    f32 fpsLast100()
    {
        return mFrameStats100.fps();
    }

    f32 fpsLast1000()
    {
        return mFrameStats1000.fps();
    }

    f32 fpsLast10000()
    {
        return mFrameStats10000.fps();
    }

    void fps(FrameInfo * pDeltas)
    {
        pDeltas->last10   = mFrameStats10.fps();
        pDeltas->last100  = mFrameStats100.fps();
        pDeltas->last1000 = mFrameStats1000.fps();
        pDeltas->last10000 = mFrameStats10000.fps();
    }

private:
    bool mIsInit = false;

    u32 mFrameCount = 0;
    f32 mLastFrameTime;

    FrameStats<10>    mFrameStats10;
    FrameStats<100>   mFrameStats100;
    FrameStats<1000>  mFrameStats1000;
    FrameStats<10000> mFrameStats10000;
};

} // namespace gaen


#endif // #ifndef GAEN_ENGINE_FRAME_TIME_H
