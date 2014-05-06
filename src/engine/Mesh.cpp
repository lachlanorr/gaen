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

static u32 calc_size(u32 vertexStride, u32 vertexCount, u32 indexStride, u32 indexCount)
{
    return (sizeof(Mesh) + vertexStride * vertexCount + indexStride * indexCount);
}

Mesh * Mesh::cast(u8 * pData, size_t dataSize)
{
    Mesh * pMesh = reinterpret_cast<Mesh*>(pData);

    if (!is_valid_vertex_type(pMesh->mVertexType))
        PANIC("Mesh invalid VertexType");

    if (!is_valid_index_type(pMesh->mIndexType))
        PANIC("Mesh invalid IndexType");

    if (dataSize <= sizeof(Mesh))
        PANIC("Mesh dataSize too small");

    u32 vertexStride = vertex_stride(pMesh->mVertexType);
    u32 indexStride = index_stride(pMesh->mIndexType);

    if (dataSize != calc_size(vertexStride, pMesh->mVertexCount, indexStride, pMesh->mIndexCount))
        PANIC("Mesh invalid dataSize");

    if (pMesh->mVertexOffset != sizeof(Mesh))
        PANIC("Mesh invalid vertexOffset");

    if (pMesh->mIndexOffset != sizeof(Mesh) + vertexStride * pMesh->mVertexCount)
        PANIC("Mesh invalid lineOffset or vertexCount");

    return pMesh;
}

Mesh * Mesh::create(VertexType vertexType, u32 vertexCount, IndexType indexType, u32 indexCount)
{
    u32 vertexStride = vertex_stride(vertexType);
    u32 indexStride = index_stride(indexType);

    u32 dataSize = calc_size(vertexStride, vertexCount, indexStride, indexCount);
    u8 * pData = static_cast<u8*>(GALLOC(kMEM_Model, dataSize));

    Mesh * pMesh = reinterpret_cast<Mesh*>(pData);
    pMesh->mVertexType = vertexType;
    pMesh->mIndexType = indexType;
    pMesh->mVertexCount = vertexCount;
    pMesh->mIndexCount = indexCount;
    pMesh->mVertexOffset = sizeof(Mesh);
    pMesh->mIndexOffset = pMesh->mVertexOffset + vertexStride * vertexCount;

    // Return result of cast, since it will sanity check our struct is built properly
    return cast(pData, dataSize);
}


} // namespace gaen
