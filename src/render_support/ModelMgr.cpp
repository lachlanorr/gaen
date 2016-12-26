//------------------------------------------------------------------------------
// ModelMgr.cpp - Provides material ordered mesh iteration and model/material release semantics
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

#include "render_support/stdafx.h"

#include "assets/Gmdl.h"

#include "engine/Handle.h"
#include "engine/Asset.h"
#include "engine/Entity.h"

#include "engine/messages/ModelInstance.h"
#include "engine/messages/UidVec3.h"
#include "engine/messages/ModelBody.h"
#include "engine/messages/CameraPersp.h"
#include "engine/messages/CameraOrtho.h"
#include "engine/messages/UidTransform.h"

#include "render_support/ModelMgr.h"

namespace gaen
{


ModelMgr::~ModelMgr()
{
    // LORRTODO: Cleanup is causing crash on exit... need to redesign how we release assets
    //for (auto & modelPair : mModelMap)
    //{
    //    ModelInstance::model_remove(kModelMgrTaskId, kRendererTaskId, modelPair.second->model().uid());
    //}
}

void ModelMgr::update(f32 delta)
{
    mPhysics.update(delta);
}

template <typename T>
MessageResult ModelMgr::message(const T & msgAcc)
{
    const Message & msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::model_insert:
    {
        messages::ModelInstanceR<T> msgr(msgAcc);

        ModelInstance * pModelInst = msgr.modelInstance();
        ASSERT(pModelInst);
        ASSERT(mModelMap.find(pModelInst->model().uid()) == mModelMap.end());
        mModelMap.emplace(pModelInst->model().uid(), pModelInst);
        mModelOwners[pModelInst->model().owner()].push_back(pModelInst->model().uid());

        if (pModelInst->mIsRenderable)
        {
            // Send a copy to the renderer
            Model * pModelRenderer = GNEW(kMEM_Renderer, Model, pModelInst->model());
            ModelInstance * pModelInstRenderer = GNEW(kMEM_Renderer,
                                                      ModelInstance,
                                                      pModelRenderer,
                                                      pModelInst->stageHash(),
                                                      pModelInst->mTransform,
                                                      pModelInst->mIsRenderable);
            ModelInstance::model_insert(kModelMgrTaskId, kRendererTaskId, pModelInstRenderer);
        }

        return MessageResult::Consumed;
    }
    case HASH::model_set_velocity:
    {
        messages::UidVec3R<T> msgr(msgAcc);
        auto modelPair = mModelMap.find(msgr.uid());
        if (modelPair != mModelMap.end())
        {
            if (modelPair->second->mHasBody)
            {
                mPhysics.setVelocity(msgr.uid(), msgr.vector());
            }
            else
            {
                ERR("model_set_velocity for non rigid body model, uid: %u", msgr.uid());
            }
        }
        else
        {
            ERR("model_set_velocity for unknown model, uid: %u", msgr.uid());
        }
        return MessageResult::Consumed;
    }
    case HASH::model_set_angular_velocity:
    {
        messages::UidVec3R<T> msgr(msgAcc);
        auto modelPair = mModelMap.find(msgr.uid());
        if (modelPair != mModelMap.end())
        {
            if (modelPair->second->mHasBody)
            {
                mPhysics.setAngularVelocity(msgr.uid(), msgr.vector());
            }
            else
            {
                ERR("model_set_angular_velocity for non rigid body model, uid: %u", msgr.uid());
            }
        }
        else
        {
            ERR("model_set_angular_velocity for unknown model, uid: %u", msgr.uid());
        }
        return MessageResult::Consumed;
    }
    case HASH::model_transform:
    {
        messages::UidTransformR<T> msgr(msgAcc);
        auto modelPair = mModelMap.find(msgr.uid());
        if (modelPair != mModelMap.end())
        {
            if (modelPair->second->mHasBody)
            {
                mPhysics.setTransform(msgr.uid(), msgr.transform());
            }
            else
            {
                ERR("model_transform for non rigid body model, uid: %u", msgr.uid());
            }
        }
        else
        {
            ERR("model_transform for unknown model, uid: %u", msgr.uid());
        }
        return MessageResult::Consumed;
    }
    case HASH::model_init_body:
    {
        messages::ModelBodyR<T> msgr(msgAcc);
        auto modelPair = mModelMap.find(msgr.uid());
        if (modelPair != mModelMap.end())
        {
            mPhysics.insert(*modelPair->second, msgr.mass(), msgr.group(), msgr.mask03(), msgr.mask47());
        }
        else
        {
            ERR("model_init_body for unknown model, uid: %u", msgr.uid());
        }
        return MessageResult::Consumed;
    }
	case HASH::remove_task:
	{
        task_id taskIdToRemove = msg.payload.u;
        auto itL = mModelOwners.find(taskIdToRemove);
        // It's ok if we don't find it, it just means this task had no models
        if (itL != mModelOwners.end())
        {
            for (u32 uid : itL->second)
            {
                auto modelPair = mModelMap.find(uid);
                if (modelPair != mModelMap.end())
                {
                    // remove from physics simulation if necessary
                    if (modelPair->second->mHasBody)
                    {
                        mPhysics.remove(uid);
                    }
                    
                    // send model_remove to renderer who in turn will send it back to us once
                    ModelInstance::model_remove(kModelMgrTaskId, kRendererTaskId, uid);
                }
                else
                {
                    ERR("remove_task for task_id: %u has non-existent model uid: %u", taskIdToRemove, uid);
                }
            }
            mModelOwners.erase(itL);
        }
        return MessageResult::Consumed;
	}
    case HASH::model_remove:
    {
        u32 uid = msg.payload.u;

        auto modelPair = mModelMap.find(uid);
        if (modelPair != mModelMap.end())
        {
            modelPair->second->destroyModel();
            mModelMap.erase(modelPair);
        }
        else
        {
            ERR("model_remove for unknown animation, uid: %u", uid);
        }
        return MessageResult::Consumed;
    }
    default:
        PANIC("Unknown ModelMgr message: %d", msg.msgId);
    }
    return MessageResult::Consumed;
}

// Template decls so we can define message func here in the .cpp
template MessageResult ModelMgr::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);


namespace system_api
{

i32 model_create(AssetHandleP pAssetHandle, i32 stageHash, const mat43 & transform, Entity & caller)
{
    ASSERT(pAssetHandle->typeHash() == HASH::asset);
    const Asset * pAsset = reinterpret_cast<const Asset*>(pAssetHandle->data());

    Model * pModel = GNEW(kMEM_Engine, Model, caller.task().id(), pAsset);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, transform, true);

    ModelInstance::model_insert(caller.task().id(), kModelMgrTaskId, pModelInst);

    return pModel->uid();
}

void model_set_velocity(i32 modelUid, const vec3 & velocity, Entity & caller)
{
    messages::UidVec3QW msgw(HASH::model_set_velocity, kMessageFlag_None, caller.task().id(), kModelMgrTaskId, modelUid);
    msgw.setVector(velocity);
}

void model_set_angular_velocity(i32 modelUid, const vec3 & velocity, Entity & caller)
{
    messages::UidVec3QW msgw(HASH::model_set_angular_velocity, kMessageFlag_None, caller.task().id(), kModelMgrTaskId, modelUid);
    msgw.setVector(velocity);
}

void model_transform(i32 modelUid, const mat43 & transform, Entity & caller)
{
    messages::UidTransformQW msgw(HASH::model_transform, kMessageFlag_None, caller.task().id(), kModelMgrTaskId, modelUid);
    msgw.setTransform(transform);
}

void model_init_body(i32 modelUid, f32 mass, i32 group, ivec4 mask03, ivec4 mask47, Entity & caller)
{
    messages::ModelBodyQW msgw(HASH::model_init_body, kMessageFlag_None, caller.task().id(), kModelMgrTaskId, modelUid);
    msgw.setMass(mass);
    msgw.setGroup(group);
    msgw.setMask03(mask03);
    msgw.setMask47(mask47);
}

void model_stage_show(i32 stageHash, Entity & caller)
{
    MessageQueueWriter msgw(HASH::model_stage_show, kMessageFlag_None, caller.task().id(), kRendererTaskId, to_cell(stageHash), 0);
}

void model_stage_hide(i32 stageHash, Entity & caller)
{
    MessageQueueWriter msgw(HASH::model_stage_hide, kMessageFlag_None, caller.task().id(), kRendererTaskId, to_cell(stageHash), 0);
}

void model_stage_remove(i32 stageHash, Entity & caller)
{
    MessageQueueWriter msgw(HASH::model_stage_remove, kMessageFlag_None, caller.task().id(), kRendererTaskId, to_cell(stageHash), 0);
}

i32 model_stage_camera_create_persp(i32 stageHash,
                                    f32 fov,
                                    f32 nearClip,
                                    f32 farClip,
                                    const mat43 & view,
                                    Entity & caller)
{
    ruid uid = RenderObject::next_uid();
    messages::CameraPerspQW msgw(HASH::model_stage_camera_insert_persp, kMessageFlag_None, caller.task().id(), kRendererTaskId, uid);
    msgw.setStageHash(stageHash);
    msgw.setFov(fov);
    msgw.setNearClip(nearClip);
    msgw.setFarClip(farClip);
    msgw.setView(view);
    return uid;
}

i32 model_stage_camera_create_ortho(i32 stageHash,
                                    f32 scale,
                                    f32 nearClip,
                                    f32 farClip,
                                    const mat43 & view,
                                    Entity & caller)
{
    ruid uid = RenderObject::next_uid();
    messages::CameraOrthoQW msgw(HASH::model_stage_camera_insert_ortho, kMessageFlag_None, caller.task().id(), kRendererTaskId, uid);
    msgw.setStageHash(stageHash);
    msgw.setScale(scale);
    msgw.setNearClip(nearClip);
    msgw.setFarClip(farClip);
    msgw.setView(view);
    return uid;
}

void model_stage_camera_view(i32 cameraUid,
                             const mat43 & view,
                             Entity & caller)
{
    messages::UidTransformQW msgw(HASH::model_stage_camera_view, kMessageFlag_None, caller.task().id(), kRendererTaskId, cameraUid);
    msgw.setTransform(view);
}

void model_stage_camera_activate(i32 cameraUid, Entity & caller)
{
    MessageQueueWriter msgw(HASH::model_stage_camera_activate, kMessageFlag_None, caller.task().id(), kRendererTaskId, to_cell(cameraUid), 0);
}

void model_stage_camera_remove(i32 cameraUid, Entity & caller)
{
    MessageQueueWriter msgw(HASH::model_stage_camera_remove, kMessageFlag_None, caller.task().id(), kRendererTaskId, to_cell(cameraUid), 0);
}

} // namespace system_api

} // namespace gaen
