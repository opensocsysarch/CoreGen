#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libclasp" for configuration "Release"
set_property(TARGET libclasp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libclasp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/clasp-3.3.4/libclasp.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS libclasp )
list(APPEND _IMPORT_CHECK_FILES_FOR_libclasp "${_IMPORT_PREFIX}/lib/clasp-3.3.4/libclasp.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
