//------------------------------------------------------------------------------
// SpriteMgr.cpp - Management of Sprite lifetimes, animations and collisions
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

#include "assets/Gspr.h"

#include "engine/Handle.h"
#include "engine/Asset.h"

#include "engine/SpriteAtlas.h"
#include "engine/SpriteMgr.h"

namespace gaen
{


template <typename T>
MessageResult SpriteMgr::message(const T & msgAcc)
{
    const Message & msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::sprite_create:
    {
        PANIC("TODO");
        return MessageResult::Consumed;
    }
    default:
        PANIC("Unknown SpriteMgr message: %d", msg.msgId);
    }
    return MessageResult::Consumed;
}

namespace system_api
{
HandleP sprite_create(AssetHandleP pAssetHandle, Entity & caller)
{
    ASSERT(pAssetHandle->typeHash() == HASH::asset);
    const Asset * pAsset = reinterpret_cast<const Asset*>(pAssetHandle->data());
    const Gspr * pGspr = Gspr::instance(pAsset->buffer(), pAsset->size());

    //SpriteAtlas * pSpriteAtlas = GNEW(kMEM_Texture, SpriteAtlas,

    /*
    // Front
    builder.addQuad(glm::vec3(xmin, ymax, zmax), glm::vec3(xmin, ymin, zmax), glm::vec3(xmax, ymin, zmax), glm::vec3(xmax, ymax, zmax));

    // Bottom
    builder.addQuad(glm::vec3(xmin, ymin, zmax), glm::vec3(xmin, ymin, zmin), glm::vec3(xmax, ymin, zmin), glm::vec3(xmax, ymin, zmax));

    // Back
    builder.addQuad(glm::vec3(xmin, ymin, zmin), glm::vec3(xmin, ymax, zmin), glm::vec3(xmax, ymax, zmin), glm::vec3(xmax, ymin, zmin));

    // Top
    builder.addQuad(glm::vec3(xmax, ymax, zmax), glm::vec3(xmax, ymax, zmin), glm::vec3(xmin, ymax, zmin), glm::vec3(xmin, ymax, zmax));

    // Left
    builder.addQuad(glm::vec3(xmin, ymax, zmax), glm::vec3(xmin, ymax, zmin), glm::vec3(xmin, ymin, zmin), glm::vec3(xmin, ymin, zmax));

    // Right
    builder.addQuad(glm::vec3(xmax, ymin, zmax), glm::vec3(xmax, ymin, zmin), glm::vec3(xmax, ymax, zmin), glm::vec3(xmax, ymax, zmax));

    Material * pMat = GNEW(kMEM_Texture, Material, HASH::faceted);
    pMat->registerVec4Var(HASH::uvColor, color.toVec4());

    Model * pModel = GNEW(kMEM_Model, Model, pMat, pMesh);
    return pModel;
    */

    return nullptr;
}
}

} // namespace gaen
