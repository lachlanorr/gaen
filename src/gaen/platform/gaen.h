//------------------------------------------------------------------------------
// gaen.h - Gaen game app
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

#ifndef GAEN_PLATFORM_GAEN_H
#define GAEN_PLATFORM_GAEN_H

#include "gaen/engine/TaskMaster.h"

namespace gaen
{

// Initialize arguments and logging
void init_gaen(int argc, char ** argv);

// Platform specific main function should call start_gaen once things are
// fully initialized.  It will start the TaskMaster threads.  Main thread
// is reserved for OS specific stuff, like event handling.
void start_gaen();

#ifndef IS_HEADLESS
// Set the renderer for the primary taskmaster
void set_renderer(const Task & rendererTask);
#endif // IS_HEADLESS

// Call this to shutdown TaskMasters when app wants to close
void fin_gaen();

void shutdown();

} // namespace gaen

#endif // #ifndef GAEN_PLATFORM_GAEN_H
