#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2016 Lachlan Orr
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

SET (chef_SOURCES
  Chef.cpp
  Chef.h
  CookerRegistry.cpp
  CookerRegistry.h
  cookers.cpp
  cookers.h
  cooker_utils.cpp
  cooker_utils.h
  CookInfo.cpp
  CookInfo.h
  main_chef.cpp
  Tga.cpp
  Tga.h
  )

MSVC_PRECOMPILED_HEADER (chef chef_SOURCES)
IDE_SOURCE_PROPERTIES( "/" "${chef_SOURCES}")

ADD_EXECUTABLE (chef
  ${chef_SOURCES}
  )

ADD_DEPENDENCIES(chef
  freetype
  )

TARGET_LINK_LIBRARIES (chef
  core
  assets
  ${PROJECT_ASSETS_LIB}
  ${PROJECT_COOKERS_LIB}
  optimized ${FREETYPE_LIB}
  debug     ${FREETYPE_LIBD}
  ${PLATFORM_LINK_LIBS}
  )

package (chef)

ADD_CUSTOM_TARGET (
  COOK_ASSETS
  COMMAND ${packaged_dir}/chef
  DEPENDS chef
  )
