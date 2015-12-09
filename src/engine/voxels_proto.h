//------------------------------------------------------------------------------
// voxels.h - CPU side voxel support
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

#ifndef GAEN_ENGINE_VOXELS_PROTO_H
#define GAEN_ENGINE_VOXELS_PROTO_H

#include "core/base_defines.h"
#include "core/mem.h"
#include "core/List.h"
#include "engine/math.h"
#include "engine/RaycastCamera.h"
#include "engine/renderer_structs.h"
#include "engine/voxels.h"

namespace gaen
{



class ShaderSimulator
{
public:
    enum ImageIndex
    {
        kIMID_Objects = 0

//        kIMID_
    };
    
    ShaderSimulator();
    ~ShaderSimulator();

    void init(u32 outputImageSize, RaycastCamera * pRaycastCamera);
    const u8 * frameBuffer() { return mFrameBuffer->buffer(); }

    void render(const RaycastCamera & camera, const List<kMEM_Renderer, DirectionalLight> & lights);
private:
    void fragShader_Blue();
    void fragShader_Raycast();

    bool mIsInit = false;
    ImageBuffer * mFrameBuffer;
    ImageBuffer * mDepthBuffer;
    ImageBuffer * mDepthBufferBlank;

    RaycastCamera * mpRaycastCamera;

    // GLSL "globals"
    struct ScreenCoords
    {
        u32 x;
        u32 y;
        u32 z;
    };

    Pix_RGB8 color; // frag output shader
    f32 zDepth;
    ScreenCoords gl_FragCoord;

    u32 uniform0 = 0;

    // camera stuff
    Mat4 projectionInv;
    Vec3 cameraPos;
    Vec3 lightDir;
    Vec4 lightColor;
    Vec2 windowSize;
    f32 nearZ;
    f32 farZ;

    // LORRTODO - temp voxel stuff - should move to game engine proper
    VoxelRoot voxelRoot;

    VoxelWorld voxelWorld;

};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_VOXELS_H

