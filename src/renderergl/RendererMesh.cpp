//------------------------------------------------------------------------------
// RendererMesh.cpp - OpenGL Mesh renderer
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

#include "core/base_defines.h"

#include "math/matrices.h"

#include "assets/Gimg.h"
#include "assets/Gmdl.h"

#include "engine/MessageQueue.h"
#include "engine/Asset.h"
#include "engine/AssetMgr.h"

#include "engine/messages/LightDistant.h"
#include "engine/messages/UidTransform.h"
#include "engine/messages/UidColor.h"
#include "engine/messages/UidVec3.h"
#include "engine/messages/UidScalar.h"
#include "engine/messages/ModelInstance.h"
#include "engine/messages/SpriteInstance.h"
#include "engine/messages/SpriteAnim.h"
#include "engine/messages/CameraPersp.h"
#include "engine/messages/CameraOrtho.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/shaders/Shader.h"
#include "renderergl/ShaderRegistry.h"

#include "renderergl/RendererMesh.h"

namespace gaen
{


void RendererMesh::init(void * pRenderDevice,
                        u32 screenWidth,
                        u32 screenHeight)
{
    mpRenderDevice = pRenderDevice;
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    mIsInit = true;
}

void RendererMesh::fin()
{
    ASSERT(mIsInit);
}


void RendererMesh::initViewport()
{
    ASSERT(mIsInit);

    // Collect some details about our GPU capabilities
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &mMaxCombinedTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mMaxTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mMaxTextureSize);

    glEnable(GL_PROGRAM_POINT_SIZE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);    // The Type Of Depth Testing To Do
    glEnable(GL_DEPTH_TEST);   // Enables Depth Testing

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    // Make sure we don't divide by zero

    if (mScreenHeight==0)
    {
        mScreenHeight=1;
    }

    // reset viewport
    glViewport(0, 0, mScreenWidth, mScreenHeight);

}

void RendererMesh::set_shader_vec4_var(u32 nameHash, const vec4 & val, void * pContext)
{
    shaders::Shader * pShader = (shaders::Shader*)pContext;
    pShader->setUniformVec4(nameHash, val);
}

u32 RendererMesh::texture_unit(u32 nameHash)
{
    switch (nameHash)
    {
    case HASH::diffuse:
        return 0;
    default:
        PANIC("Unknown texture nameHash: %u", nameHash);
        return 0;
    };
}

void RendererMesh::set_texture(u32 nameHash, u32 glId, void * pContext)
{
    glActiveTexture(GL_TEXTURE0 + texture_unit(nameHash));
    glBindTexture(GL_TEXTURE_2D, glId);
}

u32 RendererMesh::loadTexture(u32 textureUnit, const Gimg * pGimg)
{
    auto it = mLoadedTextures.find(pGimg);
    if (it == mLoadedTextures.end())
    {
        u32 glId = 0;
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glGenTextures(1, &glId);
        glBindTexture(GL_TEXTURE_2D, glId);

        ASSERT(pGimg->pixelFormat() == GL_RGBA8);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     pGimg->width(),
                     pGimg->height(),
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     pGimg->scanline(0));
    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        mLoadedTextures.emplace(std::piecewise_construct,
                                std::forward_as_tuple(pGimg),
                                std::forward_as_tuple(pGimg, glId, 1));
        return glId;
    }
    else
    {
        it->second.refCount++;
        return it->second.glId0;
    }
}

void RendererMesh::unloadTexture(const Gimg * pGimg)
{
    auto it = mLoadedTextures.find(pGimg);
    if (it != mLoadedTextures.end())
    {
        ASSERT(it->second.refCount > 0);
        it->second.refCount--;
        if (it->second.refCount == 0)
        {
            LOG_ERROR("TODO: Add code to unloadTexture");
            mLoadedTextures.erase(it);
        }
    }
    else
    {
        LOG_ERROR("unloadTexture on unkown Gimg");
    }
}



bool RendererMesh::loadVerts(u32 * pVertArrayId, u32 * pVertBufferId, const void * pVerts, u64 vertsSize)
{
    auto it = mLoadedVerts.find(pVerts);
    if (it == mLoadedVerts.end())
    {
#if HAS(OPENGL3)
        glGenVertexArrays(1, pVertArrayId);

        glBindVertexArray(*pVertArrayId);
#else
        *pVertArrayId = 0;
#endif

        glGenBuffers(1, pVertBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, *pVertBufferId);
        glBufferData(GL_ARRAY_BUFFER, vertsSize, pVerts, GL_STATIC_DRAW);

        mLoadedVerts.emplace(std::piecewise_construct,
                             std::forward_as_tuple(pVerts),
                             std::forward_as_tuple(pVerts, *pVertArrayId, *pVertBufferId, 1));
        return true;
    }
    else
    {
        it->second.refCount++;
        *pVertArrayId = it->second.glId0;
        *pVertBufferId = it->second.glId1;
        return false;
    }
}

void RendererMesh::unloadVerts(const void * pVerts)
{
    auto it = mLoadedVerts.find(pVerts);
    if (it != mLoadedVerts.end())
    {
        ASSERT(it->second.refCount > 0);
        it->second.refCount--;
        if (it->second.refCount == 0)
        {
            LOG_ERROR("TODO: Add code to unloadVerts");
            mLoadedVerts.erase(it);
        }
    }
    else
    {
        LOG_ERROR("unloadVerts on unkown Gimg");
    }
}

bool RendererMesh::loadPrims(u32 * pPrimBufferId, const void * pPrims, u64 primsSize)
{
    auto it = mLoadedPrims.find(pPrims);
    if (it == mLoadedPrims.end())
    {
        glGenBuffers(1, pPrimBufferId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pPrimBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, primsSize, pPrims, GL_STATIC_DRAW);

        mLoadedPrims.emplace(std::piecewise_construct,
                             std::forward_as_tuple(pPrims),
                             std::forward_as_tuple(pPrims, *pPrimBufferId, 1));
        return true;
    }
    else
    {
        it->second.refCount++;
        *pPrimBufferId = it->second.glId0;
        return false;
    }
}

void RendererMesh::unloadPrims(const void * pPrims)
{
    auto it = mLoadedPrims.find(pPrims);
    if (it != mLoadedPrims.end())
    {
        ASSERT(it->second.refCount > 0);
        it->second.refCount--;
        if (it->second.refCount == 0)
        {
            LOG_ERROR("TODO: Add code to unloadPrims");
            mLoadedPrims.erase(it);
        }
    }
    else
    {
        LOG_ERROR("unloadPrims on unkown Gimg");
    }
}



void RendererMesh::unbindBuffers()
{
#if HAS(OPENGL3)
    glBindVertexArray(0);
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

u32 RendererMesh::load_texture(u32 nameHash, const Gimg * pGimg, void * pContext)
{
    RendererMesh * pRenderer = (RendererMesh*)pContext;
    return pRenderer->loadTexture(texture_unit(nameHash), pGimg);
}

void RendererMesh::prepare_gmdl_attributes(const Gmdl & gmdl)
{
    // position
    if (gmdl.hasVertPosition())
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, gmdl.vertStride(), (void*)(uintptr_t)gmdl.vertPositionOffset());
        glEnableVertexAttribArray(0);

        // normal
        if (gmdl.hasVertNormal())
        {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, gmdl.vertStride(), (void*)(uintptr_t)gmdl.vertNormalOffset());
            glEnableVertexAttribArray(1);

            // uv
            if (gmdl.hasVertUv())
            {
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, gmdl.vertStride(), (void*)(uintptr_t)gmdl.vertUvOffset());
                glEnableVertexAttribArray(2);

                // uv tangents
                if (gmdl.hasVertTan())
                {
                    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, gmdl.vertStride(), (void*)(uintptr_t)gmdl.vertTanOffset());
                    glEnableVertexAttribArray(3);
                }
            }
        }
    }
}

void RendererMesh::render()
{
    ASSERT(mIsInit);

    glClear(GL_COLOR_BUFFER_BIT);

    for (auto & stagePair : mModelStages)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        stagePair.second->render();
    }

    for (auto & stagePair : mSpriteStages)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        stagePair.second->render();
    }
}

template <typename T>
MessageResult RendererMesh::message(const T & msgAcc)
{
    const Message & msg = msgAcc.message();

    switch(msg.msgId)
    {
    case HASH::light_distant_insert:
    {
        messages::LightDistantR<T> msgr(msgAcc);
        lightDistantInsert(msgr.uid(),
                           msgr.stageHash(),
                           msgr.color(),
                           msgr.ambient(),
                           msgr.direction());
        break;
    }
    case HASH::light_distant_direction:
    {
        messages::UidVec3R<T> msgr(msgAcc);
        lightDistantDirection(msgr.uid(),
                              msgr.vector());
        break;
    }
    case HASH::light_distant_color:
    {
        messages::UidColorR<T> msgr(msgAcc);
        lightDistantColor(msgr.uid(),
                          msgr.color());
        break;
    }
    case HASH::light_distant_ambient:
    {
        messages::UidScalarR<T> msgr(msgAcc);
        lightDistantAmbient(msgr.uid(),
                              msgr.scalar());
        break;
    }
    case HASH::light_distant_remove:
    {
        u32 uid = msg.payload.u;
        lightDistantRemove(uid);
        break;
    }

    // models
    case HASH::model_insert:
    {
        messages::ModelInstanceR<T> msgr(msgAcc);
        modelInsert(msgr.modelInstance());
        break;
    }
    case HASH::model_transform:
    {
        messages::UidTransformR<T> msgr(msgAcc);
        modelTransform(msgr.uid(), msgr.transform());
        break;
    }
    case HASH::model_remove:
    {
        u32 uid = msg.payload.u;
        modelRemove(uid);
        break;
    }

    case HASH::model_stage_show:
    {
        u32 stageHash = msg.payload.u;
        modelStageShow(stageHash);
        break;
    }
    case HASH::model_stage_hide:
    {
        u32 stageHash = msg.payload.u;
        modelStageHide(stageHash);
        break;
    }
    case HASH::model_stage_remove:
    {
        u32 stageHash = msg.payload.u;
        modelStageRemove(stageHash);
        break;
    }
    case HASH::model_stage_camera_insert_persp:
    {
        messages::CameraPerspR<T> msgr(msgAcc);
        modelStageCameraInsertPersp(msgr.uid(),
                                    msgr.stageHash(),
                                    msgr.fov(),
                                    msgr.nearClip(),
                                    msgr.farClip(),
                                    msgr.view());
        break;
    }
    case HASH::model_stage_camera_insert_ortho:
    {
        messages::CameraOrthoR<T> msgr(msgAcc);
        modelStageCameraInsertOrtho(msgr.uid(),
                                    msgr.stageHash(),
                                    msgr.scale(),
                                    msgr.nearClip(),
                                    msgr.farClip(),
                                    msgr.view());
        break;
    }
    case HASH::model_stage_camera_view:
    {
        messages::UidTransformR<T> msgr(msgAcc);
        modelStageCameraView(msgr.uid(),
                             msgr.transform());
        break;
    }
    case HASH::model_stage_camera_activate:
    {
        u32 uid = msg.payload.u;
        modelStageCameraActivate(uid);
        break;
    }
    case HASH::model_stage_camera_remove:
    {
        u32 uid = msg.payload.u;
        modelStageCameraRemove(uid);
        break;
    }

    // sprites
    case HASH::sprite_insert:
    {
        messages::SpriteInstanceR<T> msgr(msgAcc);
        spriteInsert(msgr.spriteInstance());
        break;
    }
    case HASH::sprite_anim:
    {
        messages::SpriteAnimR<T> msgr(msgAcc);
        spriteAnim(msgr.uid(), msgr.animHash(), msgr.animFrameIdx());
        break;
    }
    case HASH::sprite_transform:
    {
        messages::UidTransformR<T> msgr(msgAcc);
        spriteTransform(msgr.uid(), msgr.transform());
        break;
    }
    case HASH::sprite_remove:
    {
        u32 uid = msg.payload.u;
        spriteRemove(uid);
        break;
    }

    case HASH::sprite_stage_show:
    {
        u32 stageHash = msg.payload.u;
        spriteStageShow(stageHash);
        break;
    }
    case HASH::sprite_stage_hide:
    {
        u32 stageHash = msg.payload.u;
        spriteStageHide(stageHash);
        break;
    }
    case HASH::sprite_stage_remove:
    {
        u32 stageHash = msg.payload.u;
        spriteStageRemove(stageHash);
        break;
    }

    default:
        PANIC("Unknown renderer message: %d", msg.msgId);
    }

    return MessageResult::Consumed;
}

void RendererMesh::setActiveShader(u32 nameHash)
{
    if (!mpActiveShader || mpActiveShader->nameHash() != nameHash)
    {
        mpActiveShader = getShader(nameHash);
        mpActiveShader->use();
    }
}

shaders::Shader * RendererMesh::getShader(u32 nameHash)
{
    auto it = mShaders.find(nameHash);
    if (it != mShaders.end())
        return it->second;

    shaders::Shader* pShader = mShaderRegistry.constructShader(nameHash);
    mShaders[nameHash] = pShader;
    return pShader;
}


void RendererMesh::lightDistantInsert(u32 uid,
                                      u32 stageHash,
                                      Color color,
                                      f32 ambient,
                                      const vec3 & direction)
{
    ModelStage * pStage = modelStageFindOrCreate(stageHash);

    LightDistant light(kRendererTaskId,
                       uid,
                       stageHash,
                       color,
                       ambient,
                       direction);
                       
    pStage->lightDistantInsert(light);
}

void RendererMesh::lightDistantDirection(u32 uid, const vec3 & direction)
{
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.second->lightDistantDirection(uid, direction))
            return;
    }
    ERR("RendererMesh::lightDistantDirection unknown light, uid: %u", uid);
}

void RendererMesh::lightDistantColor(u32 uid, Color color)
{
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.second->lightDistantColor(uid, color))
            return;
    }
    ERR("RendererMesh::lightDistantColor unknown light, uid: %u", uid);
}

void RendererMesh::lightDistantAmbient(u32 uid, f32 ambient)
{
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.second->lightDistantAmbient(uid, ambient))
            return;
    }
    ERR("RendererMesh::lightDistantAmbient unknown light, uid: %u", uid);
}

void RendererMesh::lightDistantRemove(u32 uid)
{
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.second->lightDistantRemove(uid))
            return;
    }
    ERR("RendererMesh::lightDistantRemove unknown light, uid: %u", uid);
}



void RendererMesh::modelInsert(ModelInstance * pModelInst)
{
    ModelStage * pStage = modelStageFindOrCreate(pModelInst->stageHash());
    pStage->insertItem(pModelInst);
}

void RendererMesh::modelTransform(u32 uid, const mat43 & transform)
{
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.second->transformItem(uid, transform))
            return;
    }
    ERR("RendererMesh::modelTransform unknown model, uid: %u", uid);
}

void RendererMesh::modelRemove(u32 uid)
{
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.second->destroyItem(uid))
            return;
    }
    ERR("RendererMesh::modelRemove unknown model, uid: %u", uid);
}

ModelStage * RendererMesh::modelStageFindOrCreate(u32 stageHash)
{
    auto it = mModelStages.find(stageHash);
    if (it == mModelStages.end())
    {
        auto empIt = mModelStages.emplace(stageHash,
                                           GNEW(kMEM_Renderer, ModelStage, stageHash, this));
        ASSERT(empIt.second == true);
        it = empIt.first;
    }
    return it->second.get();
}

void RendererMesh::modelStageShow(u32 stageHash)
{
    // hide all other stages, show the one specified
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.first != stageHash)
            stagePair.second->hide();
    }

    auto it = mModelStages.find(stageHash);
    if (it != mModelStages.end())
    {
        it->second->show();
    }
    else
        ERR("modelStageShow unknown stage: %u", stageHash);
}

void RendererMesh::modelStageHide(u32 stageHash)
{
    auto it = mModelStages.find(stageHash);
    if (it != mModelStages.end())
    {
        it->second->hide();
    }
    else
        ERR("modelStageHide unknown stage: %u", stageHash);
}

void RendererMesh::modelStageRemove(u32 stageHash)
{
    auto it = mModelStages.find(stageHash);
    if (it != mModelStages.end())
    {
        mModelStages.erase(it);
    }
    else
        ERR("modelStageRemove unknown stage: %u", stageHash);
}

void RendererMesh::modelStageCameraInsertPersp(u32 uid,
                                               u32 stageHash,
                                               f32 fov,
                                               f32 nearClip,
                                               f32 farClip,
                                               const mat43 & view)
{
    Camera cam(kRendererTaskId,
               uid,
               stageHash,
               perspective(fov,
                           screenWidth() / (f32)screenHeight(),
                           nearClip,
                           farClip),
               view);

    ModelStage * pStage = modelStageFindOrCreate(stageHash);
    pStage->cameraInsert(uid, cam);
}

void RendererMesh::modelStageCameraInsertOrtho(u32 uid,
                                               u32 stageHash,
                                               f32 scale,
                                               f32 nearClip,
                                               f32 farClip,
                                               const mat43 & view)
{
    mat4 proj = ortho(screenWidth() * -0.5f,
                      screenWidth() * 0.5f,
                      screenHeight() * -0.5f,
                      screenHeight() * 0.5f,
                      nearClip,
                      farClip);

    proj = mat4::from_scale(scale) * proj;
    
    Camera cam(kRendererTaskId,
               uid,
               stageHash,
               proj,
               view);

    ModelStage * pStage = modelStageFindOrCreate(stageHash);
    pStage->cameraInsert(uid, cam);
}

void RendererMesh::modelStageCameraView(u32 uid, const mat43 & view)
{
    for (auto & stagePair : mModelStages)
    {
        Camera * pCam = stagePair.second->camera(uid);
        if (pCam)
        {
            pCam->setView(view);
            return;
        }
    }
    ERR("RendererMesh::modelStageCameraView unknown camera, uid: %u", uid);
}

void RendererMesh::modelStageCameraActivate(u32 uid)
{
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.second->cameraActivate(uid))
            return;
    }
    ERR("RendererMesh::modelStageCameraActivate unknown camera, uid: %u", uid);
}

void RendererMesh::modelStageCameraRemove(u32 uid)
{
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.second->cameraRemove(uid))
            return;
    }
    ERR("RendererMesh::modelStageCameraRemove unknown camera, uid: %u", uid);
}


void RendererMesh::spriteInsert(SpriteInstance * pSpriteInst)
{
    SpriteStage * pStage = spriteStageFindOrCreate(pSpriteInst->stageHash());
    pStage->insertItem(pSpriteInst);
}

void RendererMesh::spriteAnim(u32 uid, u32 animHash, u32 animFrameIdx)
{
    for (auto & stagePair : mSpriteStages)
    {
        if (stagePair.second->animateItem(uid, animHash, animFrameIdx))
            return;
    }
    ERR("RendererMesh::spriteAnim unknown sprite, uid: %u", uid);
}

void RendererMesh::spriteTransform(u32 uid, const mat43 & transform)
{
    for (auto & stagePair : mSpriteStages)
    {
        if (stagePair.second->transformItem(uid, transform))
            return;
    }
    ERR("RendererMesh::spriteTransform unknown sprite, uid: %u", uid);
}

void RendererMesh::spriteRemove(u32 uid)
{
    for (auto & stagePair : mSpriteStages)
    {
        if (stagePair.second->destroyItem(uid))
            return;
    }
    ERR("RendererMesh::spriteRemove unknown sprite, uid: %u", uid);
}

SpriteStage * RendererMesh::spriteStageFindOrCreate(u32 stageHash)
{
    auto it = mSpriteStages.find(stageHash);
    if (it == mSpriteStages.end())
    {
        auto empIt = mSpriteStages.emplace(stageHash,
                                           GNEW(kMEM_Renderer, SpriteStage, stageHash, this));
        ASSERT(empIt.second == true);
        it = empIt.first;
    }
    return it->second.get();
}

void RendererMesh::spriteStageShow(u32 stageHash)
{
    // hide all other stages, show the one specified
    for (auto & stagePair : mSpriteStages)
    {
        if (stagePair.first != stageHash)
            stagePair.second->hide();
    }

    auto it = mSpriteStages.find(stageHash);
    if (it != mSpriteStages.end())
    {
        it->second->show();
    }
    else
        ERR("RendererMesh::spriteStageShow unknown stage: %u", stageHash);
}

void RendererMesh::spriteStageHide(u32 stageHash)
{
    auto it = mSpriteStages.find(stageHash);
    if (it != mSpriteStages.end())
    {
        it->second->hide();
    }
    else
        ERR("RendererMesh::spriteStageHide unknown stage: %u", stageHash);
}

void RendererMesh::spriteStageRemove(u32 stageHash)
{
    auto it = mSpriteStages.find(stageHash);
    if (it != mSpriteStages.end())
    {
        mSpriteStages.erase(it);
    }
    else
        ERR("RendererMesh::spriteStageRemove unknown stage: %u", stageHash);
}


// Template decls so we can define message func here in the .cpp
template MessageResult RendererMesh::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult RendererMesh::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen

