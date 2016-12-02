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

SET (gtest_dir ${ext_dir}/gtest-1.7.0)

INCLUDE_DIRECTORIES (${gtest_dir})

SET (unit_test_SOURCES
  BaseFixture.h
  main_testcore.cpp
  test_gamevars.cpp
  test_gamevars_aux.cpp
  test_mem.cpp
  test_ringbuffers.cpp
  test_blockmemory.cpp
  test_math.cpp
  test_task.cpp
  ${gtest_dir}/gtest/gtest-all.cc
  )


# Clang gives harmelss warning about unused variable kPathSeparator.
# Supress it here.
IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  SET_SOURCE_FILES_PROPERTIES(${gtest_dir}/gtest/gtest-all.cc PROPERTIES COMPILE_FLAGS -Wno-unused-variable)
ENDIF()


IDE_SOURCE_PROPERTIES( "/" "${unit_test_SOURCES}")

ADD_EXECUTABLE (unit_tests
  ${unit_test_SOURCES}
  )

TARGET_LINK_LIBRARIES (unit_tests
  core
  engine
  render_support
  compose
  ${PLATFORM_LINK_LIBS}
)

package (unit_tests)

