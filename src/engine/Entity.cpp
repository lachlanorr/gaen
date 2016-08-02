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
#include "hashes/hashes.h"
#include "engine/Registry.h"
#include "engine/Asset.h"

#include "engine/messages/Handle.h"
#include "engine/messages/InsertComponent.h"
#include "engine/messages/InsertTask.h"
#include "engine/messages/TaskStatus.h"
#include "engine/messages/Transform.h"

#include "engine/Entity.h"

namespace gaen
{

Entity::Entity(u32 nameHash,
               u32 childrenMax,
               u32 componentsMax,
               u32 blocksMax)
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

    mTask = Task::create_updatable(this, nameHash);

    mIsFinSelfSent = false;

    // LORRTEMP
    LOG_INFO("Entity created: id: %u, name: %s", mTask.id(), HASH::reverse_hash(mTask.nameHash()));
}

Entity::~Entity()
{
    if (mpBlockMemory)
        GDELETE(mpBlockMemory);

    GFREE(mpChildren);
    GFREE(mpBlocks);
    GFREE(mpComponents);
}

void Entity::setEntityInit(Entity * pEntity, void * pCreator, EntityInitCallback initCB, EntityInitCallback initDataCB)
{
    mEntityInit = EntityInit(pEntity, pCreator, initCB, initDataCB);
}

void Entity::activate()
{
    ASSERT(mInitStatus == kIS_Uninitialized);
    ASSERT(mTask.status() == TaskStatus::Initializing);

    // Insert Entity into the TaskMasters
    messages::InsertTaskBW msgInsertTask(HASH::insert_task,
                                         kMessageFlag_None,
                                         mTask.id(),
                                         active_thread_id(),
                                         active_thread_id());
    msgInsertTask.setTask(mTask);
    broadcast_message(msgInsertTask.accessor());

    // Start initialization sequence with #init__
    // Entity will progress through initialization stages on its
    // own, eventually reaching the "activated" state. Then it will
    // be updated each frame by the TaskMasters.
    MessageQueueWriter msgInit(HASH::init__, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0), 0);
}

void Entity::finSelf()
{
    if (!mIsFinSelfSent)
    {
        MessageQueueWriter msgw(HASH::fin, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0), 0);
    }
    mIsFinSelfSent = true;
}

Entity * Entity::activate_start_entity(u32 entityHash)
{
    Entity * pEntity = get_registry().constructEntity(entityHash, 32);

    if (pEntity)
    {
        pEntity->activate();
    }
    else
    {
        PANIC("Unable to construct start entity, hash 0x%08x", entityHash);
    }

    return pEntity;
}

void Entity::update(f32 deltaSecs)
{
    ASSERT(mInitStatus == kIS_Activated);

    mScriptTask.update(deltaSecs);

    // send update our components
    for (u32 i = 0; i < mComponentCount; ++i)
        mpComponents[i].scriptTask().update(deltaSecs);

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
            Task & t = mpComponents[i].scriptTask();
            StackMessageBlockWriter<0> msgw(HASH::update_transform,
                kMessageFlag_ForcePropagate,
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
                kMessageFlag_ForcePropagate,
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
            mpComponents[i].scriptTask().message(msgAcc);
        }

        // Call our sub-classed message routine
        mScriptTask.message(msgAcc);


        StackMessageBlockWriter<0> finMsgW(HASH::fin__, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0));

        // Send fin__ message to all children entities
        for (u32 i = 0; i < mChildCount; ++i)
        {
            mpChildren[i]->message(finMsgW.accessor());
        }

        // Now, send fin__ to our components
        for (u32 i = 0; i < mComponentCount; ++i)
        {
            mpComponents[i].scriptTask().message(finMsgW.accessor());
        }

        // Call our sub-classed message routine
        mScriptTask.message(finMsgW.accessor());

        // Remove us from TaskMasters
        broadcast_message(HASH::remove_task, kMessageFlag_None, mTask.id(), to_cell(mTask.id()));

        return MessageResult::Consumed;
    }
    else if (msgId == HASH::fin__)
    {
        // Our owning task master will send us this message immediately after
        // we're removed from the task list.

        // And finally, delete ourselves
        GDELETE(this);
    }

    // Always pass set_property through to our mScriptTask
    // since they can come in very early in initialization
    // and we don't have any reason to prevent them from
    // going through at any stage of an Entity's life.
    if (msgId == HASH::set_property)
    {
        mScriptTask.message(msgAcc);
        return MessageResult::Consumed;
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
        }

        MessageResult res;

        // Call our subclassed message routine
        res = mScriptTask.message(msgAcc);
        if (res == MessageResult::Consumed && !msgAcc.message().ForcePropagate())
            return MessageResult::Consumed;
    
        // Send the message to all components
        for (u32 i = 0; i < mComponentCount; ++i)
        {
            res = mpComponents[i].scriptTask().message(msgAcc);
            if (res == MessageResult::Consumed && !msgAcc.message().ForcePropagate())
                return MessageResult::Consumed;
        }

        return MessageResult::Propagate;

    }
    else
    {
        // Entity initialization has a very strict ordering,
        // and specific messages must arrive in the correct
        // sequence.
        switch (mInitStatus)
        {
        case kIS_Uninitialized:
            if (msgId == HASH::init__)
            {
                // We don't need to send into components since they haven't been
                // created yet. Passing into our mScriptTask will create components
                // and pass init__ to each component as it is added to the
                // entity.

                // Send to our sub-classed message routine
                mScriptTask.message(msgAcc);

                // Call the scripted init callback to set any overridden values
                mEntityInit.init();

                mInitStatus = kIS_Init;
                // LORRTEMP
                LOG_INFO("Entity change state: message: %s, taskid: %u, name: %s, newstate: %d", HASH::reverse_hash(msgId), mTask.id(), HASH::reverse_hash(mTask.nameHash()), mInitStatus);
        
                // Send ourself #request_assets__
                StackMessageBlockWriter<0> msg(HASH::request_assets__, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0));
                mTask.message(msg.accessor());

                return MessageResult::Consumed;
            }
            break;
        case kIS_Init:
            if (msgId == HASH::request_assets__)
            {
                // Send to our components
                for (u32 i = 0; i < mComponentCount; ++i)
                {
                    mpComponents[i].scriptTask().message(msgAcc);
                }

                // Send to our sub-classed message routine
                mScriptTask.message(msgAcc);

                if (areAllAssetsLoaded())
                    finalizeAssetInit();
                else
                {
                    mInitStatus = kIS_RequestAssets;
                    // LORRTEMP
                    LOG_INFO("Entity change state: message: %s, taskid: %u, name: %s, newstate: %d", HASH::reverse_hash(msgId), mTask.id(), HASH::reverse_hash(mTask.nameHash()), mInitStatus);
                }

                // HandleMgr will send us asset_ready__ messages as assets
                // are loaded.
            
                return MessageResult::Consumed;
            }
            break;
        case kIS_RequestAssets:
            if (msgId == HASH::asset_ready__)
            {
                ASSERT(!areAllAssetsLoaded());

                messages::HandleR<T> msgr(msgAcc);

                task_id subTask = msgr.taskId();
                u32 nameHash = msgr.nameHash();
                const Handle * pHandle = msgr.handle();
                const Asset * pAsset = reinterpret_cast<const Asset*>(pHandle->data());


                if (!pAsset->hadError())
                {
                    bool taskMatch = false;
                    // send to script task if task id matches
                    if (subTask == mScriptTask.id())
                    {
                        mScriptTask.message(msgAcc);
                        taskMatch = true;
                    }

                    // send to a component if task id matches
                    if (!taskMatch)
                    {
                        for (u32 i = 0; i < mComponentCount; ++i)
                        {
                            if (subTask == mpComponents[i].scriptTask().id())
                            {
                                mpComponents[i].scriptTask().message(msgAcc);
                                taskMatch = true;
                                break;
                            }
                        }
                    }

                    ERR_IF(!taskMatch, "Asset sent to entity for non-matching subTask");

                    mAssetsLoaded++;

                    // Send ourself #init if all assets have been loaded
                    if (areAllAssetsLoaded())
                    {
                        finalizeAssetInit();
                    }
                    return MessageResult::Consumed;
                }

                // If we get to this line, asset load was a failure, send ourselves #fin
                ERR("Killing Entity %u since it failed to load asset: %s", mTask.id(), pAsset->path().c_str());
                finSelf();
                return MessageResult::Consumed;
            }
            break;
        case kIS_AssetsReady:
            if (msgId == HASH::init)
            {
                // Send to our components
                for (u32 i = 0; i < mComponentCount; ++i)
                {
                    mpComponents[i].scriptTask().message(msgAcc);
                }

                // Send to our sub-classed message routine
                mScriptTask.message(msgAcc);

                mInitStatus = kIS_Activated;
                // LORRTEMP
                LOG_INFO("Entity change state: message: %s, taskid: %u, name: %s, newstate: %d", HASH::reverse_hash(msgId), mTask.id(), HASH::reverse_hash(mTask.nameHash()), mInitStatus);

                // Set us running
                // LORRTODO: Add mSetRunningOnInit flag and respect it.a
                //if (mSetRunningOnInit)
                {
                    messages::TaskStatusQW msgW(HASH::set_task_status, kMessageFlag_Editor, mTask.id(), mTask.id(), TaskStatus::Running);
                }

                return MessageResult::Consumed;
            }
            break;
        }
    }

    if (mInitStatus != kIS_Activated)
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
    }
    return MessageResult::Propagate;
}


void Entity::setTransform(const glm::mat4x3 & mat)
{
    mIsTransformDirty = true;
    mTransform = mat;

    LOG_INFO("transform: %f, %f, %f", mat[3][0], mat[3][1], mat[3][2]);

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

void Entity::finalizeAssetInit()
{
    mInitStatus = kIS_AssetsReady;

    // LORRTEMP
    LOG_INFO("Entity change state: taskid: %u, name: %s, newstate: %d", mTask.id(), HASH::reverse_hash(mTask.nameHash()), mInitStatus);

    {
    StackMessageBlockWriter<0> msg(HASH::init_data__, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0));
    // init all fields and properties in each component
    mScriptTask.message(msg.accessor());

    }

    // Call the scripted init_data callback to set any overridden values
    mEntityInit.initData();

    {
    StackMessageBlockWriter<0> msg(HASH::init, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0));
    mTask.message(msg.accessor());
    }
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

    // HASH::init__ will be sent to component in codegen'd .cpp for component/entity

    // LORRTEMP
    LOG_INFO("Component inserted: entityId: %u, entityName: %s, taskId: %u, taskName: %s", mTask.id(), HASH::reverse_hash(mTask.nameHash()), pComp->scriptTask().id(), HASH::reverse_hash(pComp->scriptTask().nameHash()));

    // Activate our component's task so it can perform updates
    pComp->scriptTask().setStatus(TaskStatus::Running);

    return pComp->scriptTask();
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

void Entity::requestAsset(u32 subTaskId, u32 nameHash, const CmpString & path)
{
    mAssetsRequested++;

    MessageQueueWriter msgw(HASH::request_asset__, kMessageFlag_None, mTask.id(), kAssetMgrTaskId, to_cell(subTaskId), path.blockCount() + 1);
    msgw[0].cells[0].u = nameHash;
    path.writeMessage(msgw.accessor(), 1);
}

// Template decls so we can define message func here in the .cpp
template MessageResult Entity::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult Entity::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen
