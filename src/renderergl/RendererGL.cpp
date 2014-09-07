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
#include "engine/messages/InsertModelInstance.h"
#include "engine/ModelMgr.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/RendererGL.h"

namespace gaen
{

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

    mIsInit = true;
}

void RendererGL::fin()
{
    ASSERT(mIsInit);
    mpModelMgr->fin();
    GDELETE(mpModelMgr);
}

void RendererGL::render()
{
    ASSERT(mIsInit);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0f);

    ModelMgr<RendererGL>::MeshIterator meshIt = mpModelMgr->begin();
    ModelMgr<RendererGL>::MeshIterator meshItEnd = mpModelMgr->end();

//--//    mesh_->render(projection, modelView_, currProgram);

    while (meshIt != meshItEnd)
    {
        // Set current program if necessary
//--//        if (shader_->programId() != currProgram)
//--//        {
//--//            glUseProgram(shader_->programId());
//--//            currProgram = shader_->programId();
//--//        }
//--//        glUniformMatrix4fv(shader_->uniform(eUniform_Projection), 1, 0, projection.elems);
//--//        glBindBuffer(GL_ARRAY_BUFFER, vertBufferId_);
//--//
//--//        // position
//--//        glEnableVertexAttribArray(eAttrib_Position);
//--//        glVertexAttribPointer(eAttrib_Position, 3, GL_FLOAT, GL_FALSE, vertStride_, reinterpret_cast<GLvoid*>(kOffsetPosition));
//--//        
//--//        // texture
//--//        if (vertType_ >= 'B')
//--//        {
//--//            glEnableVertexAttribArray(eAttrib_UV);
//--//            glVertexAttribPointer(eAttrib_UV, 2, GL_FLOAT, GL_FALSE, vertStride_, reinterpret_cast<GLvoid*>(kOffsetUV));
//--//        }
//--//
//--//        // normal
//--//        if (vertType_ >= 'C')
//--//        {
//--//            glEnableVertexAttribArray(eAttrib_Normal);
//--//            glVertexAttribPointer(eAttrib_Normal, 3, GL_FLOAT, GL_FALSE, vertStride_, reinterpret_cast<GLvoid*>(kOffsetNormal));
//--//        }
//--//
//--//        // tangent
//--//        if (vertType_ >= 'D')
//--//        {
//--//            glEnableVertexAttribArray(eAttrib_Tangent);
//--//            glVertexAttribPointer(eAttrib_Tangent, 3, GL_FLOAT, GL_FALSE, vertStride_, reinterpret_cast<GLvoid*>(kOffsetTangent));
//--//        }
//--//
//--//        // Bind triangle buffer
//--//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBufferId_);
//--//
//--//        // Draw our triangles
//--//        glDrawElements(GL_TRIANGLES, elemCount_, GL_UNSIGNED_SHORT, 0);
//--//

        ++meshIt;
    }
}

void RendererGL::initViewport()
{
    ASSERT(mIsInit);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do

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

MessageResult RendererGL::message(const MessageQueueAccessor & msgAcc)
{
    const Message & msg = msgAcc.message();

    switch(msg.msgId)
    {
    case HASH::renderer_insert_model_instance:
    {
        msg::InsertModelInstanceQR msgr(msgAcc);
        mpModelMgr->insertModelInstance(msgr.instanceId(),
                                      msgr.model(),
                                      msgr.worldTransform(),
                                      msgr.isAssetManaged());
        break;
    }
    case HASH::renderer_remove_model_instance:
    {
        model_instance_id instanceId = msg.payload.u;
        mpModelMgr->removeModelInstance(instanceId);
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

    if (mesh.rendererVertsId() == -1)
    {
        glGenBuffers(1, &mesh.rendererVertsId());
        glBindBuffer(GL_ARRAY_BUFFER, mesh.rendererVertsId());
        glBufferData(GL_ARRAY_BUFFER, mesh.vertsSize(), mesh.verts(), GL_STATIC_DRAW);
    }

    if (mesh.rendererPrimsId() == -1)
    {
        glGenBuffers(1, &mesh.rendererVertsId());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.rendererPrimsId());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.primsSize(), mesh.prims(), GL_STATIC_DRAW);
    }
}

} // namespace gaen

