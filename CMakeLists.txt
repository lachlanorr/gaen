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

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.12)

PROJECT (gaen)

CMAKE_POLICY(SET CMP0054 NEW)
CMAKE_POLICY(SET CMP0026 OLD)

SET (cmake_dir ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
SET (ext_dir ${CMAKE_CURRENT_SOURCE_DIR}/external)
SET (python_dir ${CMAKE_CURRENT_SOURCE_DIR}/python)
SET (src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

INCLUDE (${cmake_dir}/core_defs.cmake)

# If we're compiling gaen as a project (instead of a project that uses
# gaen) set IS_GAEN_PROJECT so we can selectively include some hooks
# for the project to register asset types and cookers.
IF ("${CMAKE_PROJECT_NAME}" STREQUAL "gaen")
  ADD_DEFINITIONS("-DIS_GAEN_PROJECT=1")
ELSE ()
  SET(PROJECT_ASSETS_LIB
    "${CMAKE_PROJECT_NAME}_assets"
    )
  SET(PROJECT_COOKERS_LIB
    "${CMAKE_PROJECT_NAME}_cookers"
    )
ENDIF ()

#-------------------------------------------------------------------------------
# External dependencies
#-------------------------------------------------------------------------------
INCLUDE (ExternalProject)

# zlib
SET (ZLIB_VER "1.2.8")
SET (ZLIB_NAME_VER "zlib-${ZLIB_VER}")
SET (ZLIB_PATH "${CMAKE_CURRENT_BINARY_DIR}/zlib")
ExternalProject_Add(zlib
  URL "${ext_dir}/${ZLIB_NAME_VER}.tar.gz"
  URL_MD5 44d667c142d7cda120332623eab69f40
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/zlib
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${ZLIB_PATH}"
             "-DCMAKE_CXX_FLAGS_DEBUG:STRING=/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1"
             "-DCMAKE_CXX_FLAGS_MINSIZEREL:STRING=/MT /O1 /Ob1 /D NDEBUG"
             "-DCMAKE_CXX_FLAGS_RELEASE:STRING=/MT /O2 /Ob2 /D NDEBUG"
             "-DCMAKE_CXX_FLAGS_RELWITHDEBINFO:STRING=/MT /Zi /O2 /Ob1 /D NDEBUG"
             "-DCMAKE_C_FLAGS_DEBUG:STRING=/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1"
             "-DCMAKE_C_FLAGS_MINSIZEREL:STRING=/MT /O1 /Ob1 /D NDEBUG"
             "-DCMAKE_C_FLAGS_RELEASE:STRING=/MT /O2 /Ob2 /D NDEBUG"
             "-DCMAKE_C_FLAGS_RELWITHDEBINFO:STRING=/MT /Zi /O2 /Ob1 /D NDEBUG"
  )
SET (ZLIB_INCLUDE "${ZLIB_PATH}/include")
INCLUDE_DIRECTORIES (ZLIB_INCLUDE)
SET (ZLIB_LIB "${ZLIB_PATH}/lib/zlibstatic.lib")
SET (ZLIB_LIBD "${ZLIB_PATH}/lib/zlibstaticd.lib")

# libpng
SET (LIBPNG_VER "1.6.25")
SET (LIBPNG_NAME_VER "libpng-${LIBPNG_VER}")
SET (LIBPNG_PATH "${CMAKE_CURRENT_BINARY_DIR}/libpng")
ExternalProject_Add(libpng
  URL "${ext_dir}/${LIBPNG_NAME_VER}.tar.gz"
  URL_MD5 87cbd0f69e4bff0215de3994a149df77
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libpng
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${LIBPNG_PATH}"
             "-DZLIB_INCLUDE_DIR:PATH=${ZLIB_INCLUDE}"
             "-DZLIB_LIBRARY_RELEASE:PATH=${ZLIB_LIB}"
             "-DZLIB_LIBRARY_DEBUG:PATH=${ZLIB_LIBD}"
             "-DCMAKE_CXX_FLAGS_DEBUG:STRING=/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1"
             "-DCMAKE_CXX_FLAGS_MINSIZEREL:STRING=/MT /O1 /Ob1 /D NDEBUG"
             "-DCMAKE_CXX_FLAGS_RELEASE:STRING=/MT /O2 /Ob2 /D NDEBUG"
             "-DCMAKE_CXX_FLAGS_RELWITHDEBINFO:STRING=/MT /Zi /O2 /Ob1 /D NDEBUG"
             "-DCMAKE_C_FLAGS_DEBUG:STRING=/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1"
             "-DCMAKE_C_FLAGS_MINSIZEREL:STRING=/MT /O1 /Ob1 /D NDEBUG"
             "-DCMAKE_C_FLAGS_RELEASE:STRING=/MT /O2 /Ob2 /D NDEBUG"
             "-DCMAKE_C_FLAGS_RELWITHDEBINFO:STRING=/MT /Zi /O2 /Ob1 /D NDEBUG"
  )
ADD_DEPENDENCIES(libpng
  zlib
  )
INCLUDE_DIRECTORIES (${LIBPNG_PATH}/include)
SET (LIBPNG_LIB "${LIBPNG_PATH}/lib/libpng16_static.lib")
SET (LIBPNG_LIBD "${LIBPNG_PATH}/lib/libpng16_staticd.lib")

# FreeType
SET (FREETYPE_VER "2.6.5")
SET (FREETYPE_NAME_VER "freetype-${FREETYPE_VER}")
SET (FREETYPE_PATH "${CMAKE_CURRENT_BINARY_DIR}/freetype")
ExternalProject_Add(freetype
  URL "${ext_dir}/${FREETYPE_NAME_VER}.tar.gz"
  URL_MD5 c255ed60339ec54d310a7bc3c9988e68
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/freetype
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${FREETYPE_PATH}"
             "-DCMAKE_CXX_FLAGS_DEBUG:STRING=/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1"
             "-DCMAKE_CXX_FLAGS_MINSIZEREL:STRING=/MT /O1 /Ob1 /D NDEBUG"
             "-DCMAKE_CXX_FLAGS_RELEASE:STRING=/MT /O2 /Ob2 /D NDEBUG"
             "-DCMAKE_CXX_FLAGS_RELWITHDEBINFO:STRING=/MT /Zi /O2 /Ob1 /D NDEBUG"
             "-DCMAKE_C_FLAGS_DEBUG:STRING=/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1"
             "-DCMAKE_C_FLAGS_MINSIZEREL:STRING=/MT /O1 /Ob1 /D NDEBUG"
             "-DCMAKE_C_FLAGS_RELEASE:STRING=/MT /O2 /Ob2 /D NDEBUG"
             "-DCMAKE_C_FLAGS_RELWITHDEBINFO:STRING=/MT /Zi /O2 /Ob1 /D NDEBUG"
  )
INCLUDE_DIRECTORIES (${FREETYPE_PATH}/include/freetype2)
SET (FREETYPE_LIB "${FREETYPE_PATH}/lib/freetype.lib")
SET (FREETYPE_LIBD "${FREETYPE_PATH}/lib/freetyped.lib")

# Bullet Physics
SET (BULLET_VER "2.83.7")
SET (BULLET_NAME_VER "bullet3-${BULLET_VER}")
SET (BULLET_PATH "${CMAKE_CURRENT_BINARY_DIR}/bullet/src")
ExternalProject_Add(bullet
  URL "${ext_dir}/${BULLET_NAME_VER}.tar.gz"
  URL_MD5 39fd0138fcb59047c12861f3b65c063e
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/bullet
  INSTALL_COMMAND ""
)
INCLUDE_DIRECTORIES (${BULLET_PATH}/bullet/src)
SET (BULLET_LIBS
     optimized ${BULLET_PATH}/bullet-build/lib/Release/Bullet3Common.lib
     optimized ${BULLET_PATH}/bullet-build/lib/Release/BulletDynamics.lib
     optimized ${BULLET_PATH}/bullet-build/lib/Release/BulletCollision.lib
     optimized ${BULLET_PATH}/bullet-build/lib/Release/LinearMath.lib

     debug ${BULLET_PATH}/bullet-build/lib/Debug/Bullet3Common_Debug.lib
     debug ${BULLET_PATH}/bullet-build/lib/Debug/BulletDynamics_Debug.lib
     debug ${BULLET_PATH}/bullet-build/lib/Debug/BulletCollision_Debug.lib
     debug ${BULLET_PATH}/bullet-build/lib/Debug/LinearMath_Debug.lib
     )

# glfw
SET (GLFW_VER "3.2.1")
SET (GLFW_NAME_VER "glfw-${GLFW_VER}")
SET (GLFW_PATH "${CMAKE_CURRENT_BINARY_DIR}/glfw/src")
ExternalProject_Add(glfw
  URL "${ext_dir}/${GLFW_NAME_VER}.zip"
  URL_MD5 824c99eea073bdd6d2fec76b538f79af
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glfw
  INSTALL_COMMAND ""
  CMAKE_ARGS "-DCMAKE_CXX_FLAGS_DEBUG:STRING=/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1"
             "-DCMAKE_CXX_FLAGS_MINSIZEREL:STRING=/MT /O1 /Ob1 /D NDEBUG"
             "-DCMAKE_CXX_FLAGS_RELEASE:STRING=/MT /O2 /Ob2 /D NDEBUG"
             "-DCMAKE_CXX_FLAGS_RELWITHDEBINFO:STRING=/MT /Zi /O2 /Ob1 /D NDEBUG"
             "-DCMAKE_C_FLAGS_DEBUG:STRING=/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1"
             "-DCMAKE_C_FLAGS_MINSIZEREL:STRING=/MT /O1 /Ob1 /D NDEBUG"
             "-DCMAKE_C_FLAGS_RELEASE:STRING=/MT /O2 /Ob2 /D NDEBUG"
             "-DCMAKE_C_FLAGS_RELWITHDEBINFO:STRING=/MT /Zi /O2 /Ob1 /D NDEBUG"
)
INCLUDE_DIRECTORIES (${GLFW_PATH}/glfw/include)
SET (GLFW_LIBS
     optimized ${GLFW_PATH}/glfw-build/src/Release/glfw3.lib
     debug ${GLFW_PATH}/glfw-build/src/Debug/glfw3.lib
     )
#-------------------------------------------------------------------------------
# External dependencies (END)
#-------------------------------------------------------------------------------

# Support for creating launch params in Visual Studio
LIST (APPEND CMAKE_MODULE_PATH "${ext_dir}/rpavlik-cmake")
INCLUDE (CreateLaunchers)

ADD_DEFINITIONS("-DNANA_ENABLE_PNG=1")
ADD_SUBDIRECTORY (${ext_dir}/nana)
INCLUDE_DIRECTORIES (${ext_dir}/nana/include)

ADD_SUBDIRECTORY(src)

# Set debugging params for ALL_BUILD
create_default_target_launcher(
    ${CMAKE_PROJECT_NAME}
    ARGS "-l 127.0.0.1"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)


SET (publish_dir "${CMAKE_BINARY_DIR}/publish")
ADD_CUSTOM_TARGET (
  PUBLISH
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gac> ${publish_dir}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets/cooked_win ${publish_dir}/assets
  DEPENDS COOK_ASSETS ${CMAKE_PROJECT_NAME}
  )
