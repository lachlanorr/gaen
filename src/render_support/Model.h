//------------------------------------------------------------------------------
// Model.h - Model, a collection of gmdls/materials
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

#ifndef GAEN_RENDER_SUPPORT_MODEL_H
#define GAEN_RENDER_SUPPORT_MODEL_H

#include <glm/mat4x3.hpp>

#include "engine/task.h"

class Asset;
class Gmdl;

namespace gaen
{

typedef u32 model_id;

class Model
{
public:
    Model(task_id owner, const Asset* pGmdlAsset);
    Model(const Model& rhs);
    ~Model();

    model_id uid() const { return mUid; }
    task_id owner() const { return mOwner; }

    const Gmdl & gmdl() const;

private:
    // Delete these to make sure w construct through the asset->addref path
    Model(Model&&)                  = delete;
    Model & operator=(const Model&) = delete;
    Model & operator=(Model&&)      = delete;

    model_id mUid;

    task_id mOwner;
    const Asset * mpGmdlAsset;

    // pointers into mpGmdlAsset, no need to clean up
    const Gmdl * mpGmdl;
};

class ModelBody;
class ModelInstance
{
    friend class ModelMgr;
    friend class ModelBody;
    friend class ModelMotionState;
    friend class ModelPhysics;
public:
    ModelInstance(Model * pModel, u32 stageHash, const glm::mat4x3 & transform);

    const Model & model() { return *mpModel; }
    u32 stageHash() { return mStageHash; }

    model_id uid() const { return mpModel->uid(); }

    void destroyModel();

    static void send_model_insert(task_id source, task_id target, ModelInstance * pModelInst);
    static void send_model_transform(task_id source, task_id target, u32 uid, const glm::mat4x3 & transform);
    static void send_model_destroy(task_id source, task_id target, u32 uid);

    glm::vec3 position() { return mTransform[3]; }
    f32 zdepth() { return mTransform[3][2]; }

    glm::mat4x3 mTransform;

private:
    // Delete these to make sure we construct through the asset->addref path
    ModelInstance(const ModelInstance&)             = delete;
    ModelInstance(ModelInstance&&)                  = delete;
    ModelInstance & operator=(const ModelInstance&) = delete;
    ModelInstance & operator=(ModelInstance&&)      = delete;

    Model * mpModel;
    u32 mStageHash;

    bool mHasBody;
};

typedef UniquePtr<ModelInstance> ModelInstanceUP;

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_MODEL_H
