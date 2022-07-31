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

set(gac_SOURCES
  main_gac.cpp
  )

if(WIN32)
  set(VER_PRODUCTNAME "Gac")
  set(VER_FILEDESCRIPTION "Game Afar Console")
  set(VER_ORIGINALFILENAME "gac.exe")
  configure_file(
    ${CMAKE_SOURCE_DIR}/src/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
    )
  set(gac_SOURCES
    ${gac_SOURCES}
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    )
endif()

source_group("" FILES ${gac_SOURCES})

add_executable(gac
  ${gac_SOURCES}
  )

target_link_libraries(gac
  gaen_core
  ${PLATFORM_LINK_LIBS}
  )

package(gac)
