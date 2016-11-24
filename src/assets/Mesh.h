//------------------------------------------------------------------------------
// Mesh.h - Geometry structs
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

#ifndef GAEN_ASETS_MESH_H
#define GAEN_ASETS_MESH_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "core/base_defines.h"
#include "core/mem.h"
#include "assets/Color.h"

namespace gaen
{

typedef u16 index;



class Mesh
{
public:
    static const u32 kRendererReservedCount = 4;

    VertType vertType() const { return static_cast<VertType>(mVertType); }
    PrimType primType() const { return static_cast<PrimType>(mPrimType); }

    f32 * verts()
    {
        return reinterpret_cast<f32*>(reinterpret_cast<u8*>(this) + vertOffset());
    }

    const f32 * verts() const
    {
        return reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(this) + vertOffset());
    }

    index * prims()
    {
        return reinterpret_cast<index*>(reinterpret_cast<u8*>(this) + primOffset());
    }

    const index * prims() const
    {
        return reinterpret_cast<const index*>(reinterpret_cast<const u8*>(this) + primOffset());
    }

    u32 vertOffset() const
    {
        return sizeof(Mesh);
    }

    u32 primOffset() const
    {
        return mPrimOffset;
    }

    u32 vertStride() const
    {
        // if there are morph targets, multiply by that count
        return vert_stride(vertType()) * (1 + mMorphTargetCount);
    }

    u32 vertsSize() const
    {
        return vertStride() * mVertCount;
    }

    u32 primStride() const
    {
        // double the primitive stride if indices are 32 bit instead of 16 bit
        return prim_stride(primType()) << mHas32BitIndices; // * 2 if mhas32BitIndices == 1
    }

    u32 primsSize() const
    {
        return primStride() * mPrimCount;
    }

    u32 indexCount() const
    {
        return mPrimCount * index_count(primType());
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

    bool hasVertPosition() const
    {
        return true; // all vert types have position
    }

    u32 vertPositionOffset() const
    {
        return 0; // position always at the start
    }

    bool hasVertNormal() const
    {
        return (mVertType == kVERT_PosNorm ||
                mVertType == kVERT_PosNormUv ||
                mVertType == kVERT_PosNormUvTan);
    }

    u32 vertNormalOffset() const
    {
        return sizeof(VertPos);
    }

    bool hasVertColor() const
    {
        return (mVertType == kVERT_PosNormCol);
    }

    u32 vertColorOffset() const
    {
        return sizeof(VertPosNorm);
    }

    bool hasVertUv() const
    {
        return (mVertType == kVERT_PosNormUv ||
                mVertType == kVERT_PosNormUvTan);
    }

    u32 vertUvOffset() const
    {
        return sizeof(VertPosNorm);
    }

    bool hasTan() const
    {
        return (mVertType == kVERT_PosNormUvTan);
    }

    u32 vertTanOffset() const
    {
        return sizeof(VertPosNormUv);
    }

    //--------------------------------------------------------------------------
    // Cast operators which provide a convenient way to get to vertices
    // and indices without a lot of explicit reinterpret casts.
    // E.g. Rather than:
    //    VertPosNorm * pVerts = reinterpret_cast<VertPosNorm*>(mesh.prims);
    // use instead:
    //    VertPosNorm * pVerts = mesh
    // 
    // The second form will call one of the cast operators below.
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
    operator VertPosNormCol*()
    {
        ASSERT(mVertType == kVERT_PosNormCol);
        return reinterpret_cast<VertPosNormCol*>(verts());
    }
    operator const VertPosNormCol*() const
    {
        ASSERT(mVertType == kVERT_PosNormCol);
        return reinterpret_cast<const VertPosNormCol*>(verts());
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

    static bool is_valid(const void * pBuffer, u64 size);
    static u64 required_size(VertType vertType, u32 vertCount, PrimType primType, u32 primCount);

    static Mesh * cast(u8 * pBuffer, u64 size);
    static Mesh * init(Mesh * pMesh, VertType vertType, u32 vertCount, PrimType primType, u32 primCount);
    static Mesh * create(VertType vertType, u32 vertCount, PrimType primType, u32 primCount);
    static void destroy(Mesh * pMesh);

private:
    // Class should not be constructed directly.  Use cast and create static methods.
    Mesh() = default;
    Mesh(const Mesh&) = delete;
    Mesh & operator=(const Mesh&) = delete;

    u32 mVertType:8;
    u32 mPrimType:8;
    u32 mHas32BitIndices:1;
    u32 mMorphTargetCount:7;
    u32 RESERVED_0:8;

    u32 mVertCount;
    u32 mPrimCount;
    // VertOffset is sizeof(Mesh), they start immediately after the header
    u32 mPrimOffset;  // offset from start of struct

    u32 mRendererReserved[kRendererReservedCount];
};

// Sanity checks for sizeof our structs.
static_assert(sizeof(VertPos) == 12,          "VertPos geometry struct has unexpected size");
static_assert(sizeof(VertPosNorm) == 24,      "VertPosNorm geometry struct has unexpected size");
static_assert(sizeof(VertPosNormCol) == 28,   "VertPosNormCol geometry struct has unexpected size");
static_assert(sizeof(VertPosNormUv) == 32,    "VertPosNormUv geometry struct has unexpected size");
static_assert(sizeof(VertPosNormUvTan) == 48, "VertPosNormUvTan geometry struct has unexpected size");
static_assert(sizeof(PrimPoint) == 2,         "PrimLine geometry struct has unexpected size");
static_assert(sizeof(PrimLine) == 4,          "PrimLine geometry struct has unexpected size");
static_assert(sizeof(PrimTriangle) == 6,      "PrimTriangle geometry struct has unexpected size");
static_assert(sizeof(Mesh) == 32,             "Mesh struct has unexpected size");

} // namespace gaen

#endif // #ifndef GAEN_ASETS_MESH_H
