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

#include "engine/MessageQueue.h"
#include "engine/ModelMgr.h"
#include "engine/glm_ext.h"
#include "engine/Asset.h"
#include "engine/AssetMgr.h"

#include "engine/messages/InsertModelInstance.h"
#include "engine/messages/InsertLightDirectional.h"
#include "engine/messages/TransformUid.h"
#include "engine/messages/SpriteInstance.h"
#include "engine/messages/SpriteAnim.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/shaders/Shader.h"
#include "renderergl/ShaderRegistry.h"

#include "renderergl/RendererMesh.h"

namespace gaen
{


void RendererMesh::init(device_context deviceContext,
                        render_context renderContext,
                        u32 screenWidth,
                        u32 screenHeight)
{
    mDeviceContext = deviceContext;
    mRenderContext = renderContext;
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    mpModelMgr = GNEW(kMEM_Engine, ModelMgr<RendererMesh>, *this);

    mIsInit = true;
}

void RendererMesh::fin()
{
    ASSERT(mIsInit);
    mpModelMgr->fin();
    GDELETE(mpModelMgr);
}


void RendererMesh::initViewport()
{
    ASSERT(mIsInit);

    // Collect some details about our GPU capabilities
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &mMaxCombinedTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mMaxTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mMaxTextureSize);

    glEnable(GL_PROGRAM_POINT_SIZE);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);   // Enables Depth Testing
    //glDepthFunc(GL_LEQUAL);    // The Type Of Depth Testing To Do

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    // Make sure we don't divide by zero

    if (mScreenHeight==0)
    {
        mScreenHeight=1;
    }

    // reset viewport
    glViewport(0, 0, mScreenWidth, mScreenHeight);

    // setup projection with current width/height
    mProjection = glm::perspective(glm::radians(60.0f),
                                   mScreenWidth / static_cast<f32>(mScreenHeight),
                                   0.1f,
                                   100.0f);

    // setup gui projection, which is orthographic
    mGuiProjection = glm::ortho(static_cast<f32>(mScreenWidth) * -0.5f,
                                static_cast<f32>(mScreenWidth) * 0.5f,
                                static_cast<f32>(mScreenHeight) * -0.5f,
                                static_cast<f32>(mScreenHeight) * 0.5f,
                                0.0f,
                                100.0f);

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
    auto itTI = mLoadedTextures.find(pGimg);
    if (itTI == mLoadedTextures.end())
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
        itTI->second.refCount++;
        return itTI->second.glId;
    }
}

u32 RendererMesh::load_texture(u32 nameHash, const Gimg * pGimg, void * pContext)
{
    RendererMesh * pRenderer = (RendererMesh*)pContext;
    return pRenderer->loadTexture(texture_unit(nameHash), pGimg);
}

void RendererMesh::prepare_mesh_attributes(const Mesh & mesh)
{
    // position
    if (mesh.hasVertPosition())
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)(uintptr_t)mesh.vertPositionOffset());
        glEnableVertexAttribArray(0);

        // normal
        if (mesh.hasVertNormal())
        {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)(uintptr_t)mesh.vertNormalOffset());
            glEnableVertexAttribArray(1);

            // uv
            if (mesh.hasVertUv())
            {
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)(uintptr_t)mesh.vertUvOffset());
                glEnableVertexAttribArray(2);

                // uv tangents
                if (mesh.hasTan())
                {
                    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)(uintptr_t)mesh.vertTanOffset());
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
    GL_CLEAR_DEPTH(1.0f);


    ModelMgr<RendererMesh>::MeshIterator meshIt = mpModelMgr->begin();
    ModelMgr<RendererMesh>::MeshIterator meshItEnd = mpModelMgr->end();

    while (meshIt != meshItEnd)
    {
        const MaterialMeshInstance & matMeshInst = *meshIt;
        Mesh & mesh = matMeshInst.pMaterialMesh->mesh();
        Material & mat = matMeshInst.pMaterialMesh->material();

        setActiveShader(mat.shaderNameHash());

        if (mDirectionalLights.size() > 0)
        {
            const DirectionalLight & light = mDirectionalLights.front();
            mpActiveShader->setUniformVec3(HASH::uvLightDirection, light.direction);
            //mpActiveShader->setUniformVec4(HASH::uvLightColor, light.color);
        }

        static glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        glm::mat4 mvp = mProjection * view * glm::to_mat4x4(matMeshInst.pModelInstance->transform);
        glm::mat3 normalTrans = glm::to_mat3x3(view * glm::to_mat4x4(matMeshInst.pModelInstance->transform));

        mpActiveShader->setUniformMat4(HASH::umMVP, mvp);
        mpActiveShader->setUniformMat3(HASH::umNormal, normalTrans);

        // Set all material specific uniforms.
        // Add new types here as they become necessary, and the associated
        // callbacks, support in Material class, etc.
        mat.setShaderVec4Vars(set_shader_vec4_var, mpActiveShader);

        mat.setTextures(set_texture, this);
        
#if HAS(OPENGL3)
        glBindVertexArray(mesh.rendererReserved(kMSHR_VAO));
#else
        glBindBuffer(GL_ARRAY_BUFFER, mesh.rendererReserved(kMSHR_VertBuffer));

        prepare_mesh_attributes(mesh);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.rendererReserved(kMSHR_PrimBuffer));
#endif

        glDrawElements(GL_TRIANGLES, mesh.indexCount(), GL_UNSIGNED_SHORT, (void*)0);

        ++meshIt;
    }

    if (mSpriteMap.size() > 0)
    {
        setActiveShader(HASH::sprite);
        static glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        glm::mat4 mvp = mGuiProjection * view * glm::mat4x4(0.05); // to_mat4x4(matMeshInst.pModelInstance->transform);

        mpActiveShader->setUniformMat4(HASH::proj, mvp);
        
        for(const auto & spriteGlPair : mSpriteMap)
        {
            spriteGlPair.second->render();
        }
    }
}

template <typename T>
MessageResult RendererMesh::message(const T & msgAcc)
{
    const Message & msg = msgAcc.message();

    switch(msg.msgId)
    {
    case HASH::renderer_insert_model_instance:
    {
        messages::InsertModelInstanceR<T> msgr(msgAcc);
        mpModelMgr->insertModelInstance(msgAcc.message().source,
                                        msgr.uid(),
                                        msgr.model(),
                                        msgr.transform(),
                                        msgr.isAssetManaged());
        break;
    }
    case HASH::renderer_transform_model_instance:
    {
        messages::TransformUidR<T> msgr(msgAcc);
        ModelInstance * pModelInst = mpModelMgr->findModelInstance(msgr.uid());
        ASSERT(pModelInst);
        pModelInst->transform = msgr.transform();
        break;
    }
    case HASH::renderer_remove_model_instance:
    {
        mpModelMgr->removeModelInstance(msg.source, msg.payload.u);
        break;
    }
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

    // sprites
    case HASH::sprite_insert:
    {
        messages::SpriteInstanceR<T> msgR(msgAcc);

        SpriteInstance * pSpriteInst = msgR.spriteInstance();

        insertSprite(pSpriteInst);

        return MessageResult::Consumed;

        break;
    }
    case HASH::sprite_anim:
    {
        messages::SpriteAnimR<T> msgr(msgAcc);
        auto & spritePairIt = mSpriteMap.find(msgr.uid());

        if (spritePairIt != mSpriteMap.end())
        {
            spritePairIt->second->mpSpriteInstance->animate(msgr.animHash(), msgr.animFrameIdx());
        }
        else
        {
            ERR("sprite_anim in renderer for unkonwn sprite, uid: %u", msgr.uid());
        }
        break;
    }
    case HASH::sprite_transform:
    {
        break;
    }
    case HASH::sprite_destory:
    {
        break;
    }

    default:
        PANIC("Unknown renderer message: %d", msg.msgId);
    }

    return MessageResult::Consumed;
}

void RendererMesh::loadMaterialMesh(Model::MaterialMesh & matMesh)
{
    Mesh & mesh = matMesh.mesh();

#if HAS(OPENGL3)
    if (mesh.rendererReserved(kMSHR_VAO) == -1)
    {
        glGenVertexArrays(1, &mesh.rendererReserved(kMSHR_VAO));
    }

    glBindVertexArray(mesh.rendererReserved(kMSHR_VAO));
#endif
    
    if (mesh.rendererReserved(kMSHR_VertBuffer) == -1)
    {
        glGenBuffers(1, &mesh.rendererReserved(kMSHR_VertBuffer));
        glBindBuffer(GL_ARRAY_BUFFER, mesh.rendererReserved(kMSHR_VertBuffer));
        glBufferData(GL_ARRAY_BUFFER, mesh.vertsSize(), mesh.verts(), GL_STATIC_DRAW);

#if HAS(OPENGL3)
        prepare_mesh_attributes(mesh);
#endif
    }

    if (mesh.rendererReserved(kMSHR_PrimBuffer) == -1)
    {
        glGenBuffers(1, &mesh.rendererReserved(kMSHR_PrimBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.rendererReserved(kMSHR_PrimBuffer));
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.primsSize(), mesh.prims(), GL_STATIC_DRAW);
    }

#if HAS(OPENGL3)
    glBindVertexArray(0);
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Load textures
    matMesh.material().loadTextures(load_texture, this);
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

void RendererMesh::insertSprite(SpriteInstance * pSpriteInst)
{
    ASSERT(pSpriteInst);
    ASSERT(mSpriteMap.find(pSpriteInst->sprite().uid()) == mSpriteMap.end());

    SpriteGLUP spriteGL(GNEW(kMEM_Renderer, SpriteGL, pSpriteInst, this));

    spriteGL->loadGpu();
    
    mSpriteMap.emplace(pSpriteInst->sprite().uid(), std::move(spriteGL));
}

// Template decls so we can define message func here in the .cpp
template MessageResult RendererMesh::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult RendererMesh::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen

