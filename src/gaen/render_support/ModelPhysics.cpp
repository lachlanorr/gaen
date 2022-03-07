//------------------------------------------------------------------------------
// ModelPhysics.cpp - Model physics integration to bullet
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

#include "gaen/render_support/stdafx.h"

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>

#include "gaen/assets/Gmdl.h"
#include "gaen/core/gamevars.h"

#include "gaen/engine/messages/PropertyMat43.h"
#include "gaen/engine/messages/Collision.h"
#include "gaen/render_support/ModelPhysics.h"

#define LOG_TRANS_INFO HAS__

namespace gaen
{

GAMEVAR_DECL_BOOL(collision_debug, false);


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
    gaen_to_bullet_transform(worldTrans, mModelInstance.mTransform);
}

void ModelMotionState::setWorldTransform(const btTransform& worldTrans)
{
    if (!mIsMarkedForRemoval)
    {
        mat43 newTrans;
        bullet_to_gaen_transform(newTrans, worldTrans);

        if (newTrans != mModelInstance.mTransform)
        {
#if HAS(LOG_TRANS_INFO)
            vec3 v1{1.0, 2.0, 0.0};
            vec3 v2{1.0, 2.0, -0.0};

            bool eq = v1 == v2;


            LOG_INFO("sWT newTrans:    %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef",
                     newTrans[0][0],
                     newTrans[0][1],
                     newTrans[0][2],
                     newTrans[1][0],
                     newTrans[1][1],
                     newTrans[1][2],
                     newTrans[2][0],
                     newTrans[2][1],
                     newTrans[2][2],
                     newTrans[3][0],
                     newTrans[3][1],
                     newTrans[3][2]
                );
            LOG_INFO("sWT mTransform:  %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef, %1.8ef",
                     mModelInstance.mTransform[0][0],
                     mModelInstance.mTransform[0][1],
                     mModelInstance.mTransform[0][2],
                     mModelInstance.mTransform[1][0],
                     mModelInstance.mTransform[1][1],
                     mModelInstance.mTransform[1][2],
                     mModelInstance.mTransform[2][0],
                     mModelInstance.mTransform[2][1],
                     mModelInstance.mTransform[2][2],
                     mModelInstance.mTransform[3][0],
                     mModelInstance.mTransform[3][1],
                     mModelInstance.mTransform[3][2]
                );
#endif // #if HAS(LOG_TRANS_INFO)

            mModelInstance.mTransform = newTrans;

            // Send transform to entity
            {
                messages::PropertyMat43BW msgw(HASH::set_property, kMessageFlag_None, kModelMgrTaskId, mModelInstance.model().owner(), HASH::transform);
                msgw.setValue(mModelInstance.mTransform);
                send_message(msgw);
            }
        }
    }
}

void ModelBody::setVelocity(const vec3 & velocity)
{
    mVelocity = velocity;
    if (!isKinematic())
    {
        //activate();
        setLinearVelocity(btVector3(mVelocity.x, mVelocity.y, mVelocity.z));
    }
}

void ModelBody::update(f32 delta)
{
    if (isKinematic() && mVelocity != vec3(0.0f))
    {
        mat43 trans = mpMotionState->mModelInstance.mTransform;
        trans[3] += mVelocity * delta;
        setGaenTransform(trans);
    }
}

void ModelBody::setGaenTransform(const mat43 & transform)
{
    // Update bullet
    btTransform btTrans;
    gaen_to_bullet_transform(btTrans, transform);
    mpMotionState->setWorldTransform(btTrans);

    if (!isKinematic())
    {
        activate();
    }
}

void ModelBody::handleCollision(f32 dist, const vec3 & norm, const vec3 & locSelf, const vec3 & locOther) const
{
    if (isKinematic())
    {
        vec3 dir = normalize(velocity());
        if (dir != vec3(0.0f))
        {
            mat43 trans = transform();

            vec3 move;
            if (stopOnCollide())
                move = dir * dist;
            else // slideOnCollide
                move = dot((dir * dist), norm) * norm;

            move = move * linearFactor();
            trans[3] += move;
            {
                // We have to strip const since this function is
                // called from within collision handler and bullet
                // gives us a const pointer to the body.
                ModelBody * bodyMut = const_cast<ModelBody*>(this);
                bodyMut->setGaenTransform(trans);

                if (stopOnCollide())
                    bodyMut->markForRemoval();
            }
        }
    }
}

PhysicsDebugDraw::PhysicsDebugDraw(btDiscreteDynamicsWorld * pDynamicsWorld)
  : mpDynamicsWorld(pDynamicsWorld)
  , mDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb)
  , mLinesUid(0)
{
    mpDynamicsWorld->setDebugDrawer(this);
    mLines.reserve(1024);
}

void PhysicsDebugDraw::update()
{
    if (mLinesUid != 0)
    {
         ModelInstance::model_remove(kModelMgrTaskId, kRendererTaskId, mLinesUid);
         mLinesUid = 0;
    }
    if (collision_debug)
    {
        ASSERT(mLines.size() == 0);
        mpDynamicsWorld->debugDrawWorld();
        render();
    }
}

void PhysicsDebugDraw::render()
{
    if (mLines.size() > 0)
    {
        ASSERT(mLines.size() % 2 == 0);
        Gmdl * pGmdl = Gmdl::create(kVERT_PosNormCol, (u32)mLines.size() * 2, kPRIM_Line, (u32)mLines.size(), HASH::debug_lines);
        VertPosNormCol * pVerts = (VertPosNormCol*)pGmdl->verts();
        for (const Line & l : mLines)
        {
            pVerts->position = l.from;
            pVerts->normal = vec3(0.0f);
            pVerts->color = l.color;
            pVerts++;
            pVerts->position = l.to;
            pVerts->normal = vec3(0.0f);
            pVerts->color = l.color;
            pVerts++;
        }
        PrimLine * pPrims = (PrimLine*)pGmdl->prims();
        for (u32 i = 0; i < mLines.size(); i++)
        {
            pPrims[i].p0 = i * 2;
            pPrims[i].p1 = i * 2 + 1;
        }
        mLines.clear();

        Model * pModel = GNEW(kMEM_Engine, Model, kModelMgrTaskId, pGmdl, true);
        ModelInstance * pModelInst = GNEW(kMEM_Engine, ModelInstance, pModel, HASH::main, kRP_Opaque, kRF_Collision, mat43{1.0f}, true, true);
        mLinesUid = pModel->uid();
        ModelInstance::model_insert(kModelMgrTaskId, kModelMgrTaskId, pModelInst);
    }
}

void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    mLines.emplace_back(vec3(from.x(), from.y(), from.z()), vec3(to.x(), to.y(), to.z()), Color((u8)(color.x() * 255), (u8)(color.y() * 255), (u8)(color.z() * 255)));
}

void PhysicsDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
    LOG_INFO("drawContactPoint");
}

void PhysicsDebugDraw::reportErrorWarning(const char* warningString)
{
    LOG_WARNING("reportErrorWarning: %s", warningString);
}

void PhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
    LOG_WARNING("draw3dText: %s", textString);
}

void PhysicsDebugDraw::setDebugMode(int debugMode)
{
    mDebugMode = debugMode;
}

int PhysicsDebugDraw::getDebugMode() const
{
    return mDebugMode;
}

ModelPhysics::ModelPhysics()
{
    mIsUpdating = false;

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

    mpDebugDraw = GNEW(kMEM_Physics, PhysicsDebugDraw, mpDynamicsWorld);
}

ModelPhysics::~ModelPhysics()
{
    GDELETE(mpDebugDraw);
    GDELETE(mpDynamicsWorld);
    GDELETE(mpSolver);
    GDELETE(mpDispatcher);
    GDELETE(mpCollisionConfiguration);
    GDELETE(mpBroadphase);
}

void ModelPhysics::updateKinematics(f32 delta)
{
    for (auto & bodyPair : mBodies)
    {
        bodyPair.second->update(delta);
    }
}

void ModelPhysics::update(f32 delta)
{
    mIsUpdating = true;

    updateKinematics(delta);

    mpDynamicsWorld->stepSimulation((f32)delta, 0);

    // Check for collisions
    int numManifolds = mpDynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; ++i)
    {
        btPersistentManifold* contactManifold = mpDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const ModelBody* obA = static_cast<const ModelBody*>(contactManifold->getBody0());
        const ModelBody* obB = static_cast<const ModelBody*>(contactManifold->getBody1());

        if (!obA->isMarkedForRemoval() && !obB->isMarkedForRemoval())
        {
            int numContacts = contactManifold->getNumContacts();

            if (numContacts > 0)
            {
                vec3 locA(0.0f);
                vec3 locB(0.0f);
                vec3 norm(0.0f);
                f32 dist = 0.0f;
                for (int j=0; j < numContacts; j++)
                {
                    btManifoldPoint& pt = contactManifold->getContactPoint(j);
                    if (pt.getDistance() < 0.0f)
                    {
                        const btVector3& ptA = pt.getPositionWorldOnA();
                        const btVector3& ptB = pt.getPositionWorldOnB();
                        const btVector3& normalOnB = pt.m_normalWorldOnB;

                        dist += pt.getDistance();
                        locA += vec3(ptA.x(), ptA.y(), ptA.z());
                        locB += vec3(ptB.x(), ptB.y(), ptB.z());
                        norm += vec3(normalOnB.x(), normalOnB.y(), normalOnB.z());
                    }
                }

                dist /= numContacts;
                locA = locA / (f32)numContacts;
                locB = locB / (f32)numContacts;
                norm = norm / (f32)numContacts;

                obA->handleCollision(dist, norm, locA, locB);
                obB->handleCollision(dist, norm, locB, locA);

                // Send collision messages to both entities
                if (obA->message() != 0)
                {
                    messages::CollisionBW msgw(HASH::collision, kMessageFlag_None, kModelMgrTaskId, obA->owner(), obB->groupHash());
                    msgw.setSubject(obB->owner());
                    msgw.setDistance(dist);
                    msgw.setLocationSelf(locA);
                    msgw.setLocationOther(locB);
                    send_message(msgw);
                }
                if (obB->message() != 0)
                {
                    messages::CollisionBW msgw(HASH::collision, kMessageFlag_None, kModelMgrTaskId, obB->owner(), obA->groupHash());
                    msgw.setSubject(obA->owner());
                    msgw.setDistance(dist);
                    msgw.setLocationSelf(locB);
                    msgw.setLocationOther(locA);
                    send_message(msgw);
                }
            }
        }
    }
    mIsUpdating = false;

    for (u32 uid : mBodiesToRemove)
    {
        removeRigidBody(uid);
    }
    mBodiesToRemove.clear();

    mpDebugDraw->update();
}

static btCompoundShape * newOffsetBox(const vec3 & halfExtents, const vec3 & center)
{
    btCollisionShape * pBoxShape = GNEW(kMEM_Physics, btBoxShape, btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
    btCompoundShape * pCompoundShape = GNEW(kMEM_Physics, btCompoundShape);
    btTransform localTrans;
    localTrans.setIdentity();
    localTrans.setOrigin(btVector3(center.x, center.y, center.z));
    pCompoundShape->addChildShape(localTrans, pBoxShape);
    return pCompoundShape;
}

btCompoundShape * ModelPhysics::findBox(const vec3 & halfExtents, const vec3 & center)
{
    HitBox hb{halfExtents, center};
    auto cmpShapeIt = mBoxes.find(hb);

    btCompoundShape * pCompoundShape = nullptr;
    if (cmpShapeIt != mBoxes.end())
        pCompoundShape = cmpShapeIt->second.get();
    else
    {
        btVector3 btExtents(halfExtents.x, halfExtents.y, halfExtents.z);
        auto empRes = mBoxes.emplace(std::piecewise_construct,
                                     std::forward_as_tuple(hb),
                                     std::forward_as_tuple(newOffsetBox(halfExtents, center)));
        pCompoundShape = empRes.first->second.get();
    }
    return pCompoundShape;
}

static btCompoundShape * newOffsetCapsule(f32 radius, f32 height, const vec3 & center)
{
    btCollisionShape * pCapsuleShape = GNEW(kMEM_Physics, btCapsuleShape, radius, height);
    btCompoundShape * pCompoundShape = GNEW(kMEM_Physics, btCompoundShape);
    btTransform localTrans;
    localTrans.setIdentity();
    localTrans.setOrigin(btVector3(center.x, center.y, center.z));
    pCompoundShape->addChildShape(localTrans, pCapsuleShape);
    return pCompoundShape;
}

btCompoundShape * ModelPhysics::findCapsule(f32 radius, f32 height, const vec3 & center)
{
    Capsule cap{radius, height, center};
    auto cmpShapeIt = mCapsules.find(cap);

    btCompoundShape * pCompoundShape = nullptr;
    if (cmpShapeIt != mCapsules.end())
        pCompoundShape = cmpShapeIt->second.get();
    else
    {
        auto empRes = mCapsules.emplace(std::piecewise_construct,
                                        std::forward_as_tuple(cap),
                                        std::forward_as_tuple(newOffsetCapsule(radius, height, center)));
        pCompoundShape = empRes.first->second.get();
    }
    return pCompoundShape;
}

btCollisionShape * ModelPhysics::findConvexHull(const Gmdl * pGmdlPoints)
{
    auto colShapeIt = mConvexHulls.find(pGmdlPoints);

    btCollisionShape * pCollisionShape = nullptr;
    if (colShapeIt != mConvexHulls.end())
        pCollisionShape = colShapeIt->second.get();
    else
    {
        auto empRes = mConvexHulls.emplace(std::piecewise_construct,
                                           std::forward_as_tuple(pGmdlPoints),
                                           std::forward_as_tuple(GNEW(kMEM_Physics, btConvexHullShape, pGmdlPoints->verts(), pGmdlPoints->vertCount(), pGmdlPoints->vertStride())));
        pCollisionShape = empRes.first->second.get();
    }
    return pCollisionShape;
}


void ModelPhysics::insertRigidBody(u32 uid,
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
                                   const ivec4 & mask47)
{
    if (mBodies.find(uid) == mBodies.end())
    {
        if (pMotionState)
            LOG_INFO("insertRigidBody %u, %s, %s", uid, pMotionState->mModelInstance.model().gmdlAssetPath(), pMotionState->mModelInstance.model().gaimAssetPath());
        btRigidBody::btRigidBodyConstructionInfo constrInfo(mass, pMotionState, pCollisionShape);
        gaen_to_bullet_transform(constrInfo.m_startWorldTransform, transform);
        constrInfo.m_friction = friction;

        ModelBody * pBody = GNEW(kMEM_Physics, ModelBody, owner, center, flags, linearFactor, angularFactor, message, group, pMotionState, constrInfo);
        mBodies.emplace(uid, pBody);

        if (pBody->isKinematic())
        {
            pBody->setCollisionFlags(pBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
            pBody->setActivationState(DISABLE_DEACTIVATION);
        }

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
            mpDynamicsWorld->addRigidBody(pBody, groupMask, mask);
        }
        pBody->setGravity(btVector3(0,0,0));
    }
    else
    {
        LOG_ERROR("ModelBody for %u already created", uid);
    }
}

void ModelPhysics::removeRigidBody(u32 uid)
{
    auto it = mBodies.find(uid);
    if (it != mBodies.end())
    {
        if (!mIsUpdating)
        {
            mpDynamicsWorld->removeRigidBody(it->second.get());
            mBodies.erase(it);
        }
        else
        {
            it->second->markForRemoval();
            mBodiesToRemove.emplace(uid);
        }
    }
    else
    {
        LOG_ERROR("Cannot find ModelBody %u to remove", uid);
    }
}

void ModelPhysics::insertCollisionBox(u32 uid,
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
                                      const ivec4 & mask47)
{
    btCompoundShape * pCompoundShape = findBox(halfExtents, center);
    insertRigidBody(uid,
                    owner,
                    pCompoundShape,
                    nullptr,
                    center,
                    transform,
                    mass,
                    friction,
                    false,
                    linearFactor,
                    angularFactor,
                    message,
                    group,
                    mask03,
                    mask47);
}

void ModelPhysics::insertCollisionConvexHull(u32 uid,
                                             task_id owner,
                                             const Gmdl * pGmdlPoints,
                                             const mat43 & transform,
                                             f32 mass,
                                             f32 friction,
                                             u32 message,
                                             u32 group,
                                             const ivec4 & mask03,
                                             const ivec4 & mask47)
{
    btCollisionShape * pCollisionShape = findConvexHull(pGmdlPoints);
    insertRigidBody(uid,
                    owner,
                    pCollisionShape,
                    nullptr,
                    pGmdlPoints->center(),
                    transform,
                    mass,
                    friction,
                    false,
                    vec3(1.0f, 1.0f, 1.0f),
                    vec3(1.0f, 1.0f, 1.0f),
                    message,
                    group,
                    mask03,
                    mask47);
}

void ModelPhysics::setTransform(u32 uid, const mat43 & transform)
{
    auto it = mBodies.find(uid);
    if (it != mBodies.end())
    {
        it->second->setGaenTransform(transform);
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
        it->second->setVelocity(velocity);
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
