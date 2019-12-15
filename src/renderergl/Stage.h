//------------------------------------------------------------------------------
// Stage.h - Base class for SpriteStage and CameraStage
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

#ifndef GAEN_RENDERERGL_STAGE_H
#define GAEN_RENDERERGL_STAGE_H

#include "render_support/render_objects.h"
#include "renderergl/RenderCollection.h"

namespace gaen
{

class RendererMesh;

template <class ItemT>
class Stage
{
public:
    typedef ItemT ItemType;
    const u32 kMaxLightVecSize = 4;

    Stage(u32 stageHash, RendererMesh * pRenderer, const Camera & defaultCamera)
      : mStageHash(stageHash)
      , mpRenderer(pRenderer)
      , mIsShown(false)
    {
        auto pair = mCameraMap.emplace(defaultCamera.uid(), defaultCamera);
        mpDefaultCamera = &pair.first->second;
        mpCamera = mpDefaultCamera;

        mLights.reserve(kMaxLightVecSize);
    }

    u32 stageHash() { return mStageHash; }

    const Camera * cameraActive() const { return mpCamera; }

    void render()
    {
        if (isShown())
        {
            const mat4 & viewProj = cameraActive()->viewProjection();

            for (u32 i = 0; i < kRP_COUNT; ++i)
            {
                for(auto it = mItems[i].begin();
                    it != mItems[i].end();
                    /* no increment so we can remove while iterating */)
                {
                    ItemT * pItem = *it;
                    mpRenderer->setActiveShader(pItem->shaderHash());
                    mpRenderer->activeShader().setTextureUniforms();

                    if (pItem->status() == kRIS_Active)
                    {
                        mat4 mvp = viewProj * mat4(pItem->transform());
                        mpRenderer->activeShader().setUniformMat4(HASH::mvp, mvp);

                        if (pItem->hasNormal())
                            mpRenderer->activeShader().setUniformMat3(HASH::rot, mat3(pItem->transform()));

                        if (mLights.size() > 0)
                        {
                            mpRenderer->activeShader().setUniformVec3(HASH::light0_incidence, mLights[0].incidence());
                            mpRenderer->activeShader().setUniformVec3(HASH::light0_color, mLights[0].color());
                            mpRenderer->activeShader().setUniformFloat(HASH::light0_ambient, mLights[0].ambient());

                            if (mLights.size() > 1)
                            {
                                mpRenderer->activeShader().setUniformVec3(HASH::light1_incidence, mLights[1].incidence());
                                mpRenderer->activeShader().setUniformVec3(HASH::light1_color, mLights[1].color());
                                mpRenderer->activeShader().setUniformFloat(HASH::light1_ambient, mLights[1].ambient());
                            }
                        }

                        if (mpRenderer->activeShader().hasUniform(HASH::frame_offset, GL_UNSIGNED_INT))
                            mpRenderer->activeShader().setUniformUint(HASH::frame_offset, pItem->frameOffset());

                        pItem->render();
                        ++it;
                    }
                    else if (pItem->status() == kRIS_Destroyed)
                    {
                        pItem->unloadGpu();

                        mItems[i].erase(it++);
                    }
                }
            }
        }
    }

    u32 camerasSize()
    {
        return mCameraMap.size();
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

    void lightInsert(const Light & light)
    {
        if (mLights.size() < kMaxLightVecSize)
        {
            mLights.emplace_back(light);
            return;
        }
    }

    Vector<kMEM_Renderer, Light>::iterator lightFind(u32 uid)
    {
        for (auto it = mLights.begin();
             it != mLights.end();
             ++it)
        {
            if (it->uid() == uid)
                return it;
        }
        return mLights.end();
    }

    bool lightDirection(u32 uid, const vec3 & direction)
    {
        auto it = lightFind(uid);
        if (it != mLights.end())
        {
            it->setDirection(direction);
            return true;
        }
        return false;
    }

    bool lightColor(u32 uid, Color color)
    {
        auto it = lightFind(uid);
        if (it != mLights.end())
        {
            it->setColor(color);
            return true;
        }
        return false;
    }

    bool lightAmbient(u32 uid, f32 ambient)
    {
        auto it = lightFind(uid);
        if (it != mLights.end())
        {
            it->setAmbient(ambient);
            return true;
        }
        return false;
    }

    bool lightRemove(u32 uid)
    {
        auto it = lightFind(uid);
        if (it != mLights.end())
        {
            mLights.erase(it);
            return true;
        }
        return false;
    }

    bool isShown() { return mIsShown; }
    void show() { mIsShown = true; }
    void hide() { mIsShown = false; }

    void itemInsert(typename ItemT::InstanceType * pInst)
    {
        ASSERT(pInst->pass() >= 0 && pInst->pass() < kRP_COUNT);
        ASSERT(pInst);
        ASSERT(mItems[pInst->pass()].find(pInst->uid()) == mItems[pInst->pass()].end());

        UniquePtr<ItemT> pItem(GNEW(kMEM_Renderer, ItemT, pInst, mpRenderer));
        pItem->loadGpu();
        mItems[pInst->pass()].insert(std::move(pItem));
    }

    bool itemTransform(u32 uid, const mat43 & transform)
    {
        for (u32 i = 0; i < kRP_COUNT; ++i)
        {
            auto it = mItems[i].find(uid);
            if (it != mItems[i].end())
            {
                f32 oldOrder = it->order();

                it->setTransform(transform);
                if (oldOrder != it->order())
                {
                    mItems[i].reorder(uid);
                }

                return true;
            }
        }
        return false;
    }

    bool itemFrameOffset(u32 uid, u32 offset)
    {
        for (u32 i = 0; i < kRP_COUNT; ++i)
        {
            auto it = mItems[i].find(uid);
            if (it != mItems[i].end())
            {
                it->setFrameOffset(offset);
                return true;
            }
        }
        return false;
    }

    bool itemAnimate(u32 uid, u32 animHash, u32 animFrameIdx)
    {
        for (u32 i = 0; i < kRP_COUNT; ++i)
        {
            auto it = mItems[i].find(uid);
            if (it != mItems[i].end())
            {
                it->animate(animHash, animFrameIdx);
                return true;
            }
        }
        return false;
    }

    bool itemDestroy(u32 uid)
    {
        for (u32 i = 0; i < kRP_COUNT; ++i)
        {
            auto it = mItems[i].find(uid);
            if (it != mItems[i].end())
            {
                // Mark destroyed, it will get pulled from maps during next render pass.
                it->setStatus(kRIS_Destroyed);
                it->reportDestruction();

                return true;
            }
        }
        return false;
    }

private:
    u32 mStageHash;
    RendererMesh * mpRenderer;
    bool mIsShown;

    const Camera * mpCamera;
    const Camera * mpDefaultCamera;
    HashMap<kMEM_Renderer, u32, Camera> mCameraMap;

    Vector<kMEM_Renderer, Light> mLights;

    RenderCollection<ItemT> mItems[kRP_COUNT];
}; // class Stage

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_STAGE_H
