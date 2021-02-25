#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2020 Lachlan Orr
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

IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/messages/messages.cmake")
  INCLUDE ("${CMAKE_CURRENT_BINARY_DIR}/messages/messages.cmake")
ENDIF ()

SET (engine_SOURCES
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
  field_types.yaml
  messages_def.yaml
  ${engine_messages_SOURCES}
  )

MSVC_PRECOMPILED_HEADER (engine engine_SOURCES)
IDE_SOURCE_PROPERTIES( "/" "${engine_SOURCES}")
IDE_SOURCE_PROPERTIES( "/messages" "${engine_messages_SOURCES}")

SET (IMGUI_SRC "${ext_dir}/imgui")
SET (imgui_SOURCES
  ${IMGUI_SRC}/imconfig.h
  ${IMGUI_SRC}/imgui.cpp
  ${IMGUI_SRC}/imgui.h
  ${IMGUI_SRC}/imgui_demo.cpp
  ${IMGUI_SRC}/imgui_draw.cpp
  ${IMGUI_SRC}/imgui_internal.h
  ${IMGUI_SRC}/imgui_tables.cpp
  ${IMGUI_SRC}/imgui_widgets.cpp
  ${IMGUI_SRC}/imstb_rectpack.h
  ${IMGUI_SRC}/imstb_textedit.h
  ${IMGUI_SRC}/imstb_truetype.h
  ${IMGUI_SRC}/backends/imgui_impl_glfw.cpp
  ${IMGUI_SRC}/backends/imgui_impl_glfw.h
  ${IMGUI_SRC}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_SRC}/backends/imgui_impl_opengl3.h
  )
IDE_SOURCE_PROPERTIES( "/imgui" "${imgui_SOURCES}")

ADD_LIBRARY (engine
  ${engine_SOURCES}
  ${imgui_SOURCES}
  )

TARGET_INCLUDE_DIRECTORIES (engine PRIVATE
  ${BULLET_INCLUDE}
  ${GLFW_INCLUDE}
  ${IMGUI_SRC}
)

# Run each time we build.
ADD_CUSTOM_TARGET (
  CODEGEN_MESSAGES ALL
  python ${python_dir}/codegen_messages.py "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Generating message classes"
  )

ADD_DEPENDENCIES(engine
  hashes
  CODEGEN_MESSAGES
  )
