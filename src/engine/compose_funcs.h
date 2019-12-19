//------------------------------------------------------------------------------
// compose_funcs.h - Built in compose scripts
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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

template <typename T>
inline T radians(T degrees, Entity * pCaller)
{
    return gaen::radians(degrees);
}

template <typename T>
inline T degrees(T radians, Entity * pCaller)
{
    return gaen::degrees(radians);
}

template <typename T>
inline T min(const T & x, const T & y, Entity * pCaller)
{
    return gaen::min(x, y);
}
template <typename T>
inline T max(const T & x, const T & y, Entity * pCaller)
{
    return gaen::max(x, y);
}

template <typename T>
inline T clamp(const T & x, const T & minVal, const T & maxVal, Entity * pCaller)
{
    return gaen::clamp(x, minVal, maxVal);
}

template <typename T>
inline T step(const T & edge, const T & x, Entity * pCaller)
{
    return gaen::step(edge, x);
}

template <typename T>
inline T smoothstep(const T & edge0, const T & edge1, const T & x, Entity * pCaller)
{
    return gaen::smoothstep(edge0, edge1, x);
}

template <typename T, typename U>
inline T lerp(const T & x, const T & y, const U & a, Entity * pCaller)
{
    return gaen::lerp(x, y, a);
}

template <typename T, typename U>
inline T slerp(const T & x, const T & y, const U & a, Entity * pCaller)
{
    return gaen::slerp(x, y, a);
}

inline vec3 position(const mat43 & transform, Entity * pCaller)
{
    return gaen::position(transform);
}

inline vec3 euler(const mat43 & transform, Entity * pCaller)
{
    return gaen::euler(transform);
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

inline f32 cross(const vec2 & lhs, const vec2 & rhs, Entity * pCaller)
{
    return gaen::cross(lhs, rhs);
}

inline vec3 cross(const vec3 & lhs, const vec3 & rhs, Entity * pCaller)
{
    return gaen::cross(lhs, rhs);
}

inline f32 dot(const vec2 & lhs, const vec2 & rhs, Entity * pCaller)
{
    return gaen::dot(lhs, rhs);
}

inline f32 dot(const vec3 & lhs, const vec3 & rhs, Entity * pCaller)
{
    return gaen::dot(lhs, rhs);
}

inline f32 length(const vec3 & v, Entity * pCaller)
{
    return gaen::length(v);
}

template <typename T>
inline T acos(T x, Entity * pCaller)
{
    return gaen::acos(x);
}

template <typename T>
inline T atan(T x, Entity * pCaller)
{
    return gaen::atan(x);
}



} // namespace compose_funcs

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_COMPOSE_FUNCS_H
