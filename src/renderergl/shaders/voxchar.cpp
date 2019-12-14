//------------------------------------------------------------------------------
// voxchar.cpp - Auto-generated shader from voxchar.shd
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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
#include "renderergl/shaders/voxchar.h"

namespace gaen
{
namespace shaders
{

static const char * kShaderCode_shv =
    "#ifdef OPENGL3\n"
    "\n"
    "layout(location = 0) in vec4 vPosition;\n"
    "layout(location = 1) in vec3 vNormal;\n"
    "layout(location = 2) in vec2 vVertUv;\n"
    "layout(location = 3) in uint vBoneId;\n"
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
    "uniform sampler2D animations;\n"
    "\n"
    "out vec2 vFragUv;\n"
    "out vec3 vLight0;\n"
    "out vec3 vLight1;\n"
    "out vec4 colorTemp;\n"
    "\n"
    "vec2 trans_coords(uint offset)\n"
    "{\n"
    "    uvec2 sz = textureSize(animations, 0);\n"
    "    return vec2((offset % sz.x) / float(sz.x), (offset / sz.x) / float(sz.y));\n"
    "}\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec3 normalTrans = normalize(uNormal * vNormal);\n"
    "    vLight0 = max(dot(normalTrans, uLight0_Incidence), 0.0) * uLight0_Color + uLight0_Ambient * uLight0_Color;\n"
    "    vLight1 = max(dot(normalTrans, uLight1_Incidence), 0.0) * uLight1_Color + uLight1_Ambient * uLight1_Color;\n"
    "\n"
    "    vFragUv = vVertUv;\n"
    "\n"
    "    vec4 anim0 = texture(animations, trans_coords(vBoneId * 3));\n"
    "    vec4 anim1 = texture(animations, trans_coords(vBoneId * 3 + 1));\n"
    "    vec4 anim2 = texture(animations, trans_coords(vBoneId * 3 + 2));\n"
    "\n"
    "    mat4 boneTrans = mat4(vec4(anim0.x, anim0.y, anim0.z, 0),\n"
    "                         vec4(anim0.w, anim1.x, anim1.y, 0),\n"
    "                         vec4(anim1.z, anim1.w, anim2.x, 0),\n"
    "                         vec4(anim2.y, anim2.z, anim2.w, 1));\n"
    "\n"
    "    gl_Position = uMvp * boneTrans * vPosition;\n"
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
    "in vec2 vFragUv;\n"
    "in vec3 vLight0;\n"
    "in vec3 vLight1;\n"
    "in vec4 colorTemp;\n"
    "out vec4 vColor;\n"
    "\n"
    "uniform sampler2D animations;\n"
    "uniform sampler2D diffuse;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    const vec3 gamma = vec3(1.0/2.2);\n"
    "\n"
    "    vec4 texColor = texture(diffuse, vFragUv).rgba;\n"
    "    vec3 linearColor = vLight0 * texColor.rgb;\n"
    "    vColor = vec4(linearColor, texColor.a);\n"
    "};\n"
    "\n"
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

Shader * voxchar::construct()
{
    voxchar * pShader = GNEW(kMEM_Renderer, voxchar);

    // Program Codes
    pShader->mCodes[0].stage = GL_VERTEX_SHADER;
    pShader->mCodes[0].filename = "voxchar.shv";
    pShader->mCodes[0].code = kShaderCode_shv;

    pShader->mCodes[1].stage = GL_FRAGMENT_SHADER;
    pShader->mCodes[1].filename = "voxchar.shf";
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

    pShader->mUniforms[3].nameHash = 0xce837dc9; /* HASH::uMvp */
    pShader->mUniforms[3].index = 3;
    pShader->mUniforms[3].location = 3;
    pShader->mUniforms[3].type = GL_FLOAT_MAT4;

    pShader->mUniforms[4].nameHash = 0x4b2c9b13; /* HASH::uNormal */
    pShader->mUniforms[4].index = 4;
    pShader->mUniforms[4].location = 4;
    pShader->mUniforms[4].type = GL_FLOAT_MAT3;

    pShader->mUniforms[5].nameHash = 0xcf15f26a; /* HASH::animations */
    pShader->mUniforms[5].index = 5;
    pShader->mUniforms[5].location = 5;
    pShader->mUniforms[5].type = GL_SAMPLER_2D;

    pShader->mUniforms[6].nameHash = 0x546e2a3d; /* HASH::diffuse */
    pShader->mUniforms[6].index = 6;
    pShader->mUniforms[6].location = 6;
    pShader->mUniforms[6].type = GL_SAMPLER_2D;


    // Attributes
    pShader->mAttributes[0].nameHash = 0x8b962316; /* HASH::vBoneId */
    pShader->mAttributes[0].index = 0;
    pShader->mAttributes[0].location = 3;
    pShader->mAttributes[0].type = GL_UNSIGNED_INT;

    pShader->mAttributes[1].nameHash = 0x0df141b6; /* HASH::vNormal */
    pShader->mAttributes[1].index = 1;
    pShader->mAttributes[1].location = 1;
    pShader->mAttributes[1].type = GL_FLOAT_VEC3;

    pShader->mAttributes[2].nameHash = 0xe61b84be; /* HASH::vPosition */
    pShader->mAttributes[2].index = 2;
    pShader->mAttributes[2].location = 0;
    pShader->mAttributes[2].type = GL_FLOAT_VEC4;

    pShader->mAttributes[3].nameHash = 0x3ee1a545; /* HASH::vVertUv */
    pShader->mAttributes[3].index = 3;
    pShader->mAttributes[3].location = 2;
    pShader->mAttributes[3].type = GL_FLOAT_VEC2;


    // Textures
    pShader->mTextures[0].nameHash = 0xcf15f26a; /* HASH::animations */
    pShader->mTextures[0].index = 0;
    pShader->mTextures[0].location = 5;
    pShader->mTextures[0].type = GL_SAMPLER_2D;

    pShader->mTextures[1].nameHash = 0x546e2a3d; /* HASH::diffuse */
    pShader->mTextures[1].index = 1;
    pShader->mTextures[1].location = 6;
    pShader->mTextures[1].type = GL_SAMPLER_2D;


    // Set base Shader members to our arrays and counts
    pShader->mCodeCount = kCodeCount;
    pShader->mpCodes = pShader->mCodes;
    pShader->mUniformCount = kUniformCount;
    pShader->mpUniforms = pShader->mUniforms;
    pShader->mAttributeCount = kAttributeCount;
    pShader->mpAttributes = pShader->mAttributes;
    pShader->mTextureCount = kTextureCount;
    pShader->mpTextures = pShader->mTextures;

    return pShader;
}

} // namespace shaders
} // namespace gaen
