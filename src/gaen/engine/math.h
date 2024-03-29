//------------------------------------------------------------------------------
// math.h - Vector and matrix math
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2022 Lachlan Orr
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

// LORRTODO: Remove math.h/cpp when glm is fully tested and replaces all this stuff.
// Define away this file for now, but keep around for reference.
#if 0

#ifndef GAEN_ENGINE_MATH_H
#define GAEN_ENGINE_MATH_H

#include <cmath>

#include "gaen/core/base_defines.h"

namespace gaen
{

static const f32 kPi = 3.1415926535897932384626433832795f;
inline f32 radians(f32 deg) { return deg * (kPi / 180.0f); }
inline f32 degrees(f32 rad) { return rad * (180.0f / kPi); }

inline i32 round(f32 val) { return (i32)(val + 0.5f); }

static const f32 kFpErrThreshold = 0.00000001f;
inline bool is_fp_eq(f32 actual, f32 expected) { return (actual >= expected - kFpErrThreshold) && (actual <= expected + kFpErrThreshold); }

f32 invSqrt(const f32 x);
f32 fastSqrt(const f32 x);

struct Vec2
{
    Vec2() = default;
    Vec2(f32 x, f32 y);
    Vec2(const struct Vec3 & rhs);
    Vec2(const struct Vec4 & rhs);

    f32 & x();
    f32 & y();

    const f32 & x() const;
    const f32 & y() const;

    f32 elems[2];
};

Vec2 max_(const Vec2 & lhs, const Vec2 & rhs);

struct Vec3
{
    // constructors
    Vec3() = default;
    Vec3(f32 x, f32 y, f32 z);
    Vec3(const struct Vec2 &vec2);
    Vec3(const struct Vec4 &vec4);

    f32 length() const;
    void normalize();
    void normalizeIfNecessary();

    Vec3 operator-() const;
    Vec3& operator+=(const Vec3& rhs);
    Vec3& operator-=(const Vec3& rhs);
    bool operator==(const Vec3 & rhs) const;
    f32 & operator[](size_t idx);
    const f32 & operator[](size_t idx) const;

    f32 & x();
    f32 & y();
    f32 & z();

    const f32 & x() const;
    const f32 & y() const;
    const f32 & z() const;

    static Vec3 normalize(const Vec3 &vec3);
    static f32 dot(const Vec3 &lhs, const Vec3 &rhs);
    static Vec3 cross(const Vec3 &lhs, const Vec3 &rhs);

    f32 elems[3];
};

Vec3 min_(const Vec3 & lhs, const Vec3 & rhs);
Vec3 max_(const Vec3 & lhs, const Vec3 & rhs);
Vec3 operator+ (const Vec3 & lhs, const Vec3 & rhs);
Vec3 operator- (const Vec3 & lhs, const Vec3 & rhs);
Vec3 operator* (f32 lhs, const Vec3 & rhs);
Vec3 operator* (const Vec3 & lhs, float rhs);
Vec3 operator* (const Vec3 & lhs, const Vec3 & rhs);
Vec3 operator/ (f32 lhs, const Vec3 & rhs);
Vec3 operator/ (const Vec3 & lhs, float rhs);
Vec3 operator/ (const Vec3 & lhs, const Vec3 & rhs);

Vec3 triNormal(const Vec3 & p0, const Vec3 & p1, const Vec3 & p2);

struct Quat;
struct Vec4
{
    Vec4() = default;
    Vec4(f32 x, f32 y, f32 z, f32 w);
    Vec4(const Vec3 &vec3);
    Vec4(const Vec3 &vec3, f32 w);
    Vec4(const Vec2 &vec2);
    Vec4(const Vec2 &vec2, f32 w);
    Vec4(const Quat &quat);

    f32 length() const;
    void normalize();
    void normalizeIfNecessary();

    Vec4 operator-() const;
    bool operator==(const Vec4 & rhs) const;
    f32 & operator[](size_t idx);
    const f32 & operator[](size_t idx) const;

    f32 & x();
    f32 & y();
    f32 & z();
    f32 & w();

    const f32 & x() const;
    const f32 & y() const;
    const f32 & z() const;
    const f32 & w() const;

    static Vec4 normalize(const Vec4 &vec4);
    static f32 dot(const Vec3 &lhs, const Vec3 &rhs);
    static Vec4 cross(const Vec4 &lhs, const Vec4 &rhs);

    f32 elems[4];
};

struct Quat
{
    Quat() = default;
    Quat(f32 x, f32 y, f32 z, f32 w);

    void normalize();

    f32 & operator[](size_t idx);
    const f32 & operator[](size_t idx) const;

    f32 & x();
    f32 & y();
    f32 & z();
    f32 & w();

    const f32 & x() const;
    const f32 & y() const;
    const f32 & z() const;
    const f32 & w() const;

    Quat operator-() const;
    Vec3 operator* (const Vec3 & rhs) const;
    Quat operator* (const Quat & rhs) const;

    static Quat normalize(const Quat & q);
    static Quat from_axis_angle(const Vec3 & dir, f32 angle, bool shouldNormalize = false);
    static void to_axis_angle(const Quat & q, Vec3 & dir, f32 & angle, bool shouldNormalize = false);

    static Quat conjugate(const Quat & q);
    static Vec3 multiply(const Quat & qlhs, const Vec3 & vrhs);
    static Quat multiply(const Quat & qlhs, const Quat & qrhs);

    f32 elems[4];
};

struct Mat2
{
    Mat2();
    Mat2(f32 diag);
    Mat2(f32 e0, f32 e1,
         f32 e2, f32 e3);

    static const Mat2 & zero();
    static const Mat2 & identity();

    static Vec2 multiply(const Mat2 & mat2, const Vec2 & vec2);
    static Mat2 rotation(f32 angle);

    f32 & operator[](size_t idx);
    const f32 & operator[](size_t idx) const;

    f32 elems[4];
};

struct Mat34;
struct Mat4;
struct Mat3
{
    Mat3();
    Mat3(f32 diag);
    Mat3(f32 e0, f32 e1, f32 e2,
         f32 e3, f32 e4, f32 e5,
         f32 e6, f32 e7, f32 e8);
    Mat3(const Mat34 & mat34);
    Mat3(const Mat4 & mat4);
    Mat3(const Quat & quat);

    static const Mat3 & zero();
    static const Mat3 & identity();

    static Vec3 multiply(const Mat3 & mat3, const Vec3 & vec3);

    static f32 determinant(const Mat3 & mat3);
    static Mat3 inverse(const Mat3 & mat3);
    static Mat3 transpose(const Mat3 & mat3);

    static Mat3 rotation(const Quat & quat);
    static Mat3 rotation(const Vec3 & angles);

    // build a 3x3 matrix to use to transform normals
    static Mat3 model_view_to_normal_transform(const Mat4 & mv);

    f32 & operator[](size_t idx);
    const f32 & operator[](size_t idx) const;

    f32 elems[9];
};

struct Mat34
{
    Mat34();
    Mat34(f32 diag);
    Mat34(const Mat4 & mat4);
    Mat34(f32 e0, f32 e1,  f32 e2,
          f32 e3, f32 e4,  f32 e5,
          f32 e6, f32 e7,  f32 e8,
          f32 e9, f32 e10, f32 e11);
    Mat34(const Quat & quat);

    static const Mat34 & zero();
    static const Mat34 & identity();

    static Vec4 multiply(const Mat34 & mat34, const Vec4 & vec4);
    static Mat34 multiply(const Mat34 & lhs, const Mat34 & rhs);
    static Mat4 multiply(const Mat34 & lhs, const Mat4 & rhs);

    static f32 determinant(const Mat34 & mat34);
    static Mat34 inverse(const Mat34 & mat34);

    static Mat34 translation(const Vec3 & trans);
    static Mat34 rotation(const Vec3 & angles);
    static Mat34 scale(f32 factor);
    static Mat34 transform(const Vec3 & trans,
                           const Vec3 & angles,
                           f32 factor);

    f32 & operator[](size_t idx);
    const f32 & operator[](size_t idx) const;

    Vec4  operator* (const Vec4 & rhs) const;
    Mat34 operator* (const Mat34 & rhs) const;
    Mat4  operator* (const Mat4 & rhs) const;

    Vec4 & vec4(size_t idx);
    const Vec4 & vec4(size_t idx) const;

    f32 elems[12];
};

struct Mat4
{
    Mat4();
    Mat4(f32 diag);
    Mat4(f32 e0,  f32 e1,  f32 e2,  f32 e3,
         f32 e4,  f32 e5,  f32 e6,  f32 e7,
         f32 e8,  f32 e9,  f32 e10, f32 e11,
         f32 e12, f32 e13, f32 e14, f32 e15);
    Mat4(const Mat34 & mat34);
    Mat4(const Quat & quat);

    void setTranslation(const Vec3 & trans);

    static const Mat4 & zero();
    static const Mat4 & identity();

    static Vec4 multiply(const Mat4 & mat4, const Vec4 & vec4);
    static Mat4 multiply(const Mat4 & lhs, const Mat4 & rhs);

    static f32 determinant(const Mat4 & mat4);
    static Mat4 inverse(const Mat4 & mat4);
    static Mat4 transpose(const Mat4 & mat4);

    static Mat4 translation(const Vec3 & trans);
    static Mat4 rotation(const Vec3 & angles);
    static Mat4 scale(f32 factor);
    static Mat4 transform(const Vec3 & trans,
                          const Vec3 & angles,
                          f32 factor);

    static Mat4 frustum(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ);
    static Mat4 perspective(f32 fovy, f32 aspect, f32 nearZ, f32 farZ);
    static Mat4 lookat(const Vec3 & eye, const Vec3 & center, const Vec3 & up);
    static Mat4 fps_view(const Vec3 & eye, f32 pitch, f32 yaw);
    
    static Mat4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ);

    f32 & operator[](size_t idx);
    const f32 & operator[](size_t idx) const;

    Vec4 operator* (const Vec4 & rhs) const;
    Mat4 operator* (const Mat4 & rhs) const;

    f32 elems[16];
};


//------------------------------------------------------------------------------
// Inline function definitions
//------------------------------------------------------------------------------

// This is the Quake 3 Walsh method
inline f32 invSqrt(const f32 x)
{
    const f32 xhalf = 0.5f * x;
 
    union // get bits for floating value
    {
        f32 x;
        int i;
    } u;

    u.x = x;
    u.i = 0x5f3759df - (u.i >> 1); // gives initial guess y0
    u.x = u.x*(1.5f - xhalf * u.x * u.x); // Newton step, repeating increases accuracy
    return u.x;
}

inline f32 fastSqrt(const f32 x)
{
    return x * invSqrt(x);
}

//--------------------------------------
// Vec2 inlined methods
//--------------------------------------
inline Vec2::Vec2(f32 x, f32 y)
{
    elems[0] = x;
    elems[1] = y;
}

inline Vec2::Vec2(const struct Vec3 & vec3)
{
    elems[0] = vec3.x();
    elems[1] = vec3.y();
}

inline Vec2::Vec2(const struct Vec4 & vec4)
{
    elems[0] = vec4.x();
    elems[1] = vec4.y();
}

inline f32 & Vec2::x()
{
    return elems[0];
}

inline f32 & Vec2::y()
{
    return elems[1];
}

inline const f32 & Vec2::x() const
{
    return elems[0];
}

inline const f32 & Vec2::y() const
{
    return elems[1];
}

inline Vec2 max_(const Vec2 & lhs, const Vec2 & rhs)
{
    return Vec2(glm::max(lhs.x(), rhs.x()), glm::max(lhs.y(), rhs.y()));
}


//--------------------------------------
// Vec3 inlined methods
//--------------------------------------
inline Vec3::Vec3(f32 x, f32 y, f32 z)
{
    elems[0] = x;
    elems[1] = y;
    elems[2] = z;
}

inline Vec3::Vec3(const Vec2 &vec2)
{
    elems[0] = vec2.x();
    elems[1] = vec2.y();
    elems[2] = 0.0f;
}

inline Vec3::Vec3(const Vec4 &vec4)
{
    elems[0] = vec4.x();
    elems[1] = vec4.y();
    elems[2] = vec4.z();
}

inline f32 Vec3::length() const
{
    return sqrt(x()*x() + y()*y() + z()*z());
}

inline void Vec3::normalize()
{
    //float inv_mag = invSqrt(x*x + y*y + z*z);
    f32 inv_mag = 1.0f / sqrt(x()*x() + y()*y() + z()*z());
    x() *= inv_mag;
    y() *= inv_mag;
    z() *= inv_mag;
}

inline void Vec3::normalizeIfNecessary()
{
    f32 sumSquares = x()*x() + y()*y() + z()*z();
    if (!is_fp_eq(sumSquares, 1.0f))
    {
        f32 inv_mag = 1.0f / sqrt(sumSquares);
        x() *= inv_mag;
        y() *= inv_mag;
        z() *= inv_mag;
    }
}

inline Vec3 Vec3::operator-() const 
{
    return Vec3(-x(), -y(), -z());
}


inline Vec3 min_(const Vec3 & lhs, const Vec3 & rhs)
{
    return Vec3(glm::min(lhs.x(), rhs.x()),
                glm::min(lhs.y(), rhs.y()),
                glm::min(lhs.z(), rhs.z()));
}

inline Vec3 max_(const Vec3 & lhs, const Vec3 & rhs)
{
    return Vec3(glm::max(lhs.x(), rhs.x()),
                glm::max(lhs.y(), rhs.y()),
                glm::max(lhs.z(), rhs.z()));
}

inline Vec3 operator+ (const Vec3 & lhs, const Vec3 & rhs)
{
    return Vec3(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

inline Vec3 operator- (const Vec3 & lhs, const Vec3 & rhs)
{
    return Vec3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

inline Vec3 operator* (f32 lhs, const Vec3 & rhs)
{
    return Vec3(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z());
}

inline Vec3 operator* (const Vec3 & lhs, float rhs)
{
    return Vec3(lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs);
}

inline Vec3 operator* (const Vec3 & lhs, const Vec3 & rhs)
{
    return Vec3(lhs.x() * rhs.x(),
                lhs.y() * rhs.y(),
                lhs.z() * rhs.z());
}

inline Vec3 operator/ (f32 lhs, const Vec3 & rhs)
{
    return Vec3(lhs / rhs.x(), lhs / rhs.y(), lhs / rhs.z());
}

inline Vec3 operator/ (const Vec3 & lhs, float rhs)
{
    return Vec3(lhs.x() / rhs, lhs.y() / rhs, lhs.z() / rhs);
}

inline Vec3 operator/ (const Vec3 & lhs, const Vec3 & rhs)
{
    return Vec3(lhs.x() / rhs.x(),
                lhs.y() / rhs.y(),
                lhs.z() / rhs.z());
}

inline Vec3& Vec3::operator+=(const Vec3& rhs)
{
    elems[0] += rhs.elems[0];
    elems[1] += rhs.elems[1];
    elems[2] += rhs.elems[2];

    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& rhs)
{
    elems[0] -= rhs.elems[0];
    elems[1] -= rhs.elems[1];
    elems[2] -= rhs.elems[2];

    return *this;
}

inline bool Vec3::operator==(const Vec3 & rhs) const
{
    return x() == rhs.x() && y() == rhs.y() && z() == rhs.z();
}

inline f32 & Vec3::operator[](size_t idx)
{
    ASSERT(idx < 4);
    return elems[idx];
}

inline const f32 & Vec3::operator[](size_t idx) const
{
    ASSERT(idx < 4);
    return elems[idx];
}

inline f32 & Vec3::x() { return elems[0]; }
inline f32 & Vec3::y() { return elems[1]; }
inline f32 & Vec3::z() { return elems[2]; }

inline const f32 & Vec3::x() const { return elems[0]; }
inline const f32 & Vec3::y() const { return elems[1]; }
inline const f32 & Vec3::z() const { return elems[2]; }

inline Vec3 Vec3::normalize(const Vec3 &vec)
{
    //float inv_mag = invSqrt(x*x + y*y + z*z);
    f32 inv_mag = 1.0f / sqrt(vec.x()*vec.x() + vec.y()*vec.y() + vec.z()*vec.z());
    return Vec3(vec.x() * inv_mag, vec.y() * inv_mag, vec.z() * inv_mag);
}

inline f32 Vec3::dot(const Vec3 &lhs, const Vec3 &rhs)
{
    return lhs.x()*rhs.x() + lhs.y()*rhs.y() + lhs.z()*rhs.z();
}

inline Vec3 Vec3::cross(const Vec3 &lhs, const Vec3 &rhs)
{
    Vec3 vec(lhs.y()*rhs.z() - lhs.z()*rhs.y(),
             lhs.z()*rhs.x() - lhs.x()*rhs.z(),
             lhs.x()*rhs.y() - lhs.y()*rhs.x());
    return vec;
}

inline Vec3 triNormal(const Vec3 & p0, const Vec3 & p1, const Vec3 & p2)
{
    Vec3 vecA = p1 - p0;
    Vec3 vecB = p2 - p0;
    return Vec3::cross(vecA, vecB);
}


//--------------------------------------
// Vec4 inlined methods
//--------------------------------------
inline Vec4::Vec4(f32 x, f32 y, f32 z, f32 w)
{
    elems[0] = x;
    elems[1] = y;
    elems[2] = z;
    elems[3] = w;
}

inline Vec4::Vec4(const Vec2 &vec2)
{
    elems[0] = vec2.x();
    elems[1] = vec2.y();
    elems[2] = 0.0f;
    elems[3] = 1.0f;
}

inline Vec4::Vec4(const Vec2 &vec2, f32 w)
{
    elems[0] = vec2.x();
    elems[1] = vec2.y();
    elems[2] = 0.0f;
    elems[3] = w;
}

inline Vec4::Vec4(const Vec3 &vec3)
{
    elems[0] = vec3.x();
    elems[1] = vec3.y();
    elems[2] = vec3.z();
    elems[3] = 1.0f;
}

inline Vec4::Vec4(const Vec3 &vec3, f32 w)
{
    elems[0] = vec3.x();
    elems[1] = vec3.y();
    elems[2] = vec3.z();
    elems[3] = w;
}

inline Vec4::Vec4(const Quat &quat)
{
    elems[0] = quat.elems[0];
    elems[1] = quat.elems[1];
    elems[2] = quat.elems[2];
    elems[3] = quat.elems[3];
}

inline f32 Vec4::length() const 
{ 
    return sqrt(x()*x() + y()*y() + z()*z());
}

inline void Vec4::normalize()
{
    //float inv_mag = invSqrt(x()*x() + y()*y() + z()*z());
    f32 inv_mag = 1.0f / sqrt(x()*x() + y()*y() + z()*z());
    x() *= inv_mag;
    y() *= inv_mag;
    z() *= inv_mag;
    w() = 1.0f;
}

inline void Vec4::normalizeIfNecessary()
{
    f32 sumSquares = x()*x() + y()*y() + z()*z();
    if (!is_fp_eq(sumSquares, 1.0f))
    {
        f32 inv_mag = 1.0f / sqrt(sumSquares);
        x() *= inv_mag;
        y() *= inv_mag;
        z() *= inv_mag;
        w() = 1.0f;
    }
}

inline Vec4 Vec4::operator-() const
{
    return Vec4(-x(), -y(), -z(), w());
}

inline bool Vec4::operator==(const Vec4 & rhs) const
{
    return x() == rhs.x() && y() == rhs.y() && z() == rhs.z() && w() == rhs.w();
}

inline f32 & Vec4::operator[](size_t idx)
{
    ASSERT(idx < 4);
    return elems[idx];
}

inline const f32 & Vec4::operator[](size_t idx) const
{
    ASSERT(idx < 4);
    return elems[idx];
}

inline f32 & Vec4::x() { return elems[0]; }
inline f32 & Vec4::y() { return elems[1]; }
inline f32 & Vec4::z() { return elems[2]; }
inline f32 & Vec4::w() { return elems[3]; }

inline const f32 & Vec4::x() const { return elems[0]; }
inline const f32 & Vec4::y() const { return elems[1]; }
inline const f32 & Vec4::z() const { return elems[2]; }
inline const f32 & Vec4::w() const { return elems[3]; }

inline Vec4 Vec4::normalize(const Vec4 &vec)
{
    //float inv_mag = invSqrt(x()*x() + y()*y() + z()*z());
    f32 inv_mag = 1.0f / sqrt(vec.x()*vec.x() + vec.y()*vec.y() + vec.z()*vec.z());
    return Vec4(vec.x() * inv_mag, vec.y() * inv_mag, vec.z() * inv_mag, 1.0f);
}

inline f32 Vec4::dot(const Vec3 &lhs, const Vec3 &rhs)
{
    return lhs.x()*rhs.x() + lhs.y()*rhs.y() + lhs.z()*rhs.z();
}

inline Vec4 Vec4::cross(const Vec4 &lhs, const Vec4 &rhs)
{
    Vec4 vec(lhs.y()*rhs.z() - lhs.z()*rhs.y(),
             lhs.z()*rhs.x() - lhs.x()*rhs.z(),
             lhs.x()*rhs.y() - lhs.y()*rhs.x(),
             0.0f);
    return vec;
}

//--------------------------------------
// Quat inlined methods
//--------------------------------------

inline Quat::Quat(f32 x, f32 y, f32 z, f32 w)
{
    elems[0] = x;
    elems[1] = y;
    elems[2] = z;
    elems[3] = w;
}

inline void Quat::normalize()
{
    f32 inv_mag = 1.0f / sqrt(x()*x() + y()*y() + z()*z() + w()*w());
    x() *= inv_mag;
    y() *= inv_mag;
    z() *= inv_mag;
    w() *= inv_mag;
}

inline f32 & Quat::operator[](size_t idx)
{
    ASSERT(idx < 4);
    return elems[idx];
}
inline const f32 & Quat::operator[](size_t idx) const
{
    ASSERT(idx < 4);
    return elems[idx];
}

inline f32 & Quat::x() { return elems[0]; }
inline f32 & Quat::y() { return elems[1]; }
inline f32 & Quat::z() { return elems[2]; }
inline f32 & Quat::w() { return elems[3]; }

inline const f32 & Quat::x() const { return elems[0]; }
inline const f32 & Quat::y() const { return elems[1]; }
inline const f32 & Quat::z() const { return elems[2]; }
inline const f32 & Quat::w() const { return elems[3]; }

inline Quat Quat::operator-() const
{
    return Quat::normalize(Quat(-elems[0], -elems[1], -elems[2], elems[3]));
}

inline Vec3 Quat::operator* (const Vec3 & rhs) const
{
    return Quat::multiply(*this, rhs);
}

inline Quat Quat::operator* (const Quat & rhs) const
{
    return Quat::multiply(*this, rhs);
}

inline Quat Quat::normalize(const Quat & q)
{
    Quat qn = q;
    qn.normalize();
    return qn;
}

inline Quat Quat::from_axis_angle(const Vec3 & dir, f32 angle, bool shouldNormalize)
{
    Vec3 nDir = !shouldNormalize ? dir : Vec3::normalize(dir);

    f32 sin_a = sin(angle / 2);
    f32 cos_a = cos(angle / 2);

    Quat res(nDir.x() * sin_a,
             nDir.y() * sin_a,
             nDir.z() * sin_a,
             cos_a);

    return res;
}

inline void Quat::to_axis_angle(const Quat & q, Vec3 & dir, f32 & angle, bool shouldNormalize)
{
    Quat qn = !shouldNormalize ? q : Quat::normalize(q);
    
}

inline Quat Quat::conjugate(const Quat & q)
{
    return Quat(-q.x(), -q.y(), -q.z(), q.w());
}

inline Quat Quat::multiply(const Quat & qlhs, const Quat & qrhs)
{
    return Quat(qlhs.w() * qrhs.x() + qlhs.x() * qrhs.w() + qlhs.y() * qrhs.z() - qlhs.z() * qrhs.y(),
                qlhs.w() * qrhs.y() - qlhs.x() * qrhs.z() + qlhs.y() * qrhs.w() + qlhs.z() * qrhs.x(),
                qlhs.w() * qrhs.z() + qlhs.x() * qrhs.y() - qlhs.y() * qrhs.x() + qlhs.z() * qrhs.w(),
                qlhs.w() * qrhs.w() - qlhs.x() * qrhs.x() - qlhs.y() * qrhs.y() - qlhs.z() * qrhs.z());
}

inline Vec3 Quat::multiply(const Quat & qlhs, const Vec3 & vrhs)
{
    float x2 = qlhs.x() * 2.0f;
    float y2 = qlhs.y() * 2.0f;
    float z2 = qlhs.z() * 2.0f;
    float xx2 = qlhs.x() * x2;
    float yy2 = qlhs.y() * y2;
    float zz2 = qlhs.z() * z2;
    float xy2 = qlhs.x() * y2;
    float xz2 = qlhs.x() * z2;
    float yz2 = qlhs.y() * z2;
    float wx2 = qlhs.w() * x2;
    float wy2 = qlhs.w() * y2;
    float wz2 = qlhs.w() * z2;

    Vec3 vres;
    vres.x() = (1.0f - (yy2 + zz2)) * vrhs.x() + (xy2 - wz2) * vrhs.y() + (xz2 + wy2) * vrhs.z();
    vres.y() = (xy2 + wz2) * vrhs.x() + (1.0f - (xx2 + zz2)) * vrhs.y() + (yz2 - wx2) * vrhs.z();
    vres.z() = (xz2 - wy2) * vrhs.x() + (yz2 + wx2) * vrhs.y() + (1.0f - (xx2 + yy2)) * vrhs.z();
    return vres;
}

//--------------------------------------
// Mat2 inlined methods
//--------------------------------------
inline Mat2::Mat2() {}
inline Mat2::Mat2(f32 diag)
{
    elems[0] = diag;
    elems[1] = 0.0f;

    elems[2] = 0.0f;
    elems[3] = diag;
}
inline Mat2::Mat2(f32 e0, f32 e1,
                  f32 e2, f32 e3)
{
    elems[0] = e0;
    elems[1] = e1;

    elems[2] = e2;
    elems[3] = e3;
}

inline const Mat2 & Mat2::zero()
{
    static Mat2 zero2{0.0f};
    return zero2;
}

inline const Mat2 & Mat2::identity()
{
    static Mat2 ident2{1.0f};
    return ident2;
}

inline f32 & Mat2::operator[](size_t idx)
{
    ASSERT(idx < 4);
    return elems[idx];
}

inline const f32 & Mat2::operator[](size_t idx) const
{
    ASSERT(idx < 4);
    return elems[idx];
}

//--------------------------------------
// Mat3 inlined methods
//--------------------------------------
inline Mat3::Mat3() {}
inline Mat3::Mat3(f32 diag)
{
    elems[0] = diag;
    elems[1] = 0.0f;
    elems[2] = 0.0f;

    elems[3] = 0.0f;
    elems[4] = diag;
    elems[5] = 0.0f;

    elems[6] = 0.0f;
    elems[7] = 0.0f;
    elems[8] = diag;
}
inline Mat3::Mat3(f32 e0, f32 e1, f32 e2,
                  f32 e3, f32 e4, f32 e5,
                  f32 e6, f32 e7, f32 e8)
{
    elems[0] = e0;
    elems[1] = e1;
    elems[2] = e2;

    elems[3] = e3;
    elems[4] = e4;
    elems[5] = e5;

    elems[6] = e6;
    elems[7] = e7;
    elems[8] = e8;
}

inline Mat3::Mat3(const Mat34 & mat34)
{
    elems[0] = mat34.elems[0];
    elems[1] = mat34.elems[1];
    elems[2] = mat34.elems[2];

    elems[3] = mat34.elems[3];
    elems[4] = mat34.elems[4];
    elems[5] = mat34.elems[5];

    elems[6] = mat34.elems[6];
    elems[7] = mat34.elems[7];
    elems[8] = mat34.elems[8];
}

inline Mat3::Mat3(const Mat4 & mat4)
{
    elems[0] = mat4.elems[0];
    elems[1] = mat4.elems[1];
    elems[2] = mat4.elems[2];

    elems[3] = mat4.elems[4];
    elems[4] = mat4.elems[5];
    elems[5] = mat4.elems[6];

    elems[6] = mat4.elems[8];
    elems[7] = mat4.elems[9];
    elems[8] = mat4.elems[10];
}

inline Mat3::Mat3(const Quat & quat)
{
    f32 xx = quat.x() * quat.x();
    f32 xy = quat.x() * quat.y();
    f32 xz = quat.x() * quat.z();
    f32 xw = quat.x() * quat.w();
    f32 yy = quat.y() * quat.y();
    f32 yz = quat.y() * quat.z();
    f32 yw = quat.y() * quat.w();
    f32 zz = quat.z() * quat.z();
    f32 zw = quat.z() * quat.w();
    elems[0]  = 1.0f - 2.0f * ( yy + zz );
    elems[1]  =        2.0f * ( xy - zw );
    elems[2]  =        2.0f * ( xz + yw );
    elems[3]  =        2.0f * ( xy + zw );
    elems[4]  = 1.0f - 2.0f * ( xx + zz );
    elems[5]  =        2.0f * ( yz - xw );
    elems[6]  =        2.0f * ( xz - yw );
    elems[7]  =        2.0f * ( yz + xw );
    elems[8]  = 1.0f - 2.0f * ( xx + yy );
}

inline const Mat3 & Mat3::zero()
{
    static Mat3 zero3{0.0f};
    return zero3;
}

inline const Mat3 & Mat3::identity()
{
    static Mat3 ident3{1.0f};
    return ident3;
}

inline f32 & Mat3::operator[](size_t idx)
{
    ASSERT(idx < 9);
    return elems[idx];
}

inline const f32 & Mat3::operator[](size_t idx) const
{
    ASSERT(idx < 9);
    return elems[idx];
}

inline Mat3 Mat3::transpose(const Mat3 & mat3)
{
    return Mat3(mat3.elems[0], mat3.elems[3], mat3.elems[6],
                mat3.elems[1], mat3.elems[4], mat3.elems[7],
                mat3.elems[2], mat3.elems[5], mat3.elems[8]);
}

//--------------------------------------
// Mat34 inlined methods
//--------------------------------------
inline Mat34::Mat34() {}
inline Mat34::Mat34(f32 diag)
{
    elems[0]  = diag;
    elems[1]  = 0.0f;
    elems[2]  = 0.0f;

    elems[3]  = 0.0f;
    elems[4]  = diag;
    elems[5]  = 0.0f;

    elems[6]  = 0.0f;
    elems[7]  = 0.0f;
    elems[8]  = diag;

    elems[9]  = 0.0f;
    elems[10] = 0.0f;
    elems[11] = 0.0f;
}
inline Mat34::Mat34(f32 e0, f32 e1,  f32 e2,
                    f32 e3, f32 e4,  f32 e5,
                    f32 e6, f32 e7,  f32 e8,
                    f32 e9, f32 e10, f32 e11)
{
    elems[0]  = e0;
    elems[1]  = e1;
    elems[2]  = e2;

    elems[3]  = e3;
    elems[4]  = e4;
    elems[5]  = e5;

    elems[6]  = e6;
    elems[7]  = e7;
    elems[8]  = e8;

    elems[9]  = e9;
    elems[10] = e10;
    elems[11] = e11;
}
inline Mat34::Mat34(const Mat4 & mat4)
{
    ASSERT_MSG(mat4[3]  == 0.0f &&
               mat4[7]  == 0.0f &&
               mat4[11] == 0.0f &&
               mat4[15] == 1.0f,
               "Incompatible Mat4 array to convert to Mat34");

    elems[0]  = mat4[0];
    elems[1]  = mat4[1];
    elems[2]  = mat4[2];

    elems[3]  = mat4[4];
    elems[4]  = mat4[5];
    elems[5]  = mat4[6];

    elems[6]  = mat4[8];
    elems[7]  = mat4[9];
    elems[8]  = mat4[10];

    elems[9]  = mat4[12];
    elems[10] = mat4[13];
    elems[11] = mat4[14];
}

inline Mat34::Mat34(const Quat & quat)
{
    f32 xx = quat.x() * quat.x();
    f32 xy = quat.x() * quat.y();
    f32 xz = quat.x() * quat.z();
    f32 xw = quat.x() * quat.w();
    f32 yy = quat.y() * quat.y();
    f32 yz = quat.y() * quat.z();
    f32 yw = quat.y() * quat.w();
    f32 zz = quat.z() * quat.z();
    f32 zw = quat.z() * quat.w();
    elems[0]  = 1.0f - 2.0f * ( yy + zz );
    elems[1]  =        2.0f * ( xy - zw );
    elems[2]  =        2.0f * ( xz + yw );
    elems[3]  =        2.0f * ( xy + zw );
    elems[4]  = 1.0f - 2.0f * ( xx + zz );
    elems[5]  =        2.0f * ( yz - xw );
    elems[6]  =        2.0f * ( xz - yw );
    elems[7]  =        2.0f * ( yz + xw );
    elems[8]  = 1.0f - 2.0f * ( xx + yy );
    elems[9]  = 0.0f;
    elems[10] = 0.0f;
    elems[11] = 0.0f;
}

inline const Mat34 & Mat34::zero()
{
    static Mat34 zero34{0.0f};
    return zero34;
}

inline const Mat34 & Mat34::identity()
{
    static Mat34 ident34{1.0f};
    return ident34;
}

inline Mat34 Mat34::transform(const Vec3 & trans,
                              const Vec3 & angles,
                              f32 factor)
{
    return translation(trans) * rotation(angles) * scale(factor);
}

inline f32 & Mat34::operator[](size_t idx)
{
    ASSERT(idx < 12);
    return elems[idx];
}

inline const f32 & Mat34::operator[](size_t idx) const
{
    ASSERT(idx < 12);
    return elems[idx];
}

inline Vec4 Mat34::operator* (const Vec4 & rhs) const
{
    return multiply(*this, rhs);
}

inline Mat34 Mat34::operator* (const Mat34 & rhs) const
{
    return multiply(*this, rhs);
}

inline Mat4 Mat34::operator* (const Mat4 & rhs) const
{
    return multiply(*this, rhs);
}

inline Vec4 & Mat34::vec4(size_t idx)
{
    ASSERT(idx < 3);
    return *reinterpret_cast<Vec4*>(&elems[idx * 4]);
}

inline const Vec4 & Mat34::vec4(size_t idx) const
{
    ASSERT(idx < 3);
    return *reinterpret_cast<const Vec4*>(&elems[idx * 4]);
}


//--------------------------------------
// Mat4 inlined methods
//--------------------------------------
inline Mat4::Mat4() {}
inline Mat4::Mat4(f32 diag)
{
    elems[0]  = 1.0f;
    elems[1]  = 0.0f;
    elems[2]  = 0.0f;
    elems[3]  = 0.0f;

    elems[4]  = 0.0f;
    elems[5]  = 1.0f;
    elems[6]  = 0.0f;
    elems[7]  = 0.0f;

    elems[8]  = 0.0f;
    elems[9]  = 0.0f;
    elems[10] = 1.0f;
    elems[11] = 0.0f;

    elems[12] = 0.0f;
    elems[13] = 0.0f;
    elems[14] = 0.0f;
    elems[15] = 1.0f;
}
inline Mat4::Mat4(f32 e0,  f32 e1,  f32 e2,  f32 e3,
                  f32 e4,  f32 e5,  f32 e6,  f32 e7,
                  f32 e8,  f32 e9,  f32 e10, f32 e11,
                  f32 e12, f32 e13, f32 e14, f32 e15)
{
    elems[0]  = e0;
    elems[1]  = e1;
    elems[2]  = e2;
    elems[3]  = e3;

    elems[4]  = e4;
    elems[5]  = e5;
    elems[6]  = e6;
    elems[7]  = e7;

    elems[8]  = e8;
    elems[9]  = e9;
    elems[10] = e10;
    elems[11] = e11;

    elems[12] = e12;
    elems[13] = e13;
    elems[14] = e14;
    elems[15] = e15;
}
inline Mat4::Mat4(const Mat34 & mat34)
{
    elems[0]  = mat34[0];
    elems[1]  = mat34[1];
    elems[2]  = mat34[2];
    elems[3]  = 0.0f;

    elems[4]  = mat34[3];
    elems[5]  = mat34[4];
    elems[6]  = mat34[5];
    elems[7]  = 0.0f;

    elems[8]  = mat34[6];
    elems[9]  = mat34[7];
    elems[10] = mat34[8];
    elems[11] = 0.0f;

    elems[12] = mat34[9];
    elems[13] = mat34[10];
    elems[14] = mat34[11];
    elems[15] = 1.0f;
}

inline Mat4::Mat4(const Quat & quat)
{
    f32 xx = quat.x() * quat.x();
    f32 xy = quat.x() * quat.y();
    f32 xz = quat.x() * quat.z();
    f32 xw = quat.x() * quat.w();
    f32 yy = quat.y() * quat.y();
    f32 yz = quat.y() * quat.z();
    f32 yw = quat.y() * quat.w();
    f32 zz = quat.z() * quat.z();
    f32 zw = quat.z() * quat.w();
    elems[0]  = 1.0f - 2.0f * ( yy + zz );
    elems[1]  =        2.0f * ( xy - zw );
    elems[2]  =        2.0f * ( xz + yw );
    elems[3]  = 0.0f;
    elems[4]  =        2.0f * ( xy + zw );
    elems[5]  = 1.0f - 2.0f * ( xx + zz );
    elems[6]  =        2.0f * ( yz - xw );
    elems[7]  = 0.0f;
    elems[8]  =        2.0f * ( xz - yw );
    elems[9]  =        2.0f * ( yz + xw );
    elems[10] = 1.0f - 2.0f * ( xx + yy );
    elems[11] = 0.0f;
    elems[12] = 0.0f;
    elems[13] = 0.0f;
    elems[14] = 0.0f;
    elems[15] = 1.0f;
}

inline void Mat4::setTranslation(const Vec3 & trans)
{
    elems[12] = trans.x();
    elems[13] = trans.y();
    elems[14] = trans.z();
}

inline const Mat4 & Mat4::zero()
{
    static Mat4 zero4{0.0f};
    return zero4;
}

inline const Mat4 & Mat4::identity()
{
    static Mat4 ident4{1.0f};
    return ident4;
}

inline Mat4 Mat4::transform(const Vec3 & trans,
                            const Vec3 & angles,
                            f32 factor)
{
    return translation(trans) * rotation(angles) * scale(factor);
}

inline f32 & Mat4::operator[](size_t idx)
{
    ASSERT(idx < 16);
    return elems[idx];
}

inline const f32 & Mat4::operator[](size_t idx) const
{
    ASSERT(idx < 16);
    return elems[idx];
}

inline Vec4 Mat4::operator* (const Vec4 & rhs) const
{
    return multiply(*this, rhs);
}

inline Mat4 Mat4::operator* (const Mat4 & rhs) const
{
    return multiply(*this, rhs);
}

inline Mat4 Mat4::transpose(const Mat4 & mat4)
{
    return Mat4(mat4.elems[0], mat4.elems[4], mat4.elems[8], mat4.elems[12],
                mat4.elems[1], mat4.elems[5], mat4.elems[9], mat4.elems[13],
                mat4.elems[2], mat4.elems[6], mat4.elems[10], mat4.elems[14],
                mat4.elems[3], mat4.elems[7], mat4.elems[11], mat4.elems[15]);
}

//------------------------------------------------------------------------------
// Inline Function definitions (END)
//------------------------------------------------------------------------------


// Sanity checks for sizeof our structs.  Just in case a different compiler/platform pads stuff unexpectedly.
static_assert(sizeof(Vec3) == 12,      "geometry struct has unexpected size");
static_assert(sizeof(Vec3[10]) == 120, "geometry struct has unexpected size");
static_assert(sizeof(Vec4) == 16,      "geometry struct has unexpected size");
static_assert(sizeof(Vec4[10]) == 160, "geometry struct has unexpected size");
static_assert(sizeof(Mat3) == 36,      "geometry struct has unexpected size");
static_assert(sizeof(Mat3[10]) == 360, "geometry struct has unexpected size");
static_assert(sizeof(Mat34) == 48,     "geometry struct has unexpected size");
static_assert(sizeof(Mat34[10]) == 480,"geometry struct has unexpected size");
static_assert(sizeof(Mat4) == 64,      "geometry struct has unexpected size");
static_assert(sizeof(Mat4[10]) == 640, "geometry struct has unexpected size");


} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MATH_H

#endif // #if 0
