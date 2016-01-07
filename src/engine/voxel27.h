//------------------------------------------------------------------------------
// voxel27.h - Septemvigesimal (27 degree) Voxels
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

#ifndef GAEN_ENGINE_VOXEL27_H
#define GAEN_ENGINE_VOXEL27_H

#include "core/base_defines.h"

namespace gaen
{

struct Voxel27PointData
{
    u32 x:7; // 0-80, each LOD level is a 81x81x81 voxel space
    u32 y:7; // 0-80, each LOD level is a 81x81x81 voxel space
    u32 z:7; // 0-80, each LOD level is a 81x81x81 voxel space
    u32 voxelLevel:4; // base level used for voxel size, we support 16 distinct levels
    u32 material:7; // 128 possible materials

    Voxel27PointData(u32 x, u32 y, u32 z, u32 voxelLevel, u32 material)
      : x(x), y(y), z(z), voxelLevel(voxelLevel), material(material)
    {}
};
static_assert(sizeof(Voxel27PointData) == 4, "Voxel27PointData not 4 bytes");

struct Voxel27Ref
{
    u64 isTerminal:1;
    u64 offset:27; // offset to Voxel27, which are 32 bytes each. Voxel27_addr = (voxel_start_addr + offset * sizeof(Voxel27))
    u64 material:7; // 128 possible materials
    u64 PADDING__:29;

    bool isTerminalEmpty() { return (isTerminal == 1) && (offset == 0); }
    bool isTerminalFull() { return (isTerminal == 1) && (offset != 0); }
    void setTerminalEmpty() { isTerminal = 1; offset = 0; }
    void setTerminalFull() { isTerminal = 1; offset = 1; }
};
static_assert(sizeof(Voxel27Ref) == 8, "Voxel27Ref not 4 bytes");


struct Voxel27
{
    Voxel27Ref children[27];
    Voxel27Ref parent;
    u8 PADDING__[32];
};
static_assert(sizeof(Voxel27) == 256, "Voxel27 not 128 bytes");
    

Voxel27PointData extract_point_data(u32 voxelPoint);

} // namespace gaen



#endif // #ifndef GAEN_ENGINE_VOXEL27_H

