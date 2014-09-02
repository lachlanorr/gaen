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

#include "engine/messages/InsertComponent.h"

#include "engine/Entity.h"

namespace gaen
{

Entity::Entity(u32 nameHash, u32 childrenMax, u32 componentsMax, u32 blocksMax)
{
    mLocalTransform = Mat34::identity();
    mGlobalTransform = Mat34::identity();

    mChildrenMax = childrenMax;
    mChildCount = 0;
    if (mChildrenMax > 0)
        mpChildren = (Task*)GALLOC(kMEM_Engine, sizeof(Task)* mChildrenMax);
    else
        mpChildren = nullptr;

    mComponentsMax = componentsMax;
    mComponentCount = 0;
    if (mComponentsMax > 0)
        mpComponents = (Component*)GALLOC(kMEM_Engine, sizeof(Component) * mComponentsMax);
    else
        mpComponents = nullptr;

    // Most entities that aren't undergoing active design should have
    // a fixed amount of blocks.
    mBlocksMax = blocksMax;
    mBlockCount = 0;
    if (mBlocksMax > 0)
        mpBlocks = (Block*)GALLOC(kMEM_Engine, sizeof(Block) * mBlocksMax);
    else
        mpBlocks = nullptr;
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
    
    // send update our components
    for (u32 i = 0; i < mComponentCount; ++i)
        mpComponents[i].task().update(deltaSecs);

    // send update to our child entities
    for (u32 i = 0; i < mChildCount; ++i)
        mpChildren[i].update(deltaSecs);
}

template <typename T>
MessageResult Entity::message(const T & msgAcc)
{
    switch (msgAcc.message().msgId)
    {
    case HASH::fin:
    {
        // fin messages are like destructors and should be handled specially.
        // fin method will propogate fin to all tasks/entity children
        // and delete this entity.

        // Send fin message to all children entities
        for (u32 i = 0; i < mChildCount; ++i)
        {
            mpChildren[i].message(msgAcc);
        }

        // Now, send fin to our components
        for (u32 i = 0; i < mComponentCount; ++i)
        {
            mpComponents[i].task().message(msgAcc);
        }

        // Call our subclassed message routine
        mTask.message(msgAcc);

        // And finally, delete ourselves
        GDELETE(this);

        return MessageResult::Propogate;
    }
    case HASH::init:
    {
        // We consume this message. New init messages will be generated and
        // passed to Components when they are added to us.

        // Call our subclassed message routine
        mTask.message(msgAcc);

        return MessageResult::Consumed;
    }
    case HASH::insert_component:
    {
        msg::InsertComponentR<T> msgr(msgAcc);
        u32 index = msgr.index() == (u32)-1 ? mComponentCount : msgr.index();
        insertComponent(msgr.nameHash(), index);
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
    res = mTask.message(msgAcc);
    if (res == MessageResult::Consumed)
        return MessageResult::Consumed;
    

    // Send the message to all components
    for (u32 i = 0; i < mComponentCount; ++i)
    {
        res = mpComponents[i].task().message(msgAcc);
        if (res == MessageResult::Consumed)
            return MessageResult::Consumed;
    }

    return MessageResult::Propogate;
}

void Entity::growComponents()
{
    u32 newMax = mpComponents ? mComponentsMax * 2 : 4;
    Component * pNewComponents = (Component*)GALLOC(kMEM_Engine, sizeof(Component) * newMax);

    if (mpComponents)
    {
        for (u32 i = 0; i < mComponentCount; ++i)
            pNewComponents[i] = mpComponents[i];
        
        GFREE(mpComponents);
    }

    mComponentsMax = newMax;
    mpComponents = pNewComponents;
}

void Entity::growBlocks(u32 minSizeIncrease)
{
    // double max enough times to store
    // the new blocks
    u32 newMax = mpBlocks ? mBlocksMax * 2 : 4;
    while (newMax < mBlockCount + minSizeIncrease)
        newMax *= 2;

    Block * pNewBlocks = (Block*)GALLOC(kMEM_Engine, sizeof(Block) * newMax);

    if (mpBlocks)
    {
        // copy old blocks into new
        for (u32 i = 0; i < mBlockCount; ++i)
            pNewBlocks[i] = mpBlocks[i];

        // update components to use new blocks storage
        for (u32 i = 0; i < mComponentCount; ++i)
            mpComponents[i].mpBlocks = pNewBlocks + (mpComponents[i].mpBlocks - mpBlocks);

        GFREE(mpBlocks);
    }

    mBlocksMax = newMax;
    mpBlocks = pNewBlocks;
}

Task& Entity::insertComponent(u32 nameHash, u32 index)
{
    ASSERT(mComponentCount <= mComponentsMax);
    ASSERT(index <= mComponentCount);

    if (index > mComponentCount)
        index = mComponentCount;
    
    // Resize buffer if necessary
    if (mComponentCount == mComponentsMax)
        growComponents();

    ASSERT(mComponentCount < mComponentsMax);

    if (index != mComponentCount)
    {
        // shift all components the right
        for (u32 i = mComponentCount-1; i > index; --i)
            mpComponents[i] = mpComponents[i-1];
    }

    Component * pLoc = &mpComponents[index];
    Component * pComp = ComponentRegistry::construct(nameHash, pLoc);

    ASSERT(pComp);
    ASSERT(pComp == pLoc);

    pComp->mpEntity = this;

    // Check if we have enough blocks for this new component
    if (mBlockCount + pComp->mBlockCount > mBlocksMax)
        growBlocks(pComp->mBlockCount);

    ASSERT(mBlockCount + pComp->mBlockCount <= mBlocksMax);

    pComp->mpBlocks = &mpBlocks[mBlockCount];
    mBlockCount += pComp->mBlockCount;

    mComponentCount++;

    StackMessageBlockWriter<0> initDataMsgw(HASH::init_data, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(pComp->task().id()));
    pComp->task().message(initDataMsgw.accessor());

    return pComp->task();
}


u32 Entity::findComponent(u32 nameHash)
{
    for (u32 i = 0; i < mComponentCount; ++i)
    {
        if (mpComponents[i].mTask.nameHash() == nameHash)
        {
            return i;
        }
    }
    return mComponentCount;
}


void Entity::moveComponentUp(u32 nameHash)
{
    u32 i = findComponent(nameHash);
    if (i != mComponentCount && i > 0)
    {
        Component temp = mpComponents[i];
        mpComponents[i] = mpComponents[i-1];
        mpComponents[i-1] = temp;
    }
}

void Entity::moveComponentDown(u32 nameHash)
{
    u32 i = findComponent(nameHash);
    if (i != mComponentCount && i < mComponentCount-1)
    {
        Component temp = mpComponents[i];
        mpComponents[i] = mpComponents[i+1];
        mpComponents[i+1] = temp;
    }
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
    u32 i = findComponent(nameHash);
    if (i != mComponentCount)
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
    }
}

void Entity::growChildren()
{
    u32 newMax = mpChildren ? mChildrenMax * 2 : 4;
    
    Task * pNewChildren = (Task*)GALLOC(kMEM_Engine, sizeof(Task) * newMax);

    if (mpChildren)
    {
        for (u32 i = 0; i < mChildCount; ++i)
            pNewChildren[i] = mpChildren[i];
        
        GFREE(mpChildren);
    }

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

// Template decls so we can define message func here in the .cpp
template MessageResult Entity::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult Entity::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen
