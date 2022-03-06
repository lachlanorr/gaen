//------------------------------------------------------------------------------
// quat.h - 3d vector
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

#ifndef GAEN_MATH_QUAT_H
#define GAEN_MATH_QUAT_H

#include <glm/gtx/quaternion.hpp>

#include "gaen/core/base_defines.h"

namespace gaen
{

template <typename T>
struct tquat
{
    // glm conversions
    typedef glm::tquat<T, glm::highp> glm_t;
    explicit tquat(const glm_t & rhs)
      : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
    {}
    operator glm_t() const { return *reinterpret_cast<const glm_t*>(this); }

    union
    {
        struct { T x, y, z, w;};
    };

    tquat() = default;
	tquat(T angle, tvec3<T> const & axis);

	tquat(T w, T x, T y, T z)
      : w(w), x(x), y(y), z(z)
    {}

}; // class tquat

typedef tquat<f32> quat;

static_assert(sizeof(quat) == sizeof(glm::tquat<f32, glm::highp>), "quat differs in size from glm::quat");

template <typename T>
tquat<T>::tquat(T angle, tvec3<T> const & axis)
{
    tvec3<T> axisN = normalize(axis);
    T sin_a = sin(angle / 2);
    T cos_a = cos(angle / 2);
    x = axisN.x * sin_a;
    y = axisN.y * sin_a;
    z = axisN.z * sin_a;
    w = cos_a;
}

template <typename T>
inline tquat<T> operator-(const tquat<T> & rhs)
{
    return tquat<T>(glm::conjugate((tquat<T>::glm_t)rhs));
}

template <typename T>
inline tquat<T> operator~(const tquat<T> & rhs)
{
    return tquat<T>(glm::inverse((tquat<T>::glm_t)rhs));
}

template <typename T>
inline tvec3<T> operator*(const tquat<T> & lhs, const tvec3<T> & rhs)
{
    return tvec3<T>((tquat<T>::glm_t)lhs * (tvec3<T>::glm_t)rhs);
}

template <typename T>
inline tquat<T> operator*(const tquat<T> & lhs, const tquat<T> & rhs)
{
    return tquat<T>((tquat<T>::glm_t)lhs * (tquat<T>::glm_t)rhs);
}

template <typename T>
inline tquat<T> slerp(const tquat<T> & x, const tquat<T> & y, T a)
{
    tquat<T> ret = tquat<T>(glm::slerp((tquat<T>::glm_t)x, (tquat<T>::glm_t)y, a));
    return ret;
}


template<typename T>
inline T* value_ptr(tquat<T> & x)
{
    return reinterpret_cast<T*>(&x);
}

template<typename T>
inline const T* value_ptr(const tquat<T> & x)
{
    return reinterpret_cast<const T*>(&x);
}


template <typename T>
inline tquat<T> normalize(const tquat<T> & q)
{
    return tquat<T>(glm::normalize((tquat<T>::glm_t)q));
}

} // namespace gaen

#endif // #ifndef GAEN_MATH_VEC4_H
