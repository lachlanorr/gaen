//------------------------------------------------------------------------------
// Entity.cpp - A game entity which contains a collection of Components
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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
#include "assets/file_utils.h"
#include "engine/BlockMemory.h"
#include "engine/MessageWriter.h"
#include "engine/glm_ext.h"
#include "engine/hashes.h"
#include "engine/Registry.h"
#include "engine/Asset.h"

#include "engine/messages/InsertComponent.h"
#include "engine/messages/Transform.h"

#include "engine/Entity.h"

namespace gaen
{

Entity::Entity(u32 nameHash, u32 childrenMax, u32 componentsMax, u32 blocksMax)
  : mpParent(nullptr)
  , mpBlockMemory(nullptr)
{
    mTransform = glm::mat4x3(1.0f);
    mIsTransformDirty = false;

    mChildrenMax = childrenMax;
    mChildCount = 0;
    if (mChildrenMax > 0)
        mpChildren = (Entity**)GALLOC(kMEM_Engine, sizeof(Entity*) * mChildrenMax);
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

    mInitStatus = kIS_Uninitialized;

    mAssetsRequested = 0;
    mAssetsLoaded = 0;

    // Entity stage, we manage entities here that we've created but
    // haven't yet been added to engine.
    mEntityStageCount = 0;
    for (u32 i = 0; i < kMaxEntityStage; ++i)
    {
        mpEntityStage[i] = 0;
    }

    mTask = Task::create_updatable(this, nameHash);
}

Entity::~Entity()
{
    // Delete any staged entities that were never unstaged
    for (u32 i = 0; i < kMaxEntityStage; ++i)
    {
        if (mpEntityStage[i])
        {
            mpEntityStage[i] = 0;
        }
    }

    if (!mpBlockMemory)
        GDELETE(mpBlockMemory);

    GFREE(mpChildren);
    GFREE(mpBlocks);
    GFREE(mpComponents);
}

void Entity::setTransform(const glm::mat4x3 & mat)
{
    mIsTransformDirty = true;
    mTransform = mat;
}

void Entity::applyTransform(bool isLocal, const glm::mat4x3 & mat)
{
    if (isLocal)
    {
        glm::mat4x3 invTrans = inverse(mTransform);
        setTransform(invTrans * mat * mTransform);
    }
    else
    {
        if (!mpParent)
            setTransform(mat);
        else
        {
            glm::mat4x3 invParent = inverse(mpParent->transform());
            setTransform(mTransform * mat * invParent);
        }
    }
}

void Entity::setParent(Entity * pEntity)
{
    ASSERT(!mpParent);

    if (pEntity)
    {
        // Convert our global transform into local coords relative to parent
        mpParent = pEntity;
        glm::mat4x3 invParent = inverse(mpParent->transform());
        setTransform(mTransform * invParent);
    }
}

const glm::mat4x3 & Entity::parentTransform() const
{
    if (!mpParent)
    {
        static const glm::mat4x3 ident = glm::mat4x3(1.0);
        return ident;
    }
    else
    {
        return mpParent->transform();
    }
}


Entity * Entity::create_start_entity(u32 entityHash)
{
    Entity * pEntity = get_registry().constructEntity(entityHash, 32);

    PANIC_IF(pEntity == nullptr, "Unable to construct start entity, hash 0x%08x", entityHash);

    if (pEntity)
    {
        // Stage into ourselves... yes, like Inception.
        pEntity->stageEntity(pEntity);

    }

    return pEntity;
}


void Entity::stageEntity(Entity * pEntity)
{
    if (mEntityStageCount >= kMaxEntityStage)
    {
        PANIC("Unable to stage entity, stage is full, stage size: %u", kMaxEntityStage);
        return;
    }

    mpEntityStage[mEntityStageCount] = pEntity;
    ++mEntityStageCount;
}

Entity * Entity::findStagedEntity(task_id id)
{
    for (u32 i = 0; i < mEntityStageCount; ++i)
    {
        Entity * pEnt = mpEntityStage[i];
        ASSERT(pEnt);
        if (pEnt->task().id() == id)
        {
            return pEnt;
        }
    }
    return nullptr;
}

void Entity::activateEntity(task_id id)
{
    Entity * pEnt = findStagedEntity(id);
    if (pEnt)
    {
        ASSERT(pEnt->mInitStatus == kIS_Uninitialized);

        // Start initialization sequence with #init_data__
        // Entity will progress through initialization stages on its
        // on, eventually reaching the activated state. Then it can be
        // unstaged into the engine by calling unstageEntity below.
        StackMessageBlockWriter<0> msg(HASH::init_data__, kMessageFlag_None, mTask.id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msg.accessor());
    }
    else
    {
        PANIC("Unable to activate entity, not on stage: id = %u", id);
    }
}

void Entity::unstageEntity(task_id id)
{
    for (u32 i = 0; i < mEntityStageCount; ++i)
    {
        Entity * pEnt = mpEntityStage[i];
        ASSERT(pEnt);
        if (pEnt->task().id() == id)
        {
            ASSERT(pEnt->mInitStatus == kIS_Activated);

            // remove entity
            mpEntityStage[i] = nullptr;
            // if we're not the last, replace with the last
            if (i < (mEntityStageCount-1))
            {
                mpEntityStage[i] = mpEntityStage[mEntityStageCount-1];
                mpEntityStage[mEntityStageCount-1] = nullptr;
            }
            --mEntityStageCount;

            // Initialization is complete, announce our arrival
            // to the world, and start running in a TaskMaster.
            messages::InsertTaskBW msgw(HASH::insert_task,
                                        kMessageFlag_None,
                                        pEnt->mTask.id(),
                                        active_thread_id(),
                                        active_thread_id());
            msgw.setTask(pEnt->mTask);
            broadcast_message(msgw.accessor());

            return pEnt;
        }
    }
    PANIC("Unable to unstage entity, not on stage: id = %u", id);
    return nullptr;
}

BlockMemory & Entity::blockMemory()
{
    // We don't allocate until we need, as many entities will
    // never need BlockMemory.
    if (!mpBlockMemory)
        mpBlockMemory = GNEW(kMEM_Engine, BlockMemory);

    return *mpBlockMemory;
}

void Entity::collect()
{
    if (mpBlockMemory)
        mpBlockMemory->collect();
}

void Entity::update(f32 deltaSecs)
{
    mScriptTask.update(deltaSecs);

    // send update our components
    for (u32 i = 0; i < mComponentCount; ++i)
        mpComponents[i].task().update(deltaSecs);

    // send update to our child entities
    for (u32 i = 0; i < mChildCount; ++i)
        mpChildren[i]->update(deltaSecs);

    // Now, if there has been any change to the transform,
    // notify components and child entities.
    // This has to happen in a separate stage to the update
    // loops above since one of the updates may have altered
    // our transform.
    if (mIsTransformDirty)
    {
        // send update our components
        for (u32 i = 0; i < mComponentCount; ++i)
        {
            Task & t = mpComponents[i].task();
            StackMessageBlockWriter<0> msgw(HASH::update_transform,
                kMessageFlag_ForcePropogate,
                mTask.id(),
                t.id(),
                to_cell(0));
            t.message(msgw.accessor());
        }

        // send update to our child entities
        for (u32 i = 0; i < mChildCount; ++i)
        {
            Entity * pEnt = mpChildren[i];
            StackMessageBlockWriter<0> msgw(HASH::update_transform,
                kMessageFlag_ForcePropogate,
                mTask.id(),
                pEnt->task().id(),
                to_cell(0));
            pEnt->message(msgw.accessor());
        }

        mIsTransformDirty = false;
    }

    // Collect any unused BlockMemory
    collect();
}

template <typename T>
MessageResult Entity::message(const T & msgAcc)
{
    u32 msgId = msgAcc.message().msgId;

    // Prioritize fin message
    if (msgId == HASH::fin)
    {
        PANIC_IF(initStatus() != kIS_Fin, "fin message received on already finalized Entity");

        // fin messages are like destructors and should be handled specially.
        // fin method will propagate fin to all tasks/entity children
        // and delete this entity.

        // Send fin message to all children entities
        for (u32 i = 0; i < mChildCount; ++i)
        {
            mpChildren[i]->message(msgAcc);
        }

        // Now, send fin to our components
        for (u32 i = 0; i < mComponentCount; ++i)
        {
            mpComponents[i].task().message(msgAcc);
        }

        // Call our sub-classed message routine
        mScriptTask.message(msgAcc);

        // And finally, delete ourselves
        GDELETE(this);

        return MessageResult::Propogate;
    }


    // Interesting messages are handled here, initialization
    // messages are below
    if (mInitStatus == kIS_Activated)
    {
        switch (msgId)
        {
        case HASH::insert_component:
        {
            messages::InsertComponentR<T> msgr(msgAcc);
            u32 index = msgr.index() == (u32)-1 ? mComponentCount : msgr.index();
            insertComponent(msgr.nameHash(), index);
            return MessageResult::Consumed;
        }
        case HASH::register_watcher:
        {
            // register a property watcher for some combination of:
            // - component type
            // - property id
            PANIC("TODO");
            return MessageResult::Consumed;
        }
        case HASH::transform:
        {
            messages::TransformR<T> msgr(msgAcc);
            applyTransform(msgr.isLocal(), msgr.transform());
            return MessageResult::Consumed;
        }
        case HASH::unstage__:
        {
            unstageEntity(msgAcc.message().source);
            return MessageResult::Consumed;
        }
        }

        MessageResult res;

        // Call our subclassed message routine
        res = mScriptTask.message(msgAcc);
        if (res == MessageResult::Consumed && !msgAcc.message().ForcePropogate())
            return MessageResult::Consumed;
    
        // Send the message to all components
        for (u32 i = 0; i < mComponentCount; ++i)
        {
            res = mpComponents[i].task().message(msgAcc);
            if (res == MessageResult::Consumed && !msgAcc.message().ForcePropogate())
                return MessageResult::Consumed;
        }

        return MessageResult::Propogate;

    }
    else
    {
        // Entity initialization has a very strict ordering,
        // and specific messages must arrive in the correct
        // sequence.
        switch (mInitStatus)
        {
        case kIS_Uninitialized:
            if (msgId == HASH::init_data__)
            {
                // Send to our components
                for (u32 i = 0; i < mComponentCount; ++i)
                {
                    mpComponents[i].task().message(msgAcc);
                }

                // Send to our sub-classed message routine
                mScriptTask.message(msgAcc);

                mInitStatus = kIS_InitData;
        
                // Send ourself #init_assets__
                StackMessageBlockWriter<0> msg(HASH::init_assets__, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0));
                mTask.message(msg.accessor());

                return MessageResult::Consumed;
            }
            break;
        case kIS_InitData:
            if (msgId == HASH::init_assets__)
            {
                // Send to our components
                for (u32 i = 0; i < mComponentCount; ++i)
                {
                    mpComponents[i].task().message(msgAcc);
                }

                // Send to our sub-classed message routine
                mScriptTask.message(msgAcc);

                mInitStatus = kIS_InitAssets;

                // HandleMgr will send us asset_ready__ messages as assets
                // are loaded.
            
                return MessageResult::Consumed;
            }
            break;
        case kIS_InitAssets:
            if (msgId == HASH::asset_ready__)
            {
                ASSERT(mAssetsLoaded <= mAssetsRequested);

                // LORRTODO: if asset load was a failure, send ourselves #fin__

                mAssetsLoaded++;

                // Send ourself #init if all assets have been loaded
                if (mAssetsLoaded == mAssetsRequested)
                {
                    mInitStatus = kIS_AssetsReady;
                
                    StackMessageBlockWriter<0> msg(HASH::init, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0));
                    mTask.message(msg.accessor());
                }

                return MessageResult::Consumed;
            }
            break;
        case kIS_AssetsReady:
            if (msgId == HASH::init)
            {
                // Send to our components
                for (u32 i = 0; i < mComponentCount; ++i)
                {
                    mpComponents[i].task().message(msgAcc);
                }

                // Send to our sub-classed message routine
                mScriptTask.message(msgAcc);

                mInitStatus = kIS_Init;

                // HandleMgr will send us asset_ready__ messages as assets
                // are loaded.
            
                return MessageResult::Consumed;
            }
            break;
        case kIS_Init:
            if (msgId == HASH::activate__)
            {
                mInitStatus = kIS_Activated;

                // unstage us into engine
                if (mpParent)
                {
                    StackMessageBlockWriter<0> msg(HASH::unstage__, kMessageFlag_None, mTask.id(), mpParent->task().id(), to_cell(0));
                    mpParent->task().message(msg.accessor());
                }
                else
                {
                    // if no parent, we must be the start entity, thus we
                    // are staged in ourself.
                    StackMessageBlockWriter<0> msg(HASH::unstage__, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0));
                    mTask.message(msg.accessor());
                }
                
                return MessageResult::Consumed;
            }
            break;
        }
    }

    if (mInitStatus != kIS_Init)
    {
#if HAS(TRACK_HASHES)
        ERR("Message received in invalid state, task name: %s, message: %s, state: %d",
            HASH::reverse_hash(mTask.nameHash()),
            HASH::reverse_hash(msgAcc.message().msgId),
            mInitStatus);
#else
        ERR("Message received in invalid state, task nameHash: 0x%08x, message: 0x%08x, state: %d",
            mTask.nameHash(),
            msgAcc.message().msgId,
            mInitStatus);
#endif
        return MessageResult::Consumed;
    }
    
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
    Component * pComp = get_registry().constructComponent(nameHash, pLoc, this);

    ASSERT(pComp);
    ASSERT(pComp == pLoc);

    // Check if we have enough blocks for this new component
    if (mBlockCount + pComp->mBlockCount > mBlocksMax)
        growBlocks(pComp->mBlockCount);

    ASSERT(mBlockCount + pComp->mBlockCount <= mBlocksMax);

    pComp->mpBlocks = &mpBlocks[mBlockCount];
    mBlockCount += pComp->mBlockCount;

    mComponentCount++;

    // Send int_data message
    StackMessageBlockWriter<0> initDataMsgw(HASH::init_data__, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(pComp->task().id()));
    pComp->task().message(initDataMsgw.accessor());

    return pComp->task();
}


u32 Entity::findComponent(u32 nameHash)
{
    for (u32 i = 0; i < mComponentCount; ++i)
    {
        if (mpComponents[i].mScriptTask.nameHash() == nameHash)
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
    
    Entity ** pNewChildren = (Entity**)GALLOC(kMEM_Engine, sizeof(Entity**) * newMax);

    if (mpChildren)
    {
        for (u32 i = 0; i < mChildCount; ++i)
            pNewChildren[i] = mpChildren[i];
        
        GFREE(mpChildren);
    }

    mChildrenMax = newMax;
    mpChildren = pNewChildren;
}

void Entity::insertChild(Entity * pEntity)
{
    ASSERT(mChildCount <= mChildrenMax);
    ASSERT(!pEntity->parent());

    // Resize buffer if necessary
    if (mChildCount == mChildrenMax)
    {
        growChildren();
    }

    ASSERT(mChildCount < mChildrenMax);

    mpChildren[mChildCount] = pEntity;
    mChildCount++;

    pEntity->setParent(this);
}

void Entity::removeChild(Entity * pEntity)
{
    removeChild(pEntity->task().id());
}

void Entity::removeChild(task_id taskId)
{
    if (mChildCount == 1)
    {
        ASSERT_MSG(mpChildren[0]->task().id() == taskId, "Attempt to remove task that Entity does not have");
        if (mpChildren[0]->task().id() == taskId)
        {
            mChildCount = 0;
            return;
        }
    }
    else
    {
        for (u32 i = 0; i < mChildCount; ++i)
        {
            if (mpChildren[i]->task().id() == taskId)
            {
                mpChildren[i]->setParent(nullptr);
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

void Entity::requestAsset(u32 taskId, u32 name, const CmpString & path)
{
    MessageQueueWriter msgw(HASH::request_asset, kMessageFlag_None, mScriptTask.id(), kHandleMgrTaskId, to_cell(taskId), path.blockCount() + 1);
    msgw[0].cells[0].u = name;
    path.writeMessage(msgw.accessor(), 1);
}

// Template decls so we can define message func here in the .cpp
template MessageResult Entity::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult Entity::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen
