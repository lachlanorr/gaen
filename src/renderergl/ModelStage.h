//------------------------------------------------------------------------------
// ModelStage.h - Grouping of models in a plane, including a cara layer
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

#ifndef GAEN_RENDERERGL_MODELSTAGE_H
#define GAEN_RENDERERGL_MODELSTAGE_H

#include "core/base_defines.h"

#include "renderergl/ModelGL.h"
#include "renderergl/FrameGL.h"

namespace gaen
{

class RendererMesh;
class ModelInstance;

class ModelStage
{
public:
    ModelStage(RendererMesh * pRenderer)
      : mpRenderer(pRenderer)
      , mIsShown(false)
    {}

    u32 modelsSize() { return mModels.size(); }

    void insertModel(ModelInstance * pModelInst);
    bool transformModel(u32 uid, const glm::mat4x3 & transform);
    bool destroyModel(u32 uid);

    bool isShown() { return mIsShown; }
    void show() { mIsShown = true; }
    void hide() { mIsShown = false; }
    
    RenderCollection<ModelGL>::Iter beginModels()       { return mModels.begin(); }
    RenderCollection<ModelGL>::Iter endModels()         { return mModels.end(); }
    void eraseModel(RenderCollection<ModelGL>::Iter it) { mModels.erase(it); }

private:
    RendererMesh * mpRenderer;
    bool mIsShown;

    RenderCollection<ModelGL> mModels;
    
}; // class ModelStage

typedef UniquePtr<ModelStage> ModelStageUP;

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_MODELSTAGE_H
