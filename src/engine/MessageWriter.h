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

#include "engine/MessageQueue.h"
#include "engine/TaskMaster.h"

namespace gaen
{

// Base class that all writers should inherit from.
// Handles the message queue book keeping.
class MessageQueueWriter
{
public:
    MessageQueueWriter(u32 msgId,
                       u32 flags,
                       task_id source,
                       task_id target,
                       cell payload,
                       u32 blockCount)
    {
        mpMsgQueue = get_message_queue(msgId, flags, source, target);
        mpMsgQueue->pushBegin(&mMsgAcc,
                              msgId,
                              flags,
                              source,
                              target,
                              payload,
                              blockCount);
    }

    ~MessageQueueWriter()
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

    MessageQueueAccessor & accessor() { return mMsgAcc; }

    Block & operator[] (u32 index)
    {
        return mMsgAcc[index];
    }

    const Block & operator[] (u32 index) const
    {
        return mMsgAcc[index];
    }

protected:
    MessageQueue * mpMsgQueue;
    MessageQueueAccessor mMsgAcc;
    bool mIsCommitted = false;
};

// Similar to MessageWriter, but used when writing to a header/block array.
// Used for messages not intended to be queued but constructed on the stack
// and sent directly to a messsage handler.
class MessageBlockWriter
{
public:
    MessageBlockWriter(u32 msgId,
                       u32 flags,
                       task_id source,
                       task_id target,
                       cell payload,
                       u32 blockCount,
                       Block * pBlocks)
      : mMsgAcc(pBlocks, blockCount)
    {
        mMsgAcc.message() = Message(msgId, flags, source, target, payload, blockCount);
    }

    MessageBlockAccessor & accessor() { return mMsgAcc; }

    // Access blocks of message
    Block & operator[] (u32 index)
    {
        return mMsgAcc[index];
    }

    const Block & operator[] (u32 index) const
    {
        return mMsgAcc[index];
    }

protected:
    MessageBlockAccessor mMsgAcc;
};

// Simple block writer that allocates storage for us on the stack.
template <u32 blockCount>
class StackMessageBlockWriter : public MessageBlockWriter
{
public:
    StackMessageBlockWriter(u32 msgId,
                            u32 flags,
                            task_id source,
                            task_id target,
                            cell payload)
      : MessageBlockWriter(msgId, flags, source, target, payload, blockCount, mBlocks)
    {
    }

    // Access blocks of message
    Block & operator[] (u32 index)
    {
        ASSERT(index < blockCount);
        return mBlocks[index+1]; // +1 to skip past header
    }

    const Block & operator[] (u32 index) const
    {
        ASSERT(index < blockCount);
        return mBlocks[index+1]; // +1 to skip past header
    }

private:
    Block mBlocks[blockCount + 1]; // +1 for header
};


// Simple block writer that allocates storage in thread local storage
class ThreadLocalMessageBlockWriter : public MessageBlockWriter
{
public:
    ThreadLocalMessageBlockWriter(u32 msgId,
                                  u32 flags,
                                  task_id source,
                                  task_id target,
                                  cell payload,
                                  u32 blockCount);

    // Access blocks of message
    Block & operator[] (u32 index);
    const Block & operator[] (u32 index) const;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGEWRITER_H
