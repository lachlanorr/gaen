//------------------------------------------------------------------------------
// FrameTime.h - Calculates time deltas and tracks frame rate stats
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

#ifndef GAEN_ENGINE_FRAME_TIME_H
#define GAEN_ENGINE_FRAME_TIME_H

//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#include "gaen/core/base_defines.h"
#include "gaen/core/platutils.h"

namespace gaen
{

struct FrameInfo
{
    f64 last10;
    f64 last100;
    f64 last1000;
    f64 last10000;
};

template <u64 count>
class FrameStats
{
public:
    void init()
    {
        for (u64 i = 0; i < count; ++i)
        {
            mDeltas[i] = 0.0;
        }
        mSum = 0.0;
        mFrameCount = 0;
    }

    void addFrame(f64 delta)
    {
        u64 index = mFrameCount % count;

        mSum -= mDeltas[index];
        mDeltas[index] = delta;
        mSum += delta;

        mFrameCount++;
    }

    f64 deltaMean() const
    {
        if (mFrameCount >= count)
            return mSum / count;
        else
            return mSum / mFrameCount;
    }

    f64 fps() const
    {
        return 1.0 / deltaMean();
    }

private:
    f64 mDeltas[count];
    f64 mSum = 0.0;
    u64 mFrameCount = 0;
};



class FrameTime
{
public:
    void init()
    {
        mFrameCount = 0;
        mLastTickCount = now_ticks();

        mFrameStats10.init();
        mFrameStats100.init();
        mFrameStats1000.init();
        mFrameStats10000.init();

        mIsInit = true;
    }

    u64 frameCount() { return mFrameCount; }

    void resetLastFrameTime()
    {
        mLastTickCount = now_ticks();
    }

    f64 calcDelta()
    {
        ASSERT(mIsInit);

        TickCount tickCount = now_ticks();
        TickCount tickCountDelta = tickCount - mLastTickCount;
        mLastTickCount = tickCount;
        f64 delta = ticks_to_secs(tickCountDelta);

        mFrameStats10.addFrame(delta);
        mFrameStats100.addFrame(delta);
        mFrameStats1000.addFrame(delta);
        mFrameStats10000.addFrame(delta);

        mFrameCount++;

        return delta;
    }

    f64 deltaMeanLast10()
    {
        return mFrameStats10.deltaMean();
    }

    f64 deltaMeanLast100()
    {
        return mFrameStats100.deltaMean();
    }

    f64 deltaMeanLast1000()
    {
        return mFrameStats1000.deltaMean();
    }

    f64 deltaMeanLast10000()
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

    f64 fpsLast10() const
    {
        return mFrameStats10.fps();
    }

    f64 fpsLast100() const
    {
        return mFrameStats100.fps();
    }

    f64 fpsLast1000() const
    {
        return mFrameStats1000.fps();
    }

    f64 fpsLast10000() const
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

    u64 mFrameCount = 0;
    TickCount mLastTickCount = 0;

    FrameStats<10>    mFrameStats10;
    FrameStats<100>   mFrameStats100;
    FrameStats<1000>  mFrameStats1000;
    FrameStats<10000> mFrameStats10000;
};

} // namespace gaen


#endif // #ifndef GAEN_ENGINE_FRAME_TIME_H
