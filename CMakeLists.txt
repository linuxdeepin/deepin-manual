cmake_minimum_required(VERSION 3.0)

if (NOT DEFINED VERSION)
    set(VERSION 1.2.2)
endif ()

project(deepin-manual)

option(DMAN_RELEAE OFF "Install dman resources to system or not")

add_subdirectory(src)

option (USE_TEST "TEST" OFF)

if(USE_TEST)
add_subdirectory(dman_qtest)
endif(USE_TEST)
