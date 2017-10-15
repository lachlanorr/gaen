//------------------------------------------------------------------------------
// common.h - Common math functions
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

#ifndef GAEN_MATH_COMMON_H
#define GAEN_MATH_COMMON_H

#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/base_defines.h"

namespace gaen
{

static const f32 kFpErrThreshold = 0.00000001f;
inline bool is_fp_eq(f32 actual, f32 expected) { return (actual >= expected - kFpErrThreshold) && (actual <= expected + kFpErrThreshold); }

static const f32 kPi = 3.1415927f;
static const f32 k2Pi = 2.0f * kPi;
inline f32 radians(f32 deg) { return deg * (kPi / 180.0f); }
inline f32 degrees(f32 rad) { return rad * (180.0f / kPi); }

inline i32 round(f32 val) { return (i32)(val + 0.5f); }

template<typename T>
inline T min(const T & lhs, const T & rhs)
{
    return lhs <= rhs ? lhs : rhs;
}

template<typename T>
inline T max(const T & lhs, const T & rhs)
{
    return lhs >= rhs ? lhs : rhs;
}

template <typename T>
inline T clamp(T val, T min, T max)
{
    if (val < min)
        return min;
    else if (val > max)
        return max;
    return val;
}

template<typename T>
inline T sqrt(T x)
{
    return glm::sqrt(x);
}

template<typename T>
inline T abs(T x)
{
    return glm::abs(x);
}

template<typename T>
inline T acos(T x)
{
    return glm::acos(x);
}

template<typename T>
inline T atan(T x)
{
    return glm::atan(x);
}

inline f32 uintBitsToFloat(u32 x)
{
    return glm::uintBitsToFloat(x);
}

} // namespace gaen

#endif // #ifndef GAEN_MATH_COMMON_H
