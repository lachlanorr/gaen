#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gameafar.com
# Copyright (c) 2014-2023 Game Afar LLC
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
  cookers/Qubicle.cpp
  cookers/Qubicle.h
  cookers/Sprite.cpp
  cookers/Sprite.h
  )

set(gaen_cheflib_SOURCES
  Chef.cpp
  Chef.h
  CookerRegistry.cpp
  CookerRegistry.h
  Cooker.h
  cookers.cpp
  cookers.h
  CookInfo.cpp
  CookInfo.h
  ${cooker_SOURCES}
  )

source_group("" FILES ${gaen_cheflib_SOURCES})
source_group("cookers" FILES ${cooker_SOURCES})

add_library(gaen_cheflib STATIC
  ${gaen_cheflib_SOURCES}
  )

set_target_properties(gaen_cheflib PROPERTIES
  FOLDER lib
  )

target_link_libraries(gaen_cheflib
  ${PROJECT_LINK_LIBS}
  gaen_voxel
  assimp
  freetype
  #harfbuzz
  #harfbuzz-subset
  rapidjson
  zlibstatic
  brotlidec-static
  brotlicommon-static
  ${PLATFORM_LINK_LIBS}
  )
