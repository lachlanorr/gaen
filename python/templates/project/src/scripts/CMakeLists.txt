INCLUDE (codegen.cmake)

SET (scripts_standard_SOURCES
  registration.cpp
  )

SET (scripts_SOURCES
  ${scripts_standard_SOURCES}
  ${scripts_codegen_SOURCES}
  )

IDE_SOURCE_PROPERTIES( "/" "${scripts_standard_SOURCES}" )

ADD_LIBRARY (scripts
  ${scripts_SOURCES}
  )

ADD_CUSTOM_TARGET (
  CODEGEN_SCRIPTS ALL
  COMMAND python ${python_dir}/codegen.py
  DEPENDS cmpc
  )
