//------------------------------------------------------------------------------
// cooker_utils.h - Shared utilities used by various cookers
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

#ifndef GAEN_CHEF_COOKER_UTILS_H
#define GAEN_CHEF_COOKER_UTILS_H

#include "core/base_defines.h"

namespace gaen
{

enum ImageOrigin
{
    kORIG_TopLeft,
    kORIG_BottomLeft
};


struct ImageInfo
{
    ImageInfo(ImageOrigin origin, u16 width, u16 height)
      : origin(origin)
      , width(width)
      , height(height)
    {
        // sanity check dimensions, as we always work with textures
        PANIC_IF(width != height, "Texture has mismatched width and height: width=%u, height=%u", width, height);
        PANIC_IF(!is_power_of_two(width) || !is_power_of_two(height), "Texture has non power of 2 width or height: width=%u, height=%u", width, height);
    }

    ImageOrigin origin;
    u16 width;
    u16 height;
};

ImageInfo read_image_info(char * path);



} // namespace gaen

#endif // #ifndef GAEN_CHEF_COOKER_UTILS_H
