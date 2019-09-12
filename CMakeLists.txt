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

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.12)

SET_PROPERTY (GLOBAL PROPERTY USE_FOLDERS ON)

PROJECT (gaen)

CMAKE_POLICY(SET CMP0054 NEW)

IF (WIN32)
# Rpavlik CreateLauncher still requires this policy for use of LOCATION
# https://github.com/rpavlik/cmake-modules/issues/25
CMAKE_POLICY(SET CMP0026 OLD)
ENDIF (WIN32)



SET (cmake_dir ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
SET (ext_dir ${CMAKE_CURRENT_SOURCE_DIR}/external)
SET (python_dir ${CMAKE_CURRENT_SOURCE_DIR}/python)
SET (src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

INCLUDE (${cmake_dir}/core_defs.cmake)

# If we're compiling gaen as a project (instead of a project that uses
# gaen) set IS_GAEN_PROJECT so we can selectively include some hooks
# for the project to register asset types and cookers.
IF ("${CMAKE_PROJECT_NAME}" STREQUAL "gaen")
  ADD_DEFINITIONS("-DIS_GAEN_PROJECT=1")
ELSE ()
  SET(PROJECT_ASSETS_LIB
    "${CMAKE_PROJECT_NAME}_assets"
    )
  SET(PROJECT_COOKERS_LIB
    "${CMAKE_PROJECT_NAME}_cookers"
    )
ENDIF ()

#-------------------------------------------------------------------------------
# External dependencies
#-------------------------------------------------------------------------------
# Support for creating launch params in Visual Studio
LIST (APPEND CMAKE_MODULE_PATH "${ext_dir}/cmake-modules")
INCLUDE (CreateLaunchers)

ADD_SUBDIRECTORY(external)
ADD_SUBDIRECTORY(src)

# Set debugging params for ALL_BUILD
create_default_target_launcher(
    ${CMAKE_PROJECT_NAME}
    ARGS "-l 127.0.0.1"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)


SET (publish_dir "${CMAKE_BINARY_DIR}/publish")
ADD_CUSTOM_TARGET (
  PUBLISH
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gac> ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets/cooked_win ${publish_dir}/assets
  DEPENDS COOK_ASSETS ${CMAKE_PROJECT_NAME}
  )
