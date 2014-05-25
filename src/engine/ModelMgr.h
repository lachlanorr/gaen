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

class ModelMgr
{
public:
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


    typedef List<kMEM_Model,
                 MaterialMeshInstance> MeshList;
    typedef Map<kMEM_Model,
                model_instance_id,
                MeshList> ModelMeshMap;
    typedef Map<kMEM_Model,
                shader_hash,
                ModelMeshMap> ShaderModelMap;


    //--------------------------------------------------------------------------
    class MeshIterator
    {
        friend class ModelMgr;
    public:
        const MeshIterator& operator++();
        bool operator!=(const MeshIterator & rhs) const;
        const MaterialMeshInstance & operator*() const;

    private:
        MeshIterator(const ShaderModelMap::const_iterator & shaderModelIterator,
                     const ModelMeshMap::const_iterator & modelMeshIterator,
                     const MeshList::const_iterator & meshIterator);

        ShaderModelMap::const_iterator mShaderModelIterator;
        ModelMeshMap::const_iterator mModelMeshIterator;
        MeshList::const_iterator mMeshIterator;
        
    };
    friend class MeshIterator;
    //--------------------------------------------------------------------------

    ModelMgr();

    void fin();

    // Iterate meshes sorted by material/vertex type
    MeshIterator begin();
    MeshIterator end();

    // Once inserted, don't delete.  To delete objects, send
    // appropriate delete message to renderer.
    void insertModelInstance(model_instance_id instanceId,
                             Model * pModel,
                             const Mat34 & worldTransform,
                             bool isAssetManaged);
    void removeModelInstance(model_instance_id instanceId);

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
                        bool isAssetManaged);
    void removeMaterial(Material * pMaterial);
    

    // Ref counted models and materials
    MaterialMap mMaterialMap;
    ModelMap mModelMap;
    ModelInstanceMap mModelInstanceMap;

    // Meshes sorted by material. Iterate this with begin and end.
    ShaderModelMap mShaderModelMap;
};



} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MODELMGR_H
