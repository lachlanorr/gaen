//------------------------------------------------------------------------------
// Model.cpp - Model cooker
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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

#include "chef/stdafx.h"

#include "math/common.h"
#include "math/vec3.h"

#include "core/Vector.h"

#include "assets/file_utils.h"
#include "assets/Config.h"
#include "assets/Gmdl.h"
#include "assets/Gmat.h"

#include "chef/cooker_utils.h"
#include "chef/CookInfo.h"
#include "chef/Chef.h"
#include "chef/cookers/Image.h"
#include "chef/cookers/Model.h"

namespace gaen
{
namespace cookers
{

Model::Model()
{
    setVersion(8);
    addRawExt(kExtObj);
    addRawExt(kExtGltf);
    addCookedExtExclusive(kExtGmdl);
}

static VertType choose_vert_type(aiMesh* pMesh, CookInfo * pCookInfo, bool legacyObjWithColor)
{
    if (legacyObjWithColor)
    {
        return kVERT_PosNormCol;
    }
    else if (pMesh->HasBones())
    {
        if (pMesh->mNumBones == 1)
        {
            PANIC_IF(!pMesh->HasPositions() || !pMesh->HasNormals() || !pMesh->HasTextureCoords(0), "Mesh with bones missing other required characteristics, %s", pCookInfo->rawPath().c_str());
            return kVERT_PosNormUvBone;
        }
        else
        {
            PANIC_IF(!pMesh->HasPositions() || !pMesh->HasNormals() || !pMesh->HasTextureCoords(0) || !pMesh->HasTangentsAndBitangents(), "Mesh with bones missing other required characteristics, %s", pCookInfo->rawPath().c_str());
            PANIC_IF(pMesh->mNumBones <= 1, "No bones specified, %s", pCookInfo->rawPath().c_str());
            return kVERT_PosNormUvTanBones;
        }
    }
    else if (pMesh->HasTangentsAndBitangents())
    {
        PANIC_IF(!pMesh->HasPositions() || !pMesh->HasNormals() || !pMesh->HasTextureCoords(0), "Mesh with tangents missing other required characteristics, %s", pCookInfo->rawPath().c_str());
        return kVERT_PosNormUvTan;
    }
    else if (pMesh->HasTextureCoords(0))
    {
        PANIC_IF(!pMesh->HasPositions() || !pMesh->HasNormals(), "Mesh with uvs missing other required characteristics, %s", pCookInfo->rawPath().c_str());
        return kVERT_PosNormUv;
    }
    else if (pMesh->HasNormals())
    {
        PANIC_IF(!pMesh->HasPositions(), "Mesh with normals missing other required characteristics, %s", pCookInfo->rawPath().c_str());
        return kVERT_PosNorm;
    }
    else if (pMesh->HasPositions())
    {
        return kVERT_Pos;
    }
    PANIC("Unable to determine vertex type, %s", pCookInfo->rawPath().c_str());
    return kVERT_Unknown;
}

void Model::cook(CookInfo * pCookInfo) const
{
    const struct aiScene * pScene = aiImportFile(pCookInfo->rawPath().c_str(),
                                                 aiProcess_Triangulate |
                                                 aiProcess_SortByPType |
                                                 aiProcess_GenNormals |
                                                 aiProcess_ImproveCacheLocality |
                                                 aiProcess_GenBoundingBoxes);

    PANIC_IF(!pScene, "Failure in aiImportFile, %s", pCookInfo->rawPath().c_str());

    Vector<kMEM_Chef, Gimg*> textures;
    textures.reserve(kTXTY_COUNT);

    ChefString texDiffusePath;

    for (u32 i = 0; i < pScene->mNumMaterials; ++i)
    {
        if (pScene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;
            pScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            texDiffusePath.assign(path.C_Str());
            normalize_path(texDiffusePath);
            pCookInfo->recordDependency(texDiffusePath.c_str());

            ChefString fullTexDiffusePath = pCookInfo->chef().getRelativeDependencyRawPath(pCookInfo->rawPath(), texDiffusePath);
            textures.push_back(Image::load_png(fullTexDiffusePath.c_str()));
        }
    }

    bool legacyObjWithColor = false;
    if (0 == strcmp(kExtObj, get_ext(pCookInfo->rawPath().c_str())))
    {
        static const ChefString kMtlExt = "mtl";
        ChefString mtlPath = pCookInfo->rawPath();
        change_ext<ChefString>(mtlPath, kMtlExt);
        if (file_exists(mtlPath.c_str()))
        {
            pCookInfo->recordDependency(get_filename(mtlPath.c_str()));

            if (texDiffusePath.empty())
                legacyObjWithColor = true;
        }
    }

    u32 vertCount = 0;
    u32 triCount = 0;

    VertType vertType = kVERT_Unknown;

    for (u32 i = 0; i < pScene->mNumMeshes; ++i)
    {
        vertCount += pScene->mMeshes[i]->mNumVertices;
        triCount += pScene->mMeshes[i]->mNumFaces;
        if (vertType == kVERT_Unknown)
            vertType = choose_vert_type(pScene->mMeshes[i], pCookInfo, legacyObjWithColor);
        else
        {
            VertType meshVertType = choose_vert_type(pScene->mMeshes[i], pCookInfo, legacyObjWithColor);
            PANIC_IF(meshVertType != vertType, "Incompatible vert types in same model, %s", pCookInfo->rawPath().c_str());
        }
    }

    Gmat * pMat = nullptr;
    if (textures.size() > 0)
    {
        pMat = Gmat::create(textures);
    }

    Gmdl * pGmdl = Gmdl::create(vertType, vertCount, kPRIM_Triangle, triCount, pMat);

    PANIC_IF(!pGmdl, "Failure in Gmdl::create, %s", pCookInfo->rawPath().c_str());

    f32 * pVert = pGmdl->verts();
    u32 vertIdxOffset = 0;
    PrimTriangle * pTri = *pGmdl;
    vec3 & halfExtents = pGmdl->halfExtents();

    for (u32 i = 0; i < pScene->mNumMeshes; ++i)
    {
        aiMesh * pAiMesh = pScene->mMeshes[i];

        if (pAiMesh->mName.data == strstr(pAiMesh->mName.data, "col_"))
        {
            continue;
        }

        // Materials like this are only used for legacy obj colored verts
        aiMaterial * pAiMaterial = pScene->mMaterials[pAiMesh->mMaterialIndex];
        aiColor3D diffuse(1.0f, 1.0f, 1.0f);
        aiReturn ret = pAiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

        // Check for .rcp scale
        f32 scale = 1.0f;
        if (pCookInfo->fullRecipe().hasKey("scale"))
            scale = pCookInfo->fullRecipe().getFloat("scale");

        for (u32 v = 0; v < pAiMesh->mNumVertices; ++v)
        {
            VertPos * pVertPos = (VertPos*)pVert;
            pVertPos->position.x = pAiMesh->mVertices[v].x * scale;
            pVertPos->position.y = pAiMesh->mVertices[v].y * scale;
            pVertPos->position.z = pAiMesh->mVertices[v].z * scale;

            // Calculate extents as we are iterating verts
            halfExtents.x = max(halfExtents.x, abs(pVertPos->position.x));
            halfExtents.y = max(halfExtents.y, abs(pVertPos->position.y));
            halfExtents.z = max(halfExtents.z, abs(pVertPos->position.z));

            if (pGmdl->hasVertNormal())
            {
                VertPosNorm * pVertPosNorm = (VertPosNorm*)pVert;
                pVertPosNorm->normal.x = pAiMesh->mNormals[v].x;
                pVertPosNorm->normal.y = pAiMesh->mNormals[v].y;
                pVertPosNorm->normal.z = pAiMesh->mNormals[v].z;
            }

            if (pGmdl->hasVertUv())
            {
                VertPosNormUv * pVertPosNormUv = (VertPosNormUv*)pVert;
                pVertPosNormUv->u = pAiMesh->mTextureCoords[0]->x;
                pVertPosNormUv->v = pAiMesh->mTextureCoords[0]->y;
            }

            if (pGmdl->hasVertColor())
            {
                VertPosNormCol* pVertPosNormCol = (VertPosNormCol*)pVert;
                Color col;
                col.setrf(diffuse.r);
                col.setgf(diffuse.g);
                col.setbf(diffuse.b);
                col.seta(255);
                pVertPosNormCol->color = col;
            }

            pVert += pGmdl->vertStride() / sizeof(f32);
        }

        PANIC_IF(pAiMesh->mNumFaces <= 0, "No faces in mesh, %s", pCookInfo->rawPath().c_str());

        for (u32 t = 0; t < pAiMesh->mNumFaces; ++t)
        {
            PANIC_IF(pAiMesh->mFaces->mNumIndices != 3, "Non triangularized mesh, %s", pCookInfo->rawPath().c_str());
            pTri[t].p0 = pAiMesh->mFaces[t].mIndices[0] + vertIdxOffset;
            pTri[t].p1 = pAiMesh->mFaces[t].mIndices[1] + vertIdxOffset;
            pTri[t].p2 = pAiMesh->mFaces[t].mIndices[2] + vertIdxOffset;
        }

        vertIdxOffset += pAiMesh->mNumVertices;
        pTri += pAiMesh->mNumFaces;
    }

    pCookInfo->setCookedBuffer(pGmdl);
}

}
} // namespace gaen


