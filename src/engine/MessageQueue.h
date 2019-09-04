//------------------------------------------------------------------------------
// MessageQueue.h - Single producer, single consumer message queue
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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
#include "core/logging.h"
#include "hashes/hashes.h"
#include "engine/Message.h"
#include "engine/MessageAccessor.h"

#define VERBOSE_MESSAGE_LOGGING HAS__

#if HAS(VERBOSE_MESSAGE_LOGGING)
#define LOG_MESSAGE_DETAILS(caption, msgId, source, target) LOG_INFO("%s %s, source: %u, target: %u", caption, HASH::reverse_hash(msgId), source, target);
#else
#define LOG_MESSAGE_DETAILS(caption, msgId, source, target) do {} while(0)
#endif

namespace gaen
{

// We can have up to 16 Message blocks (16 bytes each) in a single message.
// Thus 16 * 16.  First 12 bytes are used for message header.  Last 4 of
// header can be used for payload.  And finally, we require null terminated
// strings in messages so they are easily consumed (thus the -1).
static const u32 kMaxMessageStringLen = 16 * 16 - 12 - 1; // 243

class MessageQueue
{
    friend class MessageQueueAccessor;
public:
    explicit MessageQueue(u32 messageCount)
      : mRingBuffer(messageCount, kMEM_Engine)
    {}

    // Convenience functions for single Message 16 byte messages
    void push(u32 msgId,
              u32 flags,
              task_id source,
              task_id target)
    {
        push(msgId, source, target, 0);
    }

    void push(u32 msgId,
              u32 flags,
              task_id source,
              task_id target,
              cell payload)
    {
        MessageQueueAccessor msgAcc;
        pushHeader(&msgAcc, msgId, flags, source, target, payload, 0);
        mRingBuffer.pushCommit(1);
    }

    void pushBegin(MessageQueueAccessor * pMsgAcc,
                   u32 msgId,
                   u32 flags,
                   task_id source,
                   task_id target,
                   cell payload,
                   u32 blockCount)
    {
        LOG_MESSAGE_DETAILS("pushBegin", msgId, source, target);

        pushHeader(pMsgAcc, msgId, flags, source, target, payload, blockCount);
    }

    void pushCommit(const MessageQueueAccessor & msgAcc)
    {
        LOG_MESSAGE_DETAILS("pushCommit", msgAcc.message().msgId, msgAcc.message().source, msgAcc.message().target);

        // We always commit the Message Header, plus any additional Blocks
        mRingBuffer.pushCommit(msgAcc.mAccessor[0].blockCount + 1);
    }

    // Transcribe a message into another queue
    void transcribeMessage(const MessageQueueAccessor & sourceAcc)
    {
        MessageQueueAccessor targetAcc;

        Message sourceMsg = sourceAcc.message();

        mRingBuffer.pushBegin(&targetAcc.mAccessor, sourceMsg.blockCount + 1); // + 1 for header

        for (u32 i = 0; i < sourceMsg.blockCount + 1; ++i) // + 1 for header
        {
            targetAcc.mAccessor[i] = sourceAcc.mAccessor[i];
        }

        mRingBuffer.pushCommit(sourceMsg.blockCount + 1); // + 1 for header
    }

    bool popBegin(MessageQueueAccessor * pMsgAcc)
    {
        mRingBuffer.popBegin(&pMsgAcc->mAccessor);

        if (pMsgAcc->mAccessor.available() == 0)
            return false;

        LOG_MESSAGE_DETAILS("popBegin", pMsgAcc->message().msgId, pMsgAcc->message().source, pMsgAcc->message().target);

        // By inspecting the message, we know how many are actually available.
        ASSERT(pMsgAcc->mAccessor.available() >= pMsgAcc->mAccessor[0].blockCount + 1);
        ASSERT(pMsgAcc->mAccessor[0].blockCount < pMsgAcc->mAccessor.available());

        return true;
    }

    void popCommit(const MessageQueueAccessor & msgAcc)
    {
        LOG_MESSAGE_DETAILS("popCommit", msgAcc.message().msgId, msgAcc.message().source, msgAcc.message().target);

        ASSERT(msgAcc.mAccessor.available() >= msgAcc.mAccessor[0].blockCount + (u32)1);
        mRingBuffer.popCommit(msgAcc.mAccessor[0].blockCount + (u32)1);
    }

private:
    void pushHeader(MessageQueueAccessor * pMsgAcc,
                    u32 msgId,
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


} // namespace gaen


#endif // #ifndef GAEN_ENGINE_MESSAGEQUEUE_H
