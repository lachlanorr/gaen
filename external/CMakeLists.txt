
INCLUDE (ExternalProject)

SET (GAEN_BUILD_FLAGS
  "-DCMAKE_CXX_FLAGS_DEBUG:STRING=${CMAKE_CXX_FLAGS_DEBUG}"
  "-DCMAKE_CXX_FLAGS_RELEASE:STRING=${CMAKE_CXX_FLAGS_RELEASE}"
  "-DCMAKE_CXX_FLAGS_MINSIZEREL:STRING=${CMAKE_CXX_FLAGS_MINSIZEREL}"
  "-DCMAKE_CXX_FLAGS_RELWITHDEBINFO:STRING=${CMAKE_CXX_FLAGS_RELWITHDEBINFO}"
  "-DCMAKE_C_FLAGS_DEBUG:STRING=${CMAKE_C_FLAGS_DEBUG}"
  "-DCMAKE_C_FLAGS_RELEASE:STRING=${CMAKE_C_FLAGS_RELEASE}"
  "-DCMAKE_C_FLAGS_MINSIZEREL:STRING=${CMAKE_C_FLAGS_MINSIZEREL}"
  "-DCMAKE_C_FLAGS_RELWITHDEBINFO:STRING=${CMAKE_C_FLAGS_RELWITHDEBINFO}"
  )

# zlib
SET (ZLIB_PATH "${CMAKE_CURRENT_BINARY_DIR}/zlib")
SET (ZLIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/zlib")
ExternalProject_Add(zlib
  SOURCE_DIR "${ZLIB_SRC}"
  PREFIX "${ZLIB_PATH}"
  EXCLUDE_FROM_ALL TRUE
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${ZLIB_PATH}"
             ${GAEN_BUILD_FLAGS}
  )
SET (ZLIB_INCLUDE "${ZLIB_PATH}/include" CACHE INTERNAL "")
SET (ZLIB_LIB "${ZLIB_PATH}/lib/zlibstatic.lib" CACHE INTERNAL "")
SET (ZLIB_LIBD "${ZLIB_PATH}/lib/zlibstaticd.lib" CACHE INTERNAL "")
SET (ZLIB_LIBRARY
  optimized "${ZLIB_LIB}"
  debug "${ZLIB_LIBD}"
  CACHE INTERNAL ""
  )

# brotli
SET (BROTLI_SRC "${CMAKE_CURRENT_SOURCE_DIR}/brotli")
SET (BROTLI_PATH "${CMAKE_CURRENT_BINARY_DIR}/brotli")
ExternalProject_Add(brotli
  SOURCE_DIR "${BROTLI_SRC}"
  PREFIX "${BROTLI_PATH}"
  EXCLUDE_FROM_ALL TRUE
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${BROTLI_PATH}"
             "-DCMAKE_DEBUG_POSTFIX:STRING=d"
             ${GAEN_BUILD_FLAGS}
  )
SET (BROTLI_INCLUDE "${BROTLI_PATH}/include")
SET (BROTLI_LIBRARY
  optimized "${BROTLI_PATH}/lib/brotlicommon-static.lib"
  optimized "${BROTLI_PATH}/lib/brotlidec-static.lib"
  optimized "${BROTLI_PATH}/lib/brotlienc-static.lib"
  debug "${BROTLI_PATH}/lib/brotlicommon-staticd.lib"
  debug "${BROTLI_PATH}/lib/brotlidec-staticd.lib"
  debug "${BROTLI_PATH}/lib/brotlienc-staticd.lib"
  CACHE INTERNAL ""
  )

# libpng
SET (LIBPNG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/libpng")
SET (LIBPNG_PATH "${CMAKE_CURRENT_BINARY_DIR}/libpng")
ExternalProject_Add(libpng
  SOURCE_DIR "${LIBPNG_SRC}"
  PREFIX "${LIBPNG_PATH}"
  EXCLUDE_FROM_ALL TRUE
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${LIBPNG_PATH}"
             "-DZLIB_INCLUDE_DIR:PATH=${ZLIB_INCLUDE}"
             "-DZLIB_LIBRARY_RELEASE:PATH=${ZLIB_LIB}"
             "-DZLIB_LIBRARY_DEBUG:PATH=${ZLIB_LIBD}"
             "-DBUILD_ZLIB:STRING=ON"
             "-DPNG_SHARED:STRING=OFF"
             "-DPNG_TESTS:STRING=OFF"
             ${GAEN_BUILD_FLAGS}
  )
SET (LIBPNG_INCLUDE "${LIBPNG_PATH}/include" CACHE INTERNAL "")
SET (LIBPNG_LIB "${LIBPNG_PATH}/lib/libpng16_static.lib" CACHE INTERNAL "")
SET (LIBPNG_LIBD "${LIBPNG_PATH}/lib/libpng16_staticd.lib" CACHE INTERNAL "")
SET (LIBPNG_LIBRARY
  optimized "${LIBPNG_LIB}"
  debug "${LIBPNG_LIBD}"
  CACHE INTERNAL ""
  )


# libtiff
SET (LIBTIFF_SRC "${CMAKE_CURRENT_SOURCE_DIR}/libtiff")
SET (LIBTIFF_PATH "${CMAKE_CURRENT_BINARY_DIR}/libtiff")
ExternalProject_Add(libtiff
  SOURCE_DIR "${LIBTIFF_SRC}"
  PREFIX "${LIBTIFF_PATH}"
  EXCLUDE_FROM_ALL TRUE
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${LIBTIFF_PATH}"
             "-DBUILD_SHARED_LIBS:STRING=OFF"
             "-DBUILD_PORT:STRING=OFF"
             "-DBUILD_TOOLS:STRING=OFF"
             "-DBUILD_TEST:STRING=OFF"
             "-DBUILD_CONTRIB:STRING=OFF"
             "-DBUILD_BUILD:STRING=OFF"
             "-DBUILD_MAN:STRING=OFF"
             "-DBUILD_HTML:STRING=OFF"
             ${GAEN_BUILD_FLAGS}
  )
SET (LIBTIFF_INCLUDE "${LIBTIFF_PATH}/include" CACHE INTERNAL "")
SET (LIBTIFF_LIBRARY
  optimized "${LIBTIFF_PATH}/lib/tiff.lib"
  debug "${LIBTIFF_PATH}/lib/tiffd.lib"
  CACHE INTERNAL ""
  )


# FreeType
SET (FREETYPE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/freetype2")
SET (FREETYPE_PATH "${CMAKE_CURRENT_BINARY_DIR}/freetype2")
INCLUDE_DIRECTORIES ("${BROTLI_INCLUDE}")
ExternalProject_Add(freetype
  SOURCE_DIR "${FREETYPE_SRC}"
  PREFIX "${FREETYPE_PATH}"
  EXCLUDE_FROM_ALL TRUE
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${FREETYPE_PATH}"
             "-DBUILD_SHARED_LIBS:STRING=OFF"
             "-DBROTLI_INCLUDE_DIR:PATH=${BROTLI_INCLUDE}"
             "-DZLIB_INCLUDE_DIR:PATH=${ZLIB_INCLUDE}"
             "-DZLIB_LIBRARY_RELEASE:PATH=${ZLIB_LIB}"
             "-DZLIB_LIBRARY_DEBUG:PATH=${ZLIB_LIBD}"
             "-DPNG_PNG_INCLUDE_DIR:PATH=${LIBPNG_INCLUDE}"
             "-DPNG_LIBRARY_RELEASE:PATH=${LIBPNG_LIB}"
             "-DPNG_LIBRARY_DEBUG:PATH=${LIBPNG_LIBD}"
             ${GAEN_BUILD_FLAGS}
  )
SET (FREETYPE_INCLUDE "${FREETYPE_PATH}/include/freetype2" CACHE INTERNAL "")
SET (FREETYPE_LIBRARY
  optimized "${FREETYPE_PATH}/lib/freetype.lib"
  debug "${FREETYPE_PATH}/lib/freetyped.lib"
  CACHE INTERNAL ""
  )


# glfw
SET (GLFW_SRC "${CMAKE_CURRENT_SOURCE_DIR}/glfw")
SET (GLFW_PATH "${CMAKE_CURRENT_BINARY_DIR}/glfw")
ExternalProject_Add(glfw
  SOURCE_DIR "${GLFW_SRC}"
  PREFIX "${GLFW_PATH}"
  EXCLUDE_FROM_ALL TRUE
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${GLFW_PATH}"
             "-DCMAKE_DEBUG_POSTFIX:STRING=d"
             "-DCMAKE_MSVC_RUNTIME_LIBRARY:STRING="
             "-DBUILD_SHARED_LIBS:STRING=OFF"
             "-DGLFW_BUILD_EXAMPLES:STRING=OFF"
             "-DGLFW_BUILD_TESTS:STRING=OFF"
             "-DGLFW_BUILD_DOCS:STRING=OFF"
             ${GAEN_BUILD_FLAGS}
  )
SET (GLFW_INCLUDE "${GLFW_PATH}/include" CACHE INTERNAL "")
SET (GLFW_LIBRARY
  optimized "${GLFW_PATH}/lib/glfw3.lib"
  debug "${GLFW_PATH}/lib/glfw3d.lib"
  CACHE INTERNAL ""
  )

# bullet
SET (BULLET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/bullet3")
SET (BULLET_PATH "${CMAKE_CURRENT_BINARY_DIR}/bullet3")
ExternalProject_Add(bullet
  SOURCE_DIR "${BULLET_SRC}"
  PREFIX "${BULLET_PATH}"
  EXCLUDE_FROM_ALL TRUE
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${BULLET_PATH}"
             "-DBUILD_CPU_DEMOS:STRING=OFF"
             "-DUSE_GLUT:STRING=OFF"
             "-DBUILD_PYBULLET:STRING=OFF"
             "-DBUILD_ENET:STRING=OFF"
             "-DBUILD_CLSOCKET:STRING=OFF"
             "-DBUILD_BULLET2_DEMOS:STRING=OFF"
             "-DBUILD_EXTRAS:STRING=OFF"
             "-DBUILD_UNIT_TESTS:STRING=OFF"
             "-DINSTALL_LIBS:STRING=ON"
             "-DINSTALL_CMAKE_FILES:STRING=ON"
             ${GAEN_BUILD_FLAGS}
  )
SET (BULLET_INCLUDE "${BULLET_PATH}/include/bullet" CACHE INTERNAL "")
SET (BULLET_LIBRARY
  optimized "${BULLET_PATH}/lib/BulletCollision.lib"
  optimized "${BULLET_PATH}/lib/BulletDynamics.lib"
  optimized "${BULLET_PATH}/lib/LinearMath.lib"
  debug "${BULLET_PATH}/lib/BulletCollision_Debug.lib"
  debug "${BULLET_PATH}/lib/BulletDynamics_Debug.lib"
  debug "${BULLET_PATH}/lib/LinearMath_Debug.lib"
  CACHE INTERNAL ""
  )


# assimp
SET (ASSIMP_SRC "${CMAKE_CURRENT_SOURCE_DIR}/assimp")
SET (ASSIMP_PATH "${CMAKE_CURRENT_BINARY_DIR}/assimp")
ExternalProject_Add(assimp
  SOURCE_DIR "${ASSIMP_SRC}"
  PREFIX "${ASSIMP_PATH}"
  EXCLUDE_FROM_ALL TRUE
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${ASSIMP_PATH}"
             "-DBUILD_SHARED_LIBS:STRING=OFF"
             "-DASSIMP_INSTALL_PDB:STRING=OFF"
             "-DASSIMP_BUILD_ZLIB:STRING=OFF"
             "-DZLIB_INCLUDE_DIR:PATH=${ZLIB_INCLUDE}"
             "-DASSIMP_BUILD_ASSIMP_TOOLS:STRING=OFF"
             "-DASSIMP_BUILD_ASSIMP_SAMPLES:STRING=OFF"
             "-DASSIMP_BUILD_TESTS:STRING=OFF"
             "-DASSIMP_BUILD_ASSIMP_VIEW:STRING=OFF"
             "-DZLIB_FOUND:STRING=ON"
             ${GAEN_BUILD_FLAGS}
  )
SET (ASSIMP_INCLUDE "${ASSIMP_PATH}/include" CACHE INTERNAL "")
SET (ASSIMP_LIBRARY
  optimized "${ASSIMP_PATH}/lib/assimp-vc142-mt.lib"
  optimized "${ASSIMP_PATH}/lib/draco.lib"
  debug "${ASSIMP_PATH}/lib/assimp-vc142-mtd.lib"
  debug "${ASSIMP_PATH}/lib/dracod.lib"
  CACHE INTERNAL ""
  )

# rapidjson
SET (RAPIDJSON_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include" CACHE INTERNAL "")

add_custom_target (external_build_all)
add_custom_command (TARGET external_build_all
  COMMAND ${CMAKE_COMMAND} --build . --target zlib --config Debug
  COMMAND ${CMAKE_COMMAND} --build . --target zlib --config Release
  COMMAND ${CMAKE_COMMAND} --build . --target brotli --config Debug
  COMMAND ${CMAKE_COMMAND} --build . --target brotli --config Release
  COMMAND ${CMAKE_COMMAND} --build . --target libpng --config Debug
  COMMAND ${CMAKE_COMMAND} --build . --target libpng --config Release
  COMMAND ${CMAKE_COMMAND} --build . --target libtiff --config Debug
  COMMAND ${CMAKE_COMMAND} --build . --target libtiff --config Release
  COMMAND ${CMAKE_COMMAND} --build . --target freetype --config Debug
  COMMAND ${CMAKE_COMMAND} --build . --target freetype --config Release
  COMMAND ${CMAKE_COMMAND} --build . --target glfw --config Debug
  COMMAND ${CMAKE_COMMAND} --build . --target glfw --config Release
  COMMAND ${CMAKE_COMMAND} --build . --target bullet --config Debug
  COMMAND ${CMAKE_COMMAND} --build . --target bullet --config Release
  COMMAND ${CMAKE_COMMAND} --build . --target assimp --config Debug
  COMMAND ${CMAKE_COMMAND} --build . --target assimp --config Release
  )
set_target_properties (external_build_all PROPERTIES
  FOLDER ExternalProjectTargets
  )
