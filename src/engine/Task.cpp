//------------------------------------------------------------------------------
// Task.cpp - Base definition of an updatable task
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

#include "engine/stdafx.h"

#include <atomic>

#include "engine/Task.h"

namespace gaen
{

task_id next_task_id()
{
    // LORRTODO - When we support networking someday, we need to generate
    // globally unique ids across all network clients.  One strategy is to
    // have each client get a range of ids they can use, in 100k chunks,
    // for example.  When they run out of these they can request more.
    // The central server will manage dolling out these chunks and make
    // sure there is no overlap in the ranges.
    static std::atomic<u32> nextId{kMaxThreads}; // 0-kMaxThreads are reserved for TaskManagers
    task_id taskId = nextId++;
    ASSERT(is_valid_task_id(taskId));
    return taskId;
}

} // namespace gaen
