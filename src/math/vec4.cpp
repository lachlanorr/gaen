//------------------------------------------------------------------------------
// vec4.cpp - 4d vector
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

#include "math/stdafx.h"

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"

namespace gaen
{

template <typename T>
tvec4<T>::tvec4(const tvec3<T> & v, T w)
  : x(v.x), y(v.y), z(v.z), w(w)
{}

// Template definitions for linker
template struct tvec4<f32>;
template struct tvec4<u32>;
template struct tvec4<i32>;

} // namespace gaen
