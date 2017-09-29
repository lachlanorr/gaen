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

# Run during configure so we have system_api_meta.cpp existing for .sln creation
EXECUTE_PROCESS (
  COMMAND python ${python_dir}/codegen_api.py "${CMAKE_CURRENT_BINARY_DIR}/system_api_meta.cpp"
  )

SET (compose_SOURCES
  codegen_cpp.cpp
  codegen_cpp.h
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

ADD_CUSTOM_TARGET(
  CODEGEN_API ALL
  python ${python_dir}/codegen_api.py "${CMAKE_CURRENT_BINARY_DIR}/system_api_meta.cpp"
  COMMENT "Generating system_api_meta.cpp"
  )

IDE_SOURCE_PROPERTIES( "/" "${compose_SOURCES}")

ADD_LIBRARY (compose
  ${compose_SOURCES}
  )

ADD_DEPENDENCIES(compose
  nana
  CODEGEN_API
  )
