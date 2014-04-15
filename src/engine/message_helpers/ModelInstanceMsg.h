//------------------------------------------------------------------------------
// ModelInstanceMsg.h - Helper for data in renderer model instance messages
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

#ifndef GAEN_ENGINE_MESSAGE_HELPERS_MODELINSTANCEMSG_H
#define GAEN_ENGINE_MESSAGE_HELPERS_MODELINSTANCEMSG_H

#include "engine/Model.h"
#include "engine/message_helpers/BaseMsg.h"

namespace gaen
{

// Helper classes to encode/decode messages
// Block structure of message is as follows
// * Header (0-15)
//   instanceId in payload (12-15)
// * Block 1 (16-31)
//   model pointer (0-7)
//   isAssetManaged bool (8-11)
//   EMPTY (12-15)
// * Block 2 (32-47)
//   Floats 0-3 of Transform Mat34
// * Block 3 (48-63)
//   Floats 4-7 of Transform Mat34
// * Block 4 (64-95)
//   Floats 8-11 of Transform Mat34

class ModelInstanceMsgReader
{
public:
    ModelInstanceMsgReader(const MessageQueue::MessageAccessor & msgAcc)
      : mMsgAcc(msgAcc)
    {
        // Mat34 has 3 qcells (each qcell is 16 bytes, i.e. 4 floats)
        // We want to know if the matrix wraps the ring buffer.
        // - If it doesn't, we can simply return a pointer to the matrix inside the queue
        // - If it does wrap, we need to copy it into our mWorldTransform data member
        //   so that we can make it contiguous for the caller.
        const qcell & matVec4_0 = msgAcc[1].q;
        const qcell & matVec4_2 = msgAcc[3].q;
        if (&matVec4_2 > &matVec4_0)
            mpWorldTransform = reinterpret_cast<const Mat34*>(&matVec4_0); // matrix is contiguous in ring buffer
        else
        {
            // matrix wraps ring buffer
            const qcell & matVec4_1 = msgAcc[2].q;
            mWorldTransform = Mat34::build_from_vec4(matVec4_0.vec4,
                                              matVec4_1.vec4,
                                              matVec4_2.vec4);
            mpWorldTransform = &mWorldTransform;
        }
    }
    model_instance_id instanceId() const { return mMsgAcc.message().payload.u; }
    Model * model() const { return static_cast<Model*>(mMsgAcc[0].d[0].p); }
    bool isAssetManaged() const { return mMsgAcc[0].c[2].b; }
    const Mat34 & worldTransform() const { return * mpWorldTransform; }
        
private:
    const MessageQueue::MessageAccessor & mMsgAcc;
    Mat34 mWorldTransform;    // place copy copy matrix if it wraps the ring buffer
    const Mat34 * mpWorldTransform; // pointer to the transform we'll return
};



class ModelInstanceMsgWriter : protected BaseMsgWriter
{
public:
    ModelInstanceMsgWriter(fnv msgId,
                           u32 flags,
                           task_id source,
                           task_id target)
      : BaseMsgWriter(msgId,
                      flags,
                      source,
                      target,
                      4) {}
    
    void setInstanceId(model_instance_id instanceId) { mMsgAcc.message().payload.u = instanceId; }
    void setModel(Model * pModel) { mMsgAcc[0].d[0].p = pModel; }
    void setIsAssetManaged(bool val) { mMsgAcc[0].c[2].b = val; }
    void setWorldTransform(Mat34 & worldTransform)
    {
        mMsgAcc[1].q.vec4 = worldTransform.vec4(0);
        mMsgAcc[2].q.vec4 = worldTransform.vec4(1);
        mMsgAcc[3].q.vec4 = worldTransform.vec4(2);
    }
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGE_HELPERS_MODELINSTANCE_H
