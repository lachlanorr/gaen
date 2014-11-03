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

#include "core/base_defines.h"
#include "core/threading.h"
#include "core/HashMap.h"
#include "core/List.h"
#include "core/Vector.h"
#include "engine/FrameTime.h"
#include "engine/hashes.h"
#include "engine/Message.h"
#include "engine/Task.h"
#include "engine/Registry.h"

namespace gaen
{

class InputMgr;
class MessageQueue;
class Entity;

// Call this from main to prep one task master per thread and
// get them running.
void init_task_masters();

// Once main TaskMaster quits (i.e. runGameLoop exits)
// you should call this from main thread.
void fin_task_masters();

// Call this from main loop after all other initialization
// is complete.  It will start all threads, and their game
// loops.  This function will return immediately, main thread
// should be used for OS tasks, or go to sleep.
void start_game_loops();

Registry & get_registry();

// Get the correct message queue against which you should queue
MessageQueue & get_message_queue(u32 msgId,
                                 u32 flags,
                                 task_id source,
                                 task_id target);
// Broadcast a message to all TaskMasters
void broadcast_message(u32 msgId,
                       u32 flags,
                       task_id source,
                       cell payload = to_cell(0),
                       u32 blockCount = 0,
                       const Block * pBlocks = nullptr);
void broadcast_message(const MessageBlockAccessor & msgAcc);

class TaskMaster
{
public:
    void init(thread_id tid);
    void fin(const MessageQueueAccessor& msgAcc);
    void cleanup();

    static TaskMaster & task_master_for_thread(thread_id tid);
    static TaskMaster & task_master_for_active_thread();
    inline static TaskMaster & primary_task_master()
    {
        return task_master_for_thread(0);
    }

    // Get message queue for messages from main thread
    MessageQueue & mainMessageQueue()
    {
        ASSERT(mIsInit);
        ASSERT(active_thread_id() == kMainThreadId);
        return *mpMainMessageQueue;
    }

    // Get message queue for messages from another task master
    MessageQueue & taskMasterMessageQueue()
    {
        ASSERT(mIsInit);
        ASSERT(active_thread_id() < num_threads());
        return *mTaskMasterMessageQueues[active_thread_id()];
    }

    MessageQueue & messageQueueForTarget(task_id target);

    Registry & registry() { return mRegistry; }

    // Start the game loop.  This function won't return
    // until the TaskMaster gets shut down.
    void runPrimaryGameLoop();
    void runAuxiliaryGameLoop();

    // Register a path as a mutable data dependency,
    // i.e. the task can modify this data.
    // Any other task also registered for this data
    // must run within the same TaskMaster.
    void registerMutableDependency(task_id taskId, u32 path);

    // Deregister a task from a mutable data dependency
    void deregisterMutableDependency(task_id taskId, u32 path);

    void setRenderer(const Task & rendererTask)
    {
        ASSERT(mIsInit);
        ASSERT(mIsPrimary);
        mRendererTask = rendererTask;
    }

    thread_id threadId() { return mThreadId; }
    bool isPrimary() { return mIsPrimary; }

private:
    // Process any messages on the queue
    void processMessages(MessageQueue & msgQueue);

    MessageResult message(const MessageQueueAccessor& msgAcc);
    
    void insertTask(thread_id threadOwner, const Task & task);

    MessageQueue * mpMainMessageQueue; // messages from main queue here
    Vector<kMEM_Engine, MessageQueue*> mTaskMasterMessageQueues; // message from other task masters queue here

    // List of tasks owned by this TaskMaster
    typedef Vector<kMEM_Engine, Task> TaskVec;
    TaskVec mOwnedTasks;

    // Maps task_id to its index in mOwnedTasks
    typedef HashMap<kMEM_Engine, task_id, size_t> TaskMap;
    TaskMap mOwnedTaskMap;
    
    // Maps task_id to the TaskMaster's thread_id that owns it
    typedef HashMap<kMEM_Engine, task_id, thread_id> TaskOwnerMap;
    TaskOwnerMap mTaskOwnerMap;

    UniquePtr<InputMgr> mpInputMgr;
    Task mRendererTask;

    FrameTime mFrameTime;

    // Maps mutable data paths to the set of task_ids that depend on it
    // We maintain a reference count the data path has to the task
    typedef HashMap<kMEM_Engine, task_id, u32> DataToTaskRefs;
    typedef UniquePtr<DataToTaskRefs> DataToTaskRefsUP;
    typedef HashMap<kMEM_Engine, u32, DataToTaskRefsUP> MutableDataUsersMap;
    MutableDataUsersMap mMutableDataUsers;

    // Maps root task_id to the set of mutable data paths that it depends on
    // We maintain a reference count the root task_id has towards the data path
    typedef HashMap<kMEM_Engine, u32, u32> TaskToDataRefs;
    typedef UniquePtr<TaskToDataRefs> TaskToDataRefsUP;
    typedef HashMap<kMEM_Engine, task_id, TaskToDataRefsUP> MutableDataMap;
    MutableDataMap mMutableData;

    thread_id mThreadId = kInvalidThreadId;
    bool mIsPrimary = false; // primary task master has GPU, handles rendering/physics
    bool mIsInit = false;
    bool mIsRunning = false;

    Registry mRegistry;
};

} // namespace gaen



#endif // #ifndef GAEN_ENGINE_TASKMASTER_H
