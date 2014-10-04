//------------------------------------------------------------------------------
// Entity.h - A game entity which contains a collection of Components
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

#ifndef GAEN_ENGINE_ENTITY_H
#define GAEN_ENGINE_ENTITY_H

#include "core/Vector.h"
#include "core/List.h"

#include "engine/Task.h"
#include "engine/Component.h"
#include "engine/MessageQueue.h"
#include "engine/math.h"

namespace gaen
{

class Entity
{
public:
    Entity(u32 nameHash, u32 childrenMax, u32 componentsMax, u32 blocksMax);
    ~Entity();

    const Task & task() const { return mTask; }
    Task & task() { return mTask; }

    const Task & scriptTask() const { return mScriptTask; }
    Task & scripTask() { return mScriptTask; }

    void update(f32 deltaSecs);

    template <typename T>
    MessageResult message(const T& msgAcc);

    const Mat34 & localTransform() const { return mLocalTransform; }
    const Mat34 & globalTransform() const { return mGlobalTransform; }

protected:
    Entity & entity() { return *this; }
    
    Task& insertComponent(u32 nameHash, u32 index);

    u32 findComponent(u32 nameHash);

    void moveComponentUp(u32 nameHash);
    void moveComponentDown(u32 nameHash);
    
    void removeBlocks(Block * pStart, u32 count);
    void removeComponent(u32 nameHash);

    void insertChild(Task & task);
    void removeChild(Task & task);

    void growComponents();
    void growBlocks(u32 minSizeIncrease);
    void growChildren();

    // Task representing this base class update/message methods.  This
    // is the "public" task_id used external to the entity to refer to
    // it. Messages sent to entities need to use this task_id as this
    // is is the task_id that will be registered within TaskMasters.
    Task mTask;

    // Task representing Entity "sub class" created by writing an
    // entity Compose script.
    Task mScriptTask;
    
    Mat34 mLocalTransform;
    Mat34 mGlobalTransform;

    Component * mpComponents;
    u32 mComponentsMax;
    u32 mComponentCount;

    Block * mpBlocks;
    u32 mBlocksMax;
    u32 mBlockCount;

    Task * mpChildren;
    u32 mChildrenMax;
    u32 mChildCount;
};

} // namespcae gaen

#endif // #ifndef GAEN_ENGINE_ENTITY_H
