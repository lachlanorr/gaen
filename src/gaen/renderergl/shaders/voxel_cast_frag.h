//------------------------------------------------------------------------------
// voxel_cast_frag.h - Auto-generated shader from voxel_cast_frag.shd
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
#ifndef GAEN_RENDERERGL_SHADERS_VOXEL_CAST_FRAG_H
#define GAEN_RENDERERGL_SHADERS_VOXEL_CAST_FRAG_H
#include "gaen/renderergl/shaders/Shader.h"
namespace gaen
{
namespace shaders
{

class voxel_cast_frag : Shader
{
public:
    static Shader * construct();

private:
    voxel_cast_frag() : Shader(0x427a5ca4 /* HASH::voxel_cast_frag */) {}

    static const u32 kCodeCount = 2;
    static const u32 kUniformCount = 5;
    static const u32 kAttributeCount = 2;
    static const u32 kTextureCount = 0;

    Shader::ShaderCode mCodes[kCodeCount];
    Shader::VariableInfo mUniforms[kUniformCount];
    Shader::VariableInfo mAttributes[kAttributeCount];
}; // class voxel_cast_frag

} // namespace shaders
} // namespace gaen
#endif // #ifdef GAEN_RENDERERGL_SHADERS_VOXEL_CAST_FRAG_H
