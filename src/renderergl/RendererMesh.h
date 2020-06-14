//------------------------------------------------------------------------------
// RendererMesh.h - OpenGL Mesh renderer
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

#ifndef GAEN_RENDERERGL_RENDERERMESH_H
#define GAEN_RENDERERGL_RENDERERMESH_H

#include "core/List.h"
#include "core/HashMap.h"

#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"

#include "engine/Message.h"
#include "engine/MessageAccessor.h"
#include "render_support/render_objects.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/ModelGL.h"
#include "renderergl/SpriteGL.h"
#include "renderergl/Stage.h"
#include "renderergl/StageMgr.h"
#include "renderergl/ShaderRegistry.h"

namespace gaen
{
class Gimg;

class RendererMesh
{
public:
    enum MeshReservedIndex
    {
        kMSHR_VAO = 0,
        kMSHR_VertBuffer = 1,
        kMSHR_PrimBuffer = 2
    };

    RendererMesh();

    void init(void * pRenderDevice,
              u32 screenWidth,
              u32 screenHeight);

    void fin();

    void initRenderDevice();
    void initViewport();

    u32 screenWidth() { return mScreenWidth; }
    u32 screenHeight() { return mScreenHeight; }

    void render();
    void endFrame();

    template <typename T>
    MessageResult message(const T& msgAcc);

    u32 loadTexture(const Gimg * pGimg);
    void setTexture(u32 nameHash, u32 glId);
    void unloadTexture(const Gimg* pGimg);

    bool loadVerts(u32 * pVertArrayId, u32 * pVertBufferId, const void * pVerts, u64 vertsSize);
    void unloadVerts(const void * pVerts);

    bool loadPrims(u32 * pPrimBufferId, const void * pPrims, u64 trisSize);
    void unloadPrims(const void * pPrims);

    void unbindBuffers();

    void setActiveShader(u32 nameHash);
    shaders::Shader & activeShader() { return *mpActiveShader; }

private:
    static void set_shader_vec4_var(u32 nameHash, const vec4 & val, void * pContext);
    static void set_texture(u32 nameHash, u32 glId, void * pContext);

    static u32 load_texture(const Gimg * pGimg, void * pContext);

    shaders::Shader * getShader(u32 nameHash);

    bool mIsInit = false;

    void * mpRenderDevice = nullptr;
    u32 mScreenWidth = 0;
    u32 mScreenHeight = 0;

    // GPU capabilities
    GLint mMaxCombinedTextureImageUnits = 0;
    GLint mMaxTextureImageUnits = 0;
    GLint mMaxTextureSize = 0;

    StageMgr<Stage<ModelGL>> mModelStages;
    StageMgr<Stage<SpriteGL>> mSpriteStages;

    shaders::Shader * mpActiveShader = nullptr;

    ShaderRegistry mShaderRegistry;
    HashMap<kMEM_Renderer, u32, shaders::Shader*> mShaders;

    struct LoadInfo
    {
        u32 glId0;
        u32 glId1;
        u32 refCount;

        LoadInfo(u32 glId0, u32 refCount)
          : glId0(glId0)
          , glId1(0)
          , refCount(refCount)
        {}

        LoadInfo(u32 glId0, u32 glId1, u32 refCount)
          : glId0(glId0)
          , glId1(glId1)
          , refCount(refCount)
        {}
    };

    template <class T>
    using LoadMap = HashMap<kMEM_Renderer, T, LoadInfo>;

    LoadMap<u32> mLoadedTextures;
    LoadMap<const void*> mLoadedVerts;
    LoadMap<const void*> mLoadedPrims;

    // tracking structure for images not associated with a gmdl
    HashMap<kMEM_Engine, task_id, List<kMEM_Engine, const Asset*>> mImageOwners;

};

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_RENDERERMESH_H


