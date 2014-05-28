//------------------------------------------------------------------------------
// RemoveModelInstanceMessage.h - Autogenerated message class for RemoveModelInstance
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

#ifndef GAEN_ENGINE_MESSAGES_REMOVEMODELINSTANCEMESSAGE_H
#define GAEN_ENGINE_MESSAGES_REMOVEMODELINSTANCEMESSAGE_H

#include "engine/MessageWriter.h"
#include "core/threading.h"
#include "engine/Model.h"

namespace gaen
{
namespace msg
{

class RemoveModelInstanceR
{
public:
    RemoveModelInstanceR(const MessageQueue::MessageAccessor & msgAcc)
      : mMsgAcc(msgAcc)
    {

    }

    model_instance_id instanceId() const { return mMsgAcc.message().payload.u; }
        
private:
    const MessageQueue::MessageAccessor & mMsgAcc;


};



class RemoveModelInstanceW : protected MessageWriter
{
public:
    RemoveModelInstanceW(u32 msgId,
                            u32 flags,
                            task_id source,
                            task_id target,
                              model_instance_id instanceId)
      : MessageWriter(msgId,
                      flags,
                      source,
                      target,
                      to_cell(instanceId),
                      0) {}
    
    void setInstanceId(model_instance_id val) { mMsgAcc.message().payload.u = val; }
};

} // namespcae msg
} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGES_REMOVEMODELINSTANCEMESSAGE_H

