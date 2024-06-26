//------------------------------------------------------------------------------
// platutils_win32.cpp - Win32 versions of misc platform specific functions
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

#include "gaen/core/stdafx.h"

#include "gaen/core/logging.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "gaen/core/thread_local.h"
#include "gaen/core/threading.h"
#include "gaen/core/platutils.h"

namespace gaen
{

TL(i64, sStartTimeTicks) = 0;
TL(f64, sFrequencyRatio) = 0.0;

bool is_time_init()
{
    return sStartTimeTicks != 0 && sFrequencyRatio != 0.0;
}

void init_time()
{
    ASSERT(sStartTimeTicks == 0 && sFrequencyRatio == 0.0);

    LARGE_INTEGER freq;
    BOOL ret = QueryPerformanceFrequency(&freq);
    ASSERT(ret);

    sFrequencyRatio = 1.0 / freq.QuadPart;

    LARGE_INTEGER startTime;
    ret = QueryPerformanceCounter(&startTime);
    ASSERT(ret);
    sStartTimeTicks = startTime.QuadPart;
}

f64 now()
{
    ASSERT_MSG(sStartTimeTicks != 0 && sFrequencyRatio != 0.0, "init_time must be called first");

    LARGE_INTEGER nowTicks;
    BOOL ret = QueryPerformanceCounter(&nowTicks);
    ASSERT(ret);

    ASSERT_MSG(nowTicks.QuadPart > sStartTimeTicks, "Time has gone backwards");

    u64 delta = nowTicks.QuadPart - sStartTimeTicks;

    return delta * sFrequencyRatio;
}

TickCount now_ticks()
{
    LARGE_INTEGER ticks;
    BOOL ret = QueryPerformanceCounter(&ticks);
    ASSERT(ret);
    return ticks.QuadPart;
}

f64 ticks_to_secs(TickCount ticks)
{
    ASSERT_MSG(sStartTimeTicks != 0 && sFrequencyRatio != 0.0, "init_time must be called first");
    return ticks * sFrequencyRatio;
}

void sleep(u32 milliSecs)
{
    Sleep(milliSecs);
}

u32 platform_core_count()
{
    static u32 sCoreCount = 0;
    if (sCoreCount == 0)
    {
        // LORRTODO - Handle case where processor affinity is set to a subset of the cores
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        sCoreCount = info.dwNumberOfProcessors;
    }
    return sCoreCount;
}

void set_thread_affinity(u32 coreId)
{
    // LORRTODO - Handle case where processor affinity is set to a subset of the cores
    ASSERT(coreId < 64 && coreId < platform_core_count());

    HANDLE hThread = GetCurrentThread(); // can't use ti.thread.native_handle() since it may not be set yet

    // LORRTODO - Works on 32 bit? VS2015 gave warning about following line and I added (u64) casts
    DWORD_PTR affinityMask = (u64)1 << coreId;
    DWORD_PTR ret = SetThreadAffinityMask(hThread, affinityMask);

    if (ret == 0)
    {
        DWORD err = GetLastError();
        PANIC("Failed to SetThreadAffinityMask for coreId %d, err %d", coreId, err);
    }

    ASSERT_MSG(ret == (1 << platform_core_count()) - 1, "Gaen process doesn't have access to all cores, this is not currently supported");
}


// LORRTODO - Move these to their platform files when necessary
///* Linux */ 
//#include <sched.h> 
//int sched_getaffinity(pid_t pid, unsigned int cpusetsize, cpu_set_t 
//*mask); 
//
//static inline int num_processors() 
//{ 
//        unsigned int bit; 
//        int np; 
//        cpu_set_t aff; 
//        memset(&aff, 0, sizeof(aff) ); 
//        sched_getaffinity(0, sizeof(aff), &aff ); 
//        for(np = 0, bit = 0; bit < 8*sizeof(aff); bit++) 
//                np += (((char *)&aff)[bit / 8] >> (bit % 8)) & 1; 
//        return np; 
//} 
//
//
///* Mac OS X */ 
//#include <sys/types.h> 
//#include <sys/sysctl.h> 
//static inline int num_processors() 
//{ 
//        int np = 1; 
//        size_t length = sizeof( np ); 
//        sysctlbyname("hw.ncpu", &np, &length, NULL, 0); 
//        return np; 
//} 
//
//
///* Windows NT */ 
//#include <windows.h> 
//static inline int num_processors() 
//{ 
//        SYSTEM_INFO info; 
//        GetSystemInfo(&info); 
//        return info.dwNumberOfProcessors; 
//} 


} //namespace gaen
