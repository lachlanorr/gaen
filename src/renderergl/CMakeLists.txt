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

SET (shaders_dir ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
INCLUDE (${shaders_dir}/codegen.cmake)

SET (renderergl_SOURCES
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
  ${shaders_codegen_SOURCES}
  )

IDE_SOURCE_PROPERTIES( "/" "${renderergl_SOURCES}")

SET (glad_SOURCES
  ${ext_dir}/glad/src/glad.c
  ${ext_dir}/glad/include/glad/glad.h
  ${ext_dir}/glad/include/KHR/khrplatform.h
  )
IDE_SOURCE_PROPERTIES( "/glad" "${glad_SOURCES}")

SET (NANOVG_SRC "${ext_dir}/nanovg/src")

SET (nanovg_SOURCES
  ${NANOVG_SRC}/fontstash.h
  ${NANOVG_SRC}/nanovg.c
  ${NANOVG_SRC}/nanovg_gl.h
  ${NANOVG_SRC}/nanovg_gl_utils.h
  ${NANOVG_SRC}/nanovg.h
  ${NANOVG_SRC}/stb_image.h
  ${NANOVG_SRC}/stb_truetype.h
  )
IDE_SOURCE_PROPERTIES( "/nanovg" "${nanovg_SOURCES}")

ADD_LIBRARY (renderergl
  ${renderergl_SOURCES}
  ${glad_SOURCES}
  ${nanovg_SOURCES}
  )

SET (IMGUI_SRC "${ext_dir}/imgui")
TARGET_INCLUDE_DIRECTORIES (renderergl PRIVATE
  ${GLFW_INCLUDE}
  ${NANOVG_SRC}
  ${IMGUI_SRC}
)

ADD_DEPENDENCIES(renderergl
  hashes
  CODEGEN_MESSAGES
  )
