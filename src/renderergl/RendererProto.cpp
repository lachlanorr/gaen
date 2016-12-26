//------------------------------------------------------------------------------
// RendererProto.cpp - Prototype and experimental rendering code
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

#include "math/common.h"
#include "math/matrices.h"
#include "math/vec3.h"

#include "assets/Gmdl.h"

#include "engine/MessageQueue.h"

#include "engine/messages/LightDistant.h"
#include "engine/messages/UidTransform.h"

#include "render_support/voxel27.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/shaders/Shader.h"
#include "renderergl/ShaderRegistry.h"

#include "renderergl/RendererProto.h"

namespace gaen
{


#if RENDERTYPE == RENDERTYPE_CPUFRAGVOXEL
static const u32 kPresentImgSize = 256;
static f32 kPresentSurface[] = { -1.0f, -1.0f, // pos 0
                                  0.0f,  0.0f, // uv  0

                                  1.0f, -1.0f, // pos 1
                                  1.0f,  0.0f, // uv  1

                                 -1.0f,  1.0f, // pos 2
                                  0.0f,  1.0f, // uv  2

                                  1.0f,  1.0f, // pos 3
                                  1.0f,  1.0f  // uv  3
};
#elif RENDERTYPE == RENDERTYPE_CPUCOMPVOXEL
static const u32 kPresentImgSize = 512;
static f32 kPresentSurface[] = { -1.0f, -1.0f, // pos 0
                                  0.0f,  0.0f, // uv  0

                                  1.0f, -1.0f, // pos 1
                                  320.0f / kPresentImgSize,  0.0f, // uv  1

                                 -1.0f,  1.0f, // pos 2
                                  0.0f,  192.0f / kPresentImgSize, // uv  2

                                  1.0f,  1.0f, // pos 3
                                  320.0f / kPresentImgSize, 192.0f / kPresentImgSize  // uv  3
};
#elif RENDERTYPE == RENDERTYPE_GPUFRAGVOXEL
static f32 kPresentSurface[] ={-1.0f, -1.0f, // pos 0
                               -1.0f, -1.0f, // uv  0

                               1.0f, -1.0f, // pos 1
                               1.0f, -1.0f, // uv  1

                               -1.0f, 1.0f, // pos 2
                               -1.0f, 1.0f, // uv  2

                               1.0f, 1.0f, // pos 3
                               1.0f, 1.0f, // uv  3
};
#elif RENDERTYPE == RENDERTYPE_GPUCOMPVOXEL
static const u32 kPresentImgSize = 2048;
static f32 kPresentSurface[] = { -1.0f, -1.0f, // pos 0
                                  0.0f,  0.0f, // uv  0

                                  1.0f, -1.0f, // pos 1
                                  1280.0f / 2048.0f,  0.0f, // uv  1

                                 -1.0f,  1.0f, // pos 2
                                  0.0f,  720.0f / 2048.0f, // uv  2

                                  1.0f,  1.0f, // pos 3
                                  1280.0f / 2048.0f,  720.0f / 2048.0f  // uv  3
};
#elif RENDERTYPE == RENDERTYPE_VOXEL27
static Voxel27PointData kVoxelPoints[] = { {  0,  0,  0,  0, 1 },
                                           {  0,  0, 80,  0, 1 },
                                           {  0, 80,  0,  0, 1 },
                                           {  0, 80, 80,  0, 1 },
                                           { 80,  0,  0,  0, 1 },
                                           { 80,  0, 80,  0, 1 },
                                           { 80, 80,  0,  0, 1 },
                                           { 80, 80, 80,  0, 1 }
};
static_assert(sizeof(kVoxelPoints) == sizeof(u32) * 8, "kVoxelPoints unexpected size");
#endif


void RendererProto::init(void * pRenderDevice,
                         u32 screenWidth,
                         u32 screenHeight)
{
    mpRenderDevice = pRenderDevice;
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

#if RENDERTYPE == RENDERTYPE_CPUFRAGVOXEL
    mShaderSim.init(kPresentImgSize, &mRaycastCamera);
#elif RENDERTYPE == RENDERTYPE_CPUCOMPVOXEL
    mShaderSim.init(uvec3(16, 16, 1), uvec3(20, 12, 1));
#endif

    mIsInit = true;
}

void RendererProto::fin()
{
    ASSERT(mIsInit);
}


static mat4 sMVPMat(1.0f);

void RendererProto::initViewport()
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

#if RENDERTYPE != RENDERTYPE_MESH // all voxel shaders
    mRaycastCamera.init(mScreenWidth, mScreenHeight, 60.0f, 0.1f, 1000.0f);
#endif

    // setup projection with current width/height
    mProjection = perspective(radians(60.0f),
                              mScreenWidth / static_cast<f32>(mScreenHeight),
                              0.1f,
                              100.0f);

    // setup gui projection, which is orthographic
    mGuiProjection = ortho(static_cast<f32>(mScreenWidth) * -0.5f,
                           static_cast<f32>(mScreenWidth) * 0.5f,
                           static_cast<f32>(mScreenHeight) * -0.5f,
                           static_cast<f32>(mScreenHeight) * 0.5f,
                           0.0f,
                           100.0f);

    //sMVPMat = glm::translation(glm::vec3(0.0f, 0.0f, 0.0f));
    //sMVPMat = glm::lookat(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    sMVPMat = mat4(1.0f);

    // LORRTODO: fix for new math.h, no more glm::f
//    sMVPMat = glm::rotate(sMVPMat, glm::pi<f32>() / 4.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//    sMVPMat = glm::rotate(sMVPMat, glm::pi<f32>() / 4.0f, glm::vec3(0.0f, 1.0f, 0.0f));

#if RENDERTYPE == RENDERTYPE_CPUFRAGVOXEL || RENDERTYPE == RENDERTYPE_CPUCOMPVOXEL || RENDERTYPE == RENDERTYPE_GPUCOMPVOXEL
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

    // vertex UV / RayScreenPos
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*)8);
    glEnableVertexAttribArray(1);

#elif RENDERTYPE == RENDERTYPE_VOXEL27

    Voxel27PointData vpx = extract_point_data(*reinterpret_cast<u32*>(&kVoxelPoints[4]));

    // Prepare GPU renderer presentation vars
    glGenVertexArrays(1, &mPresentVAO);
    glBindVertexArray(mPresentVAO);

    glGenBuffers(1, &mPresentVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mPresentVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVoxelPoints) - (4 * 0), kVoxelPoints, GL_STATIC_DRAW);

    mpPresentShader = getShader(HASH::voxel27);
    mpPresentShader->use();

    // vertex positions
    glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

#endif


#if RENDERTYPE == RENDERTYPE_CPUFRAGVOXEL || RENDERTYPE == RENDERTYPE_CPUCOMPVOXEL
    // prep image
    glActiveTexture(GL_TEXTURE0 + 0);
    glGenTextures(1, &mPresentImage);
    glBindTexture(GL_TEXTURE_2D, mPresentImage);

    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, kPresentImgSize, kPresentImgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

#elif RENDERTYPE == RENDERTYPE_GPUFRAGVOXEL
    static const f32 kRad = 2.0f;
    mVoxelRoot = set_shape_generic(mVoxelWorld, 0, 0, 3, vec3(1.0f, 2.0f, -20.0f), kRad, Mat3::rotation(vec3(0.0f, 0.0f, 0.0f)), SphereHitTest(kRad));

    // prep voxel cast shader
    mpVoxelCast = getShader(HASH::voxel_cast_frag);

    mVoxelRootsImageLocation = mpVoxelCast->textureLocation(HASH::un_VoxelRoots, GL_UNSIGNED_INT_IMAGE_BUFFER);

    glGenBuffers(1, &mVoxelRoots);
    glBindBuffer(GL_ARRAY_BUFFER, mVoxelRoots);
    glBufferData(GL_ARRAY_BUFFER,
        mVoxelWorld.voxelRoots()->bufferSize(),
        mVoxelWorld.voxelRoots()->buffer(),
        GL_DYNAMIC_COPY);

    glGenTextures(1, &mVoxelRootsImage);
    glBindTexture(GL_TEXTURE_BUFFER, mVoxelRootsImage);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32UI, mVoxelRoots);



    mVoxelDataImageLocation = mpVoxelCast->textureLocation(HASH::un_VoxelData, GL_UNSIGNED_INT_IMAGE_BUFFER);

    glGenBuffers(1, &mVoxelData);
    glBindBuffer(GL_ARRAY_BUFFER, mVoxelData);
    glBufferData(GL_ARRAY_BUFFER,
                 mVoxelWorld.imageBufferSize(0),
                 mVoxelWorld.imageBuffer(0),
                 GL_DYNAMIC_COPY);

    glGenTextures(1, &mVoxelDataImage);
    glBindTexture(GL_TEXTURE_BUFFER, mVoxelDataImage);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32UI, mVoxelData);

#elif RENDERTYPE == RENDERTYPE_GPUCOMPVOXEL
    static const f32 kRad = 2.0f;
    mVoxelRoot = set_shape_generic(mVoxelWorld, 0, 0, 3, vec3(1.0f, 2.0f, -20.0f), kRad, Mat3::rotation(vec3(0.0f, 0.0f, 0.0f)), SphereHitTest(kRad));

    // prep voxel cast shader
    mpVoxelCast = getShader(HASH::voxel_cast);

    // prep present image (frameBuffer)
    mPresentImageLocation = mpVoxelCast->textureLocation(HASH::un_FrameBuffer, GL_IMAGE_2D);

    glActiveTexture(GL_TEXTURE0 + mPresentImageLocation);
    glGenTextures(1, &mPresentImage);
    glBindTexture(GL_TEXTURE_2D, mPresentImage);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, kPresentImgSize, kPresentImgSize);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindImageTexture(mPresentImageLocation, mPresentImage, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);



    mVoxelRootsImageLocation = mpVoxelCast->textureLocation(HASH::un_VoxelRoots, GL_UNSIGNED_INT_IMAGE_BUFFER);

    glGenBuffers(1, &mVoxelRoots);
    glBindBuffer(GL_ARRAY_BUFFER, mVoxelRoots);
    glBufferData(GL_ARRAY_BUFFER,
        mVoxelWorld.voxelRoots()->bufferSize(),
        mVoxelWorld.voxelRoots()->buffer(),
        GL_DYNAMIC_COPY);

    glGenTextures(1, &mVoxelRootsImage);
    glBindTexture(GL_TEXTURE_BUFFER, mVoxelRootsImage);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32UI, mVoxelRoots);



    mVoxelDataImageLocation = mpVoxelCast->textureLocation(HASH::un_VoxelData, GL_UNSIGNED_INT_IMAGE_BUFFER);

    glGenBuffers(1, &mVoxelData);
    glBindBuffer(GL_ARRAY_BUFFER, mVoxelData);
    glBufferData(GL_ARRAY_BUFFER,
                 mVoxelWorld.imageBufferSize(0),
                 mVoxelWorld.imageBuffer(0),
                 GL_DYNAMIC_COPY);

    glGenTextures(1, &mVoxelDataImage);
    glBindTexture(GL_TEXTURE_BUFFER, mVoxelDataImage);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32UI, mVoxelData);

#endif // #elif RENDERTYPE == RENDERTYPE_GPUCOMPVOXEL


}

static void set_shader_vec4_var(u32 nameHash, const vec4 & val, void * context)
{
    shaders::Shader * pShader = (shaders::Shader*)context;
    pShader->setUniformVec4(nameHash, val);
}

static void prepare_gmdl_attributes(const Gmdl & gmdl)
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

void RendererProto::render()
{
    ASSERT(mIsInit);

    glClear(GL_COLOR_BUFFER_BIT);
    GL_CLEAR_DEPTH(1.0f);


#if RENDERTYPE == RENDERTYPE_CPUFRAGVOXEL || RENDERTYPE == RENDERTYPE_CPUCOMPVOXEL
    mShaderSim.render(mRaycastCamera, mLightDistants);

    mpPresentShader->use();

    glActiveTexture(GL_TEXTURE0 + 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mPresentImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, kPresentImgSize, kPresentImgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, mShaderSim.frameBuffer());
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glBindImageTexture(0, mPresentImage, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGB8);

    glBindVertexArray(mPresentVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

#elif RENDERTYPE == RENDERTYPE_GPUFRAGVOXEL // #if RENDERTYPE == RENDERTYPE_CPUFRAGVOXEL
    mpVoxelCast->use();
    if (mVoxelRootsImageLocation != -1)
        glBindImageTexture(mVoxelRootsImageLocation, mVoxelRootsImage, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32UI);
    if (mVoxelDataImageLocation != -1)
        glBindImageTexture(mVoxelDataImageLocation, mVoxelDataImage, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32UI);

    //mpVoxelCast->setUniformUint(HASH::un_VoxelRootCount, mVoxelWorld.voxelRootCount());
    mpVoxelCast->setUniformVec3(HASH::un_CameraPos, mRaycastCamera.position());
    mpVoxelCast->setUniformVec4(HASH::un_CameraDir, mRaycastCamera.direction());
    mpVoxelCast->setUniformMat4(HASH::un_CameraProjectionInv, mRaycastCamera.projectionInv());

    glBindVertexArray(mPresentVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


#elif RENDERTYPE == RENDERTYPE_GPUCOMPVOXEL // #elif RENDERTYPE == RENDERTYPE_GPUFRAGVOXEL
    mpVoxelCast->use();
    glBindImageTexture(mPresentImageLocation, mPresentImage, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glBindImageTexture(mVoxelRootsImageLocation, mVoxelRootsImage, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32UI);
    glBindImageTexture(mVoxelDataImageLocation, mVoxelDataImage, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32UI);

    //mpVoxelCast->setUniformUint(HASH::un_VoxelRootCount, mVoxelWorld.voxelRootCount());
    mpVoxelCast->setUniformVec3(HASH::un_CameraPos, mRaycastCamera.position());
    mpVoxelCast->setUniformVec4(HASH::un_CameraDir, mRaycastCamera.direction());
    mpVoxelCast->setUniformMat4(HASH::un_CameraProjectionInv, mRaycastCamera.projectionInv());

    glDispatchCompute(160, 90, 1);
    //glDispatchCompute(80, 45, 1);

    mpPresentShader->use();

    glBindVertexArray(mPresentVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

#elif RENDERTYPE == RENDERTYPE_VOXEL27
    mat4 proj = mRaycastCamera.projection();
    mat4 view = mRaycastCamera.view();

    mat4 mvp = mRaycastCamera.projection() * mRaycastCamera.view();
    mpPresentShader->setUniformMat4(HASH::un_MVP, mvp);

    mpPresentShader->use();
    glBindVertexArray(mPresentVAO);
    glDrawArrays(GL_POINTS, 0, 8);

#elif RENDERTYPE == RENDERTYPE_MESH
#error Mesh rendering needs to be reimplemented based on new code in RendererMesh.h/cpp
#endif // #elif RENDERTYPE == RENDERTYPE_MESH
}

template <typename T>
MessageResult RendererProto::message(const T & msgAcc)
{
    const Message & msg = msgAcc.message();

    switch(msg.msgId)
    {
    case HASH::light_directional_insert:
    {
        /*
        messages::LightDistantR<T> msgr(msgAcc);
        vec3 normDir = normalize(msgr.direction());
        vec3 relDir = -normDir; // flip direction of vector relative to objects
        mLightDistants.emplace_back(msgAcc.message().source,
                                    RenderObject::next_uid(),
                                    HASH::default,
                                    Color(255, 255, 255, 255),
                                    1.0f,
                                    relDir,
                                    msgr.color());
                                    */
        break;
    }
    case HASH::light_directional_update:
    {
        /*
        messages::LightDistantR<T> msgr(msgAcc);
        mLightDistants.emplace_back(msgAcc.message().source,
                                    msgr.direction(),
                                    msgr.color());
                                    */
        break;
    }
    case HASH::camera_transform:
    {
#if RENDERTYPE == RENDERTYPE_CPUFRAGVOXEL || RENDERTYPE == RENDERTYPE_CPUCOMPVOXEL || RENDERTYPE == RENDERTYPE_GPUFRAGVOXEL || RENDERTYPE == RENDERTYPE_GPUCOMPVOXEL
// LORRTODO: Support uid/stage cameras
//        messages::MoveCameraR<T> msgr(msgAcc);
//        mRaycastCamera.move(msgr.position(), msgr.direction());
        ERR("Need support for uid/stage based cameras");
#endif
        break;
    }
    default:
        PANIC("Unknown renderer message: %d", msg.msgId);
    }

    return MessageResult::Consumed;
}


void RendererProto::setActiveShader(u32 nameHash)
{
    if (!mpActiveShader || mpActiveShader->nameHash() != nameHash)
    {
        mpActiveShader = getShader(nameHash);
        mpActiveShader->use();
    }
}

shaders::Shader * RendererProto::getShader(u32 nameHash)
{
    auto it = mShaders.find(nameHash);
    if (it != mShaders.end())
        return it->second;

    shaders::Shader* pShader = mShaderRegistry.constructShader(nameHash);
    mShaders[nameHash] = pShader;
    return pShader;
}


// Template decls so we can define message func here in the .cpp
template MessageResult RendererProto::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult RendererProto::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen

