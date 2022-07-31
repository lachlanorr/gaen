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

set(gaen_test_SOURCES
  BaseFixture.h
  main_testcore.cpp
  test_gamevars.cpp
  test_gamevars_aux.cpp
  test_mem.cpp
  test_ringbuffers.cpp
  test_blockmemory.cpp
  test_math.cpp
  test_task.cpp
  )

if(WIN32)
  set(VER_PRODUCTNAME "gaen_tests")
  set(VER_FILEDESCRIPTION "Gaen Unit Tests")
  set(VER_ORIGINALFILENAME "gaen_tests.exe")
  configure_file(
    ${CMAKE_SOURCE_DIR}/src/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
    )
  set(gaen_test_SOURCES
    ${gaen_test_SOURCES}
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    )
endif()

source_group("" FILES ${gaen_test_SOURCES})

add_executable(gaen_tests
  ${gaen_test_SOURCES}
  )

target_link_libraries(gaen_tests
  gaen_engine
  gaen_render_support
  gaen_compose
  gtest
  ${PLATFORM_LINK_LIBS}
)

package(gaen_tests)
