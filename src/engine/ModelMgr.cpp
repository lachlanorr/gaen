//------------------------------------------------------------------------------
// ModelMgr.cpp - Provides material ordered mesh iteration and model/material release semantics
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

#include "engine/ModelMgr.h"

namespace gaen
{

//------------------------------------------------------------------------------
// MeshIterator
//------------------------------------------------------------------------------
ModelMgr::MeshIterator::MeshIterator(const ShaderModelMap::const_iterator & shaderModelIterator,
                                     const ModelMeshMap::const_iterator & modelMeshIterator,
                                     const MeshList::const_iterator & meshIterator)
{
    mShaderModelIterator = shaderModelIterator;
    mModelMeshIterator = modelMeshIterator;
    mMeshListIterator = meshIterator;
}

const ModelMgr::MeshIterator& ModelMgr::MeshIterator::operator++()
{
    ++mMeshListIterator;

    if (mMeshListIterator == mModelMeshIterator->second.end())
    {
        ++mModelMeshIterator;

        if (mModelMeshIterator == mShaderModelIterator->second.end())
        {
            ++mShaderModelIterator;
            mModelMeshIterator = mShaderModelIterator->second.begin();
        }

        mMeshListIterator = mModelMeshIterator->second.begin();
    }

    return *this;
}

bool ModelMgr::MeshIterator::operator!=(const MeshIterator & rhs) const
{
    return mMeshListIterator->pModelInstance != rhs.mMeshListIterator->pModelInstance;
}

const ModelMgr::MaterialMeshInstance & ModelMgr::MeshIterator::operator*() const
{
    return *mMeshListIterator;
}


//------------------------------------------------------------------------------
// MeshIterator (END)
//------------------------------------------------------------------------------

ModelMgr::MeshIterator ModelMgr::begin()
{
    ShaderModelMap::const_iterator shaderModelIt = mShaderModelMap.begin();
    ModelMeshMap::const_iterator modelMeshIt = shaderModelIt->second.begin();
    
    return ModelMgr::MeshIterator(shaderModelIt,
                                  modelMeshIt,
                                  modelMeshIt->second.begin());
}

ModelMgr::MeshIterator ModelMgr::end()
{
    ShaderModelMap::const_iterator shaderModelIt = mShaderModelMap.end();
    --shaderModelIt;

    ModelMeshMap::const_iterator modelMeshIt = shaderModelIt->second.end();
    --modelMeshIt;

    return ModelMgr::MeshIterator(shaderModelIt,
                                  modelMeshIt,
                                  modelMeshIt->second.end());
}


ModelMgr::ModelMgr()
{
    // Place a dummy entry to ShaderMeshInstanceMap.
    // This simplifies our iterators knowing that there is always at
    // least this entry. Mesh list will remain empty, so impact
    // to iteration will be minimal.
    mShaderModelMap[-1][-1] = MeshList();
}

void ModelMgr::fin()
{
    // LORRTODO - delete all models and materials not managed by asset mgr
}

void ModelMgr::insertModelInstance(model_instance_id instanceId,
                                   Model * pModel,
                                   const Mat34 & worldTransform,
                                   bool isAssetManaged)
{
    // Insert into mModelMap if necessary
    ModelMap::iterator modelIt = mModelMap.find(pModel->id());
    if (modelIt == mModelMap.end())
    {
        mModelMap.emplace(pModel->id(), make_ref_counted(pModel, ModelDeleter(isAssetManaged)));
    }
    else
    {
        modelIt->second.addRef();
    }

    // Insert into mModelInstanceMap
    ASSERT(mModelInstanceMap.find(instanceId) == mModelInstanceMap.end());
    auto empResult = mModelInstanceMap.emplace(instanceId, ModelInstance(instanceId, pModel, worldTransform));
    if (!empResult.second)
        PANIC("Failed to emplace model instance");
    auto modelInstanceIt = empResult.first;

    // Insert materials
    for (Model::MaterialMesh & matMesh : *pModel)
    {
        insertMaterial(&matMesh.material(), isAssetManaged);

        // Insert meshes into mShaderModelMap
        mShaderModelMap[matMesh.shaderHash()][instanceId].push_back(MaterialMeshInstance(&modelInstanceIt->second, &matMesh));
    }
    
}

void ModelMgr::removeModelInstance(model_instance_id instanceId)
{
    // Insert into mModelInstanceMap
    auto modelInstanceIt = mModelInstanceMap.find(instanceId);
    if (modelInstanceIt == mModelInstanceMap.end())
    {
        PANIC("removeModelInstance of non existent material");
    }


    Model * pModel = modelInstanceIt->second.pModel;

    // Remove materials
    for (Model::MaterialMesh & matMesh : *pModel)
    {
        // Delete all meshes from mShaderModelMap for this instanceId
        mShaderModelMap[matMesh.shaderHash()].erase(instanceId);

        removeMaterial(&matMesh.material());
    }

    // Remove the modelInstanceIt
    mModelInstanceMap.erase(modelInstanceIt);

    // Remove from into mModelMap
    ModelMap::iterator modelIt = mModelMap.find(pModel->id());
    if (modelIt != mModelMap.end())
    {
        modelIt->second.release();
        if (!modelIt->second.get()) // if refcount has gone to zero
            mModelMap.erase(modelIt);
    }
    else
    {
        PANIC("removeModelInstance of non existent material");
    }
}

    
void ModelMgr::insertMaterial(Material * pMaterial,
                              bool isAssetManaged)
{
    MaterialMap::iterator materialIt = mMaterialMap.find(pMaterial->id());

    if (materialIt == mMaterialMap.end())
    {
        mMaterialMap.emplace(pMaterial->id(), make_ref_counted(pMaterial, MaterialDeleter(isAssetManaged)));
    }
    else
    {
        materialIt->second.addRef();
    }
}

void ModelMgr::removeMaterial(Material * pMaterial)
{
    MaterialMap::iterator materialIt = mMaterialMap.find(pMaterial->id());

    if (materialIt != mMaterialMap.end())
    {
        materialIt->second.release();
        if (!materialIt->second.get()) // if refcount has gone to zero
            mMaterialMap.erase(materialIt);
    }
    else
    {
        PANIC("removeMaterial of non existent material");
    }

}

} // namespace gaen
