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

set(gaen_assets_SOURCES
  AssetHeader.h
  Color.h
  Config.cpp
  Config.h
  file_utils.cpp
  file_utils.h
  file_utils_${platform}.cpp
  Gaim.cpp
  Gaim.h
  Gatl.cpp
  Gatl.h
  Gaud.cpp
  Gaud.h
  Gimg.cpp
  Gimg.h
  Gmat.h
  Gmat.cpp
  Gmdl.h
  Gmdl.cpp
  Gspr.cpp
  Gspr.h
  )

source_group("" FILES ${gaen_assets_SOURCES})

add_library(gaen_assets STATIC
  ${gaen_assets_SOURCES}
  )

set_target_properties(gaen_assets PROPERTIES
  FOLDER lib
  )

target_link_libraries(gaen_assets PUBLIC
  gaen_math
  gaen_hashes
  )
