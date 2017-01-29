//------------------------------------------------------------------------------
// Registry.h - Central factory for components and entities
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

#ifndef GAEN_ENGINE_REGISTRY_H
#define GAEN_ENGINE_REGISTRY_H

#include "core/base_defines.h"
#include "core/HashMap.h"
#include "engine/Message.h"
#include "engine/EntityInit.h"

namespace gaen
{

class Component;
class Entity;

class Registry
{
public:
    typedef Component * (*ComponentConstructor)(void * place, Entity * pEntity);
    typedef Entity * (*EntityConstructor)(u32 childCount, task_id initParentTask, task_id creatorTask, u32 readyMessage);

    bool registerComponentConstructor(u32 nameHash, ComponentConstructor constructor);
    Component * constructComponent(u32 nameHash, void * place, Entity * pEntity);

    bool registerEntityConstructor(u32 nameHash, EntityConstructor constructor);
    Entity * constructEntity(u32 nameHash, u32 childCount, task_id initParentTask, task_id creatorTask, u32 readyMessage);

private:
    HashMap<kMEM_Engine, u32, ComponentConstructor> mComponentConstructors;
    HashMap<kMEM_Engine, u32, EntityConstructor> mEntityConstructors;
    
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_REGISTRY_H
