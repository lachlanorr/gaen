//------------------------------------------------------------------------------
// ModelPhysics.cpp - Model physics integration to bullet
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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

#include "render_support/stdafx.h"

#include "assets/Gmdl.h"

#include "engine/messages/PropertyMat43.h"
#include "engine/messages/Collision.h"
#include "render_support/ModelPhysics.h"


namespace gaen
{

void gaen_to_bullet_transform(btTransform & bT, const mat43 & gT)
{
    bT.setBasis(btMatrix3x3(gT[0][0], gT[1][0], gT[2][0],
                            gT[0][1], gT[1][1], gT[2][1],
                            gT[0][2], gT[1][2], gT[2][2]));

    bT.setOrigin(btVector3(gT[3][0], gT[3][1], gT[3][2]));
}

void bullet_to_gaen_transform(mat43 & gT, const btTransform & bT)
{
    gT[0][0] = bT.getBasis()[0][0];
    gT[0][1] = bT.getBasis()[1][0];
    gT[0][2] = bT.getBasis()[2][0];

    gT[1][0] = bT.getBasis()[0][1];
    gT[1][1] = bT.getBasis()[1][1];
    gT[1][2] = bT.getBasis()[2][1];

    gT[2][0] = bT.getBasis()[0][2];
    gT[2][1] = bT.getBasis()[1][2];
    gT[2][2] = bT.getBasis()[2][2];

    gT[3][0] = bT.getOrigin()[0];
    gT[3][1] = bT.getOrigin()[1];
    gT[3][2] = bT.getOrigin()[2];
}

void ModelMotionState::getWorldTransform(btTransform& worldTrans) const
{
    vec3 pos = position(mModelInstance.mTransform);
    LOG_INFO("getWorldTra    pos(%f, %f, %f)", pos.x, pos.y, pos.z);
    gaen_to_bullet_transform(worldTrans, mModelInstance.mTransform);
}

static bool MOVE_FRAME = false;

void ModelMotionState::setWorldTransform(const btTransform& worldTrans)
{
    mat43 newTrans;
    bullet_to_gaen_transform(newTrans, worldTrans);

    if (newTrans != mModelInstance.mTransform)
    {
        MOVE_FRAME = true;
        //LOG_INFO("setWorldTransform uid = %u, pos = {%f, %f, %f}", mModelInstance.uid(), newTrans.cols[3][0], newTrans.cols[3][1], newTrans.cols[3][2]);
        vec3 oldPos = position(mModelInstance.mTransform);
        vec3 newPos = position(newTrans);
        vec3 diff = newPos - oldPos;
        static u32 badCount = 0;
        if (abs(diff.x) < 10.0f)
        {
            badCount++;
            if (badCount > 10)
            {
                int i = 0;
            }
        }
//        LOG_INFO("setWorldTra oldPos(%f, %f, %f) --> newPos(%f, %f, %f)", oldPos.x, oldPos.y, oldPos.z, newPos.x, newPos.y, newPos.z);

        mModelInstance.mTransform = newTrans;

        // Send transform to entity
        {
            messages::PropertyMat43QW msgw(HASH::set_property, kMessageFlag_None, kModelMgrTaskId, mModelInstance.model().owner(), HASH::transform);
            msgw.setValue(mModelInstance.mTransform);
        }
    }
}



ModelPhysics::ModelPhysics()
{
    mTimePrev = mTimeCurr = now();

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

void ModelPhysics::update()
{
    mTimeCurr = now();
    f64 delta = mTimeCurr - mTimePrev;
    mpDynamicsWorld->stepSimulation(delta, 2, 1.0/60.0);
    mTimePrev = mTimeCurr;
    //LOG_INFO("deltal = %f", delta);

    if (MOVE_FRAME)
    {
        LOG_INFO("EOF: %f", delta);
        MOVE_FRAME = false;
    }

    return;
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
                    msgw.setLocation(vec3(ptA.x(), ptA.y(), ptA.z()));
                }
                {
                    messages::CollisionQW msgw(HASH::collision, kMessageFlag_None, kModelMgrTaskId, obB->mpMotionState->mModelInstance.model().owner(), obA->mGroupHash);
                    msgw.setSubject(obA->mpMotionState->mModelInstance.model().owner());
                    msgw.setLocation(vec3(ptB.x(), ptB.y(), ptB.z()));
                }


            }
        }
    }

}

void ModelPhysics::insert(ModelInstance & modelInst,
                          f32 mass,
                          f32 friction,
                          vec3 linearFactor,
                          vec3 angularFactor,
                          u32 group,
                          const ivec4 & mask03,
                          const ivec4 & mask47)
{
    if (mBodies.find(modelInst.model().uid()) == mBodies.end())
    {
        ASSERT(modelInst.mHasBody == false);

        vec3 halfExtents = modelInst.model().gmdl().halfExtents();
        auto colShapeIt = mCollisionShapes.find(halfExtents);
        btVector3 btExtents(halfExtents.x, halfExtents.y, halfExtents.z);

        btCollisionShape * pCollisionShape = nullptr;
        if (colShapeIt != mCollisionShapes.end())
            pCollisionShape = colShapeIt->second.get();
        else
        {
            auto empRes = mCollisionShapes.emplace(std::piecewise_construct,
                                                   std::forward_as_tuple(halfExtents),
                                                   std::forward_as_tuple(GNEW(kMEM_Physics, btBoxShape, btExtents)));
            pCollisionShape = empRes.first->second.get();
        }

        ModelMotionState * pMotionState = GNEW(kMEM_Physics, ModelMotionState, modelInst);
        btRigidBody::btRigidBodyConstructionInfo constrInfo(mass, pMotionState, pCollisionShape);
        constrInfo.m_friction = friction;

//        constrInfo.m_angularDamping = 0.1;
//        constrInfo.m_localInertia = btExtents;

        ModelBody * pBody = GNEW(kMEM_Physics, ModelBody, pMotionState, group, constrInfo);
        mBodies.emplace(modelInst.model().uid(), pBody);

        pBody->setLinearFactor(btVector3(linearFactor.x, linearFactor.y, linearFactor.z));
        pBody->setAngularFactor(btVector3(angularFactor.x, angularFactor.y, angularFactor.z));

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

void ModelPhysics::setTransform(u32 uid, const mat43 & transform)
{
    auto it = mBodies.find(uid);
    if (it != mBodies.end())
    {
        LOG_INFO("setTransform uid = %u, pos = {%f, %f, %f}", uid, transform.cols[3][0], transform.cols[3][1], transform.cols[3][2]);

        // Update bullet
        btTransform btTrans;
        gaen_to_bullet_transform(btTrans, transform);
        it->second->setWorldTransform(btTrans);

        //it->second->getMotionState()->setWorldTransform(btTrans);
        it->second->activate();
    }
    else
    {
        LOG_ERROR("Cannot find ModelBody %u to setTransform", uid);
    }
}

void ModelPhysics::setVelocity(u32 uid, const vec3 & velocity)
{
    auto it = mBodies.find(uid);
    if (it != mBodies.end())
    {
        it->second->activate();
        it->second->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
    }
    else
    {
        LOG_ERROR("Cannot find ModelBody %u to setVelocity", uid);
    }
}

void ModelPhysics::setAngularVelocity(u32 uid, const vec3 & velocity)
{
    auto it = mBodies.find(uid);
    if (it != mBodies.end())
    {
        it->second->activate();
        it->second->setAngularVelocity(btVector3(velocity.x, velocity.y, velocity.z));
    }
    else
    {
        LOG_ERROR("Cannot find ModelBody %u to setVelocity", uid);
    }
}

u16 ModelPhysics::buildMask(const ivec4 & mask03, const ivec4 & mask47)
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
