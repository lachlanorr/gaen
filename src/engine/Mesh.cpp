//------------------------------------------------------------------------------
// Mesh.cpp - Geometry structs
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

#include "engine/stdafx.h"

#include "engine/Mesh.h"

namespace gaen
{

static u32 calc_size(u32 vertexStride, u32 vertexCount, u32 primitiveStride, u32 primitiveCount)
{
    return (sizeof(Mesh) + vertexStride * vertexCount + primitiveStride * primitiveCount);
}

Mesh * Mesh::cast(u8 * pData, size_t dataSize)
{
    Mesh * pMesh = reinterpret_cast<Mesh*>(pData);

    if (!is_valid_vertex_type(pMesh->mVertexType))
        PANIC("Mesh invalid VertexType");

    if (!is_valid_primitive_type(pMesh->mPrimitiveType))
        PANIC("Mesh invalid PrimitiveType");

    if (dataSize <= sizeof(Mesh))
        PANIC("Mesh dataSize too small");

    u32 vertexStride = vertex_stride(pMesh->mVertexType);
    u32 primitiveStride = primitive_stride(pMesh->mPrimitiveType);

    if (dataSize != calc_size(vertexStride, pMesh->mVertexCount, primitiveStride, pMesh->mPrimitiveCount))
        PANIC("Mesh invalid dataSize");

    if (pMesh->mVertexOffset != sizeof(Mesh))
        PANIC("Mesh invalid vertexOffset");

    if (pMesh->mPrimitiveOffset != sizeof(Mesh) + vertexStride * pMesh->mVertexCount)
        PANIC("Mesh invalid lineOffset or vertexCount");

    return pMesh;
}

Mesh * Mesh::create(VertexType vertexType, u32 vertexCount, PrimitiveType primitiveType, u32 primitiveCount)
{
    u32 vertexStride = vertex_stride(vertexType);
    u32 primitiveStride = primitive_stride(primitiveType);

    u32 dataSize = calc_size(vertexStride, vertexCount, primitiveStride, primitiveCount);
    u8 * pData = static_cast<u8*>(GALLOC(kMEM_Model, dataSize));

    Mesh * pMesh = reinterpret_cast<Mesh*>(pData);
    pMesh->mVertexType = vertexType;
    pMesh->mPrimitiveType = primitiveType;
    pMesh->mVertexCount = vertexCount;
    pMesh->mPrimitiveCount = primitiveCount;
    pMesh->mVertexOffset = sizeof(Mesh);
    pMesh->mPrimitiveOffset = pMesh->mVertexOffset + vertexStride * vertexCount;

    // Return result of cast, since it will sanity check our struct is built properly
    return cast(pData, dataSize);
}

void destroy(Mesh * pMesh)
{
    GFREE(pMesh);
}


} // namespace gaen
