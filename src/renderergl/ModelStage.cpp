//------------------------------------------------------------------------------
// ModelStage.cpp - Grouping of models in a plane, including a cara layer
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

#include "engine/Model.h"

#include "renderergl/RendererMesh.h"
#include "renderergl/ModelGL.h"
#include "renderergl/ModelStage.h"

namespace gaen
{

void ModelStage::insertModel(ModelInstance * pModelInst)
{
    ASSERT(pModelInst);
    ASSERT(mModels.find(pModelInst->model().uid()) == mModels.end());

    ModelGLUP pModelGL(GNEW(kMEM_Renderer, ModelGL, pModelInst, mpRenderer));
    pModelGL->loadGpu();
    mModels.insert(std::move(pModelGL));
}

bool ModelStage::transformModel(u32 uid, const glm::mat4x3 & transform)
{
    auto it = mModels.find(uid);
    if (it != mModels.end())
    {
        it->setTransform(transform);
        return true;
    }
    return false;
}

bool ModelStage::destroyModel(u32 uid)
{
    auto it = mModels.find(uid);
    if (it != mModels.end())
    {
        // Mark destroyed, it will get pulled from maps during next render pass.
        it->setStatus(kRIS_Destroyed);

        ModelInstance::send_model_destroy(kRendererTaskId, kModelMgrTaskId, uid);

        return true;
    }
    return false;
}

} // namespace gaen
