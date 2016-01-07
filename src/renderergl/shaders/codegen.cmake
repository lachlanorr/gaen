#-------------------------------------------------------------------------------
# codegen.cmake - Autogenerated cmake containing generated classes
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2015 Lachlan Orr
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

SET (shaders_codegen_SOURCES
  ${shaders_dir}/compute_present.cpp
  ${shaders_dir}/compute_present.h
  ${shaders_dir}/compute_present.shd
  ${shaders_dir}/compute_present.shf
  ${shaders_dir}/compute_present.shv
  ${shaders_dir}/compute_test.cpp
  ${shaders_dir}/compute_test.h
  ${shaders_dir}/compute_test.shc
  ${shaders_dir}/compute_test.shd
  ${shaders_dir}/faceted.cpp
  ${shaders_dir}/faceted.h
  ${shaders_dir}/faceted.shd
  ${shaders_dir}/faceted.shf
  ${shaders_dir}/faceted.shv
  ${shaders_dir}/Shader.cpp
  ${shaders_dir}/Shader.h
  ${shaders_dir}/ShaderRegistry_codegen.cpp
  ${shaders_dir}/voxel27.cpp
  ${shaders_dir}/voxel27.h
  ${shaders_dir}/voxel27.shd
  ${shaders_dir}/voxel27.shf
  ${shaders_dir}/voxel27.shv
  ${shaders_dir}/voxel_cast.cpp
  ${shaders_dir}/voxel_cast.h
  ${shaders_dir}/voxel_cast.shc
  ${shaders_dir}/voxel_cast.shd
  ${shaders_dir}/voxel_cast_frag.cpp
  ${shaders_dir}/voxel_cast_frag.h
  ${shaders_dir}/voxel_cast_frag.shd
  ${shaders_dir}/voxel_cast_frag.shf
  ${shaders_dir}/voxel_cast_frag.shv
)

IDE_SOURCE_PROPERTIES( "/shaders" "${shaders_codegen_SOURCES}")
