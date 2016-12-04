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
    const u32 kMaxLightDistantVecSize = 4;

    Stage(u32 stageHash, RendererMesh * pRenderer, const Camera & defaultCamera)
      : mStageHash(stageHash)
      , mpRenderer(pRenderer)
      , mIsShown(false)
    {
        auto pair = mCameraMap.emplace(defaultCamera.uid(), defaultCamera);
        mpDefaultCamera = &pair.first->second;
        mpCamera = mpDefaultCamera;

        mLightDistants.reserve(kMaxLightDistantVecSize);
    }

    u32 stageHash() { return mStageHash; }

    const Camera * cameraActive() const { return mpCamera; }

    void render()
    {
        if (isShown() && itemsSize() > 0)
        {
            //static glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
            const glm::mat4 & viewProj = cameraActive()->viewProjection();

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

                    if (mLightDistants.size() > 0)
                    {
                        mpRenderer->activeShader().setUniformVec3(HASH::uLight0_Incidence, mLightDistants[0].incidence());
                        mpRenderer->activeShader().setUniformVec3(HASH::uLight0_Color, mLightDistants[0].color());
                        mpRenderer->activeShader().setUniformFloat(HASH::uLight0_Ambient, mLightDistants[0].ambient());

                        if (mLightDistants.size() > 1)
                        {
                            mpRenderer->activeShader().setUniformVec3(HASH::uLight1_Incidence, mLightDistants[1].incidence());
                            mpRenderer->activeShader().setUniformVec3(HASH::uLight1_Color, mLightDistants[1].color());
                            mpRenderer->activeShader().setUniformFloat(HASH::uLight1_Ambient, mLightDistants[1].ambient());
                        }
                    }

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

    Camera * camera(u32 uid)
    {
        auto it = mCameraMap.find(uid);
        if (it != mCameraMap.end())
            return &it->second;
        return nullptr;
    }

    const Camera * camera(u32 uid) const
    {
        auto it = mCameraMap.find(uid);
        if (it != mCameraMap.end())
            return &it->second;
        return nullptr;
    }

    void cameraInsert(u32 uid, const Camera & camera)
    {
        EXPECT_MSG(mCameraMap.find(uid) == mCameraMap.end(), "cameraInsert: camera (%u) already exists in stage %u", uid, mStageHash);
        mCameraMap.emplace(uid, camera);
    }

    bool cameraRemove(u32 uid)
    {
        PANIC_IF(uid == mpDefaultCamera->uid(), "Attempt to remove default camera");
        size_t count = mCameraMap.erase(uid);
        if (count > 0)
        {
            mpCamera = mpDefaultCamera;
            return true;
        }
        return false;
    }

    bool cameraActivate(u32 uid)
    {
        auto it = mCameraMap.find(uid);
        if (it != mCameraMap.end())
        {
            mpCamera = &it->second;
            return true;
        }
        return false;
    }

    void lightDistantInsert(const LightDistant & light)
    {
        if (mLightDistants.size() < kMaxLightDistantVecSize)
        {
            mLightDistants.emplace_back(light);
            return;
        }
    }

    Vector<kMEM_Renderer, LightDistant>::iterator lightDistantFind(u32 uid)
    {
        for (auto it = mLightDistants.begin();
             it != mLightDistants.end();
             ++it)
        {
            if (it->uid() == uid)
                return it;
        }
        return mLightDistants.end();
    }

    bool lightDistantDirection(u32 uid, const glm::vec3 & direction)
    {
        auto it = lightDistantFind(uid);
        if (it != mLightDistants.end())
        {
            it->setDirection(direction);
            return true;
        }
        return false;
    }

    bool lightDistantColor(u32 uid, Color color)
    {
        auto it = lightDistantFind(uid);
        if (it != mLightDistants.end())
        {
            it->setColor(color);
            return true;
        }
        return false;
    }

    bool lightDistantAmbient(u32 uid, f32 ambient)
    {
        auto it = lightDistantFind(uid);
        if (it != mLightDistants.end())
        {
            it->setAmbient(ambient);
            return true;
        }
        return false;
    }

    bool lightDistantRemove(u32 uid)
    {
        auto it = lightDistantFind(uid);
        if (it != mLightDistants.end())
        {
            mLightDistants.erase(it);
            return true;
        }
        return false;
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
    u32 mStageHash;
    RendererMesh * mpRenderer;
    bool mIsShown;

    const Camera * mpCamera;
    const Camera * mpDefaultCamera;
    HashMap<kMEM_Renderer, u32, Camera> mCameraMap;

    Vector<kMEM_Renderer, LightDistant> mLightDistants;

    RenderCollection<ItemGLT> mItems;

}; // class Stage

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_STAGE_H
