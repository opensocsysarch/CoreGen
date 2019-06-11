include("${CMAKE_CURRENT_LIST_DIR}/clasp.cmake")

find_package(potassco 1.0 REQUIRED)

if(ON)
    find_package(Threads REQUIRED)
endif()

