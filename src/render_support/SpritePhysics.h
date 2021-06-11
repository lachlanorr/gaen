//------------------------------------------------------------------------------
// SpritePhysics.h - Sprite physics integration to bullet
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#ifndef GAEN_RENDER_SUPPORT_SPRITE_PHYSICS_H
#define GAEN_RENDER_SUPPORT_SPRITE_PHYSICS_H

#include <LinearMath/btMotionState.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "math/vec2.h"
#include "math/vec3.h"

#include "core/mem.h"
#include "render_support/Sprite.h"


class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
struct btDispatcherInfo;

namespace gaen
{

class SpriteMotionState : public btMotionState
{
    friend class SpritePhysics;
    friend class SpriteBody;
public:
    SpriteMotionState(SpriteInstance & spriteInstance)
      : mSpriteInstance(spriteInstance)
    {
        mSpriteInstance.mHasBody = true;
    }

    void getWorldTransform(btTransform& worldTrans) const;
    void setWorldTransform(const btTransform& worldTrans);

private:
    SpriteInstance & mSpriteInstance;
};
typedef UniquePtr<SpriteMotionState> SpriteMotionStateUP;

class SpriteBody : public btRigidBody
{
    friend class SpritePhysics;
public:
    SpriteBody(SpriteMotionState * pMotionState, u32 groupHash, const btRigidBodyConstructionInfo& constructionInfo)
      : btRigidBody(constructionInfo)
      , mpMotionState(pMotionState)
      , mGroupHash(groupHash)
    {
        mpMotionState->mSpriteInstance.mHasBody = true;
    }

private:
    SpriteMotionStateUP mpMotionState;
    u32 mGroupHash;
};

typedef UniquePtr<SpriteBody> SpriteBodyUP;
typedef UniquePtr<btCollisionShape> btCollisionShapeUP;

class SpritePhysics
{
public:
    SpritePhysics();
    ~SpritePhysics();

    void update(f32 delta);

    void insert(SpriteInstance & spriteInst,
                f32 mass,
                u32 group,
                const ivec4 & mask03,
                const ivec4 & mask47);
    void remove(u32 uid);

    void setVelocity(u32 uid, const vec2 & velocity);
private:
    u16 buildMask(const ivec4 & mask03, const ivec4 & mask47);
    u16 maskFromHash(u32 hash);

    static void near_callback(btBroadphasePair & collisionPair,
                              btCollisionDispatcher & dispatcher,
                              const btDispatcherInfo & dispatchInfo);

    btBroadphaseInterface * mpBroadphase;
    btDefaultCollisionConfiguration * mpCollisionConfiguration;
    btCollisionDispatcher * mpDispatcher;
    btSequentialImpulseConstraintSolver * mpSolver;
    btDiscreteDynamicsWorld * mpDynamicsWorld;

    HashMap<kMEM_Physics, u32, SpriteBodyUP> mBodies;
    HashMap<kMEM_Physics, vec3, btCollisionShapeUP> mCollisionShapes;
    HashMap<kMEM_Physics, u32, u16> mMaskBits;
};

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_SPRITE_PHYSICS_H
