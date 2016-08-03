//------------------------------------------------------------------------------
// EntityInit.h - Initialization delegation for compose entities
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

#ifndef GAEN_ENGINE_ENTITY_INIT_H
#define GAEN_ENGINE_ENTITY_INIT_H

namespace gaen
{

class Entity;

typedef void (*EntityInitCallback)(Entity*, void *);

class EntityInit
{
public:
    EntityInit()
      : mpEntity(nullptr)
      , mpCreator(nullptr)
      , mInitCB(nullptr)
      , mInitPropertiesCB(nullptr)
    {}

    EntityInit(Entity * pEntity,
               void * pCreator,
               EntityInitCallback initCB,
               EntityInitCallback initPropertiesCB,
               EntityInitCallback initFieldsCB)
      : mpEntity(pEntity)
      , mpCreator(pCreator)
      , mInitCB(initCB)
      , mInitPropertiesCB(initPropertiesCB)
      , mInitFieldsCB(initFieldsCB)
    {}

    void init()
    {
        if (mpEntity && mpCreator && mInitCB)
            mInitCB(mpEntity, mpCreator);
    }

    void initProperties()
    {
        if (mpEntity && mpCreator && mInitPropertiesCB)
            mInitPropertiesCB(mpEntity, mpCreator);
    }

    void initFields()
    {
        if (mpEntity && mpCreator && mInitFieldsCB)
            mInitFieldsCB(mpEntity, mpCreator);
    }
private:
    Entity * mpEntity;
    void * mpCreator;
    EntityInitCallback mInitCB;
    EntityInitCallback mInitPropertiesCB;
    EntityInitCallback mInitFieldsCB;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ENTITY_INIT_H
