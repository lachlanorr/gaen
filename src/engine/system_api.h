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

#ifndef GAEN_COMPOSE_SYSTEM_API_H
#define GAEN_COMPOSE_SYSTEM_API_H

#include "engine/Handle.h"
#include "engine/Model.h"
#include "engine/Entity.h"

namespace gaen
{

// This file gets parsed by codegen.py to create system_api_meta.cpp, which
// is used by Compose to compile type safe system calls.

// All parameters must be references or const references.

namespace system_api
{

f32 radians(const f32 & degrees, const Entity & caller);
f32 degrees(const f32 & radians, const Entity & caller);

Handle create_shape_box(const Vec3 & size, const Color & color, const Entity & caller);

void watch_input_state(const u32 & state, const u32 & deviceId, const u32 & message, const Entity & caller);

Mat34 transform_rotate(const Vec3 & angles, const Entity & caller);

u32 renderer_gen_uid(const Entity & caller);
void renderer_insert_model_instance(const u32 & uid,
                                    Handle & modelHandle,
                                    const Mat34 & transform,
                                    const Entity & caller);
void renderer_transform_model_instance(const u32 & uid, const Mat34 & transform, const Entity & caller);
void renderer_remove_model_instance(const u32 & uid, const Entity & caller);

void renderer_insert_light_directional(const u32 & uid,
                                       const Vec3 & direction,
                                       const Color & color,
                                       const Entity & caller);
void renderer_remove_light_directional(const u32 & uid, const Entity & caller);

}

} // namespace gaen

#endif // #ifndef GAEN_COMPOSE_SYSTEM_API_H
