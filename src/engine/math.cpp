//------------------------------------------------------------------------------
// math.cpp - Vector and matrix math
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

#include "engine/stdafx.h"

#include "engine/math.h"

namespace gaen
{

//------------------------------------------------------------------------------
// Mat3 methods
//------------------------------------------------------------------------------
f32 Mat3::determinant(const Mat3 & mat3)
{
    return mat3[0] * (mat3[4] * mat3[8] - mat3[7] * mat3[5]) +
           mat3[3] * (mat3[7] * mat3[2] - mat3[1] * mat3[8]) +
           mat3[6] * (mat3[1] * mat3[5] - mat3[4] * mat3[2]);
}


Mat3 Mat3::inverse(const Mat3 & mat3)
{
    // SIMPLE TEST CASE
    // 
    // 1  3  1       2  9 -5
    // 1  1  2  ->   0 -2  1
    // 2  3  4      -1 -3  2
    //


    // Calc determinant.  We will reuse some of the terms, so we calc
    // the determinant here rather than using determinant method.
    f32 term48minus75 = mat3[4] * mat3[8] - mat3[7] * mat3[5];
    f32 term72minus18 = mat3[7] * mat3[2] - mat3[1] * mat3[8];
    f32 term15minus42 = mat3[1] * mat3[5] - mat3[4] * mat3[2];

    f32 det = mat3[0] * term48minus75 +
              mat3[3] * term72minus18 +
              mat3[6] * term15minus42;
    
    f32 invDet = 1.0f / det;

    Mat3 inv;

    inv[0] = term48minus75 * invDet;
    inv[1] = term72minus18 * invDet;
    inv[2] = term15minus42 * invDet;
    
    inv[3] = (mat3[6] * mat3[5] - mat3[3] * mat3[8]) * invDet;
    inv[4] = (mat3[0] * mat3[8] - mat3[6] * mat3[2]) * invDet;
    inv[5] = (mat3[3] * mat3[2] - mat3[0] * mat3[5]) * invDet;

    inv[6] = (mat3[3] * mat3[7] - mat3[6] * mat3[4]) * invDet;
    inv[7] = (mat3[6] * mat3[1] - mat3[0] * mat3[7]) * invDet;
    inv[8] = (mat3[0] * mat3[4] - mat3[3] * mat3[1]) * invDet;
    
    return inv;
}

Mat3 Mat3::model_view_to_normal_transform(const Mat4 & mv)
{
    // Calculate the transpose of the inverse of the top 3x3 of
    // modelView.  This 3x3 can be used to transform normals without
    // messing them up with translation or scaling.

    Mat3 mat3;
    
    // Calc determinant.  We will reuse some of the terms, so we calc
    // the determinant here rather than using determinant method.
    // Names of these vars refer to 3x3 indices, but remember we're
    // pulling them straight out of the 4x4 model view.
    f32 term48minus75 = mv[5] * mv[10] - mv[9] * mv[6];
    f32 term72minus18 = mv[9] * mv[2]  - mv[1] * mv[10];
    f32 term15minus42 = mv[1] * mv[6]  - mv[5] * mv[2];

    f32 det = mv[0] * term48minus75 +
              mv[4] * term72minus18 +
              mv[8] * term15minus42;
    
    f32 invDet = 1.0f / det;

    // Basically calc inverse, but since we need to transpose as well,
    // we set results into transposed indices in the same step.
    mat3[0] = term48minus75 * invDet;
    mat3[3] = term72minus18 * invDet;
    mat3[6] = term15minus42 * invDet;
    
    mat3[1] = (mv[8] * mv[6] - mv[4] * mv[10]) * invDet;
    mat3[4] = (mv[0] * mv[10] - mv[8] * mv[2]) * invDet;
    mat3[7] = (mv[4] * mv[2] - mv[0] * mv[6]) * invDet;

    mat3[2] = (mv[4] * mv[9] - mv[8] * mv[5]) * invDet;
    mat3[5] = (mv[8] * mv[1] - mv[0] * mv[9]) * invDet;
    mat3[8] = (mv[0] * mv[5] - mv[4] * mv[1]) * invDet;
    
    return mat3;
}
//------------------------------------------------------------------------------
// Mat3 methods (END)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Mat34 methods
//------------------------------------------------------------------------------
Vec4 Mat34::multiply(const Mat34 & mat34, const Vec4 & vec4)
{
    Vec4 newVec;

    newVec.x = mat34[0]  * vec4.x +
               mat34[1]  * vec4.y +
               mat34[2]  * vec4.z;

    newVec.y = mat34[3]  * vec4.x +
               mat34[4]  * vec4.y +
               mat34[5]  * vec4.z;

    newVec.z = mat34[6]  * vec4.x +
               mat34[7]  * vec4.y +
               mat34[8]  * vec4.z;

    newVec.w = mat34[9]  * vec4.x +
               mat34[10] * vec4.y +
               mat34[11] * vec4.z +
               vec4.w;
    
    return newVec;
}

Mat34 Mat34::multiply(const Mat34 & lhs, const Mat34 & rhs)
{
    Mat34 mat34;

    // first row
    mat34[0] =  lhs[0]  *  rhs[0] +
                lhs[4]  *  rhs[1] +
                lhs[8]  *  rhs[2];
    
    mat34[4] =  lhs[0]  *  rhs[4] +
                lhs[4]  *  rhs[5] +
                lhs[8]  *  rhs[6];
    
    mat34[8] =  lhs[0]  *  rhs[8] +
                lhs[4]  *  rhs[9] +
                lhs[8]  *  rhs[10];
    
    mat34[12] = lhs[0]  *  rhs[12] +
                lhs[4]  *  rhs[13] +
                lhs[8]  *  rhs[14] +
                lhs[12];
    
    // second row
    mat34[1] =  lhs[1]  *  rhs[0] +
                lhs[5]  *  rhs[1] +
                lhs[9]  *  rhs[2];
    
    mat34[5] =  lhs[1]  *  rhs[4] +
                lhs[5]  *  rhs[5] +
                lhs[9]  *  rhs[6];
     
    mat34[9] =  lhs[1]  *  rhs[8] +
                lhs[5]  *  rhs[9] +
                lhs[9]  * rhs[10];
    
    mat34[13] = lhs[1]  * rhs[12] +
                lhs[5]  * rhs[13] +
                lhs[9]  * rhs[14] +
                lhs[13];
    
    // third row
    mat34[2] =  lhs[2]  *  rhs[0] +
                lhs[6]  *  rhs[1] +
                lhs[10] *  rhs[2];
    
    mat34[6] =  lhs[2]  *  rhs[4] +
                lhs[6]  *  rhs[5] +
                lhs[10] *  rhs[6];
     
    mat34[10] = lhs[2]  *  rhs[8] +
                lhs[6]  *  rhs[9] +
                lhs[10] * rhs[10];
    
    mat34[14] = lhs[2]  * rhs[12] +
                lhs[6]  * rhs[13] +
                lhs[10] * rhs[14] +
                lhs[14];
    
    // fourth row
    mat34[3] =  lhs[3]  *  rhs[0] +
                lhs[7]  *  rhs[1] +
                lhs[11] *  rhs[2];
    
    mat34[7] =  lhs[3]  *  rhs[4] +
                lhs[7]  *  rhs[5] +
                lhs[11] *  rhs[6];
     
    mat34[11] = lhs[3]  *  rhs[8] +
                lhs[7]  *  rhs[9] +
                lhs[11] * rhs[10];
    
    mat34[15] = lhs[3]  * rhs[12] +
                lhs[7]  * rhs[13] +
                lhs[11] * rhs[14] +
                1.0f;

    return mat34;
}

f32 Mat34::determinant(const Mat34 & mat34)
{
    f32 a0 = mat34[0]  * mat34[4]  - mat34[3]  * mat34[1];
    f32 a1 = mat34[0]  * mat34[7]  - mat34[6]  * mat34[1];
    //f32 a2 = mat34[0]  * mat34[10] - mat34[9] * mat34[1];
    f32 a3 = mat34[3]  * mat34[7]  - mat34[6]  * mat34[4];
    //f32 a4 = mat34[3]  * mat34[10] - mat34[9] * mat34[4];
    //f32 a5 = mat34[6]  * mat34[10] - mat34[9] * mat34[7];
    //f32 b0 = 0.0f;
    //f32 b1 = 0.0f;
    f32 b2 = mat34[2];
    //f32 b3 = 0.0f;
    f32 b4 = mat34[5];
    f32 b5 = mat34[8];

    float det = a0 * b5 - a1 * b4 + a3 * b2;
    ASSERT(!is_fp_eq(det, 0.0f));
    return det;
}

Mat34 Mat34::inverse(const Mat34 & mat34)
{
    f32 a0 = mat34[0]  * mat34[4]  - mat34[3] * mat34[1];
    f32 a1 = mat34[0]  * mat34[7]  - mat34[6] * mat34[1];
    f32 a2 = mat34[0]  * mat34[10] - mat34[9] * mat34[1];
    f32 a3 = mat34[3]  * mat34[7]  - mat34[6] * mat34[4];
    f32 a4 = mat34[3]  * mat34[10] - mat34[9] * mat34[4];
    f32 a5 = mat34[6]  * mat34[10] - mat34[9] * mat34[7];
    //f32 b0 = 0.0f;
    //f32 b1 = 0.0f;
    f32 b2 = mat34[2];
    //f32 b3 = 0.0f;
    f32 b4 = mat34[5];
    f32 b5 = mat34[8];

    f32 det = a0 * b5 - a1 * b4 + a3 * b2;
    ASSERT(!is_fp_eq(det, 0.0f));
    f32 invDet = 1.0f / det;

    Mat34 inv;

    inv[0]  = invDet * ( mat34[4] * b5 - mat34[7] * b4);
    inv[1]  = invDet * (-mat34[1] * b5 + mat34[7] * b2);
    inv[2]  = invDet * ( mat34[1] * b4 - mat34[4] * b2);

    inv[3]  = invDet * (-mat34[3] * b5 + mat34[6] * b4);
    inv[4]  = invDet * ( mat34[0] * b5 - mat34[6] * b2);
    inv[5]  = invDet * (-mat34[0] * b4 + mat34[3] * b2);

    inv[6]  = invDet * a3;
    inv[7]  = invDet * -a1;
    inv[8]  = invDet * a0;

    inv[9]  = invDet * (-mat34[5] * a5 + mat34[8] * a4 - mat34[11] * a3);
    inv[10] = invDet * ( mat34[2] * a5 - mat34[8] * a2 + mat34[11] * a1);
    inv[11] = invDet * (-mat34[2] * a4 + mat34[5] * a2 - mat34[11] * a0);

    return inv;
}


Mat34 Mat34::build_translation(const Vec3 & pos)
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

    mat34[9]  = pos.x;
    mat34[10] = pos.y;
    mat34[11] = pos.z;

    return mat34;
}

Mat34 Mat34::build_rotation(const Vec3 & rot)
{
    f32 cosX = cos(rot.x);
    f32 sinX = sin(rot.x);
    f32 cosY = cos(rot.y);
    f32 sinY = sin(rot.y);
    f32 cosZ = cos(rot.z);
    f32 sinZ = sin(rot.z);
    f32 cosXsinY = cosX * sinY;
    f32 sinXsinY = sinX * sinY;

    Mat4 mat4;

    mat4[0]  = cosY * cosZ;
    mat4[1]  = -cosY * sinZ;
    mat4[2]  = sinY;

    mat4[3]  = sinXsinY * cosZ + cosX * sinZ;
    mat4[4]  = -sinXsinY * sinZ + cosX * cosZ;
    mat4[5]  = -sinX * cosY;
    
    mat4[6]  = -cosXsinY * cosZ + sinX * sinZ;
    mat4[7]  = cosXsinY * sinZ + sinX * cosZ;
    mat4[8]  = cosX * cosY;

    mat4[9]  = 0.0f;
    mat4[10] = 0.0f;
    mat4[11] = 0.0f;

    return mat4;
}

Mat34 Mat34::build_scale(f32 scale)
{
    Mat34 mat34;
    mat34[0]  = scale;
    mat34[1]  = 0.0f;
    mat34[2]  = 0.0f;

    mat34[3]  = 0.0f;
    mat34[4]  = scale;
    mat34[5]  = 0.0f;

    mat34[6]  = 0.0f;
    mat34[7]  = 0.0f;
    mat34[8]  = scale;

    mat34[9]  = 0.0f;
    mat34[10] = 0.0f;
    mat34[11] = 0.0f;
    return mat34;
}

//------------------------------------------------------------------------------
// Mat34 methods (END)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Mat4 methods
//------------------------------------------------------------------------------
Vec4 Mat4::multiply(const Mat4 & mat4, const Vec4 & vec4)
{
    Vec4 newVec;

    newVec.x = mat4[0]  * vec4.x +
               mat4[1]  * vec4.y +
               mat4[2]  * vec4.z +
               mat4[3]  * vec4.w;

    newVec.y = mat4[4]  * vec4.x +
               mat4[5]  * vec4.y +
               mat4[6]  * vec4.z +
               mat4[7]  * vec4.w;

    newVec.z = mat4[8]  * vec4.x +
               mat4[9]  * vec4.y +
               mat4[10] * vec4.z +
               mat4[11] * vec4.w;

    newVec.w = mat4[12] * vec4.x +
               mat4[13] * vec4.y +
               mat4[14] * vec4.z +
               mat4[15] * vec4.w;

    return newVec;
}


Mat4 Mat4::multiply(const Mat4 & lhs, const Mat4 & rhs)
{
    Mat4 mat4;

    // first row
    mat4[0] =  lhs[0]  *  rhs[0] +
               lhs[4]  *  rhs[1] +
               lhs[8]  *  rhs[2] +
               lhs[12] *  rhs[3];
    
    mat4[4] =  lhs[0]  *  rhs[4] +
               lhs[4]  *  rhs[5] +
               lhs[8]  *  rhs[6] +
               lhs[12] *  rhs[7];
    
    mat4[8] =  lhs[0]  *  rhs[8] +
               lhs[4]  *  rhs[9] +
               lhs[8]  * rhs[10] +
               lhs[12] * rhs[11];
    
    mat4[12] = lhs[0]  * rhs[12] +
               lhs[4]  * rhs[13] +
               lhs[8]  * rhs[14] +
               lhs[12] * rhs[15];
    
    // second row
    mat4[1] =  lhs[1]  *  rhs[0] +
               lhs[5]  *  rhs[1] +
               lhs[9]  *  rhs[2] +
               lhs[13] *  rhs[3];
    
    mat4[5] =  lhs[1]  *  rhs[4] +
               lhs[5]  *  rhs[5] +
               lhs[9]  *  rhs[6] +
               lhs[13] *  rhs[7];
     
    mat4[9] =  lhs[1]  *  rhs[8] +
               lhs[5]  *  rhs[9] +
               lhs[9]  * rhs[10] +
               lhs[13] * rhs[11];
    
    mat4[13] = lhs[1]  * rhs[12] +
               lhs[5]  * rhs[13] +
               lhs[9]  * rhs[14] +
               lhs[13] * rhs[15];
    
    // third row
    mat4[2] =  lhs[2]  *  rhs[0] +
               lhs[6]  *  rhs[1] +
               lhs[10] *  rhs[2] +
               lhs[14] *  rhs[3];
    
    mat4[6] =  lhs[2]  *  rhs[4] +
               lhs[6]  *  rhs[5] +
               lhs[10] *  rhs[6] +
               lhs[14] *  rhs[7];
     
    mat4[10] = lhs[2]  *  rhs[8] +
               lhs[6]  *  rhs[9] +
               lhs[10] * rhs[10] +
               lhs[14] * rhs[11];
    
    mat4[14] = lhs[2]  * rhs[12] +
               lhs[6]  * rhs[13] +
               lhs[10] * rhs[14] +
               lhs[14] * rhs[15];
    
    // fourth row
    mat4[3] =  lhs[3]  *  rhs[0] +
               lhs[7]  *  rhs[1] +
               lhs[11] *  rhs[2] +
               lhs[15] *  rhs[3];
    
    mat4[7] =  lhs[3]  *  rhs[4] +
               lhs[7]  *  rhs[5] +
               lhs[11] *  rhs[6] +
               lhs[15] *  rhs[7];
     
    mat4[11] = lhs[3]  *  rhs[8] +
               lhs[7]  *  rhs[9] +
               lhs[11] * rhs[10] +
               lhs[15] * rhs[11];
    
    mat4[15] = lhs[3]  * rhs[12] +
               lhs[7]  * rhs[13] +
               lhs[11] * rhs[14] +
               lhs[15] * rhs[15];

    return mat4;
}

f32 Mat4::determinant(const Mat4 & mat4)
{
    f32 a0 = mat4[0]  * mat4[5]  - mat4[4]  * mat4[1];
    f32 a1 = mat4[0]  * mat4[9]  - mat4[8]  * mat4[1];
    f32 a2 = mat4[0]  * mat4[13] - mat4[12] * mat4[1];
    f32 a3 = mat4[4]  * mat4[9]  - mat4[8]  * mat4[5];
    f32 a4 = mat4[4]  * mat4[13] - mat4[12] * mat4[5];
    f32 a5 = mat4[8]  * mat4[13] - mat4[12] * mat4[9];
    f32 b0 = mat4[2]  * mat4[7]  - mat4[6]  * mat4[3];
    f32 b1 = mat4[2]  * mat4[11] - mat4[10] * mat4[3];
    f32 b2 = mat4[2]  * mat4[15] - mat4[14] * mat4[3];
    f32 b3 = mat4[6]  * mat4[11] - mat4[10] * mat4[7];
    f32 b4 = mat4[6]  * mat4[15] - mat4[14] * mat4[7];
    f32 b5 = mat4[10] * mat4[15] - mat4[14] * mat4[11];

    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
    ASSERT(!is_fp_eq(det, 0.0f));
    return det;
}

Mat4 Mat4::inverse(const Mat4 & mat4)
{
    f32 a0 = mat4[0]  * mat4[5]  - mat4[4]  * mat4[1];
    f32 a1 = mat4[0]  * mat4[9]  - mat4[8]  * mat4[1];
    f32 a2 = mat4[0]  * mat4[13] - mat4[12] * mat4[1];
    f32 a3 = mat4[4]  * mat4[9]  - mat4[8]  * mat4[5];
    f32 a4 = mat4[4]  * mat4[13] - mat4[12] * mat4[5];
    f32 a5 = mat4[8]  * mat4[13] - mat4[12] * mat4[9];
    f32 b0 = mat4[2]  * mat4[7]  - mat4[6]  * mat4[3];
    f32 b1 = mat4[2]  * mat4[11] - mat4[10] * mat4[3];
    f32 b2 = mat4[2]  * mat4[15] - mat4[14] * mat4[3];
    f32 b3 = mat4[6]  * mat4[11] - mat4[10] * mat4[7];
    f32 b4 = mat4[6]  * mat4[15] - mat4[14] * mat4[7];
    f32 b5 = mat4[10] * mat4[15] - mat4[14] * mat4[11];

    f32 det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
    ASSERT(!is_fp_eq(det, 0.0f));
    f32 invDet = 1.0f / det;

    Mat4 inv;

    inv[0]  = invDet * ( mat4[5] * b5 - mat4[9]  * b4 + mat4[13] * b3);
    inv[1]  = invDet * (-mat4[1] * b5 + mat4[9]  * b2 - mat4[13] * b1);
    inv[2]  = invDet * ( mat4[1] * b4 - mat4[5]  * b2 + mat4[13] * b0);
    inv[3]  = invDet * (-mat4[1] * b3 + mat4[5]  * b1 - mat4[9]  * b0);

    inv[4]  = invDet * (-mat4[4] * b5 + mat4[8]  * b4 - mat4[12] * b3);
    inv[5]  = invDet * ( mat4[0] * b5 - mat4[8]  * b2 + mat4[12] * b1);
    inv[6]  = invDet * (-mat4[0] * b4 + mat4[4]  * b2 - mat4[12] * b0);
    inv[7]  = invDet * ( mat4[0] * b3 - mat4[4]  * b1 + mat4[8]  * b0);

    inv[8]  = invDet * ( mat4[7] * a5 - mat4[11] * a4 + mat4[15] * a3);
    inv[9]  = invDet * (-mat4[3] * a5 + mat4[11] * a2 - mat4[15] * a1);
    inv[10] = invDet * ( mat4[3] * a4 - mat4[7]  * a2 + mat4[15] * a0);
    inv[11] = invDet * (-mat4[3] * a3 + mat4[7]  * a1 - mat4[11] * a0);

    inv[12] = invDet * (-mat4[6] * a5 + mat4[10] * a4 - mat4[14] * a3);
    inv[13] = invDet * ( mat4[2] * a5 - mat4[10] * a2 + mat4[14] * a1);
    inv[14] = invDet * (-mat4[2] * a4 + mat4[6]  * a2 - mat4[14] * a0);
    inv[15] = invDet * ( mat4[2] * a3 - mat4[6]  * a1 + mat4[10] * a0);

    return inv;
}


Mat4 Mat4::build_translation(const Vec3 & pos)
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

    mat4[12] = pos.x;
    mat4[13] = pos.y;
    mat4[14] = pos.z;
    mat4[15] = 1.0f;

    return mat4;
}

Mat4 Mat4::build_rotation(const Vec3 & rot)
{
    f32 cosX = cos(rot.x);
    f32 sinX = sin(rot.x);
    f32 cosY = cos(rot.y);
    f32 sinY = sin(rot.y);
    f32 cosZ = cos(rot.z);
    f32 sinZ = sin(rot.z);
    f32 cosXsinY = cosX * sinY;
    f32 sinXsinY = sinX * sinY;

    Mat4 mat4;

    mat4[0]  = cosY * cosZ;
    mat4[1]  = -cosY * sinZ;
    mat4[2]  = sinY;
    mat4[3]  = 0.0f;

    mat4[4]  = sinXsinY * cosZ + cosX * sinZ;
    mat4[5]  = -sinXsinY * sinZ + cosX * cosZ;
    mat4[6]  = -sinX * cosY;
    mat4[7]  = 0.0f;
    
    mat4[8]  = -cosXsinY * cosZ + sinX * sinZ;
    mat4[9]  = cosXsinY * sinZ + sinX * cosZ;
    mat4[10] = cosX * cosY;
    mat4[11] = 0.0f;

    mat4[12] = 0.0f;
    mat4[13] = 0.0f;
    mat4[14] = 0.0f;
    mat4[15] = 1.0f;

    return mat4;
}

Mat4 Mat4::build_scale(f32 scale)
{
    Mat4 mat4;
    mat4[0]  = scale;
    mat4[1]  = 0.0f;
    mat4[2]  = 0.0f;
    mat4[3]  = 0.0f;

    mat4[4]  = 0.0f;
    mat4[5]  = scale;
    mat4[6]  = 0.0f;
    mat4[7]  = 0.0f;

    mat4[8]  = 0.0f;
    mat4[9]  = 0.0f;
    mat4[10] = scale;
    mat4[11] = 0.0f;

    mat4[12] = 0.0f;
    mat4[13] = 0.0f;
    mat4[14] = 0.0f;
    mat4[15] = 1.0f;
    return mat4;
}


Mat4 Mat4::frustum(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ)
{
    f32 deltaX = right - left;
    f32 deltaY = top - bottom;
    f32 deltaZ = farZ - nearZ;

    ASSERT((nearZ > 0.0f) && (farZ > 0.0f) &&
           (deltaX > 0.0f) && (deltaY > 0.0f) && (deltaZ > 0.0f));

    Mat4 frust;
    frust[0]  = 2.0f * nearZ / deltaX;
    frust[1]  = 0.0f;
    frust[2]  = 0.0f;
    frust[3]  = 0.0f;

    frust[4]  = 0.0f;
    frust[5]  = 2.0f * nearZ / deltaY;
    frust[6]  = 0.0f;
    frust[7]  = 0.0f;

    frust[8]  = (right + left) / deltaX;
    frust[9]  = (top + bottom) / deltaY;
    frust[10] = -(nearZ + farZ) / deltaZ;
    frust[11] = -1.0f;

    frust[12] = 0.0f;
    frust[13] = 0.0f;
    frust[14] = -2.0f * nearZ * farZ / deltaZ;
    frust[15] = 0.0f;

    return frust;
}


Mat4 Mat4::perspective(f32 fovy, f32 aspect, f32 nearZ, f32 farZ)
{
   f32 frustumW, frustumH;
   
   frustumH = tan(fovy / 360.0f * kPi) * nearZ;
   frustumW = frustumH * aspect;

   return frustum(-frustumW, frustumW, -frustumH, frustumH, nearZ, farZ);
}

Mat4 Mat4::orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ)
{
    f32 deltaX = right - left;
    f32 deltaY = top - bottom;
    f32 deltaZ = farZ - nearZ;

    ASSERT((deltaX != 0.0f) && (deltaY != 0.0f) && (deltaZ != 0.0f));

    Mat4 ortho;
    ortho[0]  = 2.0f / deltaX;
    ortho[1]  = 0.0f;
    ortho[2]  = 0.0f;
    ortho[3]  = 0.0f;

    ortho[4]  = 0.0f;
    ortho[5]  = 2.0f / deltaY;
    ortho[6]  = 0.0f;
    ortho[7]  = 0.0f;

    ortho[8]  = 0.0f;
    ortho[9]  = 0.0f;
    ortho[10] = -2.0f / deltaZ;
    ortho[11] = 0.0f;

    ortho[12] = -(right + left) / deltaX;
    ortho[13] = -(top + bottom) / deltaY;
    ortho[14] = -(nearZ + farZ) / deltaZ;
    ortho[15] = 1.0f;

    return ortho;
}

//------------------------------------------------------------------------------
// Mat4 methods (END)
//------------------------------------------------------------------------------


}
