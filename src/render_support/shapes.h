//------------------------------------------------------------------------------
// shapes.h - Routines to create various geometrical shapes
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

#ifndef GAEN_RENDER_SUPPORT_SHAPES_H
#define GAEN_RENDER_SUPPORT_SHAPES_H

#include "math/mat43.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "assets/Gmdl.h"
#include "engine/Handle.h"
#include "render_support/Model.h"

namespace gaen
{

// 12 triangles each on top and bottom (to support multi colored hexes), 2 per side
static const u32 kHexTriCount = 8 * 2 + 2 * 6;
static const u32 kHexVertCount = kHexTriCount * 3;

enum HexSides
{
    kHXS_Top         = 0x01,
    kHXS_TopLeft     = 0x02,
    kHXS_Left        = 0x04,
    kHXS_BottomLeft  = 0x08,
    kHXS_BottomRight = 0x10,
    kHXS_Right       = 0x20,
    kHXS_TopRight    = 0x40,
    kHXS_Bottom      = 0x80,

    kHXS_All         = 0xff
};

class ShapeBuilder
{
public:
    ShapeBuilder(Gmdl * pGmdl);

    u16 setVert(u32 * pVertIdx,
                const vec3 & pos,
                const vec3 & norm,
                Color color);

    void setTri(u32 * pPrimIdx,
                u16 idx0,
                u16 idx1,
                u16 idx2);

    void setTri(u32 * pVertIdx,
                u32 * pPrimIdx,
                const vec3 & p0,
                const vec3 & p1,
                const vec3 & p2,
                Color color);

    void addTri(const vec3 & p0,
                const vec3 & p1,
                const vec3 & p2,
                Color color);
    void addTri(const vec3 * pPoints,
                Color color);
    void addTri(const vec4 & p0,
                const vec4 & p1,
                const vec4 & p2,
                Color color);

    void setQuad(u32 * pVertIdx,
                 u32 * pPrimIdx,
                 const vec3 & p0,
                 const vec3 & p1,
                 const vec3 & p2,
                 const vec3 & p3,
                 const vec3 & norm,
                 Color color);

    void addQuad(const vec3 & p0,
                 const vec3 & p1,
                 const vec3 & p2,
                 const vec3 & p3,
                 Color color);
    void addQuad(const vec3 * pPoints,
                 Color color);
    void addQuad(const vec4 & p0,
                 const vec4 & p1,
                 const vec4 & p2,
                 const vec4 & p3,
                 Color color);

    void setHex(u32 * pVertIdx,
                u32 * pPrimIdx,
                f32 height,
                f32 length,
                Color * pColors,
                u32 colorsSize,
                const vec3 & offset,
                HexSides sides = kHXS_All);

    void addGmdl(const Gmdl & gmdl);

    Gmdl & gmdl() { return mGmdl; }
    const Gmdl & gmdl() const { return mGmdl; }

    u32 currVertex() { return mCurrVertex; }
    u32 currPrimitive() { return mCurrPrimitive; }

    bool isFull() { return mCurrVertex >= mGmdl.vertCount() || mCurrPrimitive >= mGmdl.primCount(); }

private:
    Gmdl & mGmdl;
    u32 mCurrVertex = 0;
    u32 mCurrPrimitive = 0;
};

Gmdl * build_hex(f32 height, f32 length, Color * pColors, u32 colorsSize, const mat43 & transform);

Gmdl * build_box(const vec3 & size, Color color, const mat43 & transform);
Gmdl * build_cone(const vec3 & size, u32 slices, Color color, const mat43 & transform);
Gmdl * build_cylinder(const vec3 & size, u32 slices, Color color, const mat43 & transform);
Gmdl * build_sphere(const vec3 & size, u32 slices, u32 sections, Color color, const mat43 & transform);
Gmdl * build_quad_sphere(const vec3 & size, u32 sections, Color color, const mat43 & transform);


// Compose wrappers
class Entity;
namespace system_api
{
i32 shape_box(i32 stageHash, i32 passHash, const vec3 & size, Color color, const mat43 & transform, Entity * pCaller);
i32 shape_cone(i32 stageHash, i32 passHash, const vec3 & size, i32 slices, Color color, const mat43 & transform, Entity * pCaller);
i32 shape_cylinder(i32 stageHash, i32 passHash, const vec3 & size, i32 slices, Color color, const mat43 & transform, Entity * pCaller);

i32 shape_hex(i32 stageHash, i32 passHash, f32 height, f32 length, Color color0, Color color1, Color color2, Color color3, Color color4, Color color5, const mat43 & transform, Entity * pCaller);
i32 shape_hex(i32 stageHash, i32 passHash, f32 height, f32 length, Color color, const mat43 & transform, Entity * pCaller);
i32 shape_hex(i32 stageHash, i32 passHash, f32 height, f32 length, Color color0, Color color1, const mat43 & transform, Entity * pCaller);
i32 shape_hex(i32 stageHash, i32 passHash, f32 height, f32 length, Color color0, Color color1, Color color2, const mat43 & transform, Entity * pCaller);

i32 shape_sphere(i32 stageHash, i32 passHash, const vec3 & size, i32 slices, i32 sections, Color color, const mat43 & transform, Entity * pCaller);
i32 shape_quad_sphere(i32 stageHash, i32 passHash, const vec3 & size, i32 sections, Color color, const mat43 & transform, Entity * pCaller);
} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_SHAPES_H

