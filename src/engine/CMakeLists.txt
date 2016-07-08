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

INCLUDE (messages/messages.cmake)

SET (engine_SOURCES
  Asset.cpp
  Asset.h
  AssetLoader.cpp
  AssetLoader.h
  AssetMgr.cpp
  AssetMgr.h
  Block.h
  BlockData.h
  BlockMemory.cpp
  BlockMemory.h
  CmpString.cpp
  CmpString.h
  collision.h
  Color.h
  Component.cpp
  Component.h
  Entity.cpp
  Entity.h
  FrameTime.h
  glm_ext.h
  Handle.cpp
  Handle.h
  ImageBuffer.cpp
  ImageBuffer.h
  input.cpp
  input_${platform}.${platform_ext}
  input.h
  InputMgr.cpp
  InputMgr.h
  Material.cpp
  Material.h
  Mesh.cpp
  Mesh.h
  Message.h
  MessageAccessor.h
  MessageQueue.h
  MessageWriter.cpp
  MessageWriter.h
  Model.cpp
  Model.h
  ModelMgr.h
  RaycastCamera.cpp
  RaycastCamera.h
  Registry.cpp
  Registry.h
  renderer_api.h
  renderer_structs.h
  shapes.cpp
  shapes.h
  system_api.cpp
  system_api.h
  Task.cpp
  Task.h
  TaskMaster.cpp
  TaskMaster.h
  voxel.cpp
  voxel.h
  voxel27.cpp
  voxel27.h
  voxel_proto.cpp
  voxel_proto.h
  ${engine_codegen_SOURCES}
  ${engine_messages_SOURCES}
  )

MSVC_PRECOMPILED_HEADER (engine engine_SOURCES)
IDE_SOURCE_PROPERTIES( "/" "${engine_SOURCES}")
IDE_SOURCE_PROPERTIES( "/messages" "${engine_messages_SOURCES}")

ADD_LIBRARY (engine
  ${engine_SOURCES}
  )

# Run each time we build.
ADD_CUSTOM_TARGET (
  CODEGEN_MESSAGES ALL
  python ${python_dir}/codegen_messages.py
  COMMENT "Generating message classes"
  )

ADD_DEPENDENCIES(engine
  core
  hashes
  assets
  CODEGEN_MESSAGES
  )
