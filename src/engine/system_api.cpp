//------------------------------------------------------------------------------
// system_api.h - C functionas available to Compose scripts
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
#include "engine/stdafx.h"

#include "core/mem.h"
#include "engine/hashes.h"
#include "engine/math.h"
#include "engine/shapes.h"
#include "engine/InputMgr.h"
#include "engine/MessageWriter.h"
#include "engine/messages/InsertModelInstance.h"
#include "engine/messages/Transform.h"
#include "engine/messages/InsertLightDistant.h"
#include "engine/messages/RegisterInputStateListener.h"

#include "engine/system_api.h"

namespace gaen
{

//------------------------------------------------------------------------------
// API Defs start here
//------------------------------------------------------------------------------

namespace system_api
{

f32 radians(const f32 & degrees, const Entity & caller)
{
    return ::gaen::radians(degrees);
}

f32 degrees(const f32 & radians, const Entity & caller)
{
    return ::gaen::degrees(radians);
}


Handle create_shape_box(const Vec3 & size, const Color & color, const Entity & caller)
{
    Model * pModel = build_box(size, color);

    return Handle(HASH::Model, 0, 0, sizeof(Model), pModel, nullptr);
}

void register_input_state_listener(const u32 & state, const u32 & deviceId, const u32 & message, const Entity & caller)
{
    msg::RegisterInputStateListenerQW msgQW(HASH::register_input_state_listener,
                                            kMessageFlag_None,
                                            caller.task().id(),
                                            kInputMgrTaskId,
                                            state);
    msgQW.setDeviceId(deviceId);
    msgQW.setMessage(message);
}

Mat34 transform_rotate(const Vec3 & angles, const Entity & caller)
{
    return Mat34::rotation(angles);
}

u32 renderer_gen_uid(const Entity & caller)
{
    static std::atomic<u32> sNextUid(1);
    return sNextUid.fetch_add(1, std::memory_order_relaxed);
}
void renderer_insert_model_instance(const u32 & uid,
                                    Handle & modelHandle,
                                    const Mat34 & transform,
                                    const Entity & caller)
{
    if (modelHandle.typeHash() != HASH::Model)
        PANIC("Invalid model handle");

    msg::InsertModelInstanceQW msgQW(HASH::renderer_insert_model_instance,
                                     kMessageFlag_None,
                                     caller.task().id(),
                                     kRendererTaskId,
                                     uid);
    msgQW.setIsAssetManaged(true);
    msgQW.setModel((Model*)modelHandle.data());
    msgQW.setWorldTransform(transform);
}

void renderer_transform_model_instance(const u32 & uid, const Mat34 & transform, const Entity & caller)
{
    msg::TransformQW msgQW(HASH::renderer_transform_model_instance,
                           kMessageFlag_None,
                           caller.task().id(),
                           kRendererTaskId,
                           uid);
    msgQW.setTransform(transform);
}

void renderer_remove_model_instance(const u32 & uid, const Entity & caller)
{
    MessageQueueWriter msgQW(HASH::renderer_remove_model_instance,
                             kMessageFlag_None,
                             caller.task().id(),
                             kRendererTaskId,
                             to_cell(uid),
                             0);
}

void renderer_insert_light_distant(const u32 & uid, const Vec3 & direction, const Color & color, const Entity & caller)
{
    msg::InsertLightDistantQW msgQW(HASH::renderer_insert_light_distant,
                                    kMessageFlag_None,
                                    caller.task().id(),
                                    kRendererTaskId,
                                    uid);
    msgQW.setDirection(direction);
    msgQW.setColor(color);
}

void renderer_remove_light_distant(const u32 & uid, const Entity & caller)
{
    MessageQueueWriter msgQW(HASH::renderer_remove_light_distant,
                             kMessageFlag_None,
                             caller.task().id(),
                             kRendererTaskId,
                             to_cell(uid),
                             0);
}



} // namespace system_api

} // namespace gaen
