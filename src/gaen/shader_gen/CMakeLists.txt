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

set(shader_gen_SOURCES
  shader_gen.cpp
  glutils.h
  glutils_${platform}.${platform_ext}
  )
source_group("" FILES ${shader_gen_SOURCES})

add_executable(shader_gen
  ${shader_gen_SOURCES}
  )

target_link_libraries(shader_gen
  gaen_assets
  gaen_engine
  glfw
  glad
  ${PLATFORM_LINK_LIBS}
  )

package(shader_gen)
