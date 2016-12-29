//------------------------------------------------------------------------------
// compose_funcs.h - Built in compose scripts
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

#ifndef GAEN_ENGINE_COMPOSE_FUNCS_H
#define GAEN_ENGINE_COMPOSE_FUNCS_H

#include "math/common.h"
#include "math/matrices.h"

#include "engine/Handle.h"
#include "engine/Entity.h"
#include "engine/BlockMemory.h"
#include "engine/TaskMaster.h"

namespace gaen
{

// This file gets parsed by codegen.py to create system_api_meta.cpp, which
// is used by Compose to compile type safe system calls.

// All parameters must be references or const references.

namespace compose_funcs
{

inline void print(CmpString str, Entity * pCaller)
{
    LOG_INFO(str.c_str());
}

inline f32 random(Entity * pCaller)
{
    return TaskMaster::task_master_for_active_thread().rand();
}

inline CmpString hashstr(i32 hash, Entity * pCaller)
{
    return pCaller->blockMemory().stringAlloc(HASH::reverse_hash(hash));
}

inline f32 radians(f32 degrees, Entity * pCaller)
{
    return gaen::radians(degrees);
}

inline f32 degrees(f32 radians, Entity * pCaller)
{
    return gaen::degrees(radians);
}

inline vec3 position(const mat43 & transform, Entity * pCaller)
{
    return gaen::position(transform);
}

inline vec2 normalize(const vec2 & v, Entity * pCaller)
{
    return gaen::normalize(v);
}

inline vec3 normalize(const vec3 & v, Entity * pCaller)
{
    return gaen::normalize(v);
}

inline vec4 normalize(const vec4 & v, Entity * pCaller)
{
    return gaen::normalize(v);
}

inline quat normalize(const quat & q, Entity * pCaller)
{
    return gaen::normalize(q);
}

} // namespace compose_funcs

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_COMPOSE_FUNCS_H
