//------------------------------------------------------------------------------
// ShaderRegistry.cpp - Shader factory class
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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

#include "renderergl/ShaderRegistry.h"

namespace gaen
{

ShaderRegistry::ShaderRegistry()
{
    registerAllShaderConstructors();
}

shaders::Shader * ShaderRegistry::constructShader(u32 nameHash)
{
    auto it = mShaderConstructors.find(nameHash);

    if (it == mShaderConstructors.end())
    {
        PANIC("Unknown shader name hash: 0x%08x", nameHash);
        return nullptr;
    }

    return it->second();
}

void ShaderRegistry::registerShaderConstructor(u32 nameHash, ShaderConstructor constructor)
{
    auto it = mShaderConstructors.find(nameHash);

    if (it != mShaderConstructors.end())
    {
        PANIC("Shader name hash already registered: 0x%08x", nameHash);
        return;
    }
    
    mShaderConstructors[nameHash] = constructor;
}

} // namespace gaen
