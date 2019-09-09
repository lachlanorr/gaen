
# zlib
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
  zlib
  )
SET (PNG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libpng" "${CMAKE_CURRENT_BINARY_DIR}/libpng" CACHE INTERNAL "")
SET (PNG_PNG_INCLUDE_DIR "${PNG_INCLUDE_DIR}" CACHE INTERNAL "") # freetype wants this var name
SET (PNG_LIBRARY
  optimized "${CMAKE_CURRENT_BINARY_DIR}/libpng/Release/libpng16_static.lib"
  debug "${CMAKE_CURRENT_BINARY_DIR}/libpng/Debug/libpng16_staticd.lib"
  CACHE INTERNAL ""
  )

#libtiff
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
#SET_TARGET_PROPERTIES (addtiffo PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (ascii_tag PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (custom_dir PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (defer_strile_loading PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (defer_strile_writing PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (fax2ps PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (fax2tiff PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (htmldoc PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (iptcutil PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (long_tag PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (pal2rgb PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (port PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (ppm2tiff PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (raw2tiff PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (rewrite PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (rgb2ycbcr PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (short_tag PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (strip_rw PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (testtypes PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (thumbnail PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiff2bw PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiff2pdf PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiff2ps PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiff2rgba PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiff-bi PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiffcmp PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiffcp PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiffcrop PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiffdither PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiffdump PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiff-grayscale PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiffinfo PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiffmedian PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiff-palette PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiff-rgb PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiffset PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiffsplit PROPERTIES FOLDER external/libtiff)
#SET_TARGET_PROPERTIES (tiffxx PROPERTIES FOLDER external/libtiff)
SET (TIFF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libtiff/libtiff" "${CMAKE_CURRENT_BINARY_DIR}/libtiff/libtiff" CACHE INTERNAL "")

# freetype2
SET (BUILD_FRAMEWORK OFF)
SET (BUILD_SHARED_LIBS OFF)
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
