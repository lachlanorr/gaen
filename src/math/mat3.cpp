//------------------------------------------------------------------------------
// mat3.cpp - 3x3 f32 matrix
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

#include "math/matrices.h"

#include "math/mat43.h"
#include "math/mat4.h"
#include "math/mat3.h"

namespace gaen
{


template <typename T>
tmat3<T>::tmat3(const tvec3<T> & rot)
{
    *this = rotation_from_euler<tmat3<T>>(rot.x, rot.y, rot.z);
}

template <typename T>
tmat3<T>::tmat3(const tmat3a<T> & rhs)
{
    cols[0] = rhs[0];
    cols[1] = rhs[1];
    cols[2] = rhs[2];
}

template <typename T>
tmat3<T>::tmat3(const tmat43<T> & rhs)
{
    *this = rhs;
}

template <typename T>
tmat3<T>::tmat3(const tmat4<T> & rhs)
{
    *this = rhs;
}

template <typename T>
tmat3<T> tmat3<T>::from_rot(T x, T y, T z)
{
    return rotation_from_euler<tmat3<T>>(x, y, z);
}

template <typename T>
tmat3<T> tmat3<T>::from_rot(const tvec3<T> & v)
{
    return rotation_from_euler<tmat3<T>>(v.x, v.y, v.z);
}

template <typename T>
tmat3<T> & tmat3<T>::operator=(const tmat43<T> & rhs)
{
    cols[0] = rhs[0];
    cols[1] = rhs[1];
    cols[2] = rhs[2];
    return *this;
}

template <typename T>
tmat3<T> & tmat3<T>::operator=(const tmat4<T> & rhs)
{
    cols[0] = tvec3<T>(rhs[0]);
    cols[1] = tvec3<T>(rhs[1]);
    cols[2] = tvec3<T>(rhs[2]);
    return *this;
}

template <typename T>
tmat3<T> tmat3<T>::operator~() const
{
    return tmat3<T>(glm::inverse((tmat3<T>::glm_t)*this));
}

template <typename T>
tmat3a<T>::tmat3a(const tmat43<T> & rhs)
{
    cols[0] = rhs[0];
    cols[1] = rhs[1];
    cols[2] = rhs[2];
}

// Template definitions for linker
template struct tmat3<f32>;
template struct tmat3a<f32>;

} // namespace gaen
