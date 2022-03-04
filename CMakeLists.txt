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

cmake_minimum_required(VERSION 3.22.0)

set(CMAKE_CXX_STANDARD 11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(gaen)

option(gaen_BUILD_TESTS "Build tests" ON)
option(gaen_MINEXT "Strip out all but the minimal targets in external dependencies" ON)

set(ext_dir ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(python_dir ${CMAKE_CURRENT_SOURCE_DIR}/python)

# Set these to avoid any external libs overwriting
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "")
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "")
set(CMAKE_MINSIZEREL_POSTFIX "" CACHE STRING "")
set(CMAKE_RELWITHDEBINFO_POSTFIX "" CACHE STRING "")

include(compile_defs.cmake)

# If we're compiling gaen as a project (instead of a project that uses
# gaen) set IS_GAEN_PROJECT so we can selectively include some hooks
# for the project to register asset types and cookers.
if("${CMAKE_PROJECT_NAME}" STREQUAL "gaen")
  add_compile_definitions(IS_GAEN_PROJECT=1)
else()
  set(PROJECT_ASSETS_LIB
    "${CMAKE_PROJECT_NAME}_assets"
    )
  set(PROJECT_COOKERS_LIB
    "${CMAKE_PROJECT_NAME}_cookers"
    )
endif()

add_subdirectory(external)
add_subdirectory(src)

#--## Set gaen as startup project
#--#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
#--#
#--#set(publish_dir "${CMAKE_BINARY_DIR}/publish")
#--#add_custom_target(
#--#  PUBLISH
#--#  COMMAND ${CMAKE_COMMAND} -E remove_directory ${publish_dir}
#--#  COMMAND ${CMAKE_COMMAND} -E make_directory ${publish_dir}
#--#  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${publish_dir}
#--#  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gac> ${publish_dir}
#--#  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets/cooked_win ${publish_dir}/assets
#--#  DEPENDS COOK_ASSETS ${CMAKE_PROJECT_NAME}
#--#  )
