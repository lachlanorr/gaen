//------------------------------------------------------------------------------
// compute_present.cpp - Auto-generated shader from compute_present.shd
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
#include "gaen/renderergl/shaders/compute_present.h"

namespace gaen
{
namespace shaders
{

static const char * kShaderCode_shv =
    "layout(location = 0) in vec4 vPosition;\n"
    "layout(location = 1) in vec2 vUV;\n"
    "\n"
    "out vec2 UV;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vPosition;\n"
    "    UV = vUV;\n"
    "};\n"
    ; // kShaderCode_shv (END)

static const char * kShaderCode_shf =
    "in vec2 UV;\n"
    "\n"
    "out vec3 color;\n"
    "\n"
    "layout(binding=0) uniform sampler2D imageSampler;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    color = texture(imageSampler, UV).rgb;\n"
    "};\n"
    ; // kShaderCode_shf (END)

Shader * compute_present::construct()
{
    compute_present * pShader = GNEW(kMEM_Renderer, compute_present);

    // Program Codes
    pShader->mCodes[0].stage = GL_VERTEX_SHADER;
    pShader->mCodes[0].filename = "compute_present.shv";
    pShader->mCodes[0].code = kShaderCode_shv;

    pShader->mCodes[1].stage = GL_FRAGMENT_SHADER;
    pShader->mCodes[1].filename = "compute_present.shf";
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
    pShader->mpTextures = pShader->mTextures;

    return pShader;
}

} // namespace shaders
} // namespace gaen
