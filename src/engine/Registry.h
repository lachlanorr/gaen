//------------------------------------------------------------------------------
// Registry.h - Central factory for components and entities
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

#ifndef GAEN_ENGINE_REGISTRY_H
#define GAEN_ENGINE_REGISTRY_H

#include "core/base_defines.h"
#include "core/HashMap.h"

#include "engine/Component.h"
#include "engine/Entity.h"

namespace gaen
{

template <class T>
class Registry
{
public:
    typedef T*(*Constructor)();

    static bool register_constructor(u32 hash, Constructor constructor)
    {
        auto it = singleton<Registry>().mConstructors.find(hash);

        if (it != singleton<Registry>().mConstructors.end())
        {
            PANIC("Hash already registered: 0x%08x", hash);
            return false;
        }
    
        singleton<Registry>().mConstructors[hash] = constructor;
        return true;
    }
    
    static T* construct(u32 hash)
    {
        auto it = singleton<Registry>().mConstructors.find(hash);

        if (it == singleton<Registry>().mConstructors.end())
        {
            PANIC("Unknown hash, cannot construct: 0x%08x", hash);
            return nullptr;
        }
    
        return it->second();
    }

private:
    HashMap<kMEM_Engine, u32, Constructor> mConstructors;
};

typedef Registry<Component> ComponentRegistry;
typedef Registry<Entity> EntityRegistry;

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_REGISTRY_H
