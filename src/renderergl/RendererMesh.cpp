//------------------------------------------------------------------------------
// RendererMesh.cpp - OpenGL Mesh renderer
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

#include <algorithm>

#include "renderergl/gaen_opengl.h"
#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

#if HAS(ENABLE_EDITOR)
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#endif

#include "core/base_defines.h"
#include "core/gamevars.h"

#include "math/matrices.h"

#include "assets/Gimg.h"
#include "assets/Gmdl.h"

#include "engine/MessageQueue.h"
#include "engine/Asset.h"
#include "engine/AssetMgr.h"

#include "engine/messages/CameraOrtho.h"
#include "engine/messages/CameraPersp.h"
#include "engine/messages/Handle.h"
#include "engine/messages/LightDistant.h"
#include "engine/messages/ModelInstance.h"
#include "engine/messages/NotifyWatcherMat43.h"
#include "engine/messages/SpriteAnim.h"
#include "engine/messages/SpriteInstance.h"
#include "engine/messages/UidTransform.h"
#include "engine/messages/UidScalarTransform.h"
#include "engine/messages/UidColor.h"
#include "engine/messages/UidInteger.h"
#include "engine/messages/UidVec3.h"
#include "engine/messages/UidScalar.h"

#include "renderergl/shaders/Shader.h"
#include "renderergl/ShaderRegistry.h"

#include "cara/Cara.h"

#include "renderergl/RendererMesh.h"

namespace gaen
{

GAMEVAR_DECL_BOOL(testui, false);

RendererMesh::RendererMesh()
  : mModelStages(this)
  , mSpriteStages(this)
{}

void RendererMesh::init(void * pRenderDevice,
                        u32 screenWidth,
                        u32 screenHeight)
{
    mpRenderDevice = pRenderDevice;
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    Camera defaultModelCamera(kRendererTaskId,
                              HASH::main,
                              1.0f,
                              perspective(radians(60.0f),
                                          mScreenWidth / (f32)mScreenHeight,
                                          0.1f,
                                          100000.0f),
                              mat43(1.0f));
    mModelStages.cameraSetDefault(defaultModelCamera);

    Camera defaultSpriteCamera(kRendererTaskId,
                               HASH::main,
                               1.0f,
                               ortho(mScreenWidth * -0.5f,
                                     mScreenWidth * 0.5f,
                                     mScreenHeight * -0.5f,
                                     mScreenHeight * 0.5f),
                               mat43(1.0f));
    mSpriteStages.cameraSetDefault(defaultSpriteCamera);

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
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    // Make sure we don't divide by zero


    glEnable(GL_MULTISAMPLE);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);    // The Type Of Depth Testing To Do
    glEnable(GL_DEPTH_TEST);   // Enables Depth Testing
    glDisable(GL_SCISSOR_TEST);


    if (mScreenHeight == 0)
    {
        mScreenHeight = 1;
    }

    // reset viewport
    glViewport(0, 0, mScreenWidth, mScreenHeight);

#if HAS(ENABLE_EDITOR)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)mpRenderDevice, true);
    ImGui_ImplOpenGL3_Init("#version 130");
#endif
}

void RendererMesh::set_shader_vec4_var(u32 nameHash, const vec4 & val, void * pContext)
{
    shaders::Shader * pShader = (shaders::Shader*)pContext;
    pShader->setUniformVec4(nameHash, val);
}

void RendererMesh::set_texture(u32 nameHash, u32 glId, void * pContext)
{
    RendererMesh * pRenderer = (RendererMesh*)pContext;
    pRenderer->setTexture(nameHash, glId);
}

void RendererMesh::setTexture(u32 nameHash, u32 glId)
{
    int textureUnit = activeShader().textureUnit(nameHash);
    ASSERT(textureUnit >= 0);

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, glId);
    if (nameHash == HASH::animations)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

static void image_format_and_type(u32 &pixelFormat, u32 &pixelType, PixelFormat format)
{
    switch (format)
    {
    case kPXL_RGBA32F:
        pixelFormat = GL_RGBA;
        pixelType = GL_FLOAT;
        break;
    case kPXL_RGBA8:
        pixelFormat = GL_RGBA;
        pixelType = GL_UNSIGNED_BYTE;
        break;
    default:
        PANIC("Unsupported PixelFormat: %d", format);
    }
}

u32 RendererMesh::loadTexture(const Gimg * pGimg)
{
    auto it = mLoadedTextures.find(pGimg->referencePathHash());
    if (it == mLoadedTextures.end())
    {
        PANIC_IF(pGimg->pixelFormat() == kPXL_Reference, "Gimg must be loaded before references");
        u32 glId = 0;
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &glId);
        glBindTexture(GL_TEXTURE_2D, glId);

        u32 pixelFormat = 0;
        u32 pixelType = 0;
        image_format_and_type(pixelFormat, pixelType, pGimg->pixelFormat());

        int err = glGetError();

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     pGimg->pixelFormat(),
                     pGimg->width(),
                     pGimg->height(),
                     0,
                     pixelFormat,
                     pixelType,
                     pGimg->pixels());

        err = glGetError();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        err = glGetError();

        mLoadedTextures.emplace(std::piecewise_construct,
                                std::forward_as_tuple(pGimg->referencePathHash()),
                                std::forward_as_tuple(glId, 1));
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
    auto it = mLoadedTextures.find(pGimg->referencePathHash());
    if (it != mLoadedTextures.end())
    {
        ASSERT(it->second.refCount > 0);
        it->second.refCount--;
        if (it->second.refCount == 0)
        {
            ASSERT(it->second.glId0 > 0);
            glDeleteTextures(1, &it->second.glId0);
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
                             std::forward_as_tuple(*pVertArrayId, *pVertBufferId, 1));
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
            ASSERT(it->second.glId0 > 0); // probably not if not OPENGL3
            ASSERT(it->second.glId1 > 0);
            glDeleteVertexArrays(1, &it->second.glId0);
            glDeleteBuffers(1, &it->second.glId1);
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
                             std::forward_as_tuple(*pPrimBufferId, 1));
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
            ASSERT(it->second.glId0 > 0);
            glDeleteBuffers(1, &it->second.glId0);
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

u32 RendererMesh::load_texture(const Gimg * pGimg, void * pContext)
{
    RendererMesh * pRenderer = (RendererMesh*)pContext;
    return pRenderer->loadTexture(pGimg);
}

void drawColorwheel(NVGcontext* vg, float x, float y, float w, float h, float t)
{
    int i;
    float r0, r1, ax, ay, bx, by, cx, cy, aeps, r;
    float hue = sinf(t * 0.12f);
    NVGpaint paint;

    nvgSave(vg);

    /*	nvgBeginPath(vg);
        nvgRect(vg, x,y,w,h);
        nvgFillColor(vg, nvgRGBA(255,0,0,128));
        nvgFill(vg);*/

    cx = x + w * 0.5f;
    cy = y + h * 0.5f;
    r1 = (w < h ? w : h) * 0.5f - 5.0f;
    r0 = r1 - 20.0f;
    aeps = 0.5f / r1;	// half a pixel arc length in radians (2pi cancels out).

    for (i = 0; i < 6; i++) {
        float a0 = (float)i / 6.0f * NVG_PI * 2.0f - aeps;
        float a1 = (float)(i + 1.0f) / 6.0f * NVG_PI * 2.0f + aeps;
        nvgBeginPath(vg);
        nvgArc(vg, cx, cy, r0, a0, a1, NVG_CW);
        nvgArc(vg, cx, cy, r1, a1, a0, NVG_CCW);
        nvgClosePath(vg);
        ax = cx + cosf(a0) * (r0 + r1) * 0.5f;
        ay = cy + sinf(a0) * (r0 + r1) * 0.5f;
        bx = cx + cosf(a1) * (r0 + r1) * 0.5f;
        by = cy + sinf(a1) * (r0 + r1) * 0.5f;
        paint = nvgLinearGradient(vg, ax, ay, bx, by, nvgHSLA(a0 / (NVG_PI * 2), 1.0f, 0.55f, 255), nvgHSLA(a1 / (NVG_PI * 2), 1.0f, 0.55f, 255));
        nvgFillPaint(vg, paint);
        nvgFill(vg);
    }

    nvgBeginPath(vg);
    nvgCircle(vg, cx, cy, r0 - 0.5f);
    nvgCircle(vg, cx, cy, r1 + 0.5f);
    nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 64));
    nvgStrokeWidth(vg, 1.0f);
    nvgStroke(vg);

    // Selector
    nvgSave(vg);
    nvgTranslate(vg, cx, cy);
    nvgRotate(vg, hue * NVG_PI * 2);

    // Marker on
    nvgStrokeWidth(vg, 2.0f);
    nvgBeginPath(vg);
    nvgRect(vg, r0 - 1, -3, r1 - r0 + 2, 6);
    nvgStrokeColor(vg, nvgRGBA(255, 255, 255, 192));
    nvgStroke(vg);

    paint = nvgBoxGradient(vg, r0 - 3, -5, r1 - r0 + 6, 10, 2, 4, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
    nvgBeginPath(vg);
    nvgRect(vg, r0 - 2 - 10, -4 - 10, r1 - r0 + 4 + 20, 8 + 20);
    nvgRect(vg, r0 - 2, -4, r1 - r0 + 4, 8);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, paint);
    nvgFill(vg);

    // Center triangle
    r = r0 - 6;
    ax = cosf(120.0f / 180.0f * NVG_PI) * r;
    ay = sinf(120.0f / 180.0f * NVG_PI) * r;
    bx = cosf(-120.0f / 180.0f * NVG_PI) * r;
    by = sinf(-120.0f / 180.0f * NVG_PI) * r;
    nvgBeginPath(vg);
    nvgMoveTo(vg, r, 0);
    nvgLineTo(vg, ax, ay);
    nvgLineTo(vg, bx, by);
    nvgClosePath(vg);
    paint = nvgLinearGradient(vg, r, 0, ax, ay, nvgHSLA(hue, 1.0f, 0.5f, 255), nvgRGBA(255, 255, 255, 255));
    nvgFillPaint(vg, paint);
    nvgFill(vg);
    paint = nvgLinearGradient(vg, (r + ax) * 0.5f, (0 + ay) * 0.5f, bx, by, nvgRGBA(0, 0, 0, 0), nvgRGBA(0, 0, 0, 255));
    nvgFillPaint(vg, paint);
    nvgFill(vg);
    nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 64));
    nvgStroke(vg);

    // Select circle on triangle
    ax = cosf(120.0f / 180.0f * NVG_PI) * r * 0.3f;
    ay = sinf(120.0f / 180.0f * NVG_PI) * r * 0.4f;
    nvgStrokeWidth(vg, 2.0f);
    nvgBeginPath(vg);
    nvgCircle(vg, ax, ay, 5);
    nvgStrokeColor(vg, nvgRGBA(255, 255, 255, 192));
    nvgStroke(vg);

    paint = nvgRadialGradient(vg, ax, ay, 7, 9, nvgRGBA(0, 0, 0, 64), nvgRGBA(0, 0, 0, 0));
    nvgBeginPath(vg);
    nvgRect(vg, ax - 20, ay - 20, 40, 40);
    nvgCircle(vg, ax, ay, 7);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, paint);
    nvgFill(vg);

    nvgRestore(vg);

    nvgRestore(vg);
}

static void render_test_nanovg()
{


    static struct NVGcontext* vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    nvgBeginFrame(vg, 1024, 728, 1.0);
    nvgBeginPath(vg);
    nvgRect(vg, 100, 100, 120, 30);
    nvgFillColor(vg, nvgRGBA(255,192,255,100));
    nvgFill(vg);

    drawColorwheel(vg, 500.0f, 500.0f, 100.0f, 100.0f, 0.0f);

    nvgEndFrame(vg);
}

void RendererMesh::render()
{
    ASSERT(mIsInit);

    // Undo nanovg stuff
    mpActiveShader = nullptr; // force us to re-load one of our shaders
    glEnable(GL_DEPTH_TEST);   // Enables Depth Testing
    glEnable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    mModelStages.render();

    glClear(GL_STENCIL_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    mSpriteStages.render();

    if (testui)
    {
        glClear(GL_STENCIL_BUFFER_BIT);
        render_test_nanovg();
    }
}

template <typename T>
MessageResult RendererMesh::message(const T & msgAcc)
{
#if HAS(MESSAGE_TRACING)
    LOG_INFO("MSG: Renderer %s(0x%x) -> %s(0x%x): %s(%s)", task_name(msgAcc.message().source), msgAcc.message().source, task_name(msgAcc.message().target), msgAcc.message().target, HASH::reverse_hash(msgAcc.message().msgId), HASH::reverse_hash(msgAcc.message().payload.u));
#endif
    const Message & msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::light_insert:
    {
        messages::LightDistantR<T> msgr(msgAcc);
        mModelStages.lightInsert(msgr.uid(),
                                 msgr.stageHash(),
                                 msgr.color(),
                                 msgr.ambient(),
                                 msgr.direction());
        break;
    }
    case HASH::light_direction:
    {
        messages::UidVec3R<T> msgr(msgAcc);
        mModelStages.lightDirection(msgr.uid(),
                                    msgr.vector());
        break;
    }
    case HASH::light_color:
    {
        messages::UidColorR<T> msgr(msgAcc);
        mModelStages.lightColor(msgr.uid(),
                                msgr.color());
        break;
    }
    case HASH::light_ambient:
    {
        messages::UidScalarR<T> msgr(msgAcc);
        mModelStages.lightAmbient(msgr.uid(),
                                  msgr.scalar());
        break;
    }
    case HASH::light_remove:
    {
        u32 uid = msg.payload.u;
        mModelStages.lightRemove(uid);
        break;
    }

    // models
    case HASH::model_insert:
    {
        messages::ModelInstanceR<T> msgr(msgAcc);
        mModelStages.itemInsert(msgr.modelInstance());
        break;
    }
    case HASH::model_transform:
    {
        messages::NotifyWatcherMat43R<T> msgr(msgAcc);
        ASSERT(msgr.valueType() == HASH::mat43);
        mModelStages.itemTransform(msgr.uid(), msgr.value());
        break;
    }
    case HASH::model_frame_offset:
    {
        messages::UidIntegerR<T> msgr(msgAcc);
        mModelStages.itemFrameOffset(msgr.uid(), msgr.integer());
        break;
    }
    case HASH::model_remove:
    {
        u32 uid = msg.payload.u;
        mModelStages.itemRemove(uid);
        break;
    }

    case HASH::model_stage_show:
    {
        u32 stageHash = msg.payload.u;
        mModelStages.show(stageHash);
        break;
    }
    case HASH::model_stage_hide:
    {
        u32 stageHash = msg.payload.u;
        mModelStages.hide(stageHash);
        break;
    }
    case HASH::model_stage_hide_all:
    {
        mModelStages.hideAll();
        break;
    }
    case HASH::model_stage_remove:
    {
        u32 stageHash = msg.payload.u;
        mModelStages.remove(stageHash);
        break;
    }
    case HASH::model_stage_remove_all:
    {
        mModelStages.removeAll();
        break;
    }
    case HASH::model_stage_camera_insert_persp:
    {
        messages::CameraPerspR<T> msgr(msgAcc);
        mModelStages.cameraInsertPersp(msgr.uid(),
                                       msgr.stageHash(),
                                       screenWidth(),
                                       screenHeight(),
                                       msgr.fov(),
                                       msgr.nearClip(),
                                       msgr.farClip(),
                                       msgr.view());
        break;
    }
    case HASH::model_stage_camera_insert_ortho:
    {
        messages::CameraOrthoR<T> msgr(msgAcc);
        mModelStages.cameraInsertOrtho(msgr.uid(),
                                       msgr.stageHash(),
                                       screenWidth(),
                                       screenHeight(),
                                       msgr.bounds(),
                                       msgr.nearClip(),
                                       msgr.farClip(),
                                       msgr.view());
        break;
    }
    case HASH::model_stage_camera_scale:
    {
        messages::UidScalarR<T> msgr(msgAcc);
        mModelStages.cameraScale(msgr.uid(),
                                 msgr.scalar());
        break;
    }
    case HASH::model_stage_camera_view:
    {
        messages::UidTransformR<T> msgr(msgAcc);
        mModelStages.cameraView(msgr.uid(),
                                msgr.transform());
        break;
    }
    case HASH::model_stage_camera_scale_and_view:
    {
        messages::UidScalarTransformR<T> msgr(msgAcc);
        mModelStages.cameraScaleAndView(msgr.uid(),
                                        msgr.scalar(),
                                        msgr.transform());
        break;
    }
    case HASH::model_stage_camera_activate:
    {
        u32 uid = msg.payload.u;
        mModelStages.cameraActivate(uid);
        break;
    }
    case HASH::model_stage_camera_remove:
    {
        u32 uid = msg.payload.u;
        mModelStages.cameraRemove(uid);
        break;
    }

    // sprites
    case HASH::sprite_insert:
    {
        messages::SpriteInstanceR<T> msgr(msgAcc);
        mSpriteStages.itemInsert(msgr.spriteInstance());
        break;
    }
    case HASH::sprite_anim:
    {
        messages::SpriteAnimR<T> msgr(msgAcc);
        mSpriteStages.itemAnimate(msgr.uid(), msgr.animHash(), msgr.animFrameIdx());
        break;
    }
    case HASH::sprite_transform:
    {
        messages::NotifyWatcherMat43R<T> msgr(msgAcc);
        ASSERT(msgr.valueType() == HASH::mat43);
        mSpriteStages.itemTransform(msgr.uid(), msgr.value());
        break;
    }
    case HASH::sprite_remove:
    {
        u32 uid = msg.payload.u;
        mSpriteStages.itemRemove(uid);
        break;
    }

    case HASH::sprite_stage_show:
    {
        u32 stageHash = msg.payload.u;
        mSpriteStages.show(stageHash);
        break;
    }
    case HASH::sprite_stage_hide:
    {
        u32 stageHash = msg.payload.u;
        mSpriteStages.hide(stageHash);
        break;
    }
    case HASH::sprite_stage_remove:
    {
        u32 stageHash = msg.payload.u;
        mSpriteStages.remove(stageHash);
        break;
    }

    case HASH::image_insert:
    {
        messages::HandleR<T> msgr(msgAcc);
        HandleP pHandle = msgr.handle();
        const Asset * pAsset = pHandle->data<Asset>();
        const Gimg * pGimg = pAsset->buffer<Gimg>();
        AssetMgr::addref_asset(kRendererTaskId, pAsset);
        mImageOwners[msgr.taskId()].push_back(pAsset);
        loadTexture(pGimg);
        break;
    }
	case HASH::remove_task__:
	{
        task_id taskIdToRemove = msg.payload.u;
        auto itL = mImageOwners.find(taskIdToRemove);
        // It's ok if we don't find it, it just means this task had no models
        if (itL != mImageOwners.end())
        {
            for (const Asset * pAsset : itL->second)
            {
                const Gimg * pGimg = pAsset->buffer<Gimg>();
                unloadTexture(pGimg);
                AssetMgr::release_asset(kRendererTaskId, pAsset);
            }
            mImageOwners.erase(itL);
        }
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


// Template decls so we can define message func here in the .cpp
template MessageResult RendererMesh::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult RendererMesh::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen

