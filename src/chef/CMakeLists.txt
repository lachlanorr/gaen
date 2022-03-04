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

set(cooker_SOURCES
  cookers/AnimImage.cpp
  cookers/AnimImage.h
  cookers/Atlas.cpp
  cookers/Atlas.h
  cookers/Audio.cpp
  cookers/Audio.h
  cookers/Font.cpp
  cookers/Font.h
  cookers/Image.cpp
  cookers/Image.h
  cookers/Model.cpp
  cookers/Model.h
  cookers/Sprite.cpp
  cookers/Sprite.h
  )

set(chef_SOURCES
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

#msvc_precompiled_header(chef chef_SOURCES)

source_group("" FILES ${chef_SOURCES})
source_group("cookers" FILES ${cooker_SOURCES})

add_compile_definitions(IS_BUILD_Tool=1)

add_executable(chef
  ${chef_SOURCES}
  )

target_include_directories(chef PRIVATE
  ${RAPIDJSON_INCLUDE}
)

target_link_libraries(chef
  core
  assets
  hashes
  math
  ${PROJECT_ASSETS_LIB}
  ${PROJECT_COOKERS_LIB}
  assimp
  freetype
  #harfbuzz
  #harfbuzz-subset
  png_static
  tiff
  zlibstatic
  brotlidec-static
  brotlicommon-static
  ${PLATFORM_LINK_LIBS}
  )

package (chef)

add_custom_target(
  COOK_ASSETS
  COMMAND ${packaged_dir}/chef
  DEPENDS chef
  )
