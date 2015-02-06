//------------------------------------------------------------------------------
// Mesh.h - Geometry structs
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

#ifndef GAEN_ENGINE_MESH_H
#define GAEN_ENGINE_MESH_H

#include "core/base_defines.h"
#include "core/mem.h"
#include "engine/Color.h"

namespace gaen
{

typedef u16 index;

enum VertType
{
    kVERT_Pos          = GAEN_4CC('V','T','X','A'),
    kVERT_PosNorm      = GAEN_4CC('V','T','X','B'),
    kVERT_PosNormUv    = GAEN_4CC('V','T','X','C'),
    kVERT_PosNormUvTan = GAEN_4CC('V','T','X','D'),

    kVERT_END
};

enum PrimType
{
    kPRIM_Point    = GAEN_4CC('P','R','M','A'),
    kPRIM_Line     = GAEN_4CC('P','R','M','B'),
    kPRIM_Triangle = GAEN_4CC('P','R','M','C'),

    kPRIM_END
};


//------------------------------------------------------------------------------
// Vertex structs
//------------------------------------------------------------------------------
struct VertPos
{
    static const VertType kVertType = kVERT_Pos;
    Vec3 position;
};

struct VertPosNorm
{
    static const VertType kVertType = kVERT_PosNorm;
    Vec3 position;
    Vec3 normal;
};

struct VertPosNormUv
{
    static const VertType kVertType = kVERT_PosNormUv;
    Vec3 position;
    Vec3 normal;
    f32 u;
    f32 v;
};

struct VertPosNormUvTan
{
    static const VertType kVertType = kVERT_PosNormUvTan;
    Vec3 position;
    Vec3 normal;
    f32 u;
    f32 v;
    Vec4 tangent;
};

const u32 kOffsetPosition = 0;
const u32 kOffsetNormal   = kOffsetPosition + sizeof(Vec3);
const u32 kOffsetUV       = kOffsetNormal + sizeof(Vec3);
const u32 kOffsetTangent  = kOffsetUV + sizeof(f32) + sizeof(f32);

inline bool is_valid_vert_type(u32 vertType)
{
    return vertType >= kVERT_PosNorm && vertType < kVERT_END;
}

inline bool is_valid_prim_type(u32 primType)
{
    return primType >= kPRIM_Point && primType < kPRIM_END;
}

inline u8 vert_type_zero_based_id(VertType vertType)
{
    // LORRTODO - Big endian support needed here if we ever port to such a platform
    ASSERT(is_valid_vert_type(vertType));
    static const u32 kSwapped_kVERT_Pos = BYTESWAP32(kVERT_Pos);
    return (u8)(BYTESWAP32(vertType) - kSwapped_kVERT_Pos);
}

inline u8 vert_type_zero_based_id_end()
{
    // LORRTODO - Big endian support needed here if we ever port to such a platform
    static const u32 diff = BYTESWAP32(kVERT_END) - BYTESWAP32(kVERT_Pos);
    return (u8)diff;
}


inline u8 prim_type_zero_based_id(PrimType primType)
{
    // LORRTODO - Big endian support needed here if we ever port to such a platform
    ASSERT(is_valid_prim_type(primType));
    static const u32 kSwapped_kPRIM_Point = BYTESWAP32(kPRIM_Point);
    return (u8)(BYTESWAP32(primType) - kSwapped_kPRIM_Point);
}

inline u8 prim_type_zero_based_id_end()
{
    // LORRTODO - Big endian support needed here if we ever port to such a platform
    static const u32 diff = BYTESWAP32(kPRIM_END) - BYTESWAP32(kPRIM_Point);
    return (u8)diff;
}

struct PrimPoint
{
    PrimPoint(index p0)
      : p0(p0) {}

    // a point is composed of a single index into point array
    index p0;
};

struct PrimLine
{
    PrimLine(index p0, index p1)
      : p0(p0), p1(p1) {}

    // a line is composed of 2 indices into point array
    index p0;
    index p1;
};


struct PrimTriangle
{
    // a triangle is composed of 3 indices into point array
    PrimTriangle(index p0, index p1, index p2)
      : p0(p0), p1(p1), p2(p2) {}

    index p0;
    index p1;
    index p2;
};


inline u32 vert_stride(VertType vertType)
{
    switch(vertType)
    {
    case kVERT_Pos:
        return sizeof(VertPos);
    case kVERT_PosNorm:
        return sizeof(VertPosNorm);
    case kVERT_PosNormUv:
        return sizeof(VertPosNormUv);
    case kVERT_PosNormUvTan:
        return sizeof(VertPosNormUvTan);
    default:
        PANIC("Invalid VertexType: %d", vertType);
        return 0;
    }
}

inline u32 prim_stride(PrimType primType)
{
    switch(primType)
    {
    case kPRIM_Point:
        return sizeof(PrimPoint);
    case kPRIM_Line:
        return sizeof(PrimLine);
    case kPRIM_Triangle:
        return sizeof(PrimTriangle);
    default:
        PANIC("Invalid PrimType: %d", primType);
        return 0;
    }
}

inline u32 index_count(PrimType primType)
{
    switch (primType)
    {
    case kPRIM_Point:
        return 1;
    case kPRIM_Line:
        return 2;
    case kPRIM_Triangle:
        return 3;
    default:
        PANIC("Invalid PrimType: %d", primType);
        return 0;
    }
}

//-------------------------------------------
// Comparison operators for Polygon and Line
//-------------------------------------------
inline bool operator==(const PrimTriangle & lhs, const PrimTriangle & rhs)
{
    return lhs.p0 == rhs.p0 &&
        lhs.p1 == rhs.p1 &&
        lhs.p2 == rhs.p2;
}

inline bool operator<(const PrimTriangle & lhs, const PrimTriangle & rhs)
{
    if(lhs.p0 != rhs.p0)
        return lhs.p0 < rhs.p0;
    else if(lhs.p1 != rhs.p1)
        return lhs.p1 < rhs.p1;
    else if(lhs.p2 != rhs.p2)
        return lhs.p2 < rhs.p2;

    // if we get here, all elements match
    return false;
}

inline bool operator==(const PrimLine & lhs, const PrimLine & rhs)
{
    // do a simple sort of points to consider two lines equal even if
    // their points are in different order
    index line0p0,line0p1,line1p0,line1p1;

    if(lhs.p0 < lhs.p1)
    {
        line0p0 = lhs.p0;
        line0p1 = lhs.p1;
    }
    else
    {
        line0p0 = lhs.p1;
        line0p1 = lhs.p0;
    }

    if(rhs.p0 <= rhs.p1)
    {
        line1p0 = rhs.p0;
        line1p1 = rhs.p1;
    }
    else
    {
        line1p0 = rhs.p1;
        line1p1 = rhs.p0;
    }


    return line0p0 == line1p0 &&
        line0p1 == line1p1;
}

inline bool operator<(const PrimLine & lhs, const PrimLine & rhs)
{
    // do a simple sort of points to consider two lines equal even if
    // their points are in different order
    index line0p0,line0p1,line1p0,line1p1;

    if(lhs.p0 < lhs.p1)
    {
        line0p0 = lhs.p0;
        line0p1 = lhs.p1;
    }
    else
    {
        line0p0 = lhs.p1;
        line0p1 = lhs.p0;
    }

    if(rhs.p0 <= rhs.p1)
    {
        line1p0 = rhs.p0;
        line1p1 = rhs.p1;
    }
    else
    {
        line1p0 = rhs.p1;
        line1p1 = rhs.p0;
    }


    if(line0p0 != line1p0)
        return line0p0 < line1p0;
    else if(line0p1 != line1p1)
        return line0p1 < line1p1;

    // if we get here, all elements match
    return false;
}

class Mesh
{
public:
    static const u32 kRendererReservedCount = 4;

    VertType vertType() const { return mVertType; }
    PrimType primType() const { return mPrimType; }

    f32 * verts()
    {
        return reinterpret_cast<f32*>(reinterpret_cast<u8*>(this) + mVertOffset);
    }

    const f32 * verts() const
    {
        return reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(this) + mVertOffset);
    }

    index * prims()
    {
        return reinterpret_cast<index*>(reinterpret_cast<u8*>(this) + mPrimOffset);
    }

    const index * prims() const
    {
        return reinterpret_cast<const index*>(reinterpret_cast<const u8*>(this) + mPrimOffset);
    }

    u32 vertStride() const
    {
        // if there are morph targets, multiply by that count
        return vert_stride(mVertType) * (1 + mMorphTargetCount);
    }

    u32 vertsSize() const
    {
        return vertStride() * mVertCount;
    }

    u32 primStride() const
    {
        // double the primitive stride if indices are 32 bit instead of 16 bit
        return prim_stride(mPrimType) << mHas32BitIndices; // * 2 if mhas32BitIndices == 1
    }

    u32 primsSize() const
    {
        return primStride() * mPrimCount;
    }

    u32 indexCount() const
    {
        return mPrimCount * index_count(mPrimType);
    }

    u32 totalSize() const
    {
        return sizeof(Mesh) + vertsSize() + primsSize();
    }

    u32& rendererReserved(u32 idx)
    {
        ASSERT(idx < kRendererReservedCount);
        return mRendererReserved[idx];
    }

    //--------------------------------------------------------------------------
    // Cast operators which provide a convenient way to get to vertices
    // and indices without a lot of explicit reinterpret casts.
    // E.g. Rather than:
    //    VertPosNorm * pVerts = reinterpret_cast<VertPosNorm*>(mesh.prims);
    // use instead:
    //    VertPosNorm * pVerts = mesh
    // 
    // The second form will call one of the cast operators below
    //--------------------------------------------------------------------------
    operator VertPos*()
    {
        ASSERT(mVertType == kVERT_Pos);
        return reinterpret_cast<VertPos*>(verts());
    }
    operator const VertPos*() const
    {
        ASSERT(mVertType == kVERT_Pos);
        return reinterpret_cast<const VertPos*>(verts());
    }
    operator VertPosNorm*()
    {
        ASSERT(mVertType == kVERT_PosNorm);
        return reinterpret_cast<VertPosNorm*>(verts());
    }
    operator const VertPosNorm*() const
    {
        ASSERT(mVertType == kVERT_PosNorm);
        return reinterpret_cast<const VertPosNorm*>(verts());
    }
    operator VertPosNormUv*()
    {
        ASSERT(mVertType == kVERT_PosNormUv);
        return reinterpret_cast<VertPosNormUv*>(verts());
    }
    operator const VertPosNormUv*() const
    {
        ASSERT(mVertType == kVERT_PosNormUv);
        return reinterpret_cast<const VertPosNormUv*>(verts());
    }
    operator VertPosNormUvTan*()
    {
        ASSERT(mVertType == kVERT_PosNormUvTan);
        return reinterpret_cast<VertPosNormUvTan*>(verts());
    }
    operator const VertPosNormUvTan*() const
    {
        ASSERT(mVertType == kVERT_PosNormUvTan);
        return reinterpret_cast<const VertPosNormUvTan*>(verts());
    }

    operator PrimPoint*()
    {
        ASSERT(mPrimType == kPRIM_Point);
        return reinterpret_cast<PrimPoint*>(prims());
    }
    operator const PrimPoint*() const
    {
        ASSERT(mPrimType == kPRIM_Point);
        return reinterpret_cast<const PrimPoint*>(prims());
    }
    operator PrimLine*()
    {
        ASSERT(mPrimType == kPRIM_Line);
        return reinterpret_cast<PrimLine*>(prims());
    }
    operator const PrimLine*() const
    {
        ASSERT(mPrimType == kPRIM_Line);
        return reinterpret_cast<const PrimLine*>(prims());
    }
    operator PrimTriangle*()
    {
        ASSERT(mPrimType == kPRIM_Triangle);
        return reinterpret_cast<PrimTriangle*>(prims());
    }
    operator const PrimTriangle*() const
    {
        ASSERT(mPrimType == kPRIM_Triangle);
        return reinterpret_cast<const PrimTriangle*>(prims());
    }
    //--------------------------------------------------------------------------
    // Cast operators (END)
    //--------------------------------------------------------------------------


    u32 vertCount() const { return mVertCount; }
    u32 primCount() const { return mPrimCount; }

    static Mesh * cast(u8 * pData, size_t dataSize);
    static Mesh * create(VertType vertType, u32 vertCount, PrimType primType, u32 primCount);
    static void destroy(Mesh * pMesh);

private:
    // Class should not be constructed directly.  Use cast and create static methods.
    Mesh() = default;
    Mesh(const Mesh&) = delete;
    Mesh & operator=(const Mesh&) = delete;

    VertType mVertType;
    PrimType mPrimType;

    u32 mVertCount;
    u32 mPrimCount;
    u32 mVertOffset;     // offset from start of struct
    u32 mPrimOffset;  // offset from start of struct

    u32 mHas32BitIndices:1;
    u32 mMorphTargetCount:7;
    u32 RESERVED_0:24;

    u32 mRendererReserved[kRendererReservedCount];

    u32 RESERVED[5]; // Pad to 64 bytes, for future possibilities.
};


// Sanity checks for sizeof our structs.
static_assert(sizeof(VertPos) == 12,          "VertPos geometry struct has unexpected size");
static_assert(sizeof(VertPosNorm) == 24,      "VertPosNorm geometry struct has unexpected size");
static_assert(sizeof(VertPosNormUv) == 32,    "VertPosNormUv geometry struct has unexpected size");
static_assert(sizeof(VertPosNormUvTan) == 48, "VertPosNormUvTan geometry struct has unexpected size");
static_assert(sizeof(PrimPoint) == 2,         "PrimLine geometry struct has unexpected size");
static_assert(sizeof(PrimLine) == 4,          "PrimLine geometry struct has unexpected size");
static_assert(sizeof(PrimTriangle) == 6,      "PrimTriangle geometry struct has unexpected size");
static_assert(sizeof(Mesh) == 64,             "Mesh struct has unexpected size");

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESH_H
