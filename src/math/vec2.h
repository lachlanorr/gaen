//------------------------------------------------------------------------------
// vec2.h - 2d vector
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

#ifndef GAEN_MATH_VEC2_H
#define GAEN_MATH_VEC2_H

#include <glm/vec2.hpp>
#include <glm/geometric.hpp>// glm::cross, glm::normalize

#include "core/base_defines.h"
#include "math/common.h"

namespace gaen
{

template <typename T>
struct tvec2
{
    // glm conversions
    typedef glm::tvec2<T, glm::highp> glm_t;
    explicit tvec2(const glm_t & rhs)
      : x(rhs.x), y(rhs.y)
    {}
    operator glm_t() const { return *reinterpret_cast<const glm_t*>(this); }

    union
    {
        struct{ T x, y; };
        struct{ T r, g; };
        struct{ T s, t; };
    };
    
    tvec2() = default;

    tvec2(T x, T y)
      : x(x), y(y)
    {}

    explicit tvec2(T x)
      : x(x), y(x)
    {}
    
    u32 length() const { return sizeof(*this) / sizeof(T); }

    T& operator[](u32 i) { ASSERT(i < length()); return (&x)[i]; }
    const T& operator[](u32 i) const { ASSERT(i < length()); return (&x)[i]; }

}; // struct vec2

typedef tvec2<f32> vec2;
typedef tvec2<u32> uvec2;
typedef tvec2<i32> ivec2;

static_assert(sizeof(vec2) == sizeof(glm::tvec2<f32, glm::highp>), "vec2 differs in size from glm::vec2");

template <typename T>
inline bool operator==(const tvec2<T> & lhs, const tvec2<T> & rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename T>
inline bool operator!=(const tvec2<T> & lhs, const tvec2<T> & rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

template <typename T>
inline tvec2<T> operator-(const tvec2<T> & rhs)
{
    return tvec2<T>(-rhs.x, -rhs.y);
}

template <typename T>
inline tvec2<T> operator-(const tvec2<T> & lhs, const tvec2<T> & rhs)
{
    return tvec2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template <typename T>
inline tvec2<T> operator+(const tvec2<T> & lhs, const tvec2<T> & rhs)
{
    return tvec2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template <typename T>
inline tvec2<T> operator+=(tvec2<T> & lhs, const tvec2<T> & rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

template <typename T>
inline tvec2<T> operator*(const tvec2<T> & lhs, T rhs)
{
    return tvec2<T>(lhs.x * rhs, lhs.y * rhs);
}
    
template <>
inline vec2 abs<vec2>(const vec2 & v)
{
    return vec2(abs(v.x),
                abs(v.y));
}

template <>
inline vec2 min<vec2>(const vec2 & lhs, const vec2 & rhs)
{
    return vec2(min(lhs.x, rhs.x),
                min(lhs.y, rhs.y));
}

template <>
inline vec2 max<vec2>(const vec2 & lhs, const vec2 & rhs)
{
    return vec2(max(lhs.x, rhs.x),
                max(lhs.y, rhs.y));
}

template<typename T>
inline T* value_ptr(tvec2<T> & x)
{
    return reinterpret_cast<T*>(&x);
}

template<typename T>
inline const T* value_ptr(const tvec2<T> & x)
{
    return reinterpret_cast<const T*>(&x);
}

inline vec2 normalize(const vec2 & v)
{
    return vec2(glm::normalize((vec2::glm_t)v));
}

inline f32 dot(const vec2 & lhs, const vec2 & rhs)
{
    return glm::dot((vec2::glm_t)lhs, (vec2::glm_t)rhs);
}

inline f32 length(const vec2 & v)
{
    return glm::length((vec2::glm_t)v);
}

} // namespace gaen

#endif // #ifndef GAEN_MATH_VEC2_H
