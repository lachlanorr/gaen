//------------------------------------------------------------------------------
// Component.h - Core functionality all components must have
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

#ifndef GAEN_ENGINE_COMPONENT_H
#define GAEN_ENGINE_COMPONENT_H

#include "engine/Message.h"
#include "engine/Entity.h"

namespace gaen
{

enum class PropertyGroup
{
    Current,
    Initial
};

struct ComponentDesc;

// Base component, basically a way for all components
// to have a pointer to their ComponentDesc.
class Component
{
public:
    Component(ComponentDesc * pCompDesc)
      : mpCompDesc(pCompDesc) {}

protected:
    ComponentDesc * mpCompDesc;
};

#pragma warning(push)
#pragma warning(disable : 4200)
struct ComponentDesc
{
    Task task;
    u32 blockCount:24;
    u32 isInit:8;
    u32 entityTaskId;
    Component component;
    Block blocks[0];

    u32 size() { return sizeof(ComponentDesc)+sizeof(Block)* blockCount; }
    ComponentDesc & next() { *reinterpret_cast<ComponentDesc*>(reinterpret_cast<u8*>(this) + size()); }


    // Disable copy/move, etc since we have 0 byte array
    ComponentDesc(const ComponentDesc&) = delete;
    ComponentDesc(const ComponentDesc&&) = delete;
    ComponentDesc & operator=(const ComponentDesc&) = delete;
    ComponentDesc & operator=(const ComponentDesc&&) = delete;
};
#pragma warning(pop)

static_assert(sizeof(ComponentDesc) == 48, "ComponentDesc unexpected size");

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_COMPONENT_H
