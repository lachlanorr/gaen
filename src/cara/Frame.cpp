//------------------------------------------------------------------------------
// Frame.cpp - Base class for all UI frames (text boxes, labels, dialogs, etc)
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

#include "cara/stdafx.h"

#include "assets/Gatl.h"

#include "engine/AssetMgr.h"

#include "cara/Frame.h"

namespace gaen
{

Frame::Frame(task_id owner,
             const Asset* pGatlFont,
             const char * text,
             Color textColor,
             Color backgroundColor)
  : UniqueObject(owner)
  , mpGatlFont(pGatlFont)
  , mText(text)
  , mTextColor(textColor)
  , mBackgroundColor(backgroundColor)
{
    mpGatl = Gatl::instance(mpGatlFont->buffer(), mpGatlFont->size());
    // create quads for each letter
    //pGatlFont->
}

const Gimg & Frame::gimg() const
{
    return *mpGatl->gimg();
}

} // namespace gaen

