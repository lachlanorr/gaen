//------------------------------------------------------------------------------
// Mesh.h - Geometry structs
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

#ifndef GAEN_ENGINE_MESH_H
#define GAEN_ENGINE_MESH_H

#include "core/base_defines.h"
#include "core/mem.h"
#include "engine/Color.h"

namespace gaen
{

typedef u16 index;

enum VertexType
{
    kVERT_Pos          = GAEN_4CC('V','T','X','A'),
    kVERT_PosNorm      = GAEN_4CC('V','T','X','B'),
    kVERT_PosNormUv    = GAEN_4CC('V','T','X','C'),
    kVERT_PosNormUvTan = GAEN_4CC('V','T','X','D'),

    kVERT_END
};

enum PrimitiveType
{
    kPRIM_Point    = GAEN_4CC('P','R','M','A'),
    kPRIM_Line     = GAEN_4CC('P','R','M','B'),
    kPRIM_Triangle = GAEN_4CC('P','R','M','C'),

    kPRIM_END
};


//------------------------------------------------------------------------------
// Vertex structs
//------------------------------------------------------------------------------
struct VertexPos
{
    static const VertexType kVertexType = kVERT_Pos;
    Vec3 position;
};

struct VertexPosNorm
{
    static const VertexType kVertexType = kVERT_PosNorm;
    Vec3 position;
    Vec3 normal;
};

struct VertexPosNormUv
{
    static const VertexType kVertexType = kVERT_PosNormUv;
    Vec3 position;
    Vec3 normal;
    f32 u;
    f32 v;
};

struct VertexPosNormUvTan
{
    static const VertexType kVertexType = kVERT_PosNormUvTan;
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

inline bool is_valid_vertex_type(u32 vertexType)
{
    return vertexType >= kVERT_PosNorm && vertexType < kVERT_END;
}

inline bool is_valid_primitive_type(u32 primitiveType)
{
    return primitiveType >= kPRIM_Point && primitiveType < kPRIM_END;
}

inline u8 vertex_type_zero_based_id(VertexType vertexType)
{
    // LORRTODO - Big endian support needed here if we ever port to such a platform
    ASSERT(is_valid_vertex_type(vertexType));
    static const u32 kSwapped_kVERT_Pos = BYTESWAP32(kVERT_Pos);
    return (u8)(BYTESWAP32(vertexType) - kSwapped_kVERT_Pos);
}

inline u8 vertex_type_zero_based_id_end()
{
    // LORRTODO - Big endian support needed here if we ever port to such a platform
    static const u32 diff = BYTESWAP32(kVERT_END) - BYTESWAP32(kVERT_Pos);
    return (u8)diff;
}


inline u8 primitive_type_zero_based_id(PrimitiveType primitiveType)
{
    // LORRTODO - Big endian support needed here if we ever port to such a platform
    ASSERT(is_valid_primitive_type(primitiveType));
    static const u32 kSwapped_kPRIM_Point = BYTESWAP32(kPRIM_Point);
    return (u8)(BYTESWAP32(primitiveType) - kSwapped_kPRIM_Point);
}

inline u8 primitive_type_zero_based_id_end()
{
    // LORRTODO - Big endian support needed here if we ever port to such a platform
    static const u32 diff = BYTESWAP32(kPRIM_END) - BYTESWAP32(kPRIM_Point);
    return (u8)diff;
}

struct PrimitivePoint
{
    PrimitivePoint(index p0)
      : p0(p0) {}

    // a point is composed of a single index into point array
    index p0;
};

struct PrimitiveLine
{
    PrimitiveLine(index p0, index p1)
      : p0(p0), p1(p1) {}

    // a line is composed of 2 indices into point array
    index p0;
    index p1;
};


struct PrimitiveTriangle
{
    // a triangle is composed of 3 indices into point array
    PrimitiveTriangle(index p0, index p1, index p2)
      : p0(p0), p1(p1), p2(p2) {}

    index p0;
    index p1;
    index p2;
};


inline u32 vertex_stride(VertexType vertexType)
{
    switch(vertexType)
    {
    case kVERT_Pos:
        return sizeof(VertexPos);
    case kVERT_PosNorm:
        return sizeof(VertexPosNorm);
    case kVERT_PosNormUv:
        return sizeof(VertexPosNormUv);
    case kVERT_PosNormUvTan:
        return sizeof(VertexPosNormUvTan);
    default:
        PANIC("Invalid VertexType: %d", vertexType);
        return 0;
    }
}

inline u32 primitive_stride(PrimitiveType primitiveType)
{
    switch(primitiveType)
    {
    case kPRIM_Point:
        return sizeof(PrimitivePoint);
    case kPRIM_Line:
        return sizeof(PrimitiveLine);
    case kPRIM_Triangle:
        return sizeof(PrimitiveTriangle);
    default:
        PANIC("Invalid PrimitiveType: %d", primitiveType);
        return 0;
    }
}


//-------------------------------------------
// Comparison operators for Polygon and Line
//-------------------------------------------
inline bool operator==(const PrimitiveTriangle & lhs, const PrimitiveTriangle & rhs)
{
    return lhs.p0 == rhs.p0 &&
        lhs.p1 == rhs.p1 &&
        lhs.p2 == rhs.p2;
}

inline bool operator<(const PrimitiveTriangle & lhs, const PrimitiveTriangle & rhs)
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

inline bool operator==(const PrimitiveLine & lhs, const PrimitiveLine & rhs)
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

inline bool operator<(const PrimitiveLine & lhs, const PrimitiveLine & rhs)
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
    VertexType vertexType() const { return mVertexType; }
    PrimitiveType primitiveType() const { return mPrimitiveType; }

    f32 * vertices()
    {
        return reinterpret_cast<f32*>(reinterpret_cast<u8*>(this) + mVertexOffset);
    }

    const f32 * vertices() const
    {
        return reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(this) + mVertexOffset);
    }

    index * indices()
    {
        return reinterpret_cast<index*>(reinterpret_cast<u8*>(this) + mPrimitiveOffset);
    }

    const index * indices() const
    {
        return reinterpret_cast<const index*>(reinterpret_cast<const u8*>(this) + mPrimitiveOffset);
    }

    //--------------------------------------------------------------------------
    // Cast operators which provide a convenient way to get to vertices
    // and indices without a lot of explicit reinterpret casts.
    // E.g. Rather than:
    //    VertexPosNorm * pVerts = reinterpret_cast<VertexPosNorm*>(mesh.indices);
    // use instead:
    //    VertexPosNorm * pVerts = mesh
    // 
    // The second form will call one of the cast operators below
    //--------------------------------------------------------------------------
    operator VertexPos*()
    {
        ASSERT(mVertexType == kVERT_Pos);
        return reinterpret_cast<VertexPos*>(vertices());
    }
    operator const VertexPos*() const
    {
        ASSERT(mVertexType == kVERT_Pos);
        return reinterpret_cast<const VertexPos*>(vertices());
    }
    operator VertexPosNorm*()
    {
        ASSERT(mVertexType == kVERT_PosNorm);
        return reinterpret_cast<VertexPosNorm*>(vertices());
    }
    operator const VertexPosNorm*() const
    {
        ASSERT(mVertexType == kVERT_PosNorm);
        return reinterpret_cast<const VertexPosNorm*>(vertices());
    }
    operator VertexPosNormUv*()
    {
        ASSERT(mVertexType == kVERT_PosNormUv);
        return reinterpret_cast<VertexPosNormUv*>(vertices());
    }
    operator const VertexPosNormUv*() const
    {
        ASSERT(mVertexType == kVERT_PosNormUv);
        return reinterpret_cast<const VertexPosNormUv*>(vertices());
    }
    operator VertexPosNormUvTan*()
    {
        ASSERT(mVertexType == kVERT_PosNormUvTan);
        return reinterpret_cast<VertexPosNormUvTan*>(vertices());
    }
    operator const VertexPosNormUvTan*() const
    {
        ASSERT(mVertexType == kVERT_PosNormUvTan);
        return reinterpret_cast<const VertexPosNormUvTan*>(vertices());
    }

    operator PrimitivePoint*()
    {
        ASSERT(mPrimitiveType == kPRIM_Point);
        return reinterpret_cast<PrimitivePoint*>(indices());
    }
    operator const PrimitivePoint*() const
    {
        ASSERT(mPrimitiveType == kPRIM_Point);
        return reinterpret_cast<const PrimitivePoint*>(indices());
    }
    operator PrimitiveLine*()
    {
        ASSERT(mPrimitiveType == kPRIM_Line);
        return reinterpret_cast<PrimitiveLine*>(indices());
    }
    operator const PrimitiveLine*() const
    {
        ASSERT(mPrimitiveType == kPRIM_Line);
        return reinterpret_cast<const PrimitiveLine*>(indices());
    }
    operator PrimitiveTriangle*()
    {
        ASSERT(mPrimitiveType == kPRIM_Triangle);
        return reinterpret_cast<PrimitiveTriangle*>(indices());
    }
    operator const PrimitiveTriangle*() const
    {
        ASSERT(mPrimitiveType == kPRIM_Triangle);
        return reinterpret_cast<const PrimitiveTriangle*>(indices());
    }
    //--------------------------------------------------------------------------
    // Cast operators (END)
    //--------------------------------------------------------------------------


    u32 vertexCount() const { return mVertexCount; }
    u32 primitiveCount() const { return mPrimitiveCount; }

    static Mesh * cast(u8 * pData, size_t dataSize);
    static Mesh * create(VertexType vertexType, u32 vertexCount, PrimitiveType primitiveType, u32 primitiveCount);
    static void destroy(Mesh * pMesh);

private:
    // Class should not be constructed directly.  Use cast and create static methods.
    Mesh() = default;
    Mesh(const Mesh&) = delete;
    Mesh & operator=(const Mesh&) = delete;

    VertexType mVertexType;
    PrimitiveType mPrimitiveType;
    u32 mVertexOffset;
    u32 mPrimitiveOffset;
    index mVertexCount;
    index mPrimitiveCount;
};


// Sanity checks for sizeof our structs.  Just in case a different compiler/platform pads stuff unexpectedly.
static_assert(sizeof(VertexPos) == 12,               "geometry struct has unexpected size");
static_assert(sizeof(VertexPos[10]) == 120,          "geometry struct has unexpected size");
static_assert(sizeof(VertexPosNorm) == 24,           "geometry struct has unexpected size");
static_assert(sizeof(VertexPosNorm[10]) == 240,      "geometry struct has unexpected size");
static_assert(sizeof(VertexPosNormUv) == 32,         "geometry struct has unexpected size");
static_assert(sizeof(VertexPosNormUv[10]) == 320,    "geometry struct has unexpected size");
static_assert(sizeof(VertexPosNormUvTan) == 48,      "geometry struct has unexpected size");
static_assert(sizeof(VertexPosNormUvTan[10]) == 480, "geometry struct has unexpected size");
static_assert(sizeof(PrimitiveTriangle) == 6,            "geometry struct has unexpected size");
static_assert(sizeof(PrimitiveTriangle[10]) == 60,       "geometry struct has unexpected size");
static_assert(sizeof(PrimitiveLine) == 4,                "geometry struct has unexpected size");
static_assert(sizeof(PrimitiveLine[10]) == 40,           "geometry struct has unexpected size");
static_assert(sizeof(Mesh) % sizeof(f32) == 0,       "Mesh struct size not aligned. Must be multiple of sizeof(f32)");

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESH_H
