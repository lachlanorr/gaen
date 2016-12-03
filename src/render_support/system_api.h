//------------------------------------------------------------------------------
// system_api.h - C functions available to Compose scripts
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

#ifndef GAEN_RENDER_SUPPORT_SYSTEM_API_H
#define GAEN_RENDER_SUPPORT_SYSTEM_API_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x3.hpp>

namespace gaen
{

// This file gets parsed by codegen.py to create system_api_meta.cpp, which
// is used by Compose to compile type safe system calls.

// All parameters must be references or const references.

namespace system_api
{

i32 gen_uid(Entity & caller);

void camera_move(i32 uid,
                 const glm::vec3 & position,
                 const glm::quat & direction,
                 Entity & caller);

glm::mat4x3 view_look_at(const glm::vec3 & position,
                         const glm::vec3 & target,
                         Entity & caller);

void light_distant_insert(i32 uid,
                          const glm::vec3 & direction,
                          Color color,
                          Entity & caller);

void light_distant_direction(i32 uid,
                             const glm::vec3 & direction,
                             Entity & caller);

void light_distant_color(i32 uid,
                         Color color,
                         Entity & caller);

void light_distant_remove(i32 uid, Entity & caller);

} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_SYSTEM_API_H
