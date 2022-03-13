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

include("${CMAKE_CURRENT_BINARY_DIR}/codegen.cmake" OPTIONAL)

set(gaen_scripts_SOURCES
  ${gaen_scripts_codegen_SOURCES}
  )

add_library(gaen_scripts STATIC
  DUMMY.cpp
  ${gaen_scripts_SOURCES}
  )

set_target_properties(gaen_scripts PROPERTIES
  FOLDER lib
  )

target_link_libraries(gaen_scripts PUBLIC
  ${PROJECT_LINK_LIBS}
  gaen_math
  gaen_hashes
  LinearMath
  )

source_group("" FILES "DUMMY.cpp")

add_custom_target(
  CODEGEN_SCRIPTS ALL
  C:/python310/python ${python_dir}/codegen_scripts.py "${CMAKE_BINARY_DIR}"
  # re-run codegen_hashes.py since generated .cpp files will likely
  # have new hashes to consider.
  COMMAND C:/python310/python ${python_dir}/codegen_hashes.py "${CMAKE_BINARY_DIR}"
  DEPENDS cmpc
  COMMENT "Generating scripts"
  )

set_target_properties(CODEGEN_SCRIPTS PROPERTIES
  FOLDER CMakePredefinedTargets
  )

add_dependencies(gaen_scripts
  CODEGEN_MESSAGES
  CODEGEN_SCRIPTS
  )
