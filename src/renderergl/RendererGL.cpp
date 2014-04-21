//------------------------------------------------------------------------------
// RendererGL.cpp - OpenGL Renderer
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#include "renderergl/stdafx.h"

#include "core/base_defines.h"

#include "engine/MessageQueue.h"
#include "engine/message_defs/ModelInstanceMessage.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/RendererGL.h"

namespace gaen
{

static const size_t kExpectedMaxModelCount = 4096;

void RendererGL::init(device_context deviceContext,
                      render_context renderContext,
                      u32 screenWidth,
                      u32 screenHeight)
{
    mDeviceContext = deviceContext;
    mRenderContext = renderContext;
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    mIsInit = true;
}

void RendererGL::fin()
{
    mModelMgr.fin();
}

void RendererGL::render()
{
    ASSERT(mIsInit);

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RendererGL::initViewport()
{
    ASSERT(mIsInit);

    // Make sure we don't divide by zero
    if (mScreenHeight==0)
    {
        mScreenHeight=1;
    }

    // reset viewport
    glViewport(0, 0, mScreenWidth, mScreenHeight);

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
}

MessageResult RendererGL::message(const MessageQueue::MessageAccessor& msgAcc)
{
    const Message & msg = msgAcc.message();

    switch(msg.msgId)
    {
    case FNV::renderer_insert_model_instance:
    {
        ModelInstanceReader rdr(msgAcc);
        mModelMgr.insertModelInstance(rdr.instanceId(),
                                      rdr.model(),
                                      rdr.worldTransform(),
                                      rdr.isAssetManaged());
        break;
    }
    case FNV::renderer_remove_model_instance:
    {
        model_instance_id instanceId = msg.payload.u;
        mModelMgr.removeModelInstance(instanceId);
        break;
    }
    default:
        PANIC("Unknown renderer message: %d", msg.msgId);
    }

    return MessageResult::Consumed;
}


} // namespace gaen

