//------------------------------------------------------------------------------
// ModelMgr.h - Provides material ordered mesh iteration and model/material release semantics
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

#ifndef GAEN_ENGINE_MODELMGR_H
#define GAEN_ENGINE_MODELMGR_H

#include "core/Map.h"
#include "core/List.h"
#include "core/RefCounted.h"

#include "engine/Model.h"
#include "engine/MessageQueue.h"

namespace gaen
{

//------------------------------------------------------------------------------

struct ModelInstance
{
    ModelInstance(model_instance_id instanceId,
                  Model * pModel,
                  const Mat34 & worldTransform)
      : worldTransform(worldTransform)
      , pModel(pModel)
      , instanceId(instanceId)
    {}
        
    Mat34 worldTransform;
    Model * pModel;
    model_instance_id instanceId;
};
    
//------------------------------------------------------------------------------

struct MaterialMeshInstance
{
    MaterialMeshInstance(ModelInstance * pModelInstance,
                         Model::MaterialMesh * pMaterialMesh)
      : pModelInstance(pModelInstance)
      , pMaterialMesh(pMaterialMesh)
    {}

    ModelInstance * pModelInstance;
    Model::MaterialMesh * pMaterialMesh;
};

//------------------------------------------------------------------------------

typedef List<kMEM_Model,
    MaterialMeshInstance> MeshList;
typedef Map<kMEM_Model,
    model_instance_id,
    MeshList> ModelMeshMap;
typedef Map<kMEM_Model,
    shader_hash,
    ModelMeshMap> ShaderModelMap;

//------------------------------------------------------------------------------

template <class RendererT>
class ModelMgr
{
public:

    class MeshIterator
    {
        friend class ModelMgr<RendererT>;
    public:
        const MeshIterator& operator++()
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
        
        bool operator!=(const MeshIterator & rhs) const
        {
            return mMeshListIterator->pModelInstance != rhs.mMeshListIterator->pModelInstance;
        }
            
        const MaterialMeshInstance & operator*() const
        {
            return *mMeshListIterator;
        }

    private:
        MeshIterator(const ShaderModelMap::const_iterator & shaderModelIterator,
                     const ModelMeshMap::const_iterator & modelMeshIterator,
                     const MeshList::const_iterator & meshIterator)
        {
            mShaderModelIterator = shaderModelIterator;
            mModelMeshIterator = modelMeshIterator;
            mMeshListIterator = meshIterator;
        }

        ShaderModelMap::const_iterator mShaderModelIterator;
        ModelMeshMap::const_iterator mModelMeshIterator;
        MeshList::const_iterator mMeshListIterator;
    };

    //--------------------------------------------------------------------------

    friend class MeshIterator;

    ModelMgr(RendererT & renderer)
      : mRenderer(renderer)
    {
        // Place a dummy entry to ShaderMeshInstanceMap.
        // This simplifies our iterators knowing that there is always at
        // least this entry. Mesh list will remain empty, so impact
        // to iteration will be minimal.
        mShaderModelMap[-1][-1] = MeshList();
    }

    void fin()
    {
        // LORRTODO - delete all models and materials not managed by asset mgr
    }

    // Iterate meshes sorted by material/vertex type
    MeshIterator begin()
    {
        ShaderModelMap::const_iterator shaderModelIt = mShaderModelMap.begin();
        ModelMeshMap::const_iterator modelMeshIt = shaderModelIt->second.begin();
    
        return MeshIterator(shaderModelIt,
                            modelMeshIt,
                            modelMeshIt->second.begin());
    }

    MeshIterator end()
    {
        ShaderModelMap::const_iterator shaderModelIt = mShaderModelMap.end();
        --shaderModelIt;

        ModelMeshMap::const_iterator modelMeshIt = shaderModelIt->second.end();
        --modelMeshIt;

        return ModelMgr<RendererT>::MeshIterator(shaderModelIt,
                                                 modelMeshIt,
                                                 modelMeshIt->second.end());
    }

    // Once inserted, don't delete.  To delete objects, send
    // appropriate delete message to renderer.
    void insertModelInstance(model_instance_id instanceId,
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

            // Load material and mesh into GPU through renderer
            if (matMesh.mesh().rendererVertsId() == -1)
            {
                ASSERT(matMesh.mesh().rendererPrimsId() == -1); // Both should be -1 as they are initialized tegether
                mRenderer.loadMaterialMesh(matMesh);
            }
        }
    }
    
    void removeModelInstance(model_instance_id instanceId)
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

private:
    class ModelDeleter
    {
    public:
        ModelDeleter(bool isAssetManaged)
          : mIsAssetManaged(isAssetManaged) {}
        
        void operator()(Model * pObj)
        {
            if (mIsAssetManaged)
            {
                PANIC("LORRTODO - Send a message to asset manager to delete this resource");
            }
            else
            {
                GDELETE(pObj);
            }
        }
    private:
        bool mIsAssetManaged;
    };

    class MaterialDeleter
    {
    public:
        MaterialDeleter(bool isAssetManaged)
          : mIsAssetManaged(isAssetManaged) {}
        
        void operator()(Material * pObj)
        {
            if (mIsAssetManaged)
            {
                PANIC("LORRTODO - Send a message to asset manager to delete this resource");
            }
            else
            {
                GDELETE(pObj);
            }
        }
    private:
        bool mIsAssetManaged;
    };


    // Store materials based on material_id.
    // This struct controls deletion of materials using ref counting.
    typedef Map<kMEM_Model,
                material_id,
                RefCounted<Material, MaterialDeleter>> MaterialMap;

    // Store models based on model_id.
    // This struct controls deletion of models using ref counting.
    typedef Map<kMEM_Model,
                model_id,
                RefCounted<Model, ModelDeleter>> ModelMap;

    // Instances of models (a model and transform)
    typedef Map<kMEM_Model,
                model_instance_id,
                ModelInstance> ModelInstanceMap;


    void insertMaterial(Material * pMaterial,
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
    
    void removeMaterial(Material * pMaterial)
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
    
    RendererT & mRenderer;

    // Ref counted models and materials
    MaterialMap mMaterialMap;
    ModelMap mModelMap;
    ModelInstanceMap mModelInstanceMap;

    // Meshes sorted by material. Iterate this with begin and end.
    ShaderModelMap mShaderModelMap;
};



} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MODELMGR_H
