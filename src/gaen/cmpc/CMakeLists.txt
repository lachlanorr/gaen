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

set(cmpc_SOURCES
  main_cmpc.cpp
  )

if(WIN32)
  set(VER_PRODUCTNAME "cmpc")
  set(VER_FILEDESCRIPTION "Compose Compiler")
  set(VER_ORIGINALFILENAME "cmpc.exe")
  configure_file(
    ${CMAKE_SOURCE_DIR}/src/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
    )
  set(cmpc_SOURCES
    ${cmpc_SOURCES}
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    )
endif()

source_group("" FILES ${cmpc_SOURCES})

add_executable(cmpc
  ${cmpc_SOURCES}
  )

target_link_libraries(cmpc
  gaen_compose
  ${PLATFORM_LINK_LIBS}
  )

set_property(TARGET cmpc PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(TARGET cmpc PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "-i engine/shapes.h -i engine/system_api.h ${CMAKE_SOURCE_DIR}/src/scripts/cmp/test.cmp")

package(cmpc)
