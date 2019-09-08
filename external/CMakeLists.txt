

# zlib
SET (SKIP_INSTALL_ALL ON)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/zlib)
SET_TARGET_PROPERTIES (zlib PROPERTIES FOLDER external/zlib)
SET_TARGET_PROPERTIES (zlibstatic PROPERTIES FOLDER external/zlib)
SET_TARGET_PROPERTIES (minigzip PROPERTIES FOLDER external/zlib)
SET_TARGET_PROPERTIES (example PROPERTIES FOLDER external/zlib)
SET (ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zlib" "${CMAKE_CURRENT_BINARY_DIR}/zlib" CACHE INTERNAL "")
SET (ZLIB_LIBRARY
  optimized "${CMAKE_CURRENT_BINARY_DIR}/zlib/Release/zlibstatic.lib"
  debug "${CMAKE_CURRENT_BINARY_DIR}/zlib/Debug/zlibstaticd.lib"
  CACHE INTERNAL ""
  )


# libpng
SET (PNG_BUILD_ZLIB ON CACHE BOOL "Custom zlib Location, else find_package is used" FORCE)
SET (PNG_SHARED OFF CACHE BOOL "Build shared lib" FORCE)
SET (PNG_TESTS OFF CACHE BOOL "Build libpng tests" FORCE)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/libpng)
SET_TARGET_PROPERTIES (png_static PROPERTIES FOLDER external/libpng)
SET_TARGET_PROPERTIES (genfiles PROPERTIES FOLDER external/libpng)
SET_TARGET_PROPERTIES (genprebuilt PROPERTIES FOLDER external/libpng)
SET_TARGET_PROPERTIES (gensym PROPERTIES FOLDER external/libpng)
SET_TARGET_PROPERTIES (genvers PROPERTIES FOLDER external/libpng)
SET_TARGET_PROPERTIES (symbol-check PROPERTIES FOLDER external/libpng)
ADD_DEPENDENCIES(png_static
  zlib
  )
SET (PNG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libpng" "${CMAKE_CURRENT_BINARY_DIR}/libpng" CACHE INTERNAL "")
SET (PNG_PNG_INCLUDE_DIR "${PNG_INCLUDE_DIR}" CACHE INTERNAL "") # freetype wants this var name
SET (PNG_LIBRARY
  optimized "${CMAKE_CURRENT_BINARY_DIR}/libpng/Release/libpng16_static.lib"
  debug "${CMAKE_CURRENT_BINARY_DIR}/libpng/Debug/libpng16_staticd.lib"
  CACHE INTERNAL ""
  )


# brotli
SET (BROTLI_DISABLE_TESTS ON)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/brotli)
SET_TARGET_PROPERTIES (brotli PROPERTIES FOLDER external/brotli)
SET_TARGET_PROPERTIES (brotlicommon PROPERTIES FOLDER external/brotli)
SET_TARGET_PROPERTIES (brotlicommon-static PROPERTIES FOLDER external/brotli)
SET_TARGET_PROPERTIES (brotlidec PROPERTIES FOLDER external/brotli)
SET_TARGET_PROPERTIES (brotlidec-static PROPERTIES FOLDER external/brotli)
SET_TARGET_PROPERTIES (brotlienc PROPERTIES FOLDER external/brotli)
SET_TARGET_PROPERTIES (brotlienc-static PROPERTIES FOLDER external/brotli)
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

# freetype2
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/freetype2)
TARGET_INCLUDE_DIRECTORIES (freetype PRIVATE ${BROTLI_INCLUDE_DIR})
SET_TARGET_PROPERTIES (freetype PROPERTIES FOLDER external/freetype)
ADD_DEPENDENCIES(freetype
  png_static
  zlib
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
SET (BUILD_SHARED_LIBS OFF CACHE BOOL "Build package with shared libraries." FORCE)
SET (ASSIMP_BUILD_ZLIB OFF CACHE BOOL "Build your own zlib" FORCE)
SET (ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "If the supplementary tools for Assimp are built in addition to the library." FORCE)
SET (ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suite for Assimp is built in addition to the library." FORCE)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/assimp)
SET_TARGET_PROPERTIES (assimp PROPERTIES FOLDER external/assimp)
SET_TARGET_PROPERTIES (IrrXML PROPERTIES FOLDER external/assimp)
SET_TARGET_PROPERTIES (uninstall PROPERTIES FOLDER external/assimp)
SET_TARGET_PROPERTIES (UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER external/assimp)
SET (ASSIMP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assimp/include" "${CMAKE_CURRENT_BINARY_DIR}/assimp/include" CACHE INTERNAL "")
