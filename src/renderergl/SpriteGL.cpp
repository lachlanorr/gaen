//------------------------------------------------------------------------------
// SpriteGL.cpp - OpenGL wrapper for sprites
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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

#include "assets/Gatl.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/RendererMesh.h"

#include "renderergl/SpriteGL.h"

namespace gaen
{

void SpriteGL::loadGpu()
{
    // Load images if they're not loaded yet
    mTextureId = mpRenderer->loadTexture(&mpSpriteInstance->sprite().gimg());

    // Load sprite's verts and prims
    if (mpRenderer->loadVerts(&mVertArrayId,
                              &mVertBufferId,
                              mpSpriteInstance->sprite().verts(),
                              mpSpriteInstance->sprite().vertsSize()))
    {
#if HAS(OPENGL3)
        prepareMeshAttributes();
#endif
    }

    mpRenderer->loadPrims(&mPrimBufferId,
                          mpSpriteInstance->sprite().tris(),
                          mpSpriteInstance->sprite().trisSize());


    mpRenderer->unbindBuffers();
}

void SpriteGL::unloadGpu()
{
    mpRenderer->unloadTexture(&mpSpriteInstance->sprite().gimg());
    mpRenderer->unloadVerts(mpSpriteInstance->sprite().verts());
    mpRenderer->unloadPrims(mpSpriteInstance->sprite().tris());
}

void SpriteGL::render()
{
    mpRenderer->setTexture(HASH::diffuse, mTextureId);
#if HAS(OPENGL3)
    int err = glGetError();
    glBindVertexArray(mVertArrayId);
#else
    glBindBuffer(GL_ARRAY_BUFFER, mpSpriteInstance->vertBufferId);

    prepareMeshAttributes();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimBufferId);
#endif
    err = glGetError();
    const void * pOffset = mpSpriteInstance->currentFrameElemsOffset();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, mpSpriteInstance->currentFrameElemsOffset());
    err = glGetError();

    mpRenderer->unbindBuffers();

    err = glGetError();
}

void SpriteGL::animate(u32 animHash, u32 animFrameIdx)
{
    mpSpriteInstance->animate(animHash, animFrameIdx);
}

void SpriteGL::prepareMeshAttributes()
{
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GlyphVert), (void*)(uintptr_t)0);
    glEnableVertexAttribArray(0);
}

void SpriteGL::reportDestruction()
{
    SpriteInstance::sprite_remove(kRendererTaskId, kSpriteMgrTaskId, uid());
}

} // namespace gaen

