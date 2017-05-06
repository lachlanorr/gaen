//------------------------------------------------------------------------------
// vec4.h - 4d vector
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

#ifndef GAEN_MATH_VEC4_H
#define GAEN_MATH_VEC4_H

#include <glm/vec4.hpp>
#include <glm/geometric.hpp>// glm::cross, glm::normalize

#include "core/base_defines.h"

namespace gaen
{

template <typename T>
struct tvec3;

template <typename T>
struct tvec4
{
    // glm conversions
    typedef glm::tvec4<T, glm::highp> glm_t;
    explicit tvec4(const glm_t & rhs)
      : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
    {}
    operator glm_t() const { return *reinterpret_cast<const glm_t*>(this); }

    union
    {
        struct{ T x, y, z, w; };
        struct{ T r, g, b, a; };
        struct{ T s, t, p, q; };
    };
    
    tvec4() = default;

    tvec4(T x, T y, T z, T w)
      : x(x), y(y), z(z), w(w)
    {}

    explicit tvec4(T x)
      : x(x), y(x), z(x), w(x)
    {}
    
    tvec4(const tvec3<T> & v, T w);

    u32 length() const { return sizeof(*this) / sizeof(T); }

    T& operator[](u32 i) { ASSERT(i < length()); return (&x)[i]; }
    const T& operator[](u32 i) const { ASSERT(i < length()); return (&x)[i]; }

}; // struct vec4

typedef tvec4<f32> vec4;
typedef tvec4<u32> uvec4;
typedef tvec4<i32> ivec4;
static_assert(sizeof(vec4) == sizeof(glm::tvec4<f32, glm::highp>), "vec4 differs in size from glm::vec4");

template <typename T>
inline bool operator==(const tvec4<T> & lhs, const tvec4<T> & rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

template <typename T>
inline bool operator!=(const tvec4<T> & lhs, const tvec4<T> & rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w;
}

template <typename T>
inline tvec4<T> operator-(const tvec4<T> & rhs)
{
    return tvec4<T>(-rhs.x, -rhs.y, -rhs.z, -rhs.w);
}

template <typename T>
inline tvec4<T> operator-(const tvec4<T> & lhs, const tvec4<T> & rhs)
{
    return tvec4<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template <typename T>
inline tvec4<T> operator+(const tvec4<T> & lhs, const tvec4<T> & rhs)
{
    return tvec4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
    
template<typename T>
inline T* value_ptr(tvec4<T> & x)
{
    return reinterpret_cast<T*>(&x);
}

template<typename T>
inline const T* value_ptr(const tvec4<T> & x)
{
    return reinterpret_cast<const T*>(&x);
}

inline vec4 normalize(const vec4 & v)
{
    return vec4(glm::normalize((vec4::glm_t)v));
}

inline f32 dot(const vec4 & lhs, const vec4 & rhs)
{
    return glm::dot((vec4::glm_t)lhs, (vec4::glm_t)rhs);
}

inline f32 length(const vec4 & v)
{
    return glm::length((vec4::glm_t)v);
}

} // namespace gaen

#endif // #ifndef GAEN_MATH_VEC4_H
