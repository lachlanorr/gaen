//------------------------------------------------------------------------------
// Component.h - Core functionality all components must have
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

#ifndef GAEN_ENGINE_COMPONENT_H
#define GAEN_ENGINE_COMPONENT_H

#include "engine/Entity.h"

namespace gaen
{

// Base component for common stuff for all Components.
class Component
{
public:
    task_id taskId() { ASSERT(mIsInit); return mTaskId; }
    task_id entityTaskId() { ASSERT(mIsInit); return mEntityTaskId; }

    MessageResult message(const MessageQueue::MessageAccessor& msgAcc)
    {
        MessageResult msgRes = MessageResult::Propogate;
        const Message & msg = msgAcc.message();
        switch (msg.msgId)
        {
        case FNV::init:
            mEntityTaskId = msg.source;
            mTaskId = msg.target;
            msgRes = MessageResult::Consumed;
        }
        return msgRes;
    }

protected:
    task_id mTaskId = kInvalidTaskId;
    task_id mEntityTaskId = kInvalidTaskId;
    bool mIsInit = false;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_COMPONENT_H
