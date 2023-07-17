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

include("${CMAKE_CURRENT_BINARY_DIR}/messages/messages.cmake" OPTIONAL)

set(gaen_engine_SOURCES
  Asset.cpp
  Asset.h
  AssetLoader.cpp
  AssetLoader.h
  AssetMgr.cpp
  AssetMgr.h
  AssetType.h
  AssetTypes.cpp
  AssetTypes.h
  AssetWithDep.h
  Block.h
  BlockData.h
  BlockMemory.cpp
  BlockMemory.h
  CmpString.cpp
  CmpString.h
  Component.h
  compose_funcs.h
  Entity.cpp
  Entity.h
  EntityInit.h
  FrameTime.h
  glm_ext.h
  Handle.cpp
  Handle.h
  Message.h
  MessageAccessor.h
  MessageQueue.h
  MessageWriter.cpp
  MessageWriter.h
  Randomizer.cpp
  Randomizer.h
  Registry.cpp
  Registry.h
  Task.cpp
  Task.h
  TaskMaster.cpp
  TaskMaster.h
  UniqueObject.cpp
  UniqueObject.h
  field_types.yaml
  messages_def.yaml
  ${gaen_engine_messages_SOURCES}
  )

if (NOT DEFINED IS_HEADLESS)
list(APPEND gaen_engine_SOURCES
  Editor.cpp
  Editor.h
  input.cpp
  input.h
  InputMgr.cpp
  InputMgr.h
  )
endif()

source_group("" FILES ${gaen_engine_SOURCES})
source_group("messages" FILES ${gaen_engine_messages_SOURCES})

add_library(gaen_engine STATIC
  ${gaen_engine_SOURCES}
  )

set_target_properties(gaen_engine PROPERTIES
  FOLDER lib
  )

set(ENGINE_LINK_LIBS
  gaen_math
  gaen_hashes
  BulletCollision
  BulletDynamics
  LinearMath
  )

if (NOT DEFINED IS_HEADLESS)
  list(APPEND ENGINE_LINK_LIBS
	imgui
	glfw
	glad
	)
endif()

target_link_libraries(gaen_engine PUBLIC
  ${ENGINE_LINK_LIBS}
  )

# Run each time we build.
add_custom_target(
  CODEGEN_MESSAGES ALL
  C:/python310/python ${python_dir}/codegen_messages.py "${CMAKE_BINARY_DIR}"
  COMMENT "Generating message classes"
  )

set_target_properties(CODEGEN_MESSAGES PROPERTIES
  FOLDER CMakePredefinedTargets
  )

add_dependencies(gaen_engine
  CODEGEN_MESSAGES
  )
