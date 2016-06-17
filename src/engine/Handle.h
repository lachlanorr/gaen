//------------------------------------------------------------------------------
// Handle.h - Manages data reference sharing and access from Compose scripts
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

namespace gaen
{

class Handle;
typedef void(*HandleFreeFunc)(Handle & handle);

class Handle
{
public:
    Handle(u32 typeHash, u32 nameHash, u32 ownerTaskId, u32 dataSize, void * pData, HandleFreeFunc pFreeFunc)
      : mTypeHash(typeHash)
      , mNameHash(nameHash)
      , mOwnerTaskId(ownerTaskId)
      , mDataSize(dataSize)
      , mpData(pData)
      , mpFreeFunc(pFreeFunc)
    {
    }

    void free()
    {
        if (mpFreeFunc)
            mpFreeFunc(*this);
    }

    u32 typeHash() const { return mTypeHash; }
    u32 nameHash() const { return mNameHash; }
    u32 ownerTaskId() const { return mOwnerTaskId; }

    u32 dataSize() const { return mDataSize; }
    void * data() { return mpData; }
    const void * data() const { return mpData; }

    bool isNull() const { return mTypeHash == 0; }
    static Handle null() { return Handle(0, 0, 0, 0, nullptr, nullptr); }

private:
    u32 mTypeHash;
    u32 mNameHash;
    u32 mOwnerTaskId;
    u32 mDataSize;
    void * mpData;
    PAD_IF_32BIT_A
    HandleFreeFunc mpFreeFunc;
    PAD_IF_32BIT_B
};

// We have 4 extra bytes for padding, but 32 will keep these
// 16-byte block alligned properly if we need an array of them.
static_assert(sizeof(Handle)==32, "Handle should be 32 bytes");


// Simple free callbacks usable in most cases.
// We want to call the mem.h macros to handle
// memory tracking propertly when it is enabled.
inline void handle_free(Handle & handle)
{
    GFREE(handle.data());
}
template <class T>
inline void handle_delete(Handle & handle)
{
    T * tPtr = reinterpret_cast<T*>(handle.data());
    GDELETE(tPtr);
}

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_HANDLE_H
