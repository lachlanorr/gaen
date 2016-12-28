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

#include "math/stdafx.h"

#include "math/common.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/quat.h"

#include "engine/Entity.h"

namespace gaen
{

namespace system_api
{

f32 radians(f32 degrees, Entity * pCaller)
{
    return gaen::radians(degrees);
}

f32 degrees(f32 radians, Entity * pCaller)
{
    return gaen::degrees(radians);
}

vec2 normalize(const vec2 & vec2, Entity * pCaller)
{
    return gaen::normalize(vec2);
}

vec3 normalize(const vec3 & vec3, Entity * pCaller)
{
    return gaen::normalize(vec3);
}

vec4 normalize(const vec4 & vec4, Entity * pCaller)
{
    return gaen::normalize(vec4);
}

quat normalize(const quat & quat, Entity * pCaller)
{
    return gaen::normalize(quat);
}

} // namespace system_api

} // namespace gaen

