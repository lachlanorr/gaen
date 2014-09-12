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

static const char * sVertShaderCode = 
    "#version 330 core\n"
    "in vec4 vPosition;\n"
    "uniform mat4 mvp;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = mvp * vPosition;\n"
    "}\n";

/*
    "layout(location=0) in vec4 vPosition;\n"



"attribute vec3 position; \n"
    "varying vec4 colorVarying; \n"
    "uniform mat4 modelView; \n"
    "uniform mat4 projection; \n"
    "void main() \n"
    "{ \n"
    "    //gl_Position = projection * modelView * vec4(position, 1.0); \n"
    "    gl_Position = vec4(position, 1.0);\n"
    "    colorVarying = vec4(0.0, 1.0, 1.0, 1.0); \n"
    "} \n";
*/


static const char * sFragShaderCode =
    "#version 330 core\n"
    "out vec4 fColor;\n"
    "void main()\n"
    "{\n"
    "    fColor = vec4(0.0, 0.0, 1.0, 1.0);\n"
    "}\n";

/*    "varying vec4 colorVarying; \n"
    "void main() \n"
    "{ \n"
    "    gl_FragColor = colorVarying; \n"
    "} \n";
*/

static bool compile_shader(GLuint * pShader, GLenum type, const char * shaderCode)
{
    GLuint shader;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        char errMsg[256];
        int len;
        glGetShaderInfoLog(shader, 256, &len, errMsg);

        glDeleteShader(shader);
        ERR("Failed to compile shader: %s", errMsg);
        return false;
    }

    *pShader = shader;
    return true;
}

static GLuint sProgramId = -1;
static GLint sMVPUniform = -1;
static Mat4 sMVPMat(1.0f);

static bool build_program(GLuint * pProgramId,
                          GLint * pMVPUniform,
                          const char * vertShaderCode,
                          const char * fragShaderCode)
{
    // create program
    GLuint programId = glCreateProgram();

    GLuint vertShader, fragShader;
    // load shaders
    if (!compile_shader(&vertShader, GL_VERTEX_SHADER, vertShaderCode) ||
        !compile_shader(&fragShader, GL_FRAGMENT_SHADER, fragShaderCode))
    {
        glDeleteProgram(programId);
        ERR("Failed to compile shaders");
        return false;
    }

    // attach shaders to program
    glAttachShader(programId, vertShader);
    glAttachShader(programId, fragShader);

    // bind attribute locations
    //glBindAttribLocation(programId, 0, "position");
    //glBindAttribLocation(programId, 1, "uv");
    //--//glBindAttribLocation(programId, eAttrib_Normal, "normal");

    // link program
    GLint status;
    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        glDeleteProgram(programId);
        ERR("Failed to link shader program.");
        return false;
    }

    // Get uniform locations
    GLint mvpUniform = glGetUniformLocation(programId, "mvp");
    //--//uniforms[eUniform_NormalTransform] = glGetUniformLocation(program_, "normalTransform");

    // Release vertex and fragment shaders
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    *pProgramId = programId;
    *pMVPUniform = mvpUniform;

    return true;
}

void RendererGL::initViewport()
{
    ASSERT(mIsInit);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glDisable(GL_CULL_FACE);

    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    //glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do

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

    //sMVPMat = Mat4::translation(Vec3(0.0f, 0.0f, 0.0f));
    //sMVPMat = Mat4::lookat(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f));
    sMVPMat = Mat4::rotation(Vec3(20.0f, -20.0f, 0.0f));

    if (!build_program(&sProgramId, &sMVPUniform, sVertShaderCode, sFragShaderCode))
    {
        PANIC("Failed to build_program for default shader");
    }

    glUseProgram(sProgramId);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void RendererGL::render()
{
    ASSERT(mIsInit);

    glClear(GL_COLOR_BUFFER_BIT);

//    glBindVertexArray(sVAO);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glFlush();


//    return;
    //


    glClearDepth(1.0f);

    ModelMgr<RendererGL>::MeshIterator meshIt = mpModelMgr->begin();
    ModelMgr<RendererGL>::MeshIterator meshItEnd = mpModelMgr->end();

    //--//    mesh_->render(projection, modelView_, currProgram);

    glUseProgram(sProgramId);

    while (meshIt != meshItEnd)
    {
        const MaterialMeshInstance & matMeshInst = *meshIt;
        Mesh & mesh = matMeshInst.pMaterialMesh->mesh();

        Mat4 mvp = matMeshInst.pModelInstance->worldTransform * mProjection;

        glUniformMatrix4fv(sMVPUniform, 1, 0, sMVPMat.elems);

        glBindVertexArray(mesh.rendererReserved(kMSHR_VAO));

        // Draw our triangles
        glDrawElements(GL_TRIANGLES, mesh.indexCount(), GL_UNSIGNED_SHORT, 0);

        ++meshIt;
    }
}

MessageResult RendererGL::message(const MessageQueueAccessor & msgAcc)
{
    const Message & msg = msgAcc.message();

    switch(msg.msgId)
    {
    case HASH::renderer_insert_model_instance:
    {
        msg::InsertModelInstanceQR msgr(msgAcc);
        mpModelMgr->insertModelInstance(msgAcc.message().source,
                                        msgr.uid(),
                                        msgr.model(),
                                        msgr.worldTransform(),
                                        msgr.isAssetManaged());
        break;
    }
    case HASH::renderer_remove_model_instance:
    {
        mpModelMgr->removeModelInstance(msg.source, msg.payload.u);
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

    if (mesh.rendererReserved(kMSHR_VAO) == -1)
    {
        glGenVertexArrays(1, &mesh.rendererReserved(kMSHR_VAO));
    }

    glBindVertexArray(mesh.rendererReserved(kMSHR_VAO));

    if (mesh.rendererReserved(kMSHR_VertBuffer) == -1)
    {
        glGenBuffers(1, &mesh.rendererReserved(kMSHR_VertBuffer));
        glBindBuffer(GL_ARRAY_BUFFER, mesh.rendererReserved(kMSHR_VertBuffer));
        glBufferData(GL_ARRAY_BUFFER, mesh.vertsSize(), mesh.verts(), GL_STATIC_DRAW);

        // position
        glVertexAttribPointer(0 /* eAttrib_position */, 3, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)0);
        glEnableVertexAttribArray(0); // eAttrib_Position

        // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)12);
        glEnableVertexAttribArray(1);

    }

    if (mesh.rendererReserved(kMSHR_PrimBuffer) == -1)
    {
        glGenBuffers(1, &mesh.rendererReserved(kMSHR_PrimBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.rendererReserved(kMSHR_PrimBuffer));
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.primsSize(), mesh.prims(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace gaen

