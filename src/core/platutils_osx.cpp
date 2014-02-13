//------------------------------------------------------------------------------
// platutils_osx.cpp - OSX versiosn of misc platform specific functions
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#include "core/platutils.h"

namespace gaen
{

static const u64 kNanoPerMicro = 1000;

static u64 sStartTimeAbsolute = 0;
static mach_timebase_info_data_t sTimebaseInfoNanos = {0, 0};
static mach_timebase_info_data_t sTimebaseInfoMicros = {0, 0};

void init_time()
{
    ASSERT(sStartTimeAbsolute == 0);
    ASSERT(sTimebaseInfoMicros.denom == 0);

    sStartTimeAbsolute = mach_absolute_time();

    mach_timebase_info(&sTimebaseInfoNanos);
    mach_timebase_info(&sTimebaseInfoMicros);
    // Now adjust nanos to micros... no need to do the multiply each time
    sTimebaseInfoMicros.denom *= kNanoPerMicro;
}

u64 now_in_microsecs()
{
    ASSERT_MSG(sStartTimeAbsolute != 0, "init_time_utils must be called first");
    ASSERT_MSG(sTimebaseInfoMicros.denom != 0, "init_time_utils must be called first");
    
    u64 nowAbsolute = mach_absolute_time();
    u64 deltaAbsolute = nowAbsolute - sStartTimeAbsolute;

    u64 deltaMicros = deltaAbsolute * sTimebaseInfoMicros.numer / sTimebaseInfoMicros.denom;

    return deltaMicros;
}

u64 now_in_nanosecs()
{
    ASSERT_MSG(sStartTimeAbsolute != 0, "init_time_utils must be called first");
    ASSERT_MSG(sTimebaseInfoMicros.denom != 0, "init_time_utils must be called first");

    u64 nowAbsolute = mach_absolute_time();
    u64 deltaAbsolute = nowAbsolute - sStartTimeAbsolute;

    u64 deltaNanos = deltaAbsolute * sTimebaseInfoNanos.numer / sTimebaseInfoNanos.denom;

    return deltaNanos;
}

void sleep_nanosecs(u64 timeNanos)
{
    timespec ts = {0};
    ts.tv_nsec = timeNanos;
    int ret = nanosleep(&ts, nullptr);
    ASSERT(ret == 0);
}

size_t platform_core_count()
{
    int np = 1;
    size_t length = sizeof( np );
    int ret = sysctlbyname("hw.ncpu", &np, &length, NULL, 0);
    ASSERT(ret == 0);
    ASSERT(np > 0);
    return static_cast<size_t>(np);
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
