//------------------------------------------------------------------------------
// voxel27.cpp - Septemvigesimal (27 degree) Voxels
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

#include "engine/stdafx.h"

#include "engine/voxel27.h"


namespace gaen
{

Voxel27PointData extract_point_data(u32 voxelPoint)
{
    return Voxel27PointData(voxelPoint & 0x7f,
                            (voxelPoint & 0x3f80) >> 7,
                            (voxelPoint & 0x1fc000) >> 14,
                            (voxelPoint & 0x1e00000) >> 21,
                            (voxelPoint & 0xfe000000) >> 25);
}

} // namespace gaen
