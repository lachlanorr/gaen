//------------------------------------------------------------------------------
// MessageWriter.h - Generic message queue writer class for simple messages
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#include "gaen/engine/MessageQueue.h"
#include "gaen/engine/TaskMaster.h"

namespace gaen
{

// Base class that all writers should inherit from.
// Handles the message queue book keeping.
class MessageQueueWriter
{
public:
    // Choose standard message queue
    MessageQueueWriter(u32 msgId,
                       u32 flags,
                       task_id source,
                       task_id target,
                       cell payload,
                       u32 blockCount)
    {
        mpMsgQueue = get_message_queue(source, target);
        mpMsgQueue->pushBegin(&mMsgAcc,
                              msgId,
                              flags,
                              source,
                              target,
                              payload,
                              blockCount);
    }

    // Specific message queue
    MessageQueueWriter(u32 msgId,
                       u32 flags,
                       task_id source,
                       task_id target,
                       cell payload,
                       u32 blockCount,
                       MessageQueue * pMsgQueue)
    {
        mpMsgQueue = pMsgQueue;
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

    inline void commit()
    {
        mpMsgQueue->pushCommit(mMsgAcc);
        mIsCommitted = true;
    }

    inline const MessageQueueAccessor& accessor() const { return mMsgAcc; }
    inline MessageQueueAccessor& accessor() { return mMsgAcc; }

    inline Block & operator[] (u32 index)
    {
        return mMsgAcc[index];
    }

    inline const Block & operator[] (u32 index) const
    {
        return mMsgAcc[index];
    }

    template<typename T>
    void insertBlocks(u32 startIndex, const T & val)
    {
        ASSERT_MSG(sizeof(T) >= kBlockSize, "insertBlocks only valid for values larger than one block size");
        u32 blockCount = (sizeof(T) / kBlockSize) + (sizeof(T) % kBlockSize > 0 ? 1 : 0);
        const Block * pValBlocks = reinterpret_cast<const Block*>(&val);
        for (u32 i = 0; i < blockCount; ++i)
        {
            mMsgAcc[startIndex+i] = pValBlocks[i];
        }
    }

protected:
    MessageQueue * mpMsgQueue;
    MessageQueueAccessor mMsgAcc;
    bool mIsCommitted = false;
};

// Similar to MessageWriter, but used when writing to a header/block array.
// Used for messages not intended to be queued but constructed on the stack
// and sent directly to a message handler.
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

    const MessageBlockAccessor& accessor() const { return mMsgAcc; }
    MessageBlockAccessor& accessor() { return mMsgAcc; }

    // Access blocks of message
    Block & operator[] (u32 index)
    {
        return mMsgAcc[index];
    }

    const Block & operator[] (u32 index) const
    {
        return mMsgAcc[index];
    }

    template<typename T>
    void insertBlocks(u32 startIndex, const T& val)
    {
        ASSERT_MSG(sizeof(T) >= kBlockSize, "insertBlocks only valid for values larger than one block size");
        u32 blockCount = (sizeof(T) / kBlockSize) + (sizeof(T) % kBlockSize > 0 ? 1 : 0);
        const Block* pValBlocks = reinterpret_cast<const Block*>(&val);
        for (u32 i = 0; i < blockCount; ++i)
        {
            mMsgAcc[startIndex + i] = pValBlocks[i];
        }
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
    {}

private:
    Block mBlocks[blockCount + 1]; // +1 for header
};


template <u32 blockCount>
class ImmediateMessageWriter : public StackMessageBlockWriter<blockCount>
{
public:
    ImmediateMessageWriter(u32 msgId,
                           u32 flags,
                           task_id source,
                           task_id target,
                           cell payload)
      : StackMessageBlockWriter(msgId, flags, source, target, payload)
    {}
    ~ImmediateMessageWriter()
    {
        send_message(*this);
    }
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
