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
        size_t availableCells() const { return messageBlockCount() * kCellsPerMessageBlock + 1; }
        size_t availableDcells() const { return messageBlockCount() * kDcellsPerMessageBlock; }
        size_t availableQcells() const { return messageBlockCount(); }

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

        // Access cells of message
        cell & cellAt(size_t index)
        {
            ASSERT(mAccessor.available() > 0);
            return cellFromIndex(index);
        }

        // Access cells of message
        const cell & cellAt(size_t index) const
        {
            ASSERT(mAccessor.available() > 0);
            return cellFromIndex(index);
        }

        dcell & dcellAt(size_t index)
        {
            ASSERT(mAccessor.available() > 0);
            return dcellFromIndex(index);
        }

        const dcell & dcellAt(size_t index) const
        {
            ASSERT(mAccessor.available() > 0);
            return dcellFromIndex(index);
        }

        qcell & qcellAt(size_t index)
        {
            ASSERT(mAccessor.available() > 0);
            return qcellFromIndex(index);
        }

        const qcell & qcellAt(size_t index) const
        {
            ASSERT(mAccessor.available() > 0);
            return qcellFromIndex(index);
        }

    private:
        cell & cellFromIndex(size_t index) const
        {
            ASSERT(index < availableCells());
            size_t factoredIndex = index + 3; // since first available cell is last 4 bytes of header
            
            size_t indexIntoQueue = factoredIndex / kCellsPerMessageBlock;
            size_t cellIndexIntoMessageBlock = factoredIndex % kCellsPerMessageBlock;
            
            MessageBlock * pMsgBlock = reinterpret_cast<MessageBlock*>(&mAccessor[indexIntoQueue]);
            return pMsgBlock->cells[cellIndexIntoMessageBlock];
        }

        dcell & dcellFromIndex(size_t index) const
        {
            ASSERT(index < availableDcells());

            size_t indexIntoQueue = 1 + index / kDcellsPerMessageBlock;
            size_t dcellIndexIntoMessageBlock = index % kDcellsPerMessageBlock;
            
            MessageBlock * pMsgBlock = reinterpret_cast<MessageBlock*>(&mAccessor[indexIntoQueue]);
            return pMsgBlock->dcells[dcellIndexIntoMessageBlock];
        }

        qcell & qcellFromIndex(size_t index) const
        {
            ASSERT(index < availableQcells());

            size_t indexIntoQueue = 1 + index;
            size_t qcellIndexIntoMessageBlock = index;
            
            qcell * pqcell = reinterpret_cast<qcell*>(&mAccessor[indexIntoQueue]);
            return *pqcell;
        }

        size_t messageBlockCount() const
        {
            ASSERT(mAccessor.available() > 0);
            return mAccessor[0].size;
        }

        mutable SpscRingBuffer<Message>::Accessor mAccessor;
    };


    MessageQueue(size_t messageCount)
      : mRingBuffer(messageCount, kMT_Engine)
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
        pushHeader(&msgAcc, msgId, flags, source, target, 0);
        msgAcc.mAccessor[0].payload = payload;
        mRingBuffer.pushCommit(1);
    }

    void pushBegin(MessageAccessor * pMsgAcc,
                   fnv msgId,
                   u32 flags,
                   task_id source,
                   task_id target,
                   size_t msgBlockCount)
    {
        pushHeader(pMsgAcc, msgId, flags, source, target, msgBlockCount);
    }

    void pushCommit(const MessageAccessor & msgAcc)
    {
        // We always commit the Message Header, plus any additional MessageBlocks
        mRingBuffer.pushCommit(msgAcc.mAccessor[0].size + 1);
    }

    bool popBegin(MessageAccessor * pMsgAcc)
    {
        mRingBuffer.popBegin(&pMsgAcc->mAccessor);

        if (pMsgAcc->mAccessor.available() == 0)
            return false;

        // By inspecting the message, we know how many are actually available.
        u32 msgSize = pMsgAcc->mAccessor[0].size;

        ASSERT(pMsgAcc->mAccessor.available() == msgSize + 1);
        ASSERT(msgSize < pMsgAcc->mAccessor.available());

        return true;
    }

    void popCommit(const MessageAccessor & msgAcc)
    {
        ASSERT(msgAcc.mAccessor.available() >= msgAcc.mAccessor[0].size + 1);
        mRingBuffer.popCommit(msgAcc.mAccessor[0].size + 1);
    }

private:
    void pushHeader(MessageAccessor * pMsgAcc,
                    fnv msgId,
                    u32 flags,
                    task_id source,
                    task_id target,
                    size_t msgBlockCount)
    {
        mRingBuffer.pushBegin(&pMsgAcc->mAccessor, msgBlockCount+1); // + 1 for header
        
        Message & msg = pMsgAcc->mAccessor[0];
        msg.msgId = msgId;
        msg.flags = flags;
        msg.source = source;
        msg.target = target;
        msg.size = msgBlockCount;
    }

    SpscRingBuffer<Message> mRingBuffer;
};

} // namesapce gaen


#endif // #ifndef GAEN_ENGINE_MESSAGEQUEUE_H
