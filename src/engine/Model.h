//------------------------------------------------------------------------------
// Model.h - Model, a collection of meshes/materials
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

#ifndef GAEN_ENGINE_MODEL_H
#define GAEN_ENGINE_MODEL_H

#include "core/Vector.h"

#include "engine/Mesh.h"
#include "engine/Material.h"

namespace gaen
{

typedef u32 model_id;

typedef u32 material_mesh_id;
typedef u32 shader_hash;

class Model
{
public:
    // Mesh and material
    class MaterialMesh
    {
    public:
        MaterialMesh(Model * pModel, Material * pMaterial, Mesh * pMesh);

        ~MaterialMesh();

        material_mesh_id id() const { return mId; }

        shader_hash shaderHash() const { return mShaderHash; }

        Model & model() { return *mpModel; }
        Material & material() { return *mpMaterial; }
        Mesh & mesh() { return *mpMesh; }

    private:
        shader_hash calcShaderHash();

        material_mesh_id mId;
        shader_hash mShaderHash;
        Model * mpModel;
        Material * mpMaterial;
        Mesh * mpMesh;
    };

    Model(Material * pMaterial, Mesh * pMesh, size_t meshCount=1);
    ~Model();

    model_id id() const { return mId; }
    
    typedef Vector<kMEM_Model, MaterialMesh> MaterialMeshVector;
    MaterialMeshVector::iterator begin() { return mMaterialMeshes.begin(); }
    MaterialMeshVector::iterator end() { return mMaterialMeshes.end(); }

    void reserveMeshCapacity(size_t meshCount);
    void insertMaterialMesh(Material * pMaterial, Mesh * pMesh);

    // Call this once all meshes have been pushed.  After that
    // the Model should not be modified.
    void makeReadOnly() { mIsReadOnly = true; }

private:
    model_id mId;
    bool mIsReadOnly = false;
    MaterialMeshVector mMaterialMeshes;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MODEL_H
