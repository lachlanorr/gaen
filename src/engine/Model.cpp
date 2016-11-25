//------------------------------------------------------------------------------
// Model.cpp - Model, a collection of gmdls/materials
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

#include "engine/stdafx.h"

#include "engine/Model.h"

namespace gaen
{

static std::atomic<material_gmdl_id> sNextMaterialGmdlId(0);
static std::atomic<model_id> sNextModelId(0);

Model::MaterialGmdl::MaterialGmdl(Model * pModel, Material * pMaterial, Gmdl * pGmdl)
  : mpModel(pModel)
  , mpMaterial(pMaterial)
  , mpGmdl(pGmdl)
{
    ASSERT(pMaterial);
    ASSERT(pGmdl);

    mId = sNextMaterialGmdlId.fetch_add(1,std::memory_order_relaxed);

    // Calculate unique shader hash
    mSortOrder = calcSortOrder();
}

Model::MaterialGmdl::~MaterialGmdl()
{
    GDELETE(mpMaterial);
    GDELETE(mpGmdl);
}

material_gmdl_sort Model::MaterialGmdl::calcSortOrder()
{
    // Pack the bits to build a sort order/unique hash for the shader.

    // Current packing is: (high order on left)
    //
    // | MaterialLayer | ModelId | ShaderNameHash |
    // | 4 bits        | 28 bits | 32 bits        |
    
    u8 matLayer = static_cast<u8>(mpMaterial->layer());

    PANIC_IF(matLayer      >= (1 << 4),  "Not enough bits for MaterialLayer %d", matLayer);
    PANIC_IF(mpModel->id() >= (1 << 28), "Not enough bits for ModelId %d", mpModel->id());

    material_gmdl_sort matGmdlSort = ((u64)matLayer << 60) |
                                     ((u64)mpModel->id() << 32) |
                                     mpMaterial->shaderNameHash();

    return matGmdlSort;
}


Model::Model(Material * pMaterial, Gmdl * pGmdl, size_t gmdlCount)
{
    mId = sNextModelId.fetch_add(1, std::memory_order_relaxed);

    if (gmdlCount > 1)
        reserveGmdlCapacity(gmdlCount);
    insertMaterialGmdl(pMaterial, pGmdl);
}

Model::~Model()
{
    mMaterialGmdls.clear();
}

void Model::reserveGmdlCapacity(size_t gmdlCount)
{
    mMaterialGmdls.reserve(gmdlCount);
}

void Model::insertMaterialGmdl(Material * pMaterial, Gmdl * pGmdl)
{
    if (mIsReadOnly)
        PANIC("Gmdl being added to read only model");
    mMaterialGmdls.emplace_back(this, pMaterial, pGmdl);
}


} // namespace gaen
