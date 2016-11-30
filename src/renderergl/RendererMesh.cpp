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

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/base_defines.h"

#include "assets/Gimg.h"
#include "assets/Gmdl.h"

#include "engine/MessageQueue.h"
#include "engine/glm_ext.h"
#include "engine/Asset.h"
#include "engine/AssetMgr.h"

#include "engine/messages/InsertLightDirectional.h"
#include "engine/messages/TransformUid.h"
#include "engine/messages/ModelInstance.h"
#include "engine/messages/SpriteInstance.h"
#include "engine/messages/SpriteAnim.h"

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
    //glEnable(GL_DEPTH_TEST);   // Enables Depth Testing
    //glDepthFunc(GL_LEQUAL);    // The Type Of Depth Testing To Do

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    // Make sure we don't divide by zero

    if (mScreenHeight==0)
    {
        mScreenHeight=1;
    }

    // reset viewport
    glViewport(0, 0, mScreenWidth, mScreenHeight);

}

void RendererMesh::set_shader_vec4_var(u32 nameHash, const glm::vec4 & val, void * pContext)
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
    //GL_CLEAR_DEPTH(1.0f);



    setActiveShader(HASH::faceted);
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.second->isShown() && stagePair.second->itemsSize() > 0)
        {
            //static glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
            const glm::mat4 & projection = stagePair.second->camera().projection();

            for(auto it = stagePair.second->beginItems();
                it != stagePair.second->endItems();
                /* no increment so we can remove while iterating */)
            {
                ModelGL * pModelGL = *it;
                if (pModelGL->status() == kRIS_Active)
                {
                    glm::mat4 mvp = projection * to_mat4x4(pModelGL->transform());
                    mpActiveShader->setUniformMat4(HASH::umMVP, mvp);
                    pModelGL->render();
                    ++it;
                }
                else if (pModelGL->status() == kRIS_Destroyed)
                {
                    pModelGL->unloadGpu();

                    stagePair.second->eraseItem(it++);
                }
            }
        }
    }

    

    setActiveShader(HASH::sprite);
    for (auto & stagePair : mSpriteStages)
    {
        if (stagePair.second->isShown() && stagePair.second->itemsSize() > 0)
        {
            //static glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
            //glm::mat4 mvp = mGuiProjection; // * view  ;// * glm::mat4x4(0.05); // to_mat4x4(matGmdlInst.pModelInstance->transform);
            const glm::mat4 & projection = stagePair.second->camera().projection();

            for(auto it = stagePair.second->beginItems();
                it != stagePair.second->endItems();
                /* no increment so we can remove while iterating */)
            {
                SpriteGL * pSpriteGL = *it;
                if (pSpriteGL->status() == kRIS_Active)
                {
                    glm::mat4 mvp = projection * to_mat4x4(pSpriteGL->transform());
                    mpActiveShader->setUniformMat4(HASH::proj, mvp);
                    pSpriteGL->render();
                    ++it;
                }
                else if (pSpriteGL->status() == kRIS_Destroyed)
                {
                    pSpriteGL->unloadGpu();

                    stagePair.second->eraseItem(it++);
                }
            }
        }
    }
}

template <typename T>
MessageResult RendererMesh::message(const T & msgAcc)
{
    const Message & msg = msgAcc.message();

    switch(msg.msgId)
    {
    case HASH::renderer_insert_light_directional:
    {
        messages::InsertLightDirectionalR<T> msgr(msgAcc);
        glm::vec3 normDir = glm::normalize(msgr.direction());
        glm::vec3 relDir = -normDir; // flip direction of vector relative to objects
        mDirectionalLights.emplace_back(msgAcc.message().source,
                                        msgr.uid(),
                                        relDir,
                                        msgr.color());
        break;
    }
    case HASH::renderer_update_light_directional:
    {
        messages::InsertLightDirectionalR<T> msgr(msgAcc);
        mDirectionalLights.emplace_back(msgAcc.message().source,
                                        msgr.uid(),
                                        msgr.direction(),
                                        msgr.color());
        break;
    }

    // models
    case HASH::model_insert:
    {
        messages::ModelInstanceR<T> msgr(msgAcc);
        insertModel(msgr.modelInstance());
        break;
    }
    case HASH::model_transform:
    {
        messages::TransformUidR<T> msgr(msgAcc);
        transformModel(msgr.uid(), msgr.transform());
        break;
    }
    case HASH::model_destroy:
    {
        u32 uid = msg.payload.u;
        destroyModel(uid);
        break;
    }

    case HASH::model_show_stage:
    {
        u32 stageHash = msg.payload.u;
        showModelStage(stageHash);
        break;
    }
    case HASH::model_hide_stage:
    {
        u32 stageHash = msg.payload.u;
        hideModelStage(stageHash);
        break;
    }
    case HASH::model_destroy_stage:
    {
        u32 stageHash = msg.payload.u;
        destroyModelStage(stageHash);
        break;
    }

    // sprites
    case HASH::sprite_insert:
    {
        messages::SpriteInstanceR<T> msgr(msgAcc);
        insertSprite(msgr.spriteInstance());
        break;
    }
    case HASH::sprite_anim:
    {
        messages::SpriteAnimR<T> msgr(msgAcc);
        animateSprite(msgr.uid(), msgr.animHash(), msgr.animFrameIdx());
        break;
    }
    case HASH::sprite_transform:
    {
        messages::TransformUidR<T> msgr(msgAcc);
        transformSprite(msgr.uid(), msgr.transform());
        break;
    }
    case HASH::sprite_destroy:
    {
        u32 uid = msg.payload.u;
        destroySprite(uid);
        break;
    }

    case HASH::sprite_show_stage:
    {
        u32 stageHash = msg.payload.u;
        showSpriteStage(stageHash);
        break;
    }
    case HASH::sprite_hide_stage:
    {
        u32 stageHash = msg.payload.u;
        hideSpriteStage(stageHash);
        break;
    }
    case HASH::sprite_destroy_stage:
    {
        u32 stageHash = msg.payload.u;
        destroySpriteStage(stageHash);
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


void RendererMesh::insertModel(ModelInstance * pModelInst)
{
    auto it = mModelStages.find(pModelInst->stageHash());
    if (it == mModelStages.end())
    {
        auto empIt = mModelStages.emplace(pModelInst->stageHash(),
                                          GNEW(kMEM_Renderer, ModelStage, this));
        ASSERT(empIt.second == true);
        it = empIt.first;
    }
    it->second->insertItem(pModelInst);
}

void RendererMesh::transformModel(u32 uid, const glm::mat4x3 & transform)
{
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.second->transformItem(uid, transform))
            return;
    }
    ERR("transformModel in renderer for unkonwn model, uid: %u", uid);
}

void RendererMesh::destroyModel(u32 uid)
{
    for (auto & stagePair : mModelStages)
    {
        if (stagePair.second->destroyItem(uid))
            return;
    }
    ERR("destroyModel in renderer for unkonwn model, uid: %u", uid);
}

void RendererMesh::showModelStage(u32 stageHash)
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
}

void RendererMesh::hideModelStage(u32 stageHash)
{
    auto it = mModelStages.find(stageHash);
    if (it != mModelStages.end())
    {
        it->second->hide();
    }
}

void RendererMesh::destroyModelStage(u32 stageHash)
{
    auto it = mModelStages.find(stageHash);
    if (it != mModelStages.end())
    {
        mModelStages.erase(it);
    }
}



void RendererMesh::insertSprite(SpriteInstance * pSpriteInst)
{
    auto it = mSpriteStages.find(pSpriteInst->stageHash());
    if (it == mSpriteStages.end())
    {
        auto empIt = mSpriteStages.emplace(pSpriteInst->stageHash(),
                                           GNEW(kMEM_Renderer, SpriteStage, this));
        ASSERT(empIt.second == true);
        it = empIt.first;
    }
    it->second->insertItem(pSpriteInst);
}

void RendererMesh::animateSprite(u32 uid, u32 animHash, u32 animFrameIdx)
{
    for (auto & stagePair : mSpriteStages)
    {
        if (stagePair.second->animateItem(uid, animHash, animFrameIdx))
            return;
    }
    ERR("animateSprite in renderer for unkonwn sprite, uid: %u", uid);
}

void RendererMesh::transformSprite(u32 uid, const glm::mat4x3 & transform)
{
    for (auto & stagePair : mSpriteStages)
    {
        if (stagePair.second->transformItem(uid, transform))
            return;
    }
    ERR("transformSprite in renderer for unkonwn sprite, uid: %u", uid);
}

void RendererMesh::destroySprite(u32 uid)
{
    for (auto & stagePair : mSpriteStages)
    {
        if (stagePair.second->destroyItem(uid))
            return;
    }
    ERR("destroySprite in renderer for unkonwn sprite, uid: %u", uid);
}

void RendererMesh::showSpriteStage(u32 stageHash)
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
}

void RendererMesh::hideSpriteStage(u32 stageHash)
{
    auto it = mSpriteStages.find(stageHash);
    if (it != mSpriteStages.end())
    {
        it->second->hide();
    }
}

void RendererMesh::destroySpriteStage(u32 stageHash)
{
    auto it = mSpriteStages.find(stageHash);
    if (it != mSpriteStages.end())
    {
        mSpriteStages.erase(it);
    }
}


// Template decls so we can define message func here in the .cpp
template MessageResult RendererMesh::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult RendererMesh::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen

