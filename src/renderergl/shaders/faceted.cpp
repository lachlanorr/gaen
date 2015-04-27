//------------------------------------------------------------------------------
// faceted.cpp - Auto-generated shader from faceted.shd
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#include "core/mem.h"
#include "renderergl/shaders/faceted.h"

namespace gaen
{
namespace shaders
{

static const char * kShaderCode_shv =
    "#ifdef OPENGL3\n"
    "\n"
    "in vec4 vPosition;\n"
    "in vec3 vNormal;\n"
    "\n"
    "uniform mat4 umMVP;\n"
    "uniform mat3 umNormal;\n"
    "uniform vec4 uvColor;\n"
    "uniform vec3 uvLightDirection;\n"
    "uniform vec4 uvLightColor;\n"
    "\n"
    "out vec4 vColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec3 normalTrans = normalize(umNormal * vNormal);\n"
    "    float intensity = max(dot(normalTrans, uvLightDirection), 0.0);\n"
    "    intensity += min(intensity + 0.3, 1.0);\n"
    "    vColor = intensity * uvColor;\n"
    "    //vColor = vec4((umNormal * vNormal), 1.0);\n"
    "    //vColor = vec4(dot(uvLightDirection, normalTrans));\n"
    "    //vColor = abs(dot(uvLightDirection, normalTrans)) * uvColor;\n"
    "    //vColor = vec4(abs(uvLightDirection), 1.0);\n"
    "    //vColor = 0.5 * uvColor;\n"
    "    //vColor = vec4(1.0, 1.0, 0.0, 0.6);\n"
    "    gl_Position = umMVP * vPosition;\n"
    "};\n"
    "\n"
    "\n"
    "#else //#ifdef OPENGL3\n"
    "#ifdef PLATFORM_IOS\n"
    "precision mediump float;\n"
    "#endif\n"
    "attribute vec4 vPosition;\n"
    "attribute vec3 vNormal;\n"
    "\n"
    "uniform mat4 umMVP;\n"
    "uniform mat3 umNormal;\n"
    "uniform vec4 uvColor;\n"
    "uniform vec3 uvLightDirection;\n"
    "uniform vec4 uvLightColor;\n"
    "\n"
    "varying vec4 vColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec3 normalTrans = normalize(umNormal * vNormal);\n"
    "    float intensity = max(dot(normalTrans, uvLightDirection), 0.0);\n"
    "    intensity += min(intensity + 0.3, 1.0);\n"
    "    vColor = intensity * uvColor;\n"
    "    gl_Position = umMVP * vPosition;\n"
    "};\n"
    "#endif //#else //#ifdef OPENGL3\n"
    ; // kShaderCode_shv (END)

static const char * kShaderCode_shf =
    "#ifdef OPENGL3\n"
    "\n"
    "in vec4 vColor;\n"
    "out vec4 color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    color = vColor;\n"
    "};\n"
    "\n"
    "#else // #ifdef OPENGL3\n"
    "#if IS_PLATFORM_IOS\n"
    "precision mediump float;\n"
    "#endif\n"
    "\n"
    "varying vec4 vColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = vColor;\n"
    "};\n"
    "#endif // #else // #ifdef OPENGL3\n"
    ; // kShaderCode_shf (END)

Shader * faceted::construct()
{
    faceted * pShader = GNEW(kMEM_Renderer, faceted);

    // Program Codes
    pShader->mCodes[0].stage = GL_VERTEX_SHADER;
    pShader->mCodes[0].filename = "faceted.shv";
    pShader->mCodes[0].code = kShaderCode_shv;

    pShader->mCodes[1].stage = GL_FRAGMENT_SHADER;
    pShader->mCodes[1].filename = "faceted.shf";
    pShader->mCodes[1].code = kShaderCode_shf;


    // Uniforms
    pShader->mUniforms[0].nameHash = 0xd89e6f38; /* HASH::umMVP */
    pShader->mUniforms[0].index = 0;
    pShader->mUniforms[0].type = GL_FLOAT_MAT4;

    pShader->mUniforms[1].nameHash = 0x8dd6b1d0; /* HASH::umNormal */
    pShader->mUniforms[1].index = 1;
    pShader->mUniforms[1].type = GL_FLOAT_MAT3;

    pShader->mUniforms[2].nameHash = 0xc3c5df05; /* HASH::uvColor */
    pShader->mUniforms[2].index = 2;
    pShader->mUniforms[2].type = GL_FLOAT_VEC4;

    pShader->mUniforms[3].nameHash = 0xa7a7a88b; /* HASH::uvLightDirection */
    pShader->mUniforms[3].index = 3;
    pShader->mUniforms[3].type = GL_FLOAT_VEC3;


    // Attributes
    pShader->mAttributes[0].nameHash = 0x0df141b6; /* HASH::vNormal */
    pShader->mAttributes[0].index = 0;
    pShader->mAttributes[0].type = GL_FLOAT_VEC3;

    pShader->mAttributes[1].nameHash = 0xe61b84be; /* HASH::vPosition */
    pShader->mAttributes[1].index = 1;
    pShader->mAttributes[1].type = GL_FLOAT_VEC4;


    // Set base Shader members to our arrays and counts
    pShader->mCodeCount = kCodeCount;
    pShader->mpCodes = pShader->mCodes;
    pShader->mUniformCount = kUniformCount;
    pShader->mpUniforms = pShader->mUniforms;
    pShader->mAttributeCount = kAttributeCount;
    pShader->mpAttributes = pShader->mAttributes;

    return pShader;
}

} // namespace shaders
} // namespace gaen
