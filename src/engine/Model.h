//------------------------------------------------------------------------------
// Model.h - Model, a collection of gmdls/materials
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

#ifndef GAEN_ENGINE_MODEL_H
#define GAEN_ENGINE_MODEL_H

#include "core/Vector.h"

#include "assets/Gmdl.h"
#include "engine/Material.h"

namespace gaen
{

typedef u32 model_id;
typedef u32 material_gmdl_id;
typedef u64 material_gmdl_sort;

class Model
{
public:
    // Gmdl and material
    class MaterialGmdl
    {
    public:
        MaterialGmdl(Model * pModel, Material * pMaterial, Gmdl * pGmdl);

        ~MaterialGmdl();

        material_gmdl_id id() const { return mId; }

        material_gmdl_sort sortOrder() const { return mSortOrder; }

        Model & model() { return *mpModel; }
        Material & material() { return *mpMaterial; }
        Gmdl & gmdl() { return *mpGmdl; }

    private:
        material_gmdl_sort calcSortOrder();

        material_gmdl_id mId;
        material_gmdl_sort mSortOrder;

        Model * mpModel;
        Material * mpMaterial;
        Gmdl * mpGmdl;
    };

    Model(Material * pMaterial, Gmdl * pGmdl, size_t gmdlCount=1);
    ~Model();

    model_id id() const { return mId; }
    
    typedef Vector<kMEM_Model, MaterialGmdl> MaterialGmdlVector;
    MaterialGmdlVector::iterator begin() { return mMaterialGmdls.begin(); }
    MaterialGmdlVector::iterator end() { return mMaterialGmdls.end(); }

    void reserveGmdlCapacity(size_t gmdlCount);
    void insertMaterialGmdl(Material * pMaterial, Gmdl * pGmdl);

    // Call this once all gmdls have been pushed.  After that
    // the Model should not be modified.
    void makeReadOnly() { mIsReadOnly = true; }

private:
    model_id mId;
    bool mIsReadOnly = false;
    MaterialGmdlVector mMaterialGmdls;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MODEL_H
