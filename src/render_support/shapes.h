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

#include <glm/vec3.hpp>

#include "assets/Gmdl.h"
#include "engine/Handle.h"
#include "render_support/Model.h"

namespace gaen
{

class ShapeBuilder
{
public:
    ShapeBuilder(Gmdl * pGmdl);

    void addTri(const glm::vec3 & p0,
                const glm::vec3 & p1,
                const glm::vec3 & p2,
                Color color);
    void addTri(const glm::vec3 * pPoints,
                Color color);
    void addTri(const glm::vec4 & p0,
                const glm::vec4 & p1,
                const glm::vec4 & p2,
                Color color);

    void addQuad(const glm::vec3 & p0,
                 const glm::vec3 & p1,
                 const glm::vec3 & p2,
                 const glm::vec3 & p3,
                 Color color);
    void addQuad(const glm::vec3 * pPoints,
                 Color color);
    void addQuad(const glm::vec4 & p0,
                 const glm::vec4 & p1,
                 const glm::vec4 & p2,
                 const glm::vec4 & p3,
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


Gmdl * build_box(const glm::vec3 & size, Color color);
Gmdl * build_cone(const glm::vec3 & size, Color color);
Gmdl * build_cylinder(const glm::vec3 & size, u32 slices, Color color);
Gmdl * build_sphere(const glm::vec3 & size, u32 slices, u32 sections, Color color);
Gmdl * build_quad_sphere(const glm::vec3 & size, u32 sections, Color color);


// Compose wrappers
class Entity;
namespace system_api
{
    i32 create_shape_box(i32 stageHash, const glm::vec3 & size, Color color, Entity & caller);
    i32 create_shape_cone(i32 stageHash, const glm::vec3 & size, i32 slices, Color color, Entity & caller);
    i32 create_shape_cylinder(i32 stageHash, const glm::vec3 & size, i32 slices, Color color, Entity & caller);
    i32 create_shape_sphere(i32 stageHash, const glm::vec3 & size, i32 slices, i32 sections, Color color, Entity & caller);
    i32 create_shape_quad_sphere(i32 stageHash, const glm::vec3 & size, i32 sections, Color color, Entity & caller);
}

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_SHAPES_H

