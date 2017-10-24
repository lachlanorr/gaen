//------------------------------------------------------------------------------
// mat4.h - 4x4 matrix
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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

#ifndef GAEN_MATH_MAT4_H
#define GAEN_MATH_MAT4_H

#include <glm/mat4x4.hpp>

#include "core/base_defines.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/quat.h"

namespace gaen
{

template <typename T>
struct tmat3;

template <typename T>
struct tmat43;

template <typename T>
struct tmat4
{
    typedef glm::tmat4x4<T, glm::highp> glm_t;
    explicit tmat4(const glm_t & rhs)
    {
        *this = *reinterpret_cast<const tmat4*>(&rhs);
    }
    operator glm_t() const { return *reinterpret_cast<const glm_t*>(this); }

    tvec4<T> cols[4];

    tmat4() = default;
    tmat4(T x)
    {
        cols[0] = tvec4<T>(x, 0, 0, 0);
        cols[1] = tvec4<T>(0, x, 0, 0);
        cols[2] = tvec4<T>(0, 0, x, 0);
        cols[3] = tvec4<T>(0, 0, 0, x);
    }

    explicit tmat4(const tvec3<T> & pos);
    explicit tmat4(const tquat<T> & q);
    tmat4(const tvec3<T> & pos, const tvec3<T> & rot);
    static tmat4 from_pos(T x, T y, T z);
    static tmat4 from_rot(T x, T y, T z);
    static tmat4 from_scale(T scale);

    explicit tmat4(const tmat43<T> & rhs);
    explicit tmat4(const tmat3<T> & rhs);

    tvec4<T> & operator[](u32 i) { ASSERT(i < sizeof(cols) / sizeof(tvec4<T>)); return cols[i]; }
    const tvec4<T> & operator[](u32 i) const { ASSERT(i < sizeof(cols) / sizeof(tvec4<T>)); return cols[i]; }
    
    tmat4 & operator=(const tmat43<T> & rhs);
    tmat4 & operator=(const tmat3<T> & rhs);

    // inverse
    tmat4 operator~() const;

    const tvec3<T> pos() const { return tvec3<T>(cols[3]); }
};

template <typename T>
inline tvec4<T> operator*(const tmat4<T> & lhs, const tvec4<T> & rhs)
{
    return tvec4<T>((tmat4<T>::glm_t)lhs * (tvec4<T>::glm_t)rhs);
}

template <typename T>
inline tmat4<T> operator*(const tmat4<T> & lhs, const tmat4<T> & rhs)
{
    return tmat4<T>((tmat4<T>::glm_t)lhs * (tmat4<T>::glm_t)rhs);
}

template<typename T>
inline T* value_ptr(tmat4<T> & x)
{
    return reinterpret_cast<T*>(&x);
}

template<typename T>
inline const T* value_ptr(const tmat4<T> & x)
{
    return reinterpret_cast<const T*>(&x);
}

typedef tmat4<f32> mat4;
static_assert(sizeof(mat4) == sizeof(glm::tmat4x4<f32, glm::highp>), "mat4 differs in size from glm::mat4x4");

} // namespace gaen

#endif // #ifndef GAEN_MATH_MAT4_H
