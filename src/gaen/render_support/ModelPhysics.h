//------------------------------------------------------------------------------
// ModelPhysics.h - Model physics integration to bullet
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

#ifndef GAEN_RENDER_SUPPORT_MODEL_PHYSICS_H
#define GAEN_RENDER_SUPPORT_MODEL_PHYSICS_H

#include <LinearMath/btMotionState.h>
#include <LinearMath/btIDebugDraw.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "gaen/core/mem.h"
#include "gaen/core/HashSet.h"
#include "gaen/math/vec3.h"
#include "gaen/render_support/physics.h"
#include "gaen/render_support/collision.h"
#include "gaen/render_support/Model.h"


class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
struct btDispatcherInfo;

namespace gaen
{

class ModelMotionState : public btMotionState
{
    friend class ModelPhysics;
    friend class ModelBody;
public:
    ModelMotionState(ModelInstance & modelInstance)
      : mModelInstance(modelInstance)
      , mIsMarkedForRemoval(false)
    {
        mModelInstance.mHasBody = true;
    }

    void markForRemoval()
    {
        mIsMarkedForRemoval = true;
    }

    virtual void getWorldTransform(btTransform& worldTrans) const;
    virtual void setWorldTransform(const btTransform& worldTrans);

private:
    ModelInstance & mModelInstance;
    bool mIsMarkedForRemoval;
};
typedef UniquePtr<ModelMotionState> ModelMotionStateUP;

class ModelBody : public btRigidBody
{
public:
    ModelBody(task_id owner,
              const vec3 & center,
              u32 flags,
              const vec3 & linearFactor,
              const vec3 & angularFactor,
              u32 message,
              u32 groupHash,
              ModelMotionState * pMotionState,
              const btRigidBodyConstructionInfo& constructionInfo)
      : btRigidBody(constructionInfo)
      , mOwner(owner)
      , mCenter(center)
      , mFlags(flags)
      , mVelocity(0.0f)
      , mLinearFactor(linearFactor)
      , mAngularFactor(angularFactor)
      , mMessage(message)
      , mGroupHash(groupHash)
      , mpMotionState(pMotionState)
      , mIsMarkedForRemoval(false)
    {}

    task_id owner() const { return mOwner; }
    const vec3 & center() const { return mCenter; }
    u32 flags() const { return mFlags; }
    const vec3 & velocity() const { return mVelocity; }
    const vec3 & linearFactor() const { return mLinearFactor; }
    const vec3 & angularFactor() const { return mAngularFactor; }
    bool isKinematic() const { return (mFlags & system_api::PHY_FLAG_KINEMATIC); }
    bool stopOnCollide() const { return (mFlags & system_api::PHY_FLAG_KINEMATIC) && (mFlags & system_api::PHY_FLAG_STOP_ON_COLLIDE); }
    bool slideOnCollide() const { return (mFlags & system_api::PHY_FLAG_KINEMATIC) && !(mFlags & system_api::PHY_FLAG_STOP_ON_COLLIDE) ; }
    u32 message() const { return mMessage; }
    u32 groupHash() const { return mGroupHash; }

    void setVelocity(const vec3 & velocity);
    void update(f32 delta);
    void setGaenTransform(const mat43 & transform);

    const mat43 transform() const { return mpMotionState->mModelInstance.mTransform; }

    bool isMarkedForRemoval() const { return mIsMarkedForRemoval; }

    void handleCollision(f32 dist, const vec3 & normal, const vec3 & locSelf, const vec3 & locTarget) const;

    void markForRemoval()
    {
        mIsMarkedForRemoval = true;
        if (mpMotionState)
        {
            mpMotionState->markForRemoval();
        }
    }

private:
    task_id mOwner;
    vec3 mCenter;
    u32 mFlags;
    u32 mCollisionShape;
    vec3 mVelocity;
    vec3 mLinearFactor;
    vec3 mAngularFactor;
    u32 mMessage;
    u32 mGroupHash;
    ModelMotionStateUP mpMotionState; // LORRNOTE: only reason we have this pointer is so we can delete it when the ModelBody gets destroyed
    bool mIsMarkedForRemoval;
};

class PhysicsDebugDraw : public btIDebugDraw
{
public:
    PhysicsDebugDraw(btDiscreteDynamicsWorld * pDynamicsWorld);

    void update();
    void render();

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
    void reportErrorWarning(const char* warningString) override;
    void draw3dText(const btVector3& location, const char* textString) override;
    void setDebugMode(int debugMode) override;
    int getDebugMode() const override;

    struct Line
    {
        vec3 from;
        vec3 to;
        Color color;

        Line(const vec3 & from, const vec3 & to, Color color)
          : from(from)
          , to(to)
          , color(color)
        {}
    };

private:
    btDiscreteDynamicsWorld * mpDynamicsWorld;
    i32 mDebugMode;
    i32 mLinesUid;
    Vector<kMEM_Physics, Line> mLines;
};

typedef UniquePtr<ModelBody> ModelBodyUP;
typedef UniquePtr<btCollisionShape> btCollisionShapeUP;
typedef UniquePtr<btCompoundShape> btCompoundShapeUP;

class ModelPhysics
{
public:
    ModelPhysics();
    ~ModelPhysics();

    void update(f32 delta);
    void updateKinematics(f32 delta);

    btCompoundShape * findBox(const vec3 & halfExtents, const vec3 & center);
    btCompoundShape * findCapsule(f32 radius, f32 height, const vec3 & center);
    btCollisionShape * findConvexHull(const Gmdl * pGmdlPoints);

    void insertRigidBody(u32 uid,
                         task_id owner,
                         btCollisionShape * pCollisionShape,
                         ModelMotionState * pMotionState,
                         const vec3 & center,
                         const mat43 & transform,
                         f32 mass,
                         f32 friction,
                         u32 flags,
                         const vec3 & linearFactor,
                         const vec3 & angularFactor,
                         u32 message,
                         u32 group,
                         const ivec4 & mask03,
                         const ivec4 & mask47);
    void removeRigidBody(u32 uid);

    void insertCollisionBox(u32 uid,
                            task_id owner,
                            const vec3 & center,
                            const vec3 & halfExtents,
                            const mat43 & transform,
                            f32 mass,
                            f32 friction,
                            const vec3 & linearFactor,
                            const vec3 & angularFactor,
                            u32 message,
                            u32 group,
                            const ivec4 & mask03,
                            const ivec4 & mask47);

    void insertCollisionConvexHull(u32 uid,
                                   task_id owner,
                                   const Gmdl * pGmdlPoints,
                                   const mat43 & transform,
                                   f32 mass,
                                   f32 friction,
                                   u32 message,
                                   u32 group,
                                   const ivec4 & mask03,
                                   const ivec4 & mask47);

    void destroyCollisionConvexHull(u32 uid);

    void setTransform(u32 uid, const mat43 & transform);
    void setVelocity(u32 uid, const vec3 & velocity);
    void setAngularVelocity(u32 uid, const vec3 & velocity);
private:
    u16 buildMask(const ivec4 & mask03, const ivec4 & mask47);
    u16 maskFromHash(u32 hash);

    static void near_callback(btBroadphasePair & collisionPair,
                              btCollisionDispatcher & dispatcher,
                              const btDispatcherInfo & dispatchInfo);

    bool mIsUpdating;

    btBroadphaseInterface * mpBroadphase;
    btDefaultCollisionConfiguration * mpCollisionConfiguration;
    btCollisionDispatcher * mpDispatcher;
    btSequentialImpulseConstraintSolver * mpSolver;
    btDiscreteDynamicsWorld * mpDynamicsWorld;
    PhysicsDebugDraw * mpDebugDraw;

    HashMap<kMEM_Physics, u32, ModelBodyUP> mBodies;
    HashMap<kMEM_Physics, HitBox, btCompoundShapeUP> mBoxes;
    HashMap<kMEM_Physics, Capsule, btCompoundShapeUP> mCapsules;
    HashMap<kMEM_Physics, const Gmdl *, btCollisionShapeUP> mConvexHulls;
    HashMap<kMEM_Physics, u32, u16> mMaskBits;
    HashSet<kMEM_Physics, u32> mBodiesToRemove;
};

} // namespace gaen


#endif // #ifndef GAEN_RENDER_SUPPORT_MODEL_PHYSICS_H
