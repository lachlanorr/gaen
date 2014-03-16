//------------------------------------------------------------------------------
// Task.h - Base definition of an updatable task
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

#ifndef GAEN_ENGINE_TASK_H
#define GAEN_ENGINE_TASK_H

#include "core/base_defines.h"
#include "engine/MessageQueue.h"

namespace gaen
{

static const task_id kRootTaskId = 0; // A task with this as their parent is a top level, root task

enum class TaskStatus : u8
{
    Initializing = 0,
    Paused,
    Running,
    Dead
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

//------------------------------------------------------------------------------
// Tasks are the base work unit for the TaskMaster scheduler.
// They're small (32 bytes) and can be stored contiguously to iterate over
// them in a cache friendly fashion.
//
// Usage:
//   pObj must implement methods:
//     void update(f32 deltaSecs);
//     MessageResult message(const MessageQueue::MessageAccessor& msgAcc);
//     void setTaskId(task_id taskId);
//
// E.g.
//   Task t = Task::create(pObj);
//   
//
// Instead of an object hierarchy with virtual methods for
// update and message functions, we opt for this delegate'ish
// approach.  It eliminates the It eliminates at least one dereference on each, and the
// compiler can completely inline the delegate call in some cases.
//
// This can be thought of as a special case delegate with two methods,
// modeled after Sergey Ryazanov's Impossibly Fast C++ Delegates
// article at:
// http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates
//------------------------------------------------------------------------------
struct Task
{
public:
    Task()
    {
        // zero out all fields
        // Simpler than setting all or calling memset.
        // Task is guaranteed to be 32 bytes, see static_assert further down.
        u64 * p64 = reinterpret_cast<u64*>(this);
        *p64++ = 0;
        *p64++ = 0;
        *p64++ = 0;
        *p64 = 0;
    }

    template <class T>
    static Task create(T* pThat)
    {
        Task task;

        task.mStatus = static_cast<u8>(TaskStatus::Initializing);
        task.mPermissions = 0;
        task.mTaskId = next_task_id();

        pThat->setTaskId(task.mTaskId);

        task.mIndex = 0;
        task.mParent = 0;
        task.mFirstSibling = 0;
        task.mFirstChild = 0;
        
        task.mpThat = pThat;

        task.mpUpdateStub = &updateStub<T>;
        MessageStub messageStub_ = &messageStub<T>;

        std::intptr_t iptrUpdateStub = reinterpret_cast<std::intptr_t>(task.mpUpdateStub);
        std::intptr_t iptrMessageStub = reinterpret_cast<std::intptr_t>(messageStub_);

        // We don't store the address of the messageStub, but the offset from the updateStub.
        // In this way we save 4 bytes.  It is a reasonable assumption that the two stubs
        // will be within 2^31 bytes of each other, and we need those 4 bytes for other stuff.
        task.mpMessageStubOffset = static_cast<i32>(iptrMessageStub - iptrUpdateStub);

        return task;
    }

    task_id id() { return mTaskId; }

    TaskStatus status() { return static_cast<TaskStatus>(mStatus); }
    void setStatus(TaskStatus newStatus) { mStatus = static_cast<u8>(newStatus); }

    TaskPermissions permissions() { return static_cast<TaskPermissions>(mPermissions); }
    void setPermissions(TaskPermissions newPermissions) { mPermissions = static_cast<u8>(newPermissions); }

    u16 index() { return mIndex; }
    void setIndex(u16 index) { mIndex = index; }

    u16 parent() { return mParent; }
    void setParent(u16 parent) { mParent = parent; }
    
    u16 firstSibling() { return mFirstSibling; }
    void setFirstSibling(u16 firstSibling) { mFirstSibling = firstSibling; }

    u16 firstChild() { return mFirstChild; }
    void setFirstChild(u16 firstChild) { mFirstChild = firstChild; }

    void * that() { return mpThat; }


    void update(f32 deltaSecs)
    {
        (*mpUpdateStub)(mpThat, deltaSecs);
    }

    MessageResult message(const MessageQueue::MessageAccessor& msgAcc)
    {
        // Since we're storing the offset of the message stub from the
        // update stub we do a little pointer arithmetic to get the
        // actual address.

        std::intptr_t iptrUpdateStub = reinterpret_cast<std::intptr_t>(mpUpdateStub);
        std::intptr_t iptrMessageStub = iptrUpdateStub + mpMessageStubOffset;
        
        MessageStub pMessageStub = reinterpret_cast<MessageStub>(iptrMessageStub);
        return (*pMessageStub)(mpThat, msgAcc);
    }


private:
    typedef void (*UpdateStub)(void*, f32);
    typedef MessageResult (*MessageStub)(void*, const MessageQueue::MessageAccessor&);

    u32 mStatus:2;           // current running state
    u32 mPermissions:2;      // permissions for others to send messages
    u32 mTaskId:28;          // our task id

    u16 mIndex;              // 0 based index to of task... used in TaskTree for fast tree traversal
    u16 mParent;             // 0 based index to parent... used in TaskTree for fast tree traversal
    u16 mFirstSibling;       // 0 based index to first sibling... used in TaskTree for fast tree traversal
    u16 mFirstChild;         // 0 based index to first child... used in TaskTree for fast tree traversal

    i32 mpMessageStubOffset; // offset in bytes from UpdateStub pointer to MessageStub pointer
    UpdateStub mpUpdateStub; // address of update method
    void* mpThat;            // pointer to class instance that has the real update/message methods

    template <class T>
    static void updateStub(void* pThat, f32 deltaSecs)
    {
        T* p = static_cast<T*>(pThat);
        p->update(deltaSecs);
    }

    template <class T>
    static MessageResult messageStub(void* pThat, const MessageQueue::MessageAccessor& msgAcc)
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
