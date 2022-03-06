//------------------------------------------------------------------------------
// ModelGL.cpp - OpenGL wrapper for models
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

#include "gaen/assets/Gmdl.h"
#include "gaen/assets/Gaim.h"

#include "gaen/renderergl/gaen_opengl.h"
#include "gaen/renderergl/RendererMesh.h"

#include "gaen/renderergl/ModelGL.h"

namespace gaen
{


ModelGL::ModelGL(ModelInstance * pModelInstance, RendererMesh * pRenderer)
  : mpModelInstance(pModelInstance)
  , mpRenderer(pRenderer)
  , mStatus(kRIS_Active)
  , mVertArrayId(0)
  , mVertBufferId(0)
  , mPrimBufferId(0)
  , mTextureId_diffuse(0)
  , mTextureId_animations(0)
  , mFrameOffset(0)
  , mGlPrimType(0)
{
    switch (pModelInstance->model().gmdl().primType())
    {
    case kPRIM_Triangle:
        mGlPrimType = GL_TRIANGLES;
        break;
    case kPRIM_Line:
        mGlPrimType = GL_LINES;
        break;
    case kPRIM_Point:
        mGlPrimType = GL_POINTS;
        break;
    default:
        PANIC("No GLenum for PrimType %u", pModelInstance->model().gmdl().primType());
        break;
    }
}

void ModelGL::loadGpu()
{
    // Load model's verts and tris
    if (mpRenderer->loadVerts(&mVertArrayId,
                              &mVertBufferId,
                              mpModelInstance->model().gmdl().verts(),
                              mpModelInstance->model().gmdl().vertsSize()))
    {
#if HAS(OPENGL3)
        prepareMeshAttributes();
#endif
    }

    mpRenderer->loadPrims(&mPrimBufferId,
                          mpModelInstance->model().gmdl().prims(),
                          mpModelInstance->model().gmdl().primsSize());

    if (mpModelInstance->model().gmdl().mat())
    {
        mTextureId_diffuse = mpRenderer->loadTexture(mpModelInstance->model().gmdl().mat()->texture(kTXTY_Diffuse));
    }
    if (mpModelInstance->model().hasGaim())
    {
        mTextureId_animations = mpRenderer->loadTexture(mpModelInstance->model().gaim().image());
    }

    mpRenderer->unbindBuffers();
}

void ModelGL::unloadGpu()
{
    mpRenderer->unloadVerts(mpModelInstance->model().gmdl().verts());
    mpRenderer->unloadPrims(mpModelInstance->model().gmdl().prims());
    if (mpModelInstance->model().gmdl().mat() != nullptr)
    {
        mpRenderer->unloadTexture(mpModelInstance->model().gmdl().mat()->texture(kTXTY_Diffuse));
    }
}

void ModelGL::render()
{
    if (mTextureId_diffuse > 0)
        mpRenderer->setTexture(HASH::diffuse, mTextureId_diffuse);
    if (mTextureId_animations > 0)
        mpRenderer->setTexture(HASH::animations, mTextureId_animations);
#if HAS(OPENGL3)
    glBindVertexArray(mVertArrayId);
#else
    glBindBuffer(GL_ARRAY_BUFFER, mpModelInstance->vertBufferId);

    prepareMeshAttributes();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimBufferId);
#endif
    glDrawElements(mGlPrimType, mpModelInstance->model().gmdl().indexCount(), GL_UNSIGNED_SHORT, (void*)(uintptr_t)0);

    mpRenderer->unbindBuffers();
}

void ModelGL::prepareMeshAttributes()
{
    const Gmdl & gmdl = mpModelInstance->model().gmdl();
    u32 vertStride = gmdl.vertStride();

    ASSERT(gmdl.hasVertPosition());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertStride, (void*)(uintptr_t)gmdl.vertPositionOffset());
    glEnableVertexAttribArray(0);

    if (gmdl.hasVertNormal())
    {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertStride, (void*)(uintptr_t)gmdl.vertNormalOffset());
        glEnableVertexAttribArray(1);

        if (gmdl.hasVertUv())
        {
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertStride, (void*)(uintptr_t)gmdl.vertUvOffset());
            glEnableVertexAttribArray(2);

            if (gmdl.hasVertBone())
            {
                glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, vertStride, (void*)(uintptr_t)gmdl.vertBoneOffset());
                glEnableVertexAttribArray(3);
            }
            else if (gmdl.hasVertTan())
            {
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, vertStride, (void*)(uintptr_t)gmdl.vertTanOffset());
                glEnableVertexAttribArray(3);
            }
        }
        else if (gmdl.hasVertColor())
        {
            glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertStride, (void*)(uintptr_t)gmdl.vertColorOffset());
            glEnableVertexAttribArray(2);
        }
    }

}

void ModelGL::reportDestruction(u32 uid)
{
    ModelInstance::model_remove(kRendererTaskId, kModelMgrTaskId, uid);
}

} // namespace gaen

