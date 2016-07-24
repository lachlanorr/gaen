//------------------------------------------------------------------------------
// system_api.cpp - C functionas available to Compose scripts
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
#include "hashes/hashes.h"
#include "engine/glm_ext.h"
#include "engine/shapes.h"
#include "engine/InputMgr.h"
#include "engine/MessageWriter.h"
#include "engine/messages/InsertModelInstance.h"
#include "engine/messages/TransformUid.h"
#include "engine/messages/InsertLightDirectional.h"
#include "engine/messages/UpdateLightDirectional.h"
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

void print_asset_info(AssetHandleP asset, Entity & caller)
{
    LOG_INFO("print_asset_info");
}

//void activate_entity(u32 id, Entity & caller)
//{
//    caller.activateEntity(id);
//}

f32 radians(f32 degrees, Entity & caller)
{
    return glm::radians(degrees);
}

f32 degrees(f32 radians, Entity & caller)
{
    return glm::degrees(radians);
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

glm::mat4x3 mat43_rotation(const glm::vec3 & angles, Entity & caller)
{
    return glm::mat43_rotation(angles);
}

glm::mat3 mat3_rotation(const glm::vec3 & angles, Entity & caller)
{
    return glm::mat3_rotation(angles);
}

glm::quat quat_from_axis_angle(const glm::vec3 & dir, f32 angle, Entity & caller)
{
    return glm::quat(angle, dir);
}

glm::quat quat_normalize(const glm::quat & quat, Entity & caller)
{
    glm::quat q = quat;
    glm::normalize(q);
    return q;
}

u32 renderer_gen_uid(Entity & caller)
{
    static std::atomic<u32> sNextUid(1);
    return sNextUid.fetch_add(1, std::memory_order_relaxed);
}

void renderer_move_camera(const glm::vec3 & position, const glm::quat & direction, Entity & caller)
{
    messages::MoveCameraQW msgQW(HASH::renderer_move_camera,
                                 kMessageFlag_None,
                                 caller.task().id(),
                                 kRendererTaskId);

    msgQW.setPosition(position);
    msgQW.setDirection(direction);
}

void renderer_insert_model_instance(u32 uid,
                                    HandleP modelHandle,
                                    Entity & caller)
{
    if (modelHandle->typeHash() != HASH::model)
        PANIC("Invalid model handle");

    messages::InsertModelInstanceQW msgQW(HASH::renderer_insert_model_instance,
                                          kMessageFlag_None,
                                          caller.task().id(),
                                          kRendererTaskId,
                                          uid);
    msgQW.setIsAssetManaged(true);
    msgQW.setModel((Model*)modelHandle->data());
    msgQW.setTransform(caller.transform());

    // Renderer now owns the model
    modelHandle->clearData();
}

void renderer_transform_model_instance(u32  uid, const glm::mat4x3 & transform, Entity & caller)
{
    messages::TransformUidQW msgQW(HASH::renderer_transform_model_instance,
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

void renderer_insert_light_directional(u32 uid,
                                       const glm::vec3 & direction,
                                       Color color,
                                       Entity & caller)
{
    messages::InsertLightDirectionalQW msgQW(HASH::renderer_insert_light_directional,
                                             kMessageFlag_None,
                                             caller.task().id(),
                                             kRendererTaskId,
                                             uid);
    msgQW.setDirection(direction);
    msgQW.setColor(color);
}

void renderer_update_light_directional(u32 uid,
                                       const glm::vec3 & direction,
                                       Color color,
                                       Entity & caller)
{
    messages::UpdateLightDirectionalQW msgQW(HASH::renderer_update_light_directional,
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
