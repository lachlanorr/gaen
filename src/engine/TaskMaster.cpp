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
#include "engine/TaskMaster.h"

namespace gaen
{

static const u32 kMaxMainThreadMessages = 4096;

static const u64 kFPS = 60;
static const u64 kNanosPerFrame = 1000000000 / kFPS;

template <class RendererT>
void init_task_masters()
{
    static bool sIsInit = false;
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
    message_from_main<RendererT>(kBroadcastThreadId, FNV::fin);
    join_all_threads();
}

// Entry point of our thread
template <class RendererT>
static void start_game_loop()
{
    init_time();
    thread_id tid = active_thread_id();
    ASSERT(tid >= 0 && tid < num_threads());

    LOG_INFO("Starting thread: %d", tid);

    TaskMaster<RendererT> & tm = TaskMaster<RendererT>::task_master_for_thread(active_thread_id());

    if (tm.isPrimary())
        tm.runPrimaryGameLoop();
    else
        tm.runAuxiliaryGameLoop();
}

template <class RendererT>
void start_game_loops()
{
    thread_id numThreads = num_threads();

    // Start a TaskMaster for every thread
    for (thread_id i = 0; i < numThreads; ++i)
    {
        start_thread(start_game_loop<RendererT>);
    }
}

template <class RendererT>
inline void task_master_message_from_main(thread_id threadId,
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
                 msgBlockCount);
    for (size_t i = 0; i < msgBlockCount; ++i)
    {
        msgAcc.qcellAt(i) = pMsgBlock[i].qcells[0];
    }
    mq.pushCommit(msgAcc);
}

template <class RendererT>
void message_from_main(thread_id threadId,
                       fnv msgId)
{
    message_from_main<RendererT>(threadId, msgId, to_cell(0), nullptr, 0);
}

template <class RendererT>
void message_from_main(thread_id threadId,
                       fnv msgId,
                       cell payload,
                       const MessageBlock * pMsgBlock,
                       size_t msgBlockCount)
{
    ASSERT(active_thread_id() == kMainThreadId);
    thread_id numThreads = num_threads();
    ASSERT((threadId >= 0 && threadId < numThreads) ||
           threadId == kBroadcastThreadId);
    if (threadId != kBroadcastThreadId)
    {
        task_master_message_from_main<RendererT>(threadId,
                                                 msgId,
                                                 payload,
                                                 pMsgBlock,
                                                 msgBlockCount);
    }
    else
    {
        for (thread_id tid = 0; tid < numThreads; ++tid)
        {
            task_master_message_from_main<RendererT>(tid,
                                                     msgId,
                                                     payload,
                                                     pMsgBlock,
                                                     msgBlockCount);
        }
    }
}

template <class RendererT>
void TaskMaster<RendererT>::init(thread_id tid)
{
    ASSERT(!mIsInit);
    mThreadId = tid;
    mIsPrimary = tid == 0;

    // Allocate a message queue that the main thread can use to communicate with us
    void * tmp = ALLOC(sizeof(MessageQueue), kMT_Engine);
    mpMainThreadMessageQueue.reset(new (tmp) MessageQueue(kMaxMainThreadMessages));

    // Pre-allocate reasonable sizes for hash tables
    // LORRTODO - these should be command line options
    const u32 kEstimatedTaskCount = 65536;
    const u32 kEstimatedMutableDataCount = 128;
    mOwnedTasks.reserve(kEstimatedTaskCount);
    mTasks.reserve(kEstimatedTaskCount);
    mTaskOwners.reserve(kEstimatedTaskCount);
    mMutableDataUsers.reserve(kEstimatedMutableDataCount);
    mMutableData.reserve(kEstimatedTaskCount);

    mIsInit = true;
}

template <class RendererT>
void TaskMaster<RendererT>::fin()
{
    ASSERT(mIsInit);

    

    mIsInit = false;
}

template <class RendererT>
TaskMaster<RendererT> & TaskMaster<RendererT>::task_master_for_thread(thread_id tid)
{
    ASSERT(is_threading_init());
    ASSERT(tid >= 0 && tid < num_threads());
    static Vector<TaskMaster<RendererT>,kMT_Engine> sTaskMasters(num_threads());
    ASSERT(tid < sTaskMasters.size());
    return sTaskMasters[tid];
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
        processMessages(*mpMainThreadMessageQueue); // messages from main thread
        //processMessages(); // messages from tasks, directed to our tasks

        
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
        processMessages(*mpMainThreadMessageQueue); // messages from main thread
        //processMessages(); // messages from tasks, directed to our tasks

        // call update on each task owned by this TaskMaster
        for(Task & task : mOwnedTasks)
        {
            task.update(deltaSecs);
        }
    };
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
        message(msgAcc);
        msgQueue.popCommit(msgAcc);
    }
}


template <class RendererT>
void TaskMaster<RendererT>::processTaskMessages()
{
    
}


template <class RendererT>
MessageResult TaskMaster<RendererT>::message(const MessageQueue::MessageAccessor& msgAcc)
{
    const Message & msg = msgAcc.message();

    // Handle messages sent to us, the TaskMaster
    if (msg.flags & kMessageFlag_TaskMaster)
    {
        switch(msg.msgId)
        {
        case FNV::fin:
            ASSERT(mIsRunning);
            mIsRunning = false;
            return MessageResult::Consumed;
        default:
            ERR("Unhandled message type, fnv: %d", msg.msgId);
            return MessageResult::Unhandled;
        }
    }
    else
    {
        // Message was meant for a task, send it appropriately
    }

    return MessageResult::Unhandled;
}

// Instantiate TaskMaster and helper funcs with our renderer class.
template class TaskMaster<RendererType>;
template void init_task_masters<RendererType>();
template void fin_task_masters<RendererType>();
template void start_game_loops<RendererType>();
template void message_from_main<RendererType>(thread_id threadId,
                                              fnv msgId);
template void message_from_main<RendererType>(thread_id threadId,
                                              fnv msgId,
                                              cell payload,
                                              const MessageBlock * pMsgBlock,
                                              size_t msgBlockCount);

} // namespace gaen



