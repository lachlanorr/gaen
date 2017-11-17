//------------------------------------------------------------------------------
// Entity.cpp - A game entity which contains a collection of Components
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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
#include "hashes/hashes.h"
#include "engine/Registry.h"
#include "engine/Asset.h"

#include "engine/messages/BoundingBox.h"
#include "engine/messages/ComponentIndex.h"
#include "engine/messages/Handle.h"
#include "engine/messages/OwnerTask.h"
#include "engine/messages/PropertyMat43.h"
#include "engine/messages/PropertyMat3.h"
#include "engine/messages/PropertyVec3.h"
#include "engine/messages/TaskEntity.h"
#include "engine/messages/TaskStatus.h"
#include "engine/messages/Transform.h"
#include "engine/messages/UidTransform.h"

#include "engine/Entity.h"

namespace gaen
{

Entity::Entity(u32 nameHash,
               u32 childrenMax,
               u32 componentsMax,
               u32 blocksMax,
               task_id initParentTask,
               task_id creatorTask,
               u32 readyMessage)
  : mpParent(nullptr)
  , mpBlockMemory(nullptr)
  , mInitParentTask(initParentTask)
  , mCreatorTask(creatorTask)
  , mReadyMessage(readyMessage)
{
    mTransform = mat43(1.0f);
    mLocalTransform = mat43(1.0f);

    mPosMin = vec3(std::numeric_limits<f32>::lowest());
    mPosMax = vec3(std::numeric_limits<f32>::max());

    memset(mTransformListeners, 0, sizeof(mTransformListeners));

    mPlayer = 0;

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
    mIsDead = false;

    mpEntityInit = nullptr;

    memset(mReadyInfos, 0, sizeof(mReadyInfos));
    // Send our creator a ready_init
    if (mCreatorTask != 0 && mReadyMessage != 0)
        send_ready_init(task().id(), mCreatorTask, mReadyMessage);

    // LORRTEMP
    //LOG_INFO("Entity created: id: %u, name: %s", mTask.id(), HASH::reverse_hash(mTask.nameHash()));
}

Entity::~Entity()
{
    if (mpBlockMemory)
        GDELETE(mpBlockMemory);

    GFREE(mpChildren);
    GFREE(mpBlocks);
    GFREE(mpComponents);
}

void Entity::activate()
{
    ASSERT(mInitStatus == kIS_Uninitialized);
    ASSERT(mTask.status() == TaskStatus::Initializing);

    // Insert Entity into the TaskMasters
    broadcast_insert_task(mTask.id(), active_thread_id(), mTask);

    if (mInitParentTask != 0)
    {
        broadcast_request_set_parent(mTask.id(), mInitParentTask, this);
    }

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
    Entity * pEntity = get_registry().constructEntity(entityHash, 32, 0, 0, 0);

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

void Entity::update(f32 delta)
{
    ASSERT(mInitStatus == kIS_Activated);

    // Look for any ready notifications that should be processed
    for (u32 i = 0; i < kReadyInfoCount; ++i)
    {
        if (mReadyInfos[i].message != 0 && mReadyInfos[i].waitingCount == 0)
        {
            // All tasks we were waiting on have notified us, send us the ready message.
            u32 msg = mReadyInfos[i].message;
            mReadyInfos[i].clear();
            StackMessageBlockWriter<0> msgw(msg, kMessageFlag_None, task().id(), scriptTask().id(), to_cell(0));
            scriptTask().message(msgw.accessor());
        }
    }


    mScriptTask.update(delta);

    // send update our components
    for (u32 i = 0; i < mComponentCount; ++i)
        mpComponents[i].scriptTask().update(delta);

    // Collect any unused BlockMemory
    collect();
}

template <typename T>
MessageResult Entity::message(const T & msgAcc)
{
    u32 msgId = msgAcc.message().msgId;

    // Prioritize fin message
    if (msgId == HASH::fin__)
    {
        // Our owning task master will send us this message immediately after
        // we're removed from the task list.
        ASSERT(mIsDead);
        // And finally, delete ourselves
        GDELETE(this);
        return MessageResult::Consumed;
    }
    if (!mIsDead)
    {
        if (msgId == HASH::fin)
        {
            // If we are a child, only accept HASH::fin from our
            // parent, otherwise request our parent sends it to use.
            if (mpParent && mpParent->mTask.id() != msgAcc.message().source)
            {
                ASSERT(TaskMaster::task_master_for_active_thread().isOwnedTask(mpParent->mTask.id()));
                StackMessageBlockWriter<0> msgw(HASH::request_fin,
                                                kMessageFlag_None,
                                                mTask.id(),
                                                mpParent->task().id(),
                                                to_cell(0));
                mpParent->message(msgw.accessor());
                return MessageResult::Consumed;
            }

            mIsDead = true;

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

            // Now, send fin__ to our components
            for (u32 i = 0; i < mComponentCount; ++i)
            {
                mpComponents[i].scriptTask().message(finMsgW.accessor());
            }

            // Call our sub-classed message routine
            mScriptTask.message(finMsgW.accessor());

            // Remove us from TaskMasters
            broadcast_remove_task(mTask.id(), mTask.id());

            return MessageResult::Consumed;
        }
        if (msgId == HASH::request_fin)
        {
            // Verify this is one of our children
            for (u32 i = 0; i < mChildCount; ++i)
            {
                Entity * pEnt = mpChildren[i];

            if (msgAcc.message().payload.u == HASH::position)
            {
                messages::PropertyMat43R<T> msgr(msgAcc);
                setTransform(msgAcc.message().source, msgr.value());
                return MessageResult::Consumed;
            }
                if (pEnt->task().id() == msgAcc.message().source)
                {
                    ASSERT_MSG(TaskMaster::task_master_for_active_thread().isOwnedTask(pEnt->mTask.id()), "Parent/child entities must reside on same TaskMaster");

                    // Remove the child
                    removeChild(pEnt);

                    // Send the child fin from us, so it respects the message.
                    // Child entities will only conduct fin logic if message is
                    // sent from their parent.
                    StackMessageBlockWriter<0> msgw(HASH::fin,
                                                    kMessageFlag_None,
                                                    mTask.id(),
                                                    pEnt->task().id(),
                                                    to_cell(0));
                    pEnt->message(msgw.accessor());
                    return MessageResult::Consumed;
                }
            }
            PANIC("HASH::request_fin sent from a non-child entitty");
        }

        // Always pass set_property through to our mScriptTask
        // since they can come in very early in initialization
        // and we don't have any reason to prevent them from
        // going through at any stage of an Entity's life.
        else if (msgId == HASH::set_property)
        {
            // special case setting transform, since that is always
            // a virtual property of the Entity, not the ScriptTask.
            if (msgAcc.message().payload.u == HASH::transform)
            {
                messages::PropertyMat43R<T> msgr(msgAcc);
                setTransform(msgAcc.message().source, msgr.value());
                return MessageResult::Consumed;
            }
            else if (msgAcc.message().payload.u == HASH::position)
            {
                messages::PropertyVec3R<T> msgr(msgAcc);
                setPosition(msgAcc.message().source, msgr.value());
                return MessageResult::Consumed;
            }
            else if (msgAcc.message().payload.u == HASH::rotation)
            {
                messages::PropertyMat3R<T> msgr(msgAcc);
                setRotation(msgAcc.message().source, mat3(msgr.value()));
                return MessageResult::Consumed;
            }
            else
            {
                MessageResult res = mScriptTask.message(msgAcc);

                if (res == MessageResult::Propagate)
                {
                    // Property not consumed, send to components
                    for (u32 i = 0; i < mComponentCount; ++i)
                    {
                        res = mpComponents[i].scriptTask().message(msgAcc);
                        if (res == MessageResult::Consumed)
                            break;
                    }
                }
                return MessageResult::Consumed;
            }
        }
        else if (msgId == HASH::reparent)
        {
            requestSetParent(msgAcc.message().payload.u);
            return MessageResult::Consumed;
        }
        else if (msgId == HASH::register_transform_listener)
        {
            registerTransformListener(msgAcc.message().source, msgAcc.message().payload.u);
            return MessageResult::Consumed;
        }
        else if (msgId == HASH::update_transform)
        {
            updateTransform(msgAcc.message().source);
            return MessageResult::Consumed;
        }
        else if (msgId == HASH::constrain_position)
        {
            messages::BoundingBoxR<T> msgr(msgAcc);
            constrainPosition(msgr.min(), msgr.max());
            return MessageResult::Consumed;
        }
        // Interesting messages are handled here, initialization
        // messages are below
        if (mInitStatus == kIS_Activated)
        {
            switch (msgId)
            {
            case HASH::ready_init:
            {
                u32 readyMessage = msgAcc.message().payload.u;
                readyInit(readyMessage);
                return MessageResult::Consumed;
            }
            case HASH::ready_notify:
            {
                u32 readyMessage = msgAcc.message().payload.u;
                readyNotify(readyMessage);
                return MessageResult::Consumed;
            }
            case HASH::insert_component:
            {
                messages::ComponentIndexR<T> msgr(msgAcc);
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
                applyTransform(msgAcc.message().source, msgr.isLocal(), msgr.transform());
                return MessageResult::Consumed;
            }
            case HASH::move:
            {
                messages::PropertyVec3R<T> msgr(msgAcc);
                move(msgAcc.message().source, msgr.value());
                return MessageResult::Consumed;
            }
            case HASH::rotate:
            {
                messages::PropertyMat43R<T> msgr(msgAcc);
                rotate(msgAcc.message().source, mat3(msgr.value()));
                return MessageResult::Consumed;
            }
            case HASH::insert_child:
            {
                messages::TaskEntityR<T> msgr(msgAcc);

                Entity * pChild = msgr.entity();

                // We should only become parent to Entities that are managed by our TaskMaster
                ASSERT(TaskMaster::task_master_for_active_thread().isOwnedTask(pChild->task().id()));
                insertChild(pChild);
                return MessageResult::Consumed;
            }
            }

            MessageResult res;

            // Call our sub-classed message routine
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
        else // (mInitStatus != kIS_Activated)
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
                    if (mpEntityInit)
                        mpEntityInit->init();

                    mInitStatus = kIS_Init;
                    // LORRTEMP
                    //LOG_INFO("Entity change state: message: %s, taskid: %u, name: %s, newstate: %d", HASH::reverse_hash(msgId), mTask.id(), HASH::reverse_hash(mTask.nameHash()), mInitStatus);

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
                        //LOG_INFO("Entity change state: message: %s, taskid: %u, name: %s, newstate: %d", HASH::reverse_hash(msgId), mTask.id(), HASH::reverse_hash(mTask.nameHash()), mInitStatus);
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
                    const Asset * pAsset = pHandle->data<Asset>();


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
                    //LOG_INFO("Entity change state: message: %s, taskid: %u, name: %s, newstate: %d", HASH::reverse_hash(msgId), mTask.id(), HASH::reverse_hash(mTask.nameHash()), mInitStatus);

                    // Set us running
                    // LORRTODO: Add mSetRunningOnInit flag and respect it.a
                    //if (mSetRunningOnInit)
                    {
                        messages::TaskStatusQW msgW(HASH::set_task_status, kMessageFlag_Editor, mTask.id(), mTask.id(), TaskStatus::Running);

                        // set script task and each component to running state as well so they can be updated
                        mScriptTask.setStatus(TaskStatus::Running);
                        for (u32 i = 0; i < mComponentCount; ++i)
                        {
                            mpComponents[i].scriptTask().setStatus(TaskStatus::Running);
                        }
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
    }
    return MessageResult::Propagate;
}


void Entity::setTransform(task_id source, const mat43 & mat)
{
    if (mat != mTransform)
    {
        mTransform = applyPositionConstraint(mat);

        // send update_transform to our script task
        {
            StackMessageBlockWriter<0> msgw(HASH::update_transform,
                                            kMessageFlag_ForcePropagate,
                                            source,
                                            mScriptTask.id(),
                                            to_cell(0));
            mScriptTask.message(msgw.accessor());
        }


        // send update_transform our components
        for (u32 i = 0; i < mComponentCount; ++i)
        {
            Task & t = mpComponents[i].scriptTask();
            StackMessageBlockWriter<0> msgw(HASH::update_transform,
                                            kMessageFlag_ForcePropagate,
                                            source,
                                            t.id(),
                                            to_cell(0));
            t.message(msgw.accessor());
        }

        // send update_transform to our child entities
        for (u32 i = 0; i < mChildCount; ++i)
        {
            Entity * pEnt = mpChildren[i];
            StackMessageBlockWriter<0> msgw(HASH::update_transform,
                                            kMessageFlag_ForcePropagate,
                                            source,
                                            pEnt->task().id(),
                                            to_cell(0));
            pEnt->message(msgw.accessor());
        }



        // call transform listeners
        for (u32 i = 0; i < kMaxTransformListeners; ++i)
        {
            TransformListener & tl = mTransformListeners[i];
            if (tl.taskId != 0 && tl.taskId != source) // don't send update to originator, causing a feedback loop
            {
                messages::UidTransformQW msgW(HASH::notify_transform, kMessageFlag_Editor, mTask.id(), tl.taskId, tl.uid);
                msgW.setTransform(mTransform);
            }
            else if (tl.taskId == 0)
            {
                break;
            }
        }
    }
}

void Entity::applyTransform(task_id source, bool isLocal, const mat43 & mat)
{
    if (!isLocal || !mpParent)
    {
        setTransform(source, mat * mTransform);
    }
    else // isLocal
    {
        mLocalTransform = mat;
        setTransform(source, parentTransform() * mLocalTransform);
    }
}

void Entity::updateTransform(task_id source)
{
    if (mpParent)
    {
        setTransform(source, parentTransform() * mLocalTransform);
    }
}

void Entity::setPosition(task_id source, const vec3 & pos)
{
    mat43 trans = mTransform;
    trans[3] = pos;
    setTransform(source, trans);
}

void Entity::move(task_id source, const vec3 & pos)
{
    mat43 trans = mTransform;
    trans[3] += pos;
    setTransform(source, trans);
}

void Entity::setRotation(task_id source, const mat3 & rot)
{
    mat43 trans(mTransform[3], rot);
    setTransform(source, trans);
}

void Entity::rotate(task_id source, const mat3 & rot)
{
    mat3 r(mTransform);
    r = rot * r;
    setRotation(source, r);
}

void Entity::constrainPosition(const vec3 & posMin, const vec3 & posMax)
{
    mPosMin = posMin;
    mPosMax = posMax;
}

mat43 Entity::applyPositionConstraint(const mat43 & mat) const
{
    mat43 cmat = mat;
    cmat.cols[3] = max(cmat.cols[3], mPosMin);
    cmat.cols[3] = min(cmat.cols[3], mPosMax);
    return cmat;
}

void Entity::registerTransformListener(task_id taskId, u32 uid)
{
    for (u32 i = 0; i < kMaxTransformListeners; ++i)
    {
        if (mTransformListeners[i].taskId == 0)
        {
            mTransformListeners[i].taskId = taskId;
            mTransformListeners[i].uid = uid;

            // Send current state of transform
            {
            messages::UidTransformQW msgW(HASH::notify_transform, kMessageFlag_Editor, mTask.id(), taskId, uid);
            msgW.setTransform(mTransform);
            }

            return;
        }
    }
    ERR("Too many TransformListeners registered");
}

void Entity::requestSetParent(task_id parentTaskId)
{
    broadcast_request_set_parent(mTask.id(), parentTaskId, this);
}

void Entity::setParent(Entity * pParent)
{
    ASSERT(pParent);

    if (mpParent)
        unParent();

    // Convert our global transform into local coords relative to parent
    mpParent = pParent;
    mat43 invParent = ~mpParent->transform();
    mLocalTransform = mTransform * invParent;
}

void Entity::unParent()
{
    ASSERT(mpParent);
    mLocalTransform = mat43{1.0f};
    mpParent = nullptr;
}

const mat43 & Entity::parentTransform() const
{
    if (!mpParent)
    {
        static const mat43 ident = mat43(1.0);
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
    //LOG_INFO("Entity change state: taskid: %u, name: %s, newstate: %d", mTask.id(), HASH::reverse_hash(mTask.nameHash()), mInitStatus);

    {
    StackMessageBlockWriter<0> msg(HASH::init_independent_data__, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0));
    // init all non-asset properties in each component
    mScriptTask.message(msg.accessor());
    }
    // Call the scripted init_properties callback to set any overridden values
    if (mpEntityInit)
        mpEntityInit->initIndependentData();

    {
    StackMessageBlockWriter<0> msg(HASH::init_dependent_data__, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0));
    // init all non-asset fields in each component
    mScriptTask.message(msg.accessor());
    }
    // Call the scripted init_fields callback to set any overridden values
    if (mpEntityInit)
    {
        mpEntityInit->initDependentData();

        // We can now delete the mpEntityInit since we no longer need it
        GDELETE(mpEntityInit);
        mpEntityInit = nullptr;
    }

    // Send the #init message, which may be handled explicitly in the script
    {
    StackMessageBlockWriter<0> msg(HASH::init, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(0));
    mTask.message(msg.accessor());
    }

    // Entity is now fully initialized, if it has a ready message, notify the creator
    notifyCreatorReady();
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
    //LOG_INFO("Component inserted: entityId: %u, entityName: %s, taskId: %u, taskName: %s", mTask.id(), HASH::reverse_hash(mTask.nameHash()), pComp->scriptTask().id(), HASH::reverse_hash(pComp->nameHash()));

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
                mpChildren[i]->unParent();
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

void Entity::send_ready_init(u32 sourceTaskId, u32 targetTaskId, u32 message)
{
    MessageQueueWriter msgw(HASH::ready_init, kMessageFlag_None, sourceTaskId, targetTaskId, to_cell(message), 0);
}

void Entity::send_ready_notify(u32 sourceTaskId, u32 targetTaskId, u32 message)
{
    MessageQueueWriter msgw(HASH::ready_notify, kMessageFlag_None, sourceTaskId, targetTaskId, to_cell(message), 0);
}

void Entity::readyInit(u32 message)
{
    ReadyInfo * pRi = findReadyInfo(message);
    if (pRi)
    {
        pRi->waitingCount++;
    }
    else
    {
        ReadyInfo * pRi = findReadyInfo(0);
        if (pRi)
        {
            pRi->message = message;
            pRi->waitingCount = 1;
        }
        else
        {
            ERR("readyInit unable to init, out of ReadyInfo structs: %u", message);
        }
    }
}

void Entity::readyNotify(u32 message)
{
    ReadyInfo * pRi = findReadyInfo(message);
    if (pRi)
    {
        ASSERT(pRi->waitingCount > 0);
        pRi->waitingCount--;
    }
    else
    {
        ERR("readyNotify to unknown message: %u", message);
    }
}

void Entity::notifyCreatorReady()
{
    if (mCreatorTask != 0 && mReadyMessage != 0)
    {
        send_ready_notify(mTask.id(), mCreatorTask, mReadyMessage);
    }
}

Entity::ReadyInfo * Entity::findReadyInfo(u32 message)
{
    for (u32 i = 0; i < kReadyInfoCount; ++i)
    {
        if (mReadyInfos[i].message == message)
            return &mReadyInfos[i];
    }
    return nullptr;
}


// Template decls so we can define message func here in the .cpp
template MessageResult Entity::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult Entity::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen
