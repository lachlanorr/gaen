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

#include "math/common.h"
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


void ShapeBuilder::addTri(const vec3 & p0,
                          const vec3 & p1,
                          const vec3 & p2,
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

    vec3 vecNorm = tri_normal(p0, p1, p2);

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

void ShapeBuilder::addTri(const vec3 * pPoints, Color color)
{
    addTri(pPoints[0],
           pPoints[1],
           pPoints[2],
           color);
}

void ShapeBuilder::addTri(const vec4 & p0,
                          const vec4 & p1,
                          const vec4 & p2,
                          Color color)
{
    addTri(vec3(p0),
           vec3(p1),
           vec3(p2),
           color);
}

void ShapeBuilder::addQuad(const vec3 & p0,
                           const vec3 & p1,
                           const vec3 & p2,
                           const vec3 & p3,
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

    vec3 vecNorm = tri_normal(p0, p1, p2);
    vec3 vecNorm2 = tri_normal(p3, p0, p2);

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

void ShapeBuilder::addQuad(const vec3 * pPoints, Color color)
{
    addQuad(pPoints[0],
            pPoints[1],
            pPoints[2],
            pPoints[3],
            color);
}

void ShapeBuilder::addQuad(const vec4 & p0,
                           const vec4 & p1,
                           const vec4 & p2,
                           const vec4 & p3,
                           Color color)
{
    addQuad(vec3(p0),
            vec3(p1),
            vec3(p2),
            vec3(p3),
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

template <class VertT>
void transform_gmdl(Gmdl * pGmdl, const mat43 & transform)
{
    mat4 t(transform);

    VertT * pVert = *pGmdl;
    for (u32 i = 0; i < pGmdl->vertCount(); ++i)
    {
        vec4 v4(pVert->position, 1.0f);
        v4 = t * v4;

        pVert->position = vec3(v4);

        ++pVert;
    }
}

Gmdl * build_box(const vec3 & size, Color color, const mat43 & transform)
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
    builder.addQuad(vec3(xmin, ymax, zmax), vec3(xmin, ymin, zmax), vec3(xmax, ymin, zmax), vec3(xmax, ymax, zmax), color);

    // Bottom
    builder.addQuad(vec3(xmin, ymin, zmax), vec3(xmin, ymin, zmin), vec3(xmax, ymin, zmin), vec3(xmax, ymin, zmax), color);

    // Back
    builder.addQuad(vec3(xmin, ymin, zmin), vec3(xmin, ymax, zmin), vec3(xmax, ymax, zmin), vec3(xmax, ymin, zmin), color);

    // Top
    builder.addQuad(vec3(xmax, ymax, zmax), vec3(xmax, ymax, zmin), vec3(xmin, ymax, zmin), vec3(xmin, ymax, zmax), color);

    // Left
    builder.addQuad(vec3(xmin, ymax, zmax), vec3(xmin, ymax, zmin), vec3(xmin, ymin, zmin), vec3(xmin, ymin, zmax), color);

    // Right
    builder.addQuad(vec3(xmax, ymin, zmax), vec3(xmax, ymin, zmin), vec3(xmax, ymax, zmin), vec3(xmax, ymax, zmax), color);

    transform_gmdl<VertPosNormCol>(pGmdl, transform);
    return pGmdl;
}

// Lathe around y axis
Gmdl * lathe_points(const vec4 * pPoints, u32 count, u32 slices, Color color)
{
    ASSERT(slices > 2);

    u32 triCount = (count - 3) * slices * 2;
    // If it's a closed surface, we'll not need a quad
    // on the ends of each slice.
    triCount += slices * (pPoints[0].x == 0.0f ? 1 : 2);
    triCount += slices * (pPoints[count-1].x == 0.0f ? 1 : 2);

    Gmdl * pGmdl = Gmdl::create(kVERT_PosNormCol, triCount * 3, kPRIM_Triangle, triCount);

    ShapeBuilder builder(pGmdl);

    f32 fullArc = radians(360.0f);
    f32 arcPart = fullArc / slices;

    for (u32 s = 0; s < slices; ++s)
    {
        f32 arc0 = arcPart * s;
        f32 arc1 = s < (slices-1) ? arc0 + arcPart : 0.0f; // last arc1 connects back to 0 arc

        for (u32 i = 0; i < count-1; ++i)
        {
            vec4 p0 = pPoints[i];
            vec4 p1 = pPoints[i+1];

            mat4 rotMat0 = mat4::from_rot(0.0f, arc0, 0.0f);
            mat4 rotMat1 = mat4::from_rot(0.0f, arc1, 0.0f);

            vec4 p0_0 = rotMat0 * p0;
            vec4 p1_0 = rotMat0 * p1;

            vec4 p0_1 = rotMat1 * p0;
            vec4 p1_1 = rotMat1 * p1;

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

Gmdl * build_cone(const vec3 & size, u32 slices, Color color, const mat43 & transform)
{
    // build a 2d set of points to lathe
    vec4 points[3];

    float halfX = abs(size.x) / 2.0f;
    float halfY = abs(size.y) / 2.0f;

    points[0] = vec4(0.0f, halfY, 0.0f, 1.0f);
    points[1] = vec4(halfX, -halfY, 0.0f, 1.0f);
    points[2] = vec4(0.0f, -halfY, 0.0f, 1.0f);

    Gmdl * pGmdl = lathe_points(points, 3, slices, color);

    transform_gmdl<VertPosNormCol>(pGmdl, transform);
    return pGmdl;
}

Gmdl * build_cylinder(const vec3 & size, u32 slices, Color color, const mat43 & transform)
{
    // build a 2d set of points to lathe
    vec4 points[4];

    float halfX = abs(size.x) / 2.0f;
    float halfY = abs(size.y) / 2.0f;

    points[0] = vec4(0.0f, halfY, 0.0f, 1.0f);
    points[1] = vec4(halfX, halfY, 0.0f, 1.0f);
    points[2] = vec4(halfX, -halfY, 0.0f, 1.0f);
    points[3] = vec4(0.0f, -halfY, 0.0f, 1.0f);

    Gmdl * pGmdl = lathe_points(points, 4, slices, color);

    transform_gmdl<VertPosNormCol>(pGmdl, transform);
    return pGmdl;
}

Gmdl * build_hex(f32 width, f32 height, Color color, const mat43 & transform)
{
    ASSERT(width > 0 && height > 0);
    width = abs(width);
    height = abs(height);

    f32 radius = width / 2.0f;
    f32 radiusHalf = radius / 2.0f;
    f32 radiusShort = radius * (sqrt(3.0f) / 2.0f);
    f32 heightHalf = height / 2.0f;

    // 4 triangles each on top and bottom, 2 per side
    u32 triCount = 4 * 2 + 2 * 6;

    Gmdl * pGmdl = Gmdl::create(kVERT_PosNormCol, triCount * 3, kPRIM_Triangle, triCount);

    ShapeBuilder builder(pGmdl);

    // top points, starting on rightmost and going counter clockwise, as viewed from top
    vec3 topP[6];
    topP[0] = vec3{ radius,     heightHalf,  0.0f};
    topP[1] = vec3{ radiusHalf, heightHalf, -radiusShort};
    topP[2] = vec3{-radiusHalf, heightHalf, -radiusShort};
    topP[3] = vec3{-radius,     heightHalf,  0.0f};
    topP[4] = vec3{-radiusHalf, heightHalf,  radiusShort};
    topP[5] = vec3{ radiusHalf, heightHalf,  radiusShort};

    // bottom points, starting on rightmost and going counter clockwise, as viewed from top
    vec3 botP[6];
    botP[0] = vec3{ radius,     -heightHalf,  0.0f};
    botP[1] = vec3{ radiusHalf, -heightHalf, -radiusShort};
    botP[2] = vec3{-radiusHalf, -heightHalf, -radiusShort};
    botP[3] = vec3{-radius,     -heightHalf,  0.0f};
    botP[4] = vec3{-radiusHalf, -heightHalf,  radiusShort};
    botP[5] = vec3{ radiusHalf, -heightHalf,  radiusShort};


    // Add top triangles
    builder.addTri(topP[0], topP[1], topP[5], color);
    builder.addTri(topP[1], topP[2], topP[5], color);
    builder.addTri(topP[2], topP[4], topP[5], color);
    builder.addTri(topP[2], topP[3], topP[4], color);


    // Add sides
    builder.addTri(topP[0], botP[0], topP[1], color);
    builder.addTri(topP[1], botP[0], botP[1], color);

    builder.addTri(topP[1], botP[1], topP[2], color);
    builder.addTri(topP[2], botP[1], botP[2], color);

    builder.addTri(topP[2], botP[2], topP[3], color);
    builder.addTri(topP[3], botP[2], botP[3], color);

    builder.addTri(topP[3], botP[3], topP[4], color);
    builder.addTri(topP[4], botP[3], botP[4], color);

    builder.addTri(topP[4], botP[4], topP[5], color);
    builder.addTri(topP[5], botP[4], botP[5], color);

    builder.addTri(topP[5], botP[5], topP[0], color);
    builder.addTri(topP[0], botP[5], botP[0], color);


    // Add bottom triangles
    builder.addTri(botP[0], botP[5], botP[1], color);
    builder.addTri(botP[1], botP[5], botP[2], color);
    builder.addTri(botP[2], botP[5], botP[4], color);
    builder.addTri(botP[2], botP[4], botP[3], color);


    transform_gmdl<VertPosNormCol>(pGmdl, transform);

    return pGmdl;
}

Gmdl * build_sphere(const vec3 & size, u32 slices, u32 sections, Color color, const mat43 & transform)
{
    // build a 2d set of points to lathe
    u32 pointCount = sections+1;
    vec4 * points = static_cast<vec4*>(GALLOC(kMEM_Model, sizeof(vec4) * pointCount));

    f32 fullArc = radians(180.0f);
    f32 sectionArc = fullArc / sections;

    float halfY = abs(size.y) / 2.0f;
    points[0] = vec4(0.0f, halfY, 0.0f, 1.0f);
    for (u32 i = 1; i < pointCount-1; ++i)
    {
        mat4 rotMat = mat4::from_rot(0.0f, 0.0f, i * sectionArc);
        points[i] = rotMat * points[0];
    }
    points[pointCount-1] = vec4(0.0f, -halfY, 0.0f, 1.0f);

    Gmdl * pGmdl = lathe_points(points, pointCount, slices, color);

    GFREE(points);

    transform_gmdl<VertPosNormCol>(pGmdl, transform);
    return pGmdl;
}

inline vec3 convert_quad_sphere_vert(const vec3 & vert, f32 radius)
{
    f32 len = length(vert);
    return vert * (radius / len);
}

inline vec3 project_to_sphere(f32 x, f32 y, f32 z, f32 radius)
{
    vec3 v(x, y, z);
    return v * (radius / length(v));
}

Gmdl * build_quad_sphere(const vec3 & size, u32 sections, Color color, const mat43 & transform)
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
                vec3 p0 = project_to_sphere(u, v, radius, radius);
                vec3 p1 = project_to_sphere(u, v - inc, radius, radius);
                vec3 p2 = project_to_sphere(u + inc, v - inc, radius, radius);
                vec3 p3 = project_to_sphere(u + inc, v, radius, radius);
                builder.addQuad(p0, p1, p2, p3, color);
            }

            // bottom
            {
                u = -radius + inc * i;
                v = radius - inc * j;
                vec3 p0 = project_to_sphere(u, -radius, v, radius);
                vec3 p1 = project_to_sphere(u, -radius, v - inc, radius);
                vec3 p2 = project_to_sphere(u + inc, -radius, v - inc, radius);
                vec3 p3 = project_to_sphere(u + inc, -radius, v, radius);
                builder.addQuad(p0, p1, p2, p3, color);
            }

            // back
            {
                u = radius - inc * i;
                v = radius - inc * j;
                vec3 p0 = project_to_sphere(u, v, -radius, radius);
                vec3 p1 = project_to_sphere(u, v - inc, -radius, radius);
                vec3 p2 = project_to_sphere(u - inc, v - inc, -radius, radius);
                vec3 p3 = project_to_sphere(u - inc, v, -radius, radius);
                builder.addQuad(p0, p1, p2, p3, color);
            }

/*
            // top
            {
                vec3 p0 = project_to_sphere(u, radius, v, radius);
                vec3 p1 = project_to_sphere(u + inc, radius, v, radius);
                vec3 p2 = project_to_sphere(u + inc, radius, v - inc, radius);
                vec3 p3 = project_to_sphere(u, radius, v - inc, radius);
                builder.addQuad(p0, p1, p2, p3);
            }
*/
/*
            // left
            {
                vec3 p0 = project_to_sphere(-radius, u, v, radius);
                vec3 p1 = project_to_sphere(-radius, u + inc, v, radius);
                vec3 p2 = project_to_sphere(-radius, u + inc, v - inc, radius);
                vec3 p3 = project_to_sphere(-radius, u, v - inc, radius);
                builder.addQuad(p0, p1, p2, p3);
            }
*/
            // right
        }
    }

    transform_gmdl<VertPosNormCol>(pGmdl, transform);
    return pGmdl;
}

namespace system_api
{
i32 shape_box(i32 stageHash, const vec3 & size, Color color, const mat43 & transform, Entity * pCaller)
{
    Gmdl * pGmdl = build_box(size, color, transform);

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, mat43(1.0f), true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}

i32 shape_cone(i32 stageHash, const vec3 & size, i32 slices, Color color, const mat43 & transform, Entity * pCaller)
{
    slices = slices > 0 ? slices : 0;
    Gmdl * pGmdl = build_cone(size, slices, color, transform);

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, mat43(1.0f), true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}

i32 shape_cylinder(i32 stageHash, const vec3 & size, i32 slices, Color color, const mat43 & transform, Entity * pCaller)
{
    slices = slices > 0 ? slices : 0;
    Gmdl * pGmdl = build_cylinder(size, slices, color, transform);

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, mat43(1.0f), true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}

i32 shape_hex(i32 stageHash, f32 width, f32 height, Color color, const mat43 & transform, Entity * pCaller)
{
    Gmdl * pGmdl = build_hex(width, height, color, transform);

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, mat43(1.0f), true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}

i32 shape_sphere(i32 stageHash, const vec3 & size, i32 slices, i32 sections, Color color, const mat43 & transform, Entity * pCaller)
{
    slices = slices > 0 ? slices : 0;
    sections = sections > 0 ? sections : 0;
    Gmdl * pGmdl = build_sphere(size, slices, sections, color, transform);

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, mat43(1.0f), true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}

i32 shape_quad_sphere(i32 stageHash, const vec3 & size, i32 sections, Color color, const mat43 & transform, Entity * pCaller)
{
    sections = sections > 0 ? sections : 0;
    Gmdl * pGmdl = build_quad_sphere(size, sections, color, transform);

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, mat43(1.0f), true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}
} // namespace system_api

} // namespace gaen


