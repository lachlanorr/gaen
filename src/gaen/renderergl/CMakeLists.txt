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

set(shaders_dir ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
include(${shaders_dir}/codegen.cmake)

set(gaen_renderergl_SOURCES
  FrameGL.h
  gaen_opengl.h
  ModelGL.cpp
  ModelGL.h
  RenderCollection.h
  Renderer.h
  RendererMesh.h
  RendererMesh.cpp
  RendererProto.h
  RendererProto.cpp
  Renderer_${platform}.${platform_ext}
  renderer_api.cpp
  ShaderRegistry.cpp
  ShaderRegistry.h
  SpriteGL.cpp
  SpriteGL.h
  Stage.h
  StageMgr.h
  )

source_group("" FILES ${gaen_renderergl_SOURCES})
source_group("shaders" FILES ${shaders_codegen_SOURCES})

list(APPEND gaen_renderergl_SOURCES ${shaders_codegen_SOURCES})

add_library(gaen_renderergl STATIC
  ${gaen_renderergl_SOURCES}
  )

set_target_properties(gaen_renderergl PROPERTIES
  FOLDER lib
  )

target_link_libraries(gaen_renderergl PUBLIC
  gaen_math
  gaen_hashes
  nanovg
  imgui
  glfw
  glad
  )

add_dependencies(gaen_renderergl
  CODEGEN_MESSAGES
  )
