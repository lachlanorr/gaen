//------------------------------------------------------------------------------
// Editor.h - Game editor
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

#ifndef GAEN_ENGINE_EDITOR_H
#define GAEN_ENGINE_EDITOR_H

#include "core/base_defines.h"
#if HAS(ENABLE_EDITOR)

namespace gaen
{

class Editor
{
public:
    Editor();

    template <typename T>
    MessageResult message(const T& msgAcc);

    void update(const FrameTime & frameTime);

private:
    void processKeyPress(const ivec4 & keys);
    void unpause();

    bool mIsActive;
    bool mIsPaused;

    bool mCollisionDebug;
}; // class Engine

} // namespace gaen

#endif // #if HAS(ENABLE_EDITOR)

#endif // #ifndef GAEN_ENGINE_EDITOR_H
