//------------------------------------------------------------------------------
// MessageWriter.cpp - Generic message queue writer class for simple messages
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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

#include "core/thread_local.h"
#include "engine/MessageWriter.h"

namespace gaen
{

static const u32 kMaxThreadLocalBlockCount = 64;
TLARRAY(Block, sBlocks, kMaxThreadLocalBlockCount + 1);

ThreadLocalMessageBlockWriter::ThreadLocalMessageBlockWriter(u32 msgId,
                                                             u32 flags,
                                                             task_id source,
                                                             task_id target,
                                                             cell payload,
                                                             u32 blockCount)
  : MessageBlockWriter(msgId, flags, source, target, payload, blockCount, sBlocks)
{
    if (blockCount > kMaxThreadLocalBlockCount)
        PANIC("Insufficient thread local storage for %u blocks", blockCount);
}

// Access blocks of message
Block & ThreadLocalMessageBlockWriter::operator[] (u32 index)
{
    ASSERT(index < mMsgAcc.available());
    return sBlocks[index+1]; // +1 to skip past header
}

const Block & ThreadLocalMessageBlockWriter::operator[] (u32 index) const
{
    ASSERT(index < mMsgAcc.available());
    return sBlocks[index+1]; // +1 to skip past header
}


} // namespace gaen
