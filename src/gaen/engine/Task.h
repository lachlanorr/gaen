//------------------------------------------------------------------------------
// Task.h - Base definition of an updatable task
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

#ifndef GAEN_ENGINE_TASK_H
#define GAEN_ENGINE_TASK_H

#include "gaen/core/base_defines.h"
#include "gaen/core/logging.h"
#include "gaen/engine/MessageAccessor.h"
#include "gaen/hashes/hashes.h"

#define MESSAGE_TRACING HAS__

namespace gaen
{

// We only have 28 bits per task_id inside of the Task and Message structures.
// If you change the size in either of those structs, all of them must change.
static const task_id kMaxTaskId = (1 << 28) - 1;

// Note: Target task id's less than kMaxThreads are implicitly considered as thread ids

static const task_id kInvalidTaskId       = kMaxTaskId - 0; // 268435455
static const task_id kPrimaryThreadTaskId = kMaxTaskId - 1; // 268435454, special task id used to refer to the main thread as a task

// Reserved primary task master task ids
static const task_id kPrimaryTaskIdMax   = kMaxTaskId - 3; // 268435452 -- Change as appropriate if we add more "primary" task ids
static const task_id kRendererTaskId     = kMaxTaskId - 3; // 268435452
static const task_id kInputMgrTaskId     = kMaxTaskId - 4; // 268435451
static const task_id kAssetMgrTaskId     = kMaxTaskId - 5; // 268435450
static const task_id kSpriteMgrTaskId    = kMaxTaskId - 6; // 268435449
static const task_id kModelMgrTaskId     = kMaxTaskId - 7; // 268435448
static const task_id kAudioMgrTaskId     = kMaxTaskId - 8; // 268435447
static const task_id kEditorTaskId       = kMaxTaskId - 9; // 268435446
static const task_id kPrimaryTaskIdMin   = kMaxTaskId - 9; // 268435446 -- Change as appropriate if we add more "primary" task ids

// If another "special" task id is added, update this function appropriately.
static inline bool is_primary_task(task_id taskId)
{
    return taskId <= kPrimaryTaskIdMax && taskId >= kPrimaryTaskIdMin;
}

enum class TaskStatus : u8
{
    Initializing = 0,
    Running = 1,
    Paused = 2,
    Dead = 3
};

enum class TaskPermissions : u8
{
    Private = 0,
    Group,
    Guild,
    Public
};

// Return the next globally unique task id
task_id next_task_id();

#if HAS(TRACK_HASHES)
bool register_task(task_id id, u32 nameHash);
const char * task_name(task_id id);
#endif

//------------------------------------------------------------------------------
// Tasks are the base work unit for the TaskMaster scheduler.
// They're small (32 bytes) and can be stored contiguously to iterate over
// them in a cache friendly fashion.
//
// Usage:
//   pObj must implement methods:
//     void update(f32 delta);
//     // Queue Message
//     MessageResult message(const MessageQueueAccessor& msgAcc);
//     // Block Message
//     MessageResult message(const MessageBlockAccessor& msgAcc);
//
// E.g.
//   Task t = Task::create(pObj);
//
//
// Instead of an object hierarchy with virtual methods for update and
// message functions, we opt for this delegate'ish approach. It
// eliminates at a dereference on each, call to update() and message()
// methods. It also alows us to aggregate tasks in data structures
// even though they don't share any type similarities except for the
// loose duck typing of the required methods.
//
// This can be thought of as a special case delegate with three methods,
// modeled after Sergey Ryazanov's Impossibly Fast C++ Delegates
// article at:
// http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates
//------------------------------------------------------------------------------
struct Task
{
public:
    Task() = default;

    static Task blank()
    {
        Task task;
        // set 32 bytes to 0
        u64 * pData = reinterpret_cast<u64*>(&task);
        pData[0] = 0;
        pData[1] = 0;
        pData[2] = 0;
        pData[3] = 0;
        return task;
    }

    template <class T>
    static Task create(T* pThat, u32 nameHash)
    {
        Task task;

        task.mStatus = static_cast<u8>(TaskStatus::Initializing);
        task.mPermissions = 0;
        task.mTaskId = next_task_id();

        task.mNameHash = nameHash;

#if HAS(TRACK_HASHES)
        register_task(task.mTaskId, task.mNameHash);
#endif // HAS(TRACK_HASHES)
#if HAS(MESSAGE_TRACING)
        LOG_INFO("TASK: %s(0x%x)", task_name(task.mNameHash), task.mTaskId);
#endif

        task.mpThat = pThat;

        task.mpMessageQueueStub = &message_stub<T>;
        MessageBlockStub messageBlockStub = &message_immediate_stub<T>;

        std::intptr_t iptrMessageQueueStub = reinterpret_cast<std::intptr_t>(task.mpMessageQueueStub);
        std::intptr_t iptrMessageBlockStub = reinterpret_cast<std::intptr_t>(messageBlockStub);

        // We only store one real function pointer to the stubs, and
        // the others are offsets to this pointer.  This allows us to
        // use 8 bytes for the first, but only 4 bytes each for each
        // additional message.
        task.mMessageBlockStubOffset = static_cast<i32>(iptrMessageBlockStub - iptrMessageQueueStub);

        // Not all tasks are updatable.  To create an updatable one,
        // use createUpdatable static function below.
        task.mUpdateStubOffset = 0;

        return task;
    }

    template <class T>
    static Task create_updatable(T* pThat, u32 nameHash)
    {
        Task task = Task::create(pThat, nameHash);

        // This task is updatable, so calculate the offset to that method as well.
        std::intptr_t iptrMessageQueueStub = reinterpret_cast<std::intptr_t>(task.mpMessageQueueStub);
        UpdateStub updateStub = &update_stub<T>;
        std::intptr_t iptrUpdateStub = reinterpret_cast<std::intptr_t>(updateStub);
        task.mUpdateStubOffset = static_cast<i32>(iptrUpdateStub - iptrMessageQueueStub);

        return task;
    }

    task_id id() const { return mTaskId; }

    u32 nameHash() const { return mNameHash; }

    TaskStatus status() const { return static_cast<TaskStatus>(mStatus); }
    void setStatus(TaskStatus newStatus)
    {
        // LORRTEMP
        //LOG_INFO("Task::setStatus - task: %s, oldStatus: %d, newStatus: %d", HASH::reverse_hash(mNameHash), mStatus, newStatus);
        mStatus = static_cast<u8>(newStatus);
    }

    TaskPermissions permissions() const { return static_cast<TaskPermissions>(mPermissions); }
    void setPermissions(TaskPermissions newPermissions) { mPermissions = static_cast<u8>(newPermissions); }

    void * that() { return mpThat; }
    const void * that() const { return mpThat; }

    // Queued message
    MessageResult message(const MessageQueueAccessor & msgAcc)
    {
        return (*mpMessageQueueStub)(mpThat, msgAcc);
    }

    // Block message
    MessageResult message(const MessageBlockAccessor & msgAcc)
    {
        // Since we're storing the offset of the message stub from the
        // update stub we do a little pointer arithmetic to get the
        // actual address.

        std::intptr_t iptrMessageQueueStub = reinterpret_cast<std::intptr_t>(mpMessageQueueStub);
        std::intptr_t iptrMessageBlockStub = iptrMessageQueueStub + mMessageBlockStubOffset;

        MessageBlockStub pMessageBlockStub = reinterpret_cast<MessageBlockStub>(iptrMessageBlockStub);
        return (*pMessageBlockStub)(mpThat, msgAcc);
    }

    void update(f32 delta)
    {
        // Since we're storing the offset of the message stub from the
        // update stub we do a little pointer arithmetic to get the
        // actual address.

        if (mUpdateStubOffset != 0 && // some tasks are not updateable
            status() == TaskStatus::Running) // only running tasks are updated
        {
            std::intptr_t iptrMessageQueueStub = reinterpret_cast<std::intptr_t>(mpMessageQueueStub);
            std::intptr_t iptrUpdateStub = iptrMessageQueueStub + mUpdateStubOffset;

            UpdateStub pUpdateStub = reinterpret_cast<UpdateStub>(iptrUpdateStub);
            (*pUpdateStub)(mpThat, delta);
        }
    }


private:
    typedef void (*UpdateStub)(void*, f32);

    // Message coming from the processing of a MessageQueue
    typedef MessageResult (*MessageQueueStub)(void*, const MessageQueueAccessor&);

    // Message coming from someone for immediate processing (no MessageQueue access)
    typedef MessageResult (*MessageBlockStub)(void*, const MessageBlockAccessor&);


    u32 mStatus:2;           // current running state
    u32 mPermissions:2;      // permissions for others to send messages
    u32 mTaskId:28;          // our task id - NOTE Changing this size requires changing kMaxTaskId in Task.h

    u32 mNameHash;           // storage for a name hash, typically an entity or component name

    MessageQueueStub mpMessageQueueStub; // address of message method
    PAD_IF_32BIT_A
    i32 mMessageBlockStubOffset;         // offset in bytes from MessageQueueStub pointer to MessageBlockStub pointer
    i32 mUpdateStubOffset;               // offset in bytes from MessageQueueStub pointer to UpdateStub pointer

    void* mpThat;                     // pointer to class instance that has the real update/message methods
    PAD_IF_32BIT_B

    template <class T>
    static void update_stub(void* pThat, f32 delta)
    {
        T* p = static_cast<T*>(pThat);
        p->update(delta);
    }

    template <class T>
    static MessageResult message_stub(void* pThat, const MessageQueueAccessor& msgAcc)
    {
        T* p = static_cast<T*>(pThat);
        return p->message(msgAcc);
    }

    template <class T>
    static MessageResult message_immediate_stub(void* pThat, const MessageBlockAccessor& msgAcc)
    {
        T* p = static_cast<T*>(pThat);
        return p->message(msgAcc);
    }
};

// We make assumptions about this size, like alignment, pass by value, et al.
// If the size needs to change in future, care must be taken to re-assess these
// assumptions as they appear throughout the code.
static_assert(sizeof(Task)==32, "Task should be 32 bytes");

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_TASK_H
