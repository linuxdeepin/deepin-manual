cmake_minimum_required(VERSION 3.0)
project(deepin-manual)

if (NOT DEFINED DMAN_MANUAL_DIR)
  set(DMAN_MANUAL_DIR ${CMAKE_CURRENT_BINARY_DIR}/dman)
endif ()

add_subdirectory(src)

if (DEFINED DMAN_RESOURCE_DIR)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/PageRoot
          DESTINATION ${DMAN_RESOURCE_DIR})
endif ()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dman
        DESTINATION ${DMAN_MANUAL_DIR})