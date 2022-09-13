# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: CC0-1.0

cmake_minimum_required(VERSION 3.7)

if (NOT DEFINED VERSION)
    set(VERSION 1.2.2)
endif ()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_SAFETYTEST "${CMAKE_SAFETYTEST_ARG}")

if(CMAKE_SAFETYTEST STREQUAL "")
    set(CMAKE_SAFETYTEST "CMAKE_SAFETYTEST_ARG_OFF")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${CMAKE_SAFETYTEST}")

if(CMAKE_SAFETYTEST STREQUAL "CMAKE_SAFETYTEST_ARG_ON")
  #安全测试选项
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=undefined,address -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=undefined,address -O2")
endif()


find_package(PkgConfig REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5LinguistTools)
find_package(Qt5Sql REQUIRED)
find_package(Qt5WebChannel REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
find_package(Qt5Test REQUIRED)

pkg_search_module(DtkWidget REQUIRED dtkwidget)
pkg_search_module(DtkGui REQUIRED dtkgui)
pkg_search_module(DtkCore REQUIRED dtkcore)

include_directories(${DtkWidget_INCLUDE_DIRS})
include_directories(${DtkGui_INCLUDE_DIRS})
include_directories(${DtkCore_INCLUDE_DIRS})

ADD_COMPILE_OPTIONS(-fno-access-control)

set(LINK_LIBS
    Qt5::Core
    Qt5::DBus
    Qt5::Sql
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::WebChannel
    Qt5::WebEngineWidgets
    Qt5::Test
    ${}

    ${DtkWidget_LDFLAGS}
    ${DtkGui_LDFLAGS}
    ${DtkCore_LDFLAGS}

    -lX11)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  add_definitions(-DDMAN_SEARCH_DB="${CMAKE_SOURCE_DIR}/manual-assets")
  add_definitions(-DDMAN_WEB_DIR="${CMAKE_CURRENT_SOURCE_DIR}/web_dist")
  add_definitions(-DDMAN_MANUAL_DIR="${CMAKE_SOURCE_DIR}/manual-assets")
else()
  add_definitions(-DDMAN_SEARCH_DB="${CMAKE_INSTALL_PREFIX}/share/deepin-manual/manual-assets")
  add_definitions(-DDMAN_WEB_DIR="${CMAKE_INSTALL_PREFIX}/share/deepin-manual/web_dist")
  add_definitions(-DDMAN_MANUAL_DIR="${CMAKE_INSTALL_PREFIX}/share/deepin-manual/manual-assets")
endif ()

#configure_file(../src/environments.h.in environments.h @ONLY)

# These macros ared used at compile time only.
add_definitions(-DDMAN_SEARCH_ORIG_DB="${CMAKE_CURRENT_BINARY_DIR}/search.db")
add_definitions(-DDMAN_ORIG_MANUAL_DIR="${CMAKE_SOURCE_DIR}/manual-assets")
add_definitions(-DDMAN_SEARCH_WORK_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../src/web")
add_definitions(-DDMAN_SEARCH_CREATE_DB_PATH="${CMAKE_CURRENT_BINARY_DIR}/dbPath")
# This is for icon path on db
add_definitions(-DDMAN_INSTALL_DB_PATH="${CMAKE_INSTALL_PREFIX}/share/deepin-manual/manual-assets")

#include_directories(googletest)
#include_directories(googletest/googlemock/include)
#include_directories(googletest/googletest/include)

##GTest & GMock
#add_subdirectory(googletest)



include_directories(../src)

set(PROJECT_NAME_TEST
    ${PROJECT_NAME}-test)

# 定义 source 文件
FILE (GLOB allTestSource
    src/base/*.cpp
    src/controller/*.cpp
    src/dbus/*.cpp
    src/dbus/dbusvariant/*.cpp
    src/view/*.cpp
    src/view/widget/*.cpp
    src/test_main.cpp
    src/third-party/stub/*
    )

#Include all app source files
FILE (GLOB dmanCppFiles
    ../src/base/consts.cpp
    ../src/base/utils.cpp
    ../src/controller/*.cpp
    ../src/dbus/*.cpp
    ../src/dbus/dbusvariant/*.cpp
#    ../src/resources/*
    ../src/view/*.cpp
    ../src/view/widget/*.cpp)

FILE (GLOB dmanHeaderFiles
    ../src/base/consts.h
    ../src/base/utils.h
    ../src/controller/*.h
    ../src/dbus/*.h
    ../src/dbus/dbusvariant/*.h
#    ../src/resources/*.h
    ../src/view/*.h
    ../src/widget/*.h)

FILE (GLOB dmanResourcesFiles
    ../src/resources/themes/images.cpp
    ../src/resources/themes/images.h
    ../src/resources/themes/themes.qrc
    )

# 生成测试可执行程序
add_executable(${PROJECT_NAME_TEST}
    ${dmanCppFiles}
    ${dmanHeaderFiles}
    ${dmanResourcesFiles}
    ${allTestSource}
)

target_include_directories(${PROJECT_NAME_TEST} PUBLIC ${Qt5Widgets_LIBRARIES}
                                                       ${Qt5DBus_LIBRARIES}
                                                       ${Qt5TestLib_LIBRARIES}
                                                       ${Qt5Widgets_PRIVATE_INCLUDE_DIRS}
                                                       ${Qt5Core_PRIVATE_INCLUDE_DIRS}
                                                       ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
                                                       ${DtkWidget_INCLUDE_DIRS}
                                                       ${DtkCore_INCLUDE_DIRS}
                                                       ${DtkGui_INCLUDE_DIRS}
                                                       ${PROJECT_BINARY_DIR})
# 生成测试可执行程序的依赖库
target_link_libraries(
    ${PROJECT_NAME_TEST}
    PUBLIC ${LINK_LIBS}
    ${GTEST_LIBRARYS}
    ${GTEST_MAIN_LIBRARYS}
    gtest gtest_main pthread)

#------------------------------ 创建'make test'指令---------------------------------------
add_custom_target(test)

add_custom_command(TARGET test
    COMMAND echo " =================== TEST BEGIN ==================== "

#    #1.清理原先的gcov信息
    COMMAND ${CMAKE_BINARY_DIR}/tests/${PROJECT_NAME_TEST}
    COMMAND echo " =================== TEST END ==================== "
    )

##'make test'命令依赖与我们的测试程序
add_dependencies(test ${PROJECT_NAME_TEST})

## 设置添加gocv相关信息的输出
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -fprofile-arcs -ftest-coverage -fno-inline -fstack-protector-strong -D_FORTITY_SOURCE=1 -z noexecstack -pie -fPIC -z lazy")
