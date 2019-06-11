#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libreify" for configuration "Release"
set_property(TARGET libreify APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libreify PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libreify.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS libreify )
list(APPEND _IMPORT_CHECK_FILES_FOR_libreify "${_IMPORT_PREFIX}/lib/libreify.a" )

# Import target "libgringo" for configuration "Release"
set_property(TARGET libgringo APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libgringo PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libgringo.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS libgringo )
list(APPEND _IMPORT_CHECK_FILES_FOR_libgringo "${_IMPORT_PREFIX}/lib/libgringo.a" )

# Import target "libclingo" for configuration "Release"
set_property(TARGET libclingo APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libclingo PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libclingo.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS libclingo )
list(APPEND _IMPORT_CHECK_FILES_FOR_libclingo "${_IMPORT_PREFIX}/lib/libclingo.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
