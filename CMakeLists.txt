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

# systemd service
if (NOT DEFINED SYSTEMD_USER_UNIT_DIR)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(Systemd REQUIRED systemd)
    pkg_get_variable(SYSTEMD_USER_UNIT_DIR systemd systemduserunitdir)
endif()

install(FILES ${CMAKE_SOURCE_DIR}/misc/deepin-manual.service
        DESTINATION ${SYSTEMD_USER_UNIT_DIR})