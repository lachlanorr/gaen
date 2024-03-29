//------------------------------------------------------------------------------
// Component.h - Core functionality all components must have
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

#ifndef GAEN_ENGINE_COMPONENT_H
#define GAEN_ENGINE_COMPONENT_H

#include "gaen/engine/Message.h"
#include "gaen/engine/Task.h"

namespace gaen
{

enum class PropertyGroup
{
    Current,
    Initial
};

struct ComponentDesc;

class Entity;

// Base component, basically a way for all components
// to have a pointer to their ComponentDesc.
class Component
{
    friend class Entity;
public:
    Component(Entity * pEntity)
      : mpEntity(pEntity) {}

    Task & scriptTask() { return mScriptTask; }

protected:
    const Entity & self() const { return *mpEntity; }
    Entity & self() { return *mpEntity; }

    Task mScriptTask;
    Entity * mpEntity;
    PAD_IF_32BIT_A
    Block *mpBlocks;
    PAD_IF_32BIT_B
    void * mpLastInputHandler = nullptr;
    PAD_IF_32BIT_C;
    f32 mLastInputHandlerDelta = 0.0f;
    u32 mBlockCount;
};

static_assert(sizeof(Component) == 64, "Component unexpected size");

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_COMPONENT_H
