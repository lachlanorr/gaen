//------------------------------------------------------------------------------
// VoxGeo.cpp - Voxel points, polys, uvs, etc
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2022 Lachlan Orr
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

#include "gaen/voxel/Qbt.h"

#include "gaen/voxel/VoxGeo.h"

namespace gaen
{

VoxGeo::VoxGeo(const Vector<kMEM_Chef, const VoxMatrix*> & matrices)
{
    meshes.reserve(matrices.size());
    for (const auto pMatrix : matrices)
    {
        meshes.push_back(UniquePtr<VoxMesh>(GNEW(kMEM_Chef, VoxMesh, pMatrix->node.name)));
        VoxMesh & mesh = *meshes.back();
        mesh.polys.resize(pMatrix->faces.size());

        // build face
        for (size_t i = 0; i < pMatrix->faces.size(); ++i)
        {
            VoxPoly & poly = mesh.polys[i];
            poly.side = pMatrix->faces[i].pFace->side;
            poly.pointCount = 4;
            poly.points[0] = {pMatrix->faces[i].points[0], pMatrix->faces[i].uvs[0]};
            poly.points[1] = {pMatrix->faces[i].points[1], pMatrix->faces[i].uvs[1]};
            poly.points[2] = {pMatrix->faces[i].points[2], pMatrix->faces[i].uvs[2]};
            poly.points[3] = {pMatrix->faces[i].points[3], pMatrix->faces[i].uvs[3]};
            poly.points[4] = {vec3(0), vec2(0)};
        }
    }
}

} // namespace gaen
