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
#include "engine/Registry.h"
#include "engine/Entity.h"

namespace gaen
{

static const u32 kInitialComponentsMax = 4;
static const u32 kInitialChildrenMax = 4;
static const u32 kInitialBlockGrowth = 4;

Entity::Entity(u32 nameHash, u32 blockCount)
{
    mTask = Task::create(this, nameHash);

    mLocalTransform = Mat34::identity();
    mGlobalTransform = Mat34::identity();

    mComponentsMax = kInitialComponentsMax;
    mComponentCount = 0;
    mpComponents = (Component*)GALLOC(kMEM_Engine, sizeof(Component) * mComponentsMax);

    mBlocksMax = blockCount + kInitialBlockGrowth;
    mBlockCount = blockCount;
    mpBlocks = (Block*)GALLOC(kMEM_Engine, sizeof(Block) * mBlocksMax);

    mChildrenMax = kInitialChildrenMax;
    mChildCount = 0;
    mpChildren = (Task*)GALLOC(kMEM_Engine, sizeof(Task) * mChildrenMax);
}

Entity::~Entity()
{
    GFREE(mpChildren);
    GFREE(mpBlocks);
    GFREE(mpComponents);
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

void Entity::growComponents()
{
    u32 newMax = mComponentsMax * 2;
    Component * pNewComponents = (Component*)GALLOC(kMEM_Engine, sizeof(Component) * newMax);

    for (u32 i = 0; i < mComponentCount; ++i)
        pNewComponents[i] = mpComponents[i];
        
    GFREE(mpComponents);

    mComponentsMax = newMax;
    mpComponents = pNewComponents;
}

void Entity::growBlocks(u32 minSizeIncrease)
{
    // double max enough times to store
    // the new blocks
    u32 newMax = mBlocksMax * 2;
    while (newMax < mBlockCount + minSizeIncrease)
        newMax *= 2;

    Block * pNewBlocks = (Block*)GALLOC(kMEM_Engine, sizeof(Block) * newMax);

    // copy old blocks into new
    for (u32 i = 0; i < mBlockCount; ++i)
        pNewBlocks[i] = mpBlocks[i];

    // update components to use new blocks storage
    for (u32 i = 0; i < mComponentCount; ++i)
        mpComponents[i].mpBlocks = pNewBlocks + (mpComponents[i].mpBlocks - mpBlocks);

    GFREE(mpBlocks);

    mBlocksMax = newMax;
    mpBlocks = pNewBlocks;
}

void Entity::insertComponent(u32 nameHash, ComponentPosition pos)
{
    ASSERT(mComponentCount <= mComponentsMax);
    
    // Resize buffer if necessary
    if (mComponentCount == mComponentsMax)
        growComponents();

    ASSERT(mComponentCount < mComponentsMax);

    Component * pLoc = &mpComponents[mComponentCount];

    if (pos == kCPOS_Begin)
    {
        // shift all components the right
        for (u32 i = mComponentCount-1; i > 0; --i)
            mpComponents[i] = mpComponents[i-1];
        pLoc = &mpComponents[0];
    }

    Component * pComp = ComponentRegistry::construct(nameHash, pLoc);

    ASSERT(pComp);

    // Check if we have enough blocks for this new component
    if (mBlockCount + pComp->mBlockCount > mBlocksMax)
        growBlocks(pComp->mBlockCount);

    ASSERT(mBlockCount + pComp->mBlockCount <= mBlocksMax);

    pComp->mpBlocks = &mpBlocks[mBlockCount];
    mBlockCount += pComp->mBlockCount;

    mComponentCount++;
}

void Entity::removeBlocks(Block * pStart, u32 count)
{
    Block * pEnd = pStart + count;

    ASSERT(pStart > mpBlocks);
    ASSERT(pEnd <= mpBlocks + mBlockCount);

    if (pEnd < mpBlocks + mBlockCount)
    {
        // Removing blocks in the middle, we'll shift to the left to
        // close the gap, but in doing so must also update all mpBlock
        // pointers in the components that were pointing the moved
        // blocks.
        for (u32 i = 0; i < count; ++i)
        {
            pStart[i] = pEnd[i];
        }

        // Update any component block pointers that point to blocks
        // after removed section.
        for (u32 i = 0; i < mComponentCount; ++i)
        {
            if (mpComponents[i].mpBlocks >= pEnd)
            {
                mpComponents[i].mpBlocks -= count;
            }
        }
    }
    mBlockCount -= count;
}

void Entity::removeComponent(u32 nameHash)
{
    for (u32 i = 0; i < mComponentCount; ++i)
    {
        if (mpComponents[i].mTask.nameHash() == nameHash)
        {
            removeBlocks(mpComponents[i].mpBlocks, mpComponents[i].mBlockCount);
            if (i < mComponentCount - 1)
            {
                // removing in middle, need to shift subsequent components one to left
                for (u32 j = i; j < mComponentCount; ++j)
                {
                    mpComponents[j] = mpComponents[j+1];
                }
            }
            mComponentCount--;
            return;
        }
    }
}

void Entity::growChildren()
{
    u32 newMax = mChildrenMax * 2;
    Task * pNewChildren = (Task*)GALLOC(kMEM_Engine, sizeof(Task) * newMax);

    for (u32 i = 0; i < mChildCount; ++i)
        pNewChildren[i] = mpChildren[i];
        
    GFREE(mpChildren);

    mChildrenMax = newMax;
    mpChildren = pNewChildren;
}

void Entity::insertChild(Task & task)
{
    ASSERT(mChildCount <= mChildrenMax);

    // Resize buffer if necessary
    if (mChildCount == mChildrenMax)
    {
        growChildren();
    }

    ASSERT(mChildCount < mChildrenMax);

    mpChildren[mChildCount] = task;
    mChildCount++;
}

void Entity::removeChild(Task & task)
{
    if (mChildCount == 1)
    {
        ASSERT_MSG(mpChildren[0].id() == task.id(), "Attempt to remove task that Entity does not have");
        if (mpChildren[0].id() == task.id())
        {
            mChildCount = 0;
            return;
        }
    }
    else
    {
        for (u32 i = 0; i < mChildCount; ++i)
        {
            if (mpChildren[i].id() == task.id())
            {
                if (i < mChildCount - 1)
                {
                    // item in middle, swap with the last item
                    mpChildren[i] = mpChildren[mChildCount-1];
                }
                mChildCount--;
                return;
            }
        }
    }
    PANIC("Attempt to remove task that Entity does not have");
}




// LORRTODO - do we need these, commenting out for now
// Template instantiations
//template MessageResult Entity::message<const MessageQueue::MessageAccessor&>(const Message&, const MessageQueue::MessageAccessor&);
//template MessageResult Entity::message<const Block*>(const Message&, const Block*);

} // namespace gaen
