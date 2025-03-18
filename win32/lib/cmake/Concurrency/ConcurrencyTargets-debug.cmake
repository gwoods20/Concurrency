#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Concurrency::Concurrency" for configuration "Debug"
set_property(TARGET Concurrency::Concurrency APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Concurrency::Concurrency PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/Concurrencyd.lib"
  )

list(APPEND _cmake_import_check_targets Concurrency::Concurrency )
list(APPEND _cmake_import_check_files_for_Concurrency::Concurrency "${_IMPORT_PREFIX}/lib/Concurrencyd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
