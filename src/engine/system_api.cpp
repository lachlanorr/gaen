//------------------------------------------------------------------------------
// system_api.cpp - C functionas available to Compose scripts
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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
#include "engine/math.h"
#include "engine/shapes.h"
#include "engine/InputMgr.h"
#include "engine/MessageWriter.h"
#include "engine/messages/InsertModelInstance.h"
#include "engine/messages/InsertTask.h"
#include "engine/messages/TransformId.h"
#include "engine/messages/InsertLightDirectional.h"
#include "engine/messages/MoveCamera.h"
#include "engine/messages/WatchInputState.h"
#include "engine/messages/WatchMouse.h"

#include "engine/system_api.h"

namespace gaen
{

//------------------------------------------------------------------------------
// API Defs start here
//------------------------------------------------------------------------------

namespace system_api
{

void print(CmpString str, Entity & caller)
{
    LOG_INFO(str.c_str());
}

CmpString hashstr(u32 hash, Entity & caller)
{
    return caller.blockMemory().stringAlloc(HASH::reverse_hash(hash));
}

void insert_entity(u32 id, Entity & caller)
{
    Entity * pEnt = caller.unstageEntity(id);
    if (pEnt)
    {
        ASSERT(sizeof(Task) % kBlockSize == 0);
        messages::InsertTaskBW msgw(HASH::insert_task,
                                    kMessageFlag_None,
                                    caller.task().id(),
                                    active_thread_id(),
                                    active_thread_id());
        msgw.setTask(pEnt->task());
        broadcast_message(msgw.accessor());
    }
}

f32 radians(f32 degrees, Entity & caller)
{
    return ::gaen::radians(degrees);
}

f32 degrees(f32 radians, Entity & caller)
{
    return ::gaen::degrees(radians);
}


void watch_input_state(u32 state, u32 deviceId, u32 message, Entity & caller)
{
    messages::WatchInputStateQW msgQW(HASH::watch_input_state,
                                      kMessageFlag_None,
                                      caller.task().id(),
                                      kInputMgrTaskId,
                                      state);
    msgQW.setDeviceId(deviceId);
    msgQW.setMessage(message);
}

void watch_mouse(u32 moveMessage, u32 wheelMessage, Entity & caller)
{
    messages::WatchMouseQW msgQW(HASH::watch_mouse,
                                 kMessageFlag_None,
                                 caller.task().id(),
                                 kInputMgrTaskId,
                                 moveMessage);
    msgQW.setWheelMessage(wheelMessage);
}

Mat34 mat34_rotation(const Vec3 & angles, Entity & caller)
{
    return Mat34::rotation(angles);
}

Mat3 mat3_rotation(const Vec3 & angles, Entity & caller)
{
    return Mat3::rotation(angles);
}

Quat quat_from_axis_angle(const Vec3 & dir, f32 angle, Entity & caller)
{
    return Quat::from_axis_angle(dir, angle, false);
}

Quat quat_normalize(const Quat & quat, Entity & caller)
{
    Quat q = quat;
    q.normalize();
    return q;
}

u32 renderer_gen_uid(Entity & caller)
{
    static std::atomic<u32> sNextUid(1);
    return sNextUid.fetch_add(1, std::memory_order_relaxed);
}

void renderer_move_camera(const Vec3 & position, const Quat & direction, Entity & caller)
{
    messages::MoveCameraQW msgQW(HASH::renderer_move_camera,
                                 kMessageFlag_None,
                                 caller.task().id(),
                                 kRendererTaskId);

    msgQW.setPosition(position);
    msgQW.setDirection(direction);
}

void renderer_insert_model_instance(u32 uid,
                                    Handle & modelHandle,
                                    Entity & caller)
{
    if (modelHandle.typeHash() != HASH::model)
        PANIC("Invalid model handle");

    messages::InsertModelInstanceQW msgQW(HASH::renderer_insert_model_instance,
                                          kMessageFlag_None,
                                          caller.task().id(),
                                          kRendererTaskId,
                                          uid);
    msgQW.setIsAssetManaged(true);
    msgQW.setModel((Model*)modelHandle.data());
    msgQW.setTransform(caller.transform());
}

void renderer_transform_model_instance(u32  uid, const Mat34 & transform, Entity & caller)
{
    messages::TransformIdQW msgQW(HASH::renderer_transform_model_instance,
                                  kMessageFlag_None,
                                  caller.task().id(),
                                  kRendererTaskId,
                                  uid);
    msgQW.setTransform(transform);
}

void renderer_remove_model_instance(u32  uid, Entity & caller)
{
    MessageQueueWriter msgQW(HASH::renderer_remove_model_instance,
                             kMessageFlag_None,
                             caller.task().id(),
                             kRendererTaskId,
                             to_cell(uid),
                             0);
}

void renderer_insert_light_directional(u32  uid, const Vec3 & direction, Color color, Entity & caller)
{
    messages::InsertLightDirectionalQW msgQW(HASH::renderer_insert_light_directional,
                                             kMessageFlag_None,
                                             caller.task().id(),
                                             kRendererTaskId,
                                             uid);
    msgQW.setDirection(direction);
    msgQW.setColor(color);
}

void renderer_remove_light_directional(u32  uid, Entity & caller)
{
    MessageQueueWriter msgQW(HASH::renderer_remove_light_directional,
                             kMessageFlag_None,
                             caller.task().id(),
                             kRendererTaskId,
                             to_cell(uid),
                             0);
}



} // namespace system_api

} // namespace gaen
