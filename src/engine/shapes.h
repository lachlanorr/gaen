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

#ifndef GAEN_ENGINE_SHAPES_H
#define GAEN_ENGINE_SHAPES_H

#include "engine/Mesh.h"
#include "engine/Model.h"

namespace gaen
{

class ShapeBuilder
{
public:
    ShapeBuilder(Mesh * pMesh);

    void pushTri(const Vec3 & p0,
                 const Vec3 & p1,
                 const Vec3 & p2);
    void pushTri(const Vec3 * pPoints);

    void pushQuad(const Vec3 & p0,
                  const Vec3 & p1,
                  const Vec3 & p2,
                  const Vec3 & p3);
    void pushQuad(const Vec3 * pPoints);

    void pushMesh(const Mesh & mesh);

    Mesh & mesh() { return mMesh; }
    u32 currVertex() { return mCurrVertex; }
    u32 currIndex() { return mCurrIndex; }

private:
    Mesh & mMesh;
    u32 mCurrVertex = 0;
    u32 mCurrIndex = 0;
};


Mesh * buildTriMesh(f32 width, f32 height);

Model * buildTriModel(f32 width, f32 height, Color color);


} // namespace gaen

#endif // #ifndef GAEN_ENGINE_SHAPES_H

