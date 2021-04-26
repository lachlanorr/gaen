//------------------------------------------------------------------------------
// AssetTypes.h - Mapping of asset extension to various asset properties
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

#ifndef GAEN_ENGINE_ASSET_TYPES_H
#define GAEN_ENGINE_ASSET_TYPES_H

#include "core/mem.h"
#include "core/HashMap.h"
#include "math/mat43.h"
#include "engine/Handle.h"

#include "engine/AssetType.h"

namespace gaen
{

class AssetTypes
{
public:
    AssetTypes();

    void registerAssetType(const char * extension,
                           MemType memType,
                           AssetConstructor constructor);

    void registerProjectAssetTypes();

    const AssetType * assetTypeFromExt(const char * ext) const;

private:
    HashMap<kMEM_Engine, u32, UniquePtr<AssetType>> mExtToAssetTypeMap;
};

class Entity;
namespace system_api
{
i32 gaim_anim_index(const AssetHandleP pAssetHandleGaim, i32 nameHash, Entity * pCaller);
i32 gaim_frame_offset(const AssetHandleP pAssetHandleGaim, i32 animIndex, f32 delta, bool looped, Entity * pCaller);
i32 gaim_frame_count(const AssetHandleP pAssetHandleGaim, i32 animIndex, Entity * pCaller);
mat43 gaim_bone_transform(const AssetHandleP pAssetHandleGaim, i32 animIndex, i32 frameOffset, i32 boneIndex, Entity * pCaller);
i32 gmdl_bone_index(const AssetHandleP pAssetHandleGmdl, i32 nameHash, Entity * pCaller);
mat43 gmdl_bone_transform(const AssetHandleP pAssetHandleGmdl, i32 boneIndex, Entity * pCaller);
i32 gmdl_hardpoint_index(const AssetHandleP pAssetHandleGmdl, i32 nameHash, Entity * pCaller);
mat43 gmdl_hardpoint_transform(const AssetHandleP pAssetHandleGmdl, i32 hardpointIndex, Entity * pCaller);
} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_TYPES_H


