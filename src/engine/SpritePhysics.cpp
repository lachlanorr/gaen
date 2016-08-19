//------------------------------------------------------------------------------
// SpritePhysics.cpp - Sprite physics integration to bullet
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

#include "engine/messages/Transform.h"
#include "engine/SpritePhysics.h"


namespace gaen
{

void SpriteBody::getWorldTransform(btTransform& worldTrans) const
{
    const glm::mat4x3 & t = mSpriteInstance.mTransform;
    worldTrans.setBasis(btMatrix3x3(t[0][0], t[0][1], t[0][2],
                                    t[1][0], t[1][1], t[1][2],
                                    t[2][0], t[2][1], t[2][2]));
    worldTrans.setOrigin(btVector3(t[3][0], t[3][1], t[3][2]));
}

void SpriteBody::setWorldTransform(const btTransform& worldTrans)
{
    glm::mat4x3 & t = mSpriteInstance.mTransform;

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
    t[3][2] = worldTrans.getOrigin()[2];

    SpriteInstance::send_sprite_transform(kSpriteMgrTaskId, kRendererTaskId, mSpriteInstance.sprite().uid(), mSpriteInstance.mTransform);
    {
        messages::TransformQW msgw(HASH::transform, kMessageFlag_None, kSpriteMgrTaskId, mSpriteInstance.sprite().owner(), false);
        msgw.setTransform(mSpriteInstance.mTransform);
    }
}



SpritePhysics::SpritePhysics()
{
    mpBroadphase.reset(GNEW(kMEM_Physics, btDbvtBroadphase));
    mpCollisionConfiguration.reset(GNEW(kMEM_Physics, btDefaultCollisionConfiguration));
    mpDispatcher.reset(GNEW(kMEM_Physics, btCollisionDispatcher, mpCollisionConfiguration.get()));
    mpSolver.reset(GNEW(kMEM_Physics, btSequentialImpulseConstraintSolver));
    mpDynamicsWorld.reset(GNEW(kMEM_Physics,
                               btDiscreteDynamicsWorld,
                               mpDispatcher.get(),
                               mpBroadphase.get(),
                               mpSolver.get(),
                               mpCollisionConfiguration.get()));
}

void SpritePhysics::update(f32 delta)
{
    mpDynamicsWorld->stepSimulation(delta);
}

void SpritePhysics::insert(SpriteInstance & spriteInst,
                           f32 mass,
                           u32 group,
                           const glm::uvec4 & mask03,
                           const glm::uvec4 & mask47)
{
    if (mBodies.find(spriteInst.sprite().uid()) == mBodies.end())
    {
        ASSERT(spriteInst.mHasBody == false);
        SpriteBody * pBody = GNEW(kMEM_Physics, SpriteBody, spriteInst);
        mBodies.emplace(spriteInst.sprite().uid(), pBody);

        glm::vec3 halfExtents = spriteInst.sprite().halfExtents();
        auto colShapeIt = mCollisionShapes.find(halfExtents);

        btCollisionShape * pCollisionShape = nullptr;
        if (colShapeIt != mCollisionShapes.end())
            pCollisionShape = colShapeIt->second.get();
        else
        {
            btVector3 btExtents(halfExtents.x, halfExtents.y, halfExtents.z);
            auto empRes = mCollisionShapes.emplace(std::piecewise_construct,
                                                   std::forward_as_tuple(halfExtents),
                                                   std::forward_as_tuple(GNEW(kMEM_Physics, btBox2dShape, btExtents)));
            pCollisionShape = empRes.first->second.get();
        }

        btRigidBody::btRigidBodyConstructionInfo constrInfo(mass, pBody, pCollisionShape);
        pBody->mpRigidBody.reset(GNEW(kMEM_Physics, btRigidBody, constrInfo));
        pBody->mpRigidBody->setLinearFactor(btVector3(1, 1, 0));
        pBody->mpRigidBody->setAngularFactor(btVector3(0, 0, 0));

        pBody->mpRigidBody->setLinearVelocity(btVector3(10, 0, 0));

        if (group == 0)
            mpDynamicsWorld->addRigidBody(pBody->mpRigidBody.get());
        else
        {
            u16 groupMask = maskFromHash(group);
            u16 mask = buildMask(mask03, mask47);
            mpDynamicsWorld->addRigidBody(pBody->mpRigidBody.get(), maskFromHash(group), buildMask(mask03, mask47));
        }
    }
    else
    {
        LOG_ERROR("SpriteBody for %u already created", spriteInst.sprite().uid());
    }
}

void SpritePhysics::remove(u32 uid)
{
    auto it = mBodies.find(uid);
    if (it != mBodies.end())
    {
        mpDynamicsWorld->removeRigidBody(it->second->mpRigidBody.get());
        mBodies.erase(it);
    }
    else
    {
        LOG_ERROR("Cannot find SpriteBody %u to remove", uid);
    }
}

void SpritePhysics::setVelocity(u32 uid, const glm::vec2 & velocity)
{
    auto it = mBodies.find(uid);
    if (it != mBodies.end())
    {
        it->second->mpRigidBody->setLinearVelocity(btVector3(velocity.x, velocity.y, 0));
    }
    else
    {
        LOG_ERROR("Cannot find SpriteBody %u to setVelocity", uid);
    }
}

u16 SpritePhysics::buildMask(const glm::uvec4 & mask03, const glm::uvec4 & mask47)
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

u16 SpritePhysics::maskFromHash(u32 hash)
{
    ASSERT(hash != 0);
    auto it = mMaskBits.find(hash);
    if (it != mMaskBits.end())
        return it->second;
    else
    {
        PANIC_IF(mMaskBits.size() >= 16, "Too many mask bits defined, Bullet only allows for 16");
        u16 mask = (u16)(1 << mMaskBits.size());
        mMaskBits[hash] = mask;
        return mask;
    }
}

} // namespace gaen
