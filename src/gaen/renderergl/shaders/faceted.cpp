//------------------------------------------------------------------------------
// faceted.cpp - Auto-generated shader from faceted.shd
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

#include "gaen/core/mem.h"
#include "gaen/renderergl/shaders/faceted.h"

namespace gaen
{
namespace shaders
{

static const char * kShaderCode_shv =
    "#ifdef OPENGL3\n"
    "\n"
    "layout(location = 0) in vec4 position;\n"
    "layout(location = 1) in vec3 normal;\n"
    "layout(location = 2) in vec4 color;\n"
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
    "out vec4 oicolor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec3 normalTrans = normalize(rot * normal);\n"
    "\n"
    "    float intensity = max(dot(normalTrans, light0_incidence), 0.0);\n"
    "    vec3 surfacecolor = intensity * color.rgb * light0_color;\n"
    "\n"
    "    intensity = max(dot(normalTrans, light1_incidence), 0.0);\n"
    "    surfacecolor += intensity * color.rgb * light1_color;\n"
    "\n"
    "    vec3 ambient = light0_ambient * color.rgb * light0_color;\n"
    "    ambient += light1_ambient * color.rgb * light1_color;\n"
    "\n"
    "    vec3 linearcolor = vec3(ambient + surfacecolor);\n"
    "\n"
    "    const vec3 gamma = vec3(1.0/2.2);\n"
    "\n"
    "    oicolor = vec4(pow(linearcolor, gamma), color.a);\n"
    "\n"
    "    gl_Position = mvp * position;\n"
    "};\n"
    "\n"
    "\n"
    "#else //#ifdef OPENGL3\n"
    "#ifdef PLATFORM_IOS\n"
    "precision mediump float;\n"
    "#endif\n"
    "attribute vec4 vPosition;\n"
    "attribute vec3 normal;\n"
    "\n"
    "uniform mat4 uMvp;\n"
    "uniform mat3 rot;\n"
    "uniform vec4 ucolor;\n"
    "uniform vec3 lightincidence;\n"
    "uniform vec4 lightcolor;\n"
    "\n"
    "varying vec4 color\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec3 normalTrans = normalize(rot * normal);\n"
    "    float intensity = max(dot(normalTrans, lightincidence), 0.0);\n"
    "    intensity += min(intensity + 0.3, 1.0);\n"
    "    color = intensity * ucolor;\n"
    "    gl_Position = mvp * vPosition;\n"
    "};\n"
    "#endif //#else //#ifdef OPENGL3\n"
    ; // kShaderCode_shv (END)

static const char * kShaderCode_shf =
    "#ifdef OPENGL3\n"
    "\n"
    "in vec4 oicolor;\n"
    "out vec4 color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    color = oicolor;\n"
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
    //  These are computed at runtime in Shader::processProgram()

    // Attributes
    //  These are computed at runtime in Shader::processProgram()

    // Textures
    //  These are computed at runtime in Shader::processProgram()

    // Set base Shader members to our arrays and counts
    pShader->mCodeCount = kCodeCount;
    pShader->mpCodes = pShader->mCodes;
    pShader->mUniformCount = kUniformCount;
    pShader->mpUniforms = pShader->mUniforms;
    pShader->mAttributeCount = kAttributeCount;
    pShader->mpAttributes = pShader->mAttributes;
    pShader->mTextureCount = kTextureCount;
    pShader->mpTextures = nullptr;

    return pShader;
}

} // namespace shaders
} // namespace gaen
