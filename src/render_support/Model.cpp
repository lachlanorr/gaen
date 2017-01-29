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

#include "render_support/stdafx.h"

#include "assets/Gmdl.h"
#include "engine/AssetMgr.h"

#include "engine/messages/ModelInstance.h"

#include "render_support/Model.h"

namespace gaen
{

Model::Model(task_id owner, const Asset * pGmdlAsset)
  : RenderObject(owner)
  , mpGmdlAsset(pGmdlAsset)
{
    VALIDATE_ASSET(Gmdl, pGmdlAsset);
    AssetMgr::addref_asset(0, mpGmdlAsset);
    mpGmdl = Gmdl::instance(mpGmdlAsset->buffer(), mpGmdlAsset->size());
}

Model::Model(task_id owner, const Gmdl* pGmdl)
  : RenderObject(owner)
  , mpGmdlAsset(nullptr)
{
    mpGmdl = pGmdl;
}

Model::Model(const Model& rhs)
  : RenderObject(rhs.owner(), rhs.uid())
  , mpGmdlAsset(rhs.mpGmdlAsset)
  , mpGmdl(rhs.mpGmdl)
{}

Model::~Model()
{
    if (mpGmdlAsset)
    {
       // LORRTODO: Cleanup is causing crash on exit... need to redesign how we release assets
       //AssetMgr::release_asset(0, mpGmdlAsset);
    }
    else
    {
        ASSERT(mpGmdl);
        // Most models have a const Gmdl, only if there was no Asset
        // do we force deletion and must strip const to do so.
        GDELETE(const_cast<Gmdl*>(mpGmdl));
        mpGmdl = nullptr;
    }
}

const Gmdl & Model::gmdl() const
{
    return *mpGmdl;
}


// ModelInstance methods

ModelInstance::ModelInstance(Model * pModel, u32 stageHash, const mat43 & transform, bool isRenderable)
  : mTransform(transform)
  , mpModel(pModel)
  , mStageHash(stageHash)
  , mHasBody(false)
  , mIsRenderable(isRenderable)
{}

void ModelInstance::registerTransformListener(task_id taskId)
{
    MessageQueueWriter msgw(HASH::register_transform_listener, kMessageFlag_None, taskId, mpModel->owner(), to_cell(mpModel->uid()), 0);
}

void ModelInstance::destroyModel()
{
    if (mpModel)
    {
        GDELETE(mpModel);
        mpModel = nullptr;
    }
}

void ModelInstance::model_insert(task_id source, task_id target, ModelInstance * pModelInst)
{
    messages::ModelInstanceQW msgw(HASH::model_insert, kMessageFlag_None, source, target);
    msgw.setModelInstance(pModelInst);
}

void ModelInstance::model_remove(task_id source, task_id target, u32 uid)
{
    MessageQueueWriter msgw(HASH::model_remove, kMessageFlag_None, source, target, to_cell(uid), 0);
}


} // namespace gaen

