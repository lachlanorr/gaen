//------------------------------------------------------------------------------
// mat3.h - 3x3 matrix
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

#ifndef GAEN_MATH_MAT3_H
#define GAEN_MATH_MAT3_H

#include <glm/mat3x3.hpp>

#include "math/vec3.h"

#include "core/base_defines.h"

namespace gaen
{

template <typename T>
struct tmat4;

template <typename T>
struct tmat43;

template <typename T>
struct tmat3a;

template <typename T>
struct tmat3
{
    typedef glm::tmat3x3<T, glm::highp> glm_t;
    explicit tmat3(const glm_t & rhs)
    {
        *this = *reinterpret_cast<const tmat3*>(&rhs);
    }
    operator glm_t() const { return *reinterpret_cast<const glm_t*>(this); }

    tvec3<T> cols[3];

    tmat3() = default;

    tmat3(T x)
    {
        cols[0] = tvec3<T>(x, 0, 0);
        cols[1] = tvec3<T>(0, x, 0);
        cols[2] = tvec3<T>(0, 0, x);
    }

    explicit tmat3(const tvec3<T> & rot);

    explicit tmat3(const tmat3a<T> & rhs);
    explicit tmat3(const tmat4<T> & rhs);
    explicit tmat3(const tmat43<T> & rhs);

    tvec3<T> & operator[](u32 i) { ASSERT(i < sizeof(cols) / sizeof(tvec3<T>)); return cols[i]; }
    const tvec3<T> & operator[](u32 i) const { ASSERT(i < sizeof(cols) / sizeof(tvec3<T>)); return cols[i]; }

    tmat3 & operator=(const tmat4<T> & rhs);
    tmat3 & operator=(const tmat43<T> & rhs);

    // inverse
    tmat3 operator~() const;
};

typedef tmat3<f32> mat3;
static_assert(sizeof(mat3) == sizeof(glm::tmat3x3<f32, glm::highp>), "mat3 differs in size from glm::mat3x3");

template <typename T>
struct tmat3a : tmat3<T>
{
    tmat3a() = default;
    explicit tmat3a(const tmat3<T> & rhs)
    {
        cols[0] = rhs[0];
        cols[1] = rhs[1];
        cols[2] = rhs[2];
    }
    explicit tmat3a(const tvec3<T> & rot)
    {
        tmat3<T> m(rot);
        cols[0] = m[0];
        cols[1] = m[1];
        cols[2] = m[2];
    }
private:
    vec3 padding__;
};
typedef tmat3a<f32> mat3a;
static_assert(sizeof(mat3a) % 16 == 0, "mat3a not aligned to 16 bytes");


template <typename T>
inline tmat3<T> operator*(const tmat3<T> & lhs, const tmat3<T> & rhs)
{
    return tmat3<T>((tmat3<T>::glm_t)lhs * (tmat3<T>::glm_t)rhs);
}

template <typename T>
inline tvec3<T> operator*(const tmat3<T> & lhs, const tvec3<T> & rhs)
{
    return tvec3<T>((tmat3<T>::glm_t)lhs * (tvec3<T>::glm_t)rhs);
}

template<typename T>
inline T* value_ptr(tmat3<T> & x)
{
    return reinterpret_cast<T*>(&x);
}

template<typename T>
inline const T* value_ptr(const tmat3<T> & x)
{
    return reinterpret_cast<const T*>(&x);
}

template <typename T>
inline tmat3<T> transpose(const tmat3<T> & rhs)
{
    return tmat3<T>(glm::transpose((tmat3<T>::glm_t)rhs));
}

} // namespace gaen

#endif // #ifndef GAEN_MATH_MAT3_H
