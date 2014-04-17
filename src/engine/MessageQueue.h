//------------------------------------------------------------------------------
// MessageQueue.h - Single producer, single consumer message queue
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

#ifndef GAEN_ENGINE_MESSAGEQUEUE_H
#define GAEN_ENGINE_MESSAGEQUEUE_H

#include "core/SpscRingBuffer.h"

#include "engine/Message.h"

namespace gaen
{

// We can have up to 16 Message blocks (16 bytes each) in a single message.
// Thus 16 * 16.  First 12 bytes are used for message header.  Last 4 of
// header can be used for payload.  And finally, we require null terminated
// strings in messages so they are easily consumed (thus the -1).
static const size_t kMaxMessageStringLen = 16 * 16 - 12 - 1; // 243

class MessageQueue
{
    friend class MessageAccessor;
public:
    // Provides cell access to message.  Message Header contains one 32 bit cell,
    // each subsequent "Message Header" contains 4 more cells.
    class MessageAccessor
    {
        friend MessageQueue;
    public:
        Message & message()
        {
            ASSERT(mAccessor.available() > 0);
            return mAccessor[0];
        }

        const Message & message() const
        {
            ASSERT(mAccessor.available() > 0);
            return mAccessor[0];
        }

        // Access blocks of message
        MessageBlock & operator[] (size_t index)
        {
            return blockFromIndex(index);
        }

        const MessageBlock & operator[] (size_t index) const
        {
            return blockFromIndex(index);
        }

    private:
        MessageBlock & blockFromIndex(size_t index) const
        {
            ASSERT(index < mAccessor.available()-1); // -1 since Message header is always present
            ASSERT(index < blockCount());

             // +1 since Message header is always present
            MessageBlock * pBlock = reinterpret_cast<MessageBlock*>(&mAccessor[1+index]);

            return *pBlock;
        }

        size_t blockCount() const
        {
            ASSERT(mAccessor.available() > 0);
            return mAccessor[0].blockCount;
        }

        mutable SpscRingBuffer<Message>::Accessor mAccessor;
    };


    MessageQueue(size_t messageCount)
      : mRingBuffer(messageCount, kMEM_Engine)
    {}
    
    // Convenience functions for single Message 16 byte messages
    void push(fnv msgId,
              u32 flags,
              task_id source,
              task_id target)
    {
        push(msgId, source, target, 0);
    }
    
    void push(fnv msgId,
              u32 flags,
              task_id source,
              task_id target,
              cell payload)
    {
        MessageAccessor msgAcc;
        pushHeader(&msgAcc, msgId, flags, source, target, payload, 0);
        mRingBuffer.pushCommit(1);
    }

    void pushBegin(MessageAccessor * pMsgAcc,
                   fnv msgId,
                   u32 flags,
                   task_id source,
                   task_id target,
                   cell payload = to_cell(0),
                   u32 blockCount = 0)
    {
        pushHeader(pMsgAcc, msgId, flags, source, target, payload, blockCount);
    }

    void pushCommit(const MessageAccessor & msgAcc)
    {
        // We always commit the Message Header, plus any additional MessageBlocks
        mRingBuffer.pushCommit(msgAcc.mAccessor[0].blockCount + 1);
    }

    bool popBegin(MessageAccessor * pMsgAcc)
    {
        mRingBuffer.popBegin(&pMsgAcc->mAccessor);

        if (pMsgAcc->mAccessor.available() == 0)
            return false;

        // By inspecting the message, we know how many are actually available.
        u32 msgSize = pMsgAcc->mAccessor[0].blockCount;

        ASSERT(pMsgAcc->mAccessor.available() == msgSize + 1);
        ASSERT(msgSize < pMsgAcc->mAccessor.available());

        return true;
    }

    void popCommit(const MessageAccessor & msgAcc)
    {
        ASSERT(msgAcc.mAccessor.available() >= msgAcc.mAccessor[0].blockCount + 1);
        mRingBuffer.popCommit(msgAcc.mAccessor[0].blockCount + 1);
    }

private:
    void pushHeader(MessageAccessor * pMsgAcc,
                    fnv msgId,
                    u32 flags,
                    task_id source,
                    task_id target,
                    cell payload,
                    u32 msgBlockCount)
    {
        ASSERT(flags         < (2 << 4)  &&
               msgBlockCount < (2 << 4)  &&
               source        < (2 << 28) &&
               target        < (2 << 28));
        
        mRingBuffer.pushBegin(&pMsgAcc->mAccessor, msgBlockCount+1); // + 1 for header
        
        Message & msg = pMsgAcc->mAccessor[0];
        msg.msgId = msgId;
        msg.flags = flags;
        msg.source = source;
        msg.target = target;
        msg.payload = payload;
        msg.blockCount = msgBlockCount;
    }

    SpscRingBuffer<Message> mRingBuffer;
};


} // namesapce gaen


#endif // #ifndef GAEN_ENGINE_MESSAGEQUEUE_H
