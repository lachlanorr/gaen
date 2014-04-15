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
  , mpPropertyBuffer(pPropertyBuffer)
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

template <typename T>
MessageResult Entity::message(const Message & msg, T msgAcc)
{
    switch (msg.msgId)
    {
    case FNV::fin:
        // fin messages are like destructors and should be handled specially.
        // fin method will propogate fin to all tasks/entity children
        // and delete this entity.
        fin(msg, msgAcc);
        return MessageResult::Propogate;
    case FNV::init:
        // We consume this message. New init messages will be generated and
        // passed to Components when they are added to us.
        init(msg, msgAcc);
        return MessageResult::Consumed;
    case FNV::insert_component:
        //insertComponent(msgAcc); // LORRTODO
        return MessageResult::Consumed;
    }


    MessageResult res;

    // Send the message to all components
    for (Task & task : mComponents)
    {
        res = task.message(msg, msgAcc);
        if (res == MessageResult::Consumed)
            return MessageResult::Consumed;
    }

    // Send the message to all children
    for (Entity * pChild : mChildren)
    {
        res = pChild->message(msg, msgAcc);
        if (res == MessageResult::Consumed)
            return MessageResult::Consumed;
    }

    LOG_WARNING("Unhandled message to entity - taskid: %d, msgId: %d", taskId(), msg.msgId);
    return MessageResult::Propogate;
}

template <typename T>
void Entity::fin(const Message & msg, T msgAcc)
{
    ASSERT(msg.msgId == FNV::fin);

    // Send the message to all components
    for (Task & task : mComponents)
    {
        task.message(msg, msgAcc);
    }

    // Send the message to all children
    for (Entity * pChild : mChildren)
    {
        pChild->message(msg, msgAcc);
    }

    GFREE(mpPropertyBuffer);
    GDELETE(this);
}

// Template instantiations
template MessageResult Entity::message<const MessageQueue::MessageAccessor&>(const Message&, const MessageQueue::MessageAccessor&);
template MessageResult Entity::message<const MessageBlock*>(const Message&, const MessageBlock*);

template void Entity::fin<const MessageQueue::MessageAccessor&>(const Message&, const MessageQueue::MessageAccessor&);
template void Entity::fin<const MessageBlock*>(const Message&, const MessageBlock*);


} // namespace gaen
