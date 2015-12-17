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

#ifndef GAEN_ENGINE_VOXELS_H
#define GAEN_ENGINE_VOXELS_H

#include "core/base_defines.h"
#include "core/mem.h"
#include "core/Vector.h"
#include "engine/math.h"
#include "engine/collision.h"
#include "engine/Color.h"

namespace gaen
{

#pragma pack(push, 1)

static_assert(std::numeric_limits<float>::is_iec559, "Must have IEEE 754 floats for AABB collision code");

static const u32 kWorldMaterialCount = 256;
static const u32 kRootMaterialCount = 8;

struct Pix_RGB8
{
    u8 r;
    u8 g;
    u8 b;

    Pix_RGB8(u8 r, u8 g, u8 b)
        : r(r)
        , g(g)
        , b(b)
    {}
};

struct Pix_RGBA8
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    Pix_RGBA8(u8 r, u8 g, u8 b, u8 a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}
};

struct Pix_R32F
{
    f32 r;

    Pix_R32F(f32 r)
        : r(r)
    {}
};

struct Pix_RG32F
{
    f32 r;
    f32 g;

    Pix_RG32F(f32 r, f32 g)
        : r(r)
        , g(g)
    {}
};

struct Pix_RGB32F
{
    f32 r;
    f32 g;
    f32 b;

    Pix_RGB32F(f32 r, f32 g, f32 b)
        : r(r)
        , g(g)
        , b(b)
    {}
};

struct Pix_RGBA32F
{
    f32 r;
    f32 g;
    f32 b;
    f32 a;

    Pix_RGBA32F(f32 r, f32 g, f32 b, f32 a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}
};


struct Pix_R32I
{
    i32 r;

    Pix_R32I(i32 r)
        : r(r)
    {}
};

struct Pix_RG32I
{
    i32 r;
    i32 g;

    Pix_RG32I(i32 r, i32 g)
        : r(r)
        , g(g)
    {}
};

struct Pix_RGB32I
{
    i32 r;
    i32 g;
    i32 b;

    Pix_RGB32I(i32 r, i32 g, i32 b)
        : r(r)
        , g(g)
        , b(b)
    {}
};

struct Pix_RGBA32I
{
    i32 r;
    i32 g;
    i32 b;
    i32 a;

    Pix_RGBA32I(i32 r, i32 g, i32 b, i32 a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}
};


struct Pix_R32U
{
    u32 r;

    Pix_R32U(u32 r)
        : r(r)
    {}
};

struct Pix_RG32U
{
    u32 r;
    u32 g;

    Pix_RG32U(u32 r, u32 g)
        : r(r)
        , g(g)
    {}
};

struct Pix_RGB32U
{
    u32 r;
    u32 g;
    u32 b;

    Pix_RGB32U(u32 r, u32 g, u32 b)
        : r(r)
        , g(g)
        , b(b)
    {}
};

struct Pix_RGBA32U
{
    u32 r;
    u32 g;
    u32 b;
    u32 a;

    Pix_RGBA32U(u32 r, u32 g, u32 b, u32 a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}
};

class ImageBuffer
{
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

    void imageStoreRGB8(u16 x, u16 y, const Pix_RGB8    & color);
    void imageStoreRGBA8(u16 x, u16 y, const Pix_RGBA8   & color);
    void imageStoreR32F(u16 x, u16 y, const Pix_R32F    & color);
    void imageStoreRG32F(u16 x, u16 y, const Pix_RG32F   & color);
    void imageStoreRGB32F(u16 x, u16 y, const Pix_RGB32F  & color);
    void imageStoreRGBA32F(u16 x, u16 y, const Pix_RGBA32F & color);
    void imageStoreR32I(u16 x, u16 y, const Pix_R32I    & color);
    void imageStoreRG32I(u16 x, u16 y, const Pix_RG32I   & color);
    void imageStoreRGB32I(u16 x, u16 y, const Pix_RGB32I  & color);
    void imageStoreRGBA32I(u16 x, u16 y, const Pix_RGBA32I & color);
    void imageStoreR32U(u16 x, u16 y, const Pix_R32U    & color);
    void imageStoreRG32U(u16 x, u16 y, const Pix_RG32U   & color);
    void imageStoreRGB32U(u16 x, u16 y, const Pix_RGB32U  & color);
    void imageStoreRGBA32U(u16 x, u16 y, const Pix_RGBA32U & color);

    Pix_RGB8    imageLoadRGB8(u16 x, u16 y) const;
    Pix_RGBA8   imageLoadRGBA8(u16 x, u16 y) const;
    Pix_R32F    imageLoadR32F(u16 x, u16 y) const;
    Pix_RG32F   imageLoadRG32F(u16 x, u16 y) const;
    Pix_RGB32F  imageLoadRGB32F(u16 x, u16 y) const;
    Pix_RGBA32F imageLoadRGBA32F(u16 x, u16 y) const;
    Pix_R32I    imageLoadR32I(u16 x, u16 y) const;
    Pix_RG32I   imageLoadRG32I(u16 x, u16 y) const;
    Pix_RGB32I  imageLoadRGB32I(u16 x, u16 y) const;
    Pix_RGBA32I imageLoadRGBA32I(u16 x, u16 y) const;
    Pix_R32U    imageLoadR32U(u16 x, u16 y) const;
    Pix_RG32U   imageLoadRG32U(u16 x, u16 y) const;
    Pix_RGB32U  imageLoadRGB32U(u16 x, u16 y) const;
    Pix_RGBA32U imageLoadRGBA32U(u16 x, u16 y) const;

    u8 * buffer() { return mPixels; }
    const u8 * buffer() const { return mPixels; }

    void copy(const ImageBuffer & sourceBuffer);

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

    u16 mSize;
    u8 mPixelSize;

    u8 * mPixels;
};


struct VoxelMaterial
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

enum VoxelRefFlags
{
    kVRF_IsTerminalEmpty  = 0x1,
    kVRF_IsTerminalFull   = 0x2
};

struct VoxelRef
{
    u64 flags:4;     // Bit flags for this voxel
    u64 material:16; // Index into VoxelWorld material array
    u64 imageIdx:22; // Index into images of VoxelWorld, not all can be loaded at ance
    u64 voxelIdx:22; // Index into image, converted to x/y coords during retrieval from image. 22 bits supports 4096x4096 images of 64 byte voxels

    bool isTerminalEmpty() const { return (flags & kVRF_IsTerminalEmpty) == kVRF_IsTerminalEmpty; }
    bool isTerminalFull() const { return (flags & kVRF_IsTerminalFull) == kVRF_IsTerminalFull; }

    VoxelRef()
    {
        u64 * pThis = reinterpret_cast<u64*>(this);
        *pThis = 0;
    }

    VoxelRef(u16 material, u32 imageIdx, u32 voxelIdx)
      : flags(0)
      , material{material}
      , imageIdx{imageIdx}
      , voxelIdx{voxelIdx}
    {}

    static VoxelRef terminal_empty()
    {
        VoxelRef ret;
        ret.flags = kVRF_IsTerminalEmpty;
        return ret;
    }

    static VoxelRef terminal_full(u16 material)
    {
        VoxelRef ret;
        ret.flags = kVRF_IsTerminalFull;
        ret.material = material;
        return ret;
    }
};
static_assert(sizeof(VoxelRef) == 8, "VoxelRef not 8 bytes");

struct VoxelRoot
{
    Vec3 pos;           // position    12 bytes - 12
    Mat3 rot;           // rotation    36 bytes - 48
    f32 rad;            // radius       4 bytes - 52
    VoxelRef children;  // children     8 bytes - 60
    u32 _PADDING;
};
static_assert(sizeof(VoxelRoot) == 64, "VoxelRoot not 64 bytes");

enum class SubVoxel : u8
{
    // in binary, xyz, x is most sig bit
    
    LeftBottomBack   = 0,  // 000
    LeftBottomFront  = 1,  // 001
    LeftTopBack      = 2,  // 010
    LeftTopFront     = 3,  // 011

    RightBottomBack  = 4,  // 100
    RightBottomFront = 5,  // 101
    RightTopBack     = 6,  // 110
    RightTopFront    = 7   // 111
};

enum class VoxelFace : u8
{
    None   = 0,
    Left   = 1,
    Right  = 2,
    Bottom = 3,
    Top    = 4,
    Back   = 5,
    Front  = 6
};



struct Voxel
{
    VoxelRef children[8]; // children     4 bytes each
    VoxelRef & operator[] (SubVoxel idx)
    {
        return children[(u32)idx];
    }
    const VoxelRef & operator[] (SubVoxel idx) const
    {
        return children[(u32)idx];
    }
};
static_assert(sizeof(Voxel) == 64, "Voxel not 64 bytes");

class VoxelWorld
{
public:
    static const u32 kImageCount = 16; // max is 16 since 4 bits in VoxelRef for imageIdx
    static const u32 kImageSize = 4096;
    static const u32 kPixelSize = 8; // RGBAU32
    static const u32 kPixelsPerVoxel = sizeof(Voxel) / kPixelSize;
    static const u32 kVoxelRefsPerPixel = kPixelSize / sizeof(VoxelRef);
    static const u32 kMaxVoxelIndex = 4194304; // 2 ** 22, we have 22 bits for index, which fits perfectlyinto 4096x4096x16 image

    static_assert(sizeof(Voxel) % kPixelSize == 0, "sizeof(Voxel) not a multiple of kPixelSize");
    static_assert(kPixelSize % sizeof(VoxelRef) == 0, "kPixelSize not a multiple of sizeof(VoxelRef)");
    static_assert(kVoxelRefsPerPixel == 1, "kVoxelRefsPerPixel is not 1");

    VoxelWorld();

    u8 * imageBuffer(u32 imageIdx)
    {
        ASSERT(imageIdx < mImages.size());
        return mImages[imageIdx].buffer();
    }

    u64 imageBufferSize(u32 imageIdx)
    {
        ASSERT(imageIdx < mImages.size());
        return mImages[imageIdx].bufferSize();
    }

    void imageCoords(u16 * pX, u16 * pY, u32 voxelIdx, SubVoxel subVoxel) const
    {
        u32 pixOffset = voxelIdx * kPixelsPerVoxel;

        pixOffset += (u32)subVoxel;

        *pY = pixOffset / kImageSize;
        *pX = pixOffset % kImageSize;
    }

    VoxelRef voxelRef(u32 imageIdx, u32 voxelIdx, SubVoxel subVoxel) const
    {
        ASSERT(imageIdx < mImages.size());
        ASSERT(voxelIdx < kMaxVoxelIndex);

        u16 x, y;
        imageCoords(&x, &y, voxelIdx, subVoxel);

        Pix_RG32U pix = mImages[imageIdx].imageLoadRG32U(x, y);

        const VoxelRef* ret = reinterpret_cast<const VoxelRef*>(&pix);

        return *ret;
    }

    void setVoxelRef(u32 imageIdx, u32 voxelIdx, SubVoxel subVoxel, const VoxelRef & voxel)
    {
        ASSERT(imageIdx < mImages.size());
        ASSERT(voxelIdx < kMaxVoxelIndex);

        u16 x, y;
        imageCoords(&x, &y, voxelIdx, subVoxel);

        Pix_RG32U pix = *reinterpret_cast<const Pix_RG32U*>(&voxel);
        mImages[imageIdx].imageStoreRG32U(x, y, pix);
    }



private:
    VoxelMaterial mMatPalette[kWorldMaterialCount];

    Vector<kMEM_Voxel, ImageBuffer> mImages;
};

class SphereHitTest
{
public:
    SphereHitTest(f32 radius)
      : mRadius(radius) {}

    bool operator()(const AABB_MinMax & aabb) const
    {
        f32 aabbLen = aabb.center().length();
        return aabbLen < mRadius || abs(aabbLen - mRadius) <= (aabb.max.x() - aabb.min.x()) / 1.5f;
    }
private:
    f32 mRadius;
};

template <class HitTestType>
static VoxelRef set_shape_recursive(VoxelWorld & voxelWorld, u32 imageIdx, u32 & voxelIdx, u32 depth, const AABB_MinMax & aabb, HitTestType hitTest)
{
    if (depth == 0)
    {
        if (hitTest(aabb))
            return VoxelRef::terminal_full(1);
        else
            return VoxelRef::terminal_empty();
    }

    AABB_MinMax aabbLbb = voxel_subspace(aabb, SubVoxel::LeftBottomBack);
    AABB_MinMax aabbLbf = voxel_subspace(aabb, SubVoxel::LeftBottomFront);
    AABB_MinMax aabbLtb = voxel_subspace(aabb, SubVoxel::LeftTopBack);
    AABB_MinMax aabbLtf = voxel_subspace(aabb, SubVoxel::LeftTopFront);
    AABB_MinMax aabbRbb = voxel_subspace(aabb, SubVoxel::RightBottomBack);
    AABB_MinMax aabbRbf = voxel_subspace(aabb, SubVoxel::RightBottomFront);
    AABB_MinMax aabbRtb = voxel_subspace(aabb, SubVoxel::RightTopBack);
    AABB_MinMax aabbRtf = voxel_subspace(aabb, SubVoxel::RightTopFront);

    bool lbbIn = hitTest(aabbLbb);
    bool lbfIn = hitTest(aabbLbf);
    bool ltbIn = hitTest(aabbLtb);
    bool ltfIn = hitTest(aabbLtf);
    bool rbbIn = hitTest(aabbRbb);
    bool rbfIn = hitTest(aabbRbf);
    bool rtbIn = hitTest(aabbRtb);
    bool rtfIn = hitTest(aabbRtf);

    VoxelRef lbb = lbbIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbLbb, hitTest) : VoxelRef::terminal_empty();
    VoxelRef lbf = lbfIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbLbf, hitTest) : VoxelRef::terminal_empty();
    VoxelRef ltb = ltbIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbLtb, hitTest) : VoxelRef::terminal_empty();
    VoxelRef ltf = ltfIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbLtf, hitTest) : VoxelRef::terminal_empty();
    VoxelRef rbb = rbbIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbRbb, hitTest) : VoxelRef::terminal_empty();
    VoxelRef rbf = rbfIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbRbf, hitTest) : VoxelRef::terminal_empty();
    VoxelRef rtb = rtbIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbRtb, hitTest) : VoxelRef::terminal_empty();
    VoxelRef rtf = rtfIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbRtf, hitTest) : VoxelRef::terminal_empty();


    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::LeftBottomBack,   lbb);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::LeftBottomFront,  lbf);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::LeftTopBack,      ltb);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::LeftTopFront,     ltf);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::RightBottomBack,  rbb);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::RightBottomFront, rbf);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::RightTopBack,     rtb);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::RightTopFront,    rtf);

    VoxelRef ret = VoxelRef(1, imageIdx, voxelIdx);
    voxelIdx++;
    return ret;
}

template <class HitTestType>
static VoxelRoot set_shape_generic(VoxelWorld & voxelWorld, u32 imageIdx, u32 voxelIdx, u32 depth, const Vec3 & pos, f32 rad, const Mat3 & rot, HitTestType hitTest)
{
    VoxelRoot voxelRoot;
    voxelRoot.pos = pos;
    voxelRoot.rad = rad;
    voxelRoot.rot = rot;
    voxelRoot.children = set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth, AABB_MinMax(rad), hitTest);
    return voxelRoot;
}


AABB_MinMax voxel_subspace(const AABB_MinMax & pSpace, SubVoxel subIndex);

bool test_ray_voxel(VoxelRef * pVoxelRef, Vec3 * pNormal, f32 * pZDepth, VoxelFace * pFace, Vec2 * pFaceUv, const VoxelWorld & voxelWorld, const Vec3 & rayPos, const Vec3 & rayDir, const VoxelRoot & root, u32 maxDepth);

#pragma pack(pop)


// inline fucntions of ImageBuffer

inline void ImageBuffer::imageStoreRGB8(u16 x, u16 y, const Pix_RGB8 & color)
{
    ASSERT(mPixelSize == sizeof(Pix_RGB8));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_RGB8>(x, y) = color;
}

inline void ImageBuffer::imageStoreRGBA8(u16 x, u16 y, const Pix_RGBA8 & color)
{
    ASSERT(mPixelSize == sizeof(Pix_RGBA8));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_RGBA8>(x, y) = color;
}

inline void ImageBuffer::imageStoreR32F(u16 x, u16 y, const Pix_R32F & color)
{
    ASSERT(mPixelSize == sizeof(Pix_R32F));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_R32F>(x, y) = color;
}

inline void ImageBuffer::imageStoreRG32F(u16 x, u16 y, const Pix_RG32F & color)
{
    ASSERT(mPixelSize == sizeof(Pix_RG32F));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_RG32F>(x, y) = color;
}

inline void ImageBuffer::imageStoreRGB32F(u16 x, u16 y, const Pix_RGB32F & color)
{
    ASSERT(mPixelSize == sizeof(Pix_RGB32F));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_RGB32F>(x, y) = color;
}

inline void ImageBuffer::imageStoreRGBA32F(u16 x, u16 y, const Pix_RGBA32F & color)
{
    ASSERT(mPixelSize == sizeof(Pix_RGBA32F));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_RGBA32F>(x, y) = color;
}

inline void ImageBuffer::imageStoreR32I(u16 x, u16 y, const Pix_R32I & color)
{
    ASSERT(mPixelSize == sizeof(Pix_R32I));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_R32I>(x, y) = color;
}

inline void ImageBuffer::imageStoreRG32I(u16 x, u16 y, const Pix_RG32I & color)
{
    ASSERT(mPixelSize == sizeof(Pix_RG32I));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_RG32I>(x, y) = color;
}

inline void ImageBuffer::imageStoreRGB32I(u16 x, u16 y, const Pix_RGB32I & color)
{
    ASSERT(mPixelSize == sizeof(Pix_RGB32I));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_RGB32I>(x, y) = color;
}

inline void ImageBuffer::imageStoreRGBA32I(u16 x, u16 y, const Pix_RGBA32I & color)
{
    ASSERT(mPixelSize == sizeof(Pix_RGBA32I));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_RGBA32I>(x, y) = color;
}

inline void ImageBuffer::imageStoreR32U(u16 x, u16 y, const Pix_R32U & color)
{
    ASSERT(mPixelSize == sizeof(Pix_R32U));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_R32U>(x, y) = color;
}

inline void ImageBuffer::imageStoreRG32U(u16 x, u16 y, const Pix_RG32U & color)
{
    ASSERT(mPixelSize == sizeof(Pix_RG32U));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_RG32U>(x, y) = color;
}

inline void ImageBuffer::imageStoreRGB32U(u16 x, u16 y, const Pix_RGB32U & color)
{
    ASSERT(mPixelSize == sizeof(Pix_RGB32U));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_RGB32U>(x, y) = color;
}

inline void ImageBuffer::imageStoreRGBA32U(u16 x, u16 y, const Pix_RGBA32U & color)
{
    ASSERT(mPixelSize == sizeof(Pix_RGBA32U));
    ASSERT(x < mSize && y < mSize);
    *findLoc<Pix_RGBA32U>(x, y) = color;
}


inline Pix_RGB8 ImageBuffer::imageLoadRGB8(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_RGB8));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_RGB8>(x, y);
}

inline Pix_RGBA8 ImageBuffer::imageLoadRGBA8(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_RGBA8));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_RGBA8>(x, y);
}

inline Pix_R32F ImageBuffer::imageLoadR32F(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_R32F));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_R32F>(x, y);
}

inline Pix_RG32F ImageBuffer::imageLoadRG32F(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_RG32F));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_RG32F>(x, y);
}

inline Pix_RGB32F ImageBuffer::imageLoadRGB32F(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_RGB32F));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_RGB32F>(x, y);
}

inline Pix_RGBA32F ImageBuffer::imageLoadRGBA32F(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_RGBA32F));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_RGBA32F>(x, y);
}

inline Pix_R32I ImageBuffer::imageLoadR32I(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_R32I));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_R32I>(x, y);
}

inline Pix_RG32I ImageBuffer::imageLoadRG32I(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_RG32I));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_RG32I>(x, y);
}

inline Pix_RGB32I ImageBuffer::imageLoadRGB32I(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_RGB32I));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_RGB32I>(x, y);
}

inline Pix_RGBA32I ImageBuffer::imageLoadRGBA32I(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_RGBA32I));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_RGBA32I>(x, y);
}

inline Pix_R32U ImageBuffer::imageLoadR32U(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_R32U));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_R32U>(x, y);
}

inline Pix_RG32U ImageBuffer::imageLoadRG32U(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_RG32U));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_RG32U>(x, y);
}

inline Pix_RGB32U ImageBuffer::imageLoadRGB32U(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_RGB32U));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_RGB32U>(x, y);
}

inline Pix_RGBA32U ImageBuffer::imageLoadRGBA32U(u16 x, u16 y) const
{
    ASSERT(mPixelSize == sizeof(Pix_RGBA32U));
    ASSERT(x < mSize && y < mSize);
    return *findLoc<Pix_RGBA32U>(x, y);
}



} // namespace gaen



#endif // #ifndef GAEN_ENGINE_VOXELS_H

