//------------------------------------------------------------------------------
// Model.cpp - Model, a collection of gmdls/materials
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

#include "assets/Gmdl.h"
#include "engine/AssetMgr.h"

#include "engine/messages/ModelInstance.h"
#include "engine/messages/TransformUid.h"

#include "engine/Model.h"

namespace gaen
{

static std::atomic<model_id> sNextModelId(1);


Model::Model(task_id owner, const Asset * pGmdlAsset)
  : mOwner(owner)
  , mpGmdlAsset(pGmdlAsset)
{
    VALIDATE_ASSET(Gmdl, pGmdlAsset);
    AssetMgr::addref_asset(0, mpGmdlAsset);
    mpGmdl = Gmdl::instance(mpGmdlAsset->buffer(), mpGmdlAsset->size());

    mUid = sNextModelId.fetch_add(1, std::memory_order_relaxed);
}

Model::Model(const Model& rhs)
  : mOwner(rhs.mOwner)
  , mpGmdlAsset(rhs.mpGmdlAsset)
{
    mpGmdl = Gmdl::instance(mpGmdlAsset->buffer(), mpGmdlAsset->size());
    mUid = rhs.mUid;
}

Model::~Model()
{
    // LORRTODO: Cleanup is causing crash on exit... need to redesign how we release assets
    //AssetMgr::release_asset(0, mpGmdlAsset);
}

const Gmdl & Model::gmdl() const
{
    return *mpGmdl;
}


// ModelInstance methods

ModelInstance::ModelInstance(Model * pModel, u32 stageHash, const glm::mat4x3 & transform)
  : mpModel(pModel)
  , mStageHash(stageHash)
  , mHasBody(false)
  , mTransform(transform)
{}

void ModelInstance::destroyModel()
{
    if (mpModel)
    {
        GDELETE(mpModel);
        mpModel = nullptr;
    }
}

void ModelInstance::send_model_insert(task_id source, task_id target, ModelInstance * pModelInst)
{
    messages::ModelInstanceQW msgw(HASH::model_insert, kMessageFlag_None, source, target);
    msgw.setModelInstance(pModelInst);
}

void ModelInstance::send_model_transform(task_id source, task_id target, u32 uid, const glm::mat4x3 & transform)
{
    messages::TransformUidQW msgw(HASH::model_transform, kMessageFlag_None, source, target, uid);
    msgw.setTransform(transform);
}

void ModelInstance::send_model_destroy(task_id source, task_id target, u32 uid)
{
    MessageQueueWriter msgw(HASH::model_destroy, kMessageFlag_None, source, target, to_cell(uid), 0);
}


} // namespace gaen

