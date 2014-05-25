//------------------------------------------------------------------------------
// MessageWriter.h - Generic message queue writer class for simple messages
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

#ifndef GAEN_ENGINE_MESSAGEWRITER_H
#define GAEN_ENGINE_MESSAGEWRITER_H

#include "engine/renderer_type.h"
#include "engine/MessageQueue.h"
#include "engine/TaskMaster.h"

namespace gaen
{

// Base class that wll writers should inherit from.
// Handles the message queue book keeping.
class MessageWriter
{
public:
    MessageWriter(u32 msgId,
                  u32 flags,
                  task_id source,
                  task_id target,
                  cell payload = to_cell(0),
                  u32 blockCount = 0)
    {
        mpMsgQueue = &GET_MESSAGE_QUEUE(msgId, flags, source, target);
        mpMsgQueue->pushBegin(&mMsgAcc,
                              msgId,
                              flags,
                              source,
                              target,
                              payload,
                              blockCount);
    }

    ~MessageWriter()
    {
        // RAII commit the thing if not already committed
        if (!mIsCommitted)
            commit();
    }

    void commit()
    {
        mpMsgQueue->pushCommit(mMsgAcc);
        mIsCommitted = true;
    }

    MessageQueue::MessageAccessor & accessor() { return mMsgAcc; }

protected:
    MessageQueue * mpMsgQueue;
    MessageQueue::MessageAccessor mMsgAcc;
    bool mIsCommitted = false;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGEWRITER_H
