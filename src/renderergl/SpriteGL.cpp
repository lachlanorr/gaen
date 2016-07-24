//------------------------------------------------------------------------------
// SpriteGL.cpp - OpenGL wrapper for sprites
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

#include "assets/Gatl.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/RendererMesh.h"

#include "renderergl/SpriteGL.h"

namespace gaen
{

void SpriteGL::loadGpu()
{
    // Load images if they're not loaded yet
    textureUnit = 0;
    textureId = mpRenderer->loadTexture(textureUnit, &mpSpriteInstance->sprite().image());

    // Load sprite's verts and tris
#if HAS(OPENGL3)
    glGenVertexArrays(1, &vertArrayId);

    glBindVertexArray(vertArrayId);
#endif
    u64 vertsSize = mpSpriteInstance->sprite().vertsSize();
    const GlyphVert * pVerts = mpSpriteInstance->sprite().verts();

    glGenBuffers(1, &vertBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertBufferId);
    glBufferData(GL_ARRAY_BUFFER, mpSpriteInstance->sprite().vertsSize(), mpSpriteInstance->sprite().verts(), GL_STATIC_DRAW);

#if HAS(OPENGL3)
    prepareMeshAttributes();
#endif
    u64 trisSize = mpSpriteInstance->sprite().trisSize();
    const GlyphTri * pTris = mpSpriteInstance->sprite().tris();

    glGenBuffers(1, &primBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mpSpriteInstance->sprite().trisSize(), mpSpriteInstance->sprite().tris(), GL_STATIC_DRAW);

#if HAS(OPENGL3)
    glBindVertexArray(0);
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteGL::render()
{
#if HAS(OPENGL3)
    glBindVertexArray(vertArrayId);
#else
    glBindBuffer(GL_ARRAY_BUFFER, mpSpriteInstance->vertBufferId);

    prepareMeshAttributes(mesh);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mpSpriteInstance->primBufferId);
#endif
    const void * pOffset = mpSpriteInstance->currentFrameElemsOffset();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, mpSpriteInstance->currentFrameElemsOffset());
}

void SpriteGL::prepareMeshAttributes()
{
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GlyphVert), (void*)(uintptr_t)0);
    glEnableVertexAttribArray(0);
}


} // namespace gaen

