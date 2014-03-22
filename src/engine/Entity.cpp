//------------------------------------------------------------------------------
// Entity.cpp - A game entity which contains a collection of Components
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

#include "engine/stdafx.h"

#include "core/mem.h"
#include "core/logging.h"
#include "engine/Entity.h"

namespace gaen
{

Entity::Entity(u32 propertyBufferSize,
               u8 * pPropertyBuffer,
               const Mat34 & transform)
  : mPropertyBufferSize(propertyBufferSize)
  , mPropertyBufferHWM(0)
{
    if (!pPropertyBuffer)
        pPropertyBuffer = static_cast<u8*>(GALLOC(mPropertyBufferSize, kMEM_Engine));
    
    mLocalTransform = transform;
    mGlobalTransform = transform;
}

void Entity::update(f32 deltaSecs)
{
    // Update all components
    for (Task & task : mComponents)
        task.update(deltaSecs);

    // Update all children
    for (Entity * pChild : mChildren)
        pChild->update(deltaSecs);
}

MessageResult Entity::message(const MessageQueue::MessageAccessor& msgAcc)
{
    switch (msgAcc.message().msgId)
    {
    case FNV::fin:
        break;
    }


    MessageResult res;

    // Send the message to all components
    for (Task & task : mComponents)
    {
        res = task.message(msgAcc);
        if (res == MessageResult::Consumed)
            return MessageResult::Consumed;
    }

    // Send the message to all children
    for (Entity * pChild : mChildren)
    {
        res = pChild->message(msgAcc);
        if (res == MessageResult::Consumed)
            return MessageResult::Consumed;
    }

    LOG_WARNING("Unhandled message to entity - taskid: %d, msgId: %d", taskId(), msgAcc.message().msgId);
    return MessageResult::Propogate;
}

} // namespace gaen
