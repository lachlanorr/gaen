//------------------------------------------------------------------------------
// mat43.h - 4x3 matrix, typically used for concise transforms
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

#ifndef GAEN_MATH_MAT43_H
#define GAEN_MATH_MAT43_H

#include <glm/mat4x3.hpp>

#include "math/vec3.h"

#include "core/base_defines.h"

namespace gaen
{

template <typename T>
struct tmat4;

template <typename T>
struct tmat3;

template <typename T>
struct tmat43
{
    typedef glm::tmat4x3<f32, glm::highp> glm_t;
    explicit tmat43(const glm_t & rhs)
    {
        *this = *reinterpret_cast<const tmat43*>(&rhs);
    }
    operator glm_t() const { return *reinterpret_cast<const glm_t*>(this); }

    tvec3<T> cols[4];

    tmat43() = default;
    tmat43(T x)
    {
        cols[0] = tvec3<T>(x, 0, 0);
        cols[1] = tvec3<T>(0, x, 0);
        cols[2] = tvec3<T>(0, 0, x);
        cols[3] = tvec3<T>(0, 0, 0);
    }

    explicit tmat43(const tvec3<T> & pos);
    tmat43(const tvec3<T> & pos, const tvec3<T> & rot);
    static tmat43 from_pos(T x, T y, T z);
    static tmat43 from_rot(T x, T y, T z);
    static tmat43 from_scale(T scale);

    explicit tmat43(const tmat4<T> & rhs);
    explicit tmat43(const tmat3<T> & rhs);

    tvec3<T> & operator[](u32 i) { ASSERT(i < sizeof(cols) / sizeof(tvec3<T>)); return cols[i]; }
    const tvec3<T> & operator[](u32 i) const { ASSERT(i < sizeof(cols) / sizeof(tvec3<T>)); return cols[i]; }
    
    tmat43 & operator=(const tmat4<T> & rhs);
    tmat43 & operator=(const tmat3<T> & rhs);

    // inverse
    tmat43 operator~() const;

    const tvec3<T> & pos() const { return cols[3]; }
};

typedef tmat43<f32> mat43;
static_assert(sizeof(mat43) == sizeof(glm::tmat4x3<f32, glm::highp>), "mat43 differs in size from glm::mat4x3");

template <typename T>
inline bool operator==(const tmat43<T> & lhs, const tmat43<T> & rhs)
{
    return 0 == memcmp(&lhs, &rhs, sizeof(tmat43<T>));
}

template <typename T>
inline bool operator!=(const tmat43<T> & lhs, const tmat43<T> & rhs)
{
    return !(lhs == rhs);
}

template <typename T>
inline tmat43<T> operator*(const tmat43<T> & lhs, const tmat43<T> & rhs)
{
    return tmat43<T>((tmat4<T>::glm_t)lhs * (tmat4<T>::glm_t)rhs);
}

template <typename T>
inline tmat43<T> operator*(const tmat4<T> & lhs, const tmat43<T> & rhs)
{
    return tmat43<T>((tmat4<T>::glm_t)lhs * (tmat4<T>::glm_t)tmat4<T>(rhs));
}

template <typename T>
inline tmat43<T> operator*(const tmat43<T> & lhs, const tmat4<T> & rhs)
{
    return tmat43<T>((tmat4<T>::glm_t)tmat4<T>(lhs) * (tmat4<T>::glm_t)rhs);
}

template<typename T>
inline T* value_ptr(tmat43<T> & x)
{
    return reinterpret_cast<T*>(&x);
}

template<typename T>
inline const T* value_ptr(const tmat43<T> & x)
{
    return reinterpret_cast<const T*>(&x);
}

} // namespace gaen

#endif // #ifndef GAEN_MATH_MAT43_H
