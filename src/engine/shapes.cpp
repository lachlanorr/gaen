//------------------------------------------------------------------------------
// shapes.h - Routines to create various geometrical shapes
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

void MeshBuilder::pushTri(const Vec3 & p0,
                          const Vec3 & p1,
                          const Vec3 & p2)
{
    if (mCurrVertex + 3 >= mMesh.vertexCount())
        PANIC("Vertex array overrun during pushTri");
    if (mCurrIndex + 1 >= mMesh.indexCount())
        PANIC("Index array overrun during pushTri");

    ShapeMesh::vertex_type * pVert = mMesh.vertices() + mCurrVertex;
    ShapeMesh::index_type & idx = mMesh.indices()[mCurrIndex];

    Vec3 vecNorm = triNormal(p0, p1, p2);

    pVert[0].position = p0;
    pVert[1].position = p1;
    pVert[2].position = p2;

    pVert[0].normal = vecNorm;
    pVert[1].normal = vecNorm;
    pVert[2].normal = vecNorm;
    
    idx.p0 = mCurrVertex;
    idx.p1 = mCurrVertex + 1;
    idx.p2 = mCurrVertex + 2;

    mCurrVertex += 3;
    mCurrIndex += 1;
}

void MeshBuilder::pushTri(const Vec3 * pPoints)
{
    pushTri(pPoints[0],
            pPoints[1],
            pPoints[2]);
}
        
void MeshBuilder::pushQuad(const Vec3 & p0,
                           const Vec3 & p1,
                           const Vec3 & p2,
                           const Vec3 & p3)
{
    if (mCurrVertex + 4 >= mMesh.vertexCount())
        PANIC("Vertex array overrun during pushQuad");
    if (mCurrIndex + 2 >= mMesh.indexCount())
        PANIC("Index array overrun during pushQuad");

    pushTri(p0, p1, p2);
    pushTri(p3, p0, p2);
}

void MeshBuilder::pushQuad(const Vec3 * pPoints)
{
    pushQuad(pPoints[0],
             pPoints[1],
             pPoints[2],
             pPoints[3]);
}


void MeshBuilder::pushMesh(const ShapeMesh & mesh)
{
    if (mCurrVertex + mesh.vertexCount() >= mMesh.vertexCount())
        PANIC("Vertex array overrun during pushMesh");
    if (mCurrIndex + mesh.indexCount() >= mMesh.indexCount())
        PANIC("Index array overrun during pushMesh");

    ShapeMesh::vertex_type * pVert = mMesh.vertices() + mCurrVertex;
    const ShapeMesh::vertex_type * pMeshVert = mesh.vertices();
    for (u32 i = 0; i < mesh.vertexCount(); ++i)
    {
        *pVert++ = *pMeshVert++;
    }


    ShapeMesh::index_type * pIdx = mMesh.indices() + mCurrIndex;
    const ShapeMesh::index_type * pMeshIdx = mesh.indices();
    for (u32 i = 0; i < mesh.indexCount(); ++i)
    {
        pIdx[0].p0 = pMeshIdx[0].p0 + mCurrIndex;
        pIdx[0].p1 = pMeshIdx[0].p1 + mCurrIndex;
        pIdx[0].p2 = pMeshIdx[0].p2 + mCurrIndex;
        pIdx++;
        pMeshIdx++;
    }

    mCurrVertex += mesh.vertexCount();
    mCurrIndex += mesh.indexCount();
}


} // namespace gaen

