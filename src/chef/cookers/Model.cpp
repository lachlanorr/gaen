//------------------------------------------------------------------------------
// Model.cpp - Model cooker
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

#include "chef/stdafx.h"

#include "assets/file_utils.h"
#include "assets/Config.h"
#include "assets/Gmdl.h"

#include "chef/cooker_utils.h"
#include "chef/CookInfo.h"
#include "chef/cookers/Model.h"

namespace gaen
{
namespace cookers
{

Model::Model()
{
    setVersion(1);
    addRawExt(kExtObj);
//    addRawExt(kExtPly);
//    addRawExt(kExtFbx);
    addCookedExtExclusive(kExtGmdl);
}

void Model::cook(CookInfo * pCookInfo) const
{
    const struct aiScene * pScene = aiImportFile(pCookInfo->rawPath().c_str(),
                                                 aiProcess_Triangulate |
                                                 aiProcess_SortByPType |
                                                 aiProcess_GenNormals |
                                                 aiProcess_ImproveCacheLocality);

    PANIC_IF(!pScene, "Failure in aiImportFile, %s", pCookInfo->rawPath().c_str());

    // We only support one vertex type currently, kVERT_PosNormCol
    u32 vertCount = 0;
    u32 triCount = 0;

    for (u32 i = 0; i < pScene->mNumMeshes; ++i)
    {
        vertCount += pScene->mMeshes[i]->mNumVertices;
        triCount += pScene->mMeshes[i]->mNumFaces;
    }

    Gmdl * pGmdl = Gmdl::create(kVERT_PosNormCol, vertCount, kPRIM_Triangle, triCount);

    PANIC_IF(!pGmdl, "Failure in Gmdl::create, %s", pCookInfo->rawPath().c_str());

    VertPosNormCol* pVert = *pGmdl;
    u32 vertIdxOffset = 0;
    PrimTriangle * pTri = *pGmdl;
    for (u32 i = 0; i < pScene->mNumMeshes; ++i)
    {
        aiMesh * pAiMesh = pScene->mMeshes[i];
        aiMaterial * pAiMaterial = pScene->mMaterials[pAiMesh->mMaterialIndex];

        for (u32 v = 0; v < pAiMesh->mNumVertices; ++v)
        {
            pVert[v].position.x = pAiMesh->mVertices[v].x;
            pVert[v].position.y = pAiMesh->mVertices[v].y;
            pVert[v].position.z = pAiMesh->mVertices[v].z;

            pVert[v].normal.x = pAiMesh->mNormals[v].x;
            pVert[v].normal.y = pAiMesh->mNormals[v].y;
            pVert[v].normal.z = pAiMesh->mNormals[v].z;

            pVert[v].color = Color(255, 0, 255, 255);
        }

        for (u32 t = 0; t < pAiMesh->mNumFaces; ++t)
        {
            pTri[t].p0 = pAiMesh->mFaces[t].mIndices[0] + vertIdxOffset;
            pTri[t].p1 = pAiMesh->mFaces[t].mIndices[1] + vertIdxOffset;
            pTri[t].p2 = pAiMesh->mFaces[t].mIndices[2] + vertIdxOffset;
        }

        pVert += pAiMesh->mNumVertices;
        vertIdxOffset += pAiMesh->mNumVertices;
        pTri += pAiMesh->mNumFaces;
    }

    pCookInfo->setCookedBuffer(pGmdl);
}

}
} // namespace gaen


