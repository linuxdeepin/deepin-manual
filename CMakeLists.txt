cmake_minimum_required(VERSION 3.0)
project(deepin-manual)

if (NOT DEFINED DMAN_MANUAL_DIR)
  set(DMAN_MANUAL_DIR ${CMAKE_INSTALL_PREFIX}/share/dman)
endif ()
if (NOT DEFINED DMAN_RESOURCE_DIR)
  set(DMAN_RESOURCE_DIR ${CMAKE_SOURCE_DIR}/PageRoot)
endif ()

add_subdirectory(src)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/PageRoot/www
        DESTINATION ${DMAN_RESOURCE_DIR})