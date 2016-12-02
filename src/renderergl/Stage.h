//------------------------------------------------------------------------------
// Stage.h - Base class for SpriteStage and CameraStage
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

#ifndef GAEN_RENDERERGL_STAGE_H
#define GAEN_RENDERERGL_STAGE_H

#include "render_support/render_objects.h"
#include "renderergl/RenderCollection.h"

namespace gaen
{

class RendererMesh;

template <class ItemGLT>
class Stage
{
public:
    Stage(RendererMesh * pRenderer, const Camera & defaultCamera)
      : mpRenderer(pRenderer)
      , mIsShown(false)
    {
        auto pair = mCameraMap.emplace(HASH::default, defaultCamera);
        mpCamera = &pair.first->second;
    }

    const Camera & camera() const { return *mpCamera; }

    void render()
    {
        if (isShown() && itemsSize() > 0)
        {
            //static glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
            const glm::mat4 & viewProj = camera().viewProjection();

            for(auto it = beginItems();
                it != endItems();
                /* no increment so we can remove while iterating */)
            {
                ItemGLT * pItemGL = *it;
                mpRenderer->setActiveShader(pItemGL->shaderHash());

                if (pItemGL->status() == kRIS_Active)
                {
                    glm::mat4 mvp = viewProj * to_mat4x4(pItemGL->transform());
                    mpRenderer->activeShader().setUniformMat4(HASH::uMvp, mvp);
                    pItemGL->render();
                    ++it;
                }
                else if (pItemGL->status() == kRIS_Destroyed)
                {
                    pItemGL->unloadGpu();

                    eraseItem(it++);
                }
            }
        }
    }

    void activateCamera(u32 cameraHash)
    {
        auto it = mCameraMap.find(cameraHash);

        if (it == mCameraMap.end())
        {
            ERR("Camera Hash not found %u", cameraHash);
            PANIC_IF(cameraHash == HASH::default, "Camera not found, and no default camera found either");
            activateCamera(HASH::default);
        }
        else
        {
            mpCamera = &pair.second;
        }
    }
    
    bool isShown() { return mIsShown; }
    void show() { mIsShown = true; }
    void hide() { mIsShown = false; }

    u32 itemsSize() { return mItems.size(); }

    void insertItem(typename ItemGLT::InstanceT * pInst)
    {
        ASSERT(pInst);
        ASSERT(mItems.find(pInst->uid()) == mItems.end());

        UniquePtr<ItemGLT> pItemGL(GNEW(kMEM_Renderer, ItemGLT, pInst, mpRenderer));
        pItemGL->loadGpu();
        mItems.insert(std::move(pItemGL));
    }
    
    bool transformItem(u32 uid, const glm::mat4x3 & transform)
    {
        auto it = mItems.find(uid);
        if (it != mItems.end())
        {
            f32 oldOrder = it->order();
            it->setTransform(transform);
            if (oldOrder != it->order())
            {
                mItems.reorder(uid);
            }
            return true;
        }
        return false;
    }

    bool destroyItem(u32 uid)
    {
        auto it = mItems.find(uid);
        if (it != mItems.end())
        {
            // Mark destroyed, it will get pulled from maps during next render pass.
            it->setStatus(kRIS_Destroyed);
            it->reportDestruction();

            return true;
        }
        return false;
    }

    typename RenderCollection<ItemGLT>::Iter beginItems()       { return mItems.begin(); }
    typename RenderCollection<ItemGLT>::Iter endItems()         { return mItems.end(); }
    void eraseItem(typename RenderCollection<ItemGLT>::Iter it) { mItems.erase(it); }

protected:
    RendererMesh * mpRenderer;
    bool mIsShown;

    const Camera * mpCamera;
    HashMap<kMEM_Renderer, u32, Camera> mCameraMap;

    RenderCollection<ItemGLT> mItems;

}; // class Stage

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_STAGE_H
