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
#if HAS(OPENGL3)
    "#version 330 core\n"

    "in vec4 vPosition;\n"
    "in vec3 vNormal;\n"

    "uniform mat4 umMVP;\n"
    "uniform mat3 umNormal;\n"
    "uniform vec4 uvColor;\n"
    "uniform vec3 uvLightDirection;\n"
    "uniform vec4 uvLightColor;\n"

    "out vec4 vColor;\n"

    "void main()\n"
    "{\n"
    "    vec3 normalTrans = normalize(umNormal * vNormal);\n"
    "    float intensity = max(dot(normalTrans, uvLightDirection), 0.0);\n"
    "    intensity += min(intensity + 0.3, 1.0);\n"
    "    vColor = intensity * uvColor;\n"
    "    //vColor = vec4((umNormal * vNormal), 1.0);\n"
    "    //vColor = vec4(dot(uvLightDirection, normalTrans));\n"
    "    //vColor = abs(dot(uvLightDirection, normalTrans)) * uvColor;\n"
    "    //vColor = vec4(abs(uvLightDirection), 1.0);\n"
    "    //vColor = 0.5 * uvColor;\n"
    "    //vColor = vec4(1.0, 1.0, 0.0, 0.6);\n"
    "    gl_Position = umMVP * vPosition;\n"
    "}\n";
#else //#if HAS(OPENGL3)
 #if IS_PLATFORM_IOS
    "precision mediump float;\n"
 #endif
    "attribute vec4 vPosition;\n"
    "attribute vec3 vNormal;\n"

    "uniform mat4 umMVP;\n"
    "uniform mat3 umNormal;\n"
    "uniform vec4 uvColor;\n"
    "uniform vec3 uvLightDirection;\n"
    "uniform vec4 uvLightColor;\n"

    "varying vec4 vColor;\n"

    "void main()\n"
    "{\n"
    "    vec3 normalTrans = normalize(umNormal * vNormal);\n"
    "    float intensity = max(dot(normalTrans, uvLightDirection), 0.0);\n"
    "    intensity += min(intensity + 0.3, 1.0);\n"
    "    vColor = intensity * uvColor;\n"
    "    gl_Position = umMVP * vPosition;\n"
    "}\n";
#endif // #else //#if HAS(OPENGL3)



static const char * sFragShaderCode =
#if HAS(OPENGL3)
    "#version 330 core\n"

    "in vec4 vColor;\n"
    "out vec4 color;\n"

    "void main()\n"
    "{\n"
    "    color = vColor;\n"
    "}\n";
#else // #if HAS(OPENGL3)
 #if IS_PLATFORM_IOS
    "precision mediump float;\n"
 #endif

    "varying vec4 vColor;\n"

    "void main()\n"
    "{\n"
    "    gl_FragColor = vColor;\n"
    "}\n";
#endif // #else // #if HAS(OPENGL3)

static GLuint sProgramId = -1;
static GLint sMVPUniform = -1;
static GLint sNormalUniform = -1;
static GLint sColorUniform = -1;
static GLint sLightDirectionUniform = -1;
static GLint sLightColorUniform = -1;

static Mat4 sMVPMat(1.0f);

bool RendererGL::compile_shader(GLuint * pShader, GLenum type, const char * shaderCode, const char * headerCode)
{
    const char * shaderCodes[2];
    u32 shaderCodesSize = 0;

    if (headerCode)
    {
        shaderCodes[shaderCodesSize++] = headerCode;
    }
    shaderCodes[shaderCodesSize++] = shaderCode;

    GLuint shader;

    shader = glCreateShader(type);
    glShaderSource(shader, shaderCodesSize, shaderCodes, NULL);
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

static bool build_program(GLuint * pProgramId,
                          const char * vertShaderCode,
                          const char * fragShaderCode)
{
    // create program
    GLuint programId = glCreateProgram();

    GLuint vertShader, fragShader;
    // load shaders
    if (!RendererGL::compile_shader(&vertShader, GL_VERTEX_SHADER, vertShaderCode) ||
        !RendererGL::compile_shader(&fragShader, GL_FRAGMENT_SHADER, fragShaderCode))
    {
        glDeleteProgram(programId);
        ERR("Failed to compile shaders");
        return false;
    }

    // attach shaders to program
    glAttachShader(programId, vertShader);
    glAttachShader(programId, fragShader);

#if !HAS(OPENGL3)
    // bind attribute locations
    glBindAttribLocation(programId, 0, "vPosition");
    glBindAttribLocation(programId, 1, "vNormal");
#endif
    
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

    // Release vertex and fragment shaders
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    *pProgramId = programId;

    return true;
}

void RendererGL::initViewport()
{
    ASSERT(mIsInit);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

    if (!build_program(&sProgramId, sVertShaderCode, sFragShaderCode))
    {
        PANIC("Failed to build_program for default shader");
    }

    // Get uniform locations
    sMVPUniform = glGetUniformLocation(sProgramId, "umMVP");
    sNormalUniform = glGetUniformLocation(sProgramId, "umNormal");
    sColorUniform = glGetUniformLocation(sProgramId, "uvColor");
    sLightDirectionUniform = glGetUniformLocation(sProgramId, "uvLightDirection");
    sLightColorUniform = glGetUniformLocation(sProgramId, "uvLightColor");

    glUseProgram(sProgramId);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void RendererGL::render()
{
    ASSERT(mIsInit);

    glClear(GL_COLOR_BUFFER_BIT);
    GL_CLEAR_DEPTH(1.0f);

    ModelMgr<RendererGL>::MeshIterator meshIt = mpModelMgr->begin();
    ModelMgr<RendererGL>::MeshIterator meshItEnd = mpModelMgr->end();

    glUseProgram(sProgramId);

    if (mDirectionalLights.size() > 0)
    {
        const DirectionalLight & light = mDirectionalLights.front();
        glUniform3fv(sLightDirectionUniform, 1, light.direction.elems);
        glUniform4fv(sLightColorUniform, 1, light.color.elems);
    }

    while (meshIt != meshItEnd)
    {
        const MaterialMeshInstance & matMeshInst = *meshIt;
        Mesh & mesh = matMeshInst.pMaterialMesh->mesh();
        Material & mat = matMeshInst.pMaterialMesh->material();

        static Mat4 view = Mat4::translation(Vec3(0.0f, 0.0f, -5.0f));
        Mat4 mvp = mProjection * view * matMeshInst.pModelInstance->transform;
        Mat3 normalTrans = Mat3(view * matMeshInst.pModelInstance->transform);

        glUniformMatrix4fv(sMVPUniform, 1, 0, mvp.elems);
        glUniformMatrix3fv(sNormalUniform, 1, 0, normalTrans.elems);
        glUniform4fv(sColorUniform, 1, mat.color().elems);
#if HAS(OPENGL3)
        glBindVertexArray(mesh.rendererReserved(kMSHR_VAO));
#else
        glBindBuffer(GL_ARRAY_BUFFER, mesh.rendererReserved(kMSHR_VertBuffer));
        // position
        glVertexAttribPointer(0 /* eAttrib_position */, 3, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)0);
        glEnableVertexAttribArray(0); // eAttrib_Position
        
        // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)12);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.rendererReserved(kMSHR_PrimBuffer));
#endif

        glDrawElements(GL_TRIANGLES, mesh.indexCount(), GL_UNSIGNED_SHORT, (void*)0);

        ++meshIt;
    }
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
        // position
        glVertexAttribPointer(0 /* eAttrib_position */, 3, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)0);
        glEnableVertexAttribArray(0); // eAttrib_Position

        // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh.vertStride(), (void*)12);
        glEnableVertexAttribArray(1);
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


// Template decls so we can define message func here in the .cpp
template MessageResult RendererGL::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult RendererGL::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen

