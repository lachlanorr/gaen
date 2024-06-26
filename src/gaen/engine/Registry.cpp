//------------------------------------------------------------------------------
// Registry.cpp - Central factory for components and entities
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

#include "gaen/engine/stdafx.h"

#include "gaen/engine/Component.h"
#include "gaen/engine/Entity.h"
#include "gaen/engine/MessageWriter.h"

#include "gaen/engine/Registry.h"

namespace gaen
{

bool Registry::registerComponentConstructor(u32 nameHash, ComponentConstructor constructor)
{
    auto it = mComponentConstructors.find(nameHash);

    if (it != mComponentConstructors.end())
    {
        PANIC("Component hash already registered: 0x%08x", nameHash);
        return false;
    }
    
    mComponentConstructors[nameHash] = constructor;
    return true;
}
    
Component * Registry::constructComponent(u32 nameHash, void * place, Entity * pEntity)
{
    auto it = mComponentConstructors.find(nameHash);

    if (it == mComponentConstructors.end())
    {
        PANIC("Unknown Component hash, cannot construct: 0x%08x", nameHash);
        return nullptr;
    }

    Component * pComponent = it->second(place, pEntity);

    return pComponent;
}


bool Registry::registerEntityConstructor(u32 nameHash, EntityConstructor constructor)
{
    auto it = mEntityConstructors.find(nameHash);

    if (it != mEntityConstructors.end())
    {
        PANIC("Entity hash already registered: 0x%08x", nameHash);
        return false;
    }
    
    mEntityConstructors[nameHash] = constructor;
    return true;
}
    
Entity * Registry::constructEntity(u32 nameHash, u32 childCount, task_id initParentTask, task_id creatorTask, bool isVisible, u32 readyMessage)
{
    auto it = mEntityConstructors.find(nameHash);

    if (it == mEntityConstructors.end())
    {
        PANIC("Unknown Entity hash, cannot construct: 0x%08x", nameHash);
        return nullptr;
    }
    
    Entity * pEntity = it->second(childCount, initParentTask, creatorTask, isVisible, readyMessage);

    return pEntity;
}


} // namespace gaen

