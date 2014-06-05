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
#include "engine/hashes.h"
#include "engine/Entity.h"

namespace gaen
{

Entity::Entity(u32 nameHash, u32 blockCount)
{
    mTask = Task::create(this, nameHash);

    mLocalTransform = Mat34::identity();
    mGlobalTransform = Mat34::identity();

    mComponentsMax = 16;
    mComponentCount = 0;
    mpComponents = (Component*)GALLOC(kMEM_Engine, sizeof(Component) * mComponentsMax);

    mBlocksMax = blockCount + 4; // LORRTODO: 4 is aribtrary, we need to decide a better resizing way
    mBlockCount = blockCount;
    mpBlocks = (Block*)GALLOC(kMEM_Engine, sizeof(Block) * mBlocksMax);

    mChildrenMax = 16;
    mChildCount = 0;
    mpChildren = (Task*)GALLOC(kMEM_Engine, sizeof(Task) * mChildrenMax);
}

Entity::~Entity()
{
    GFREE(mpChildren);
    GFREE(mpBlocks);
    GFREE(mpComponents);
}

void Entity::insertComponent(u32 nameHash, ComponentPosition pos)
{
    // LORRTODO
    //nASSERT(!pComp->mpEntity);
    //pComp->mpEntity = this;
    //mComponents.push_back(Task::create(pComp));
}

void Entity::update(f32 deltaSecs)
{
    mTask.update(deltaSecs);
    
    // Now, send fin to our components
    for (u32 i = 0; i < mComponentCount; ++i)
    {
        mpComponents[i].task().update(deltaSecs);
    }
}

template <typename T>
MessageResult Entity::message(const Message & msg, T msgAcc)
{
    switch (msg.msgId)
    {
    case HASH::fin:
    {
        // fin messages are like destructors and should be handled specially.
        // fin method will propogate fin to all tasks/entity children
        // and delete this entity.

        // Send fin message to all children entities
        for (u32 i = 0; i < mChildCount; ++i)
        {
            mpChildren[i].message(msg, msgAcc);
        }

        // Now, send fin to our components
        for (u32 i = 0; i < mComponentCount; ++i)
        {
            mpComponents[i].task().message(msg, msgAcc);
        }

        // Call our subclassed message routine
        mTask.message(msg, msgAcc);

        // And finally, delete ourselves
        GDELETE(this);

        return MessageResult::Propogate;
    }
    case HASH::init:
    {
        // We consume this message. New init messages will be generated and
        // passed to Components when they are added to us.

        // Call our subclassed message routine
        mTask.message(msg, msgAcc);

        return MessageResult::Consumed;
    }
    case HASH::transform:
        break;
    case HASH::insert_component:
    {
        //insertComponent(msgAcc); // LORRTODO
        return MessageResult::Consumed;
    }
    case HASH::register_watcher:
    {
        // register a property watcher for some combination of:
        // - component type
        // - compenent id
        // - property id
        PANIC("TODO");
        break;
    }
    }

    MessageResult res;

    // Call our subclassed message routine
    res = mTask.message(msg, msgAcc);
    if (res == MessageResult::Consumed)
        return MessageResult::Consumed;
    

    // Send the message to all components
    for (u32 i = 0; i < mComponentCount; ++i)
    {
        res = mpComponents[i].task().message(msg, msgAcc);
        if (res == MessageResult::Consumed)
            return MessageResult::Consumed;
    }

    return MessageResult::Propogate;
}

// LORRTODO - do we need these, commenting out for now
// Template instantiations
//template MessageResult Entity::message<const MessageQueue::MessageAccessor&>(const Message&, const MessageQueue::MessageAccessor&);
//template MessageResult Entity::message<const Block*>(const Message&, const Block*);

} // namespace gaen
