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
#include "engine/math.h"
#include "engine/RaycastCamera.h"
#include "engine/voxels.h"

namespace gaen
{


struct Pix_RGB8
{
    u8 r;
    u8 g;
    u8 b;
};

struct Pix_RGBA8
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

struct Pix_R32F
{
    f32 r;
};

struct Pix_RG32F
{
    f32 r;
    f32 g;
};

struct Pix_RGB32F
{
    f32 r;
    f32 g;
    f32 b;
};

struct Pix_RGBA32F
{
    f32 r;
    f32 g;
    f32 b;
    f32 a;
};


struct Pix_R32I
{
    i32 r;
};

struct Pix_RG32I
{
    i32 r;
    i32 g;
};

struct Pix_RGB32I
{
    i32 r;
    i32 g;
    i32 b;
};

struct Pix_RGBA32I
{
    i32 r;
    i32 g;
    i32 b;
    i32 a;
};


struct Pix_R32U
{
    u32 r;
};

struct Pix_RG32U
{
    u32 r;
    u32 g;
};

struct Pix_RGB32U
{
    u32 r;
    u32 g;
    u32 b;
};

struct Pix_RGBA32U
{
    u32 r;
    u32 g;
    u32 b;
    u32 a;
};

class ImageBuffer
{
public:
    ImageBuffer(u16 size, u8 pixelSize);
    ~ImageBuffer();

    u16 size() const { return mSize; }
    u8 pixelSize() const { return mPixelSize; }
    
    void imageStore(u16 x, u16 y, Vec4 color);
    //Vec4 imageLoad(

    u8 * buffer() { return mPixels; }
/*    T * bufferTyped() { return mPixels; }

    const u8 * buffer() const { return reinterpret_cast<const u8*>(mPixels); }
    const T * bufferTyped() const { return mPixels; }
    */
private:
    u16 mSize;
    u8 mPixelSize;

    u8 * mPixels;
};

class ShaderSimulator
{
public:
    enum ImageIndex
    {
        kIMID_Objects = 0

//        kIMID_
    };
    
    static const u32 kImageCount = 16; // max is 16 since 4 bits in VoxelRef for imageIdx

    ShaderSimulator();
    ~ShaderSimulator();

    void init(u32 outputImageSize);
    const u8 * frameBuffer() { return mFrameBuffer->buffer(); }

    void render(const RaycastCamera & camera);
private:
    void fragShader_Blue();
    void fragShader_Raycast();

    bool mIsInit = false;
    ImageBuffer * mFrameBuffer;
    ImageBuffer * mImages[kImageCount];

    // GLSL "globals"
    struct ScreenCoords
    {
        u32 x;
        u32 y;
        u32 z;
    };

    Pix_RGB8 color; // frag output shader
    ScreenCoords gl_FragCoord;

    u32 uniform0 = 0;

    // camera stuff
    Mat4 projectionInv;
    Vec3 cameraPos;
    Vec2 windowSize;
    f32 nearZ;
    f32 farZ;

    // LORRTODO - temp voxel stuff - should move to game engine proper
    VoxelRoot voxelRoot;

};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_VOXELS_H

