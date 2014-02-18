//------------------------------------------------------------------------------
// TaskMaster.h - Manages tasks, one of these runs per core
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

#ifndef GAEN_ENGINE_TASKMASTER_H
#define GAEN_ENGINE_TASKMASTER_H

#include "core/threading.h"
#include "core/HashMap.h"
#include "core/List.h"
#include "core/Vector.h"
#include "engine/FNV.h"
#include "engine/Message.h"
#include "engine/Task.h"

namespace gaen
{

class MessageQueue;
class TaskMaster;

// Call this from main to prep one task master per thread and
// get them running.
void init_task_masters();

// Once main TaskMaster quits (i.e. runGameLoop exits)
// you should call this from main thread.
void fin_task_masters();

// Call this from main loop after all other initialization
// is complete.  It will start all threads, and their game
// loops.  This function won't return until TaskMasters are
// stopped.
void start_game_loops();

TaskMaster & task_master_for_thread(thread_id tid);
void message_from_main(thread_id threadId,
                       fnv msgId);
void message_from_main(thread_id threadId,
                       fnv msgId,
                       cell payload,
                       const MessageBlock * pMsgBlock,
                       size_t msgBlockCount);

class TaskMaster
{
public:
    void init(thread_id tid);
    void fin();

    MessageQueue & mainMessageQueue()
    {
        ASSERT(mIsInit);
        ASSERT(active_thread_id() == kMainThreadId);
        return *mpMainThreadMessageQueue;
    }

    // Start the game loop.  This function won't return
    // until the TaskMaster gets shut down.
    void runGameLoop();

    // Register a path as a mutable data dependency,
    // i.e. the task can modify this data.
    // Any other task also registered for this data
    // must run within the same TaskMaster.
    void registerMutableDependency(task_id taskId, fnv path);

    // Deregister a task from a mutable data dependency
    void deregisterMutableDependency(task_id taskId, fnv path);
    

private:
    // Process any messages on the queue
    void processMessages(MessageQueue & msgQueue);

    MessageResult message(const MessageQueue::MessageAccessor& msgAcc);
    
    TaskMaster & taskOwner(task_id taskId);

    UniquePtr<MessageQueue> mpMainThreadMessageQueue;

    // List of tasks owned by this TaskMaster
    typedef Vector<Task, kMT_Engine> TaskVec;
    TaskVec mOwnedTasks;

    // Maps task_id to its Task
    typedef HashMap<task_id, Task&, kMT_Engine> TaskMap;
    TaskMap mTasks;
    
    // Maps task_id to the TaskMaster that owns it
    typedef HashMap<task_id, TaskMaster&, kMT_Engine> TaskOwnerMap;
    TaskOwnerMap mTaskOwners;

    // Maps mutable data paths to the set of task_ids that depend on it
    // We maintain a reference count the data path has to the task
    typedef HashMap<task_id, u32, kMT_Engine> DataToTaskRefs;
    typedef UniquePtr<DataToTaskRefs> DataToTaskRefsUP;
    typedef HashMap<fnv, DataToTaskRefsUP, kMT_Engine> MutableDataUsersMap;
    MutableDataUsersMap mMutableDataUsers;

    // Maps root task_id to the set of mutable data paths that it depends on
    // We maintain a reference count the root task_id has towards the data path
    typedef HashMap<fnv, u32, kMT_Engine> TaskToDataRefs;
    typedef UniquePtr<TaskToDataRefs> TaskToDataRefsUP;
    typedef HashMap<task_id, TaskToDataRefsUP, kMT_Engine> MutableDataMap;
    MutableDataMap mMutableData;

    thread_id mThreadId = kInvalidThreadId;
    bool mIsPrimary = false; // primary task master has GPU, handles rendering/physics
    bool mIsInit = false;
    bool mIsRunning = false;
};


} // namespace gaen

#endif // #ifndef GAEN_ENGINE_TASK_MASTER_H
