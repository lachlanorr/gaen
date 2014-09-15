//------------------------------------------------------------------------------
// renderer_structs.h - Structures shared by renderers and the engine
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

#ifndef GAEN_ENGINE_RENDERER_STRUCTS_H
#define GAEN_ENGINE_RENDERER_STRUCTS_H

#include "core/base_defines.h"

#include "engine/task.h"
#include "engine/Color.h"
#include "engine/math.h"

namespace gaen
{

struct Ruid
{
    task_id owner;
    u32 uid;

    Ruid(task_id owner, u32 uid)
      : owner(owner)
      , uid(uid)
    {}
};

struct DistantLight
{
    Ruid ruid;
    Vec3 direction;
    Vec4 color;

    DistantLight(task_id owner, u32 uid, const Vec3& direction, Color color)
      : ruid(owner, uid)
      , direction(direction)
      , color(Color::build_vec4(color))
    {}
};

struct PointLight
{
    Ruid ruid;
    Vec3 location;
    Vec4 color;

    PointLight(task_id owner, u32 uid, const Vec3 & location, Color color)
      : ruid(owner, uid)
      , location(location)
      , color(Color::build_vec4(color))
    {}
};



} // namespace gaen


#endif // #ifndef GAEN_ENGINE_RENDERER_STRUCTS_H
