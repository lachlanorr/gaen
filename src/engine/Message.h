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
#include "engine/math.h"
#include "engine/Color.h"

namespace gaen
{

typedef u32 task_id; // defined here since we are the root of the includes, and we need this

static const u32 kMessageFlag_None       = 0;

static const u32 kMessageFlag_Recurse    = 1 << 0; // message should be sent to all children (e.g. save_state)
static const u32 kMessageFlag_Undoable   = 1 << 1; // message originated from editor


enum class MessageResult
{
    Propogate,
    Consumed
};

union cell
{
    i32 i;
    u32 u;
    f32 f;
    bool b;
    Color c;
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

struct Message
{
    fnv msgId;        // fnv1 hash based on message string
    u32 flags:4;      // message flags
    u32 source:28;    // source task id - NOTE Changeing this size requires changing kMaxTaskId in Task.h
    u32 blockCount:4; // count of additional 16 byte payload (e.g. value of 4 means an additional 64 bytes)
    u32 target:28;    // target task id - NOTE Changeing this size requires changing kMaxTaskId in Task.h
    cell payload;     // optional payload for the message

    Message() = default;

    Message(fnv msgId,
            u32 flags,
            task_id source,
            task_id target,
            cell payload = to_cell(0),
            u32 blockCount = 0)
      : msgId(msgId)
      , flags(flags)
      , source(source)
      , blockCount(blockCount)
      , target(target)
      , payload(payload)
    {
        ASSERT(flags         < (2 << 4)  &&
               blockCount    < (2 << 4)  &&
               source        < (2 << 28) &&
               target        < (2 << 28));
    }
};

// Messages can be cast into MessageBlocks to access
// individual cells within.
static const size_t kCellsPerMessageBlock = sizeof(Message) / sizeof(cell);   // 4
static const size_t kDCellsPerMessageBlock = sizeof(Message) / sizeof(dcell); // 2
static const size_t kQCellsPerMessageBlock = sizeof(Message) / sizeof(qcell); // 1
static_assert((kCellsPerMessageBlock == 4) && (sizeof(Message) % sizeof(cell) == 0), "There should be exactly 4 cells per Message");
union MessageBlock
{
    cell c[4];
    dcell d[2];
    qcell q;
};

// 16 bytes is pretty key to the principles of the message passing system.
// We want to maintain alignment, and to be able to quickly write messages
// to a buffer.
// Changing this size will propogate necessary changes... so don't.
static_assert(sizeof(Message) == 16, "Message should be 16 bytes");
static_assert(sizeof(MessageBlock) == sizeof(Message), "MessageBlock and Message must be the same size");

// Returns a MessageBlock indexed off of start.
// Useful when copying data in and out of MessageQueue in
// block sized (16 byte) chunks.
inline MessageBlock & block_at(void * start, size_t index)
{
    return *(reinterpret_cast<MessageBlock*>(start) + index);
}
inline const MessageBlock & block_at(const void * start, size_t index)
{
    return *(reinterpret_cast<const MessageBlock*>(start) + index);
}

// We use the 4 extra bits in various places.
inline bool is_valid_task_id(task_id taskId)
{
    return taskId < (2 << 28);
}


} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGE_H
