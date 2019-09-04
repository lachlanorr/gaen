#-------------------------------------------------------------------------------
# codegen.cmake - Autogenerated cmake containing generated classes
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

SET (scripts_dir ${CMAKE_CURRENT_SOURCE_DIR})

SET (scripts_codegen_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/registration.cpp
  ${scripts_dir}/cmp/init.cmp
  ${scripts_dir}/cmp/test.cmp
  ${scripts_dir}/cmp/gaen/lights.cmp
  ${scripts_dir}/cmp/gaen/shapes.cmp
  ${scripts_dir}/cmp/gaen/utils.cmp
C:/code/gaen/build/win64/src/scripts/cpp/init.cpp
C:/code/gaen/build/win64/src/scripts/cpp/test.cpp
C:/code/gaen/build/win64/src/scripts/cpp/gaen/lights.cpp
C:/code/gaen/build/win64/src/scripts/cpp/gaen/shapes.cpp
C:/code/gaen/build/win64/src/scripts/cpp/gaen/utils.cpp
)

IDE_SOURCE_PROPERTIES( "/cmp" "${scripts_dir}/cmp/init.cmp" )
IDE_SOURCE_PROPERTIES( "/cmp" "${scripts_dir}/cmp/test.cmp" )
IDE_SOURCE_PROPERTIES( "/cmp/gaen" "${scripts_dir}/cmp/gaen/lights.cmp" )
IDE_SOURCE_PROPERTIES( "/cmp/gaen" "${scripts_dir}/cmp/gaen/shapes.cmp" )
IDE_SOURCE_PROPERTIES( "/cmp/gaen" "${scripts_dir}/cmp/gaen/utils.cmp" )
IDE_SOURCE_PROPERTIES( "C:/code/gaen/build/win64/src/scripts/cpp" "C:/code/gaen/build/win64/src/scripts/cpp/init.cpp" )
IDE_SOURCE_PROPERTIES( "C:/code/gaen/build/win64/src/scripts/cpp" "C:/code/gaen/build/win64/src/scripts/cpp/test.cpp" )
IDE_SOURCE_PROPERTIES( "C:/code/gaen/build/win64/src/scripts/cpp/gaen" "C:/code/gaen/build/win64/src/scripts/cpp/gaen/lights.cpp" )
IDE_SOURCE_PROPERTIES( "C:/code/gaen/build/win64/src/scripts/cpp/gaen" "C:/code/gaen/build/win64/src/scripts/cpp/gaen/shapes.cpp" )
IDE_SOURCE_PROPERTIES( "C:/code/gaen/build/win64/src/scripts/cpp/gaen" "C:/code/gaen/build/win64/src/scripts/cpp/gaen/utils.cpp" )
