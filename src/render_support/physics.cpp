//------------------------------------------------------------------------------
// physics.cpp - Physics helpers to integrate with bullet
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

#include "render_support/stdafx.h"

#include <LinearMath/btIDebugDraw.h>

#include "engine/UniqueObject.h"
#include "engine/messages/CollisionBox.h"
#include "engine/messages/CollisionConvexHull.h"

#include "render_support/physics.h"

namespace gaen
{

u32 collision_box_create(task_id owner,
                         const vec3 & halfExtents,
                         const mat43 & transform,
                         u32 message,
                         u32 group,
                         const ivec4 & mask03)
{
    u32 uid = UniqueObject::next_uid();
    messages::CollisionBoxIW msgw(HASH::collision_box_create, kMessageFlag_None, owner, kModelMgrTaskId, uid);

    msgw.setCenter(vec3(0.0f));
    msgw.setHalfExtents(halfExtents);
    msgw.setTransform(transform);
    msgw.setMass(0.0f);
    msgw.setFriction(0.5f);
    msgw.setLinearFactor(vec3(0.0f));
    msgw.setAngularFactor(vec3(0.0f));
    msgw.setMessage(message);
    msgw.setGroup(group);
    msgw.setMask03(mask03);
    msgw.setMask47(ivec4(0));

    return uid;
}

u32 collision_convex_hull_create(task_id owner,
                                 const Gmdl * pGmdlPoints,
                                 const mat43 & transform,
                                 u32 message,
                                 u32 group,
                                 const ivec4 & mask03)
{
    u32 uid = UniqueObject::next_uid();

    messages::CollisionConvexHullIW msgw(HASH::collision_convex_hull_create, kMessageFlag_None, owner, kModelMgrTaskId, uid);

    msgw.setGmdlPoints(pGmdlPoints);
    msgw.setTransform(transform);
    msgw.setMass(0.0f);
    msgw.setFriction(0.5f);
    msgw.setMessage(message);
    msgw.setGroup(group);
    msgw.setMask03(mask03);
    msgw.setMask47(ivec4(0));

    return uid;
}

void collision_remove_body(task_id owner, u32 uid)
{
    ImmediateMessageWriter<0> msgw(HASH::remove_body, kMessageFlag_None, owner, kModelMgrTaskId, to_cell(uid));
}


}
