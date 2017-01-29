//------------------------------------------------------------------------------
// Entity.h - A game entity which contains a collection of Components
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

#ifndef GAEN_ENGINE_ENTITY_H
#define GAEN_ENGINE_ENTITY_H

#include "core/Vector.h"
#include "core/List.h"
#include "math/mat43.h"

#include "engine/Task.h"
#include "engine/Component.h"
#include "engine/MessageQueue.h"
#include "engine/EntityInit.h"

namespace gaen
{

class Asset;
class BlockMemory;
class CmpString;

class Entity
{
public:
    Entity(u32 nameHash,
           u32 childrenMax,
           u32 componentsMax,
           u32 blocksMax,
           task_id initParentTask,
           task_id creatorTask,
           u32 readyMessage);
    ~Entity();

    const Task & task() const { return mTask; }
    Task & task() { return mTask; }

    const Task & scriptTask() const { return mScriptTask; }
    Task & scriptTask() { return mScriptTask; }

    // Make "self()" consistent in Component and Entity to simplify
    // the c++ codegen
    const Entity & self() const { return *this; }
    Entity & self() { return *this; }

    void setEntityInit(EntityInit * pEntityInit) { mpEntityInit = pEntityInit; }

    void activate();
    static Entity * activate_start_entity(u32 entityHash);

    void update(f32 delta);

    template <typename T>
    MessageResult message(const T& msgAcc);

    const mat43 & transform() const { return mTransform; }
    void setTransform(const mat43 & mat);
    void applyTransform(const mat43 & mat);
    void updateTransform();

    void registerTransformListener(task_id taskId, u32 uid);

    u32 player() const { return mPlayer; }

    Entity * parent() { return mpParent; }

    void requestSetParent(task_id parentTaskId);

    void setParent(Entity * pEntity);
    void unParent();
    const mat43 & parentTransform() const;

    BlockMemory & blockMemory();
    void collect();

    void requestAsset(u32 subTaskId, u32 nameHash, const CmpString & path);

    static void send_ready_init(u32 sourceTaskId, u32 targetTaskId, u32 message);
    static void send_ready_notify(u32 sourceTaskId, u32 targetTaskId, u32 message);
    void readyInit(u32 message);
    void readyNotify(u32 message);
protected:
    enum InitStatus
    {
        kIS_Uninitialized = 0,
        kIS_Init          = 1,
        kIS_RequestAssets = 2,
        kIS_AssetsReady   = 3,
        kIS_Activated     = 4,
        kIS_Fin           = 5
    };

    // Max entities that can be created before they're inserted into the engine
    static const u32 kMaxEntityStage = 16;

    void finSelf();

    void finalizeAssetInit();

    Task& insertComponent(u32 nameHash, u32 index);

    u32 findComponent(u32 nameHash);

    void moveComponentUp(u32 nameHash);
    void moveComponentDown(u32 nameHash);
    
    void removeBlocks(Block * pStart, u32 count);
    void removeComponent(u32 nameHash);

    void insertChild(Entity * pEntity);
    void removeChild(Entity * pEntity);
    void removeChild(task_id taskId);

    void growComponents();
    void growBlocks(u32 minSizeIncrease);
    void growChildren();

    bool areAllAssetsLoaded()
    {
        ASSERT(mAssetsLoaded <= mAssetsRequested);
        return mAssetsLoaded == mAssetsRequested;
    }

    // Task representing this base class update/message methods.  This
    // is the "public" task_id used external to the entity to refer to
    // it. Messages sent to entities need to use this task_id as this
    // is is the task_id that will be registered within TaskMasters.
    Task mTask;

    // Task representing Entity "sub class" created by writing an
    // entity Compose script.
    Task mScriptTask;

    u32 mNameHash;

    bool mIsFinSelfSent;
    bool mIsDead;

    bool mIsTransformDirty;
    mat43 mTransform;
    mat43 mLocalTransform;

    static const u32 kMaxTransformListeners = 4;
    struct TransformListener
    {
        task_id taskId;
        i32 uid;
    };
    TransformListener mTransformListeners[kMaxTransformListeners];

    u32 mPlayer;

    Component * mpComponents;
    u32 mComponentsMax;
    u32 mComponentCount;

    Block * mpBlocks;
    u32 mBlocksMax;
    u32 mBlockCount;
    InitStatus mInitStatus;

    Entity * mpParent;

    Entity ** mpChildren;
    u32 mChildrenMax;
    u32 mChildCount;

    // Dynamic memory for scripts
    BlockMemory * mpBlockMemory;

    // Asset pointers
    u32 mAssetsRequested;
    u32 mAssetsLoaded;

    EntityInit * mpEntityInit;

    // Optional initial parent
    task_id mInitParentTask;

    // Ready notification members
    task_id mCreatorTask;
    u32 mReadyMessage;
    struct ReadyInfo
    {
        u32 message;
        i32 waitingCount;
        void clear() { message = 0; waitingCount = 0; }
    };
    static const u32 kReadyInfoCount = 4;
    ReadyInfo mReadyInfos[kReadyInfoCount];
    void notifyCreatorReady();
    ReadyInfo * findReadyInfo(u32 message);

};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ENTITY_H
