//------------------------------------------------------------------------------
// system_api.cpp - C functions available to Compose scripts
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

#include "render_support/stdafx.h"

#include "math/matrices.h"

#include "engine/messages/UidTransform.h"
#include "engine/messages/UidColor.h"
#include "engine/messages/UidVec3.h"
#include "engine/messages/UidScalar.h"
#include "engine/messages/LightDistant.h"

#include "render_support/render_objects.h"

#include "render_support/system_api.h"

namespace gaen
{

//------------------------------------------------------------------------------
// API Defs start here
//------------------------------------------------------------------------------

namespace system_api
{

i32 gen_uid(Entity * pCaller)
{
    return RenderObject::next_uid();
}

void camera_move(i32 uid,
                 const vec3 & position,
                 const quat & direction,
                 Entity * pCaller)
{
    messages::UidTransformQW msgQW(HASH::camera_move,
                                   kMessageFlag_None,
                                   pCaller->task().id(),
                                   kRendererTaskId,
                                   uid);

    mat4 trans = mat4(position) * mat4(direction);
    msgQW.setTransform(mat43(trans));
}

mat43 view_look_at(const vec3 & position,
                   const vec3 & target,
                   const vec3 & up,
                   Entity * pCaller)
{
    return mat43(look_at(position, target, up));
}

void light_insert(i32 uid,
                  i32 stageHash,
                  Color color,
                  f32 ambient,
                  const vec3 & direction,
                  Entity * pCaller)
{
    messages::LightDistantQW msgQW(HASH::light_insert,
                                   kMessageFlag_None,
                                   pCaller->task().id(),
                                   kRendererTaskId,
                                   uid);
    msgQW.setStageHash(stageHash);
    msgQW.setColor(color);
    msgQW.setAmbient(ambient);
    msgQW.setDirection(direction);
}

void light_direction(i32 uid,
                     const vec3 & direction,
                     Entity * pCaller)
{
    messages::UidVec3QW msgQW(HASH::light_update,
                              kMessageFlag_None,
                              pCaller->task().id(),
                              kRendererTaskId,
                              uid);
    msgQW.setVector(direction);
}

void light_color(i32 uid,
                 Color color,
                 Entity * pCaller)
{
    messages::UidColorQW msgQW(HASH::light_update,
                               kMessageFlag_None,
                               pCaller->task().id(),
                               kRendererTaskId,
                               uid);
    msgQW.setColor(color);
}

void light_ambient(i32 uid,
                   f32 ambient,
                   Entity * pCaller)
{
    messages::UidScalarQW msgQW(HASH::light_update,
                                kMessageFlag_None,
                                pCaller->task().id(),
                                kRendererTaskId,
                                uid);
    msgQW.setScalar(ambient);
}

void light_remove(i32 uid, Entity * pCaller)
{
    MessageQueueWriter msgQW(HASH::light_remove,
                             kMessageFlag_None,
                             pCaller->task().id(),
                             kRendererTaskId,
                             to_cell(uid),
                             0);
}

} // namespace system_api

} // namespace gaen
