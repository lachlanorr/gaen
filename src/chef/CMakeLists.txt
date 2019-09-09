#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2019 Lachlan Orr
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

SET (cooker_SOURCES
  cookers/Atlas.cpp
  cookers/Atlas.h
  cookers/Font.cpp
  cookers/Font.h
  cookers/Image.cpp
  cookers/Image.h
  cookers/Model.cpp
  cookers/Model.h
  cookers/Sprite.cpp
  cookers/Sprite.h
  )

SET (chef_SOURCES
  Chef.cpp
  Chef.h
  CookerRegistry.cpp
  CookerRegistry.h
  Cooker.h
  cookers.cpp
  cookers.h
  cooker_utils.cpp
  cooker_utils.h
  CookInfo.cpp
  CookInfo.h
  main_chef.cpp
  Png.cpp
  Png.h
  Tga.cpp
  Tga.h
  Tiff.cpp
  Tiff.h
  ${cooker_SOURCES}
  )

MSVC_PRECOMPILED_HEADER (chef chef_SOURCES)
IDE_SOURCE_PROPERTIES( "/" "${chef_SOURCES}")
IDE_SOURCE_PROPERTIES( "/cookers" "${cooker_SOURCES}")

ADD_DEFINITIONS("-DIS_BUILD_Tool=1")

ADD_EXECUTABLE (chef
  ${chef_SOURCES}
  )

TARGET_INCLUDE_DIRECTORIES (chef PRIVATE
  ${PNG_INCLUDE_DIR}
  ${TIFF_INCLUDE_DIR}
  ${FREETYPE_INCLUDE_DIR}
  ${ASSIMP_INCLUDE_DIR}
)

SET (deps
  core
  assets
  zlibstatic
  png_static
  tiff
  freetype
  brotlidec-static
  assimp
  )

ADD_DEPENDENCIES(chef ${deps})
TARGET_LINK_LIBRARIES (chef ${deps})

package (chef)

ADD_CUSTOM_TARGET (
  COOK_ASSETS
  COMMAND ${packaged_dir}/chef
  DEPENDS chef
  )
