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
    if (mMesh.vertType() != kVERT_PosNorm)
        PANIC("ShapeBuilder only builds meshes with vertices of type kVERT_PosNorm");

    if (mMesh.primType() != kPRIM_Triangle)
        PANIC("ShapeBuilder only builds meshes with indices of type kIND_Triangle");
}


void ShapeBuilder::pushTri(const Vec3 & p0,
                           const Vec3 & p1,
                           const Vec3 & p2)
{
    if (mCurrVertex + 3 > mMesh.vertCount())
        PANIC("Vertex array overrun during pushTri");
    if (mCurrPrimitive + 1 > mMesh.primCount())
        PANIC("Index array overrun during pushTri");

    VertPosNorm * pVert = mMesh;
    pVert += mCurrVertex;

    PrimTriangle * pTris = mMesh;
    PrimTriangle & tri = pTris[mCurrPrimitive];

    Vec3 vecNorm = triNormal(p0, p1, p2);

    pVert[0].position = p0;
    pVert[0].normal = vecNorm;

    pVert[1].position = p1;
    pVert[1].normal = vecNorm;

    pVert[2].position = p2;
    pVert[2].normal = vecNorm;
    
    tri.p0 = mCurrVertex + 0;
    tri.p1 = mCurrVertex + 1;
    tri.p2 = mCurrVertex + 2;

    mCurrVertex += 3;
    mCurrPrimitive += 1;
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
    if (mCurrVertex + 4 > mMesh.vertCount())
        PANIC("Vertex array overrun during pushQuad");
    if (mCurrPrimitive + 2 > mMesh.primCount())
        PANIC("Index array overrun during pushQuad");

    VertPosNorm * pVert = mMesh;
    pVert += mCurrVertex;

    PrimTriangle * pTris = mMesh;
    PrimTriangle & tri0 = pTris[mCurrPrimitive];
    PrimTriangle & tri1 = pTris[mCurrPrimitive+1];

    Vec3 vecNorm = triNormal(p0, p1, p2);
    Vec3 vecNorm2 = triNormal(p3, p0, p2);

    ASSERT(vecNorm == vecNorm2);

    pVert[0].position = p0;
    pVert[0].normal = vecNorm;

    pVert[1].position = p1;
    pVert[1].normal = vecNorm;

    pVert[2].position = p2;
    pVert[2].normal = vecNorm;

    pVert[3].position = p3;
    pVert[3].normal = vecNorm;

    tri0.p0 = mCurrVertex + 0;
    tri0.p1 = mCurrVertex + 1;
    tri0.p2 = mCurrVertex + 2;

    tri1.p0 = mCurrVertex + 3;
    tri1.p1 = mCurrVertex + 0;
    tri1.p2 = mCurrVertex + 2;

    mCurrVertex += 4;
    mCurrPrimitive += 2;
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
    if (mesh.vertType() != kVERT_PosNorm)
        PANIC("ShapeBuilder only appends meshes with vertices of type kVERT_PosNorm");
    if (mesh.primType() != kPRIM_Triangle)
        PANIC("ShapeBuilder only appends meshes with indices of type kIND_Triangle");

    if (mCurrVertex + mesh.vertCount() >= mMesh.vertCount())
        PANIC("Vertex array overrun during pushMesh");
    if (mCurrPrimitive + mesh.primCount() >= mMesh.primCount())
        PANIC("Index array overrun during pushMesh");

    VertPosNorm * pVert = mMesh;
    pVert += mCurrVertex;
    
    const VertPosNorm * pMeshVert = mesh;
    for (u32 i = 0; i < mesh.vertCount(); ++i)
    {
        *pVert++ = *pMeshVert++;
    }

    PrimTriangle * pInd = mMesh;
    pInd += mCurrPrimitive;

    const PrimTriangle * pMeshInd = mesh;
    for (u32 i = 0; i < mesh.primCount(); ++i)
    {
        pInd[0].p0 = pMeshInd[0].p0 + mCurrPrimitive;
        pInd[0].p1 = pMeshInd[0].p1 + mCurrPrimitive;
        pInd[0].p2 = pMeshInd[0].p2 + mCurrPrimitive;
        pInd++;
        pMeshInd++;
    }

    mCurrVertex += mesh.vertCount();
    mCurrPrimitive += mesh.primCount();
}
//------------------------------------------------------------------------------
// ShapeBuilder (END)
//------------------------------------------------------------------------------

Mesh * buildTriMesh(f32 width, f32 height)
{
    Mesh * pMesh = Mesh::create(kVERT_PosNorm, 3, kPRIM_Triangle, 1);
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

Model * build_box(const Vec3 & size, Color color)
{
    Material * pMat = GNEW(kMEM_Texture, Material, color);
    Mesh * pMesh = Mesh::create(kVERT_PosNorm, 24, kPRIM_Triangle, 12);

    ShapeBuilder builder(pMesh);

    f32 xmax = size.x / 2.0f;
    f32 xmin = -xmax;
    
    f32 ymax = size.y / 2.0f;
    f32 ymin = -ymax;

    f32 zmax = size.z / 2.0f;
    f32 zmin = -zmax;

    // Top
    builder.pushQuad(Vec3(xmin, ymax, zmax), Vec3(xmin, ymin, zmax), Vec3(xmax, ymin, zmax), Vec3(xmax, ymax, zmax));

    // Front
    builder.pushQuad(Vec3(xmin, ymin, zmax), Vec3(xmin, ymin, zmin), Vec3(xmax, ymin, zmin), Vec3(xmax, ymin, zmax));

    // Bottom
    builder.pushQuad(Vec3(xmin, ymin, zmin), Vec3(xmin, ymax, zmin), Vec3(xmax, ymax, zmin), Vec3(xmax, ymin, zmin));

    // Back
    builder.pushQuad(Vec3(xmax, ymax, zmax), Vec3(xmax, ymax, zmin), Vec3(xmin, ymax, zmin), Vec3(xmin, ymax, zmax));

    // Left
    builder.pushQuad(Vec3(xmin, ymax, zmax), Vec3(xmin, ymax, zmin), Vec3(xmin, ymin, zmin), Vec3(xmin, ymin, zmax));

    // Right
    builder.pushQuad(Vec3(xmax, ymin, zmax), Vec3(xmax, ymin, zmin), Vec3(xmax, ymax, zmin), Vec3(xmax, ymax, zmax));

    Model * pModel = GNEW(kMEM_Model, Model, pMat, pMesh);
    return pModel;
}


} // namespace gaen

