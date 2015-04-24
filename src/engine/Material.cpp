//------------------------------------------------------------------------------
// Material.cpp - Materials used with models
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

#include "engine/stdafx.h"

#include <atomic>

#include "engine/Material.h"

namespace gaen
{

static std::atomic<material_id> sNextMaterialId(0);

Material::Material(u32 shaderNameHash)
  : mLayer(kMAT_Colored)
  , mShaderNameHash(shaderNameHash)
{
    mId = sNextMaterialId.fetch_add(1,std::memory_order_relaxed);

    mVec4VarCount = 0;
    memset(mVec4Vars, 0, sizeof(mVec4Vars));
}

void Material::registerVec4Var(u32 nameHash, const Vec4 & value)
{
    PANIC_IF(mVec4VarCount >= kMaxVec4Vars, "Too many Vec4 material vars");
    mVec4Vars[mVec4VarCount].nameHash = nameHash;
    mVec4Vars[mVec4VarCount].value = value;
    mVec4VarCount++;
}

void Material::setShaderVec4Vars(SetShaderVec4VarCB setCB, void * context)
{
    for (u32 i = 0; i < mVec4VarCount; ++i)
    {
        setCB(mVec4Vars[i].nameHash, mVec4Vars[i].value, context);
    }
}


} // namespace gaen
