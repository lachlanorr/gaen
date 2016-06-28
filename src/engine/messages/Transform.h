//------------------------------------------------------------------------------
// Transform.h - Autogenerated message class for Transform
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

#ifndef GAEN_ENGINE_MESSAGES_TRANSFORMMESSAGE_H
#define GAEN_ENGINE_MESSAGES_TRANSFORMMESSAGE_H

#include "engine/MessageWriter.h"
#include "glm/mat4x3.hpp"
#include "core/threading.h"
#include "engine/Task.h"

namespace gaen
{
namespace messages
{

template <typename T>
class TransformR
{
public:
    TransformR(const T & msgAcc)
      : mMsgAcc(msgAcc)
    {
        if (&msgAcc[2] > &msgAcc[0])
        {
            // field is contiguous in ring buffer
            mpTransform = reinterpret_cast<const glm::mat4x3*>(&msgAcc[0]);
        }
        else
        {
            // field wraps ring buffer, copy it into our datamember
            for (u32 i = 0; i < 3; ++i)
            {
                block_at(&mTransform, i) = msgAcc[i + 0];
            }
            mpTransform = &mTransform;
        }

    }

    const glm::mat4x3 & transform() const { return *mpTransform; }
    bool isLocal() const { return (bool)mMsgAcc.message().payload.b; }
        
private:
    TransformR(const TransformR &)              = delete;
    TransformR(const TransformR &&)             = delete;
    TransformR & operator=(const TransformR &)  = delete;
    TransformR & operator=(const TransformR &&) = delete;

    const T & mMsgAcc;

    // These members provide storage for fields larger than a block that wrap the ring buffer
    glm::mat4x3 mTransform;
    const glm::mat4x3 * mpTransform;
};

typedef TransformR<MessageQueueAccessor> TransformQR;
typedef TransformR<MessageBlockAccessor> TransformBR;

class TransformQW : public MessageQueueWriter
{
public:
    TransformQW(u32 msgId,
                u32 flags,
                task_id source,
                task_id target,
                bool isLocal)
      : MessageQueueWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(*reinterpret_cast<const u32*>(&isLocal)),
                           3)
    {}

    TransformQW(u32 msgId,
                u32 flags,
                task_id source,
                task_id target,
                bool isLocal,
                MessageQueue * pMsgQueue)
      : MessageQueueWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(*reinterpret_cast<const u32*>(&isLocal)),
                           3,
                           pMsgQueue)
    {}
    
    void setTransform(const glm::mat4x3 & val)
    {
        for (u32 i = 0; i < 3; ++i)
        {
            mMsgAcc[i + 0] = block_at(&val, i);
        }
    }
    void setIsLocal(bool val) { mMsgAcc.message().payload.b = (bool)val; }
private:
    TransformQW(const TransformQW &)              = delete;
    TransformQW(const TransformQW &&)             = delete;
    TransformQW & operator=(const TransformQW &)  = delete;
    TransformQW & operator=(const TransformQW &&) = delete;
};

class TransformBW : public MessageBlockWriter
{
public:
    TransformBW(u32 msgId,
                u32 flags,
                task_id source,
                task_id target,
                bool isLocal)
      : MessageBlockWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(*reinterpret_cast<const u32*>(&isLocal)),
                           3,
                           mBlocks)
    {}

    void setTransform(const glm::mat4x3 & val)
    {
        for (u32 i = 0; i < 3; ++i)
        {
            mMsgAcc[i + 0] = block_at(&val, i);
        }
    }
    void setIsLocal(bool val) { mMsgAcc.message().payload.b = (bool)val; }

private:
    TransformBW(const TransformBW &)              = delete;
    TransformBW(const TransformBW &&)             = delete;
    TransformBW & operator=(const TransformBW &)  = delete;
    TransformBW & operator=(const TransformBW &&) = delete;

    Block mBlocks[3 + 1]; // +1 for header
};

} // namespace msg
} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGES_TRANSFORMMESSAGE_H

