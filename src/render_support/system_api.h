//------------------------------------------------------------------------------
// system_api.h - C functions available to Compose scripts
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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

#ifndef GAEN_RENDER_SUPPORT_SYSTEM_API_H
#define GAEN_RENDER_SUPPORT_SYSTEM_API_H

#include "math/vec3.h"
#include "math/quat.h"
#include "math/mat3.h"
#include "math/mat43.h"

namespace gaen
{

// This file gets parsed by codegen.py to create system_api_meta.cpp, which
// is used by Compose to compile type safe system calls.

// All parameters must be references or const references.

namespace system_api
{

i32 gen_uid(Entity * pCaller);

void camera_move(i32 uid,
                 const vec3 & position,
                 const quat & direction,
                 Entity * pCaller);

mat43 view_look_at(const vec3 & position,
                   const vec3 & target,
                   const vec3 & up,
                   Entity * pCaller);

void light_insert(i32 uid,
                  i32 stageHash,
                  Color color,
                  f32 ambient,
                  const vec3 & direction,
                  Entity * pCaller);

void light_direction(i32 uid,
                     const vec3 & direction,
                     Entity * pCaller);

void light_color(i32 uid,
                 Color color,
                 Entity * pCaller);

void light_ambient(i32 uid,
                   f32 ambient,
                   Entity * pCaller);

void light_remove(i32 uid, Entity * pCaller);

} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_SYSTEM_API_H
