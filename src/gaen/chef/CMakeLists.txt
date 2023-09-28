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

set(chef_SOURCES
  main_chef.cpp
  )

if(WIN32)
  set(VER_PRODUCTNAME "chef")
  set(VER_FILEDESCRIPTION "Chef Asset Cooker")
  set(VER_ORIGINALFILENAME "chef.exe")
  configure_file(
    ${CMAKE_SOURCE_DIR}/src/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
    )
  set(chef_SOURCES
    ${chef_SOURCES}
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    )
endif()

source_group("" FILES ${chef_SOURCES})

add_compile_definitions(IS_BUILD_Tool=1)

add_executable(chef
  ${chef_SOURCES}
  )

target_link_libraries(chef
  gaen_cheflib
  )

package (chef)

add_custom_target(
  COOK_ASSETS
  COMMAND ${packaged_dir}/chef
  DEPENDS chef
  )

set_target_properties(COOK_ASSETS PROPERTIES
  FOLDER CMakePredefinedTargets
  )
