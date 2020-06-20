//------------------------------------------------------------------------------
// ModelPhysics.h - Model physics integration to bullet
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "core/mem.h"
#include "math/vec3.h"
#include "render_support/physics.h"
#include "render_support/Model.h"


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
    {
        mModelInstance.mHasBody = true;
    }

    virtual void getWorldTransform(btTransform& worldTrans) const;
    virtual void setWorldTransform(const btTransform& worldTrans);

private:
    UniquePtr<btRigidBody> mpRigidBody;
    ModelInstance & mModelInstance;
};
typedef UniquePtr<ModelMotionState> ModelMotionStateUP;

class ModelBody : public btRigidBody
{
    friend class ModelPhysics;
public:
    ModelBody(ModelMotionState * pMotionState, u32 groupHash, const btRigidBodyConstructionInfo& constructionInfo)
      : btRigidBody(constructionInfo)
      , mpMotionState(pMotionState)
      , mGroupHash(groupHash)
    {
        mpMotionState->mModelInstance.mHasBody = true;
    }

private:
    ModelMotionStateUP mpMotionState;
    u32 mGroupHash;
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

class ModelPhysics
{
public:
    ModelPhysics();
    ~ModelPhysics();

    void update();
    void resetLastFrameTime();
    void render();

    void insert(ModelInstance & modelInst,
                f32 mass,
                f32 friction,
                vec3 linearFactor,
                vec3 angularFactor,
                u32 group,
                const ivec4 & mask03,
                const ivec4 & mask47);
    void remove(u32 uid);

    void insertCollisionBox(u32 uid, const vec3 & halfExtents, const mat43 & transform);

    void setTransform(u32 uid, const mat43 & transform);
    void setVelocity(u32 uid, const vec3 & velocity);
    void setAngularVelocity(u32 uid, const vec3 & velocity);
private:
    u16 buildMask(const ivec4 & mask03, const ivec4 & mask47);
    u16 maskFromHash(u32 hash);

    static void near_callback(btBroadphasePair & collisionPair,
                              btCollisionDispatcher & dispatcher,
                              const btDispatcherInfo & dispatchInfo);

    f64 mTimePrev;
    f64 mTimeCurr;

    btBroadphaseInterface * mpBroadphase;
    btDefaultCollisionConfiguration * mpCollisionConfiguration;
    btCollisionDispatcher * mpDispatcher;
    btSequentialImpulseConstraintSolver * mpSolver;
    btDiscreteDynamicsWorld * mpDynamicsWorld;
    PhysicsDebugDraw * mpDebugDraw;

    HashMap<kMEM_Physics, u32, ModelBodyUP> mBodies;
    HashMap<kMEM_Physics, vec3, btCollisionShapeUP> mCollisionShapes;
    HashMap<kMEM_Physics, u32, u16> mMaskBits;
};

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_MODEL_PHYSICS_H
