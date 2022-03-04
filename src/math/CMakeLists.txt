#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2021 Lachlan Orr
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
#   1. The origin of this software must not be misrepresented; you must not
#   claim that you wrote the original software. If you use this software
#   in a product, an acknowledgment in the product documentation would be
#   appreciated but is not required.
#
#   2. Altered source versions must be plainly marked as such, and must not be
#   misrepresented as being the original software.
#
#   3. This notice may not be removed or altered from any source
#   distribution.
#-------------------------------------------------------------------------------

set(math_SOURCES
  common.h
  mat3.cpp
  mat3.h
  mat4.cpp
  mat4.h
  mat43.cpp
  mat43.h
  math.h
  matrices.cpp
  matrices.h
  quat.h
  vec2.h
  vec3.cpp
  vec3.h
  vec4.cpp
  vec4.h
  )

#msvc_precompiled_header(math math_SOURCES)
source_group("" FILES ${math_SOURCES})

add_library(math
  ${math_SOURCES}
  )
