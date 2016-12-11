//------------------------------------------------------------------------------
// shapes.cpp - Routines to create various geometrical shapes
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

#include "render_support/stdafx.h"

#include "engine/glm_ext.h"
#include "hashes/hashes.h"
#include "engine/entity.h"
#include "render_support/shapes.h"

namespace gaen
{

//------------------------------------------------------------------------------
// ShapeBuilder
//------------------------------------------------------------------------------
ShapeBuilder::ShapeBuilder(Gmdl * pGmdl)
  : mGmdl(*pGmdl)
{
    if (mGmdl.vertType() != kVERT_PosNormCol)
        PANIC("ShapeBuilder only builds gmdls with vertices of type kVERT_PosNormCol");

    if (mGmdl.primType() != kPRIM_Triangle)
        PANIC("ShapeBuilder only builds gmdls with indices of type kIND_Triangle");
}


void ShapeBuilder::addTri(const glm::vec3 & p0,
                          const glm::vec3 & p1,
                          const glm::vec3 & p2,
                          Color color)
{
    if (mCurrVertex + 3 > mGmdl.vertCount())
        PANIC("Vertex array overrun during pushTri");
    if (mCurrPrimitive + 1 > mGmdl.primCount())
        PANIC("Index array overrun during pushTri");

    VertPosNormCol * pVert = mGmdl;
    pVert += mCurrVertex;

    PrimTriangle * pTris = mGmdl;
    PrimTriangle & tri = pTris[mCurrPrimitive];

    glm::vec3 vecNorm = tri_normal(p0, p1, p2);

    pVert[0].position = p0;
    pVert[0].normal = vecNorm;
    pVert[0].color = color;

    pVert[1].position = p1;
    pVert[1].normal = vecNorm;
    pVert[1].color = color;

    pVert[2].position = p2;
    pVert[2].normal = vecNorm;
    pVert[2].color = color;
    
    tri.p0 = mCurrVertex + 0;
    tri.p1 = mCurrVertex + 1;
    tri.p2 = mCurrVertex + 2;

    mCurrVertex += 3;
    mCurrPrimitive += 1;
}

void ShapeBuilder::addTri(const glm::vec3 * pPoints, Color color)
{
    addTri(pPoints[0],
           pPoints[1],
           pPoints[2],
           color);
}

void ShapeBuilder::addTri(const glm::vec4 & p0,
                          const glm::vec4 & p1,
                          const glm::vec4 & p2,
                          Color color)
{
    addTri(glm::vec3(p0),
           glm::vec3(p1),
           glm::vec3(p2),
           color);
}

void ShapeBuilder::addQuad(const glm::vec3 & p0,
                           const glm::vec3 & p1,
                           const glm::vec3 & p2,
                           const glm::vec3 & p3,
                           Color color)
{
    if (mCurrVertex + 4 > mGmdl.vertCount())
        PANIC("Vertex array overrun during pushQuad");
    if (mCurrPrimitive + 2 > mGmdl.primCount())
        PANIC("Index array overrun during pushQuad");

    VertPosNormCol * pVert = mGmdl;
    pVert += mCurrVertex;

    PrimTriangle * pTris = mGmdl;
    PrimTriangle & tri0 = pTris[mCurrPrimitive];
    PrimTriangle & tri1 = pTris[mCurrPrimitive+1];

    glm::vec3 vecNorm = tri_normal(p0, p1, p2);
    glm::vec3 vecNorm2 = tri_normal(p3, p0, p2);

    //ASSERT(vecNorm == vecNorm2);

    pVert[0].position = p0;
    pVert[0].normal = vecNorm;
    pVert[0].color = color;

    pVert[1].position = p1;
    pVert[1].normal = vecNorm;
    pVert[1].color = color;

    pVert[2].position = p2;
    pVert[2].normal = vecNorm;
    pVert[2].color = color;

    pVert[3].position = p3;
    pVert[3].normal = vecNorm;
    pVert[3].color = color;

    tri0.p0 = mCurrVertex + 0;
    tri0.p1 = mCurrVertex + 1;
    tri0.p2 = mCurrVertex + 2;

    tri1.p0 = mCurrVertex + 3;
    tri1.p1 = mCurrVertex + 0;
    tri1.p2 = mCurrVertex + 2;

    mCurrVertex += 4;
    mCurrPrimitive += 2;
}

void ShapeBuilder::addQuad(const glm::vec3 * pPoints, Color color)
{
    addQuad(pPoints[0],
            pPoints[1],
            pPoints[2],
            pPoints[3],
            color);
}

void ShapeBuilder::addQuad(const glm::vec4 & p0,
                           const glm::vec4 & p1,
                           const glm::vec4 & p2,
                           const glm::vec4 & p3,
                           Color color)
{
    addQuad(glm::vec3(p0),
            glm::vec3(p1),
            glm::vec3(p2),
            glm::vec3(p3),
            color);
}

void ShapeBuilder::addGmdl(const Gmdl & gmdl)
{
    if (gmdl.vertType() != kVERT_PosNormCol)
        PANIC("ShapeBuilder only appends gmdls with vertices of type kVERT_PosNormCol");
    if (gmdl.primType() != kPRIM_Triangle)
        PANIC("ShapeBuilder only appends gmdls with indices of type kIND_Triangle");

    if (mCurrVertex + gmdl.vertCount() >= mGmdl.vertCount())
        PANIC("Vertex array overrun during pushGmdl");
    if (mCurrPrimitive + gmdl.primCount() >= mGmdl.primCount())
        PANIC("Index array overrun during pushGmdl");

    VertPosNormCol * pVert = mGmdl;
    pVert += mCurrVertex;
    
    const VertPosNormCol * pGmdlVert = gmdl;
    for (u32 i = 0; i < gmdl.vertCount(); ++i)
    {
        *pVert++ = *pGmdlVert++;
    }

    PrimTriangle * pInd = mGmdl;
    pInd += mCurrPrimitive;

    const PrimTriangle * pGmdlInd = gmdl;
    for (u32 i = 0; i < gmdl.primCount(); ++i)
    {
        pInd[0].p0 = pGmdlInd[0].p0 + mCurrPrimitive;
        pInd[0].p1 = pGmdlInd[0].p1 + mCurrPrimitive;
        pInd[0].p2 = pGmdlInd[0].p2 + mCurrPrimitive;
        pInd++;
        pGmdlInd++;
    }

    mCurrVertex += gmdl.vertCount();
    mCurrPrimitive += gmdl.primCount();
}
//------------------------------------------------------------------------------
// ShapeBuilder (END)
//------------------------------------------------------------------------------

Gmdl * build_box(const glm::vec3 & size, Color color)
{
    Gmdl * pGmdl = Gmdl::create(kVERT_PosNormCol, 24, kPRIM_Triangle, 12);

    ShapeBuilder builder(pGmdl);

    f32 xmax = size.x / 2.0f;
    f32 xmin = -xmax;
    
    f32 ymax = size.y / 2.0f;
    f32 ymin = -ymax;

    f32 zmax = size.z / 2.0f;
    f32 zmin = -zmax;

    // Front
    builder.addQuad(glm::vec3(xmin, ymax, zmax), glm::vec3(xmin, ymin, zmax), glm::vec3(xmax, ymin, zmax), glm::vec3(xmax, ymax, zmax), color);

    // Bottom
    builder.addQuad(glm::vec3(xmin, ymin, zmax), glm::vec3(xmin, ymin, zmin), glm::vec3(xmax, ymin, zmin), glm::vec3(xmax, ymin, zmax), color);

    // Back
    builder.addQuad(glm::vec3(xmin, ymin, zmin), glm::vec3(xmin, ymax, zmin), glm::vec3(xmax, ymax, zmin), glm::vec3(xmax, ymin, zmin), color);

    // Top
    builder.addQuad(glm::vec3(xmax, ymax, zmax), glm::vec3(xmax, ymax, zmin), glm::vec3(xmin, ymax, zmin), glm::vec3(xmin, ymax, zmax), color);

    // Left
    builder.addQuad(glm::vec3(xmin, ymax, zmax), glm::vec3(xmin, ymax, zmin), glm::vec3(xmin, ymin, zmin), glm::vec3(xmin, ymin, zmax), color);

    // Right
    builder.addQuad(glm::vec3(xmax, ymin, zmax), glm::vec3(xmax, ymin, zmin), glm::vec3(xmax, ymax, zmin), glm::vec3(xmax, ymax, zmax), color);

    return pGmdl;
}

// Lathe around y axis
Gmdl * lathe_points(const glm::vec4 * pPoints, u32 count, u32 slices, Color color)
{
    ASSERT(slices > 2);

    u32 triCount = (count - 3) * slices * 2;
    // If it's a closed surface, we'll not need a quad
    // on the ends of each slice.
    triCount += slices * (pPoints[0].x == 0.0f ? 1 : 2);
    triCount += slices * (pPoints[count-1].x == 0.0f ? 1 : 2);

    Gmdl * pGmdl = Gmdl::create(kVERT_PosNormCol, triCount * 3, kPRIM_Triangle, triCount);

    ShapeBuilder builder(pGmdl);

    f32 fullArc = glm::radians(360.0f);
    f32 arcPart = fullArc / slices;

    for (u32 s = 0; s < slices; ++s)
    {
        f32 arc0 = arcPart * s;
        f32 arc1 = s < (slices-1) ? arc0 + arcPart : 0.0f; // last arc1 connects back to 0 arc

        for (u32 i = 0; i < count-1; ++i)
        {
            glm::vec4 p0 = pPoints[i];
            glm::vec4 p1 = pPoints[i+1];

            glm::mat4 rotMat0 = glm::rotate(glm::mat4(1.0), arc0, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 rotMat1 = glm::rotate(glm::mat4(1.0), arc1, glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec4 p0_0 = rotMat0 * p0;
            glm::vec4 p1_0 = rotMat0 * p1;

            glm::vec4 p0_1 = rotMat1 * p0;
            glm::vec4 p1_1 = rotMat1 * p1;

            // LORRTODO: Comparing the points no longer works with
            // glm.  Floating point errors break it. However, the i==0
            // thing doesn't work for closed forms. For example, it
            // won't work with a torus, which we would like to
            // support. So, someday we need a better solution,
            // probably with epsilon comparison.

            // If first or last, make a tri
            if (i == 0)// (p0_0 == p0_1) // first, do a tri
            {
                builder.addTri(p0_0, p1_0, p1_1, color);
            }
            else if (i == count-2)// (p1_0 == p1_1) // last, do a tri
            {
                builder.addTri(p0_0, p1_0, p0_1, color);
            }
            else
            {
                builder.addQuad(p0_0, p1_0, p1_1, p0_1, color);
            }
        }
    }

    return pGmdl;
}

Gmdl * build_cone(const glm::vec3 & size, u32 slices, Color color)
{
    // build a 2d set of points to lathe
    glm::vec4 points[3];

    float halfX = abs(size.x) / 2.0f;
    float halfY = abs(size.y) / 2.0f;

    points[0] = glm::vec4(0.0f, halfY, 0.0f, 1.0f);
    points[1] = glm::vec4(halfX, -halfY, 0.0f, 1.0f);
    points[2] = glm::vec4(0.0f, -halfY, 0.0f, 1.0f);

    Gmdl * pGmdl = lathe_points(points, 3, slices, color);

    return pGmdl;
}

Gmdl * build_cylinder(const glm::vec3 & size, u32 slices, Color color)
{
    // build a 2d set of points to lathe
    glm::vec4 points[4];

    float halfX = abs(size.x) / 2.0f;
    float halfY = abs(size.y) / 2.0f;

    points[0] = glm::vec4(0.0f, halfY, 0.0f, 1.0f);
    points[1] = glm::vec4(halfX, halfY, 0.0f, 1.0f);
    points[2] = glm::vec4(halfX, -halfY, 0.0f, 1.0f);
    points[3] = glm::vec4(0.0f, -halfY, 0.0f, 1.0f);

    Gmdl * pGmdl = lathe_points(points, 4, slices, color);

    return pGmdl;
}

Gmdl * build_sphere(const glm::vec3 & size, u32 slices, u32 sections, Color color)
{
    // build a 2d set of points to lathe
    u32 pointCount = sections+1;
    glm::vec4 * points = static_cast<glm::vec4*>(GALLOC(kMEM_Model, sizeof(glm::vec4) * pointCount));

    f32 fullArc = glm::radians(180.0f);
    f32 sectionArc = fullArc / sections;

    float halfY = abs(size.y) / 2.0f;
    points[0] = glm::vec4(0.0f, halfY, 0.0f, 1.0f);
    for (u32 i = 1; i < pointCount-1; ++i)
    {
        glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), i * sectionArc, glm::vec3(0.0f, 0.0f, 1.0f));
        points[i] = rotMat * points[0];
    }
    points[pointCount-1] = glm::vec4(0.0f, -halfY, 0.0f, 1.0f);

    Gmdl * pGmdl = lathe_points(points, pointCount, slices, color);

    GFREE(points);

    return pGmdl;
}

inline glm::vec3 convert_quad_sphere_vert(const glm::vec3 & vert, f32 radius)
{
    f32 len = glm::length(vert);
    return vert * (radius / len);
}

inline glm::vec3 project_to_sphere(f32 x, f32 y, f32 z, f32 radius)
{
    glm::vec3 v(x, y, z);
    return v * (radius / glm::length(v));
}

Gmdl * build_quad_sphere(const glm::vec3 & size, u32 sections, Color color)
{
    u32 triCount = sections * sections * 2 * 6; // 2 tris per quad, 6 sides
    Gmdl * pGmdl = Gmdl::create(kVERT_PosNormCol, triCount * 3, kPRIM_Triangle, triCount);

    ShapeBuilder builder(pGmdl);

    f32 radius = size.x / 2.0f;

    f32 inc = size.x / sections;

    f32 u, v;

    for (u32 i = 0; i < sections-1; ++i)
    {
        for (u32 j = 0; j < sections-1; ++j)
        {
            // front
            {
                u = -radius + inc * i;
                v = -radius + inc * j;
                glm::vec3 p0 = project_to_sphere(u, v, radius, radius);
                glm::vec3 p1 = project_to_sphere(u, v - inc, radius, radius);
                glm::vec3 p2 = project_to_sphere(u + inc, v - inc, radius, radius);
                glm::vec3 p3 = project_to_sphere(u + inc, v, radius, radius);
                builder.addQuad(p0, p1, p2, p3, color);
            }

            // bottom
            {
                u = -radius + inc * i;
                v = radius - inc * j;
                glm::vec3 p0 = project_to_sphere(u, -radius, v, radius);
                glm::vec3 p1 = project_to_sphere(u, -radius, v - inc, radius);
                glm::vec3 p2 = project_to_sphere(u + inc, -radius, v - inc, radius);
                glm::vec3 p3 = project_to_sphere(u + inc, -radius, v, radius);
                builder.addQuad(p0, p1, p2, p3, color);
            }

            // back
            {
                u = radius - inc * i;
                v = radius - inc * j;
                glm::vec3 p0 = project_to_sphere(u, v, -radius, radius);
                glm::vec3 p1 = project_to_sphere(u, v - inc, -radius, radius);
                glm::vec3 p2 = project_to_sphere(u - inc, v - inc, -radius, radius);
                glm::vec3 p3 = project_to_sphere(u - inc, v, -radius, radius);
                builder.addQuad(p0, p1, p2, p3, color);
            }

/*
            // top
            {
                glm::vec3 p0 = project_to_sphere(u, radius, v, radius);
                glm::vec3 p1 = project_to_sphere(u + inc, radius, v, radius);
                glm::vec3 p2 = project_to_sphere(u + inc, radius, v - inc, radius);
                glm::vec3 p3 = project_to_sphere(u, radius, v - inc, radius);
                builder.addQuad(p0, p1, p2, p3);
            }
*/
/*
            // left
            {
                glm::vec3 p0 = project_to_sphere(-radius, u, v, radius);
                glm::vec3 p1 = project_to_sphere(-radius, u + inc, v, radius);
                glm::vec3 p2 = project_to_sphere(-radius, u + inc, v - inc, radius);
                glm::vec3 p3 = project_to_sphere(-radius, u, v - inc, radius);
                builder.addQuad(p0, p1, p2, p3);
            }
*/
            // right
        }
    }

    return pGmdl;
}

namespace system_api
{
    i32 create_shape_box(i32 stageHash, const glm::vec3 & size, Color color, Entity & caller)
    {
        Gmdl * pGmdl = build_box(size, color);

        Model * pModel = GNEW(kMEM_Engine, Model, caller.task().id(), pGmdl);
        ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, glm::to_mat4x3(glm::mat4(1.0)), true);
        ModelInstance::model_insert(caller.task().id(), kModelMgrTaskId, pModelInst);
        return pModel->uid();
    }

    i32 create_shape_cone(i32 stageHash, const glm::vec3 & size, i32 slices, Color color, Entity & caller)
    {
        slices = slices > 0 ? slices : 0;
        Gmdl * pGmdl = build_cone(size, slices, color);

        Model * pModel = GNEW(kMEM_Engine, Model, caller.task().id(), pGmdl);
        ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, glm::to_mat4x3(glm::mat4(1.0)), true);
        ModelInstance::model_insert(caller.task().id(), kModelMgrTaskId, pModelInst);
        return pModel->uid();
    }

    i32 create_shape_cylinder(i32 stageHash, const glm::vec3 & size, i32 slices, Color color, Entity & caller)
    {
        slices = slices > 0 ? slices : 0;
        Gmdl * pGmdl = build_cylinder(size, slices, color);

        Model * pModel = GNEW(kMEM_Engine, Model, caller.task().id(), pGmdl);
        ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, glm::to_mat4x3(glm::mat4(1.0)), true);
        ModelInstance::model_insert(caller.task().id(), kModelMgrTaskId, pModelInst);
        return pModel->uid();
    }

    i32 create_shape_sphere(i32 stageHash, const glm::vec3 & size, i32 slices, i32 sections, Color color, Entity & caller)
    {
        slices = slices > 0 ? slices : 0;
        sections = sections > 0 ? sections : 0;
        Gmdl * pGmdl = build_sphere(size, slices, sections, color);

        Model * pModel = GNEW(kMEM_Engine, Model, caller.task().id(), pGmdl);
        ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, glm::to_mat4x3(glm::mat4(1.0)), true);
        ModelInstance::model_insert(caller.task().id(), kModelMgrTaskId, pModelInst);
        return pModel->uid();
    }

    i32 create_shape_quad_sphere(i32 stageHash, const glm::vec3 & size, i32 sections, Color color, Entity & caller)
    {
        sections = sections > 0 ? sections : 0;
        Gmdl * pGmdl = build_quad_sphere(size, sections, color);

        Model * pModel = GNEW(kMEM_Engine, Model, caller.task().id(), pGmdl);
        ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, glm::to_mat4x3(glm::mat4(1.0)), true);
        ModelInstance::model_insert(caller.task().id(), kModelMgrTaskId, pModelInst);
        return pModel->uid();
    }
}

} // namespace gaen


