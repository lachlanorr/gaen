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
#include "engine/MessageQueue.h"
#include "engine/math.h"

namespace gaen
{

class Entity
{
    friend struct Task;
public:
    Entity(u32 propertyBufferSize,
           u8 * pPropertyBuffer = nullptr,
           const Mat34 & transform = Mat34::identity());

    void update(f32 deltaSecs);
    MessageResult message(const MessageQueue::MessageAccessor& msgAcc);

    task_id taskId() { return mTaskId; }

    void insertComponent(Task & task)
    {
        // LORRTODO
        //nASSERT(!pComp->mpEntity);
        //pComp->mpEntity = this;
        //mComponents.push_back(Task::create(pComp));
    }

private:
    void setTaskId(task_id taskId) { mTaskId = taskId; }

    void init(const MessageQueue::MessageAccessor& msgAcc) { mTaskId = msgAcc.message().payload.u; }
    void fin(const MessageQueue::MessageAccessor& msgAcc);

    task_id mTaskId = -1;
    
    Mat34 mLocalTransform;
    Mat34 mGlobalTransform;

    u32 mPropertyBufferSize;
    u32 mPropertyBufferHWM;
    u8 * mpPropertyBuffer;

    Vector<Task, kMEM_Engine> mComponents;
    List<Entity*, kMEM_Engine> mChildren;
};

} // namespcae gaen

#endif // #ifndef GAEN_ENGINE_ENTITY_H
