//------------------------------------------------------------------------------
// TaskMaster.h - Manages tasks, one of these runs per core
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

#ifndef GAEN_ENGINE_TASKMASTER_H
#define GAEN_ENGINE_TASKMASTER_H

#include <random>
#include <limits>
#include <thread>
#include <condition_variable>

#include "gaen/core/base_defines.h"
#include "gaen/core/threading.h"
#include "gaen/core/HashMap.h"
#include "gaen/core/List.h"
#include "gaen/core/Vector.h"
#include "gaen/engine/FrameTime.h"
#include "gaen/hashes/hashes.h"
#include "gaen/engine/Message.h"
#include "gaen/engine/Task.h"
#include "gaen/engine/Registry.h"

namespace gaen
{

class AssetMgr;
#ifndef IS_HEADLESS
class ModelMgr;
class SpriteMgr;
class AudioMgr;
class InputMgr;
class Editor;
#endif

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

void set_start_entity(const char * startEntity);

Registry & get_registry();

// Get the correct message queue against which you should queue
MessageQueue * get_message_queue(task_id source,
                                 task_id target);
// Broadcast a message to all TaskMasters
void broadcast_message(u32 msgId,
                       u32 flags,
                       task_id source,
                       cell payload = to_cell(0));
void broadcast_message(const MessageBlockAccessor & msgAcc);

// Broadcast a message to a target in each TaskMaster.
// Useful for propagating input messages to the InputMgr on each
// TaskMaster.
void broadcast_targeted_message(u32 msgId,
                                u32 flags,
                                task_id source,
                                task_id target,
                                cell payload = to_cell(0));
void broadcast_targeted_message(const MessageBlockAccessor & msgAcc);

void broadcast_insert_task(task_id source, thread_id owner, const Task & task);
void broadcast_remove_task(task_id source, task_id taskToRemove);
void broadcast_request_set_parent(task_id source,
                                  task_id parentTaskId,
                                  Entity * pChild);
void broadcast_confirm_set_parent(task_id source,
                                  thread_id parentOwner,
                                  task_id parentTaskId,
                                  Entity * pChild);

bool is_target_on_same_taskmaster(task_id source, task_id target);

void notify_next_frame();


class TaskMaster
{
public:
    void init(thread_id tid);

    template <typename T>
    void fin(const T& msgAcc);
    void initiateFin();

    void cleanup();

    static TaskMaster & task_master_for_thread(thread_id tid);
    static TaskMaster & task_master_for_active_thread();
    inline static TaskMaster & primary_task_master()
    {
        return task_master_for_thread(0);
    }

    // Get message queue for messages from another task master
    MessageQueue & taskMasterMessageQueue()
    {
        ASSERT(mStatus >= kTMS_Initialized);
        ASSERT(active_thread_id() < num_threads());
        return *mTaskMasterMessageQueues[active_thread_id()];
    }

    MessageQueue * messageQueueForTarget(task_id target);

    void notifyNextFrame();

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

    // De-register a task from a mutable data dependency
    void deregisterMutableDependency(task_id taskId, u32 path);

#ifndef IS_HEADLESS
    void setRenderer(const Task & rendererTask)
    {
        ASSERT(mStatus == kTMS_Initialized);
        ASSERT(mIsPrimary);
        mRendererTask = rendererTask;
    }
#endif

    void setPlatformTask(const Task & platformTask)
    {
        ASSERT(mStatus == kTMS_Initialized);
        ASSERT(mIsPrimary);
        mPlatformTask = platformTask;
    }

#ifndef IS_HEADLESS
    InputMgr & inputMgr() { return *mpInputMgr; }
#endif

    thread_id threadId() { return mThreadId; }
    bool isPrimary() { return mIsPrimary; }

    bool isOwnedTask(task_id taskId);

    f32 rand() { return (f32)(mRandom() / (f64)std::numeric_limits<u32>::max()); }

    template <typename T>
    MessageResult message(const T& msgAcc);

private:
    enum TaskMasterStatus
    {
        kTMS_Uninitialized = 0,
        kTMS_Initialized   = 1,
        kTMS_Finalizing    = 2,
        kTMS_Shutdown      = 3
    };

    // Process any messages on the queue
    void processMessages(MessageQueue & msgQueue);

    void insertTask(thread_id threadOwner, const Task & task);
    void removeTask(task_id taskId);
    void setTaskOwner(thread_id newOwner, const Task & task);

    Vector<kMEM_Engine, MessageQueue*> mTaskMasterMessageQueues; // message from other task masters queue here

    void waitForNextFrame();
    std::condition_variable mNextFrameCV;
    std::mutex mNextFrameMtx;

    // List of tasks owned by this TaskMaster
    typedef Vector<kMEM_Engine, Task> TaskVec;
    TaskVec mOwnedTasks;

    // Maps task_id to its index in mOwnedTasks
    typedef HashMap<kMEM_Engine, task_id, size_t> TaskMap;
    TaskMap mOwnedTaskMap;

    // Maps task_id to the TaskMaster's thread_id that owns it
    typedef HashMap<kMEM_Engine, task_id, thread_id> TaskOwnerMap;
    TaskOwnerMap mTaskOwnerMap;

    UniquePtr<AssetMgr> mpAssetMgr;

#ifndef IS_HEADLESS
	UniquePtr<ModelMgr> mpModelMgr;
    UniquePtr<SpriteMgr> mpSpriteMgr;
    UniquePtr<AudioMgr> mpAudioMgr;
    UniquePtr<InputMgr> mpInputMgr;
#if HAS(ENABLE_EDITOR)
    UniquePtr<Editor> mpEditor;
#endif
    Task mRendererTask;
#endif // IS_HEADLESS

    Task mPlatformTask;

    FrameTime mFrameTime;

    std::mt19937 mRandom;

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
    bool mIsRunning = false;
    bool mIsPaused = false;
	bool mIsFinInitiated = false;

    TaskMasterStatus mStatus = kTMS_Uninitialized;
    u32 mShutdownCount = 0; // used when shutting down to count how many peer TaskMasters have finalized

    // Storage for initial entity
    Entity * mpStartEntity = nullptr;

    Registry mRegistry;
};

template <typename T>
MessageResult send_message(const T& msg)
{
    return TaskMaster::task_master_for_active_thread().message(msg.accessor());
}

namespace system_api
{
void exit(Entity * pCaller);
} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_TASKMASTER_H
