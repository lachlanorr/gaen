//------------------------------------------------------------------------------
// system_api.h - C functions available to Compose scripts
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

#ifndef GAEN_MATH_SYSTEM_API_H
#define GAEN_MATH_SYSTEM_API_H

namespace gaen
{

namespace system_api
{

f32 radians(f32 degrees, Entity & caller);
f32 degrees(f32 radians, Entity & caller);

vec2 normalize(const vec2 & vec2, Entity & caller);
vec3 normalize(const vec3 & vec3, Entity & caller);
vec4 normalize(const vec4 & vec4, Entity & caller);
quat normalize(const quat & quat, Entity & caller);

} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_MATH_SYSTEM_API_H
