//------------------------------------------------------------------------------
// Component.h - Core functionality all components must have
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

#ifndef GAEN_ENGINE_COMPONENT_H
#define GAEN_ENGINE_COMPONENT_H

#include <glm/mat4x3.hpp>

#include "engine/Message.h"
#include "engine/Task.h"

namespace gaen
{

enum class PropertyGroup
{
    Current,
    Initial
};

enum InitStatus
{
    kIS_Uninitialized = 0,
    kIS_InitData = 1,
    kIS_InitAssets = 2,
    kIS_Init = 3,
    kIS_Fin = 4
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
      : mpEntity(pEntity)
      , mInitStatus(kIS_Uninitialized) {}

    Task & task() { return mScriptTask; }

    InitStatus initStatus() const { return (InitStatus)mInitStatus; }
    void setInitStatus(InitStatus rhs) { mInitStatus = rhs; }

protected:
    const Entity & entity() const { return *mpEntity; }
    Entity & entity() { return *mpEntity; }

    const glm::mat4x3 & transform() const;
    
    Task mScriptTask;
    Entity * mpEntity;
    PAD_IF_32BIT_A
    Block *mpBlocks;
    PAD_IF_32BIT_B
    u32 mBlockCount:24;
    u32 mInitStatus:8;
    u8 PADDING__[12];
};

static_assert(sizeof(Component) == 64, "Component unexpected size");

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_COMPONENT_H
