//------------------------------------------------------------------------------
// RendererGL.h - OpenGL Renderer
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

#ifndef GAEN_RENDERERGL_RENDERERGL_H
#define GAEN_RENDERERGL_RENDERERGL_H

#include "core/List.h"
#include "core/HashMap.h"
#include "engine/math.h"
#include "engine/Message.h"
#include "engine/MessageAccessor.h"
#include "engine/ModelMgr.h"
#include "engine/renderer_structs.h"

// LORRTODO: probably a temp include only until we get voxel stuff more defined
#include "engine/voxels_proto.h"

#include "engine/RaycastCamera.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/ShaderRegistry.h"

namespace gaen
{
class RendererGL
{
public:
    enum MeshReservedIndex
    {
        kMSHR_VAO = 0,
        kMSHR_VertBuffer = 1,
        kMSHR_PrimBuffer = 2
    };

    void init(device_context deviceContext,
              render_context renderContext,
              u32 screenWidth,
              u32 screenHeight);

    void fin();

    void initRenderDevice();
    void initViewport();

    void render();
    void endFrame();

    template <typename T>
    MessageResult message(const T& msgAcc);

    void loadMaterialMesh(Model::MaterialMesh & matMesh);

private:
    void setActiveShader(u32 nameHash);
    shaders::Shader * getShader(u32 nameHash);

    bool mIsInit = false;
    
    device_context mDeviceContext = 0;
    render_context mRenderContext = 0;
    u32 mScreenWidth = 0;
    u32 mScreenHeight = 0;

    // GPU capabilities
    GLint mMaxCombinedTextureImageUnits = 0;
    GLint mMaxTextureImageUnits = 0;
    GLint mMaxTextureSize = 0;

    // GPU custom renderer presentation support
    // I.E. a surface to render our GPU generated texture upon for display.
    shaders::Shader * mpVoxelCast;
    GLuint mPresentVAO;
    GLuint mPresentVertexBuffer;
    GLuint mPresentImage;
    shaders::Shader * mpPresentShader;

    GLuint mVoxelDataImage;
    GLuint mVoxelData;

    Mat4 mProjection;
    Mat4 mGuiProjection;

    ModelMgr<RendererGL> * mpModelMgr;

    List<kMEM_Renderer, DirectionalLight> mDirectionalLights;
    List<kMEM_Renderer, PointLight> mPointLights;

    shaders::Shader * mpActiveShader = nullptr;

    ShaderRegistry mShaderRegistry;
    HashMap<kMEM_Renderer, u32, shaders::Shader*> mShaders;


    // LORRTODO: temp voxel experiment stuff
    ShaderSimulator mShaderSim;

    RaycastCamera mRaycastCamera;

    VoxelWorld mVoxelWorld;
    VoxelRoot mVoxelRoot;
};


} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_RENDERERGL_H


