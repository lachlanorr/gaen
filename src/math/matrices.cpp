//------------------------------------------------------------------------------
// matrices.cpp - Common matrix routines
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

#include "math/stdafx.h"

#include <glm/gtc/matrix_transform.hpp>

#include "math/common.h"
#include "math/matrices.h"

namespace gaen
{

vec3 euler(const mat43 & t)
{
    vec3 rot{ 0.0f };

    if (t[0][1] == 1.0f)
    {
        rot.y = atan2(t[2][0], t[2][2]);
    }
    else if (t[0][1] == -1.0f)
    {
        rot.y = atan2(t[2][0], t[2][2]);
    }
    else
    {
        rot.x = asin(t[0][1]);
        rot.y = atan2(-t[0][2], t[0][0]);
        rot.z = atan2(-t[2][1], t[1][1]);
    }

    return rot;
}

vec3 eulerOld(const mat43 & transform)
{
    vec3 rot;

    rot.y = asin(transform[2][0]);  // Calculate Y-axis angle
    f32 c = cos(rot.y);
    if (abs(c) > 0.005) // No Gimball lock
    {
        rot.x = atan2(-transform[2][1] / c, transform[2][2] / c);
        rot.z = atan2(-transform[1][0] / c, transform[0][0] / c);
    }
    else // Gimball lock
    {
        rot.x = 0;
        rot.z  = atan2(transform[0][1], transform[1][1]);
    }

    // return only positive angles in [0,2pi]
    if (rot.x < 0) rot.x += k2Pi;
    if (rot.y < 0) rot.y += k2Pi;
    if (rot.z < 0) rot.z += k2Pi;

    return rot;
}


mat4 perspective(f32 fovy,
                 f32 aspect,
                 f32 near,
                 f32 far)
{
    return mat4(glm::perspective(fovy, aspect, near, far));
}

mat4 ortho(f32 left,
           f32 right,
           f32 bottom,
           f32 top,
           f32 zNear,
           f32 zFar)
{
    return mat4(glm::ortho(left, right, bottom, top, zNear, zFar));
}

mat4 ortho(f32 left,
           f32 right,
           f32 bottom,
           f32 top)
{
    return mat4(glm::ortho(left, right, bottom, top));
}

mat43 look_at(const vec3 & eye,
              const vec3 & center,
              const vec3 & up)
{
    return mat43(mat4(glm::lookAt((vec3::glm_t)eye, (vec3::glm_t)center, (vec3::glm_t)up)));
}


} // namespace gaen
