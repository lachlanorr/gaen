//------------------------------------------------------------------------------
// vec3.cpp - 3d f32 vector
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

#include "math/stdafx.h"

#include "math/vec2.h"
#include "math/vec4.h"
#include "math/vec3.h"
#include "math/mat43.h"

namespace gaen
{

template <typename T>
tvec3<T>::tvec3(const tmat43<T> & rhs)
{
    // extract position
    *this = rhs[3];
}

template <typename T>
tvec3<T>::tvec3(const tvec4<T> & v)
  : x(v.x), y(v.y), z(v.z)
{}

template <typename T>
tvec3<T>::tvec3(const tvec2<T> & v, T s)
  : x(v.x), y(v.y), z(s)
{}

template <typename T>
tvec3<T>::tvec3(T s, const tvec2<T> & v)
  : x(s), y(v.x), z(v.y)
{}

// Template definitions for linker
template struct tvec3<f32>;
template struct tvec3<u32>;
template struct tvec3<i32>;

} // namespace gaen

