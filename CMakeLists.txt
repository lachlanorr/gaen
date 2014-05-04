#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014 Lachlan Orr
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

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.12)

IF (CMAKE_CONFIGURATION_TYPES)
  SET (CMAKE_CONFIGURATION_TYPES "Debug;Release"
    CACHE STRING "Reset the configurations to what we need" FORCE)
ENDIF()

PROJECT (gaen)

SET (cmake_dir ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
SET (ext_dir ${CMAKE_CURRENT_SOURCE_DIR}/external)
SET (scripts_dir ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
SET (src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)


# Include a compiler specific .cmake file.  This weirdness is required
# to distinguish Clang from GCC, as well as to account for build
# wrappers.
IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
  INCLUDE (${cmake_dir}/gcc.cmake)
  ADD_DEFINITIONS("-DIS_COMPILER_CLANG=1")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  INCLUDE (${cmake_dir}/gcc.cmake)
  ADD_DEFINITIONS("-DIS_COMPILER_GCC=1")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
  ADD_DEFINITIONS("-DIS_COMPILER_INTEL=1")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  INCLUDE (${cmake_dir}/win32.cmake)
  ADD_DEFINITIONS("-DIS_COMPILER_MSVC=1")
ENDIF()


IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  INCLUDE (${cmake_dir}/osx.cmake)
  ADD_DEFINITIONS("-DIS_PLATFORM_OSX=1")
ENDIF ()
IF (WIN32)
  ADD_DEFINITIONS("-DIS_PLATFORM_WIN32=1")
ENDIF ()
IF (UNIX)
  INCLUDE (${cmake_dir}/posix.cmake)
  ADD_DEFINITIONS("-DIS_PLATFORM_POSIX=1")
ENDIF ()


ADD_SUBDIRECTORY(src)

