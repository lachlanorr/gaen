#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2022 Lachlan Orr
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

set(gaen_voxel_SOURCES
  Qbt.cpp
  Qbt.h
  VoxExporter.cpp
  VoxExporter.h
  VoxGeo.cpp
  VoxGeo.h
  VoxMatrix.cpp
  VoxMatrix.h
  VoxSkel.cpp
  VoxSkel.h
  vox_types.h
  vox_types.cpp

  VoxObj.h
  VoxObj.cpp
)

set(gaen_voxel_types_SOURCES
  # VoxObj subclasses
  types/VoxBiped.h
  types/VoxBiped.cpp
  types/VoxBow.h
  types/VoxBow.cpp
  types/VoxMissile.h
  types/VoxMissile.cpp
  types/VoxProp.h
  types/VoxProp.cpp
  types/VoxQuiver.h
  types/VoxQuiver.cpp
  types/VoxWeapon.h
  types/VoxWeapon.cpp
  )

source_group("" FILES ${gaen_voxel_SOURCES})
source_group("types" FILES ${gaen_voxel_types_SOURCES})

add_library(gaen_voxel STATIC
  ${gaen_voxel_SOURCES}
  ${gaen_voxel_types_SOURCES}
  )

set_target_properties(gaen_voxel PROPERTIES
  FOLDER lib
  )

target_link_libraries(gaen_voxel PUBLIC
  gaen_image
  zlibstatic
  )
