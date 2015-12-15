//------------------------------------------------------------------------------
// RendererGL.cpp - OpenGL Renderer
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#include "engine/MessageQueue.h"
#include "engine/ModelMgr.h"

#include "engine/messages/InsertModelInstance.h"
#include "engine/messages/InsertLightDirectional.h"
#include "engine/messages/TransformId.h"
#include "engine/messages/MoveCamera.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/shaders/Shader.h"
#include "renderergl/ShaderRegistry.h"

#include "renderergl/RendererGL.h"

#define RENDERTYPE_MESH 0
#define RENDERTYPE_CPUVOXEL 1
#define RENDERTYPE_GPUVOXEL 2

#define RENDERTYPE RENDERTYPE_GPUVOXEL

namespace gaen
{

static f32 kPresentSurface[] = { -1.0f, -1.0f, // pos 0
                                  0.0f,  0.0f, // uv  0

                                  1.0f, -1.0f, // pos 1
                                  1.0f,  0.0f, // uv  1

                                 -1.0f,  1.0f, // pos 2
                                  0.0f,  1.0f, // uv  2

                                  1.0f,  1.0f, // pos 3
                                  1.0f,  1.0f  // uv  3
};

void RendererGL::init(device_context deviceContext,
                      render_context renderContext,
                      u32 screenWidth,
                      u32 screenHeight)
{
    mDeviceContext = deviceContext;
    mRenderContext = renderContext;
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    mpModelMgr = GNEW(kMEM_Engine, ModelMgr<RendererGL>, *this);

    mShaderSim.init(kImgSize, &mRaycastCamera);

    mIsInit = true;
}

void RendererGL::fin()
{
    ASSERT(mIsInit);
    mpModelMgr->fin();
    GDELETE(mpModelMgr);
}


static Mat4 sMVPMat(1.0f);

void RendererGL::initViewport()
{
    ASSERT(mIsInit);

    // Collect some details about our GPU capabilities
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &mMaxCombinedTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mMaxTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mMaxTextureSize);

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

    mRaycastCamera.init(mScreenWidth, mScreenHeight, 60.0f, 0.1f, 1000.0f);

    // setup projection with current width/height
    mProjection = Mat4::perspective(60.0f,
                                    mScreenWidth / static_cast<f32>(mScreenHeight),
                                    0.1f,
                                    100.0f);

    // setup gui projection, which is orthographic
    mGuiProjection = Mat4::orthographic(static_cast<float>(mScreenWidth) * -0.5f,
                                        static_cast<float>(mScreenWidth) * 0.5f,
                                        static_cast<float>(mScreenHeight) * -0.5f,
                                        static_cast<float>(mScreenHeight) * 0.5f,
                                        0.0f,
                                        100.0f);

    //sMVPMat = Mat4::translation(Vec3(0.0f, 0.0f, 0.0f));
    //sMVPMat = Mat4::lookat(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f));
    sMVPMat = Mat4::rotation(Vec3(kPi / 4.0f, kPi / 4.0f, 0.0f));

    // Prepare GPU renderer presentation vars
    glGenVertexArrays(1, &mPresentVAO);
    glBindVertexArray(mPresentVAO);

    glGenBuffers(1, &mPresentVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mPresentVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kPresentSurface), kPresentSurface, GL_STATIC_DRAW);

    mpPresentShader = getShader(HASH::compute_present);
    mpPresentShader->use();

    // vertex position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void*)0);
    glEnableVertexAttribArray(0);

    // vertex UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*)8);
    glEnableVertexAttribArray(1);


    // prep image
    glActiveTexture(GL_TEXTURE0 + 0);
    glGenTextures(1, &mPresentImage);
    glBindTexture(GL_TEXTURE_2D, mPresentImage);
    
#if RENDERTYPE == RENDERTYPE_CPUVOXEL
    //glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG32F, kImgSize, kImgSize);
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, kImgSize, kImgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#elif RENDERTYPE == RENDERTYPE_GPUVOXEL
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG32F, kImgSize, kImgSize);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#endif


    // prep voxel cast shader
    mpVoxelCast = getShader(HASH::voxel_cast);
    glBindImageTexture(0, mPresentImage, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);

}

static void set_shader_vec4_var(u32 nameHash, const Vec4 & val, void * context)
{
    shaders::Shader * pShader = (shaders::Shader*)context;
    pShader->setUniformVec4(nameHash, val);
}

static void prepare_mesh_attributes(const Mesh & mesh)
{
    // position
    if (mesh.hasVertPosition())
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)mesh.vertPositionOffset());
        glEnableVertexAttribArray(0);

        // normal
        if (mesh.hasVertNormal())
        {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)mesh.vertNormalOffset());
            glEnableVertexAttribArray(1);

            // uv
            if (mesh.hasVertUv())
            {
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)mesh.vertUvOffset());
                glEnableVertexAttribArray(2);

                // uv tangents
                if (mesh.hasTan())
                {
                    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)mesh.vertTanOffset());
                    glEnableVertexAttribArray(3);
                }
            }
        }
    }
}

void RendererGL::render()
{
    ASSERT(mIsInit);

    glClear(GL_COLOR_BUFFER_BIT);
    GL_CLEAR_DEPTH(1.0f);


#if RENDERTYPE == RENDERTYPE_CPUVOXEL
    mShaderSim.render(mRaycastCamera, mDirectionalLights);

    mpPresentShader->use();

    glActiveTexture(GL_TEXTURE0 + 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mPresentImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, kImgSize, kImgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, mShaderSim.frameBuffer());
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glBindImageTexture(0, mPresentImage, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGB8);

    glBindVertexArray(mPresentVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

#elif RENDERTYPE == RENDERTYPE_GPUVOXEL // #if RENDERTYPE == RENDERTYPE_CPUVOXEL
    mpVoxelCast->use();
    glDispatchCompute(16, 16, 1);

    mpPresentShader->use();

    glBindVertexArray(mPresentVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


#elif RENDERTYPE == RENDERTYPE_MESH
    ModelMgr<RendererGL>::MeshIterator meshIt = mpModelMgr->begin();
    ModelMgr<RendererGL>::MeshIterator meshItEnd = mpModelMgr->end();

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
        mpActiveShader->setUniformVec4(HASH::uvColor, Vec4(1.0f, 0.0f, 1.0f, 1.0f));

        static Mat4 view = Mat4::translation(Vec3(0.0f, 0.0f, -5.0f));
        Mat4 mvp = mProjection * view * matMeshInst.pModelInstance->transform;
        Mat3 normalTrans = Mat3(view * matMeshInst.pModelInstance->transform);

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
#endif // #elif RENDERTYPE == RENDERTYPE_MESH
}

template <typename T>
MessageResult RendererGL::message(const T & msgAcc)
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
        Vec3 normDir = Vec3::normalize(msgr.direction());
        Vec3 relDir = -normDir; // flip direction of vector relative to objects
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
    case HASH::renderer_move_camera:
    {
        messages::MoveCameraR<T> msgr(msgAcc);
        mRaycastCamera.move(msgr.position(), msgr.direction());
        break;
    }
    default:
        PANIC("Unknown renderer message: %d", msg.msgId);
    }

    return MessageResult::Consumed;
}

void RendererGL::loadMaterialMesh(Model::MaterialMesh & matMesh)
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

void RendererGL::setActiveShader(u32 nameHash)
{
    if (!mpActiveShader || mpActiveShader->nameHash() != nameHash)
    {
        mpActiveShader = getShader(nameHash);
        mpActiveShader->use();
    }
}

shaders::Shader * RendererGL::getShader(u32 nameHash)
{
    auto it = mShaders.find(nameHash);
    if (it != mShaders.end())
        return it->second;

    shaders::Shader* pShader = mShaderRegistry.constructShader(nameHash);
    mShaders[nameHash] = pShader;
    return pShader;
}


// Template decls so we can define message func here in the .cpp
template MessageResult RendererGL::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult RendererGL::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen

