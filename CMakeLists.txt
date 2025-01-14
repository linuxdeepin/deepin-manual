cmake_minimum_required(VERSION 3.0)

if (NOT DEFINED VERSION)
    set(VERSION 1.2.2)
endif ()

project(deepin-manual)

option(DMAN_RELEAE OFF "Install dman resources to system or not")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
# 引入翻译生成
include(translation-generate)

# Find Qt version
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
message("   >>> Found Qt version: ${QT_VERSION_MAJOR}")
set(QT_DESIRED_VERSION ${QT_VERSION_MAJOR})

if (QT_VERSION_MAJOR MATCHES 6)
    set(DTK_VERSION_MAJOR 6)
else()
    set(DTK_VERSION_MAJOR "")
endif()
message("   >>> Build with DTK: ${DTK_VERSION_MAJOR}")

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