//------------------------------------------------------------------------------
// Renderer2D.cpp - OpenGL 2D renderer
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

#include "engine/MessageQueue.h"
#include "engine/ModelMgr.h"
#include "engine/glm_ext.h"

#include "engine/messages/InsertModelInstance.h"
#include "engine/messages/InsertLightDirectional.h"
#include "engine/messages/TransformId.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/shaders/Shader.h"
#include "renderergl/ShaderRegistry.h"

#include "renderergl/Renderer2D.h"

namespace gaen
{


void Renderer2D::init(device_context deviceContext,
                      render_context renderContext,
                      u32 screenWidth,
                      u32 screenHeight)
{
    mDeviceContext = deviceContext;
    mRenderContext = renderContext;
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    mpModelMgr = GNEW(kMEM_Engine, ModelMgr<Renderer2D>, *this);

    mIsInit = true;
}

void Renderer2D::fin()
{
    ASSERT(mIsInit);
    mpModelMgr->fin();
    GDELETE(mpModelMgr);
}


void Renderer2D::initViewport()
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

    // Make sure we don't divide by zero
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

static void set_shader_vec4_var(u32 nameHash, const glm::vec4 & val, void * context)
{
    shaders::Shader * pShader = (shaders::Shader*)context;
    pShader->setUniformVec4(nameHash, val);
}

static void prepare_mesh_attributes(const Mesh & mesh)
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

void Renderer2D::render()
{
    ASSERT(mIsInit);

    glClear(GL_COLOR_BUFFER_BIT);
    GL_CLEAR_DEPTH(1.0f);


    ModelMgr<Renderer2D>::MeshIterator meshIt = mpModelMgr->begin();
    ModelMgr<Renderer2D>::MeshIterator meshItEnd = mpModelMgr->end();

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

        // LORRTEMP - Remove once we get materials setting the color properly
        mpActiveShader->setUniformVec4(HASH::uvColor, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

        static glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        glm::mat4 mvp = mProjection * view * glm::to_mat4x4(matMeshInst.pModelInstance->transform);
        glm::mat3 normalTrans = glm::to_mat3x3(view * glm::to_mat4x4(matMeshInst.pModelInstance->transform));

        mpActiveShader->setUniformMat4(HASH::umMVP, mvp);
        mpActiveShader->setUniformMat3(HASH::umNormal, normalTrans);

        // Set all material specific uniforms.
        // Add new types here as they become necessary, and the associated
        // callbacks, support in Material class, etc.
        mat.setShaderVec4Vars(set_shader_vec4_var, mpActiveShader);
        
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
}

template <typename T>
MessageResult Renderer2D::message(const T & msgAcc)
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
        messages::TransformIdR<T> msgr(msgAcc);
        ModelInstance * pModelInst = mpModelMgr->findModelInstance(msgr.id());
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
    default:
        PANIC("Unknown renderer message: %d", msg.msgId);
    }

    return MessageResult::Consumed;
}

void Renderer2D::loadMaterialMesh(Model::MaterialMesh & matMesh)
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
}

void Renderer2D::setActiveShader(u32 nameHash)
{
    if (!mpActiveShader || mpActiveShader->nameHash() != nameHash)
    {
        mpActiveShader = getShader(nameHash);
        mpActiveShader->use();
    }
}

shaders::Shader * Renderer2D::getShader(u32 nameHash)
{
    auto it = mShaders.find(nameHash);
    if (it != mShaders.end())
        return it->second;

    shaders::Shader* pShader = mShaderRegistry.constructShader(nameHash);
    mShaders[nameHash] = pShader;
    return pShader;
}


// Template decls so we can define message func here in the .cpp
template MessageResult Renderer2D::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult Renderer2D::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen

