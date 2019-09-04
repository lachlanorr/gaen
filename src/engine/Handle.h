//------------------------------------------------------------------------------
// Handle.h - Enables external data references in compose scripts
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

#ifndef GAEN_ENGINE_HANDLE_H
#define GAEN_ENGINE_HANDLE_H

#include "core/base_defines.h"
#include "core/mem.h"
#include "engine/task.h"

namespace gaen
{

class Handle;
typedef void(*HandleReleaseFunc)(Handle & handle);

class Handle
{
public:
    Handle(u32 typeHash, u32 nameOrId, task_id owner, void * pData, HandleReleaseFunc pReleaseFunc)
      : mTypeHash(typeHash)
      , mNameOrId(nameOrId)
      , mOwner(owner)
      , mpData(pData)
      , mpReleaseFunc(pReleaseFunc)
    {
    }

    task_id owner() const { return mOwner; }
    u32 typeHash() const { return mTypeHash; }
    u32 nameOrId() const { return mNameOrId; }

    template <typename T>
    T * data()
    {
        return reinterpret_cast<T*>(mpData);
    }
    template <typename T>
    const T * data() const
    {
        return reinterpret_cast<const T*>(mpData);
    }

    void * data() { return mpData; }
    const void * data() const { return mpData; }

    // Clear the data pointer, useful if handle was temporary
    // and only needed to pass through to another system_api
    // call from a compose script.
    void clearData() { mpData = nullptr; }

    bool isNull() const { return mTypeHash == 0; }

    void release()
    {
        if (mpReleaseFunc)
            mpReleaseFunc(*this);
        GDELETE(this);
    }

private:
    task_id mOwner;
    u32 mTypeHash;
    u32 mNameOrId;
    void * mpData;
    HandleReleaseFunc mpReleaseFunc;
};

typedef Handle* HandleP;
typedef HandleP AssetHandleP;

// Simple release callbacks usable in many cases.
// We want to call the mem.h macros to handle
// memory tracking properly when it is enabled.
void handle_free(Handle & handle);

template <class T>
void handle_delete(Handle & handle)
{
    T * tPtr = reinterpret_cast<T*>(handle.data());
    if (tPtr)
        GDELETE(tPtr);
}

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_HANDLE_H
