//------------------------------------------------------------------------------
// TaskMaster.cpp - Manages tasks, one of these runs per core
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#include "engine/hashes.h"
#include "engine/MessageQueue.h"
#include "engine/Entity.h"
#include "engine/messages/InsertTask.h"
#include "engine/InputMgr.h"
#include "engine/renderer_api.h"

#include "engine/TaskMaster.h"

#define LOG_FPS HAS__

namespace gaen
{
extern void register_all_entities_and_components(Registry & registry);

// LORRTODO - Choose good message queue sizes here
static const u32 kMaxMainMessages = 4096;
static const u32 kMaxTaskMasterMessages = 4096;
static u32 sStartEntityHash = HASH::init__Start;

static bool sIsInit = false;

void init_task_masters()
{
    ASSERT(!sIsInit);

    for (thread_id tid = 0; tid < num_threads(); ++tid)
    {
        TaskMaster & tm = TaskMaster::task_master_for_thread(tid);
        tm.init(tid);
    }

    sIsInit = true;
}

void fin_task_masters()
{
    ASSERT(sIsInit);
    broadcast_message(HASH::fin,
                      kMessageFlag_ForcePropogate,
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

MessageQueue * get_message_queue(u32 msgId,
                                 u32 flags,
                                 task_id source,
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
        if (target == kRendererTaskId ||
            target == kInputMgrTaskId)
        {
            // send standard target messages to primary task master for now
            // LORRTODO - InputMgr may not be located on primary task master, support that possibility
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

void broadcast_message(const MessageBlockAccessor & msgAcc)
{
    broadcast_message(msgAcc.message().msgId,
                      msgAcc.message().flags,
                      msgAcc.message().source,
                      msgAcc.message().payload,
                      msgAcc.message().blockCount,
                      &msgAcc[0]);
}

void TaskMaster::init(thread_id tid)
{
    ASSERT(!mIsInit);
    mThreadId = tid;
    mIsPrimary = tid == kPrimaryThreadId;

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

    mIsInit = true;
}

void TaskMaster::fin(const MessageQueueAccessor& msgAcc)
{
    ASSERT(mIsInit);
    ASSERT(msgAcc.message().msgId == HASH::fin);

    for (Task & task : mOwnedTasks)
    {
        task.message(msgAcc);
    }

    // Entities should have deleted themselves and all dependent memory
    mOwnedTaskMap.clear();
    mOwnedTasks.clear();

    mIsRunning = false;
}


void TaskMaster::cleanup()
{
    // We can't do this cleanup in the fin() method, since message queues
    // are still active.
    // This clenaup will get called when loop actually exits and we can
    // really clean up everything.

    if (mRendererTask.id() != 0)
        renderer_fin(mRendererTask);

    GDELETE(mpMainMessageQueue);
    for (MessageQueue * pMessageQueue : mTaskMasterMessageQueues)
    {
        GDELETE(pMessageQueue);
    }

    mIsInit = false;
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
    if (target == kRendererTaskId ||
        target == kInputMgrTaskId)
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
            // and the insert_task message hasn't propogated through
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

void TaskMaster::runPrimaryGameLoop()
{
    ASSERT(mIsInit);
    ASSERT(mIsPrimary && mRendererTask.id() != 0);
    ASSERT(!mIsRunning);

    mpInputMgr.reset(GNEW(kMEM_Engine, InputMgr));

    renderer_init_device(mRendererTask);
    renderer_init_viewport(mRendererTask);

    // LORRTODO - make start entity name dynamic based on command line args
    // Init the start entity now that we have a TaskMaster running.
    Entity * pStartEntity = mRegistry.constructEntity(sStartEntityHash, 32);

    if (pStartEntity)
        LOG_INFO("Start entity: %s", HASH::reverse_hash(sStartEntityHash));
    else
        LOG_ERROR("Unable to start entity: %s", HASH::reverse_hash(sStartEntityHash));

    if (pStartEntity)
    {
        // Send init message
        StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, pStartEntity->task().id(), pStartEntity->task().id(), to_cell(0));
        pStartEntity->message(msgBW.accessor());
    
        insertTask(threadId(), pStartEntity->task());
    }

    mIsRunning = true;
    mFrameTime.init();
    
    while(mIsRunning)
    {
        // Render through the render adapter
        renderer_render(mRendererTask);

        // Get delta since the last time we ran
        f32 deltaSecs = mFrameTime.calcDelta();

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

        
        // LORRTODO - Do physics


        // call update on each task owned by this TaskMaster
        for (Task & task : mOwnedTasks)
        {
            task.update(deltaSecs);
        }

        renderer_end_frame(mRendererTask);

        // If we're not full of work, sleep a bit.
        // Don't sleep too aggressively since Windows
        // time slice is 10ms.
        if (mFrameTime.deltaMeanLast10() < 0.06)
            sleep(10);
    };
}

void TaskMaster::runAuxiliaryGameLoop()
{
    ASSERT(mIsInit);
    ASSERT(!mIsPrimary && mRendererTask.id() == 0);
    ASSERT(!mIsRunning);

    mIsRunning = true;
    mFrameTime.init();

    while(mIsRunning)
    {
        // Get delta since the last time we ran
        f32 deltaSecs = mFrameTime.calcDelta();

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

        // call update on each task owned by this TaskMaster
        for(Task & task : mOwnedTasks)
        {
            task.update(deltaSecs);
        }

        // If we're not full of work, sleep a bit
        // Don't sleep too aggressively since Windows
        // time slice is 10ms.
        if (mFrameTime.deltaMeanLast10() < 0.06)
            sleep(10);
    };
}


void TaskMaster::registerMutableDependency(task_id taskId, u32 path)
{
    ASSERT(mIsInit);
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
    ASSERT(mIsInit);
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
        case HASH::insert_task:
        {
            messages::InsertTaskR<MessageQueueAccessor> msgr(msgAcc);
            insertTask(msgr.owner(), msgr.task());
            return MessageResult::Consumed;
        }
        default:
        {
            ERR("Unhandled message type, msgId: %d", msg.msgId);
            return MessageResult::Propogate;
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
            MessageResult mr = mOwnedTasks[taskIdx].message(msgAcc);
            EXPECT_MSG(mr == MessageResult::Consumed,
                       "Task didn't consume a message intended for it, task nameHash: 0x%08x, messaeg: 0x%08x",
                       mOwnedTasks[taskIdx].nameHash(),
                       msg.msgId);
            return MessageResult::Consumed;
        }
        else
        {
            // We don't own this task, attempt to forward the message
            MessageQueue * pMsgQ = messageQueueForTarget(msg.target);

            if (!pMsgQ)
            {
                // Message directed to task we're not tracking, throw it away
                LOG_INFO("Message to non-existent target; source: %u, target: %u, msgId: %u", msg.source, msg.target, msg.msgId);
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
}


} // namespace gaen



