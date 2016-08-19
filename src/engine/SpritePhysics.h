//------------------------------------------------------------------------------
// SpritePhysics.h - Sprite physics integration to bullet
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

#ifndef GAEN_ENGINE_SPRITE_PHYSICS_H
#define GAEN_ENGINE_SPRITE_PHYSICS_H

#include <LinearMath/btMotionState.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "core/mem.h"
#include "engine/glm_ext.h"
#include "engine/Sprite.h"


class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace gaen
{

class SpriteBody : public btMotionState
{
    friend class SpritePhysics;
public:
    SpriteBody(SpriteInstance & spriteInstance)
      : mSpriteInstance(spriteInstance)
    {
        mSpriteInstance.mHasBody = true;
    }

    void getWorldTransform(btTransform& worldTrans) const;
    void setWorldTransform(const btTransform& worldTrans);

private:
    UniquePtr<btRigidBody> mpRigidBody;
    SpriteInstance & mSpriteInstance;
};

typedef UniquePtr<SpriteBody> SpriteBodyUP;
typedef UniquePtr<btCollisionShape> btCollisionShapeUP;

class SpritePhysics
{
public:
    SpritePhysics();

    void update(f32 delta);

    void insert(SpriteInstance & spriteInst,
                f32 mass,
                u32 group,
                const glm::uvec4 & mask03,
                const glm::uvec4 & mask47);
    void remove(u32 uid);

    void setVelocity(u32 uid, const glm::vec2 & velocity);
private:
    u16 buildMask(const glm::uvec4 & mask03, const glm::uvec4 & mask47);
    u16 maskFromHash(u32 hash);

    UniquePtr<btBroadphaseInterface> mpBroadphase;
    UniquePtr<btDefaultCollisionConfiguration> mpCollisionConfiguration;
    UniquePtr<btCollisionDispatcher> mpDispatcher;
    UniquePtr<btSequentialImpulseConstraintSolver> mpSolver;
    UniquePtr<btDiscreteDynamicsWorld> mpDynamicsWorld;

    HashMap<kMEM_Physics, u32, SpriteBodyUP> mBodies;
    HashMap<kMEM_Physics, glm::vec3, btCollisionShapeUP> mCollisionShapes;
    HashMap<kMEM_Physics, u32, u16> mMaskBits;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_SPRITE_PHYSICS_H
