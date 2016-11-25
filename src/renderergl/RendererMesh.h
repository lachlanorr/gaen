//------------------------------------------------------------------------------
// RendererMesh.h - OpenGL Mesh renderer
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

#ifndef GAEN_RENDERERGL_RENDERERMESH_H
#define GAEN_RENDERERGL_RENDERERMESH_H

#include <glm/mat4x4.hpp>

#include "core/List.h"
#include "core/HashMap.h"
#include "engine/Message.h"
#include "engine/MessageAccessor.h"
#include "engine/ModelMgr.h"
#include "engine/renderer_structs.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/SpriteStage.h"
#include "renderergl/ShaderRegistry.h"

namespace gaen
{

class RendererMesh
{
public:
    enum MeshReservedIndex
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

    void loadMaterialGmdl(Model::MaterialGmdl & matGmdl);

    u32 loadTexture(u32 textureUnit, const Gimg * pGimg);
    void unloadTexture(const Gimg * pGimg);

    void loadGlyphVerts(u32 * pVertArrayId, u32 * pVertBufferId, const GlyphVert * pVerts, u64 vertsSize);
    void unloadGlyphVerts(const GlyphVert * pVerts);

    void loadGlyphTris(u32 * pPrimBufferId, const GlyphTri * pTris, u64 trisSize);
    void unloadGlyphTris(const GlyphTri * pTris);

    void unbindBuffers();

private:
    static void set_shader_vec4_var(u32 nameHash, const glm::vec4 & val, void * pContext);
    static u32 texture_unit(u32 nameHash);
    static void set_texture(u32 nameHash, u32 glId, void * pContext);

    static u32 load_texture(u32 nameHash, const Gimg * pGimg, void * pContext);

    static void prepare_gmdl_attributes(const Gmdl & gmdl);


    void setActiveShader(u32 nameHash);
    shaders::Shader * getShader(u32 nameHash);

    void insertModel(task_id owner,
                     u32 uid,
                     Model * pModel,
                     const glm::mat4x3 & worldTransform,
                     bool isAssetManaged);

    void insertSprite(SpriteInstance * pSpriteInst);
    void animateSprite(u32 uid, u32 animHash, u32 animFrameIdx);
    void transformSprite(u32 uid, const glm::mat4x3 & transform);
    void destroySprite(u32 uid);
    
    void showSpriteStage(u32 stageHash);
    void hideSpriteStage(u32 stageHash);
    void destroySpriteStage(u32 stageHash);
    
    bool mIsInit = false;
    
    void * mpRenderDevice = nullptr;
    u32 mScreenWidth = 0;
    u32 mScreenHeight = 0;

    // GPU capabilities
    GLint mMaxCombinedTextureImageUnits = 0;
    GLint mMaxTextureImageUnits = 0;
    GLint mMaxTextureSize = 0;

    GLuint mPresentVAO;
    GLuint mPresentVertexBuffer;
    GLuint mPresentImage;
    GLuint mPresentImageLocation;
    shaders::Shader * mpPresentShader;

    glm::mat4 mProjection;
    glm::mat4 mGuiProjection;

    ModelMgr<RendererMesh> * mpModelMgr;

    typedef HashMap<kMEM_Renderer, u32, SpriteStageUP> SpriteStageMap;
    SpriteStageMap mSpriteStages;
    
    List<kMEM_Renderer, DirectionalLight> mDirectionalLights;
    List<kMEM_Renderer, PointLight> mPointLights;

    shaders::Shader * mpActiveShader = nullptr;

    ShaderRegistry mShaderRegistry;
    HashMap<kMEM_Renderer, u32, shaders::Shader*> mShaders;

    template <class T>
    struct LoadInfo
    {
        const T * pGimg;
        u32 glId0;
        u32 glId1;
        u32 refCount;

        LoadInfo(const T *pGimg, u32 glId0, u32 refCount)
          : pGimg(pGimg)
          , glId0(glId0)
          , glId1(0)
          , refCount(refCount)
        {}

        LoadInfo(const T *pGimg, u32 glId0, u32 glId1, u32 refCount)
          : pGimg(pGimg)
          , glId0(glId0)
          , glId1(glId1)
          , refCount(refCount)
        {}
    };

    template <class T>
    using LoadMap = HashMap<kMEM_Renderer, const T*, LoadInfo<T>>;

    LoadMap<Gimg> mLoadedTextures;
    LoadMap<GlyphVert> mLoadedGlyphVerts;
    LoadMap<GlyphTri> mLoadedGlyphTris;
};

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_RENDERERMESH_H


