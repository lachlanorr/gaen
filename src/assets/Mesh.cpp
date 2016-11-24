//------------------------------------------------------------------------------
// Mesh.cpp - Geometry structs
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

#include "assets/Mesh.h"

namespace gaen
{

static u32 calc_size(u32 vertStride, u32 vertCount, u32 primitiveStride, u32 primitiveCount)
{
    return (sizeof(Mesh) + vertStride * vertCount + primitiveStride * primitiveCount);
}

bool Mesh::is_valid(const void * pBuffer, u64 size)
{
    const Mesh * pMesh = reinterpret_cast<const Mesh*>(pBuffer);

    if (!is_valid_vert_type(pMesh->mVertType))
        return false;

    if (!is_valid_prim_type(pMesh->mPrimType))
        return false;

    if (size <= sizeof(Mesh))
        return false;

    u32 vertStride = pMesh->vertStride();

    if (size != pMesh->totalSize())
        return false;

    if (pMesh->primOffset() != sizeof(Mesh) + vertStride * pMesh->mVertCount)
        return false;

    return true;
}

u64 Mesh::required_size(VertType vertType, u32 vertCount, PrimType primType, u32 primCount)
{
    u32 vertStride = vert_stride(vertType);
    u32 primStride = prim_stride(primType);

    return calc_size(vertStride, vertCount, primStride, primCount);
}

Mesh * Mesh::cast(u8 * pBuffer, u64 size)
{
    Mesh * pMesh = reinterpret_cast<Mesh*>(pBuffer);

    if (!is_valid(pBuffer, size))
        PANIC("Mesh invalid");

    return pMesh;
}

Mesh * Mesh::init(Mesh * pMesh, VertType vertType, u32 vertCount, PrimType primType, u32 primCount)
{
    u32 vertStride = vert_stride(vertType);
    u32 primStride = prim_stride(primType);

    pMesh->mVertType = vertType;
    pMesh->mPrimType = primType;
    pMesh->mVertCount = vertCount;
    pMesh->mPrimCount = primCount;
    pMesh->mPrimOffset = pMesh->vertOffset() + vertStride * vertCount;

    for (u32 i = 0; i < kRendererReservedCount; ++i)
        pMesh->mRendererReserved[i] = -1;

    pMesh->mHas32BitIndices = 0;
    pMesh->mMorphTargetCount = 0; // no targets, just one set of verts

    return pMesh;
}

Mesh * Mesh::create(VertType vertType, u32 vertCount, PrimType primType, u32 primCount)
{
    u32 vertStride = vert_stride(vertType);
    u32 primStride = prim_stride(primType);

    u32 size = calc_size(vertStride, vertCount, primStride, primCount);
    Mesh * pMesh = static_cast<Mesh*>(GALLOC(kMEM_Model, size));

    init(pMesh, vertType, vertCount, primType, primCount);

    return pMesh;
}

void Mesh::destroy(Mesh * pMesh)
{
    GFREE(pMesh);
}


} // namespace gaen
