//------------------------------------------------------------------------------
// vec3.h - 3d vector
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

#ifndef GAEN_MATH_VEC3_H
#define GAEN_MATH_VEC3_H

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>// glm::cross, glm::normalize

#include "core/base_defines.h"
#include "core/hashing.h"

#include "math/common.h"

namespace gaen
{

template <typename T>
struct tvec4;
template <typename T>
struct tvec2;

template <typename T>
struct tvec3
{
    // glm conversions
    typedef glm::tvec3<T, glm::highp> glm_t;
    explicit tvec3(const glm_t & rhs)
      : x(rhs.x), y(rhs.y), z(rhs.z)
    {}
    operator glm_t() const { return *reinterpret_cast<const glm_t*>(this); }

    union
    {
        struct{ T x, y, z; };
        struct{ T r, g, b; };
        struct{ T s, t, p; };
    };
    
    tvec3() = default;

    tvec3(T x, T y, T z)
      : x(x), y(y), z(z)
    {}

    explicit tvec3(T x)
      : x(x), y(x), z(x)
    {}
    
    tvec3(const tvec4<T> & v);
    tvec3(const tvec2<T> & v, T s);
    tvec3(T s, const tvec2<T> & v);

    u32 length() const { return sizeof(*this) / sizeof(T); }

    T& operator[](u32 i) { ASSERT(i < length()); return (&x)[i]; }
    const T& operator[](u32 i) const { ASSERT(i < length()); return (&x)[i]; }

    const tvec3 & operator+=(const tvec3 & rhs)
    {
        *this = *this + rhs;
        return *this;
    }
}; // struct tvec3

typedef tvec3<f32> vec3;
typedef tvec3<u32> uvec3;
typedef tvec3<i32> ivec3;
static_assert(sizeof(vec3) == sizeof(glm::tvec3<f32, glm::highp>), "vec3 differs in size from glm::vec3");

template <typename T>
inline bool operator==(const tvec3<T> & lhs, const tvec3<T> & rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template <typename T>
inline bool operator!=(const tvec3<T> & lhs, const tvec3<T> & rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

template <typename T>
inline tvec3<T> operator-(const tvec3<T> & rhs)
{
    return tvec3<T>(-rhs.x, -rhs.y, -rhs.z);
}

template <typename T>
inline tvec3<T> operator-(const tvec3<T> & lhs, const tvec3<T> & rhs)
{
    return tvec3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

template <typename T>
inline tvec3<T> operator+(const tvec3<T> & lhs, const tvec3<T> & rhs)
{
    return tvec3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template <typename T>
inline tvec3<T> operator*(const tvec3<T> & lhs, T rhs)
{
    return tvec3<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

template <typename T>
inline tvec3<T> operator*(const tvec3<T> & lhs, const tvec3<T> & rhs)
{
    return tvec3<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

template <typename T>
inline tvec3<T> operator*(T lhs, const tvec3<T> & rhs)
{
    return tvec3<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

template <typename T>
inline tvec3<T> operator/(const tvec3<T> & lhs, T rhs)
{
    return tvec3<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

template <typename T>
inline tvec3<T> operator/(T lhs, const tvec3<T> & rhs)
{
    return tvec3<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
}

template <typename T>
inline tvec3<T> operator/(const tvec3<T> & lhs, const tvec3<T> & rhs)
{
    return tvec3<T>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

inline vec3 normalize(const vec3 & v)
{
    return vec3(glm::normalize((vec3::glm_t)v));
}

inline vec3 cross(const vec3 & lhs, const vec3 & rhs)
{
    return vec3(glm::cross((vec3::glm_t)lhs, (vec3::glm_t)rhs));
}

inline f32 dot(const vec3 & lhs, const vec3 & rhs)
{
    return glm::dot((vec3::glm_t)lhs, (vec3::glm_t)rhs);
}

inline f32 length(const vec3 & v)
{
    return glm::length((vec3::glm_t)v);
}

template <>
inline vec3 min<vec3>(const vec3 & lhs, const vec3 & rhs)
{
    return vec3(min(lhs.x, rhs.x),
                min(lhs.y, rhs.y),
                min(lhs.z, rhs.z));
}

template <>
inline vec3 max<vec3>(const vec3 & lhs, const vec3 & rhs)
{
    return vec3(max(lhs.x, rhs.x),
                max(lhs.y, rhs.y),
                max(lhs.z, rhs.z));
}

template<typename T>
inline T* value_ptr(tvec3<T> & x)
{
    return reinterpret_cast<T*>(&x);
}

template<typename T>
inline const T* value_ptr(const tvec3<T> & x)
{
    return reinterpret_cast<const T*>(&x);
}

inline vec3 tri_normal(const vec3 & p0, const vec3 & p1, const vec3 & p2)
{
    vec3 vecA = p1 - p0;
    vec3 vecB = p2 - p0;
    return normalize(cross(vecA, vecB));
}

inline vec3 radians(vec3 degrees)
{
    return vec3(radians(degrees.x),
                radians(degrees.y),
                radians(degrees.z));
}

inline vec3 degrees(vec3 radians)
{
    return vec3(degrees(radians.x),
                degrees(radians.y),
                degrees(radians.z));
}

} // namespace gaen

namespace std
{
    template <>
    struct hash<gaen::vec3> : public unary_function<gaen::vec3, size_t>
    {
        size_t operator()(const gaen::vec3& value) const
        {
            return gaen::fnv1a_32(reinterpret_cast<const gaen::u8*>(&value), sizeof(gaen::vec3));
        }
    };
} // namespace std

#endif // #ifndef GAEN_MATH_VEC3_H
