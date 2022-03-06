//------------------------------------------------------------------------------
// matrices.h - Common matrix routines
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

#ifndef GAEN_MATH_MATRICES_H
#define GAEN_MATH_MATRICES_H

#include <cmath>

#include "gaen/core/base_defines.h"

#include "gaen/math/vec3.h"
#include "gaen/math/mat4.h"
#include "gaen/math/mat43.h"

namespace gaen
{

template<class T>
T rotation_from_euler(f32 x, f32 y, f32 z)
{
    T mat(1);

    f32 a   = cos(x);
    f32 b   = sin(x);
    f32 c   = cos(y);
    f32 d   = sin(y);
    f32 e   = cos(z);
    f32 f   = sin(z);
    f32 ad  =  a * d;
    f32 bd  =  b * d;

    mat[0][0]  =  c * e;
    mat[1][0]  = -c * f;
    mat[2][0]  =  d;

    mat[0][1]  =  bd * e + a * f;
    mat[1][1]  = -bd * f + a * e;
    mat[2][1]  = -b * c;

    mat[0][2]  = -ad * e + b * f;
    mat[1][2]  =  ad * f + b * e;
    mat[2][2]  =  a * c;

    return mat;
}

inline vec3 position(const mat43 & transform)
{
    return transform[3];
}

inline vec3 position(const mat4 & transform)
{
    return vec3(transform[3]);
}

inline void set_position(mat43 & transform, const vec3 & position)
{
    transform[3] = position;
}

inline void set_position(mat4 & transform, const vec3 & position)
{
    transform[3] = vec4(position, 0.0f);
}

vec3 extract_rotate(const mat43 & transform);

inline vec3 extract_translate(const mat43 & transform)
{
    return transform[3];
}

inline mat43 build_translate(const vec3 & v)
{
    return mat43::from_pos(v.x, v.y, v.z);
}
inline mat43 build_rotate(const vec3 & v)
{
    return mat43::from_rot(v.x, v.y, v.z);
}
inline mat43 build_rotate(const quat & q)
{
    return mat43(q);
}

mat4 perspective(f32 fovy,
                 f32 aspect,
                 f32 near,
                 f32 far);

mat4 ortho(f32 left,
           f32 right,
           f32 bottom,
           f32 top,
           f32 zNear,
           f32 zFar);

mat4 ortho(f32 left,
           f32 right,
           f32 bottom,
           f32 top);

mat43 look_at(const vec3 & eye,
              const vec3 & center,
              const vec3 & up);

} // namespace gaen

#endif // #ifndef GAEN_MATH_MATRICES_H
