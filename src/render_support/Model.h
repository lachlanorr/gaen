//------------------------------------------------------------------------------
// Model.h - Model, a collection of gmdls/materials
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

#ifndef GAEN_RENDER_SUPPORT_MODEL_H
#define GAEN_RENDER_SUPPORT_MODEL_H

#include "math/mat43.h"
#include "math/matrices.h"
#include "engine/task.h"
#include "render_support/render_objects.h"

class Asset;
class Gmdl;
class Gaim;

namespace gaen
{

class Model : public RenderObject
{
public:
    Model(task_id owner, const Asset* pGmdlAsset, const Asset* pGaimAsset);
    Model(task_id owner, const Gmdl* pGmdl, bool doDelete);
    Model(const Model& rhs);
    ~Model();

    const Gmdl & gmdl() const;
    bool hasGaim() const;
    const Gaim & gaim() const;

private:
    // Delete these to make sure w construct through the asset->addref path
    Model(Model&&)                  = delete;
    Model & operator=(const Model&) = delete;
    Model & operator=(Model&&)      = delete;

    const Asset * mpGmdlAsset;
    const Asset * mpGaimAsset;

    // pointers into mpGmdlAsset, no need to clean up
    const Gmdl * mpGmdl;
    const Gaim * mpGaim;

    bool mDoDelete;
};

class ModelBody;
class ModelInstance
{
    friend class ModelMgr;
    friend class ModelBody;
    friend class ModelMotionState;
    friend class ModelPhysics;
public:
    ModelInstance(Model * pModel, u32 stageHash, RenderPass pass, u32 renderFlags, const mat43 & transform, bool isRenderable, bool isStatic);

    const Model & model() { return *mpModel; }
    u32 stageHash() { return mStageHash; }
    RenderPass pass() { return mPass; }

    ruid uid() const { return mpModel->uid(); }

    u32 renderFlags() const { return mRenderFlags; }

    void registerTransformWatcher(task_id taskId);

    void destroyModel();

    static void model_insert(task_id source, task_id target, ModelInstance * pModelInst);
    static void model_remove(task_id source, task_id target, u32 uid);

    vec3 position() { return gaen::position(mTransform); }
    f32 zdepth() { return gaen::position(mTransform).z; }

    mat43 mTransform;

private:
    // Delete these to make sure we construct through the asset->addref path
    ModelInstance(const ModelInstance&)             = delete;
    ModelInstance(ModelInstance&&)                  = delete;
    ModelInstance & operator=(const ModelInstance&) = delete;
    ModelInstance & operator=(ModelInstance&&)      = delete;

    Model * mpModel;
    u32 mStageHash;
    RenderPass mPass;
    u32 mRenderFlags;

    bool mHasBody;
    bool mIsRenderable;
    bool mIsStatic;
};

typedef UniquePtr<ModelInstance> ModelInstanceUP;

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_MODEL_H
