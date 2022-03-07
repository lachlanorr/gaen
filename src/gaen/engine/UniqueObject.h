//------------------------------------------------------------------------------
// UniqueObject.h - Base class for unique objects with ouid and task owners
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

#ifndef GAEN_ENGINE_UNIQUE_OBJECT_H
#define GAEN_ENGINE_UNIQUE_OBJECT_H

#include "gaen/core/base_defines.h"
#include "gaen/engine/Message.h"

namespace gaen
{

typedef i32 ouid;

class UniqueObject
{
protected:
    UniqueObject(task_id owner)
      : mOwner(owner)
      , mUid(next_uid())
    {}
    UniqueObject(task_id owner, ouid uid)
      : mOwner(owner)
      , mUid(uid)
    {}

    void zero()
    {
        mOwner = 0;
        mUid = 0;
    }
public:
    task_id owner() const { return mOwner; }
    ouid uid() const { return mUid; }

    static ouid next_uid();

private:
    static std::atomic<ouid> sNextOuid;
    task_id mOwner;
    ouid mUid;
};

} // namespace gaen

#endif // GAEN_ENGINE_UNIQUE_OBJECT_H
