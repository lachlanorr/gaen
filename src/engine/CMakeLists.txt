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

include("${CMAKE_CURRENT_BINARY_DIR}/messages/messages.cmake" OPTIONAL)

set(engine_SOURCES
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
  Editor.cpp
  Editor.h
  Entity.cpp
  Entity.h
  EntityInit.h
  FrameTime.h
  glm_ext.h
  Handle.cpp
  Handle.h
  input.cpp
  input.h
  InputMgr.cpp
  InputMgr.h
  Message.h
  MessageAccessor.h
  MessageQueue.h
  MessageWriter.cpp
  MessageWriter.h
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
  ${engine_messages_SOURCES}
  )

#msvc_precompiled_header(engine engine_SOURCES)

source_group("" FILES ${engine_SOURCES})
source_group("messages" FILES ${engine_messages_SOURCES})

add_library(engine STATIC
  ${engine_SOURCES}
  )

target_link_libraries(engine PUBLIC
  core
  BulletCollision
  BulletDynamics
  LinearMath
  imgui
  glfw
  glad
  )

# Run each time we build.
add_custom_target(
  CODEGEN_MESSAGES ALL
  python ${python_dir}/codegen_messages.py "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Generating message classes"
  )

add_dependencies(engine
  hashes
  CODEGEN_MESSAGES
  )
