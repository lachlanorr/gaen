//------------------------------------------------------------------------------
// physics.h - Physics helpers to integrate with bullet
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

#ifndef GAEN_RENDER_SUPPORT_PHYSICS_H
#define GAEN_RENDER_SUPPORT_PHYSICS_H

#include "math/vec3.h"
#include "math/mat43.h"

namespace gaen
{

class CollisionBox
{
public:
    CollisionBox(u32 uid, const vec3 & halfExtents, const mat43 & transform);

    u32 uid() { return mUid; }
    const vec3 & halfExtents() { return mHalfExtents; }
    const mat43 & transform() { return mTransform; }
private:
    u32 mUid;
    vec3 mHalfExtents;
    mat43 mTransform;
};

u32 collision_box_create(task_id owner,
                         const vec3 & halfExtents,
                         const mat43 & transform,
                         u32 group,
                         const ivec4 & mask03);

}

#endif // #ifndef GAEN_RENDER_SUPPORT_PHYSICS_H
