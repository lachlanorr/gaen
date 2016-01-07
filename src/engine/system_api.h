//------------------------------------------------------------------------------
// system_api.h - C functionas available to Compose scripts
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

#ifndef GAEN_COMPOSE_SYSTEM_API_H
#define GAEN_COMPOSE_SYSTEM_API_H

#include "engine/Handle.h"
#include "engine/Model.h"
#include "engine/Entity.h"
#include "engine/BlockMemory.h"

namespace gaen
{

// This file gets parsed by codegen.py to create system_api_meta.cpp, which
// is used by Compose to compile type safe system calls.

// All parameters must be references or const references.

namespace system_api
{

void print(CmpString str, Entity & caller);
CmpString hashstr(u32 hash, Entity & caller);

void insert_entity(task_id id, Entity & caller);

f32 radians(f32 degrees, Entity & caller);
f32 degrees(f32 radians, Entity & caller);

void watch_input_state(u32 state, u32 deviceId, u32 message, Entity & caller);
void watch_mouse(u32 moveMessage, u32 wheelMessage, Entity & caller);

Mat34 mat34_rotation(const Vec3 & angles, Entity & caller);
Mat3 mat3_rotation(const Vec3 & angles, Entity & caller);

Quat quat_from_axis_angle(const Vec3 & dir, f32 angle, Entity & caller);
Quat quat_normalize(const Quat & quat, Entity & caller);

u32 renderer_gen_uid(Entity & caller);

void renderer_move_camera(const Vec3 & position, const Quat & direction, Entity & caller);
void renderer_move_fps_camera(const Vec3 & position, f32 pitch, f32 yaw, Entity & caller);

void renderer_insert_model_instance(u32 uid,
                                    Handle & modelHandle,
                                    Entity & caller);
void renderer_transform_model_instance(u32 uid, const Mat34 & transform, Entity & caller);
void renderer_remove_model_instance(u32 uid, Entity & caller);

void renderer_insert_light_directional(u32 uid,
                                       const Vec3 & direction,
                                       Color color,
                                       Entity & caller);

void renderer_update_light_directional(u32 uid,
                                       const Vec3 & direction,
                                       Color color,
                                       Entity & caller);

void renderer_remove_light_directional(u32 uid, Entity & caller);

}

} // namespace gaen

#endif // #ifndef GAEN_COMPOSE_SYSTEM_API_H
