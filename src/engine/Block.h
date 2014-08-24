//------------------------------------------------------------------------------
// Block.h - Block definition used for message, and prop/fields of components
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

#ifndef GAEN_ENGINE_BLOCK_H
#define GAEN_ENGINE_BLOCK_H

#include "engine/Color.h"
#include "engine/math.h"

namespace gaen
{

union cell
{
    i32 i;
    u32 u;
    f32 f;
    bool b;
    char c;
    Color color;
};

static_assert(sizeof(cell) == 4, "cell must be 4 bytes");

inline cell to_cell(i32 val)
{
    cell c;
    c.i = val;
    return c;
}

inline cell to_cell(u32 val)
{
    cell c;
    c.u = val;
    return c;
}

inline cell to_cell(f32 val)
{
    cell c;
    c.f = val;
    return c;
}

union dcell
{
    i64 i;
    u64 u;
    f64 f;
    void * p;
    cell cells[2];
    // LORRTODO - Add Vec2 here once we implement it
};
static_assert(sizeof(dcell) == 8, "dcell must be 8 bytes");

union qcell
{
    Vec4 vec4;
    cell cells[4];
    dcell dCells[2];
};
static_assert(sizeof(qcell) == 16, "qcell must be 16 bytes");

union Block
{
    cell cells[4];
    dcell dCells[2];
    qcell qCell;
};

// Messages can be cast into Blocks to access
// individual cells within.
static const size_t kBlockSize = sizeof(Block);
static const size_t kCellsPerBlock = sizeof(Block) / sizeof(cell);   // 4
static const size_t kDCellsPerBlock = sizeof(Block) / sizeof(dcell); // 2
static const size_t kQCellsPerBlock = sizeof(Block) / sizeof(qcell); // 1
static_assert((kCellsPerBlock == 4) && (sizeof(Block) % sizeof(cell) == 0), "There should be exactly 4 cells per Message");
static_assert(sizeof(Block) == 16, "Block must be 16 bytes");

// Returns a Block indexed off of start.
// Useful when copying data in and out of MessageQueue in
// block sized (16 byte) chunks.
inline Block & block_at(void * start, size_t index)
{
    return *(reinterpret_cast<Block*>(start) + index);
}
inline const Block & block_at(const void * start, size_t index)
{
    return *(reinterpret_cast<const Block*>(start) + index);
}

inline u32 block_count(u32 cell_count)
{
    u32 blocks = cell_count / kCellsPerBlock;
    if (cell_count % kCellsPerBlock != 0)
        blocks++;
    return blocks;
}

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_BLOCK_H
