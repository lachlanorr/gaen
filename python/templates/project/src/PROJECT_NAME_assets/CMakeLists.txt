
set(<<PROJECT_NAME>>_ASSETS_SOURCES
  <<PROJECT_NAME>>_assets.cpp
  )

msvc_precompiled_header(<<PROJECT_NAME>>_assets <<PROJECT_NAME>>_ASSETS_SOURCES)
source_group("" FILES ${<<PROJECT_NAME>>_ASSETS_SOURCES})

add_library(<<PROJECT_NAME>>_assets
  ${<<PROJECT_NAME>>_ASSETS_SOURCES}
  )
