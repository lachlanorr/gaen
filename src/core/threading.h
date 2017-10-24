//------------------------------------------------------------------------------
// threading.h - Wrapper of C++ threads which manages Gaen thread specifics
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

//------------------------------------------------------------------------------
// Thread management
//
// Threads in Gaen must follow some rules:
//
// 1. There should be exactly one thread per core (fewer's ok for testing).
// 2. Before any are created, you must call init_threading from main thread.
// 3. Each thread must be started by calling start_thread.
//
// Threads you create by other means will affect the performance of the system
// and creating them should not be taken lightly.
//
// Thread affinity is set for CPUs, one thread per CPU if the platform
// supports it.
//------------------------------------------------------------------------------

#ifndef GAEN_CORE_THREADING_H
#define GAEN_CORE_THREADING_H

#include <thread>
#include <utility>

#include "core/base_defines.h"
#include "core/platutils.h"

namespace gaen
{

typedef u32 thread_id;

// The primary thread is special. It runs the renderer and input
// routines.  In many places we need to identify it by its thread_id.
static const thread_id kPrimaryThreadId   =  0;

// By default, each thread starts with this as its thread_id
static const thread_id kInvalidThreadId   = -1;

// When we want to broadcast a message to all TaskMasters, we use
// this.
static const thread_id kBroadcastThreadId = -2;

// Main thread isn't running a TaskMaster, but we identify it to make
// we know when it is calling various functions.
static const thread_id kMainThreadId      = -3;


static const thread_id kMinThreads = 1;   // doesn't include main, main has its own thread
static const thread_id kMaxThreads = 16;  // doesn't include main, main has its own thread

struct ThreadInfo
{
    thread_id threadId = kInvalidThreadId;
    bool isRunning = false;
    std::thread thread;
};

// NOTE: Make sure int_threading and fin_threading are called from the
// main thread.
void init_threading(thread_id numThreads);
void fin_threading();

bool is_threading_init();

thread_id num_threads();

// Threads can call these functions to get their own values.
thread_id active_thread_id();
ThreadInfo & active_thread_info();

void set_active_thread_id(thread_id tid);

// Thread id 0 is always the main thread
inline bool is_active_main_thread() { return thread_id() == 0; }

// Internal funcs, clients don't call these.
ThreadInfo & init_main_thread();
ThreadInfo & init_thread();

void join_all_threads();

// Wrapper class to start with the C++ thread, ensures we call
// fin_thread automagically.
template <class Function, class ...Args>
void thread_wrapper(thread_id tid,
                    Function threadFunc,
                    Args&&... args)
{
    // Now our thread is actually running, set the tThreadId
    set_active_thread_id(tid);

    threadFunc(std::forward<Args>(args)...);
}

// Clients call this to start a new Gaen thread
template <class Function, class ...Args>
thread_id start_thread(Function threadFunc,
                       Args&&... args)
{
    ASSERT(is_threading_init());
    ThreadInfo & ti = init_thread();
    ti.thread = std::thread(thread_wrapper<Function, Args...>, ti.threadId, threadFunc, std::forward<Args>(args)...);
    return ti.threadId;
}
                      

} // namespace gaen

#endif // #ifndef GAEN_CORE_THREADING_H
