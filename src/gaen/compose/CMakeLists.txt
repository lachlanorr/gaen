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

# Run during configure so we have system_api_meta.cpp existing for .sln creation
execute_process(
  COMMAND C:/python310/python ${python_dir}/codegen_api.py "${CMAKE_BINARY_DIR}"
  )

set(gaen_compose_SOURCES
  CodegenCpp.cpp
  CodegenCpp.h
  codegen_utils.cpp
  codegen_utils.h
  comp_mem.cpp
  comp_mem.h
  comp_string.cpp
  comp_string.h
  compiler.cpp
  compiler.h
  compiler_structs.h
  compose.l
  compose.y
  compose_parser.c
  compose_parser.h
  compose_scanner.c
  compose_scanner.h
  utils.cpp
  utils.h
  "${CMAKE_CURRENT_BINARY_DIR}/system_api_meta.cpp"
  )

add_custom_target(
  CODEGEN_API ALL
  C:/python310/python ${python_dir}/codegen_api.py "${CMAKE_BINARY_DIR}"
  COMMENT "Generating system_api_meta.cpp"
  )

set_target_properties(CODEGEN_API PROPERTIES
  FOLDER CMakePredefinedTargets
  )

source_group("" FILES ${gaen_compose_SOURCES})

add_library(gaen_compose STATIC
  ${gaen_compose_SOURCES}
  )

set_target_properties(gaen_compose PROPERTIES
  FOLDER lib
  )

target_link_libraries(gaen_compose PUBLIC
  gaen_math
  gaen_hashes
  )
