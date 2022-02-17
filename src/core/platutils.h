//------------------------------------------------------------------------------
// platutils.h - Misc platform specific code
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

//------------------------------------------------------------------------------
// Various platform specific stuff
//
// We implement our own since these are pretty simple we avoid pulling
// in more external cruft.
//------------------------------------------------------------------------------

#ifndef GAEN_CORE_PLATUTILS_H
#define GAEN_CORE_PLATUTILS_H

#include "core/base_defines.h"

namespace gaen
{

// Retruns if time has been initialized
bool is_time_init();

// Initialize any platform specific stuff required for calling time funcs
void init_time();

// Get time in secs since init_time was called
f64 now();

// Converts time to string.
// E.g.  2:15:30.123456   is 2 hours, 15 minutes, 30 secs, 123456 microsecs
bool time_to_str(char * str, size_t strLen, f32 timeSecs);

// Sleep the specified number of milliseconds
void sleep(u32 milliSecs);

// Number of cores on system.  Hyperthreads count as a core.
u32 platform_core_count();

// Set affinity of the calling thread to the core Id specified.
void set_thread_affinity(u32 coreId);

} // namespace gaen



#endif
