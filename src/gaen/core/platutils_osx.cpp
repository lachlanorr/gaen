//------------------------------------------------------------------------------
// platutils_osx.cpp - OSX versiosn of misc platform specific functions
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

#include <time.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/mach_time.h>

#include "gaen/core/thread_local.h"
#include "gaen/core/platutils.h"

namespace gaen
{

static const u64 kNanoPerMicro = 1000;
static const f64 kMicrosPerSec = 1000000.0;

TL(u64, sStartTimeAbsolute) = 0;
TL(u32, sTimebaseNumer) = 0;
TL(u32, sTimebaseDenom) = 0;

void init_time()
{
    ASSERT(sStartTimeAbsolute == (u64)0);

    sStartTimeAbsolute = mach_absolute_time();

    mach_timebase_info_data_t timebaseInfo = {0, 0};
    mach_timebase_info(&timebaseInfo);

    sTimebaseNumer = timebaseInfo.numer;
    // Now adjust nanos to micros... no need to do the multiply each time
    sTimebaseDenom = timebaseInfo.denom *= kNanoPerMicro;
}

f32 now()
{
    ASSERT_MSG(sStartTimeAbsolute != 0, "init_time_utils must be called first");
    ASSERT_MSG(sTimebaseDenom != 0, "init_time_utils must be called first");
    
    u64 nowAbsolute = mach_absolute_time();
    u64 deltaAbsolute = nowAbsolute - sStartTimeAbsolute;

    u64 deltaMicros = deltaAbsolute * sTimebaseNumer / sTimebaseDenom;

    return static_cast<f32>(deltaMicros / kMicrosPerSec);
}

void sleep(u32 milliSecs)
{
    u64 timeNanos = milliSecs * 1000;
    timespec ts = {0};
    ts.tv_nsec = timeNanos;
    int ret = nanosleep(&ts, nullptr);
    ASSERT(ret == 0);
}

u32 platform_core_count()
{
    int np = 1;
    size_t length = sizeof( np );
    int ret = sysctlbyname("hw.ncpu", &np, &length, NULL, 0);
    ASSERT(ret == 0);
    ASSERT(np > 0);
    return static_cast<u32>(np);
}

void set_thread_affinity(u32 coreId)
{
    // LORRTODO: Don't think there's a way to do this on OSX
    return; 
}

} //namespace gaen
