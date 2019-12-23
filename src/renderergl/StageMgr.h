//------------------------------------------------------------------------------
// StageMgr.h - Manages multiple stages
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

#ifndef GAEN_RENDERERGL_STAGEMGR_H
#define GAEN_RENDERERGL_STAGEMGR_H

namespace gaen
{

template <class StageT>
class StageMgr
{
public:
    StageMgr(RendererMesh * pRenderer)
      : mpRenderer(pRenderer)
      , mDefaultCamera(Camera(kRendererTaskId,
                              HASH::main,
                              1.0f,
                              perspective(radians(60.0f),
                                          16.0f / 9.0f,
                                          0.1f,
                                          100000.0f),
                              mat43(1.0f)))
    {
        mStages.reserve(16);
    }


    void render()
    {
        for (auto & stage : mStages)
        {
            stage->render();
        }
    }




    StageT * find(u32 stageHash)
    {
        auto it = std::find_if(mStages.begin(), mStages.end(), [stageHash](const UniquePtr<StageT> & s) { return s->stageHash() == stageHash; });
        if (it != mStages.end())
        {
            return it->get();
        }
        else
        {
            return nullptr;
        }
    }

    StageT * findOrCreate(u32 stageHash)
    {
        StageT * pStage = find(stageHash);
        if (pStage)
        {
            return pStage;
        }
        else
        {
            // Create stage specific camera
            Camera cam(mDefaultCamera.owner(),
                       stageHash,
                       mDefaultCamera.scale(),
                       mDefaultCamera.projection(),
                       mDefaultCamera.view());

            mStages.emplace_back(GNEW(kMEM_Renderer, StageT, stageHash, mpRenderer, cam));
            return mStages.back().get();
        }
    }

    void show(u32 stageHash)
    {
        StageT * pStage = findOrCreate(stageHash);
        if (pStage)
        {
            pStage->show();
        }
        else
        {
            ERR("StageMgr::show unknown stage: %u", stageHash);
        }
    }

    void hide(u32 stageHash)
    {
        StageT * pStage = find(stageHash);
        if (pStage)
        {
            pStage->hide();
        }
        else
        {
            ERR("StageMgr::hide unknown stage: %u", stageHash);
        }
    }

    void hideAll()
    {
        for (auto & stage : mStages)
        {
            stage->hide();
        }
    }

    void remove(u32 stageHash)
    {
        auto it = std::find_if(mStages.begin(), mStages.end(), [stageHash](const UniquePtr<StageT> & s) { return s->stageHash() == stageHash; });
        if (it != mStages.end())
        {
            mStages.erase(it);
        }
        else
            ERR("StageMgr::remove unknown stage: %u", stageHash);
    }

    void removeAll()
    {
        mStages.clear();
    }



    void itemInsert(typename StageT::ItemType::InstanceType * pInst)
    {
        StageT * pStage = findOrCreate(pInst->stageHash());
        pStage->itemInsert(pInst);
        pInst->registerTransformWatcher(kRendererTaskId);
    }

    void itemRemove(u32 uid)
    {
        for (auto & stage : mStages)
        {
            if (stage->itemDestroy(uid))
                return;
        }
        ERR("StageMgr::itemRemove unknown item, uid: %u", uid);
    }

    void itemTransform(u32 uid, const mat43 & transform)
    {
        for (auto & stage : mStages)
        {
            if (stage->itemTransform(uid, transform))
                return;
        }
        ERR("StageMgr::itemTransform unknown item, uid: %u", uid);
    }

    void itemFrameOffset(u32 uid, u32 offset)
    {
        for (auto & stage : mStages)
        {
            if (stage->itemFrameOffset(uid, offset))
                return;
        }
        ERR("StageMgr::itemFrameOffset unknown item, uid: %u", uid);
    }

    void itemAnimate(u32 uid, u32 animHash, u32 animFrameIdx)
    {
        for (auto & stage : mStages)
        {
            if (stage->itemAnimate(uid, animHash, animFrameIdx))
                return;
        }
        ERR("StageMgr::itemAnimate unknown item, uid: %u", uid);
    }



    void lightInsert(u32 uid,
                     u32 stageHash,
                     Color color,
                     f32 ambient,
                     const vec3 & direction)
    {
        StageT * pStage = findOrCreate(stageHash);

        Light light(kRendererTaskId,
                    uid,
                    stageHash,
                    color,
                    ambient,
                    direction);

        pStage->lightInsert(light);
    }

    void lightDirection(u32 uid, const vec3 & direction)
    {
        for (auto & stage : mStages)
        {
            if (stage->lightDirection(uid, direction))
                return;
        }
        ERR("StageMgr::lightDirection unknown light, uid: %u", uid);
    }

    void lightColor(u32 uid, Color color)
    {
        for (auto & stage : mStages)
        {
            if (stage->lightColor(uid, color))
                return;
        }
        ERR("StageMgr::lightColor unknown light, uid: %u", uid);
    }

    void lightAmbient(u32 uid, f32 ambient)
    {
        for (auto & stage : mStages)
        {
            if (stage->lightAmbient(uid, ambient))
                return;
        }
        ERR("StageMgr::lightAmbient unknown light, uid: %u", uid);
    }

    void lightRemove(u32 uid)
    {
        for (auto & stage : mStages)
        {
            if (stage->lightRemove(uid))
                return;
        }
        ERR("StageMgr::lightRemove unknown light, uid: %u", uid);
    }


    void cameraSetDefault(const Camera & camera)
    {
        mDefaultCamera = camera;
    }

    void cameraInsertPersp(u32 uid,
                           u32 stageHash,
                           u32 screenWidth,
                           u32 screenHeight,
                           f32 fov,
                           f32 nearClip,
                           f32 farClip,
                           const mat43 & view)
    {
        Camera cam(kRendererTaskId,
                   uid,
                   stageHash,
                   1.0f,
                   perspective(fov,
                               screenWidth / (f32)screenHeight,
                               nearClip,
                               farClip),
                   view);

        StageT * pStage = findOrCreate(stageHash);
        pStage->cameraInsert(uid, cam);
    }

    void cameraInsertOrtho(u32 uid,
                           u32 stageHash,
                           u32 screenWidth,
                           u32 screenHeight,
                           f32 bounds,
                           f32 nearClip,
                           f32 farClip,
                           const mat43 & view)
    {
        mat4 proj;
        if (bounds == 0.0f) // pixel perfect
        {
            proj = ortho(screenWidth * -0.5f,
                         screenWidth * 0.5f,
                         screenHeight * -0.5f,
                         screenHeight * 0.5f,
                         nearClip,
                         farClip);
        }
        else
        {
            f32 aspectWidth = 1.0f;
            f32 aspectHeight = 1.0f;
            if (screenWidth >= screenHeight)
                aspectWidth = (f32)screenWidth / screenHeight;
            else
                aspectHeight = (f32)screenHeight / screenWidth;
            proj = ortho(bounds * -0.5f * aspectWidth,
                         bounds * 0.5f * aspectWidth,
                         bounds * -0.5f * aspectHeight,
                         bounds * 0.5f * aspectHeight,
                         nearClip,
                         farClip);
        }

        Camera cam(kRendererTaskId,
                   uid,
                   stageHash,
                   1.0f,
                   proj,
                   view);

        StageT * pStage = findOrCreate(stageHash);
        pStage->cameraInsert(uid, cam);
    }


    void cameraScale(u32 uid, f32 scale)
    {
        for (auto & stage : mStages)
        {
            Camera * pCam = stage->camera(uid);
            if (pCam)
            {
                pCam->setScale(scale);
                return;
            }
        }
        ERR("StageMgr::cameraScale unknown camera, uid: %u", uid);
    }

    void cameraView(u32 uid, const mat43 & view)
    {
        for (auto & stage : mStages)
        {
            Camera * pCam = stage->camera(uid);
            if (pCam)
            {
                pCam->setView(view);
                return;
            }
        }
        ERR("StageMgr::cameraView unknown camera, uid: %u", uid);
    }

    void cameraScaleAndView(u32 uid, f32 scale, const mat43 & view)
    {
        for (auto & stage : mStages)
        {
            Camera * pCam = stage->camera(uid);
            if (pCam)
            {
                pCam->setScaleAndView(scale, view);
                return;
            }
        }
        ERR("StageMgr::cameraView unknown camera, uid: %u", uid);
    }

    void cameraActivate(u32 uid)
    {
        for (auto & stage : mStages)
        {
            if (stage->cameraActivate(uid))
                return;
        }
        ERR("StageMgr::cameraActivate unknown camera, uid: %u", uid);
    }

    void cameraRemove(u32 uid)
    {
        for (auto & stage : mStages)
        {
            if (stage->cameraRemove(uid))
                return;
        }
        ERR("StageMgr::cameraRemove unknown camera, uid: %u", uid);
    }


private:
    RendererMesh * mpRenderer;
    Camera mDefaultCamera;

    Vector<kMEM_Renderer, UniquePtr<StageT>> mStages;
};

} // namespace gaen

#endif // GAEN_RENDERERGL_STAGEMGR_H
