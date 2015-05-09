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
}

void ShaderSimulator::render()
{
    Pix_RGB8 * pix = reinterpret_cast<Pix_RGB8*>(mFrameBuffer->buffer());

    gl_FragCoord.z = 0;
    for (gl_FragCoord.x = 0; gl_FragCoord.x < mFrameBuffer->size(); ++gl_FragCoord.x)
    {
        for (gl_FragCoord.y = 0; gl_FragCoord.y < mFrameBuffer->size(); ++gl_FragCoord.y)
        {
            color.r = 0;
            color.g = 0;
            color.b = 0;
            fragShader_Blue();

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

} // namespace gaen
