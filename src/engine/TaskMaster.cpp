//------------------------------------------------------------------------------
// TaskMaster.cpp - Manages tasks, one of these runs per core
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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

#include "core/platutils.h"
#include "core/logging.h"
#include "core/gamevars.h"

#include "hashes/hashes.h"
#include "engine/MessageQueue.h"
#include "engine/Entity.h"
#include "engine/messages/OwnerTask.h"
#include "engine/messages/TaskStatus.h"
#include "engine/messages/TaskEntity.h"
#include "engine/InputMgr.h"
#include "engine/AssetMgr.h"
#include "engine/Editor.h"
#include "render_support/ModelMgr.h"
#include "render_support/SpriteMgr.h"
#include "render_support/renderer_api.h"

#include "engine/TaskMaster.h"

#define LOG_FPS HAS__

GAMEVAR_DECL_FLOAT(min_render_interval, 0.0f);

namespace gaen
{
extern void register_all_entities_and_components(Registry & registry);

// LORRTODO - Choose good message queue sizes here
static const u32 kMaxMainMessages = 4096;
static const u32 kMaxTaskMasterMessages = 4096;
static u32 sStartEntityHash = HASH::init__Start;

static bool sIsInit = false;

void init_task_masters(bool isEditorActive)
{
    ASSERT(!sIsInit);

    for (thread_id tid = 0; tid < num_threads(); ++tid)
    {
        TaskMaster & tm = TaskMaster::task_master_for_thread(tid);
        tm.init(tid, isEditorActive);
    }

    sIsInit = true;
}

void fin_task_masters()
{
    ASSERT(sIsInit);
    broadcast_message(HASH::fin,
                      kMessageFlag_ForcePropagate,
                      kMainThreadTaskId);
    join_all_threads();
}

// Entry point of our thread
static void start_game_loop()
{
    ASSERT(sIsInit);
    init_time();

    thread_id tid = active_thread_id();
    ASSERT(tid >= 0 && tid < num_threads());

    set_thread_affinity(tid);

    LOG_INFO("Starting thread: %d", tid);

    TaskMaster & tm = TaskMaster::task_master_for_active_thread();

    if (tm.isPrimary())
        tm.runPrimaryGameLoop();
    else
        tm.runAuxiliaryGameLoop();

    tm.cleanup();
}

void start_game_loops()
{
    ASSERT(sIsInit);
    thread_id numThreads = num_threads();

    // Start a TaskMaster for every thread
    for (thread_id i = 0; i < numThreads; ++i)
    {
        start_thread(start_game_loop);
    }

}

void set_start_entity(const char * startEntity)
{
    static const u32 kMaxEntityName = 96;
    char scratch[kMaxEntityName];

    char * str = scratch;
    const char * endStr = scratch + kMaxEntityName;

    // replace '.' with '__'
    while (*startEntity && (str < endStr - 1))
    {
        if (*startEntity == '.')
        {
            if (str >= endStr - 1)
                break;
            *str++ = '_';
            *str++ = '_';
        }
        else
            *str++ = *startEntity;
        startEntity++;
    }

    *str = '\0';

    sStartEntityHash = HASH::hash_func(scratch);
}

Registry & get_registry()
{
    ASSERT(sIsInit);
    TaskMaster & tm = TaskMaster::task_master_for_active_thread();
    return tm.registry();
}

MessageQueue * get_message_queue(task_id source,
                                 task_id target)
{
    ASSERT(sIsInit);

    if (source != kMainThreadTaskId)
    {
        TaskMaster & tm = TaskMaster::task_master_for_active_thread();
        MessageQueue * pMsgQ = tm.messageQueueForTarget(target);
        if (pMsgQ)
        {
            return pMsgQ;
        }
        else
        {
            // Default to the message queue of the active thread's
            // task master.  In some cases, if a task was just created
            // but hasn't been inserted, it is valid for this
            // scenario.
            return &tm.taskMasterMessageQueue();
        }
    }
    else // main thread sent this message
    {
        thread_id targetThreadId = -1;
        // Check to see of target is a "standard" reserved target
        if (is_primary_task(target))
        {
            targetThreadId = kPrimaryThreadId;
        }
        else
        {
            // Not a standard target so assume "target" is the thread id of a taskmaster
            targetThreadId = static_cast<thread_id>(target);
            ASSERT(targetThreadId < num_threads());
        }

        ASSERT(targetThreadId != -1);

        TaskMaster & tm = TaskMaster::task_master_for_thread(targetThreadId);
        return &tm.mainMessageQueue();
    }
}

void broadcast_message(u32 msgId,
                       u32 flags,
                       task_id source,
                       cell payload,
                       u32 blockCount,
                       const Block * pBlocks)
{
    ASSERT(sIsInit);
    ASSERT(source != kMainThreadTaskId || active_thread_id() == kMainThreadId);

    for (thread_id tid = 0; tid < num_threads(); ++tid)
    {
        MessageQueueWriter msgw(msgId,
                                flags,
                                source,
                                tid, // in this special case, thread_id is used in place of task_id to indicate the message is for a TaskMaster
                                payload,
                                blockCount);
        MessageQueueAccessor & msgAcc = msgw.accessor();
        for (u32 i = 0; i < blockCount; ++i)
        {
            msgAcc[i] = pBlocks[i];
        }
    }
}

void broadcast_targeted_message(u32 msgId,
                                u32 flags,
                                task_id source,
                                task_id target,
                                cell payload,
                                u32 blockCount,
                                const Block * pBlocks)
{
    ASSERT(sIsInit);
    ASSERT(active_thread_id() == kMainThreadId || active_thread_id() < num_threads());

    for (thread_id tid = 0; tid < num_threads(); ++tid)
    {
        TaskMaster & targetTaskMaster = TaskMaster::task_master_for_thread(tid);

        MessageQueue * pMsgQueue = nullptr;
        if (active_thread_id() == kMainThreadId)
            pMsgQueue = &targetTaskMaster.mainMessageQueue();
        else
            pMsgQueue = &targetTaskMaster.taskMasterMessageQueue();

        MessageQueueWriter msgw(msgId,
                                flags,
                                source,
                                target,
                                payload,
                                blockCount,
                                pMsgQueue);

        MessageQueueAccessor & msgAcc = msgw.accessor();
        for (u32 i = 0; i < blockCount; ++i)
        {
            msgAcc[i] = pBlocks[i];
        }
    }
}

void broadcast_insert_task(task_id source, thread_id owner, const Task & task)
{
    // Insert Entity into the TaskMasters
    messages::OwnerTaskBW msgInsertTask(HASH::insert_task__,
                                        kMessageFlag_None,
                                        source,
                                        active_thread_id(),
                                        owner);
    msgInsertTask.setTask(task);
    broadcast_message(msgInsertTask.accessor());
}

void broadcast_remove_task(task_id source, task_id taskToRemove)
{
    broadcast_message(HASH::remove_task__, kMessageFlag_None, source, to_cell(taskToRemove));
}

void broadcast_request_set_parent(task_id source,
                                  task_id parentTaskId,
                                  Entity * pChild)
{
    // Request for parenting to occur.
    //
    // We must first broadcast a request_set_parent so that every
    // TaskMaster is aware of the parenting, and move the child to the
    // parents TaskMaster if necessary.
    //
    // Then, the TaskMaster that owns us will send a
    // HASH::insert_child__ so we can complete the parenting.
    messages::TaskEntityBW msg(HASH::request_set_parent__,
                               kMessageFlag_None,
                               source,
                               active_thread_id(),
                               parentTaskId);
    msg.setEntity(pChild);
    broadcast_message(msg.accessor());
}

void broadcast_confirm_set_parent(task_id source,
                                  thread_id parentOwner,
                                  task_id parentTaskId,
                                  Entity * pChild)
{
    messages::TaskEntityBW msgw(HASH::confirm_set_parent__,
                                kMessageFlag_None,
                                source,
                                parentOwner,
                                parentTaskId);
    msgw.setEntity(pChild);
    broadcast_message(msgw.accessor());
}

void broadcast_message(const MessageBlockAccessor & msgAcc)
{
    broadcast_message(msgAcc.message().msgId,
                      msgAcc.message().flags,
                      msgAcc.message().source,
                      msgAcc.message().payload,
                      msgAcc.message().blockCount,
                      &msgAcc[0]);
}

void notify_next_frame()
{
    ASSERT(sIsInit);
    for (thread_id tid = 0; tid < num_threads(); ++tid)
    {
        TaskMaster & tm = TaskMaster::task_master_for_thread(tid);
        if (!tm.isPrimary())
        {
            tm.notifyNextFrame();
        }
    }
}

void TaskMaster::init(thread_id tid, bool isEditorActive)
{
    ASSERT(mStatus == kTMS_Uninitialized);
    mThreadId = tid;
    mIsPrimary = tid == kPrimaryThreadId;

    mIsEditorEnabled = mIsEditorActive = isEditorActive;

    // initialize mRenderTask to be blank for now
    mRendererTask = Task::blank();

    // Allocate a message queue that the main thread can use to communicate with us
    mpMainMessageQueue = GNEW(kMEM_Engine, MessageQueue, kMaxMainMessages);

    for (size_t i = 0; i < num_threads(); ++i)
    {
        mTaskMasterMessageQueues.push_back(GNEW(kMEM_Engine, MessageQueue, kMaxTaskMasterMessages));
    }

    // Pre-allocate reasonable sizes for hash tables
    // LORRTODO - these should be command line options
    const u32 kEstimatedTaskCount = 65536;
    const u32 kEstimatedMutableDataCount = 128;
    mOwnedTasks.reserve(kEstimatedTaskCount);
    mOwnedTaskMap.reserve(kEstimatedTaskCount);
    mTaskOwnerMap.reserve(kEstimatedTaskCount);
    mMutableDataUsers.reserve(kEstimatedMutableDataCount);
    mMutableData.reserve(kEstimatedTaskCount);

    register_all_entities_and_components(mRegistry);

    mStatus = kTMS_Initialized;
}

void TaskMaster::fin(const MessageQueueAccessor& msgAcc)
{
    ASSERT(mStatus == kTMS_Initialized);
    ASSERT(msgAcc.message().msgId == HASH::fin);

    for (Task & task : mOwnedTasks)
    {
        task.message(msgAcc);
    }

    // Entities should have deleted themselves and all dependent memory
    mOwnedTaskMap.clear();
    mOwnedTasks.clear();

    mStatus = kTMS_Finalizing;
    ASSERT(mShutdownCount == 0);
    mShutdownCount = 0;
    broadcast_message(HASH::shutdown__, kMessageFlag_None, threadId());

    if (isPrimary())
        notify_next_frame();
}


void TaskMaster::cleanup()
{
    // We can't do this cleanup in the fin() method, since message queues
    // are still active.
    // This cleanup will get called when loop actually exits and we can
    // really clean up everything.

    ASSERT(mStatus == kTMS_Finalizing);

    if (mRendererTask.id() != 0)
        renderer_fin(mRendererTask);

    GDELETE(mpMainMessageQueue);
    for (MessageQueue * pMessageQueue : mTaskMasterMessageQueues)
    {
        GDELETE(pMessageQueue);
    }
    mStatus = kTMS_Shutdown;
}


TaskMaster & TaskMaster::task_master_for_thread(thread_id tid)
{
    ASSERT(is_threading_init());
    ASSERT(tid >= 0 && tid < num_threads());
    static Vector<kMEM_Engine, TaskMaster> sTaskMasters(num_threads());
    ASSERT(tid < sTaskMasters.size());
    return sTaskMasters[tid];
}

TaskMaster & TaskMaster::task_master_for_active_thread()
{
    thread_id activeThreadId = active_thread_id();
    ASSERT(activeThreadId < num_threads());
    return task_master_for_thread(activeThreadId);
}

MessageQueue * TaskMaster::messageQueueForTarget(task_id target)
{
    thread_id targetThreadId;

    // Special case the Renderer and InputManager since they
    // are so common.
    if (is_primary_task(target))
    {
        targetThreadId = kPrimaryThreadId;
    }
    else if (target < kMaxThreads)
    {
        // implicit thread id as target
        targetThreadId = target;
    }
    else
    {
        // Lookup the task in our map to see which task manager it
        // belongs to.
        TaskOwnerMap::iterator it = mTaskOwnerMap.find(target);

        if (it == mTaskOwnerMap.end())
        {
            // This task_id isn't registered with any task masters.
            // This can sometimes happen if the task was just created
            // and the insert_task message hasn't propagated through
            // yet. Callers of this method should decide what to do in
            // these cases, as it is not always an error to get
            // nullptr returned from here.
            return nullptr;
        }

        targetThreadId = it->second;
    }

    ASSERT(targetThreadId < num_threads());
    TaskMaster & targetTaskMaster = TaskMaster::task_master_for_thread(targetThreadId);
    return &targetTaskMaster.taskMasterMessageQueue();
}

void TaskMaster::notifyNextFrame()
{
    mNextFrameCV.notify_one();
}

void TaskMaster::waitForNextFrame()
{
    std::unique_lock<std::mutex> lk(mNextFrameMtx);
    mNextFrameCV.wait(lk);
}

void TaskMaster::runPrimaryGameLoop()
{
    ASSERT(mStatus == kTMS_Initialized);
    ASSERT(mIsPrimary && mRendererTask.id() != 0);
    ASSERT(!mIsRunning);

    mpInputMgr.reset(GNEW(kMEM_Engine, InputMgr, isPrimary()));
    mpAssetMgr.reset(GNEW(kMEM_Engine, AssetMgr, 4));

    // LORRNOTE: SpriteMgr should be started on all TaskMasters and
    // the broadcast HASH::sprite_insert messages can be handled by
    // the appropriate TaskMaster
    mpModelMgr.reset(GNEW(kMEM_Engine, ModelMgr));
    mpSpriteMgr.reset(GNEW(kMEM_Engine, SpriteMgr));

    if (mIsEditorEnabled)
        mpEditor.reset(GNEW(kMEM_Engine, Editor, mIsEditorActive));

    renderer_init_device(mRendererTask);
    renderer_init_viewport(mRendererTask);

    // LORRTODO - make start entity name dynamic based on command line args
    // Init the start entity now that we have a TaskMaster running.
    mpStartEntity = Entity::activate_start_entity(sStartEntityHash);

    if (mpStartEntity)
        LOG_INFO("Start entity: %s", HASH::reverse_hash(sStartEntityHash));
    else
        LOG_ERROR("Unable to start entity: %s", HASH::reverse_hash(sStartEntityHash));

    mIsRunning = true;
    mFrameTime.init();

    f32 timeSinceRender = 0.0f;
    bool didRender = false;

    while(mIsRunning)
    {
        // Render through the render adapter
        if (timeSinceRender > min_render_interval)
        {
            renderer_render(mRendererTask);
            didRender = true;
            timeSinceRender = 0;
        }

#if HAS(LOG_FPS)
        if (mFrameTime.frameCount() % 100 == 0)
        {
            FrameInfo fi;
            mFrameTime.fps(&fi);
            LOG_INFO("TaskMasterFPS %d: %d tasks, %f, %f, %f, %f",
                     mThreadId,
                     mOwnedTasks.size(),
                     fi.last10,
                     fi.last100,
                     fi.last1000,
                     fi.last10000);
        }
#endif
        // process messages accumulated since last frame
        processMessages(*mpMainMessageQueue); // messages from main thread
        // messages from other TaskMasters or ourself
        for (MessageQueue * pMessageQueue : mTaskMasterMessageQueues)
        {
            processMessages(*pMessageQueue);
        }

        // Update physics (inside the Mgrs)
        if (mStatus == kTMS_Initialized)
        {
            mpModelMgr->update();
            mpSpriteMgr->update();
        }

        // process messages accumulated since last frame
        processMessages(*mpMainMessageQueue); // messages from main thread
        // messages from other TaskMasters or ourself
        for (MessageQueue * pMessageQueue : mTaskMasterMessageQueues)
        {
            processMessages(*pMessageQueue);
        }

        // Get delta since the last time we ran.
        // Use a min value to avoid issues when we are debugging for several seconds
        // within a frame, 0.5s should be a reasonable max for a frame.
        f32 delta = mFrameTime.calcDelta(); // glm::min(0.5f, mFrameTime.calcDelta());
        timeSinceRender += delta;
        if (mStatus == kTMS_Initialized)
        {
            // call update on each task owned by this TaskMaster
            for (Task & task : mOwnedTasks)
            {
                // LORRTODO: remove dead tasks from the list

                task.update(delta);
            }

            // Give AssetMgr an opportunity to process messages
            mpAssetMgr->process();
        }

        // process messages accumulated since last frame
        processMessages(*mpMainMessageQueue); // messages from main thread
        // messages from other TaskMasters or ourself
        for (MessageQueue * pMessageQueue : mTaskMasterMessageQueues)
        {
            processMessages(*pMessageQueue);
        }

        // Notify other task masters, they will wake up and process
        // messages and update tasks while we render.
        notify_next_frame();

        if (didRender)
        {
            renderer_end_frame(mRendererTask);
            didRender = false;
        }
    };
}

void TaskMaster::runAuxiliaryGameLoop()
{
    ASSERT(mStatus == kTMS_Initialized);
    ASSERT(!mIsPrimary && mRendererTask.id() == 0);
    ASSERT(!mIsRunning);

    mpInputMgr.reset(GNEW(kMEM_Engine, InputMgr, isPrimary()));

    mIsRunning = true;
    mFrameTime.init();

    while(mIsRunning)
    {
#if HAS(LOG_FPS)
        if (mFrameTime.frameCount() % 100 == 0)
        {
            FrameInfo fi;
            mFrameTime.fps(&fi);
            LOG_INFO("TaskMasterFPS %d: %d tasks, %f, %f, %f, %f",
                mThreadId,
                mOwnedTasks.size(),
                fi.last10,
                fi.last100,
                fi.last1000,
                fi.last10000);
        }
#endif

        // process messages accumulated since last frame
        processMessages(*mpMainMessageQueue); // messages from main thread
        // messages from other TaskMasters or ourself
        for (MessageQueue * pMessageQueue : mTaskMasterMessageQueues)
        {
            processMessages(*pMessageQueue);
        }

        // Get delta since the last time we ran
        f32 delta = mFrameTime.calcDelta();

        // call update on each task owned by this TaskMaster
        for(Task & task : mOwnedTasks)
        {
            task.update(delta);
        }

        // process messages accumulated since last frame
        processMessages(*mpMainMessageQueue); // messages from main thread
        // messages from other TaskMasters or ourself
        for (MessageQueue * pMessageQueue : mTaskMasterMessageQueues)
        {
            processMessages(*pMessageQueue);
        }

        // Wait until primary game loop completes next frame
        if (mStatus == kTMS_Initialized)
            waitForNextFrame();

    };
}


void TaskMaster::registerMutableDependency(task_id taskId, u32 path)
{
    ASSERT(mStatus == kTMS_Initialized);
/*
    // Get the task, and make sure it exists since it should at this point
    auto taskIt = mTasks.find(taskId);
    ASSERT(taskIt != mTasks.end());
    Task & task = taskIt->second;

    task_id rootTaskId = task.rootTaskId();

    // Insert into mMutableDataUsers if necessary
    auto pDataUsers = mMutableDataUsers.find(path);
    if (pDataUsers != mMutableDataUsers.end())
    {
        pDataUsers->second->operator[](rootTaskId)++;
    }
    else
    {
        // All this mapped_type::element_type stuff is just to avoid typing the largish template decls
        void * memSpace = ALLOC(sizeof(DataToTaskRefs), kMT_Engine);
        DataToTaskRefsUP pNewMap(new (memSpace) DataToTaskRefs);
        pNewMap->emplace(path, 1); // refcount of 1 to start
        mMutableDataUsers.emplace(path, std::move(pNewMap));
    }

    // Insert into mMutableData if necessary
    auto pData = mMutableData.find(rootTaskId);
    if (pData != mMutableData.end())
    {
        pData->second->operator[](path)++; // inc refcount
    }
    else
    {
        // All this value_type::element_type stuff is just to avoid typing the largish template decls
        void * memSpace = ALLOC(sizeof(TaskToDataRefs), kMT_Engine);
        TaskToDataRefsUP pNewMap(new (memSpace) TaskToDataRefs);
        pNewMap->emplace(path, 1); // refcount of 1 to start
        mMutableData.emplace(rootTaskId, std::move(pNewMap));
    }
*/
}

void TaskMaster::deregisterMutableDependency(task_id taskId, u32 path)
{
    ASSERT(mStatus == kTMS_Initialized);
}

bool TaskMaster::isOwnedTask(task_id taskId)
{
    return mOwnedTaskMap.find(taskId) != mOwnedTaskMap.end();
}

void TaskMaster::processMessages(MessageQueue & msgQueue)
{
    MessageQueueAccessor msgAcc;

    while (msgQueue.popBegin(&msgAcc))
    {
        message(msgAcc);
        msgQueue.popCommit(msgAcc);
    }
}


MessageResult TaskMaster::message(const MessageQueueAccessor& msgAcc)
{
    const Message & msg = msgAcc.message();

    // Check for editor messages
    if (mIsEditorEnabled)
    {
        switch (msg.msgId)
        {
        case HASH::editor_activate__:
            mIsEditorActive = msg.payload.b;
            LOG_INFO("Editor activated: %d", mIsEditorActive);
            return MessageResult::Consumed;
        }
    }

    if (mStatus == kTMS_Initialized)
    {
        // Handle messages sent to us, the TaskMaster
        if (msg.target < num_threads())
        {
            ASSERT_MSG(msg.target == active_thread_id(), "TaskMaster message sent to wrong TaskMaster");
            switch(msg.msgId)
            {
            case HASH::fin:
            {
                ASSERT(mIsRunning);
                fin(msgAcc);
                return MessageResult::Consumed;
            }
            case HASH::insert_task__:
            {
                messages::OwnerTaskR<MessageQueueAccessor> msgr(msgAcc);
                insertTask(msgr.owner(), msgr.task());
                return MessageResult::Consumed;
            }
            case HASH::request_set_task_owner__:
            {
                messages::OwnerTaskR<MessageQueueAccessor> msgr(msgAcc);

                // If we own it, remove it and send the confirm_set_task_owner
                auto ownedIt = mOwnedTaskMap.find(msgr.task().id());
                if (ownedIt != mOwnedTaskMap.end())
                {
                    removeTask(msgr.task().id());
                    messages::OwnerTaskBW msgw(HASH::confirm_set_task_owner__,
                                               kMessageFlag_None,
                                               threadId(),
                                               threadId(),
                                               msgr.owner());
                    msgw.setTask(mOwnedTasks[ownedIt->second]);
                    broadcast_message(msgw.accessor());
                }
                return MessageResult::Consumed;
            }
            case HASH::confirm_set_task_owner__:
            {
                messages::OwnerTaskR<MessageQueueAccessor> msgr(msgAcc);
                setTaskOwner(msgr.owner(), msgr.task());
                return MessageResult::Consumed;
            }
            case HASH::remove_task__:
            {
                task_id taskIdToRemove = msg.payload.u;

                auto ownedIt = mOwnedTaskMap.find(taskIdToRemove);
                if (ownedIt != mOwnedTaskMap.end())
                {
                    // Send an immediate fin__ to the task so it can delete itself
                    StackMessageBlockWriter<0> finw(HASH::fin__, kMessageFlag_Editor, threadId(), taskIdToRemove, to_cell(0));
                    mOwnedTasks[ownedIt->second].message(finw.accessor());
                }

                if (mpModelMgr)
                    mpModelMgr->message(msgAcc);

                if (mpSpriteMgr)
                    mpSpriteMgr->message(msgAcc);

                removeTask(taskIdToRemove);

                return MessageResult::Consumed;
            }
            case HASH::request_set_parent__:
            {
                messages::TaskEntityR<MessageQueueAccessor> msgr(msgAcc);

                task_id parentTaskId = msgr.taskId();
                Entity * pChild = msgr.entity();
                task_id childTaskId = pChild->task().id();

                thread_id parentOwner = mTaskOwnerMap[parentTaskId];
                thread_id childOwner = mTaskOwnerMap[childTaskId];

                // In all cases, if child and parent don't share an
                // owner, we need to get them out of our tracking
                // structures.
                if (parentOwner != childOwner)
                {
                    removeTask(childTaskId);
                }

                // This message is only relevant if we own the child
                if (childOwner == threadId())
                {
                    // If we also own the parent, things are simple
                    if (parentOwner == threadId())
                    {
                        messages::TaskEntityBW msgw(HASH::insert_child,
                                                    kMessageFlag_None,
                                                    msg.source,
                                                    parentTaskId,
                                                    parentTaskId);
                        msgw.setEntity(pChild);

                        auto ownedParentIt = mOwnedTaskMap.find(parentTaskId);
                        mOwnedTasks[ownedParentIt->second].message(msgw.accessor());
                    }

                    // We don't own the parent, so we must send the
                    // child to the parent's owning taskmaster.
                    else
                    {
                        broadcast_confirm_set_parent(msg.source,
                                                     parentOwner,
                                                     parentTaskId,
                                                     pChild);
                    }
                }
                return MessageResult::Consumed;
            }
            case HASH::confirm_set_parent__:
            {
                messages::TaskEntityR<MessageQueueAccessor> msgr(msgAcc);

                task_id parentTaskId = msgr.taskId();
                Entity * pChild = msgr.entity();
                thread_id parentOwner = msg.target;

                insertTask(parentOwner, pChild->task());

                // If we own the parent, conduct parenting
                if (parentOwner == threadId())
                {
                    messages::TaskEntityBW msgw(HASH::insert_child,
                                                kMessageFlag_None,
                                                msg.source,
                                                parentTaskId,
                                                parentTaskId);
                    msgw.setEntity(pChild);

                    auto ownedParentIt = mOwnedTaskMap.find(parentTaskId);
                    ASSERT(ownedParentIt != mOwnedTaskMap.end());
                    mOwnedTasks[ownedParentIt->second].message(msgw.accessor());
                }
                return MessageResult::Consumed;
            }
            default:
            {
                ERR("Unhandled message type, msgId: %d", msg.msgId);
                return MessageResult::Propagate;
            }
            }
        }
        else if (msg.target == kRendererTaskId)
        {
            ASSERT(mRendererTask.id() != 0);
            return mRendererTask.message(msgAcc);
        }
        else if (msg.target == kInputMgrTaskId)
        {
            ASSERT(mpInputMgr.get() != nullptr);
            mpInputMgr->message(msgAcc);
        }
        else if (msg.target == kAssetMgrTaskId)
        {
            ASSERT(mpAssetMgr.get() != nullptr);
            mpAssetMgr->message(msgAcc);
        }
        else if (msg.target == kModelMgrTaskId)
        {
            ASSERT(mpModelMgr.get() != nullptr);
            mpModelMgr->message(msgAcc);
        }
        else if (msg.target == kSpriteMgrTaskId)
        {
            ASSERT(mpSpriteMgr.get() != nullptr);
            mpSpriteMgr->message(msgAcc);
        }
        else if (msg.target == kMainThreadTaskId)
        {
            PANIC("Not Implemented");
        }
        else
        {
            // Message is for a specific task
            // Verify we own this task
            auto it = mOwnedTaskMap.find(msg.target);

            if (it != mOwnedTaskMap.end())
            {
                size_t taskIdx = it->second;
                ASSERT(taskIdx < mOwnedTasks.size());

                // If it is an activate_task message, hand it specially here
                if (msg.msgId == HASH::set_task_status)
                {
                    messages::TaskStatusQR msgRdr(msgAcc);

                    // Make sure task status is a valid 2 bit value (only has 2 bits in definition of Task struct)
                    PANIC_IF((u32)msgRdr.status() >= 4,"Invalid task status %u", msg.payload.u);
                    TaskStatus stat = msgRdr.status();
                    mOwnedTasks[taskIdx].setStatus(stat);
                    return MessageResult::Consumed;
                }

                // send message to task
                MessageResult mr = mOwnedTasks[taskIdx].message(msgAcc);

                if (mr != MessageResult::Consumed)
                {
                    int i = 0;
                }
#if HAS(TRACK_HASHES)
                EXPECT_MSG(mr == MessageResult::Consumed,
                           "Task did not consume a message intended for it, task name: %s, message: %s",
                           HASH::reverse_hash(mOwnedTasks[taskIdx].nameHash()),
                           HASH::reverse_hash(msg.msgId));
#else
                EXPECT_MSG(mr == MessageResult::Consumed,
                           "Task did not consume a message intended for it, task nameHash: 0x%08x, message: 0x%08x",
                           mOwnedTasks[taskIdx].nameHash(),
                           msg.msgId);
#endif
                return MessageResult::Consumed;
            }
            else
            {
                // We don't own this task, attempt to forward the message
                MessageQueue * pMsgQ = messageQueueForTarget(msg.target);

                if (!pMsgQ)
                {
                    // Message directed to task we're not tracking, throw it away
#if HAS(TRACK_HASHES)
                    LOG_INFO("Message to non-existent target, source: %u, target: %u, message: %s",
                             msg.source,
                             msg.target,
                             HASH::reverse_hash(msg.msgId));
#else
                    LOG_INFO("Message to non-existent target, source: %u, target: %u, message: %u",
                             msg.source,
                             msg.target,
                             msg.msgId);
#endif

                    return MessageResult::Consumed;
                }

                MessageQueueAccessor msgAccNew;
                pMsgQ->pushBegin(&msgAccNew,
                                 msg.msgId,
                                 msg.flags,
                                 msg.source,
                                 msg.target,
                                 msg.payload,
                                 msg.blockCount);

                for (u32 i = 0; i < msg.blockCount; ++i)
                {
                    msgAccNew[i] = msgAcc[i];
                }

                pMsgQ->pushCommit(msgAcc);
            }
        }
    }
    else if (mStatus == kTMS_Finalizing)
    {
        if (msg.msgId == HASH::shutdown__)
        {
            u32 numThreads = num_threads();
            ASSERT(mShutdownCount < numThreads);
            mShutdownCount++;
            if (mShutdownCount == numThreads)
            {
                mIsRunning = false;
            }
        }

        // We may get some other messages here, but we ignore them.
        // Examples include cleanup messages related to shutting down, like:
        // HASH::remove_task__
        // HASH::release_asset__
    }
    else
    {
        PANIC("Message received in invalid state, msgid: %u, status: %u", msg.msgId, mStatus);
    }

    return MessageResult::Consumed;
}


void TaskMaster::insertTask(thread_id threadOwner, const Task & task)
{
    ASSERT(mTaskOwnerMap.find(task.id()) == mTaskOwnerMap.end());

    mTaskOwnerMap[task.id()] = threadOwner;

    if (threadOwner == threadId())
    {
        ASSERT(mOwnedTaskMap.find(task.id()) == mOwnedTaskMap.end());
        mOwnedTasks.push_back(task);
        mOwnedTaskMap[task.id()] = mOwnedTasks.size() - 1;
    }

    //LOG_INFO("Task Count(%u): %u", threadId(), (u32)mOwnedTaskMap.size());
}

void TaskMaster::setTaskOwner(thread_id newOwner, const Task & task)
{
    auto itTOM = mTaskOwnerMap.find(task.id());
    PANIC_IF(itTOM == mTaskOwnerMap.end(), "setTaskOwner: task not found: %u", task.id());

    if (itTOM != mTaskOwnerMap.end() &&
        itTOM->second != threadId()) // only need to do something if we don't already own task
    {
        // Send remove message
        broadcast_remove_task(task.id(), task.id());

        // Send insert message
        broadcast_insert_task(threadId(), newOwner, task);
    }
}

void TaskMaster::removeTask(task_id taskId)
{
    // Are we the owner?
    auto itOTM = mOwnedTaskMap.find(taskId);
    if (itOTM != mOwnedTaskMap.end())
    {
        size_t idx = itOTM->second;
        // If there is more than one item in mOwnedTaskMap,
        // we swap with the last item, then pop off the last item.
        if (mOwnedTaskMap.size() > 1)
        {
            // replace item with the last one in vector
            task_id backTaskId = mOwnedTasks.back().id();
            mOwnedTasks[idx] = mOwnedTasks.back();

            // remove last task in vector (now it is in position of the task we are removing)
            mOwnedTasks.pop_back();

            // remove from mOwnedTaskMap
            mOwnedTaskMap.erase(itOTM);

            // adjust mOwnedTaskMap so it can find what was the last task that has moved indexes
            if (idx < mOwnedTasks.size()) // if idx == mOwnedTasks.size(), it was the last task so no need to reorder
            {
                mOwnedTaskMap[backTaskId] = idx;
            }
        }
        else
        {
            ASSERT(mOwnedTasks.size() == 1); // verify our two containers have the same size
            // just remove it
            mOwnedTasks.pop_back();
            mOwnedTaskMap.erase(itOTM);
        }
    }

    // In either case (we own or don't own), we remove from mTaskOwnerMap
    auto itTOM = mTaskOwnerMap.find(taskId);
    if (itTOM != mTaskOwnerMap.end())
        mTaskOwnerMap.erase(itTOM);
}

} // namespace gaen



