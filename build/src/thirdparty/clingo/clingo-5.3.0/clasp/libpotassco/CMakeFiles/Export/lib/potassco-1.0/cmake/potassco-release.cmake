#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libpotassco" for configuration "Release"
set_property(TARGET libpotassco APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libpotassco PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/potassco-1.0/libpotassco.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS libpotassco )
list(APPEND _IMPORT_CHECK_FILES_FOR_libpotassco "${_IMPORT_PREFIX}/lib/potassco-1.0/libpotassco.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
