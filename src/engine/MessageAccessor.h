//------------------------------------------------------------------------------
// MessageAccessor.h - Classes to access data within messaeges
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

#ifndef GAEN_ENGINE_MESSAGEACCESSOR_H
#define GAEN_ENGINE_MESSAGEACCESSOR_H

#include "core/SpscRingBuffer.h"
#include "engine/Message.h"

namespace gaen
{

template <typename T, typename U>
void copy_message_blocks(T * target, const U & msgAcc, u32 blockIndex)
{
    static_assert(kBlockSize == 16, "kBlockSize must be 16 bytes"); // we do some optimized math here, and it requires 16 byte blocks
    u32 targetSize = (u32)sizeof(T);
    ASSERT_MSG(targetSize >> 4 << 4 == targetSize, "Target must be a multiple of 16 bytes");
    Block * pBlocks = reinterpret_cast<Block*>(target);
    u32 blockCount = targetSize >> 4; // divide by 16
    for (u32 i = 0; i < blockCount; ++i)
    {
        pBlocks[i] = msgAcc[i + blockIndex];
    }
}
template <typename T, typename U>
void copy_message_cells(T * target, const U & msgAcc, u32 blockIndex, u32 cellIndex)
{
    u32 targetSize = (u32)sizeof(T);
    ASSERT_MSG(targetSize <= kBlockSize - (cellIndex * kCellsPerBlock), "Target cells not contained within a single block");
    cell * pCells = reinterpret_cast<cell*>(target);
    u32 cellCount = cell_count(targetSize);
    for (u32 i = 0; i < cellCount; ++i)
    {
        pCells[i] = msgAcc[blockIndex].cells[i + cellIndex];
    }
}


// Provides cell access to message.  Message Header contains one 32 bit cell,
// each subsequent "Message Header" contains 4 more cells.
class MessageQueueAccessor
{
    friend class MessageQueue;
public:
    inline Message & message()
    {
        ASSERT(mAccessor.available() > 0);
        return mAccessor[0];
    }

    inline const Message & message() const
    {
        ASSERT(mAccessor.available() > 0);
        return mAccessor[0];
    }

    // Access blocks of message
    inline Block & operator[] (u32 index)
    {
        ASSERT(mAccessor.available() > 0);
        ASSERT(index < mAccessor[0].blockCount);
        return message_to_block(mAccessor[index+1]); // +1 to skip past header
    }

    inline const Block & operator[] (u32 index) const
    {
        ASSERT(mAccessor.available() > 0);
        ASSERT(index < mAccessor[0].blockCount);
        return message_to_block(mAccessor[index+1]); // +1 to skip past header
    }

    inline u32 available() const
    {
        return mAccessor.available();
    }

    template <typename T>
    inline T extract(u32 blockIndex) const
    {
        T val;
        copy_message_blocks<T>(&val, *this, blockIndex);
        return val;
    }

    template <typename T>
    inline T extract(u32 blockIndex, u32 cellIndex) const
    {
        T val;
        copy_message_cells<T>(&val, *this, blockIndex, cellIndex);
        return val;
    }

private:
    mutable SpscRingBuffer<Message>::Accessor mAccessor;
};

// Similar to MessageQueueAccessor, but used when writing/reading a
// message from raw blocks. An example of where this is useful is when
// you want to send a message to a task you have a reference to, and
// don't want to incur the cost of going through the MessageQueue
// mechanism.
class MessageBlockAccessor
{
public:
    MessageBlockAccessor(Block * pBlocks, u32 blockCount)
      : mpBlocks(pBlocks)
      , mBlockCount(blockCount)
    {
        ASSERT(blockCount <= kMaxBlockCount);
    }

    inline Message & message()
    {
        return block_to_message(mpBlocks[0]);
    }

    inline const Message & message() const
    {
        return block_to_message(mpBlocks[0]);
    }

    // Access blocks of message
    inline Block & operator[] (u32 index)
    {
        ASSERT(index < mBlockCount);
        return mpBlocks[index+1]; // +1 to skip past header
    }

    inline const Block & operator[] (u32 index) const
    {
        ASSERT(index < mBlockCount);
        return mpBlocks[index+1]; // +1 to skip past header
    }

    inline u32 available() const
    {
        return mBlockCount;
    }

    template <typename T>
    inline T extract(u32 blockIndex) const
    {
        T val;
        copy_message_blocks<T>(&val, *this, blockIndex);
        return val;
    }

    template <typename T>
    inline T extract(u32 blockIndex, u32 cellIndex) const
    {
        T val;
        copy_message_cells<T>(&val, *this, blockIndex, cellIndex);
        return val;
    }

private:
    Block * mpBlocks;
    u32 mBlockCount;
};



} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGEACCESSOR_H
