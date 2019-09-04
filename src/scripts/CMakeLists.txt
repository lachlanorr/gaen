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

IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/codegen.cmake")
  INCLUDE ("${CMAKE_CURRENT_BINARY_DIR}/codegen.cmake")
ENDIF ()

SET (scripts_SOURCES
  ${scripts_codegen_SOURCES}
  )

ADD_LIBRARY (scripts
  DUMMY.cpp
  ${scripts_SOURCES}
  )

IDE_SOURCE_PROPERTIES( "/" "DUMMY.cpp" )

ADD_CUSTOM_TARGET (
  CODEGEN_SCRIPTS ALL
  python ${python_dir}/codegen.py "${CMAKE_CURRENT_BINARY_DIR}"
  # re-run update_hashes.py since generated .cpp files will likely
  # have new hashes to consider.
  COMMAND python ${python_dir}/update_hashes.py "${CMAKE_BINARY_DIR}/src/hashes"
  DEPENDS cmpc
  COMMENT "Generating scripts"
  )

ADD_DEPENDENCIES(scripts
  hashes
  math
  engine
  CODEGEN_SCRIPTS
  )
