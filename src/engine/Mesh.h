//------------------------------------------------------------------------------
// geometry.h - Geometry structs
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

#ifndef GAEN_CORE_GEOMETRY_H
#define GAEN_CORE_GEOMETRY_H

#include "core/base_defines.h"
#include "core/mem.h"
#include "engine/Color.h"

namespace gaen
{

//------------------------------------------------------------------------------
// Vertex structs
//------------------------------------------------------------------------------
struct VertexSMPL
{
    static const u32 kVertexType = 'SMPL';
    Vec3 position;
    Vec3 normal;
};

struct VertexTXTR
{
    static const u32 kVertexType = 'TXTR';
    Vec3 position;
    Vec3 normal;
    f32 u;
    f32 v;
};

struct VertexNRML
{
    static const u32 kVertexType = 'NRML';
    Vec3 position;
    Vec3 normal;
    f32 u;
    f32 v;
    Vec4 tangent;
};

const unsigned int kOffsetPosition = 0;
const unsigned int kOffsetNormal   = kOffsetPosition + sizeof(Vec3);
const unsigned int kOffsetUV       = kOffsetNormal + sizeof(Vec3);
const unsigned int kOffsetTangent  = kOffsetUV + sizeof(f32) + sizeof(f32);


struct Triangle
{
    // a triangle is composed of 3 indices into point array
    Triangle(u16 p0, u16 p1, u16 p2)
      : p0(p0), p1(p1), p2(p2) {}

    u16 p0;
    u16 p1;
    u16 p2;
};

struct Line
{
    Line(u16 p0, u16 p1)
      : p0(p0), p1(p1) {}

    // a line is composed of 2 indices into point array
    u16 p0;
    u16 p1;
};

//-------------------------------------------
// Comparison operators for Polygon and Line
//-------------------------------------------
inline bool operator==(const Triangle & lhs, const Triangle & rhs)
{
    return lhs.p0 == rhs.p0 &&
        lhs.p1 == rhs.p1 &&
        lhs.p2 == rhs.p2;
}

inline bool operator<(const Triangle & lhs, const Triangle & rhs)
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

inline bool operator==(const Line & lhs, const Line & rhs)
{
    // do a simple sort of points to consider two lines equal even if
    // their points are in different order
    u16 line0p0,line0p1,line1p0,line1p1;

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

inline bool operator<(const Line & lhs, const Line & rhs)
{
    // do a simple sort of points to consider two lines equal even if
    // their points are in different order
    u16 line0p0,line0p1,line1p0,line1p1;

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

template <class VertexT, class IndexT>
class Mesh
{
public:
    typedef VertexT vertex_type;
    typedef IndexT index_type;

    vertex_type * vertices()
    {
        return reinterpret_cast<vertex_type*>(reinterpret_cast<u8*>(this) + mVertexOffset);
    }

    const vertex_type * vertices() const
    {
        return reinterpret_cast<const vertex_type*>(reinterpret_cast<const u8*>(this) + mVertexOffset);
    }

    index_type * indices()
    {
        return reinterpret_cast<index_type*>(reinterpret_cast<u8*>(this) + mIndexOffset);
    }

    const index_type * indices() const
    {
        return reinterpret_cast<const index_type*>(reinterpret_cast<const u8*>(this) + mIndexOffset);
    }

    u32 vertexCount() const { return mVertexCount; }
    u32 indexCount() const { return mIndexCount; }

    static Mesh * cast(u8 * pData, size_t dataSize)
    {
        Mesh * pMesh = reinterpret_cast<Mesh*>(pData);
        if (pMesh->mVertexType != vertex_type::kVertexType)
            PANIC("Mesh wrong VertexType");

        if (dataSize <= sizeof(Mesh))
            PANIC("Mesh data too small");

        if (dataSize != calc_size(pMesh->mVertexCount, pMesh->mIndexCount))
            PANIC("Mesh data wrong size");

        if (pMesh->mVertexOffset != sizeof(Mesh))
            PANIC("Mesh wrong vertexOffset");

        if (pMesh->mIndexOffset != (pData + sizeof(Mesh) + pMesh->mVertexCount * sizeof(vertex_type)))
            PANIC("Wrong lineOffset or vertexCount");

        return pMesh;
    }

    static Mesh * create(u32 vertexCount, u32 indexCount)
    {
        size_t dataSize = sizeof(Mesh) + vertexCount * sizeof(vertex_type) + indexCount * sizeof(index_type);
        u8 * pData = ALLOC(dataSize, kMT_Model);
        Mesh * pMesh = reinterpret_cast<Mesh*>(pData);
        pMesh->mVertexType = vertex_type::kVertexType;
        pMesh->mColor.setValue(0);
        pMesh->mVertexCount = vertexCount;
        pMesh->mIndexCount = indexCount;
        pMesh->mVertexOffset = pData + sizeof(Mesh);
        pMesh->mIndexOffset = mVertexOffset + vertexCount * sizeof(vertex_type);

        // Return result of cast, since it will sanity check our struct is built properly
        return cast(pData, dataSize);
    }

    static size_t calc_size(u32 vertexCount, u32 indexCount)
    {
        return (sizeof(Mesh) + vertexCount * sizeof(vertex_type) + indexCount * sizeof(index_type));
    }

private:
    // Class should not be constructed directly.  Use cast and create static methods.
    Mesh() = default;
    Mesh(const Mesh&) = delete;
    Mesh & operator=(const Mesh&) = delete;

    u32 mVertexType;
    Color mColor;
    u32 mVertexCount;
    u32 mIndexCount;
    u32 mVertexOffset;
    u32 mIndexOffset;
};


// Sanity checks for sizeof our structs.  Just in case a different compiler/platform pads stuff unexpectedly.
static_assert(sizeof(VertexSMPL) == 24,      "geometry struct has unexpected size");
static_assert(sizeof(VertexSMPL[10]) == 240, "geometry struct has unexpected size");
static_assert(sizeof(VertexTXTR) == 32,      "geometry struct has unexpected size");
static_assert(sizeof(VertexTXTR[10]) == 320, "geometry struct has unexpected size");
static_assert(sizeof(VertexNRML) == 48,      "geometry struct has unexpected size");
static_assert(sizeof(VertexNRML[10]) == 480, "geometry struct has unexpected size");
static_assert(sizeof(Triangle) == 6,         "geometry struct has unexpected size");
static_assert(sizeof(Triangle[10]) == 60,    "geometry struct has unexpected size");
static_assert(sizeof(Line) == 4,             "geometry struct has unexpected size");
static_assert(sizeof(Line[10]) == 40,        "geometry struct has unexpected size");

} // namespace gaen

#endif // #ifndef GAEN_CORE_GEOMETRY_H
