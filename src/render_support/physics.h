//------------------------------------------------------------------------------
// physics.h - Physics helpers to integrate with bullet
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

#ifndef GAEN_RENDER_SUPPORT_PHYSICS_H
#define GAEN_RENDER_SUPPORT_PHYSICS_H

#include "math/vec3.h"
#include "math/mat43.h"

namespace gaen
{

class Gmdl;

u32 collision_box_create(task_id owner,
                         const vec3 & halfExtents,
                         const mat43 & transform,
                         u32 message,
                         u32 group,
                         const ivec4 & mask03);

u32 collision_convex_hull_create(task_id owner,
                                 const Gmdl * pGmdlPoints,
                                 const mat43 & transform,
                                 u32 message,
                                 u32 group,
                                 const ivec4 & mask03);

void collision_remove_body(task_id owner, u32 uid);

namespace system_api
{
static const i32 PHY_FLAG_NONE            = 0;
static const i32 PHY_FLAG_KINEMATIC       = 1;
static const i32 PHY_FLAG_STOP_ON_COLLIDE = 2;

// Collision shape type
static const i32 PHY_SHAPE_BOX = 0;
static const i32 PHY_SHAPE_CAPSULE = 1;

} // namespace system_api

}

#endif // #ifndef GAEN_RENDER_SUPPORT_PHYSICS_H
