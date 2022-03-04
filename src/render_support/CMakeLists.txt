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

set(render_support_SOURCES
  collision.h
  ImageBuffer.cpp
  ImageBuffer.h
  Material.cpp
  Material.h
  Model.cpp
  Model.h
  ModelMgr.cpp
  ModelMgr.h
  ModelPhysics.cpp
  ModelPhysics.h
  physics.cpp
  physics.h
  RaycastCamera.cpp
  RaycastCamera.h
  renderer_api.h
  render_objects.cpp
  render_objects.h
  shapes.cpp
  shapes.h
  Sprite.cpp
  Sprite.h
  SpriteMgr.cpp
  SpriteMgr.h
  SpritePhysics.cpp
  SpritePhysics.h
  system_api.cpp
  system_api.h
  voxel.cpp
  voxel.h
  voxel27.cpp
  voxel27.h
  voxel_proto.cpp
  voxel_proto.h
  )

#msvc_precompiled_header(render_support render_support_SOURCES)
source_group("" FILES ${render_support_SOURCES})

add_library(render_support STATIC
  ${render_support_SOURCES}
  )

target_link_libraries(render_support PUBLIC
  core
  BulletCollision
  BulletDynamics
  LinearMath
  glfw
  )

add_dependencies(render_support
  hashes
  CODEGEN_MESSAGES
  )
