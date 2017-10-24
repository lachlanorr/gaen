//------------------------------------------------------------------------------
// Material.cpp - Materials used with models
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

#include "render_support/stdafx.h"

#include "engine/task.h"
#include "engine/AssetMgr.h"
#include "render_support/Material.h"

namespace gaen
{

Material::Material(u32 shaderNameHash)
  : RenderObject(kRendererTaskId)
  , mLayer(kMAT_Colored)
  , mShaderNameHash(shaderNameHash)
{
    mVec4VarCount = 0;
    memset(mVec4Vars, 0, sizeof(mVec4Vars));

    mTextureCount = 0;
    memset(mTextures, 0, sizeof(mTextures));
}

Material::~Material()
{
    for (u32 i = 0; i < mTextureCount; ++i)
    {
        AssetMgr::release_asset(kRendererTaskId, mTextures[i].pGimgAsset);
    }
}

void Material::registerTexture(u32 nameHash, const Asset * pGimgAsset)
{
    PANIC_IF(mTextureCount >= kMaxTextures, "Too many textures for material");
    mTextures[mTextureCount++] = Material::TextureInfo(nameHash, 0, pGimgAsset);
    AssetMgr::addref_asset(kRendererTaskId, pGimgAsset);
}

void Material::registerVec4Var(u32 nameHash, const vec4 & value)
{
    PANIC_IF(mVec4VarCount >= kMaxVec4Vars, "Too many Vec4 material vars");
    mVec4Vars[mVec4VarCount++] = Material::Vec4Var(nameHash, value);
}

void Material::setShaderVec4Vars(SetShaderVec4VarCB setCB, void * context)
{
    for (u32 i = 0; i < mVec4VarCount; ++i)
    {
        setCB(mVec4Vars[i].nameHash, mVec4Vars[i].value, context);
    }
}

void Material::setTextures(SetTextureCB setCB, void * context)
{
    for (u32 i = 0; i < mTextureCount; ++i)
    {
        ASSERT(mTextures[i].rendererId > 0);
    }
}

void Material::loadTextures(LoadTextureCB loadCB, void * context)
{

}

void Material::unloadTextures(UnloadTextureCB loadCB, void * context)
{

}

} // namespace gaen
