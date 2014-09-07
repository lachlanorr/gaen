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

static u32 calc_size(u32 vertStride, u32 vertCount, u32 primitiveStride, u32 primitiveCount)
{
    return (sizeof(Mesh) + vertStride * vertCount + primitiveStride * primitiveCount);
}

Mesh * Mesh::cast(u8 * pData, size_t dataSize)
{
    Mesh * pMesh = reinterpret_cast<Mesh*>(pData);

    if (!is_valid_vert_type(pMesh->mVertType))
        PANIC("Mesh invalid VertType");

    if (!is_valid_prim_type(pMesh->mPrimType))
        PANIC("Mesh invalid PrimitiveType");

    if (dataSize <= sizeof(Mesh))
        PANIC("Mesh dataSize too small");

    u32 vertStride = pMesh->vertStride();
    u32 primitiveStride = pMesh->primStride();

    if (dataSize != pMesh->totalSize())
        PANIC("Mesh invalid dataSize");

    if (pMesh->mVertOffset != sizeof(Mesh))
        PANIC("Mesh invalid vertOffset");

    if (pMesh->mPrimOffset != sizeof(Mesh) + vertStride * pMesh->mVertCount)
        PANIC("Mesh invalid lineOffset or vertCount");

    return pMesh;
}

Mesh * Mesh::create(VertType vertType, u32 vertCount, PrimType primType, u32 primCount)
{
    u32 vertStride = vert_stride(vertType);
    u32 primStride = prim_stride(primType);

    u32 dataSize = calc_size(vertStride, vertCount, primStride, primCount);
    u8 * pData = static_cast<u8*>(GALLOC(kMEM_Model, dataSize));

    Mesh * pMesh = reinterpret_cast<Mesh*>(pData);
    pMesh->mVertType = vertType;
    pMesh->mPrimType = primType;
    pMesh->mVertCount = vertCount;
    pMesh->mPrimCount = primCount;
    pMesh->mVertOffset = sizeof(Mesh);
    pMesh->mPrimOffset = pMesh->mVertOffset + vertStride * vertCount;

    pMesh->mRendererVertsId = -1;
    pMesh->mRendererPrimsId = -1;
    pMesh->mHas32BitIndices = 0;
    pMesh->mMorphTargetCount = 0; // no targets, just one set of verts

    // Return result of cast, since it will sanity check our struct is built properly
    return cast(pData, dataSize);
}

void destroy(Mesh * pMesh)
{
    GFREE(pMesh);
}


} // namespace gaen
