//------------------------------------------------------------------------------
// glm_ext.h - Some extensions around glm math library
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

#ifndef GAEN_ENGINE_GLM_EXT_H
#define GAEN_ENGINE_GLM_EXT_H

#include <glm/mat4x3.hpp>

#include "hashes/hashes.h"
#include "core/base_defines.h"

namespace glm
{

static const f32 kFpErrThreshold = 0.00000001f;
inline bool is_fp_eq(f32 actual, f32 expected) { return (actual >= expected - kFpErrThreshold) && (actual <= expected + kFpErrThreshold); }

inline glm::vec3 tri_normal(const glm::vec3 & p0, const glm::vec3 & p1, const glm::vec3 & p2)
{
    glm::vec3 vecA = p1 - p0;
    glm::vec3 vecB = p2 - p0;
    return glm::normalize(glm::cross(vecA, vecB));
}

// We use mat4x3 as a shrunk down 4x4 with an implied identity row.
// Here's our specialized functions that work with this in mind.

// Column Mapping
// 

template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x3<T,P> to_mat3x3(tmat4x4<T,P> const & m)
{
    return tmat3x3<T,P>(tvec3<T,P>(m[0]),
                        tvec3<T,P>(m[1]),
                        tvec3<T,P>(m[2]));
}

template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x4<T,P> to_mat4x4(tmat3x3<T,P> const & m)
{
    return tmat4x4<T,P>(tvec4<T,P>(m[0], 0.0),
                        tvec4<T,P>(m[1], 0.0),
                        tvec4<T,P>(m[2], 0.0),
                        tvec4<T,P>(0.0, 0.0, 0.0, 1.0));
}

template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x3<T,P> to_mat4x3(tmat4x4<T,P> const & m)
{
    return tmat4x3<T,P>(tvec3<T,P>(m[0]),
                        tvec3<T,P>(m[1]), 
                        tvec3<T,P>(m[2]), 
                        tvec3<T,P>(m[3]));
}

template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x4<T,P> to_mat4x4(tmat4x3<T,P> const & m)
{
    return tmat4x4<T,P>(tvec4<T,P>(m[0], 0.0),
                        tvec4<T,P>(m[1], 0.0),
                        tvec4<T,P>(m[2], 0.0),
                        tvec4<T,P>(m[3], 1.0));
}

template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x3<T,P> inverse(tmat4x3<T,P> const & m)
{
    // LORRTODO: Optimize mat43 stuff, for now just convert to and from mat4
    tmat4x4<T,P> m4 = to_mat4x4(m);
    return to_mat4x3(glm::inverse(m4));


/*
  T Coef00 = m[2][2];           // m[2][2] * 1.0 - m[3][2] * 0.0;
  T Coef02 = m[1][2];           // m[1][2] * 1.0 - m[3][2] * 0.0;
    T Coef03 = static_cast<T>(0); // m[1][2] * 0.0 - m[2][2] * 0.0;

    T Coef04 = m[2][1];           // m[2][1] * 1.0 - m[3][1] * 0.0;
    T Coef06 = m[1][1];           // m[1][1] * 1.0 - m[3][1] * 0.0;
    T Coef07 = static_cast<T>(0); // m[1][1] * 0.0 - m[2][1] * 0.0;

    T Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    T Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
    T Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

    T Coef12 = m[2][0];           // m[2][0] * 1.0 - m[3][0] * 0.0;
    T Coef14 = m[1][0];           // m[1][0] * 1.0 - m[3][0] * 0.0;
    T Coef15 = static_cast<T>(0); // m[1][0] * 0.0 - m[2][0] * 0.0;

    T Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    T Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
    T Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

    T Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
    T Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
    T Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

    tvec4<T,P> Fac0(Coef00, Coef00, Coef02, Coef03);
    tvec4<T,P> Fac1(Coef04, Coef04, Coef06, Coef07);
    tvec4<T,P> Fac2(Coef08, Coef08, Coef10, Coef11);
    tvec4<T,P> Fac3(Coef12, Coef12, Coef14, Coef15);
    tvec4<T,P> Fac4(Coef16, Coef16, Coef18, Coef19);
    tvec4<T,P> Fac5(Coef20, Coef20, Coef22, Coef23);

    tvec4<T,P> Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
    tvec4<T,P> Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
    tvec4<T,P> Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
    tvec4<T,P> Vec3(0.0, 0.0, 0.0, 0.0);

    tvec4<T,P> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
    tvec4<T,P> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
    tvec4<T,P> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
    tvec4<T,P> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

    tvec4<T,P> SignA(+1, -1, +1, -1);
    tvec4<T,P> SignB(-1, +1, -1, +1);
    tmat4x4<T,P> Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

    tvec4<T,P> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

    tvec4<T,P> Dot0(tvec4<T,P>(m[0], static_cast<T>(0)) * Row0);
    T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

    T OneOverDeterminant = static_cast<T>(1) / Dot1;

    return to_mat4x3(Inverse * OneOverDeterminant);
    */
}

template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x3<T,P> operator*(tmat4x3<T,P> const & m1, tmat4x3<T,P> const & m2)
{
    // LORRTODO: Optimize mat43 stuff, for now just convert to and from mat4
    tmat4x4<T,P> m41 = to_mat4x4(m1);
    tmat4x4<T,P> m42 = to_mat4x4(m2);
    return to_mat4x3<T,P>(m41 * m42);

/*
    typename tmat4x4<T,P>::col_type const SrcA0 = tvec4<T,P>(m1[0], static_cast<T>(0));
    typename tmat4x4<T,P>::col_type const SrcA1 = tvec4<T,P>(m1[1], static_cast<T>(0));
    typename tmat4x4<T,P>::col_type const SrcA2 = tvec4<T,P>(m1[2], static_cast<T>(0));
    typename tmat4x4<T,P>::col_type const SrcA3 = tvec4<T,P>(m1[3], static_cast<T>(1));

    typename tmat4x4<T,P>::col_type const SrcB0 = tvec4<T,P>(m2[0], static_cast<T>(0));
    typename tmat4x4<T,P>::col_type const SrcB1 = tvec4<T,P>(m2[1], static_cast<T>(0));
    typename tmat4x4<T,P>::col_type const SrcB2 = tvec4<T,P>(m2[2], static_cast<T>(0));
    typename tmat4x4<T,P>::col_type const SrcB3 = tvec4<T,P>(m2[3], static_cast<T>(1));

    tmat4x4<T,P> Result(uninitialize);
    Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
    Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
    Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
    Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
    return to_mat4x3(Result);
*/
}

template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x3<T,P> mat3_rotation(glm::tvec3<T,P> angles)
{
    tmat4x4<T,P> m4(1.0f);
    m4 = glm::rotate(m4, angles.x, glm::tvec3<T,P>(1.0f, 0.0f, 0.0f));
    m4 = glm::rotate(m4, angles.y, glm::tvec3<T,P>(0.0f, 1.0f, 0.0f));
    m4 = glm::rotate(m4, angles.z, glm::tvec3<T,P>(0.0f, 0.0f, 1.0f));
    return to_mat3x3(m4);
}

template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x3<T,P> mat43_rotation(glm::tvec3<T,P> angles)
{
    tmat4x4<T,P> m4(1.0f);
    m4 = glm::rotate(m4, angles.x, glm::tvec3<T,P>(1.0f, 0.0f, 0.0f));
    m4 = glm::rotate(m4, angles.y, glm::tvec3<T,P>(0.0f, 1.0f, 0.0f));
    m4 = glm::rotate(m4, angles.z, glm::tvec3<T,P>(0.0f, 0.0f, 1.0f));
    return to_mat4x3(m4);
}

template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x3<T,P> mat43_position(glm::tvec3<T,P> pos)
{
    tmat4x4<T,P> m4(1.0f);
    m4[3] = glm::tvec4<T,P>(pos, 0);
    return to_mat4x3(m4);
}

template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x3<T,P> mat43_transform(glm::tvec3<T,P> pos, glm::tvec3<T,P> angles)
{
    tmat4x4<T,P> m4(1.0f);
    m4 = glm::rotate(m4, angles.x, glm::tvec3<T,P>(1.0f, 0.0f, 0.0f));
    m4 = glm::rotate(m4, angles.y, glm::tvec3<T,P>(0.0f, 1.0f, 0.0f));
    m4 = glm::rotate(m4, angles.z, glm::tvec3<T,P>(0.0f, 0.0f, 1.0f));
    m4[3] = glm::tvec4<T,P>(pos, 0);
    return to_mat4x3(m4);
}


} // namespace glm


namespace std
{
    template <>
    struct hash<glm::vec3> : public unary_function<glm::vec3, size_t>
    {
        size_t operator()(const glm::vec3& value) const
        {
            return gaen::fnv1a_32(reinterpret_cast<const gaen::u8*>(&value), sizeof(glm::vec3));
        }
    };
}


#endif // #ifndef GAEN_ENGINE_GLM_EXT_H
