//------------------------------------------------------------------------------
// mat43.cpp - 4x3 f32 matrix, typically used for concise transforms
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

#include "math/matrices.h"

#include "math/mat3.h"
#include "math/mat4.h"
#include "math/mat43.h"

namespace gaen
{

template <typename T>
tmat43<T>::tmat43(const tvec3<T> & pos)
  : tmat43<T>(1)
{
    cols[3] = pos;
}

template <typename T>
tmat43<T>::tmat43(const tquat<T> & q)
{
    *this = tmat43<T>(tmat4<T>(glm::mat4_cast((tquat<T>::glm_t)q)));
}

template <typename T>
tmat43<T>::tmat43(const tvec3<T> & pos, const tvec3<T> & rot)
{
    *this = rotation_from_euler<tmat43<T>>(rot.x, rot.y, rot.z);
    cols[3] = pos;
}

template <typename T>
tmat43<T>::tmat43(const tvec3<T> & pos, const tmat3<T> & rot)
{
    *this = rot;
    cols[3] = pos;
}

template <typename T>
tmat43<T>::tmat43(const tmat4<T> & rhs)
{
    *this = rhs;
}

template <typename T>
tmat43<T>::tmat43(const tmat3<T> & rhs)
{
    *this = rhs;
}

template <typename T>
tmat43<T> tmat43<T>::from_pos(T x, T y, T z)
{
    return tmat43<T>(tvec3<T>(x, y, z));
}

template <typename T>
tmat43<T> tmat43<T>::from_rot(T x, T y, T z)
{
    return rotation_from_euler<tmat43<T>>(x, y, z);
}

template <typename T>
tmat43<T> tmat43<T>::from_scale(T scale)
{
    tmat43<T> m(1);
    m[0][0] = m[1][1] = m[2][2] = scale;
    return m;
}

template <typename T>
tmat43<T> & tmat43<T>::operator=(const tmat4<T> & rhs)
{
    cols[0] = tvec3<T>(rhs[0]);
    cols[1] = tvec3<T>(rhs[1]);
    cols[2] = tvec3<T>(rhs[2]);
    cols[3] = tvec3<T>(rhs[3]);
    return *this;
}

template <typename T>
tmat43<T> & tmat43<T>::operator=(const tmat3<T> & rhs)
{
    cols[0] = rhs[0];
    cols[1] = rhs[1];
    cols[2] = rhs[2];
    cols[3] = tvec3<T>(0, 0, 0);
    return *this;
}

template <typename T>
tmat43<T> tmat43<T>::operator~() const
{
    return tmat43<T>(glm::inverse((tmat4<T>::glm_t)*this));
}

// Template definitions for linker
template tmat43<f32>;

} // namespace gaen
