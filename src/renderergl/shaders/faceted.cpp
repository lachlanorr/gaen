//------------------------------------------------------------------------------
// faceted.cpp - Auto-generated shader from faceted.shd
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
    "in vec4 vColor;\n"
    "\n"
    "uniform mat4 uMvp;\n"
    "uniform mat3 uNormal;\n"
    "\n"
    "uniform vec3 uLight0_Incidence;\n"
    "uniform vec3 uLight0_Color;\n"
    "uniform float uLight0_Ambient;\n"
    "\n"
    "uniform vec3 uLight1_Incidence;\n"
    "uniform vec3 uLight1_Color;\n"
    "uniform float uLight1_Ambient;\n"
    "\n"
    "out vec4 oiColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec3 normalTrans = vNormal; // normalize(uNormal * vNormal);\n"
    "\n"
    "    float intensity = max(dot(normalTrans, uLight0_Incidence), 0.0);\n"
    "    vec3 surfaceColor = intensity * vColor.rgb * uLight0_Color;\n"
    "\n"
    "    intensity = max(dot(normalTrans, uLight1_Incidence), 0.0);\n"
    "    surfaceColor += intensity * vColor.rgb * uLight1_Color;\n"
    "\n"
    "    vec3 ambient = uLight0_Ambient * vColor.rgb * uLight0_Color;\n"
    "    ambient += uLight1_Ambient * vColor.rgb * uLight1_Color;\n"
    "\n"
    "    vec3 linearColor = vec3(ambient + surfaceColor);\n"
    "\n"
    "    const vec3 gamma = vec3(1.0/2.2);\n"
    "\n"
    "    oiColor = vec4(pow(linearColor, gamma), vColor.a);\n"
    "\n"
    "    gl_Position = uMvp * vPosition;\n"
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
    "uniform mat4 uMvp;\n"
    "uniform mat3 uNormal;\n"
    "uniform vec4 uColor;\n"
    "uniform vec3 uLightIncidence;\n"
    "uniform vec4 uLightColor;\n"
    "\n"
    "varying vec4 vColor\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec3 normalTrans = normalize(uNormal * vNormal);\n"
    "    float intensity = max(dot(normalTrans, uLightIncidence), 0.0);\n"
    "    intensity += min(intensity + 0.3, 1.0);\n"
    "    vColor = intensity * uColor;\n"
    "    gl_Position = mvp * vPosition;\n"
    "};\n"
    "#endif //#else //#ifdef OPENGL3\n"
    ; // kShaderCode_shv (END)

static const char * kShaderCode_shf =
    "#ifdef OPENGL3\n"
    "\n"
    "in vec4 oiColor;\n"
    "out vec4 color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    color = oiColor;\n"
    "    //color = vec4(gl_FragDepth, gl_FragDepth, gl_FragDepth, 1.0);\n"
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
    "    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);//vColor;\n"
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
    pShader->mUniforms[0].nameHash = 0xd7720547; /* HASH::uLight0_Ambient */
    pShader->mUniforms[0].index = 0;
    pShader->mUniforms[0].location = 0;
    pShader->mUniforms[0].type = GL_FLOAT;

    pShader->mUniforms[1].nameHash = 0x8508e11c; /* HASH::uLight0_Color */
    pShader->mUniforms[1].index = 1;
    pShader->mUniforms[1].location = 1;
    pShader->mUniforms[1].type = GL_FLOAT_VEC3;

    pShader->mUniforms[2].nameHash = 0xecd64955; /* HASH::uLight0_Incidence */
    pShader->mUniforms[2].index = 2;
    pShader->mUniforms[2].location = 2;
    pShader->mUniforms[2].type = GL_FLOAT_VEC3;

    pShader->mUniforms[3].nameHash = 0x1b451a20; /* HASH::uLight1_Ambient */
    pShader->mUniforms[3].index = 3;
    pShader->mUniforms[3].location = 3;
    pShader->mUniforms[3].type = GL_FLOAT;

    pShader->mUniforms[4].nameHash = 0xe2bf5e87; /* HASH::uLight1_Color */
    pShader->mUniforms[4].index = 4;
    pShader->mUniforms[4].location = 4;
    pShader->mUniforms[4].type = GL_FLOAT_VEC3;

    pShader->mUniforms[5].nameHash = 0x855516a6; /* HASH::uLight1_Incidence */
    pShader->mUniforms[5].index = 5;
    pShader->mUniforms[5].location = 5;
    pShader->mUniforms[5].type = GL_FLOAT_VEC3;

    pShader->mUniforms[6].nameHash = 0xce837dc9; /* HASH::uMvp */
    pShader->mUniforms[6].index = 6;
    pShader->mUniforms[6].location = 6;
    pShader->mUniforms[6].type = GL_FLOAT_MAT4;


    // Attributes
    pShader->mAttributes[0].nameHash = 0x4712466c; /* HASH::vColor */
    pShader->mAttributes[0].index = 0;
    pShader->mAttributes[0].location = 2;
    pShader->mAttributes[0].type = GL_FLOAT_VEC4;

    pShader->mAttributes[1].nameHash = 0x0df141b6; /* HASH::vNormal */
    pShader->mAttributes[1].index = 1;
    pShader->mAttributes[1].location = 1;
    pShader->mAttributes[1].type = GL_FLOAT_VEC3;

    pShader->mAttributes[2].nameHash = 0xe61b84be; /* HASH::vPosition */
    pShader->mAttributes[2].index = 2;
    pShader->mAttributes[2].location = 0;
    pShader->mAttributes[2].type = GL_FLOAT_VEC4;


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
