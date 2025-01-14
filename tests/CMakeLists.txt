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

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

ADD_COMPILE_OPTIONS(-fno-access-control)

set(qt_required_components Core DBus Gui Widgets Sql WebChannel WebEngineWidgets Test)

if (QT_DESIRED_VERSION MATCHES 6)
    list(APPEND qt_required_components Core5Compat)
else()
    # list(APPEND qt_required_components X11Extras )
endif()

find_package(Qt${QT_DESIRED_VERSION} REQUIRED COMPONENTS ${qt_required_components})
find_package(Dtk${DTK_VERSION_MAJOR} REQUIRED COMPONENTS Widget Gui Core)

set(LINK_LIBS
    Qt${QT_DESIRED_VERSION}::Core
    Qt${QT_DESIRED_VERSION}::DBus
    Qt${QT_DESIRED_VERSION}::Gui
    Qt${QT_DESIRED_VERSION}::Widgets
    Qt${QT_DESIRED_VERSION}::Sql
    Qt${QT_DESIRED_VERSION}::WebChannel
    Qt${QT_DESIRED_VERSION}::WebEngineWidgets
    Qt${QT_DESIRED_VERSION}::Test
    Dtk${DTK_VERSION_MAJOR}::Widget
    Dtk${DTK_VERSION_MAJOR}::Gui
    Dtk${DTK_VERSION_MAJOR}::Core
)

if (QT_DESIRED_VERSION MATCHES 6)
    list(APPEND LINK_LIBS Qt${QT_DESIRED_VERSION}::Core5Compat)
    # list(APPEND LINK_LIBS Qt${QT_VERSION_MAJOR}::GuiPrivate)
else()
    # list(APPEND LINK_LIBS Qt${QT_VERSION_MAJOR}::WidgetsPrivate)
    # list(APPEND LINK_LIBS Qt${QT_DESIRED_VERSION}::X11Extras)
endif()

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
    ../src/base/eventlogutils.cpp
    ../src/controller/*.cpp
    ../src/dbus/*.cpp
    ../src/dbus/dbusvariant/*.cpp
#    ../src/resources/*
    ../src/view/*.cpp
    ../src/view/widget/*.cpp)

FILE (GLOB dmanHeaderFiles
    ../src/base/consts.h
    ../src/base/utils.h
    ../src/base/eventlogutils.h
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
