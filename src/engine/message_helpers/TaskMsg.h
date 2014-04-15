//------------------------------------------------------------------------------
// TaskMsg.h - Helper for data in insert task meessages
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

#ifndef GAEN_ENGINE_MESSAGE_HELPERS_TASKMSG_H
#define GAEN_ENGINE_MESSAGE_HELPERS_TASKMSG_H

#include "engine/message_helpers/BaseMsg.h"

namespace gaen
{


// Helper classes to encode/decode messages
// Block structure of message is as follows
// * Header (0-15)
//   Unused payload
// * Block 1 (16-31)
//   Task bytes 0-15
// * Block 2 (32-47)
//   Task bytes 16-31

class TaskMsgReader
{
public:
    TaskMsgReader(const MessageQueue::MessageAccessor & msgAcc)
      : mMsgAcc(msgAcc)
    {
        const qcell & task_0 = msgAcc[0].q;
        const qcell & task_1 = msgAcc[1].q;
        if (&task_1 > &task_0)
            mpTask = reinterpret_cast<const Task*>(&task_0);
        else
        {
            qcell_at(&mTask, 0) = task_0;
            qcell_at(&mTask, 1) = task_1;
            
            mpTask = &mTask;
        }
    }

    thread_id owner() const { return mMsgAcc.message().payload.u; }
    
    const Task & task() const { return *mpTask; }
        
private:
    const MessageQueue::MessageAccessor & mMsgAcc;
    Task mTask;
    const Task * mpTask;
};



class TaskMsgWriter : protected BaseMsgWriter
{
public:
    TaskMsgWriter(fnv msgId,
                  u32 flags,
                  task_id source,
                  task_id target)
      : BaseMsgWriter(msgId,
                      flags,
                      source,
                      target,
                      2) {}

    void setOwner(thread_id owner) { mMsgAcc.message().payload.u = owner; }

    void setTask(Task & task)
    {
        mMsgAcc[0].q = qcell_at(&task, 0);
        mMsgAcc[1].q = qcell_at(&task, 1);
    }
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGE_HELPERS_TASKMSG_H
