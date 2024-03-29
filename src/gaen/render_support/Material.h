//------------------------------------------------------------------------------
// Material.h - Materials used with models
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

#ifndef GAEN_RENDER_SUPPORT_MATERIAL_H
#define GAEN_RENDER_SUPPORT_MATERIAL_H

#include "gaen/math/vec4.h"

#include "gaen/assets/Color.h"
#include "gaen/core/base_defines.h"
#include "gaen/engine/UniqueObject.h"

#include "gaen/render_support/render_objects.h"

namespace gaen
{

class Asset;
class Gimg;

// Define these in the order of rendering.
// Higher MaterialLayers get rendered later.
// i.e., transparent stuff should be highest.
enum MaterialLayer
{
    kMAT_Colored = 0,

    // LORRTODO - Add additional shaders

    kMAT_END
};

typedef u32 material_id;

typedef void(*SetShaderVec4VarCB)(u32 nameHash, const vec4 & val, void * pContext);
typedef void(*SetTextureCB)(u32 nameHash, u32 gpuId, void * pContext);
typedef u32(*LoadTextureCB)(u32 nameHash, const Gimg * pGimg, void * pContext);
typedef void(*UnloadTextureCB)(u32 gpuId, const Gimg * pGimg, void * pContext);

class Material : UniqueObject
{
public:
    Material(u32 shaderNameHash);
    ~Material();

    MaterialLayer layer() const { return mLayer; }
    u32 shaderNameHash() { return mShaderNameHash; }

    void registerVec4Var(u32 nameHash, const vec4 & value);
    void registerTexture(u32 nameHash, const Asset * pAsset);

    void setShaderVec4Vars(SetShaderVec4VarCB setCB, void * context);
    void setTextures(SetTextureCB setCB, void * context);
    void loadTextures(LoadTextureCB loadCB, void * context);
    void unloadTextures(UnloadTextureCB loadCB, void * context);
private:
    MaterialLayer mLayer;
    u32 mShaderNameHash;

    static const u32 kMaxVec4Vars = 4;
    struct Vec4Var
    {
        u32 nameHash;
        vec4 value;
        Vec4Var() {}
        Vec4Var(u32 nameHash, const vec4 & value)
          : nameHash(nameHash)
          , value(value)
        {}
    };
    u32 mVec4VarCount;
    Vec4Var mVec4Vars[kMaxVec4Vars];


    struct TextureInfo
    {
        u32 nameHash;
        u32 rendererId;
        const Asset * pGimgAsset;
        TextureInfo() {}
        TextureInfo(u32 nameHash, u32 rendererId, const Asset * pGimgAsset)
          : nameHash(nameHash)
          , rendererId(rendererId)
          , pGimgAsset(pGimgAsset)
        {}
    };
    static const u32 kMaxTextures = 8;
    u32 mTextureCount;
    TextureInfo mTextures[kMaxTextures];
};


} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_MATERIAL_H
