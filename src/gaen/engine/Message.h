//------------------------------------------------------------------------------
// Message.h - Message definition used to send messages to tasks
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

#ifndef GAEN_ENGINE_MESSAGE_H
#define GAEN_ENGINE_MESSAGE_H

#include "gaen/core/base_defines.h"
#include "gaen/engine/Block.h"

namespace gaen
{

typedef i32 task_id; // defined here since we are the root of the includes, and we need this

static const u32 kMessageFlag_None     = 0;

static const u32 kMessageFlag_ForcePropagate  = 1 << 0; // message should be sent to all children (e.g. save_state), regardless of handlers returning "Consumed" result
static const u32 kMessageFlag_Editor          = 1 << 1; // message originated from editor

static const u32 kMaxBlockCount = 2 << 4;

enum class MessageResult
{
    Propagate,
    Consumed
};

struct Message
{
    u32 msgId;        // hash based on message string
    u32 flags:4;      // message flags
    u32 source:28;    // source task id - NOTE Changing this size requires changing kMaxTaskId in Task.h
    u32 blockCount:4; // count of additional 16 byte payload (e.g. value of 4 means an additional 64 bytes)
    u32 target:28;    // target task id - NOTE Changing this size requires changing kMaxTaskId in Task.h
    cell payload;     // optional payload for the message

    Message() = default;

    Message(u32 msgId,
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
               blockCount    < kMaxBlockCount &&
               source        < (2 << 28) &&
               target        < (2 << 28));
    }

    bool ForcePropagate() const { return (flags & kMessageFlag_ForcePropagate) != 0; }
    bool FromEditor() const { return (flags & kMessageFlag_Editor) != 0; }
};

// 16 bytes is pretty key to the principles of the message passing system.
// We want to maintain alignment, and to be able to quickly write messages
// to a buffer.
// Changing this size will propagate necessary changes... so don't.
static_assert(sizeof(Message) == 16, "Message should be 16 bytes");
static_assert(sizeof(Block) == sizeof(Message), "Block and Message must be the same size");

// We use the 4 extra bits in various places.
inline bool is_valid_task_id(task_id taskId)
{
    return taskId < (2 << 28);
}

inline const Message & block_to_message(const Block & block)
{
    return *reinterpret_cast<const Message*>(&block);
}

inline Message & block_to_message(Block & block)
{
    return *reinterpret_cast<Message*>(&block);
}

inline const Block & message_to_block(const Message & message)
{
    return *reinterpret_cast<const Block*>(&message);
}

inline Block & message_to_block(Message & message)
{
    return *reinterpret_cast<Block*>(&message);
}

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGE_H
