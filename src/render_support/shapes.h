//------------------------------------------------------------------------------
// shapes.h - Routines to create various geometrical shapes
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

class ShapeBuilder
{
public:
    ShapeBuilder(Gmdl * pGmdl);

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

    void addGmdl(const Gmdl & gmdl);

    Gmdl & gmdl() { return mGmdl; }
    const Gmdl & gmdl() const { return mGmdl; }

    u32 currVertex() { return mCurrVertex; }
    u32 currPrimitive() { return mCurrPrimitive; }

private:
    Gmdl & mGmdl;
    u32 mCurrVertex = 0;
    u32 mCurrPrimitive = 0;
};


Gmdl * build_box(const vec3 & size, Color color, const mat43 & transform);
Gmdl * build_cone(const vec3 & size, u32 slices, Color color, const mat43 & transform);
Gmdl * build_cylinder(const vec3 & size, u32 slices, Color color, const mat43 & transform);
Gmdl * build_sphere(const vec3 & size, u32 slices, u32 sections, Color color, const mat43 & transform);
Gmdl * build_quad_sphere(const vec3 & size, u32 sections, Color color, const mat43 & transform);


// Compose wrappers
class Entity;
namespace system_api
{
i32 shape_box(i32 stageHash, const vec3 & size, Color color, const mat43 & transform, Entity * pCaller);
i32 shape_cone(i32 stageHash, const vec3 & size, i32 slices, Color color, const mat43 & transform, Entity * pCaller);
i32 shape_cylinder(i32 stageHash, const vec3 & size, i32 slices, Color color, const mat43 & transform, Entity * pCaller);
i32 shape_sphere(i32 stageHash, const vec3 & size, i32 slices, i32 sections, Color color, const mat43 & transform, Entity * pCaller);
i32 shape_quad_sphere(i32 stageHash, const vec3 & size, i32 sections, Color color, const mat43 & transform, Entity * pCaller);
} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_SHAPES_H

