//------------------------------------------------------------------------------
// InsertModelInstanceMessage.h - Autogenerated message class for InsertModelInstance
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

#ifndef GAEN_ENGINE_MESSAGES_INSERTMODELINSTANCEMESSAGE_H
#define GAEN_ENGINE_MESSAGES_INSERTMODELINSTANCEMESSAGE_H

#include "engine/MessageWriter.h"
#include "core/threading.h"
#include "engine/Model.h"
#include "engine/math.h"

namespace gaen
{
namespace msg
{

template <typename T>
class InsertModelInstanceR
{
public:
    InsertModelInstanceR(const T & msgAcc)
      : mMsgAcc(msgAcc)
    {
        if (&msgAcc[3] > &msgAcc[1])
        {
            // field is contiguous in ring buffer
            mpWorldTransform = reinterpret_cast<const Mat34*>(&msgAcc[1]);
        }
        else
        {
            // field wraps ring buffer, copy it into our datamember
            for (u32 i = 0; i < 3; ++i)
            {
                block_at(&mWorldTransform, i) = msgAcc[i + 1];
            }
            mpWorldTransform = &mWorldTransform;
        }

    }

    model_instance_id instanceId() const { return mMsgAcc.message().payload.u; }
    Model * model() const { return static_cast<Model *>(mMsgAcc[0].dCells[0].p); }
    bool isAssetManaged() const { return mMsgAcc[0].cells[2].b; }
    const Mat34 & worldTransform() const { return *mpWorldTransform; }
        
private:
    const T & mMsgAcc;

    // These members provide storage for fields larger than a block that wrap the ring buffer
    Mat34 mWorldTransform;
    const Mat34 * mpWorldTransform;
};

typedef InsertModelInstanceR<MessageQueueAccessor> InsertModelInstanceQR;
typedef InsertModelInstanceR<MessageBlockAccessor> InsertModelInstanceBR;

class InsertModelInstanceQW : protected MessageQueueWriter
{
public:
    InsertModelInstanceQW(u32 msgId,
                          u32 flags,
                          task_id source,
                          task_id target,
                          model_instance_id instanceId)
      : MessageQueueWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(instanceId),
                           4)
    {}
    
    void setInstanceId(model_instance_id val) { mMsgAcc.message().payload.u = val; }
    void setModel(Model * pVal) { mMsgAcc[0].dCells[0].p = pVal; }
    void setIsAssetManaged(bool val) { mMsgAcc[0].cells[2].b = val; }
    void setWorldTransform(const Mat34 & val)
    {
        for (u32 i = 0; i < 3; ++i)
        {
            mMsgAcc[i + 1] = block_at(&val, i);
        }
    }
};

class InsertModelInstanceBW : protected MessageBlockWriter
{
public:
    InsertModelInstanceBW(u32 msgId,
                          u32 flags,
                          task_id source,
                          task_id target,
                          Block * pBlocks,
                          u32 blockCount,
                          model_instance_id instanceId)
      : MessageBlockWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(instanceId),
                           4,
                           mBlocks)
    {}

    void setInstanceId(model_instance_id val) { mMsgAcc.message().payload.u = val; }
    void setModel(Model * pVal) { mMsgAcc[0].dCells[0].p = pVal; }
    void setIsAssetManaged(bool val) { mMsgAcc[0].cells[2].b = val; }
    void setWorldTransform(const Mat34 & val)
    {
        for (u32 i = 0; i < 3; ++i)
        {
            mMsgAcc[i + 1] = block_at(&val, i);
        }
    }

private:
    Block mBlocks[4 + 1]; // +1 for header
};

} // namespace msg
} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGES_INSERTMODELINSTANCEMESSAGE_H

