//------------------------------------------------------------------------------
// BaseMsg.h - Base message queue writer to handle book keeping
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

#ifndef GAEN_ENGINE_MESSAGE_HELPERS_BASEMSG_H
#define GAEN_ENGINE_MESSAGE_HELPERS_BASEMSG_H

#include "engine/renderer_type.h"
#include "engine/MessageQueue.h"
#include "engine/TaskMaster.h"

namespace gaen
{

// Base class that wll writers should inherit from.
// Handles the message queue book keeping.
class BaseMsgWriter
{
    // protected because we don't want the destructor ever called on this class.
    // That would be improper use, and break stuff potentially since we don't have
    // a virtual destructor.
protected:
    BaseMsgWriter(fnv msgId,
                  u32 flags,
                  task_id source,
                  task_id target,
                  size_t msgBlockCount)
    {
        mpMsgQueue = &MESSAGE_QUEUE_FOR_TARGET(target);
        mpMsgQueue->pushBegin(&mMsgAcc,
                              msgId,
                              flags,
                              source,
                              target,
                              to_cell(0),
                              msgBlockCount);
    }

    ~BaseMsgWriter()
    {
        if (!mIsCommitted)
            commit();
    }

    void commit()
    {
        mpMsgQueue->pushCommit(mMsgAcc);
        mIsCommitted = true;
    }

    MessageQueue * mpMsgQueue;
    MessageQueue::MessageAccessor mMsgAcc;
    bool mIsCommitted = false;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGE_HELPERS_BASEMSG_H
