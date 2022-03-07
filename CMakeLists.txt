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

cmake_minimum_required(VERSION 3.22.0)

project(gaen)
include(compile_defs.cmake)

option(gaen_BUILD_TESTS "Build tests" ON)

set(python_dir ${CMAKE_CURRENT_SOURCE_DIR}/python)

add_subdirectory(external)
add_subdirectory(src)

# Set gaen as startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

set(publish_dir "${CMAKE_BINARY_DIR}/publish")
add_custom_target(
  PUBLISH
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gac> ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets/cooked_win ${publish_dir}/assets
  DEPENDS COOK_ASSETS ${CMAKE_PROJECT_NAME}
  )

set_target_properties(PUBLISH PROPERTIES
  FOLDER CMakePredefinedTargets
  )
