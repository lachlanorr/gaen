//------------------------------------------------------------------------------
// shapes.cpp - Routines to create various geometrical shapes
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

u16 ShapeBuilder::setVert(u32 * pVertIdx,
                          const vec3 & pos,
                          const vec3 & norm,
                          Color color)
{
    VertPosNormCol * pVert = mGmdl;
    pVert += *pVertIdx;

    pVert->position = pos;
    pVert->normal = norm;
    pVert->color = color;

    u16 idx = *pVertIdx;
    *pVertIdx += 1;

    return idx;
}

void ShapeBuilder::setTri(u32 * pPrimIdx,
                          u16 idx0,
                          u16 idx1,
                          u16 idx2)
{
    PrimTriangle * pTris = mGmdl;
    PrimTriangle & tri = pTris[*pPrimIdx];

    tri.p0 = idx0;
    tri.p1 = idx1;
    tri.p2 = idx2;

    *pPrimIdx += 1;
}

void ShapeBuilder::setTri(u32 * pVertIdx,
                          u32 * pPrimIdx,
                          const vec3 & p0,
                          const vec3 & p1,
                          const vec3 & p2,
                          Color color)
{
    VertPosNormCol * pVert = mGmdl;
    pVert += *pVertIdx;

    PrimTriangle * pTris = mGmdl;
    PrimTriangle & tri = pTris[*pPrimIdx];

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

    tri.p0 = *pVertIdx + 0;
    tri.p1 = *pVertIdx + 1;
    tri.p2 = *pVertIdx + 2;

    *pVertIdx += 3;
    *pPrimIdx += 1;
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

    setTri(&mCurrVertex, &mCurrPrimitive, p0, p1, p2, color);
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

void ShapeBuilder::setQuad(u32 * pVertIdx,
                           u32 * pPrimIdx,
                           const vec3 & p0,
                           const vec3 & p1,
                           const vec3 & p2,
                           const vec3 & p3,
                           const vec3 & norm,
                           Color color)
{
    VertPosNormCol * pVert = mGmdl;
    pVert += *pVertIdx;

    PrimTriangle * pTris = mGmdl;
    PrimTriangle & tri0 = pTris[*pPrimIdx];
    PrimTriangle & tri1 = pTris[*pPrimIdx+1];

    pVert[0].position = p0;
    pVert[0].normal = norm;
    pVert[0].color = color;

    pVert[1].position = p1;
    pVert[1].normal = norm;
    pVert[1].color = color;

    pVert[2].position = p2;
    pVert[2].normal = norm;
    pVert[2].color = color;

    pVert[3].position = p3;
    pVert[3].normal = norm;
    pVert[3].color = color;

    tri0.p0 = *pVertIdx + 0;
    tri0.p1 = *pVertIdx + 1;
    tri0.p2 = *pVertIdx + 2;

    tri1.p0 = *pVertIdx + 3;
    tri1.p1 = *pVertIdx + 0;
    tri1.p2 = *pVertIdx + 2;

    *pVertIdx += 4;
    *pPrimIdx += 2;
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

    vec3 norm = tri_normal(p0, p1, p2);

    setQuad(&mCurrVertex,
            &mCurrPrimitive,
            p0,
            p1,
            p2,
            p3,
            norm,
            color);
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

void ShapeBuilder::setHex(u32 * pVertIdx,
                          u32 * pPrimIdx,
                          f32 height,
                          f32 length,
                          Color * pColors,
                          u32 colorsSize,
                          const vec3 & offset,
                          HexSides sides)
{
    // Hexes are pointy topped.
    // Height is point to point
    // Width is edge to edge
    // Length is the "height" of the 3d hex
    f32 heightHalf = height / 2.0f;
    f32 heightQuarter = heightHalf / 2.0f;
    f32 widthHalf = heightHalf * (sqrt(3.0f) / 2.0f);
    f32 lengthHalf = length / 2.0f;

    // Support for multicolored hexes, there are 8 possible colors
    Color colors[6];
    if (colorsSize == 1)
    {
        // Single color hex
        for (u32 i = 0; i < 6; ++i)
            colors[i] = pColors[0];
    }
    else if (colorsSize == 2)
    {
        // Two color hex, vertical split in middle (pointy top and bottom)
        colors[0] = pColors[0];
        colors[1] = pColors[1];
        colors[2] = pColors[0];
        colors[3] = pColors[1];
        colors[4] = pColors[0];
        colors[5] = pColors[1];
    }
    else if (colorsSize == 3)
    {
        // Three color hex, tri on top, horizontal strip in middle, tri on bottom
        colors[0] = pColors[0];
        colors[1] = pColors[0];
        colors[2] = pColors[1];
        colors[3] = pColors[1];
        colors[4] = pColors[2];
        colors[5] = pColors[2];
    }
    else if (colorsSize == 6)
    {
        // Six custom colors
        colors[0] = pColors[0];
        colors[1] = pColors[1];
        colors[2] = pColors[2];
        colors[3] = pColors[3];
        colors[4] = pColors[4];
        colors[5] = pColors[5];
    }
    else
    {
        PANIC("Invalid colors size for hex: %u", colorsSize);
    }

    // Normals coords of angled sides
    static const f32 kNormX = 0.86602540378443871;
    static const f32 kNormZ = 0.5;

    static const vec3 kTopNorm         {   0.0f,  1.0f,    0.0f};
    static const vec3 kTopLeftNorm     {-kNormX,  0.0f, -kNormZ};
    static const vec3 kLeftNorm        {  -1.0f,  0.0f,    0.0f};
    static const vec3 kBottomLeftNorm  {-kNormX,  0.0f,  kNormZ};
    static const vec3 kBottomRightNorm { kNormX,  0.0f,  kNormZ};
    static const vec3 kRightNorm       {   1.0f,  0.0f,    0.0f};
    static const vec3 kTopRightNorm    { kNormX,  0.0f, -kNormZ};
    static const vec3 kBottomNorm      {   0.0f, -1.0f,    0.0f};

    // top points, starting on topmost (pointy) counter clockwise, as viewed from top
    vec3 topP[8];
    topP[0] = vec3{  0.0f,      lengthHalf, -heightHalf}    + offset;  // Top Point
    topP[1] = vec3{ -widthHalf, lengthHalf, -heightQuarter} + offset;  // Upper Left Point
    topP[2] = vec3{ -widthHalf, lengthHalf,  heightQuarter} + offset;  // Lower Left Point
    topP[3] = vec3{  0.0f,      lengthHalf,  heightHalf}    + offset;  // Bottom Point
    topP[4] = vec3{  widthHalf, lengthHalf,  heightQuarter} + offset;  // Lower Right Point
    topP[5] = vec3{  widthHalf, lengthHalf, -heightQuarter} + offset;  // Upper Right Point

    // Auxiliary top points to support multi color hexes
    topP[6] = vec3{  0.0f, lengthHalf, -heightQuarter} + offset;
    topP[7] = vec3{  0.0f, lengthHalf,  heightQuarter} + offset;


    // bottom points, starting on topmost (pointy) counter clockwise, as viewed from bottom
    vec3 botP[8];
    botP[0] = vec3{  0.0f,      -lengthHalf, -heightHalf}    + offset;
    botP[1] = vec3{ -widthHalf, -lengthHalf, -heightQuarter} + offset;
    botP[2] = vec3{ -widthHalf, -lengthHalf,  heightQuarter} + offset;
    botP[3] = vec3{  0.0f,      -lengthHalf,  heightHalf}    + offset;
    botP[4] = vec3{  widthHalf, -lengthHalf,  heightQuarter} + offset;
    botP[5] = vec3{  widthHalf, -lengthHalf, -heightQuarter} + offset;

    // Auxiliary bottom points to support multi color hexes
    botP[6] = vec3{  0.0f, -lengthHalf, -heightQuarter} + offset;
    botP[7] = vec3{  0.0f, -lengthHalf,  heightQuarter} + offset;


    // Add top triangles
    if (sides & kHXS_Top)
    {
        u16 triTopL[3];
        u16 triTopR[3];

        u16 triBotL[3];
        u16 triBotR[3];

        // Top Triangle
        if (colors[0] != colors[1])
        {
            // make two distinct triangles
            triTopL[0] = setVert(pVertIdx, topP[0], kTopNorm, colors[0]);
            triTopL[1] = setVert(pVertIdx, topP[1], kTopNorm, colors[0]);
            triTopL[2] = setVert(pVertIdx, topP[6], kTopNorm, colors[0]);

            triTopR[0] = setVert(pVertIdx, topP[5], kTopNorm, colors[1]);
            triTopR[1] = setVert(pVertIdx, topP[0], kTopNorm, colors[1]);
            triTopR[2] = setVert(pVertIdx, topP[6], kTopNorm, colors[1]);

            setTri(pPrimIdx, triTopL[0], triTopL[1], triTopL[2]);
            setTri(pPrimIdx, triTopR[0], triTopR[1], triTopR[2]);
        }
        else
        {
            // make one triangle
            triTopL[0] = setVert(pVertIdx, topP[0], kTopNorm, colors[0]);
            triTopL[1] = setVert(pVertIdx, topP[1], kTopNorm, colors[0]);
            triTopL[2] = setVert(pVertIdx, topP[5], kTopNorm, colors[0]);

            // Set triTopR indices to simplify logic below with rects between the triangles
            triTopR[0] = triTopL[2];
            triTopR[1] = triTopL[0];
            triTopR[2] = triTopL[1];

            setTri(pPrimIdx, triTopL[0], triTopL[1], triTopL[2]);
        }


        // Bottom Triangle
        if (colors[4] != colors[5])
        {
            // make two distinct triangles
            triBotL[0] = setVert(pVertIdx, topP[2], kTopNorm, colors[4]);
            triBotL[1] = setVert(pVertIdx, topP[3], kTopNorm, colors[4]);
            triBotL[2] = setVert(pVertIdx, topP[7], kTopNorm, colors[4]);

            triBotR[0] = setVert(pVertIdx, topP[3], kTopNorm, colors[5]);
            triBotR[1] = setVert(pVertIdx, topP[4], kTopNorm, colors[5]);
            triBotR[2] = setVert(pVertIdx, topP[7], kTopNorm, colors[5]);

            setTri(pPrimIdx, triBotL[0], triBotL[1], triBotL[2]);
            setTri(pPrimIdx, triBotR[0], triBotR[1], triBotR[2]);
        }
        else
        {
            // make one triangle
            triBotL[0] = setVert(pVertIdx, topP[2], kTopNorm, colors[4]);
            triBotL[1] = setVert(pVertIdx, topP[3], kTopNorm, colors[4]);
            triBotL[2] = setVert(pVertIdx, topP[4], kTopNorm, colors[4]);

            // Set triBotR indices to simplify logic below with rects between the triangles
            triBotR[0] = triBotL[1];
            triBotR[1] = triBotL[2];
            triBotR[2] = triBotL[0];

            setTri(pPrimIdx, triBotL[0], triBotL[1], triBotL[2]);
        }

        u16 rectL[4];
        u16 rectR[4];

        // Rects (or Rect) between the two triangles
        if (colors[2] != colors[3])
        {
            //
            // Left Rect
            //
            // If rect matches to left triangle, re-use the vert from the top triangle
            if (colors[2] == colors[0])
            {
                rectL[0] = triTopL[1];

                // If top is split between two colors, re-use the vert in the middle
                if (colors[0] != colors[1])
                    rectL[3] = triTopL[2];
                else
                    rectL[3] = setVert(pVertIdx, topP[6], kTopNorm, colors[2]);
            }
            else
            {
                rectL[0] = setVert(pVertIdx, topP[1], kTopNorm, colors[2]);
                rectL[3] = setVert(pVertIdx, topP[6], kTopNorm, colors[2]);
            }

            // If rect matches bottom left triangle, re-use the vert from the bottom triangle
            if (colors[2] == colors[4])
            {
                rectL[1] = triBotL[0];

                // If bottom is split between two colors, re-use the vert in the middle
                if (colors[4] != colors[5])
                    rectL[2] = triBotL[2];
                else
                    rectL[2] = setVert(pVertIdx, topP[7], kTopNorm, colors[2]);
            }
            else
            {
                rectL[1] = setVert(pVertIdx, topP[2], kTopNorm, colors[2]);
                rectL[2] = setVert(pVertIdx, topP[7], kTopNorm, colors[2]);
            }

            setTri(pPrimIdx, rectL[0], rectL[1], rectL[3]);
            setTri(pPrimIdx, rectL[3], rectL[1], rectL[2]);


            //
            // Right Rect
            //
            // If rect matches to right triangle, re-use the vert from the top triangle
            if (colors[3] == colors[1])
            {
                rectR[3] = triTopR[0];

                // If top is split between two colors, re-use the vert in the middle
                if (colors[0] != colors[1])
                    rectR[0] = triTopR[2];
                else
                    rectR[0] = setVert(pVertIdx, topP[6], kTopNorm, colors[3]);
            }
            else
            {
                rectR[3] = setVert(pVertIdx, topP[5], kTopNorm, colors[3]);
                rectR[0] = setVert(pVertIdx, topP[6], kTopNorm, colors[3]);
            }

            // If rect matches bottom left triangle, re-use the vert from the bottom triangle
            if (colors[3] == colors[5])
            {
                rectR[2] = triBotR[1];

                // If bottom is split between two colors, re-use the vert in the middle
                if (colors[4] != colors[5])
                    rectR[1] = triBotR[2];
                else
                    rectR[1] = setVert(pVertIdx, topP[7], kTopNorm, colors[3]);
            }
            else
            {
                rectR[2] = setVert(pVertIdx, topP[4], kTopNorm, colors[3]);
                rectR[1] = setVert(pVertIdx, topP[7], kTopNorm, colors[3]);
            }

            setTri(pPrimIdx, rectR[0], rectR[1], rectR[3]);
            setTri(pPrimIdx, rectR[3], rectR[1], rectR[2]);
        }
        else // same color so one big rect
        {
            // If rect matches top left triangle, we can re-use the vert from the top triangle
            if (colors[2] == colors[0])
                rectL[0] = triTopL[1];
            else
                rectL[0] = setVert(pVertIdx, topP[1], kTopNorm, colors[2]);

            // If rect matches bottom left triangle, re-use the vert from the bottom triangle
            if (colors[2] == colors[4])
                rectL[1] = triBotL[0];
            else
                rectL[1] = setVert(pVertIdx, topP[2], kTopNorm, colors[2]);

            // If rect matches bottom right triangle
            if (colors[2] == colors[5])
                rectL[2] = triBotR[1];
            else
                rectL[2] = setVert(pVertIdx, topP[4], kTopNorm, colors[2]);

            // If rect matches top right triangle
            if (colors[2] == colors[1])
                rectL[3] = triTopR[0];
            else
                rectL[3] = setVert(pVertIdx, topP[5], kTopNorm, colors[2]);


            setTri(pPrimIdx, rectL[0], rectL[1], rectL[3]);
            setTri(pPrimIdx, rectL[3], rectL[1], rectL[2]);
        }

    }


    // Add sides
    if (sides & kHXS_TopLeft)
    {
        setQuad(pVertIdx, pPrimIdx, topP[0], botP[0], botP[1], topP[1], kTopLeftNorm, colors[0]);
    }

    if (sides & kHXS_Left)
    {
        setQuad(pVertIdx, pPrimIdx, topP[1], botP[1], botP[2], topP[2], kLeftNorm, colors[2]);
    }

    if (sides & kHXS_BottomLeft)
    {
        setQuad(pVertIdx, pPrimIdx, topP[2], botP[2], botP[3], topP[3], kBottomLeftNorm, colors[4]);
    }

    if (sides & kHXS_BottomRight)
    {
        setQuad(pVertIdx, pPrimIdx, topP[3], botP[3], botP[4], topP[4], kBottomRightNorm, colors[5]);
    }

    if (sides & kHXS_Right)
    {
        setQuad(pVertIdx, pPrimIdx, topP[4], botP[4], botP[5], topP[5], kRightNorm, colors[3]);
    }

    if (sides & kHXS_TopRight)
    {
        setQuad(pVertIdx, pPrimIdx, topP[5], botP[5], botP[0], topP[0], kTopRightNorm, colors[1]);
    }


    // Add bottom triangles
    if (sides & kHXS_Bottom)
    {
        LOG(kLS_Error, "WARNING: kHHX_Bottom not optimized geometry");
        if (colors[0] != colors[1])
        {
            setTri(pVertIdx, pPrimIdx, botP[0], botP[6], botP[1], colors[0]);
            setTri(pVertIdx, pPrimIdx, botP[0], botP[5], botP[6], colors[1]);
        }
        else
        {
            setTri(pVertIdx, pPrimIdx, botP[0], botP[5], botP[1], colors[0]);
        }

        if (colors[2] != colors[3])
        {
            setTri(pVertIdx, pPrimIdx, botP[1], botP[6], botP[2], colors[2]);
            setTri(pVertIdx, pPrimIdx, botP[6], botP[7], botP[2], colors[2]);

            setTri(pVertIdx, pPrimIdx, botP[6], botP[5], botP[7], colors[3]);
            setTri(pVertIdx, pPrimIdx, botP[5], botP[4], botP[7], colors[3]);
        }
        else
        {
            setTri(pVertIdx, pPrimIdx, botP[1], botP[5], botP[2], colors[2]);
            setTri(pVertIdx, pPrimIdx, botP[5], botP[4], botP[2], colors[2]);
        }

        if (colors[4] != colors[5])
        {
            setTri(pVertIdx, pPrimIdx, botP[2], botP[7], botP[3], colors[4]);
            setTri(pVertIdx, pPrimIdx, botP[7], botP[4], botP[3], colors[5]);
        }
        else
        {
            setTri(pVertIdx, pPrimIdx, botP[2], botP[4], botP[3], colors[4]);
        }
    }

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

    pGmdl->updateHalfExtents();
}

Gmdl * build_hex(f32 height, f32 length, Color * pColors, u32 colorsSize, const mat43 & transform)
{
    ASSERT(height > 0 && length > 0);
    height = abs(height);
    length = abs(length);

    Gmdl * pGmdl = Gmdl::create(kVERT_PosNormCol, kHexTriCount * 3, kPRIM_Triangle, kHexTriCount);

    ShapeBuilder builder(pGmdl);

    u32 vertIdx = 0;
    u32 primIdx = 0;
    builder.setHex(&vertIdx, &primIdx, height, length, pColors, colorsSize, vec3{0.0});

    transform_gmdl<VertPosNormCol>(pGmdl, transform);

    return pGmdl;
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
i32 shape_box(i32 stageHash, i32 passHash, const vec3 & size, Color color, const mat43 & transform, Entity * pCaller)
{
    Gmdl * pGmdl = build_box(size, color, mat43(1.0f));

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl, nullptr);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, pass_from_hash(passHash), transform, true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}

i32 shape_cone(i32 stageHash, i32 passHash, const vec3 & size, i32 slices, Color color, const mat43 & transform, Entity * pCaller)
{
    slices = slices > 0 ? slices : 0;
    Gmdl * pGmdl = build_cone(size, slices, color, mat43(1.0f));

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl, nullptr);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, pass_from_hash(passHash), transform, true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}

i32 shape_cylinder(i32 stageHash, i32 passHash, const vec3 & size, i32 slices, Color color, const mat43 & transform, Entity * pCaller)
{
    slices = slices > 0 ? slices : 0;
    Gmdl * pGmdl = build_cylinder(size, slices, color, mat43(1.0f));

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl, nullptr);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, pass_from_hash(passHash), transform, true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}

i32 shape_hex(i32 stageHash, i32 passHash, f32 height, f32 length, Color color0, Color color1, Color color2, Color color3, Color color4, Color color5, const mat43 & transform, Entity * pCaller)
{
    Color colors[6];
    colors[0] = color0;
    colors[1] = color1;
    colors[2] = color2;
    colors[3] = color3;
    colors[4] = color4;
    colors[5] = color5;

    Gmdl * pGmdl = build_hex(height, length, colors, 6, mat43(1.0f));

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl, nullptr);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, pass_from_hash(passHash), transform, true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}

i32 shape_hex(i32 stageHash, i32 passHash, f32 height, f32 length, Color color, const mat43 & transform, Entity * pCaller)
{
    return shape_hex(stageHash, passHash, height, length, color, color, color, color, color, color, transform, pCaller);
}

i32 shape_hex(i32 stageHash, i32 passHash, f32 height, f32 length, Color color0, Color color1, const mat43 & transform, Entity * pCaller)
{
    return shape_hex(stageHash, passHash, height, length, color0, color1, color0, color1, color0, color1, transform, pCaller);
}

i32 shape_hex(i32 stageHash, i32 passHash, f32 height, f32 length, Color color0, Color color1, Color color2, const mat43 & transform, Entity * pCaller)
{
    return shape_hex(stageHash, passHash, height, length, color0, color0, color1, color1, color2, color2, transform, pCaller);
}

i32 shape_sphere(i32 stageHash, i32 passHash, const vec3 & size, i32 slices, i32 sections, Color color, const mat43 & transform, Entity * pCaller)
{
    slices = slices > 0 ? slices : 0;
    sections = sections > 0 ? sections : 0;
    Gmdl * pGmdl = build_sphere(size, slices, sections, color, mat43(1.0f));

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl, nullptr);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, pass_from_hash(passHash), transform, true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}

i32 shape_quad_sphere(i32 stageHash, i32 passHash, const vec3 & size, i32 sections, Color color, const mat43 & transform, Entity * pCaller)
{
    sections = sections > 0 ? sections : 0;
    Gmdl * pGmdl = build_quad_sphere(size, sections, color, mat43(1.0f));

    Model * pModel = GNEW(kMEM_Engine, Model, pCaller->task().id(), pGmdl, nullptr);
    ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, stageHash, pass_from_hash(passHash), transform, true);
    ModelInstance::model_insert(pCaller->task().id(), kModelMgrTaskId, pModelInst);
    return pModel->uid();
}
} // namespace system_api

} // namespace gaen


