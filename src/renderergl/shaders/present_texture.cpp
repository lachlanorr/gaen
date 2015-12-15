//------------------------------------------------------------------------------
// present_texture.cpp - Auto-generated shader from present_texture.shd
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
#include "renderergl/shaders/present_texture.h"

namespace gaen
{
namespace shaders
{

static const char * kShaderCode_shv =
    "layout(location = 0) in vec4 vPosition;\n"
    "layout(location = 1) in vec2 vUV;\n"
    "layout(location = 2) in vec3 vFrustum;\n"
    "\n"
    "out vec2 UV;\n"
    "out vec3 frustum;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vPosition;\n"
    "    UV = vUV;\n"
    "    frustum = vFrustum;\n"
    "};\n"
    "\n"
    ; // kShaderCode_shv (END)

static const char * kShaderCode_shf =
    "in vec2 UV;\n"
    "in vec3 frustum;\n"
    "\n"
    "out vec3 color;\n"
    "\n"
    "layout(binding=0) uniform sampler2D imageSampler;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    color = texture(imageSampler, UV).rgb;\n"
    "    //color = vec3(UV, 0.0);\n"
    "    //color = frustum;\n"
    "    //color = abs(vec3(gl_FragCoord.x / 1024.0, gl_FragCoord.y / 1024.0, 0.0));\n"
    "};\n"
    ; // kShaderCode_shf (END)

Shader * present_texture::construct()
{
    present_texture * pShader = GNEW(kMEM_Renderer, present_texture);

    // Program Codes
    pShader->mCodes[0].stage = GL_VERTEX_SHADER;
    pShader->mCodes[0].filename = "present_texture.shv";
    pShader->mCodes[0].code = kShaderCode_shv;

    pShader->mCodes[1].stage = GL_FRAGMENT_SHADER;
    pShader->mCodes[1].filename = "present_texture.shf";
    pShader->mCodes[1].code = kShaderCode_shf;


    // Uniforms
    pShader->mUniforms[0].nameHash = 0xdc50cc66; /* HASH::imageSampler */
    pShader->mUniforms[0].index = 0;
    pShader->mUniforms[0].type = GL_SAMPLER_2D;


    // Attributes
    pShader->mAttributes[0].nameHash = 0xe61b84be; /* HASH::vPosition */
    pShader->mAttributes[0].index = 0;
    pShader->mAttributes[0].type = GL_FLOAT_VEC4;

    pShader->mAttributes[1].nameHash = 0x5e092066; /* HASH::vUV */
    pShader->mAttributes[1].index = 1;
    pShader->mAttributes[1].type = GL_FLOAT_VEC2;


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
