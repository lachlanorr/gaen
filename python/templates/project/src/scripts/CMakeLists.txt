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
  python ${python_dir}/codegen.py
  # re-run update_hashes.py since generated .cpp files will likely
  # have new hashes to consider.
  COMMAND python ${python_dir}/update_hashes.py "${CMAKE_BINARY_DIR}/src/hashes"
  DEPENDS cmpc
  COMMENT "Generating scripts"
  )

ADD_DEPENDENCIES(scripts
  hashes
  engine
  CODEGEN_SCRIPTS
  )
