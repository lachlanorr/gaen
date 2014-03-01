//------------------------------------------------------------------------------
// Message.h - Message definition used to send messages to tasks
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

#ifndef GAEN_ENGINE_MESSAGE_H
#define GAEN_ENGINE_MESSAGE_H

#include "core/base_defines.h"

#include "engine/FNV.h"

namespace gaen
{

typedef u32 task_id; // defined here since we are the root of the includes, and we need this

static const u32 kMessageFlag_TaskMaster = 1 << 0;  // This message is intended for the TaskMaster.


enum class MessageResult
{
    Propogate,
    Consumed,
    Unhandled
};

union cell
{
    i32 i;
    u32 u;
    f32 f;
    struct color
    {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    };
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
    cell cells[2];
};
static_assert(sizeof(dcell) == 8, "dcell must be 8 bytes");

union qcell
{
    cell cells[4];
    dcell dcells[2];
    // LORRTODO - Add Vec4 here once we implement it
    // LORRTODO - Consider adding 128 bit simd stuff here
};
static_assert(sizeof(qcell) == 16, "qcell must be 16 bytes");

struct Message
{
    fnv msgId;       // fnv1 hash based on message string
    u32 flags:4;     // message flags
    u32 source:28;   // source task id
    u32 size:4;      // count of additional 16 byte payload (e.g. value of 4 means an additional 64 bytes)
    u32 target:28;   // target task id
    cell payload;    // optional payload for the message
};

// Messages can be cast into MessageBlocks to access
// individual cells within.
static const size_t kCellsPerMessageBlock = sizeof(Message) / sizeof(cell);
static const size_t kDcellsPerMessageBlock = kCellsPerMessageBlock / 2;
static_assert((kCellsPerMessageBlock == 4) && (sizeof(Message) % sizeof(cell) == 0), "There should be exactly 4 cells per Message");
union MessageBlock
{
    cell cells[kCellsPerMessageBlock];
    dcell dcells[kCellsPerMessageBlock / 2];
    qcell qcells[kCellsPerMessageBlock / 4];
};

// 16 bytes is pretty key to the principles of the message passing system.
// We want to maintain alignment, and to be able to quickly write messages
// to a buffer.
// Changing this size will propogate necessary changes... so don't.
static_assert(sizeof(Message) == 16, "Message should be 16 bytes");
static_assert(sizeof(MessageBlock) == sizeof(Message), "MessageBlock and Message must be the same size");

// We use the 4 extra bits in various places.
inline bool is_valid_task_id(task_id taskId)
{
    return taskId < (2 << 28);
}


} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGE_H
