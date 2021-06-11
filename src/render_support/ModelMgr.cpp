//------------------------------------------------------------------------------
// ModelMgr.cpp - Provides material ordered mesh iteration and model/material release semantics
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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
#include "engine/TaskMaster.h"

#include "engine/messages/CameraOrtho.h"
#include "engine/messages/CameraPersp.h"
#include "engine/messages/CollisionBox.h"
#include "engine/messages/CollisionConvexHull.h"
#include "engine/messages/ModelBody.h"
#include "engine/messages/ModelInstance.h"
#include "engine/messages/NotifyWatcherMat43.h"
#include "engine/messages/Transform.h"
#include "engine/messages/UidScalar.h"
#include "engine/messages/UidScalarTransform.h"
#include "engine/messages/UidTransform.h"
#include "engine/messages/UidVec3.h"

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
#if HAS(MESSAGE_TRACING)
    LOG_INFO("MSG: ModelMgr %s(0x%x) -> %s(0x%x): %s(%s)", task_name(msgAcc.message().source), msgAcc.message().source, task_name(msgAcc.message().target), msgAcc.message().target, HASH::reverse_hash(msgAcc.message().msgId), HASH::reverse_hash(msgAcc.message().payload.u));
#endif
    const Message & msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::model_transform:
    {
        messages::NotifyWatcherMat43R<T> msgr(msgAcc);
        ASSERT(msgr.valueType() == HASH::mat43);
        transformModel(msgr.uid(), msgr.value());

        return MessageResult::Consumed;
    }
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
                                                      pModelInst->pass(),
                                                      pModelInst->renderFlags(),
                                                      pModelInst->mTransform,
                                                      pModelInst->mIsRenderable,
                                                      pModelInst->mIsStatic);
            ModelInstance::model_insert(kModelMgrTaskId, kRendererTaskId, pModelInstRenderer);
        }

        pModelInst->registerTransformWatcher(kModelMgrTaskId);

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
    case HASH::model_init_body:
    {
        messages::ModelBodyR<T> msgr(msgAcc);
        auto modelPair = mModelMap.find(msgr.uid());
        if (modelPair != mModelMap.end())
        {
            modelPair->second->mHasBody = true;

            btCompoundShape * pCompoundShape = nullptr;
            const auto & halfExt = modelPair->second->model().gmdl().halfExtents();
            switch (msgr.collisionShape())
            {
            case system_api::PHY_SHAPE_BOX:
                pCompoundShape = mPhysics.findBox(halfExt, modelPair->second->model().gmdl().center());
                break;
            case system_api::PHY_SHAPE_CAPSULE:
                pCompoundShape = mPhysics.findCapsule(halfExt.x, halfExt.y * 2, modelPair->second->model().gmdl().center());
                break;
            default:
                ERR("model_init_body with unknown collision shape: %u", msgr.collisionShape());
                break;
            }

            if (pCompoundShape)
            {
                ModelMotionState * pMotionState = GNEW(kMEM_Physics, ModelMotionState, *modelPair->second);

                mPhysics.insertRigidBody(modelPair->second->model().uid(),
                                         modelPair->second->model().owner(),
                                         pCompoundShape,
                                         pMotionState,
                                         modelPair->second->model().gmdl().center(),
                                         mat43(1.0f),
                                         msgr.mass(),
                                         msgr.friction(),
                                         msgr.flags(),
                                         msgr.linearFactor(),
                                         msgr.angularFactor(),
                                         msgr.message(),
                                         msgr.group(),
                                         msgr.mask03(),
                                         msgr.mask47());
            }
        }
        else
        {
            ERR("model_init_body for unknown model, uid: %u", msgr.uid());
        }
        return MessageResult::Consumed;
    }
    case HASH::remove_body:
    {
        u32 uid = msg.payload.u;
        mPhysics.removeRigidBody(uid);
        return MessageResult::Consumed;
    }
    case HASH::model_remove_body:
    {
        u32 uid = msg.payload.u;
        auto modelPair = mModelMap.find(uid);
        if (modelPair != mModelMap.end())
        {
            if (modelPair->second->mHasBody)
            {
                modelPair->second->mHasBody = false;
                mPhysics.removeRigidBody(uid);
            }
            else
            {
                ERR("model_remove_body for model with no body, uid: %u", uid);
            }
        }
        else
        {
            ERR("model_remove_body for unknown model, uid: %u", uid);
        }
        return MessageResult::Consumed;
    }
    case HASH::collision_box_create:
    {
        messages::CollisionBoxR<T> msgr(msgAcc);
        mPhysics.insertCollisionBox(msgr.uid(),
                                    msg.source,
                                    msgr.center(),
                                    msgr.halfExtents(),
                                    msgr.transform(),
                                    msgr.mass(),
                                    msgr.friction(),
                                    msgr.linearFactor(),
                                    msgr.angularFactor(),
                                    msgr.message(),
                                    msgr.group(),
                                    msgr.mask03(),
                                    msgr.mask47());

        return MessageResult::Consumed;
    }
    case HASH::collision_convex_hull_create:
    {
        messages::CollisionConvexHullR<T> msgr(msgAcc);
        mPhysics.insertCollisionConvexHull(msgr.uid(),
                                           msg.source,
                                           msgr.gmdlPoints(),
                                           msgr.transform(),
                                           msgr.mass(),
                                           msgr.friction(),
                                           msgr.message(),
                                           msgr.group(),
                                           msgr.mask03(),
                                           msgr.mask47());

        return MessageResult::Consumed;
    }
	case HASH::remove_task__:
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
                        mPhysics.removeRigidBody(uid);
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

void ModelMgr::transformModel(i32 modelUid, const mat43 & transform)
{
    auto modelPair = mModelMap.find(modelUid);
    if (modelPair != mModelMap.end())
    {
        if (modelPair->second->mTransform != transform)
        {
            // update the instance
            modelPair->second->mDirection = position(transform) - position(modelPair->second->mTransform);
            if (modelPair->second->mDirection != vec3(0.0f))
                modelPair->second->mDirection = normalize(modelPair->second->mDirection);
            modelPair->second->mTransform = transform;

            if (modelPair->second->mHasBody)
            {
                mPhysics.setTransform(modelUid, transform);
            }
        }
    }
    else
    {
        ERR("transformModel for unknown model, uid: %u", modelUid);
    }
}

// Template decls so we can define message func here in the .cpp
template MessageResult ModelMgr::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult ModelMgr::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);


namespace system_api
{

i32 model_anim_create(AssetHandleP pAssetHandleGmdl,
                      AssetHandleP pAssetHandleGaim,
                      i32 stageHash,
                      i32 passHash,
                      const mat43 & transform,
                      Entity * pCaller)
{
    ASSERT(pAssetHandleGmdl->typeHash() == HASH::asset);
    const Asset * pAssetGmdl = pAssetHandleGmdl->data<Asset>();

    const Asset * pAssetGaim = nullptr;
    if (pAssetHandleGaim)
    {
        ASSERT(pAssetHandleGaim->typeHash() == HASH::asset);
        pAssetGaim = pAssetHandleGaim->data<Asset>();
    }

    RenderPass pass = pass_from_hash(passHash);

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pAssetGmdl, pAssetGaim);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, pass, kRF_Normal, transform, true, false);

    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);

    return pModel->uid();
}

i32 model_create(AssetHandleP pAssetHandleGmdl,
                 i32 stageHash,
                 i32 passHash,
                 const mat43 & transform,
                 Entity * pCaller)
{
    return model_anim_create(pAssetHandleGmdl, nullptr, stageHash, passHash, transform, pCaller);
}

void model_set_velocity(i32 modelUid, const vec3 & velocity, Entity * pCaller)
{
    messages::UidVec3BW msgw(HASH::model_set_velocity, kMessageFlag_None, pCaller->task().id(), kModelMgrTaskId, modelUid);
    msgw.setVector(velocity);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void model_set_angular_velocity(i32 modelUid, const vec3 & velocity, Entity * pCaller)
{
    messages::UidVec3BW msgw(HASH::model_set_angular_velocity, kMessageFlag_None, pCaller->task().id(), kModelMgrTaskId, modelUid);
    msgw.setVector(velocity);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void model_init_body(i32 modelUid,
                     f32 mass,
                     f32 friction,
                     i32 flags,
                     i32 collisionShape,
                     vec3 linearFactor,
                     vec3 angularFactor,
                     i32 message,
                     i32 group,
                     ivec4 mask03,
                     ivec4 mask47,
                     Entity * pCaller)
{
    messages::ModelBodyBW msgw(HASH::model_init_body, kMessageFlag_None, pCaller->task().id(), kModelMgrTaskId, modelUid);
    msgw.setMass(mass);
    msgw.setFriction(friction);
    msgw.setFlags(flags);
    msgw.setCollisionShape(collisionShape);
    msgw.setLinearFactor(linearFactor);
    msgw.setAngularFactor(angularFactor);
    msgw.setMessage(message);
    msgw.setGroup(group);
    msgw.setMask03(mask03);
    msgw.setMask47(mask47);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void model_remove_body(i32 modelUid, Entity * pCaller)
{
    ImmediateMessageWriter<0> msgw(HASH::model_remove_body, kMessageFlag_None, pCaller->task().id(), kModelMgrTaskId, to_cell(modelUid));
}

void model_stage_show(i32 stageHash, Entity * pCaller)
{
    ImmediateMessageWriter<0> msgw(HASH::model_stage_show, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, to_cell(stageHash));
}

void model_stage_hide(i32 stageHash, Entity * pCaller)
{
    ImmediateMessageWriter<0> msgw(HASH::model_stage_hide, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, to_cell(stageHash));
}

void model_stage_hide_all(Entity * pCaller)
{
    ImmediateMessageWriter<0> msgw(HASH::model_stage_hide_all, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, to_cell(0));
}

void model_stage_remove(i32 stageHash, Entity * pCaller)
{
    ImmediateMessageWriter<0> msgw(HASH::model_stage_remove, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, to_cell(stageHash));
}

void model_stage_remove_all(Entity * pCaller)
{
    ImmediateMessageWriter<0> msgw(HASH::model_stage_remove_all, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, to_cell(0));
}

i32 model_stage_camera_create_persp(i32 stageHash,
                                    f32 fov,
                                    f32 nearClip,
                                    f32 farClip,
                                    const mat43 & view,
                                    Entity * pCaller)
{
    ruid uid = RenderObject::next_uid();
    messages::CameraPerspBW msgw(HASH::model_stage_camera_insert_persp, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, uid);
    msgw.setStageHash(stageHash);
    msgw.setFov(fov);
    msgw.setNearClip(nearClip);
    msgw.setFarClip(farClip);
    msgw.setView(view);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
    return uid;
}

i32 model_stage_camera_create_ortho(i32 stageHash,
                                    f32 bounds,
                                    f32 nearClip,
                                    f32 farClip,
                                    const mat43 & view,
                                    Entity * pCaller)
{
    ruid uid = RenderObject::next_uid();
    messages::CameraOrthoBW msgw(HASH::model_stage_camera_insert_ortho, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, uid);
    msgw.setStageHash(stageHash);
    msgw.setBounds(bounds);
    msgw.setNearClip(nearClip);
    msgw.setFarClip(farClip);
    msgw.setView(view);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
    return uid;
}

void model_stage_camera_scale(i32 cameraUid,
                              f32 scale,
                              Entity * pCaller)
{
    messages::UidScalarBW msgw(HASH::model_stage_camera_scale, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, cameraUid);
    msgw.setScalar(scale);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void model_stage_camera_view(i32 cameraUid,
                             const mat43 & view,
                             Entity * pCaller)
{
    messages::UidTransformBW msgw(HASH::model_stage_camera_view, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, cameraUid);
    msgw.setTransform(view);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void model_stage_camera_scale_and_view(i32 cameraUid,
                                       f32 scale,
                                       const mat43 & view,
                                       Entity * pCaller)
{
    messages::UidScalarTransformBW msgw(HASH::model_stage_camera_scale_and_view, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, cameraUid);
    msgw.setScalar(scale);
    msgw.setTransform(view);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}


void model_stage_camera_activate(i32 cameraUid, Entity * pCaller)
{
    ImmediateMessageWriter<0> msgw(HASH::model_stage_camera_activate, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, to_cell(cameraUid));
}

void model_stage_camera_remove(i32 cameraUid, Entity * pCaller)
{
    ImmediateMessageWriter<0> msgw(HASH::model_stage_camera_remove, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, to_cell(cameraUid));
}

} // namespace system_api

} // namespace gaen
