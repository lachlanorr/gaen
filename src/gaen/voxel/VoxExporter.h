//------------------------------------------------------------------------------
// VoxExporter.h - Static functions to write voxel related files
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

#ifndef GAEN_VOXEL_VOX_EXPORTER_H
#define GAEN_VOXEL_VOX_EXPORTER_H

#include "gaen/core/String.h"

namespace gaen
{

struct VoxGeo;
struct VoxSkel;

struct VoxExporter
{
    static Vector<kMEM_Chef, ChefString> write_files(const VoxGeo & voxGeo,
                                                     const VoxSkel * pVoxSkel,
                                                     f32 voxelSize,
                                                     const Gimg & gimg,
                                                     const ChefString & baseName,
                                                     const ChefString & directory);

    static void write_mtl_file(const ChefString & mtlPath,
                               const ChefString & mtlName,
                               const ChefString & pngPath);

    static void write_obj_file(const VoxGeo & voxGeo,
                               f32 voxelSize,
                               const ChefString & objPath,
                               const ChefString & mtlPath,
                               const ChefString & mtlName);

    static void write_png_file(const ChefString & pngPath, const Gimg & gimg);

};

} // namespace gaen

#endif // #ifndef GAEN_VOXEL_VOX_EXPORTERS_H
