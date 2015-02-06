//------------------------------------------------------------------------------
// TransformId.h - Autogenerated message class for TransformId
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#ifndef GAEN_ENGINE_MESSAGES_TRANSFORMIDMESSAGE_H
#define GAEN_ENGINE_MESSAGES_TRANSFORMIDMESSAGE_H

#include "engine/MessageWriter.h"
#include "engine/math.h"
#include "core/threading.h"

namespace gaen
{
namespace messages
{

template <typename T>
class TransformIdR
{
public:
    TransformIdR(const T & msgAcc)
      : mMsgAcc(msgAcc)
    {
        if (&msgAcc[2] > &msgAcc[0])
        {
            // field is contiguous in ring buffer
            mpTransform = reinterpret_cast<const Mat34*>(&msgAcc[0]);
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

    const Mat34 & transform() const { return *mpTransform; }
    u32 id() const { return mMsgAcc.message().payload.u; }
        
private:
    const T & mMsgAcc;

    // These members provide storage for fields larger than a block that wrap the ring buffer
    Mat34 mTransform;
    const Mat34 * mpTransform;
};

typedef TransformIdR<MessageQueueAccessor> TransformIdQR;
typedef TransformIdR<MessageBlockAccessor> TransformIdBR;

class TransformIdQW : public MessageQueueWriter
{
public:
    TransformIdQW(u32 msgId,
                  u32 flags,
                  task_id source,
                  task_id target,
                  u32 id)
      : MessageQueueWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(id),
                           3)
    {}
    
    void setTransform(const Mat34 & val)
    {
        for (u32 i = 0; i < 3; ++i)
        {
            mMsgAcc[i + 0] = block_at(&val, i);
        }
    }
    void setId(u32 val) { mMsgAcc.message().payload.u = val; }
};

class TransformIdBW : public MessageBlockWriter
{
public:
    TransformIdBW(u32 msgId,
                  u32 flags,
                  task_id source,
                  task_id target,
                  u32 id)
      : MessageBlockWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(id),
                           3,
                           mBlocks)
    {}

    void setTransform(const Mat34 & val)
    {
        for (u32 i = 0; i < 3; ++i)
        {
            mMsgAcc[i + 0] = block_at(&val, i);
        }
    }
    void setId(u32 val) { mMsgAcc.message().payload.u = val; }

private:
    Block mBlocks[3 + 1]; // +1 for header
};

} // namespace msg
} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGES_TRANSFORMIDMESSAGE_H

