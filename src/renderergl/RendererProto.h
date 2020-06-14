//------------------------------------------------------------------------------
// RendererProto.h - Prototype and experimental rendering code
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

#ifndef GAEN_RENDERERGL_RENDERERPROTO_H
#define GAEN_RENDERERGL_RENDERERPROTO_H

#include "core/List.h"
#include "core/HashMap.h"

#include "math/mat4.h"

#include "engine/Message.h"
#include "engine/MessageAccessor.h"
#include "render_support/render_objects.h"

// LORRTODO: probably a temp include only until we get voxel stuff more defined
#include "render_support/voxel_proto.h"

#include "render_support/RaycastCamera.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/ShaderRegistry.h"

#define RENDERTYPE_MESH 0
#define RENDERTYPE_CPUFRAGVOXEL 1
#define RENDERTYPE_CPUCOMPVOXEL 2
#define RENDERTYPE_GPUFRAGVOXEL 3
#define RENDERTYPE_GPUCOMPVOXEL 4
#define RENDERTYPE_VOXEL27      5

//#define RENDERTYPE RENDERTYPE_MESH
#define RENDERTYPE RENDERTYPE_CPUFRAGVOXEL
//#define RENDERTYPE RENDERTYPE_CPUCOMPVOXEL
//#define RENDERTYPE RENDERTYPE_GPUFRAGVOXEL
//#define RENDERTYPE RENDERTYPE_GPUCOMPVOXEL
//#define RENDERTYPE RENDERTYPE_VOXEL27

namespace gaen
{
class RendererProto
{
public:
    enum GmdlReservedIndex
    {
        kMSHR_VAO = 0,
        kMSHR_VertBuffer = 1,
        kMSHR_PrimBuffer = 2
    };

    void init(void * pRenderDevice,
              u32 screenWidth,
              u32 screenHeight);

    void fin();

    void initRenderDevice();
    void initViewport();

    void render();
    void endFrame();

    template <typename T>
    MessageResult message(const T& msgAcc);

private:
    void setActiveShader(u32 nameHash);
    shaders::Shader * getShader(u32 nameHash);

    bool mIsInit = false;
    
    void * mpRenderDevice = nullptr;
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
    GLuint mPresentImageLocation;
    shaders::Shader * mpPresentShader;

    GLuint mVoxelDataImage;
    GLuint mVoxelDataImageLocation;
    GLuint mVoxelData;

    GLuint mVoxelRootsImage;
    GLuint mVoxelRootsImageLocation;
    GLuint mVoxelRoots;

    mat4 mProjection;
    mat4 mGuiProjection;

    List<kMEM_Renderer, Light> mLights;

    shaders::Shader * mpActiveShader = nullptr;

    ShaderRegistry mShaderRegistry;
    HashMap<kMEM_Renderer, u32, shaders::Shader*> mShaders;


    // LORRTODO: temp voxel experiment stuff
#if RENDERTYPE == RENDERTYPE_CPUFRAGVOXEL
    FragmentShaderSimulator mShaderSim;
#elif RENDERTYPE == RENDERTYPE_CPUCOMPVOXEL
    ComputeShaderSimulator mShaderSim;
#endif

#if RENDERTYPE == RENDERTYPE_CPUFRAGVOXEL || RENDERTYPE == RENDERTYPE_CPUCOMPVOXEL || RENDERTYPE == RENDERTYPE_GPUFRAGVOXEL || RENDERTYPE == RENDERTYPE_GPUCOMPVOXEL
    RaycastCamera mRaycastCamera;

    VoxelWorld mVoxelWorld;
    VoxelRoot mVoxelRoot;
#endif
};

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_RENDERERPROTO_H


