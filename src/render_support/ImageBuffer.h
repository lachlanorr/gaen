//------------------------------------------------------------------------------
// ImageBuffer.h - OpenGL compatible image buffers
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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

#ifndef GAEN_RENDER_SUPPORT_IMAGE_BUFFER_H
#define GAEN_RENDER_SUPPORT_IMAGE_BUFFER_H

#include "core/base_defines.h"

namespace gaen
{
#pragma pack(push, 1)

struct RGB8
{
    u8 r;
    u8 g;
    u8 b;

    RGB8(u8 r, u8 g, u8 b)
        : r(r)
        , g(g)
        , b(b)
    {}

    RGB8(f32 r, f32 g, f32 b)
        : r((u8)(r * 255.0))
        , g((u8)(g * 255.0))
        , b((u8)(b * 255.0))
    {}
};

struct RGBA8
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    RGBA8(u8 r, u8 g, u8 b, u8 a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}
};

struct R32F
{
    f32 r;

    R32F(f32 r)
        : r(r)
    {}
};

struct RG32F
{
    f32 r;
    f32 g;

    RG32F(f32 r, f32 g)
        : r(r)
        , g(g)
    {}
};

struct RGB32F
{
    f32 r;
    f32 g;
    f32 b;

    RGB32F(f32 r, f32 g, f32 b)
        : r(r)
        , g(g)
        , b(b)
    {}
};

struct RGBA32F
{
    f32 r;
    f32 g;
    f32 b;
    f32 a;

    RGBA32F(f32 r, f32 g, f32 b, f32 a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}
};


struct R32I
{
    i32 r;

    R32I(i32 r)
        : r(r)
    {}
};

struct RG32I
{
    i32 r;
    i32 g;

    RG32I(i32 r, i32 g)
        : r(r)
        , g(g)
    {}
};

struct RGB32I
{
    i32 r;
    i32 g;
    i32 b;

    RGB32I(i32 r, i32 g, i32 b)
        : r(r)
        , g(g)
        , b(b)
    {}
};

struct RGBA32I
{
    i32 r;
    i32 g;
    i32 b;
    i32 a;

    RGBA32I(i32 r, i32 g, i32 b, i32 a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}
};


struct R32U
{
    u32 r;

    R32U(u32 r)
        : r(r)
    {}
};

struct RG32U
{
    u32 r;
    u32 g;

    RG32U(u32 r, u32 g)
        : r(r)
        , g(g)
    {}
};

struct RGB32U
{
    u32 r;
    u32 g;
    u32 b;

    RGB32U(u32 r, u32 g, u32 b)
        : r(r)
        , g(g)
        , b(b)
    {}
};

struct RGBA32U
{
    u32 r;
    u32 g;
    u32 b;
    u32 a;

    RGBA32U(u32 r, u32 g, u32 b, u32 a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}
};

class ImageBuffer
{
private:
    u8 * findAddr(u16 x, u16 y)
    {
        return mPixels + (mSize * y * mPixelSize + x * mPixelSize);
    }

    const u8 * findAddr(u16 x, u16 y) const
    {
        return mPixels + (mSize * y * mPixelSize + x * mPixelSize);
    }

    template <typename T>
    T * findLoc(u16 x, u16 y)
    {
        return reinterpret_cast<T*>(findAddr(x, y));
    }

    template <typename T>
    const T * findLoc(u16 x, u16 y) const
    {
        return reinterpret_cast<const T*>(findAddr(x, y));
    }


public:
    ImageBuffer(u16 size, size_t pixelSize);

    template <typename T>
    ImageBuffer(u16 size)
        : ImageBuffer{size, sizeof(T)}
    {
    }

    ~ImageBuffer();

    u16 size() const { return mSize; }
    u8 pixelSize() const { return mPixelSize; }
    u64 bufferSize() const { return mSize * mSize * mPixelSize; }

    template<typename T>
    void imageStore2d(u16 x, u16 y, const T & color)
    {
        ASSERT(mPixelSize == sizeof(T));
        ASSERT(x < mSize && y < mSize);
        *findLoc<T>(x, y) = color;
    }

    template<typename T>
    void imageStore(u32 i, const T & color)
    {
        ASSERT(mPixelSize == sizeof(T));
        ASSERT(i < (u32)(mSize * mSize));
        T * p = reinterpret_cast<T*>(mPixels) + i;
        *p = color;
    }

    template<typename T>
    T imageLoad2d(u16 x, u16 y) const
    {
        ASSERT(mPixelSize == sizeof(T));
        ASSERT(x < mSize && y < mSize);
        return *findLoc<T>(x, y);
    }

    template<typename T>
    T imageLoad(u32 i) const
    {
        ASSERT(mPixelSize == sizeof(T));
        ASSERT(i < (u32)(mSize * mSize));
        return *(reinterpret_cast<const T*>(mPixels) + i);
    }

    u8 * buffer() { return mPixels; }
    const u8 * buffer() const { return mPixels; }

    template <typename T>
    T * buffer() { ASSERT(sizeof(T) == mPixelSize); return reinterpret_cast<T*>(mPixels); }

    template <typename T>
    const T * buffer() const { ASSERT(sizeof(T) == mPixelSize); return reinterpret_cast<const T*>(mPixels); }

    void copy(const ImageBuffer & sourceBuffer);

private:
    u16 mSize;
    u8 mPixelSize;

    u8 * mPixels;
};

#pragma pack(pop)

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_IMAGE_BUFFER_H
