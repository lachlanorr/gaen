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
public:
    typedef SpscRingBuffer<Message>::Accessor MessageAccessor;
    
    MessageQueue(size_t messageCount)
      : mRingBuffer(messageCount, kMT_Engine)
    {}
    
    // Convenience functions for single Message 16 byte messages
    void push(fnv msgId, u32 flags, task_id source, task_id target)
    {
        push(msgId, source, target, 0);
    }
    
    void push(fnv msgId, u32 flags, task_id source, task_id target, cell payload)
    {
        MessageAccessor msgAcc;
        pushHeader(&msgAcc, msgId, flags, source, target, 0);
        msgAcc[0].payload = payload;
        mRingBuffer.pushCommit(1);
    }

    void popBegin(MessageAccessor * pMsgAcc)
    {
        mRingBuffer.popBegin(pMsgAcc);

        // By inspecting the message, we know how many are actually available.
        u32 msgSize = pMsgAcc->operator[](0).size;
        if (msgSize > 0)
        {
            ASSERT(msgSize < pMsgAcc->available());
            mRingBuffer.setAccessorAvailable(pMsgAcc, msgSize + 1);
        }
    }

    void popCommit(const MessageAccessor & msgAcc)
    {
        if (msgAcc.available() > 0)
            mRingBuffer.popCommit(msgAcc[0].size + 1);
    }
    

    // Convenience functions for various typical message types
    //pushVec3
    //pushVec4
    //pushMat34
    //pushMat44

private:
    void pushHeader(MessageAccessor * pMsgAcc,
                    fnv msgId,
                    u32 flags,
                    task_id source,
                    task_id target,
                    size_t additionalElemCount)
    {
        mRingBuffer.pushBegin(pMsgAcc, additionalElemCount+1); // + 1 for header
        Message & msg = (*pMsgAcc)[0];
        msg.msgId = msgId;
        msg.flags = flags;
        msg.source = source;
        msg.target = target;
        msg.size = additionalElemCount;
    }



    SpscRingBuffer<Message> mRingBuffer;
};

} // namesapce gaen


#endif // #ifndef GAEN_ENGINE_MESSAGEQUEUE_H
