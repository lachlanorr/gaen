//------------------------------------------------------------------------------
// shapes.cpp - Routines to create various geometrical shapes
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

#include "engine/shapes.h"

namespace gaen
{

//------------------------------------------------------------------------------
// ShapeBuilder
//------------------------------------------------------------------------------
ShapeBuilder::ShapeBuilder(Mesh * pMesh)
  : mMesh(*pMesh)
{
    if (mMesh.vertexType() != kVERT_PosNorm)
        PANIC("ShapeBuilder only builds meshes with vertices of type kVERT_PosNorm");

    if (mMesh.indexType() != kIND_Triangle)
        PANIC("ShapeBuilder only builds meshes with indices of type kIND_Triangle");
}


void ShapeBuilder::pushTri(const Vec3 & p0,
                           const Vec3 & p1,
                           const Vec3 & p2)
{
    if (mCurrVertex + 3 > mMesh.vertexCount())
        PANIC("Vertex array overrun during pushTri");
    if (mCurrIndex + 1 > mMesh.indexCount())
        PANIC("Index array overrun during pushTri");

    VertexPosNorm * pVert = mMesh;
    pVert += mCurrVertex;

    IndexTriangle * pInd = mMesh;
    IndexTriangle & ind = pInd[mCurrIndex];

    Vec3 vecNorm = triNormal(p0, p1, p2);

    pVert[0].position = p0;
    pVert[1].position = p1;
    pVert[2].position = p2;

    pVert[0].normal = vecNorm;
    pVert[1].normal = vecNorm;
    pVert[2].normal = vecNorm;
    
    ind.p0 = mCurrVertex;
    ind.p1 = mCurrVertex + 1;
    ind.p2 = mCurrVertex + 2;

    mCurrVertex += 3;
    mCurrIndex += 1;
}

void ShapeBuilder::pushTri(const Vec3 * pPoints)
{
    pushTri(pPoints[0],
            pPoints[1],
            pPoints[2]);
}
        
void ShapeBuilder::pushQuad(const Vec3 & p0,
                            const Vec3 & p1,
                            const Vec3 & p2,
                            const Vec3 & p3)
{
    if (mCurrVertex + 4 > mMesh.vertexCount())
        PANIC("Vertex array overrun during pushQuad");
    if (mCurrIndex + 2 > mMesh.indexCount())
        PANIC("Index array overrun during pushQuad");

    pushTri(p0, p1, p2);
    pushTri(p3, p0, p2);
}

void ShapeBuilder::pushQuad(const Vec3 * pPoints)
{
    pushQuad(pPoints[0],
             pPoints[1],
             pPoints[2],
             pPoints[3]);
}


void ShapeBuilder::pushMesh(const Mesh & mesh)
{
    if (mesh.vertexType() != kVERT_PosNorm)
        PANIC("ShapeBuilder only appends meshes with vertices of type kVERT_PosNorm");
    if (mesh.indexType() != kIND_Triangle)
        PANIC("ShapeBuilder only appends meshes with indices of type kIND_Triangle");

    if (mCurrVertex + mesh.vertexCount() >= mMesh.vertexCount())
        PANIC("Vertex array overrun during pushMesh");
    if (mCurrIndex + mesh.indexCount() >= mMesh.indexCount())
        PANIC("Index array overrun during pushMesh");

    VertexPosNorm * pVert = mMesh;
    pVert += mCurrVertex;
    
    const VertexPosNorm * pMeshVert = mesh;
    for (u32 i = 0; i < mesh.vertexCount(); ++i)
    {
        *pVert++ = *pMeshVert++;
    }

    IndexTriangle * pInd = mMesh;
    pInd += mCurrIndex;

    const IndexTriangle * pMeshInd = mesh;
    for (u32 i = 0; i < mesh.indexCount(); ++i)
    {
        pInd[0].p0 = pMeshInd[0].p0 + mCurrIndex;
        pInd[0].p1 = pMeshInd[0].p1 + mCurrIndex;
        pInd[0].p2 = pMeshInd[0].p2 + mCurrIndex;
        pInd++;
        pMeshInd++;
    }

    mCurrVertex += mesh.vertexCount();
    mCurrIndex += mesh.indexCount();
}
//------------------------------------------------------------------------------
// ShapeBuilder (END)
//------------------------------------------------------------------------------

Mesh * buildTriMesh(f32 width, f32 height)
{
    Mesh * pMesh = Mesh::create(kVERT_PosNorm, 3, kIND_Triangle, 1);
    ShapeBuilder sb(pMesh);
    
    f32 widthHalf = width * 0.5f;
    f32 heightHalf = height * 0.5f;

    Vec3 p0(0.0f, 0.0f, heightHalf);
    Vec3 p1(-widthHalf, 0.0f, -heightHalf);
    Vec3 p2(widthHalf, 0.0f, -heightHalf);
    

    sb.pushTri(p0, p1, p2);
    
    return pMesh;
}

Model * buildTriModel(f32 width, f32 height, Color color)
{
    Material * pMat = GNEW(kMEM_Texture, Material, color);
    Mesh * pMesh = buildTriMesh(width, height);

    Model * pModel = GNEW(kMEM_Model, Model, pMat, pMesh);

    return pModel;
}


} // namespace gaen

