//------------------------------------------------------------------------------
// Model.cpp - Model, a collection of gmdls/materials
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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
#include "assets/Gaim.h"
#include "engine/AssetMgr.h"

#include "engine/messages/RegisterWatcher.h"
#include "engine/messages/ModelInstance.h"

#include "render_support/Model.h"

namespace gaen
{

Model::Model(task_id owner, const Asset * pGmdlAsset, const Asset * pGaimAsset)
  : RenderObject(owner)
  , mpGmdlAsset(pGmdlAsset)
  , mpGaimAsset(pGaimAsset)
{
    VALIDATE_ASSET(Gmdl, pGmdlAsset);
    AssetMgr::addref_asset(0, mpGmdlAsset);
    mpGmdl = Gmdl::instance(mpGmdlAsset->buffer(), mpGmdlAsset->size());
    if (pGaimAsset)
    {
        VALIDATE_ASSET(Gaim, pGaimAsset);
        AssetMgr::addref_asset(0, mpGaimAsset);
        mpGaim = Gaim::instance(mpGaimAsset->buffer(), mpGaimAsset->size());
    }
    else
    {
        mpGaim = nullptr;
    }
}

Model::Model(task_id owner, const Gmdl* pGmdl)
  : RenderObject(owner)
  , mpGmdlAsset(nullptr)
  , mpGaimAsset(nullptr)
  , mpGmdl(pGmdl)
  , mpGaim(nullptr)
{}

Model::Model(const Model& rhs)
  : RenderObject(rhs.owner(), rhs.uid())
  , mpGmdlAsset(rhs.mpGmdlAsset)
  , mpGaimAsset(rhs.mpGaimAsset)
  , mpGmdl(rhs.mpGmdl)
  , mpGaim(rhs.mpGaim)
{}

Model::~Model()
{
    if (mpGmdlAsset)
    {
       AssetMgr::release_asset(0, mpGmdlAsset);
       if (mpGaimAsset)
           AssetMgr::release_asset(0, mpGaimAsset);
    }
    else
    {
        ASSERT(mpGmdl);
        // Most models have a const Gmdl, only if there was no Asset
        // do we force deletion and must strip const to do so.
        GDELETE(const_cast<Gmdl*>(mpGmdl));
        mpGmdl = nullptr;
        if (mpGaim)
        {
            GDELETE(const_cast<Gaim*>(mpGaim));
            mpGaim = nullptr;
        }
    }
}

const Gmdl & Model::gmdl() const
{
    return *mpGmdl;
}

bool Model::hasGaim() const
{
    return mpGaim != nullptr;
}

const Gaim & Model::gaim() const
{
    ASSERT(hasGaim());
    return *mpGaim;
}


// ModelInstance methods

ModelInstance::ModelInstance(Model * pModel, u32 stageHash, RenderPass pass, const mat43 & transform, bool isRenderable)
  : mTransform(transform)
  , mpModel(pModel)
  , mStageHash(stageHash)
  , mPass(pass)
  , mHasBody(false)
  , mIsRenderable(isRenderable)
{}

void ModelInstance::registerTransformWatcher(task_id taskId)
{
    messages::RegisterWatcherQW msgw(HASH::register_watcher, kMessageFlag_None, taskId, mpModel->owner(), HASH::model_transform);
    msgw.setProperty(HASH::transform);
    msgw.setUid(mpModel->uid());
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

