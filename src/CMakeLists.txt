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

# Standard include paths
set(GAEN_SRC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})
set(GAEN_BIN_INCLUDE ${CMAKE_CURRENT_BINARY_DIR})

# Packaging
set(packaged_dir ${CMAKE_BINARY_DIR}/packaged)

file(MAKE_DIRECTORY ${packaged_dir})

macro(package target)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${target}> ${packaged_dir}/
    #COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${target}> ${packaged_dir}/
    )
endmacro()

find_package(Python COMPONENTS Interpreter)

add_subdirectory(gaen)
