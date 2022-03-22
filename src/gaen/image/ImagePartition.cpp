//------------------------------------------------------------------------------
// ImagePartition.cpp - Fill an image rect with rects keeping track of free areas
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

#include "gaen/image/ImagePartition.h"

namespace gaen
{

ImagePartition::ImagePartition(uvec2 size)
  : mSize(size)
{
    mRects.insert(Rect{uvec2(0), size});
}

bool ImagePartition::getEmptyPosition(uvec2 * pPos, uvec2 size)
{
    for (auto it = mRects.begin(); it != mRects.end(); ++it)
    {
        if (size.x <= it->size.x && size.y <= it->size.y)
        {
            Rect rightRect{uvec2(it->pos.x + size.x, it->pos.y), uvec2(it->size.x - size.x, size.y)};
            Rect lowerRect{uvec2(it->pos.x, it->pos.y + size.y), uvec2(it->size.x, it->size.y - size.y)};
            *pPos = it->pos;
            mRects.erase(it);
            if (rightRect.size.x > 0 && rightRect.size.y > 0)
                mRects.insert(rightRect);
            if (lowerRect.size.x > 0 && lowerRect.size.y > 0)
                mRects.insert(lowerRect);
            return true;
        }
    }
    return false;
}

} // namespace gaen
