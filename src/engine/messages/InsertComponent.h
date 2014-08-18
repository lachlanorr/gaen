//------------------------------------------------------------------------------
// InsertComponentMessage.h - Autogenerated message class for InsertComponent
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

#ifndef GAEN_ENGINE_MESSAGES_INSERTCOMPONENTMESSAGE_H
#define GAEN_ENGINE_MESSAGES_INSERTCOMPONENTMESSAGE_H

#include "engine/MessageWriter.h"
#include "core/threading.h"

namespace gaen
{
namespace msg
{

template <typename T>
class InsertComponentR
{
public:
    InsertComponentR(const T & msgAcc)
      : mMsgAcc(msgAcc)
    {

    }

    u32 nameHash() const { return mMsgAcc.message().payload.u; }
    u32 index() const { return mMsgAcc[0].cells[0].u; }
        
private:
    const T & mMsgAcc;


};

typedef InsertComponentR<MessageQueueAccessor> InsertComponentQR;
typedef InsertComponentR<MessageBlockAccessor> InsertComponentBR;

class InsertComponentQW : protected MessageQueueWriter
{
public:
    InsertComponentQW(u32 msgId,
                      u32 flags,
                      task_id source,
                      task_id target,
                      u32 nameHash)
      : MessageQueueWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(nameHash),
                           1)
    {}
    
    void setNameHash(u32 val) { mMsgAcc.message().payload.u = val; }
    void setIndex(u32 val) { mMsgAcc[0].cells[0].u = val; }
};

class InsertComponentBW : protected MessageBlockWriter
{
public:
    InsertComponentBW(u32 msgId,
                      u32 flags,
                      task_id source,
                      task_id target,
                      Block * pBlocks,
                      u32 blockCount,
                      u32 nameHash)
      : MessageBlockWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(nameHash),
                           1,
                           mBlocks)
    {}

    void setNameHash(u32 val) { mMsgAcc.message().payload.u = val; }
    void setIndex(u32 val) { mMsgAcc[0].cells[0].u = val; }

Block mBlocks[1 + 1]; // +1 for header
};

} // namespace msg
} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGES_INSERTCOMPONENTMESSAGE_H
