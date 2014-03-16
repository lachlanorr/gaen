//------------------------------------------------------------------------------
// Model.cpp - Model, a collection of meshes/materials
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

#include <atomic>

#include "engine/Model.h"

namespace gaen
{

static std::atomic<material_mesh_id> sNextMaterialMeshId(0);
static std::atomic<model_id> sNextModelId(0);

Model::MaterialMesh::MaterialMesh(Model * pModel, Material * pMaterial, Mesh * pMesh)
  : mpModel(pModel)
  , mpMaterial(pMaterial)
  , mpMesh(pMesh)
{
    ASSERT(pMaterial);
    ASSERT(pMesh);

    mId = sNextMaterialMeshId.fetch_add(1,std::memory_order_relaxed);

    // Calculate unique shader hash
    mShaderHash = calcShaderHash();
}

Model::MaterialMesh::~MaterialMesh()
{
    delete mpMaterial;
    delete mpMesh;
}

shader_hash Model::MaterialMesh::calcShaderHash()
{
    // Pack the bits to build a unique hash for the shader.
    // Right now, we only consider material type, vertex type
    // and index type. It's packed pretty tightly, so we have
    // room for more stuff if we need to differentiate further.

    // Current packing is: (high order on left)
    //
    // | MaterialType | VertexType | IndexType | Unused  |
    // | 4 bits       | 3 bits     | 2 bits    | 23 bits |
    
    u8 matType = static_cast<u8>(mpMaterial->type());
    u8 vertType = vertex_type_zero_based_id(mpMesh->vertexType());
    u8 indType = index_type_zero_based_id(mpMesh->indexType());

    if (matType >= 16)
        PANIC("Not enough bits for MaterialType %d", matType);
    if (vertType >= 8)
        PANIC("Not enough bits for VertexType %d", vertType);
    if (indType >= 4)
        PANIC("Not enough bits for IndexType %d", indType);

    shader_hash hash = (matType << 28) |
                       (vertType << 25) |
                       (indType << 23);

    return hash;
}


Model::Model(Material * pMaterial, Mesh * pMesh, size_t meshCount)
{
    mId = sNextModelId.fetch_add(1, std::memory_order_relaxed);

    if (meshCount > 1)
        reserveMeshCapacity(meshCount);
    insertMaterialMesh(pMaterial, pMesh);
}

Model::~Model()
{
    mMaterialMeshes.clear();
}

void Model::reserveMeshCapacity(size_t meshCount)
{
    mMaterialMeshes.reserve(meshCount);
}

void Model::insertMaterialMesh(Material * pMaterial, Mesh * pMesh)
{
    if (mIsReadOnly)
        PANIC("Mesh being added to read only model");
    mMaterialMeshes.emplace_back(this, pMaterial, pMesh);
}


} // namespace gaen
