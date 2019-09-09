
# zlib
MESSAGE ("**************** zlib external submodule ****************")
SET (SKIP_INSTALL_ALL ON)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/zlib)
SET_TARGET_PROPERTIES (
  zlibstatic
  zlib
  example
  minigzip
  PROPERTIES FOLDER external/zlib
  )
SET (ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zlib" "${CMAKE_CURRENT_BINARY_DIR}/zlib" CACHE INTERNAL "")
SET (ZLIB_LIBRARY
  optimized "${CMAKE_CURRENT_BINARY_DIR}/zlib/Release/zlibstatic.lib"
  debug "${CMAKE_CURRENT_BINARY_DIR}/zlib/Debug/zlibstaticd.lib"
  CACHE INTERNAL ""
  )

# brotli
MESSAGE ("**************** brotli external submodule ****************")
SET (BROTLI_DISABLE_TESTS ON)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/brotli)
SET_TARGET_PROPERTIES (
  brotli
  brotlicommon
  brotlicommon-static
  brotlidec
  brotlidec-static
  brotlienc
  brotlienc-static
  PROPERTIES FOLDER external/brotli
  )
SET (BROTLI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/brotli/c/include" CACHE INTERNAL "")
SET (BROTLI_LIBRARY
  debug "${CMAKE_CURRENT_BINARY_DIR}/brotli/Debug/brotlicommon-static.lib"
  debug "${CMAKE_CURRENT_BINARY_DIR}/brotli/Debug/brotlidec-static.lib"
  debug "${CMAKE_CURRENT_BINARY_DIR}/brotli/Debug/brotlienc-static.lib"
  optimized "${CMAKE_CURRENT_BINARY_DIR}/brotli/Release/brotlicommon-static.lib"
  optimized "${CMAKE_CURRENT_BINARY_DIR}/brotli/Release/brotlidec-static.lib"
  optimized "${CMAKE_CURRENT_BINARY_DIR}/brotli/Release/brotlienc-static.lib"
  CACHE INTERNAL ""
  )

# libpng
MESSAGE ("**************** libpng external submodule ****************")
SET (PNG_BUILD_ZLIB ON CACHE BOOL "Custom zlib Location, else find_package is used" FORCE)
SET (PNG_SHARED OFF CACHE BOOL "Build shared lib" FORCE)
SET (PNG_TESTS OFF CACHE BOOL "Build libpng tests" FORCE)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/libpng)
SET_TARGET_PROPERTIES (
  png_static
  genfiles
  genprebuilt
  gensym
  genvers
  symbol-check
  PROPERTIES FOLDER external/libpng
  )
ADD_DEPENDENCIES(png_static
  zlibstatic
  )
SET (PNG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libpng" "${CMAKE_CURRENT_BINARY_DIR}/libpng" CACHE INTERNAL "")
SET (PNG_PNG_INCLUDE_DIR "${PNG_INCLUDE_DIR}" CACHE INTERNAL "") # freetype wants this var name
SET (PNG_LIBRARY
  optimized "${CMAKE_CURRENT_BINARY_DIR}/libpng/Release/libpng16_static.lib"
  debug "${CMAKE_CURRENT_BINARY_DIR}/libpng/Debug/libpng16_staticd.lib"
  CACHE INTERNAL ""
  )

#libtiff
MESSAGE ("**************** libtiff external submodule ****************")
SET (BUILD_PORT OFF CACHE BOOL "build port" FORCE)
SET (BUILD_TOOLS OFF CACHE BOOL "build tools" FORCE)
SET (BUILD_TEST OFF CACHE BOOL "build test" FORCE)
SET (BUILD_CONTRIB OFF CACHE BOOL "build contrib" FORCE)
SET (BUILD_BUILD OFF CACHE BOOL "build build" FORCE)
SET (BUILD_MAN OFF CACHE BOOL "bauild man" FORCE)
SET (BUILD_HTML OFF CACHE BOOL "build html" FORCE)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/libtiff)
SET_TARGET_PROPERTIES (tiff PROPERTIES FOLDER external/libtiff)
SET_TARGET_PROPERTIES (tiffxx PROPERTIES FOLDER external/libtiff)
ADD_DEPENDENCIES(tiff
  zlibstatic
  )
SET (TIFF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libtiff/libtiff" "${CMAKE_CURRENT_BINARY_DIR}/libtiff/libtiff" CACHE INTERNAL "")

# freetype2
MESSAGE ("**************** freetype2 external submodule ****************")
SET (BUILD_FRAMEWORK OFF)
SET (BUILD_SHARED_LIBS OFF)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/freetype2)
TARGET_INCLUDE_DIRECTORIES (freetype PRIVATE ${BROTLI_INCLUDE_DIR})
SET_TARGET_PROPERTIES (freetype PROPERTIES FOLDER external/freetype)
ADD_DEPENDENCIES(freetype
  png_static
  zlibstatic
  brotlienc-static
  brotlidec-static
  brotlicommon-static
  )
SET (FREETYPE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/freetype2/include" "${CMAKE_CURRENT_BINARY_DIR}/freetype2/include" CACHE INTERNAL "")
SET (FREETYPE_LIBRARY
  debug "${CMAKE_CURRENT_BINARY_DIR}/freetype2/Debug/freetyped.lib"
  debug "${CMAKE_CURRENT_BINARY_DIR}/freetype2/Release/freetype.lib"
  CACHE INTERNAL ""
  )

# assimp
MESSAGE ("**************** assimp external submodule ****************")
SET (BUILD_SHARED_LIBS OFF CACHE BOOL "Build package with shared libraries." FORCE)
SET (BUILD_FRAMEWORK OFF CACHE BOOL "Build package as Mac OS X Framework bundle." FORCE)
SET (ASSIMP_BUILD_ZLIB OFF CACHE BOOL "Build your own zlib" FORCE)
SET (ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "If the supplementary tools for Assimp are built in addition to the library." FORCE)
SET (ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suite for Assimp is built in addition to the library." FORCE)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/assimp)
SET_TARGET_PROPERTIES (
  assimp
  IrrXML
  uninstall
  UpdateAssimpLibsDebugSymbolsAndDLLs
  PROPERTIES FOLDER external/assimp
  )
SET (ASSIMP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assimp/include" "${CMAKE_CURRENT_BINARY_DIR}/assimp/include" CACHE INTERNAL "")


# bullet3
MESSAGE ("**************** bullet3 external submodule ****************")
SET (BUILD_CPU_DEMOS OFF CACHE BOOL "Build original Bullet CPU examples" FORCE)
SET (USE_GLUT OFF CACHE BOOL "Use Glut" FORCE)
SET (BUILD_PYBULLET OFF CACHE BOOL "Set when you want to build pybullet (Python bindings for Bullet)")
SET (BUILD_ENET OFF CACHE BOOL "Set when you want to build apps with enet UDP networking support" FORCE)
SET (BUILD_CLSOCKET OFF CACHE BOOL "Set when you want to build apps with enet TCP networking support" FORCE)
SET (BUILD_BULLET2_DEMOS OFF CACHE BOOL "Set when you want to build the Bullet 2 demos" FORCE)
SET (BUILD_EXTRAS OFF CACHE BOOL "Set when you want to build the extras" FORCE)
SET (BUILD_UNIT_TESTS OFF CACHE BOOL "Build Unit Tests"	FORCE)
SET (INSTALL_LIBS OFF CACHE BOOL "Set when you want to install libraries" FORCE)
SET (INSTALL_CMAKE_FILES OFF CACHE BOOL "Install generated CMake files" FORCE)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/bullet3)
SET_TARGET_PROPERTIES (
  Bullet2FileLoader
  Bullet3Collision
  Bullet3Common
  Bullet3Dynamics
  Bullet3Geometry
  Bullet3OpenCL_clew
  BulletCollision
  BulletDynamics
  BulletInverseDynamics
  BulletSoftBody
  LinearMath
  PROPERTIES FOLDER external/bullet
  )
SET (BULLET_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src" CACHE INTERNAL "")

# glfw
MESSAGE ("**************** glfw external submodule ****************")
SET (BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
SET (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs" FORCE)
SET (GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs" FORCE)
SET (GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation" FORCE)
SET (GLFW_INSTALL OFF CACHE BOOL "Generate installation target" FORCE)
SET (USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "Use MSVC runtime library DLL" FORCE)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/glfw)
SET_TARGET_PROPERTIES (
  glfw
  PROPERTIES FOLDER external/glfw
  )
SET (GLFW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glfw/include" "${CMAKE_CURRENT_BINARY_DIR}/glfw/src" CACHE INTERNAL "")
