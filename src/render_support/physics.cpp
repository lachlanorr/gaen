//------------------------------------------------------------------------------
// physics.cpp - Physics helpers to integrate with bullet
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

#include "engine/messages/CollisionBox.h"

#include "render_support/render_objects.h"
#include "render_support/physics.h"

namespace gaen
{

CollisionBox::CollisionBox(u32 uid, const vec3 & halfExtents, const mat43 & transform)
  : mUid(uid)
  , mHalfExtents(halfExtents)
  , mTransform(transform)
{}

u32 collision_box_create(const vec3 & halfExtents, const mat43 & transform, task_id source)
{
    u32 uid = RenderObject::next_uid();
    messages::CollisionBoxBW msgw(HASH::collision_box_create, kMessageFlag_None, source, kModelMgrTaskId, uid);
    msgw.setHalfExtents(halfExtents);
    msgw.setTransform(transform);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
    return uid;
}

}