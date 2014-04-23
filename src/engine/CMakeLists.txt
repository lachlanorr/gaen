#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014 Lachlan Orr
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
INCLUDE (components/components.cmake)

SET (engine_SOURCES
  Color.h
  Component.h
  Entity.cpp
  Entity.h
  FNV.h
  FNV.cpp
  Material.cpp
  Material.h
  math.cpp
  math.h
  Mesh.cpp
  Mesh.h
  Message.h
  MessageQueue.h
  MessageWriter.h
  Model.cpp
  Model.h
  ModelMgr.cpp
  ModelMgr.h
  renderer_type.h
  shapes.cpp
  shapes.h
  Task.cpp
  Task.h
  TaskMaster.cpp
  TaskMaster.h
  ${engine_messages_SOURCES}
#  ${engine_components_SOURCES}
  )

MSVC_PRECOMPILED_HEADER (engine engine_SOURCES)
IDE_SOURCE_PROPERTIES( "/" "${engine_SOURCES}")
IDE_SOURCE_PROPERTIES( "/messages" "${engine_messages_SOURCES}")
IDE_SOURCE_PROPERTIES( "/components" "${engine_components_SOURCES}")

ADD_LIBRARY (engine
  ${engine_SOURCES}
  )



# Run during configure.
EXECUTE_PROCESS (
  COMMAND python ${scripts_dir}/codegen_messages.py
  )

# Run each time we build.
ADD_CUSTOM_TARGET (
  messages_gen
  COMMAND python ${scripts_dir}/codegen_messages.py
  )

  

# Run during configure.
EXECUTE_PROCESS (
  COMMAND python ${scripts_dir}/codegen_components.py
  )

# Run each time we build.
ADD_CUSTOM_TARGET (
  components_gen
  COMMAND python ${scripts_dir}/codegen_components.py
  )

  

# Run during configure so we have FNV.h/cpp existing for .sln creation
EXECUTE_PROCESS (
  COMMAND python ${scripts_dir}/update_fnv_hashes.py
  )

# Run each time we build. FNV.h/cpp will only get replaced if they change.
ADD_CUSTOM_TARGET (
  fnv_hashes_precalc
  COMMAND python ${scripts_dir}/update_fnv_hashes.py
  )



ADD_DEPENDENCIES(engine
  core
  messages_gen
  #components_gen
  fnv_hashes_precalc
  )

