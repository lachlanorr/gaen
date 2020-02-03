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
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/mat3.h"

#include "core/Vector.h"
#include "core/hashing.h"
#include "hashes/hashes.h"

#include "assets/file_utils.h"
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
    addRawExt(kExtPly);
    addRawExt(kExtGltf);
    addCookedExtExclusive(kExtGmdl);
}

static VertType choose_vert_type(aiMesh* pMesh, CookInfo * pCookInfo)
{
    if (pMesh->HasBones())
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
    else if (pMesh->HasVertexColors(0))
    {
        PANIC_IF(!pMesh->HasPositions() || !pMesh->HasNormals(), "Mesh with vertex colors missing other required characteristics, %s", pCookInfo->rawPath().c_str());
        return kVERT_PosNormCol;
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

static void read_bones(Vector<kMEM_Chef, Bone> & boneVec, const rapidjson::Value & bones)
{
    boneVec.reserve(bones.Size());
    for (u32 i = 0; i < bones.Size(); ++i)
    {
        const rapidjson::Value & b = bones[i];
        mat43 transform(vec3(b["transform"][0][0].GetFloat(), b["transform"][0][1].GetFloat(), b["transform"][0][2].GetFloat()),
                        vec3(b["transform"][1][0].GetFloat(), b["transform"][1][1].GetFloat(), b["transform"][1][2].GetFloat()),
                        vec3(b["transform"][2][0].GetFloat(), b["transform"][2][1].GetFloat(), b["transform"][2][2].GetFloat()),
                        vec3(b["transform"][3][0].GetFloat(), b["transform"][3][1].GetFloat(), b["transform"][3][2].GetFloat()));
        boneVec.emplace_back(HASH::hash_func(b["name"].GetString()),
                             b["parent"].IsNull() ? 0 : HASH::hash_func(b["parent"].GetString()),
                             transform);
    }
}

void Model::read_skl(Skeleton & skel, const char * path)
{
    ASSERT(skel.bones.size() == 0);

    FileReader rdr(path);
    Scoped_GFREE<char> jsonStr((char*)GALLOC(kMEM_Chef, rdr.size()+1)); // +1 for null we'll add to end
    rdr.read(jsonStr.get(), rdr.size());
    jsonStr.get()[rdr.size()] = '\0';
    rdr.ifs.close();

    skel.jsonDoc.Parse(jsonStr.get());
    if (skel.jsonDoc.HasMember("bones"))
        read_bones(skel.bones, skel.jsonDoc["bones"]);
    if (skel.jsonDoc.HasMember("hardpoints"))
        read_bones(skel.hardpoints, skel.jsonDoc["hardpoints"]);
}

u32 Model::bone_id(const Vector<kMEM_Chef, Bone> & bones, const char * name)
{
    u32 nameHash = HASH::hash_func(name);
    for (u32 i = 0; i < bones.size(); ++i)
    {
        if (bones[i].nameHash == nameHash)
        {
            return i;
        }
    }
    PANIC("Bone %s not found in skeleton", name);
    return 0;
}

void Model::cook(CookInfo * pCookInfo) const
{
    u32 aiFlags = aiProcess_Triangulate |
                  aiProcess_SortByPType |
                  aiProcess_GenNormals |
                  aiProcess_ImproveCacheLocality |
                  aiProcess_GenBoundingBoxes;

    // Our ply exporter from houdini leaves the default cw windings,
    // and the engin needs ccw.
    ChefString ext = get_ext(pCookInfo->rawPath().c_str());
    if (ext == "ply")
        aiFlags |= aiProcess_FlipWindingOrder;

    const struct aiScene * pScene = aiImportFile(pCookInfo->rawPath().c_str(),
                                                 aiFlags);

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
            textures.push_back(Image::load_png(fullTexDiffusePath.c_str(), Image::reference_path_hash(pCookInfo->chef(), fullTexDiffusePath)));
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
            vertType = choose_vert_type(pScene->mMeshes[i], pCookInfo);
        else
        {
            VertType meshVertType = choose_vert_type(pScene->mMeshes[i], pCookInfo);
            PANIC_IF(meshVertType != vertType, "Incompatible vert types in same model, %s", pCookInfo->rawPath().c_str());
        }
    }

    // check for voxel skeleton
    Skeleton skel;
    ChefString sklpath = pCookInfo->rawPath();
    change_ext(sklpath, ChefString("skl"));
    if (file_exists(sklpath.c_str()))
    {
        pCookInfo->recordDependency(get_filename(sklpath));
        read_skl(skel, sklpath.c_str());
        if (vertType == kVERT_PosNormUv && skel.bones.size() > 0)
        {
            vertType = kVERT_PosNormUvBone;
        }
    }

    Gmat * pMat = nullptr;
    if (textures.size() > 0)
    {
        u32 shaderHash = 0;
        if (pCookInfo->fullRecipe().hasKey("shader"))
        {
            shaderHash = gaen_hash(pCookInfo->fullRecipe().get("shader"));
        }
        else if (vertType == kVERT_PosNormUvBone)
        {
            shaderHash = HASH::voxchar;
        }
        else if (vertType == kVERT_PosNormCol)
        {
            shaderHash = HASH::voxvertcol;
        }
        else
        {
            shaderHash = HASH::voxprop;
        }

        pMat = Gmat::create(textures, shaderHash); // LORRTODO: allow shader specification in the .rcp file
    }

    Gmdl * pGmdl = Gmdl::create(vertType, vertCount, kPRIM_Triangle, triCount, (u32)skel.bones.size(), (u32)skel.hardpoints.size(), pMat);

    PANIC_IF(!pGmdl, "Failure in Gmdl::create, %s", pCookInfo->rawPath().c_str());

    // copy bones into gmdl
    Bone * pBones = nullptr;
    if (skel.bones.size() > 0)
    {
        pBones = pGmdl->bones();
        memcpy(pBones, skel.bones.data(), sizeof(Bone) * skel.bones.size());
    }
    if (skel.hardpoints.size() > 0)
    {
        Hardpoint * pHardpoints = pGmdl->hardpoints();
        memcpy(pHardpoints, skel.hardpoints.data(), sizeof(Hardpoint) * skel.hardpoints.size());
    }

    f32 * pVert = pGmdl->verts();
    u32 vertIdxOffset = 0;
    PrimTriangle * pTri = *pGmdl;
    vec3 mins(std::numeric_limits<f32>::max()), maxes(std::numeric_limits<f32>::lowest());

    for (u32 i = 0; i < pScene->mNumMeshes; ++i)
    {
        aiMesh * pAiMesh = pScene->mMeshes[i];

        if (pAiMesh->mName.data == strstr(pAiMesh->mName.data, "col_"))
        {
            continue;
        }

        PANIC_IF(vertType == kVERT_PosNormCol && pAiMesh->GetNumColorChannels() != 1, "Color channels not equal to 1 on a vert color imported model: %s", pCookInfo->rawPath().c_str());            

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

            mins = min(mins, pVertPos->position);
            maxes = max(maxes, pVertPos->position);

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
                pVertPosNormUv->uv = vec2(pAiMesh->mTextureCoords[0][v].x, pAiMesh->mTextureCoords[0][v].y);
            }

            if (pGmdl->hasVertBone())
            {
                ASSERT(pBones);
                VertPosNormUvBone * pVertPosNormUvBone = (VertPosNormUvBone*)pVert;
                pVertPosNormUvBone->boneId = bone_id(skel.bones, pAiMesh->mName.C_Str());
                // adjust vert positions based on bone transforms
                mat43 invTrans = ~pBones[pVertPosNormUvBone->boneId].transform;
                pVertPosNormUvBone->position = invTrans * pVertPosNormUvBone->position;
                mat3 invRot(invTrans);
                pVertPosNormUvBone->normal = invRot * pVertPosNormUvBone->normal;
            }

            if (pGmdl->hasVertColor())
            {
                VertPosNormCol* pVertPosNormCol = (VertPosNormCol*)pVert;
                Color col;
                col.setrf(pAiMesh->mColors[0][v].r);
                col.setgf(pAiMesh->mColors[0][v].g);
                col.setbf(pAiMesh->mColors[0][v].b);
                col.setaf(pAiMesh->mColors[0][v].a);
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

    pGmdl->center() = (mins + maxes) * 0.5f;
    pGmdl->halfExtents() = (maxes - mins) * 0.5f;

    pCookInfo->setCookedBuffer(pGmdl);
}

}
} // namespace gaen


