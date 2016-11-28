//------------------------------------------------------------------------------
// ModelPhysics.cpp - Model physics integration to bullet
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

#include "engine/stdafx.h"

#include "assets/Gmdl.h"

#include "engine/messages/Transform.h"
#include "engine/messages/Collision.h"
#include "engine/ModelPhysics.h"


namespace gaen
{

void ModelMotionState::getWorldTransform(btTransform& worldTrans) const
{
    const glm::mat4x3 & t = mModelInstance.mTransform;
    worldTrans.setBasis(btMatrix3x3(t[0][0], t[0][1], t[0][2],
                                    t[1][0], t[1][1], t[1][2],
                                    t[2][0], t[2][1], t[2][2]));

    worldTrans.setOrigin(btVector3(t[3][0], t[3][1], 0.0f /* Don't use t[3][2] for z position since we use it for render order */));
}

void ModelMotionState::setWorldTransform(const btTransform& worldTrans)
{
    glm::mat4x3 & t = mModelInstance.mTransform;

    t[0][0] = worldTrans.getBasis()[0][0];
    t[0][1] = worldTrans.getBasis()[0][1];
    t[0][2] = worldTrans.getBasis()[0][2];

    t[1][0] = worldTrans.getBasis()[1][0];
    t[1][1] = worldTrans.getBasis()[1][1];
    t[1][2] = worldTrans.getBasis()[1][2];

    t[2][0] = worldTrans.getBasis()[2][0];
    t[2][1] = worldTrans.getBasis()[2][1];
    t[2][2] = worldTrans.getBasis()[2][2];

    t[3][0] = worldTrans.getOrigin()[0];
    t[3][1] = worldTrans.getOrigin()[1];

    // Don't set z position since we use it for render order
    ASSERT(worldTrans.getOrigin()[2] == 0.0f);

    ModelInstance::send_model_transform(kModelMgrTaskId, kRendererTaskId, mModelInstance.model().uid(), mModelInstance.mTransform);
    {
        messages::TransformQW msgw(HASH::transform, kMessageFlag_None, kModelMgrTaskId, mModelInstance.model().owner(), false);
        msgw.setTransform(mModelInstance.mTransform);
    }
}



ModelPhysics::ModelPhysics()
{
    mpBroadphase = GNEW(kMEM_Physics, btDbvtBroadphase);
    mpCollisionConfiguration = GNEW(kMEM_Physics, btDefaultCollisionConfiguration);

    mpDispatcher = GNEW(kMEM_Physics, btCollisionDispatcher, mpCollisionConfiguration);
    mpDispatcher->setNearCallback(near_callback);

    mpSolver = GNEW(kMEM_Physics, btSequentialImpulseConstraintSolver);
    mpDynamicsWorld = GNEW(kMEM_Physics,
                           btDiscreteDynamicsWorld,
                           mpDispatcher,
                           mpBroadphase,
                           mpSolver,
                           mpCollisionConfiguration);
}

ModelPhysics::~ModelPhysics()
{
    GDELETE(mpDynamicsWorld);
    GDELETE(mpSolver);
    GDELETE(mpDispatcher);
    GDELETE(mpCollisionConfiguration);
    GDELETE(mpBroadphase);
}

void ModelPhysics::update(f32 delta)
{
    mpDynamicsWorld->stepSimulation(delta);

    // Check for collisions
    int numManifolds = mpDynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; ++i)
    {
        btPersistentManifold* contactManifold = mpDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const ModelBody* obA = static_cast<const ModelBody*>(contactManifold->getBody0());
        const ModelBody* obB = static_cast<const ModelBody*>(contactManifold->getBody1());

        int numContacts = contactManifold->getNumContacts();
        for (int j=0;j<numContacts;j++)
        {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if (pt.getDistance() < 0.f)
            {
                const btVector3& ptA = pt.getPositionWorldOnA();
                const btVector3& ptB = pt.getPositionWorldOnB();
                const btVector3& normalOnB = pt.m_normalWorldOnB;

                // Send collision messages to both entities
                {
                    messages::CollisionQW msgw(HASH::collision, kMessageFlag_None, kModelMgrTaskId, obA->mpMotionState->mModelInstance.model().owner(), obB->mGroupHash);
                    msgw.setSubject(obB->mpMotionState->mModelInstance.model().owner());
                    msgw.setLocation(glm::vec3(ptA.x(), ptA.y(), ptA.z()));
                }
                {
                    messages::CollisionQW msgw(HASH::collision, kMessageFlag_None, kModelMgrTaskId, obB->mpMotionState->mModelInstance.model().owner(), obA->mGroupHash);
                    msgw.setSubject(obA->mpMotionState->mModelInstance.model().owner());
                    msgw.setLocation(glm::vec3(ptB.x(), ptB.y(), ptB.z()));
                }


            }
        }
    }

}

void ModelPhysics::insert(ModelInstance & modelInst,
                          f32 mass,
                          u32 group,
                          const glm::ivec4 & mask03,
                          const glm::ivec4 & mask47)
{
    if (mBodies.find(modelInst.model().uid()) == mBodies.end())
    {
        ASSERT(modelInst.mHasBody == false);

        glm::vec3 halfExtents = modelInst.model().gmdl().halfExtents();
        auto colShapeIt = mCollisionShapes.find(halfExtents);

        btCollisionShape * pCollisionShape = nullptr;
        if (colShapeIt != mCollisionShapes.end())
            pCollisionShape = colShapeIt->second.get();
        else
        {
            btVector3 btExtents(halfExtents.x, halfExtents.y, halfExtents.z);
            auto empRes = mCollisionShapes.emplace(std::piecewise_construct,
                                                   std::forward_as_tuple(halfExtents),
                                                   std::forward_as_tuple(GNEW(kMEM_Physics, btBoxShape, btExtents)));
            pCollisionShape = empRes.first->second.get();
        }

        ModelMotionState * pMotionState = GNEW(kMEM_Physics, ModelMotionState, modelInst);
        btRigidBody::btRigidBodyConstructionInfo constrInfo(mass, pMotionState, pCollisionShape);

        ModelBody * pBody = GNEW(kMEM_Physics, ModelBody, pMotionState, group, constrInfo);
        mBodies.emplace(modelInst.model().uid(), pBody);

        pBody->setLinearFactor(btVector3(1, 1, 0));
        pBody->setAngularFactor(btVector3(0, 0, 0));

        if (group == 0)
        {
            mpDynamicsWorld->addRigidBody(pBody);
        }
        else
        {
            u16 groupMask = maskFromHash(group);
            u16 mask = buildMask(mask03, mask47);
            mpDynamicsWorld->addRigidBody(pBody, maskFromHash(group), buildMask(mask03, mask47));
        }
        pBody->setGravity(btVector3(0,0,0));
    }
    else
    {
        LOG_ERROR("ModelBody for %u already created", modelInst.model().uid());
    }
}

void ModelPhysics::remove(u32 uid)
{
    auto it = mBodies.find(uid);
    if (it != mBodies.end())
    {
        mpDynamicsWorld->removeRigidBody(it->second.get());
        mBodies.erase(it);
    }
    else
    {
        LOG_ERROR("Cannot find ModelBody %u to remove", uid);
    }
}

void ModelPhysics::setVelocity(u32 uid, const glm::vec2 & velocity)
{
    auto it = mBodies.find(uid);
    if (it != mBodies.end())
    {
        it->second->activate();
        it->second->setLinearVelocity(btVector3(velocity.x, velocity.y, 0));
    }
    else
    {
        LOG_ERROR("Cannot find ModelBody %u to setVelocity", uid);
    }
}

u16 ModelPhysics::buildMask(const glm::ivec4 & mask03, const glm::ivec4 & mask47)
{
    u16 mask = 0;
    for (u32 i = 0; i < 4; ++i)
    {
        if (mask03[i])
            mask |= maskFromHash(mask03[i]);
        if (mask47[i])
            mask |= maskFromHash(mask47[i]);
    }
    return mask;
}

u16 ModelPhysics::maskFromHash(u32 hash)
{
    ASSERT(hash != 0);
    auto it = mMaskBits.find(hash);
    if (it != mMaskBits.end())
        return it->second;
    else
    {
        PANIC_IF(mMaskBits.size() >= 12, "Too many mask bits defined, Bullet only allows for 12(ish)");
        u16 mask = (u16)(1 << mMaskBits.size());
        mMaskBits[hash] = mask;
        return mask;
    }
}

void ModelPhysics::near_callback(btBroadphasePair & collisionPair,
                                  btCollisionDispatcher & dispatcher,
                                  const btDispatcherInfo & dispatchInfo)
{
    ModelBody * pBody0 = static_cast<ModelBody*>(collisionPair.m_pProxy0->m_clientObject);
    ModelBody * pBody1 = static_cast<ModelBody*>(collisionPair.m_pProxy1->m_clientObject);

    dispatcher.defaultNearCallback(collisionPair, dispatcher, dispatchInfo);
}


} // namespace gaen
