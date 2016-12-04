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

i32 gen_uid(Entity & caller)
{
    return RenderObject::next_uid();
}

void camera_move(i32 uid,
                 const glm::vec3 & position,
                 const glm::quat & direction,
                 Entity & caller)
{
    messages::UidTransformQW msgQW(HASH::camera_move,
                                   kMessageFlag_None,
                                   caller.task().id(),
                                   kRendererTaskId,
                                   uid);

    glm::mat4x4 trans = glm::translate(glm::mat4(1.0), position) * glm::mat4_cast(direction);
    msgQW.setTransform(trans);
}

glm::mat4x3 view_look_at(const glm::vec3 & position,
                         const glm::vec3 & target,
                         Entity & caller)
{
    static glm::vec3 up(0.0f, 1.0f, 0.0f);
    return glm::lookAt(position, target, up);
}

void light_distant_insert(i32 uid,
                          i32 stageHash,
                          Color color,
                          f32 ambient,
                          const glm::vec3 & direction,
                          Entity & caller)
{
    messages::LightDistantQW msgQW(HASH::light_distant_insert,
                                   kMessageFlag_None,
                                   caller.task().id(),
                                   kRendererTaskId,
                                   uid);
    msgQW.setStageHash(stageHash);
    msgQW.setColor(color);
    msgQW.setAmbient(ambient);
    msgQW.setDirection(direction);
}

void light_distant_direction(i32 uid,
                             const glm::vec3 & direction,
                             Entity & caller)
{
    messages::UidVec3QW msgQW(HASH::light_distant_update,
                                   kMessageFlag_None,
                                   caller.task().id(),
                                   kRendererTaskId,
                                   uid);
    msgQW.setVector(direction);
}

void light_distant_color(i32 uid,
                         Color color,
                         Entity & caller)
{
    messages::UidColorQW msgQW(HASH::light_distant_update,
                               kMessageFlag_None,
                               caller.task().id(),
                               kRendererTaskId,
                               uid);
    msgQW.setColor(color);
}

void light_distant_ambient(i32 uid,
                             f32 ambient,
                             Entity & caller)
{
    messages::UidScalarQW msgQW(HASH::light_distant_update,
                                kMessageFlag_None,
                                caller.task().id(),
                                kRendererTaskId,
                                uid);
    msgQW.setScalar(ambient);
}

void light_distant_remove(i32 uid, Entity & caller)
{
    MessageQueueWriter msgQW(HASH::light_distant_remove,
                             kMessageFlag_None,
                             caller.task().id(),
                             kRendererTaskId,
                             to_cell(uid),
                             0);
}

} // namespace system_api

} // namespace gaen
