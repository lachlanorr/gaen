//------------------------------------------------------------------------------
// math.h - Vector and matrix math
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#ifndef GAEN_CORE_MATH_H
#define GAEN_CORE_MATH_H

#include "core/base_defines.h"

namespace gaen
{

static const f32 kPi = 3.1415926535897932384626433832795f;
inline f32 deg2rad(f32 deg) { return deg * kPi * (1.0f / 180.0f); }

static const f32 kFpErrThreshold = 0.00000001f;
inline bool is_fp_eq(f32 val, f32 expected) { return (val >= expected - kFpErrThreshold) && (val <= expected + kFpErrThreshold); }

f32 invSqrt(const f32 x);
f32 fastSqrt(const f32 x);


struct Vec3
{
    // constructors
    Vec3();
    Vec3(f32 x, f32 y, f32 z);
    Vec3(const struct Vec4 &rhs);

    f32 length() const;
    void normalize();
    void normalizeIfNecessary();

    static f32 dot(const Vec3 &lhs, const Vec3 &rhs);
    static Vec3 cross(const Vec3 &lhs, const Vec3 &rhs);

    f32 x;
    f32 y;
    f32 z;
};

struct Vec4
{
    Vec4();
    Vec4(f32 x, f32 y, f32 z, f32 w);
    Vec4(const Vec3 &vec3, f32 w);

    f32 length() const;
    void normalize();
    void normalizeIfNecessary();

    static f32 dot(const Vec3 &lhs, const Vec3 &rhs);

    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

struct Mat4;
struct Mat3
{
    static Mat3 zero();
    static Mat3 identity();

    static f32 determinant(const Mat3 & mat3);
    static Mat3 inverse(const Mat3 & mat3);

    // build a 3x3 matrix to use to transform normals
    static Mat3 modelViewToNormalTransform(const Mat4 & mv);

    f32 & operator[](size_t idx);
    const f32 & operator[](size_t idx) const;

    f32 elems[9];
};


struct Mat34
{
    Mat34();
    Mat34(const Mat4 & mat4);

    static Mat34 zero();
    static Mat34 identity();

    static Vec4 multiply(const Mat34 & mat34, const Vec4 & vec4);
    static Mat34 multiply(const Mat34 & lhs, const Mat34 & rhs);

    static f32 determinant(const Mat34 & mat34);
    static Mat34 inverse(const Mat34 & mat34);

    static Mat34 buildTranslation(f32 x, f32 y, f32 z);
    static Mat34 buildRotation(f32 x, f32 y, f32 z);
    static Mat34 buildScale(f32 factor);
    static Mat34 buildTransform(f32 translateX, f32 translateY, f32 translateZ,
                                f32 rotationX, f32 rotationY, f32 rotationZ,
                                f32 scaleFactor);


    // Convenience functions supporting vecto rparams
    static Mat34 buildTranslation(Vec3 &vec);
    static Mat34 buildRotation(Vec3 &vec);
    static Mat34 buildTransform(Vec3 translateVec, Vec3 rotationVec, f32 scaleFactor);

    f32 & operator[](size_t idx);
    const f32 & operator[](size_t idx) const;

    f32 elems[12];
};

struct Mat4
{
    Mat4();
    Mat4(const Mat34 & mat34);

    static Mat4 zero();
    static Mat4 identity();

    static Vec4 multiply(const Mat4 & mat4, const Vec4 & vec4);
    static Mat4 multiply(const Mat4 & lhs, const Mat4 & rhs);

    static f32 determinant(const Mat4 & mat4);
    static Mat4 inverse(const Mat4 & mat4);

    static Mat4 buildTranslation(f32 x, f32 y, f32 z);
    static Mat4 buildRotation(f32 x, f32 y, f32 z);
    static Mat4 buildScale(f32 factor);
    static Mat4 buildTransform(f32 translateX, f32 translateY, f32 translateZ,
                               f32 rotationX, f32 rotationY, f32 rotationZ,
                               f32 scaleFactor);


    // Convenience functions supporting vector rparams
    static Mat4 buildTranslation(Vec3 &vec);
    static Mat4 buildRotation(Vec3 &vec);
    static Mat4 buildTransform(Vec3 translateVec, Vec3 rotationVec, f32 scaleFactor);


    static Mat4 frustum(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ);
    static Mat4 perspective(f32 fovy, f32 aspect, f32 nearZ, f32 farZ);
    static Mat4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ);

    f32 & operator[](size_t idx);
    const f32 & operator[](size_t idx) const;

    f32 elems[16];
};



//------------------------------------------------------------------------------
// Vertex structs
//------------------------------------------------------------------------------
struct VertexCOLR
{
    Vec3 position;
    Vec3 normal;
    Vec3 color;
};

struct VertexTXTR
{
    Vec3 position;
    Vec3 normal;
    f32 u;
    f32 v;
};

struct VertexNRML
{
    Vec3 position;
    Vec3 normal;
    f32 u;
    f32 v;
    Vec4 tangent;
};

const unsigned int kOffsetPosition = 0;
const unsigned int kOffsetNormal   = kOffsetPosition + sizeof(Vec3);
const unsigned int kOffsetUV       = kOffsetNormal + sizeof(Vec3);
const unsigned int kOffsetTangent  = kOffsetUV + sizeof(f32) + sizeof(f32);


struct Triangle
{
    // a triangle is composed of 3 indices into point array
    Triangle(u16 p0, u16 p1, u16 p2)
      : p0(p0), p1(p1), p2(p2) {}

    u16 p0;
    u16 p1;
    u16 p2;
};

struct Line
{
    Line(u16 p0, u16 p1)
      : p0(p0), p1(p1) {}

    // a line is composed of 2 indices into point array
    u16 p0;
    u16 p1;
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
// Vec3 inlined methods
//--------------------------------------
inline Vec3::Vec3(f32 x, f32 y, f32 z)
  : x(x)
  , y(y)
  , z(z) {}

inline Vec3::Vec3(const Vec4 &vec4)
  : x(vec4.x)
  , y(vec4.y)
  , z(vec4.z) {}

inline f32 Vec3::length() const
{
    return fastSqrt(x*x + y*y + z*z);
}

inline void Vec3::normalize()
{
    //float inv_mag = invSqrt(x*x + y*y + z*z);
    f32 inv_mag = 1.0f / sqrt(x*x + y*y + z*z);
    x = x * inv_mag;
    y = y * inv_mag;
    z = z * inv_mag;
}

inline void Vec3::normalizeIfNecessary()
{
    f32 sumSquares = x*x + y*y + z*z;
    if (!is_fp_eq(sumSquares, 1.0f))
    {
        f32 inv_mag = 1.0f / sqrt(sumSquares);
        x = x * inv_mag;
        y = y * inv_mag;
        z = z * inv_mag;
    }
}

inline f32 Vec3::dot(const Vec3 &lhs, const Vec3 &rhs)
{
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

inline Vec3 Vec3::cross(const Vec3 &lhs, const Vec3 &rhs)
{
    Vec3 vec(lhs.y*rhs.z - lhs.z*rhs.y,
             lhs.z*rhs.x - lhs.x*rhs.z,
             lhs.x*rhs.y - lhs.y*rhs.x);
    return vec;
}



//--------------------------------------
// Vec4 inlined methods
//--------------------------------------
inline Vec4::Vec4(f32 x, f32 y, f32 z, f32 w) 
  : x(x)
  , y(y)
  , z(z)
  , w(w) {}

inline Vec4::Vec4(const Vec3 &vec3, f32 w)
  : x(vec3.x)
  , y(vec3.y)
  , z(vec3.z)
  , w(w) {}

inline f32 Vec4::length() const 
{ 
    return fastSqrt(x*x + y*y + z*z + w*w);
}

inline void Vec4::normalize()
{
    //float inv_mag = invSqrt(x*x + y*y + z*z);
    f32 inv_mag = 1.0f / sqrt(x*x + y*y + z*z);
    x = x * inv_mag;
    y = y * inv_mag;
    z = z * inv_mag;
    w = 1.0f;
}

inline void Vec4::normalizeIfNecessary()
{
    f32 sumSquares = x*x + y*y + z*z;
    if (!is_fp_eq(sumSquares, 1.0f))
    {
        f32 inv_mag = 1.0f / sqrt(sumSquares);
        x = x * inv_mag;
        y = y * inv_mag;
        z = z * inv_mag;
        w = 1.0f;
    }
}

inline f32 Vec4::dot(const Vec3 &lhs, const Vec3 &rhs)
{
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}


//--------------------------------------
// Mat3 inlined methods
//--------------------------------------
inline Mat3 Mat3::zero()
{
    Mat3 mat3;
    memset(&mat3, 0, sizeof(Mat3));
    return mat3;
}

inline Mat3 Mat3::identity()
{
    Mat3 mat3;
    mat3[0] = 1.0f;
    mat3[1] = 0.0f;
    mat3[2] = 0.0f;

    mat3[3] = 0.0f;
    mat3[4] = 1.0f;
    mat3[5] = 0.0f;

    mat3[6] = 0.0f;
    mat3[7] = 0.0f;
    mat3[8] = 1.0f;
    return mat3;
}

f32 & Mat3::operator[](size_t idx)
{
    ASSERT(idx < 9);
    return elems[idx];
}

const f32 & Mat3::operator[](size_t idx) const
{
    ASSERT(idx < 9);
    return elems[idx];
}

//--------------------------------------
// Mat34 inlined methods
//--------------------------------------
inline Mat34::Mat34() {}
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

inline Mat34 Mat34::zero()
{
    Mat34 mat34;
    memset(&mat34, 0, sizeof(Mat34));
    return mat34;
}

inline Mat34 Mat34::identity()
{
    Mat34 mat34;
    mat34[0]  = 1.0f;
    mat34[1]  = 0.0f;
    mat34[2]  = 0.0f;

    mat34[3]  = 0.0f;
    mat34[4]  = 1.0f;
    mat34[5]  = 0.0f;

    mat34[6]  = 0.0f;
    mat34[7]  = 0.0f;
    mat34[8]  = 1.0f;

    mat34[9]  = 0.0f;
    mat34[10] = 0.0f;
    mat34[11] = 0.0f;
    return mat34;
}

inline Mat34 Mat34::buildTranslation(Vec3 &vec)
{
    return buildTranslation(vec.x, vec.y, vec.z);
}

inline Mat34 Mat34::buildRotation(Vec3 &vec)
{
    return buildRotation(vec.x, vec.y, vec.z);
}

inline Mat34 Mat34::buildTransform(Vec3 translateVec, Vec3 rotationVec, f32 scaleFactor)
{
    return buildTransform(translateVec.x, translateVec.y, translateVec.z,
                          rotationVec.x, rotationVec.y, rotationVec.z,
                          scaleFactor);
}

f32 & Mat34::operator[](size_t idx)
{
    ASSERT(idx < 12);
    return elems[idx];
}

const f32 & Mat34::operator[](size_t idx) const
{
    ASSERT(idx < 12);
    return elems[idx];
}


//--------------------------------------
// Mat4 inlined methods
//--------------------------------------
inline Mat4::Mat4() {}
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

inline Mat4 Mat4::zero()
{
    Mat4 mat4;
    memset(&mat4, 0, sizeof(Mat4));
    return mat4;
}

inline Mat4 Mat4::identity()
{
    Mat4 mat4;
    mat4[0]  = 1.0f;
    mat4[1]  = 0.0f;
    mat4[2]  = 0.0f;
    mat4[3]  = 0.0f;

    mat4[4]  = 0.0f;
    mat4[5]  = 1.0f;
    mat4[6]  = 0.0f;
    mat4[7]  = 0.0f;

    mat4[8]  = 0.0f;
    mat4[9]  = 0.0f;
    mat4[10] = 1.0f;
    mat4[11] = 0.0f;

    mat4[12] = 0.0f;
    mat4[13] = 0.0f;
    mat4[14] = 0.0f;
    mat4[15] = 1.0f;
    return mat4;
}

inline Mat4 Mat4::buildTranslation(Vec3 &vec)
{
    return buildTranslation(vec.x, vec.y, vec.z);
}

inline Mat4 Mat4::buildRotation(Vec3 &vec)
{
    return buildRotation(vec.x, vec.y, vec.z);
}

inline Mat4 Mat4::buildTransform(Vec3 translateVec, Vec3 rotationVec, f32 scaleFactor)
{
    return buildTransform(translateVec.x, translateVec.y, translateVec.z,
                          rotationVec.x, rotationVec.y, rotationVec.z,
                          scaleFactor);
}

f32 & Mat4::operator[](size_t idx)
{
    ASSERT(idx < 16);
    return elems[idx];
}

const f32 & Mat4::operator[](size_t idx) const
{
    ASSERT(idx < 16);
    return elems[idx];
}


//-------------------------------------------
// Comparison operators for Polygon and Line
//-------------------------------------------
inline bool operator== (const Triangle & lhs, const Triangle & rhs)
{
    return lhs.p0 == rhs.p0 &&
           lhs.p1 == rhs.p1 &&
           lhs.p2 == rhs.p2;
}

inline bool operator< (const Triangle & lhs, const Triangle & rhs)
{
    if (lhs.p0 != rhs.p0)
        return lhs.p0 < rhs.p0;
    else if (lhs.p1 != rhs.p1)
        return lhs.p1 < rhs.p1;
    else if (lhs.p2 != rhs.p2)
        return lhs.p2 < rhs.p2;

    // if we get here, all elements match
    return false;
}

inline bool operator== (const Line & lhs, const Line & rhs)
{
    // do a simple sort of points to consider two lines equal even if
    // their points are in different order
    u16 line0p0, line0p1, line1p0, line1p1;

    if (lhs.p0 < lhs.p1)
    {
        line0p0 = lhs.p0;
        line0p1 = lhs.p1;
    }
    else
    {
        line0p0 = lhs.p1;
        line0p1 = lhs.p0;
    }

    if (rhs.p0 <= rhs.p1)
    {
        line1p0 = rhs.p0;
        line1p1 = rhs.p1;
    }
    else
    {
        line1p0 = rhs.p1;
        line1p1 = rhs.p0;
    }
    

    return line0p0 == line1p0 &&
           line0p1 == line1p1;
}

inline bool operator< (const Line & lhs, const Line & rhs)
{
    // do a simple sort of points to consider two lines equal even if
    // their points are in different order
    u16 line0p0, line0p1, line1p0, line1p1;

    if (lhs.p0 < lhs.p1)
    {
        line0p0 = lhs.p0;
        line0p1 = lhs.p1;
    }
    else
    {
        line0p0 = lhs.p1;
        line0p1 = lhs.p0;
    }

    if (rhs.p0 <= rhs.p1)
    {
        line1p0 = rhs.p0;
        line1p1 = rhs.p1;
    }
    else
    {
        line1p0 = rhs.p1;
        line1p1 = rhs.p0;
    }
    

    if (line0p0 != line1p0)
        return line0p0 < line1p0;
    else if (line0p1 != line1p1)
        return line0p1 < line1p1;

    // if we get here, all elements match
    return false;
}



//------------------------------------------------------------------------------
// Inline Function definitions (END)
//------------------------------------------------------------------------------


// Sanity checks for sizeof our structs.  Just in case a different compiler/platform pads stuff unexpectedly.
static_assert(sizeof(Vec3) == 12,            "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Vec3[10]) == 120,       "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Vec4) == 16,            "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Vec4[10]) == 160,       "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Mat3) == 36,            "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Mat3[10]) == 360,       "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Mat34) == 48,           "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Mat34[10]) == 480,      "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Mat4) == 64,            "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Mat4[10]) == 640,       "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(VertexCOLR) == 36,      "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(VertexCOLR[10]) == 360, "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(VertexTXTR) == 32,      "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(VertexTXTR[10]) == 320, "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(VertexNRML) == 48,      "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(VertexNRML[10]) == 480, "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Triangle) == 6,         "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Triangle[10]) == 60,    "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Line) == 4,             "sanity check for sizeof geometry struct failed... might need padding pragma");
static_assert(sizeof(Line[10]) == 40,        "sanity check for sizeof geometry struct failed... might need padding pragma");


} // namespace gaen

#endif // #ifndef GAEN_CORE_GEOMETRY_H
