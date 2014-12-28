//------------------------------------------------------------------------------
// RendererGL.h - OpenGL Renderer
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

#ifndef GAEN_RENDERERGL_RENDERERGL_H
#define GAEN_RENDERERGL_RENDERERGL_H

#include "core/Vector.h"
#include "core/List.h"
#include "engine/math.h"
#include "engine/Message.h"
#include "engine/MessageAccessor.h"
#include "engine/ModelMgr.h"
#include "engine/renderer_structs.h"

#include "renderergl/gaen_opengl.h"

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
    bool mIsInit = false;
    
    device_context mDeviceContext = 0;
    render_context mRenderContext = 0;
    u32 mScreenWidth = 0;
    u32 mScreenHeight = 0;

    Mat4 mProjection;
    Mat4 mGuiProjection;

    ModelMgr<RendererGL> * mpModelMgr;

    List<kMEM_Renderer, DirectionalLight> mDirectionalLights;
    List<kMEM_Renderer, PointLight> mPointLights;
};


} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_RENDERERGL_H


