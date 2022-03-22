//------------------------------------------------------------------------------
// ImagePartition.h - Fill an image rect with rects keeping track of free areas
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

#ifndef GAEN_IMAGE_IMAGE_PARTITION_H
#define GAEN_IMAGE_IMAGE_PARTITION_H

#include "gaen/math/vec2.h"
#include "gaen/core/Set.h"

namespace gaen
{

class ImagePartition
{
public:
    ImagePartition(uvec2 size);
    bool getEmptyPosition(uvec2 * pPos, uvec2 size);

private:
    struct Rect
    {
        uvec2 pos;
        uvec2 size;

        bool operator<(const Rect & rhs) const
        {
            return size.x * size.y < rhs.size.x * rhs.size.y;
        }
    };

    uvec2 mSize;

    MultiSet<kMEM_Chef, Rect> mRects;
};

} // namespace gaen

#endif // #ifndef GAEN_IMAGE_IMAGE_PARTITION_H
