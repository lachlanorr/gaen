//------------------------------------------------------------------------------
// voxels.cpp - CPU side voxel support
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

#include "engine/stdafx.h"

#include "engine/voxels_proto.h"

// Voxel layout
//
//
//

namespace gaen
{

ImageBuffer::ImageBuffer(u16 size, u8 pixelSize)
{
    mSize = size;
    mPixels = (u8*)GALLOC(kMEM_Engine, mSize * mSize * pixelSize);
}
ImageBuffer::~ImageBuffer()
{
    GFREE(mPixels);
}


ShaderSimulator::ShaderSimulator()
{
    mIsInit = false;
    mFrameBuffer = nullptr;
    for (u32 i = 0; i < kImageCount; ++i)
    {
        mImages[i] = nullptr;
    }
}

ShaderSimulator::~ShaderSimulator()
{
    if (mIsInit)
    {
        if (mFrameBuffer)
            GDELETE(mFrameBuffer);

        for (u32 i = 0; i < kImageCount; ++i)
        {
            if (mImages[i])
                GDELETE(mImages[i]);
        }
    }
}

void ShaderSimulator::init(u32 outputImageSize)
{
    mFrameBuffer = GNEW(kMEM_Engine, ImageBuffer, outputImageSize, sizeof(Pix_RGB8));

    // prep cameral
    cameraPos = Vec3(0.0f, 0.0f, 10.0f);
    windowSize = Vec2((f32)outputImageSize, (f32)outputImageSize);
    nearZ = 5.0f;
    farZ = 10000.0f;
}

void ShaderSimulator::render()
{
    Pix_RGB8 * pix = reinterpret_cast<Pix_RGB8*>(mFrameBuffer->buffer());

    gl_FragCoord.z = 0;
    for (gl_FragCoord.y = 0; gl_FragCoord.y < mFrameBuffer->size(); ++gl_FragCoord.y)
    {
        for (gl_FragCoord.x = 0; gl_FragCoord.x < mFrameBuffer->size(); ++gl_FragCoord.x)
        {
            color.r = 0;
            color.g = 0;
            color.b = 0;

            //fragShader_Blue();
            fragShader_Raycast();

            *pix = color;
            pix++;
        }
    }

    uniform0++;
}

void ShaderSimulator::fragShader_Blue()
{
    static u8 g = 0;
    color.r = 255;
    color.g = (u8)uniform0;
    color.b = 255;
}

void ShaderSimulator::fragShader_Raycast()
{
    Vec3 rayDir;
    rayDir.x() = 2.0f * gl_FragCoord.x / windowSize.x() - 1.0f;
    rayDir.y() = 2.0f * gl_FragCoord.y / windowSize.y() - 1.0f;
    rayDir.z() = -farZ;

    color.r = (u8)(abs(rayDir.x()) * 255.0f);
    color.g = (u8)(abs(rayDir.y()) * 255.0f);

//    color.r = (u8)((rayDir.x() + 1.0f) * 127.5f);
//    color.g = (u8)((rayDir.y() + 1.0f) * 127.5f);

//    color.r = (u8)(gl_FragCoord.x / windowSize.x() * 255.0f);
//    color.g = (u8)(gl_FragCoord.y / windowSize.y() * 255.0f);

    color.b = 0;
}

} // namespace gaen
