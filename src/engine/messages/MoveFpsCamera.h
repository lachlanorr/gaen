//------------------------------------------------------------------------------
// MoveFpsCamera.h - Autogenerated message class for MoveFpsCamera
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#ifndef GAEN_ENGINE_MESSAGES_MOVEFPSCAMERAMESSAGE_H
#define GAEN_ENGINE_MESSAGES_MOVEFPSCAMERAMESSAGE_H

#include "engine/MessageWriter.h"
#include "engine/math.h"
#include "core/threading.h"
#include "engine/Model.h"

namespace gaen
{
namespace messages
{

template <typename T>
class MoveFpsCameraR
{
public:
    MoveFpsCameraR(const T & msgAcc)
      : mMsgAcc(msgAcc)
    {

    }

    const Vec3 & position() const { return *reinterpret_cast<const Vec3*>(&mMsgAcc[0].tCellPad.tCell); }
    f32 yaw() const { return mMsgAcc[0].cells[3].f; }
    f32 pitch() const { return mMsgAcc.message().payload.f; }
        
private:
    const T & mMsgAcc;


};

typedef MoveFpsCameraR<MessageQueueAccessor> MoveFpsCameraQR;
typedef MoveFpsCameraR<MessageBlockAccessor> MoveFpsCameraBR;

class MoveFpsCameraQW : public MessageQueueWriter
{
public:
    MoveFpsCameraQW(u32 msgId,
                    u32 flags,
                    task_id source,
                    task_id target,
                    f32 pitch)
      : MessageQueueWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(*reinterpret_cast<const u32*>(&pitch)),
                           1)
    {}
    
    void setPosition(const Vec3 & val) { mMsgAcc[0].tCellPad.tCell = *reinterpret_cast<const tcell*>(&val); }
    void setYaw(f32 val) { mMsgAcc[0].cells[3].f = val; }
    void setPitch(f32 val) { mMsgAcc.message().payload.f = val; }
};

class MoveFpsCameraBW : public MessageBlockWriter
{
public:
    MoveFpsCameraBW(u32 msgId,
                    u32 flags,
                    task_id source,
                    task_id target,
                    f32 pitch)
      : MessageBlockWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(*reinterpret_cast<const u32*>(&pitch)),
                           1,
                           mBlocks)
    {}

    void setPosition(const Vec3 & val) { mMsgAcc[0].tCellPad.tCell = *reinterpret_cast<const tcell*>(&val); }
    void setYaw(f32 val) { mMsgAcc[0].cells[3].f = val; }
    void setPitch(f32 val) { mMsgAcc.message().payload.f = val; }

private:
    Block mBlocks[1 + 1]; // +1 for header
};

} // namespace msg
} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGES_MOVEFPSCAMERAMESSAGE_H

