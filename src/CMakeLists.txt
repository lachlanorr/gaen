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
    )
endmacro()

#libs
add_subdirectory(core)
add_subdirectory(hashes)
add_subdirectory(assets)
add_subdirectory(cara)
add_subdirectory(math)
add_subdirectory(engine)
add_subdirectory(render_support)
add_subdirectory(audio)
add_subdirectory(shader_gen)
add_subdirectory(renderergl)
add_subdirectory(compose)
add_subdirectory(scripts)
add_subdirectory(gaen)

#exes
if(NOT IOS)
  add_subdirectory(chef)
  add_subdirectory(cmpc)
  add_subdirectory(gac)
  add_subdirectory(unit_tests)
endif()
