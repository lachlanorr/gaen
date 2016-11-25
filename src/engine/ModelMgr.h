//------------------------------------------------------------------------------
// ModelMgr.h - Provides material ordered mesh iteration and model/material release semantics
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

#ifndef GAEN_ENGINE_MODELMGR_H
#define GAEN_ENGINE_MODELMGR_H

#include <glm/mat4x3.hpp>

#include "core/Map.h"
#include "core/List.h"
#include "core/RefCounted.h"

#include "engine/Model.h"
#include "engine/MessageQueue.h"
#include "engine/renderer_structs.h"

namespace gaen
{

//------------------------------------------------------------------------------

struct ModelInstance
{
    ModelInstance(task_id owner,
                  u32 uid,
                  Model * pModel,
                  const glm::mat4x3 & transform)
      : ruid(owner, uid)
      , transform(transform)
      , pModel(pModel)
    {}
        
    Ruid ruid;
    glm::mat4x3 transform;
    Model * pModel;
};
    
//------------------------------------------------------------------------------

struct MaterialGmdlInstance
{
    MaterialGmdlInstance(ModelInstance * pModelInstance,
                         Model::MaterialGmdl * pMaterialGmdl)
      : pModelInstance(pModelInstance)
      , pMaterialGmdl(pMaterialGmdl)
    {}

    ModelInstance * pModelInstance;
    Model::MaterialGmdl * pMaterialGmdl;
};

//------------------------------------------------------------------------------

typedef List<kMEM_Model,
    MaterialGmdlInstance> GmdlList;
typedef Map<kMEM_Model,
            u32, // uid
            GmdlList> ModelGmdlMap;
typedef Map<kMEM_Model,
            material_gmdl_sort,
            ModelGmdlMap> ShaderModelMap;

//------------------------------------------------------------------------------

template <class RendererT>
class ModelMgr
{
public:

    class GmdlIterator
    {
        friend class ModelMgr<RendererT>;
    public:
        const GmdlIterator& operator++()
        {
            ++mGmdlListIterator;

            if (mGmdlListIterator == mModelGmdlIterator->second.end())
            {
                ++mModelGmdlIterator;

                if (mModelGmdlIterator == mShaderModelIterator->second.end())
                {
                    ++mShaderModelIterator;
                    mModelGmdlIterator = mShaderModelIterator->second.begin();
                }

                mGmdlListIterator = mModelGmdlIterator->second.begin();
            }

            return *this;
        }
        
        bool operator!=(const GmdlIterator & rhs) const
        {
            return mGmdlListIterator->pModelInstance != rhs.mGmdlListIterator->pModelInstance;
        }
            
        const MaterialGmdlInstance & operator*() const
        {
            return *mGmdlListIterator;
        }

    private:
        GmdlIterator(const ShaderModelMap::const_iterator & shaderModelIterator,
                     const ModelGmdlMap::const_iterator & modelGmdlIterator,
                     const GmdlList::const_iterator & gmdlIterator)
        {
            mShaderModelIterator = shaderModelIterator;
            mModelGmdlIterator = modelGmdlIterator;
            mGmdlListIterator = gmdlIterator;
        }

        ShaderModelMap::const_iterator mShaderModelIterator;
        ModelGmdlMap::const_iterator mModelGmdlIterator;
        GmdlList::const_iterator mGmdlListIterator;
    };

    //--------------------------------------------------------------------------

    friend class GmdlIterator;

    ModelMgr(RendererT & renderer)
      : mRenderer(renderer)
    {
        // Place a dummy entry to ShaderGmdlInstanceMap.
        // This simplifies our iterators knowing that there is always at
        // least this entry. Gmdl list will remain empty, so impact
        // to iteration will be minimal.
        mShaderModelMap[-1][-1] = GmdlList();
    }

    void fin()
    {
        // LORRTODO - delete all models and materials not managed by asset mgr
    }

    // Iterate gmdls sorted by material/vertex type
    GmdlIterator begin()
    {
        ShaderModelMap::const_iterator shaderModelIt = mShaderModelMap.begin();
        ModelGmdlMap::const_iterator modelGmdlIt = shaderModelIt->second.begin();
    
        return GmdlIterator(shaderModelIt,
                            modelGmdlIt,
                            modelGmdlIt->second.begin());
    }

    GmdlIterator end()
    {
        ShaderModelMap::const_iterator shaderModelIt = mShaderModelMap.end();
        --shaderModelIt;

        ModelGmdlMap::const_iterator modelGmdlIt = shaderModelIt->second.end();
        --modelGmdlIt;

        return ModelMgr<RendererT>::GmdlIterator(shaderModelIt,
                                                 modelGmdlIt,
                                                 modelGmdlIt->second.end());
    }

    ModelInstance * findModelInstance(u32 uid)
    {
        ModelInstanceMap::iterator it = mModelInstanceMap.find(uid);
        if (it != mModelInstanceMap.end())
            return &it->second;
        else
            return nullptr;
    }

    // Once inserted, don't delete.  To delete objects, send
    // appropriate delete message to renderer.
    void insertModelInstance(task_id owner,
                             u32 uid,
                             Model * pModel,
                             const glm::mat4x3 & worldTransform,
                             bool isAssetManaged)
    {
        // Insert into mModelMap if necessary
        typename ModelMap::iterator modelIt = mModelMap.find(pModel->id());
        if (modelIt == mModelMap.end())
        {
            mModelMap.emplace(pModel->id(), make_ref_counted(pModel, ModelDeleter(isAssetManaged)));
        }
        else
        {
            modelIt->second.addRef();
        }

        // Insert into mModelInstanceMap
        ASSERT(mModelInstanceMap.find(uid) == mModelInstanceMap.end());
        auto empResult = mModelInstanceMap.emplace(uid, ModelInstance(owner, uid, pModel, worldTransform));
        if (!empResult.second)
            PANIC("Failed to emplace model instance");
        auto modelInstanceIt = empResult.first;

        // Insert materials
        for (Model::MaterialGmdl & matGmdl : *pModel)
        {
            insertMaterial(&matGmdl.material(), isAssetManaged);

            // Insert gmdls into mShaderModelMap
            mShaderModelMap[matGmdl.sortOrder()][uid].push_back(MaterialGmdlInstance(&modelInstanceIt->second, &matGmdl));

            // Load material and gmdl into GPU through renderer
            if (matGmdl.gmdl().rendererReserved(0) == -1)
            {
                // All should be -1 as they are initialized together
                static_assert(Gmdl::kRendererReservedCount == 4, "Unexpected kRendererReservedCount, ASSERT below needs to be updated");
                ASSERT(matGmdl.gmdl().rendererReserved(1) == -1 &&
                       matGmdl.gmdl().rendererReserved(2) == -1 &&
                       matGmdl.gmdl().rendererReserved(3) == -1);
                mRenderer.loadMaterialGmdl(matGmdl);
            }
        }
    }
    
    void removeModelInstance(task_id source, u32 uid)
    {
        // Insert into mModelInstanceMap
        auto modelInstanceIt = mModelInstanceMap.find(uid);
        if (modelInstanceIt == mModelInstanceMap.end())
        {
            PANIC("removeModelInstance of non existent material");
        }

        const ModelInstance & modelInst = modelInstanceIt->second;

        // Only remove if caller is the owner
        if (modelInst.ruid.owner != source)
        {
            // LORRTODO - change this to a non fatal error message
            PANIC("removeModelInstance attempted by non owning task");
            return;
        }

        Model * pModel = modelInst.pModel;

        // Remove materials
        for (Model::MaterialGmdl & matGmdl : *pModel)
        {
            // Delete all gmdls from mShaderModelMap for this uid
            mShaderModelMap[matGmdl.sortOrder()].erase(uid);

            removeMaterial(&matGmdl.material());
        }

        // Remove the modelInstanceIt
        mModelInstanceMap.erase(modelInstanceIt);

        // Remove from into mModelMap
        typename ModelMap::iterator modelIt = mModelMap.find(pModel->id());
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
                u32, // uid
                ModelInstance> ModelInstanceMap;


    void insertMaterial(Material * pMaterial,
                        bool isAssetManaged)
    {
        typename MaterialMap::iterator materialIt = mMaterialMap.find(pMaterial->id());

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
        typename MaterialMap::iterator materialIt = mMaterialMap.find(pMaterial->id());

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

    // Gmdls sorted by material. Iterate this with begin and end.
    ShaderModelMap mShaderModelMap;
};



} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MODELMGR_H
