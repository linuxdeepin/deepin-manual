cmake_minimum_required(VERSION 3.0)

if (NOT DEFINED VERSION)
    set(VERSION 1.2.2)
endif ()

project(deepin-manual)

option(DMAN_RELEAE OFF "Install dman resources to system or not")

add_subdirectory(src)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(tests)
endif()

