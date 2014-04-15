//------------------------------------------------------------------------------
// TaskMaster.cpp - Manages tasks, one of these runs per core
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

#include "engine/stdafx.h"

#include "core/platutils.h"
#include "core/logging.h"

#include "engine/FNV.h"
#include "engine/MessageQueue.h"
#include "engine/Entity.h"
#include "engine/renderer_type.h"
#include "engine/message_helpers/TaskMsg.h"

#include "engine/TaskMaster.h"


namespace gaen
{

static const u32 kMaxMainMessages = 4096;
static const u32 kMaxTaskMasterMessages = 4096;

static const u64 kFPS = 60;
static const u64 kNanosPerFrame = 1000000000 / kFPS;

static bool sIsInit = false;

template <class RendererT>
void init_task_masters()
{
    ASSERT(!sIsInit);

    for (thread_id tid = 0; tid < num_threads(); ++tid)
    {
        TaskMaster<RendererT> & tm = TaskMaster<RendererT>::task_master_for_thread(tid);
        tm.init(tid);
    }

    sIsInit = true;
}

template <class RendererT>
void fin_task_masters()
{
    ASSERT(sIsInit);
    queue_message_from_main<RendererT>(kBroadcastThreadId, FNV::fin);
    join_all_threads();
}

// Entry point of our thread
template <class RendererT>
static void start_game_loop()
{
    ASSERT(sIsInit);
    init_time();

    thread_id tid = active_thread_id();
    ASSERT(tid >= 0 && tid < num_threads());

    set_thread_affinity(tid);

    LOG_INFO("Starting thread: %d", tid);

    TaskMaster<RendererT> & tm = TaskMaster<RendererT>::task_master_for_thread(active_thread_id());

    if (tm.isPrimary())
        tm.runPrimaryGameLoop();
    else
        tm.runAuxiliaryGameLoop();
}

template <class RendererT>
void start_game_loops(Entity * pInitEntity)
{
    ASSERT(sIsInit);
    thread_id numThreads = num_threads();

    // Start a TaskMaster for every thread
    for (thread_id i = 0; i < numThreads; ++i)
    {
        start_thread(start_game_loop<RendererT>);
    }

    // Create a task out of the entity and start it up
    Task t = Task::create(pInitEntity);
    MessageBlock * pMb = reinterpret_cast<MessageBlock*>(&t);
    queue_message_from_main<RendererT>(kBroadcastThreadId, FNV::add_task, to_cell(kPrimaryThreadId), pMb, sizeof(Task)/sizeof(MessageBlock));
}

template <class RendererT>
inline void task_master_queue_message_from_main(thread_id threadId,
                                          fnv msgId,
                                          cell payload,
                                          const MessageBlock * pMsgBlock,
                                          size_t msgBlockCount)
{
    TaskMaster<RendererT> & tm = TaskMaster<RendererT>::task_master_for_thread(threadId);
    MessageQueue & mq = tm.mainMessageQueue();
    MessageQueue::MessageAccessor msgAcc;
    mq.pushBegin(&msgAcc,
                 msgId,
                 kMessageFlag_TaskMaster,
                 0,
                 0,
                 payload,
                 msgBlockCount);
    for (size_t i = 0; i < msgBlockCount; ++i)
    {
        msgAcc[i] = pMsgBlock[i];
    }
    mq.pushCommit(msgAcc);
}

template <class RendererT>
void queue_message_from_main(thread_id threadId,
                       fnv msgId)
{
    queue_message_from_main<RendererT>(threadId, msgId, to_cell(0), nullptr, 0);
}

template <class RendererT>
void queue_message_from_main(thread_id threadId,
                       fnv msgId,
                       cell payload,
                       const MessageBlock * pMsgBlock,
                       size_t msgBlockCount)
{
    ASSERT(sIsInit);
    ASSERT(active_thread_id() == kMainThreadId);
    thread_id numThreads = num_threads();
    ASSERT((threadId >= 0 && threadId < numThreads) ||
           threadId == kBroadcastThreadId);
    if (threadId != kBroadcastThreadId)
    {
        task_master_queue_message_from_main<RendererT>(threadId,
                                                 msgId,
                                                 payload,
                                                 pMsgBlock,
                                                 msgBlockCount);
    }
    else
    {
        for (thread_id tid = 0; tid < numThreads; ++tid)
        {
            task_master_queue_message_from_main<RendererT>(tid,
                                                     msgId,
                                                     payload,
                                                     pMsgBlock,
                                                     msgBlockCount);
        }
    }
}

template <class RendererT>
void queue_message(fnv msgId,
                  task_id source,
                  task_id target,
                  cell payload,
                  const MessageBlock * pMsgBlock,
                  size_t msgBlockCount)
{
    ASSERT(sIsInit);

    thread_id activeTid = active_thread_id();
    ASSERT(activeTid < num_threads());

    TaskMaster<RendererT> & tm = TaskMaster<RendererT>::task_master_for_thread(activeTid);
    tm.queueMessage(msgId, target, source, payload, pMsgBlock, msgBlockCount);
}


template <class RendererT>
MessageQueue & message_queue_for_target(task_id target)
{
    ASSERT(sIsInit);

    thread_id activeTid = active_thread_id();
    ASSERT(activeTid < num_threads());

    TaskMaster<RendererT> & tm = TaskMaster<RendererT>::task_master_for_thread(activeTid);
    return tm.messageQueueForTarget(target);
}


template <class RendererT>
void TaskMaster<RendererT>::init(thread_id tid)
{
    ASSERT(!mIsInit);
    mThreadId = tid;
    mIsPrimary = tid == kPrimaryThreadId;

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

    mIsInit = true;
}

template <class RendererT>
void TaskMaster<RendererT>::fin(const Message & msg, const MessageQueue::MessageAccessor& msgAcc)
{
    ASSERT(mIsInit);
    ASSERT(msg.msgId == FNV::fin);

    for (Task & task : mOwnedTasks)
    {
        task.message(msg, msgAcc);
    }

    // Fin our renderer
    ASSERT(mpRenderer);
    mpRenderer->fin();
        
    GDELETE(mpMainMessageQueue);
    for (MessageQueue * pMessageQueue : mTaskMasterMessageQueues)
    {
        GDELETE(pMessageQueue);
    }

    mIsRunning = false;
    mIsInit = false;
}

template <class RendererT>
TaskMaster<RendererT> & TaskMaster<RendererT>::task_master_for_thread(thread_id tid)
{
    ASSERT(is_threading_init());
    ASSERT(tid >= 0 && tid < num_threads());
    static Vector<TaskMaster<RendererT>,kMEM_Engine> sTaskMasters(num_threads());
    ASSERT(tid < sTaskMasters.size());
    return sTaskMasters[tid];
}

template <class RendererT>
MessageQueue & TaskMaster<RendererT>::messageQueueForTarget(task_id target)
{
    thread_id targetThreadId;

    // Special case the Renderer and InputManager since they
    // are so common.
    if (target == kRendererTaskId ||
        target == kInputManagerTaskId)
    {
        targetThreadId = kPrimaryThreadId;
    }
    else
    {
        // Lookup the task in our map to see which task masger it
        // belogns to.
        TaskOwnerMap::iterator it = mTaskOwnerMap.find(target);
        if (it == mTaskOwnerMap.end())
            PANIC("queueMessage to unknown target task_id %d", target);

        targetThreadId = it->second;
    }

    ASSERT(targetThreadId < num_threads());
    TaskMaster<RendererT> & targetTaskMaster = TaskMaster<RendererT>::task_master_for_thread(targetThreadId);
    return targetTaskMaster.taskMasterMessageQueue();
}

template <class RendererT>
void TaskMaster<RendererT>::runPrimaryGameLoop()
{
    ASSERT(mIsInit);
    ASSERT(mIsPrimary && mpRenderer);
    ASSERT(!mIsRunning);
    
    mIsRunning = true;

    mpRenderer->initRenderDevice();
    mpRenderer->initViewport();

    f32 lastFrameTime = 0.0f;
    thread_id numThreads = num_threads();
    
    f32 deltaSecs = 0.0f;

    while(mIsRunning)
    {
        // Render through the render adapter
        mpRenderer->render();

        // Get delta since the last time we ran
        f32 startFrameTime = now();
        f32 deltaSecs = startFrameTime - lastFrameTime;
        lastFrameTime = startFrameTime;


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

        mpRenderer->endFrame();
    };
}

template <class RendererT>
void TaskMaster<RendererT>::runAuxiliaryGameLoop()
{
    ASSERT(mIsInit);
    ASSERT(!mIsPrimary && !mpRenderer);
    ASSERT(!mIsRunning);

    mIsRunning = true;

    while(mIsRunning)
    {
        f32 deltaSecs = 0.0f;

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
    };
}

template <class RendererT>
void TaskMaster<RendererT>::queueMessage(fnv msgId,
                                         task_id source,
                                         task_id target,
                                         cell payload,
                                         const MessageBlock * pMsgBlock,
                                         size_t msgBlockCount)
{
    MessageQueue & mq = TaskMaster<RendererT>::messageQueueForTarget(target);
        
    MessageQueue::MessageAccessor msgAcc;
    mq.pushBegin(&msgAcc,
                 msgId,
                 kMessageFlag_None,
                 source,
                 target,
                 payload,
                 msgBlockCount);
    for (size_t i = 0; i < msgBlockCount; ++i)
    {
        msgAcc[i] = pMsgBlock[i];
    }
    mq.pushCommit(msgAcc);
}



template <class RendererT>
void TaskMaster<RendererT>::registerMutableDependency(task_id taskId, fnv path)
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

template <class RendererT>
void TaskMaster<RendererT>::deregisterMutableDependency(task_id taskId, fnv path)
{
    ASSERT(mIsInit);
}

template <class RendererT>
void TaskMaster<RendererT>::processMessages(MessageQueue & msgQueue)
{
    MessageQueue::MessageAccessor msgAcc;

    while (msgQueue.popBegin(&msgAcc))
    {
        message(msgAcc.message(), msgAcc);
        msgQueue.popCommit(msgAcc);
    }
}


template <class RendererT>
MessageResult TaskMaster<RendererT>::message(const Message & msg, const MessageQueue::MessageAccessor& msgAcc)
{
    // Handle messages sent to us, the TaskMaster
    if (msg.flags & kMessageFlag_TaskMaster)
    {
        switch(msg.msgId)
        {
        case FNV::fin:
            ASSERT(mIsRunning);
            fin(msg, msgAcc);
            return MessageResult::Consumed;
        case FNV::insert_task:
        {
            TaskMsgReader tmr(msgAcc);
            insertTask(tmr.owner(), tmr.task());
            break;
        }
        default:
            ERR("Unhandled message type, fnv: %d", msg.msgId);
            return MessageResult::Propogate;
        }
    }
    else if (msg.target == kRendererTaskId)
    {
        ASSERT(mpRenderer);
        return mpRenderer->message(msgAcc);
    }
    else if (msg.target == kInputManagerTaskId)
    {

    }
    else
    {
        // Message was meant for a task, send it appropriately
        
    }

    return MessageResult::Consumed;
}


template <class RendererT>
void TaskMaster<RendererT>::insertTask(thread_id threadOwner, const Task & task)
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


// Instantiate TaskMaster and helper funcs with our renderer class.
template class TaskMaster<renderer_type>;
template void init_task_masters<renderer_type>();
template void fin_task_masters<renderer_type>();
template void start_game_loops<renderer_type>(Entity * pInitEntity);
template void queue_message_from_main<renderer_type>(thread_id threadId,
                                              fnv msgId);
template void queue_message_from_main<renderer_type>(thread_id threadId,
                                              fnv msgId,
                                              cell payload,
                                              const MessageBlock * pMsgBlock,
                                              size_t msgBlockCount);
template void queue_message<renderer_type>(fnv msgId,
                                          task_id target,
                                          task_id source,
                                          cell payload,
                                          const MessageBlock * pMsgBlock,
                                          size_t msgBlockCount);

template MessageQueue & message_queue_for_target<renderer_type>(task_id target);

} // namespace gaen



