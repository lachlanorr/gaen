//------------------------------------------------------------------------------
// prop.cpp - Auto-generated shader from prop.shd
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
#include "renderergl/shaders/prop.h"

namespace gaen
{
namespace shaders
{

static const char * kShaderCode_shv =
    "#ifdef OPENGL3\n"
    "\n"
    "layout(location = 0) in vec4 position;\n"
    "layout(location = 1) in vec3 normal;\n"
    "layout(location = 2) in vec2 vert_uv;\n"
    "\n"
    "uniform mat4 mvp;\n"
    "uniform mat3 rot;\n"
    "\n"
    "uniform vec3 light0_incidence;\n"
    "uniform vec3 light0_color;\n"
    "uniform float light0_ambient;\n"
    "\n"
    "uniform vec3 light1_incidence;\n"
    "uniform vec3 light1_color;\n"
    "uniform float light1_ambient;\n"
    "\n"
    "out vec2 frag_uv;\n"
    "out vec3 light0;\n"
    "out vec3 light1;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    frag_uv = vert_uv;\n"
    "\n"
    "    vec3 normal_corrected = normalize(rot * normal);\n"
    "    light0 = max(dot(normal_corrected, light0_incidence), 0.0) * light0_color + light0_ambient * light0_color;\n"
    "    light1 = max(dot(normal_corrected, light1_incidence), 0.0) * light1_color + light1_ambient * light1_color;\n"
    "\n"
    "    gl_Position = mvp * position;\n"
    "};\n"
    "\n"
    "\n"
    "#else //#ifdef OPENGL3\n"
    "#ifdef PLATFORM_IOS\n"
    "precision mediump float;\n"
    "#endif\n"
    "attribute vec4 position;\n"
    "attribute vec3 normal;\n"
    "\n"
    "uniform mat4 mvp;\n"
    "uniform mat3 rot;\n"
    "uniform vec4 ucolor;\n"
    "uniform vec3 light0_incidence;\n"
    "uniform vec4 light0_color;\n"
    "\n"
    "varying vec4 vcolor\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec3 normal_corrected = normalize(rot * normal);\n"
    "    float intensity = max(dot(normal_corrected, light0_incidence), 0.0);\n"
    "    intensity += min(intensity + 0.3, 1.0);\n"
    "    vcolor = intensity * ucolor;\n"
    "    gl_Position = mvp * position;\n"
    "};\n"
    "#endif //#else //#ifdef OPENGL3\n"
    ; // kShaderCode_shv (END)

static const char * kShaderCode_shf =
    "#ifdef OPENGL3\n"
    "\n"
    "in vec2 frag_uv;\n"
    "in vec3 light0;\n"
    "in vec3 light1;\n"
    "out vec4 color;\n"
    "\n"
    "uniform sampler2D diffuse;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    const vec3 gamma = vec3(1.0/2.2);\n"
    "\n"
    "    vec4 tex_color = texture(diffuse, frag_uv).rgba;\n"
    "    vec3 linear_color = light0 * tex_color.rgb;\n"
    "    color = vec4(linear_color, tex_color.a);\n"
    "};\n"
    "\n"
    "\n"
    "#else // #ifdef OPENGL3\n"
    "#if IS_PLATFORM_IOS\n"
    "precision mediump float;\n"
    "#endif\n"
    "\n"
    "varying vec4 color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);//color;\n"
    "};\n"
    "#endif // #else // #ifdef OPENGL3\n"
    ; // kShaderCode_shf (END)

Shader * prop::construct()
{
    prop * pShader = GNEW(kMEM_Renderer, prop);

    // Program Codes
    pShader->mCodes[0].stage = GL_VERTEX_SHADER;
    pShader->mCodes[0].filename = "prop.shv";
    pShader->mCodes[0].code = kShaderCode_shv;

    pShader->mCodes[1].stage = GL_FRAGMENT_SHADER;
    pShader->mCodes[1].filename = "prop.shf";
    pShader->mCodes[1].code = kShaderCode_shf;


    // Uniforms
    pShader->mUniforms[0].nameHash = 0x9d3cf50a; /* HASH::light0_ambient */
    pShader->mUniforms[0].index = 0;
    pShader->mUniforms[0].location = 0;
    pShader->mUniforms[0].type = GL_FLOAT;

    pShader->mUniforms[1].nameHash = 0x52ed2439; /* HASH::light0_color */
    pShader->mUniforms[1].index = 1;
    pShader->mUniforms[1].location = 1;
    pShader->mUniforms[1].type = GL_FLOAT_VEC3;

    pShader->mUniforms[2].nameHash = 0x4baa3a20; /* HASH::light0_incidence */
    pShader->mUniforms[2].index = 2;
    pShader->mUniforms[2].location = 2;
    pShader->mUniforms[2].type = GL_FLOAT_VEC3;

    pShader->mUniforms[3].nameHash = 0xdd88a90e; /* HASH::mvp */
    pShader->mUniforms[3].index = 3;
    pShader->mUniforms[3].location = 3;
    pShader->mUniforms[3].type = GL_FLOAT_MAT4;

    pShader->mUniforms[4].nameHash = 0x450e1f0e; /* HASH::rot */
    pShader->mUniforms[4].index = 4;
    pShader->mUniforms[4].location = 4;
    pShader->mUniforms[4].type = GL_FLOAT_MAT3;

    pShader->mUniforms[5].nameHash = 0x546e2a3d; /* HASH::diffuse */
    pShader->mUniforms[5].index = 5;
    pShader->mUniforms[5].location = 5;
    pShader->mUniforms[5].type = GL_SAMPLER_2D;


    // Attributes
    pShader->mAttributes[0].nameHash = 0xe68b9c52; /* HASH::normal */
    pShader->mAttributes[0].index = 0;
    pShader->mAttributes[0].location = 1;
    pShader->mAttributes[0].type = GL_FLOAT_VEC3;

    pShader->mAttributes[1].nameHash = 0x934f4e0a; /* HASH::position */
    pShader->mAttributes[1].index = 1;
    pShader->mAttributes[1].location = 0;
    pShader->mAttributes[1].type = GL_FLOAT_VEC4;

    pShader->mAttributes[2].nameHash = 0x0e669ca0; /* HASH::vert_uv */
    pShader->mAttributes[2].index = 2;
    pShader->mAttributes[2].location = 2;
    pShader->mAttributes[2].type = GL_FLOAT_VEC2;


    // Textures
    pShader->mTextures[0].nameHash = 0x546e2a3d; /* HASH::diffuse */
    pShader->mTextures[0].index = 0;
    pShader->mTextures[0].location = 5;
    pShader->mTextures[0].type = GL_SAMPLER_2D;


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
