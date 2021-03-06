//------------------------------------------------------------------------------
// matrices.cpp - Common matrix routines
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

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "math/common.h"
#include "math/matrices.h"

namespace gaen
{

vec3 extract_rotate(const mat43 & t)
{
    mat4 t4(t);

    vec3 euler;

    glm::extractEulerAngleXYZ<f32>(t4, euler.x, euler.y, euler.z);

    return euler;
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
