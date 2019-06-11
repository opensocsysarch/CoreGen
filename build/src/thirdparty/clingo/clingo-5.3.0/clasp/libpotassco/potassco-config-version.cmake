# Note: Adapted from Jonathan Müller's blog post on supporting CMake install:
# https://foonathan.github.io/blog/2016/03/03/cmake-install.html
#
# checks version: major must match, minor must be less than or equal

set(PACKAGE_VERSION 1.0)

if("${PACKAGE_FIND_VERSION_MAJOR}" EQUAL "1")
    if ("${PACKAGE_FIND_VERSION_MINOR}" EQUAL "0")
        set(PACKAGE_VERSION_EXACT TRUE)
    elseif(NOT ("1" EQUAL 0)
           AND "${PACKAGE_FIND_VERSION_MINOR}" LESS "0")
        set(PACKAGE_VERSION_COMPATIBLE TRUE)
    else()
        set(PACKAGE_VERSION_UNSUITABLE TRUE)
    endif()
else()
    set(PACKAGE_VERSION_UNSUITABLE TRUE)
endif()
