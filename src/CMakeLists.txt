project(deepin-manual C CXX)

#set(CMAKE_C_COMPILER  /usr/bin/clang)
#set(CMAKE_CXX_COMPILER  /usr/bin/clang++)

add_definitions(-DDMAN_RESOURCE_DIR="${DMAN_RESOURCE_DIR}")
add_definitions(-DDMAN_MANUAL_DIR="${DMAN_MANUAL_DIR}")
message("DMAN_RESOURCE_DIR: ${DMAN_RESOURCE_DIR}")

add_subdirectory(resources)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror -Wextra")
  # -fno-rtti, Disable real-time type information
  # -fno-exceptions Disable exceptions
  # -fsanitize=address Enable memory sanitize plugin
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wextra -fno-exceptions")
  # -fsanitize=address, Enable memory address sanitizer.
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  # -fsanitize=leak, Enable memory leak sanitizer.
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
  # -fsanitize=memory, Enable detecting uninitited memory sanitizer.
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
  #    "-fsanitize-memory-track-origins -fPIC -pie -fno-omit-frame-pointer")

  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wimplicit-fallthrough=1")

  # Enable Qt builtin debug mode
  add_definitions("-DQT_MESSAGELOGCONTEXT")
else()
  # -Wl, -O2 Enable linker optimizations
  # -Wl, --gc-sections Remove unused code resulting from -fdsta-sections and
  # -ffunction-sections
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections")
endif()

find_package(PkgConfig REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5LinguistTools)
find_package(Qt5Sql REQUIRED)
find_package(Qt5WebChannel REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

pkg_search_module(Dtk REQUIRED dtkwidget)
pkg_search_module(QCef REQUIRED libqcef)

add_subdirectory(third_party/cppjieba)

include_directories(${Dtk_INCLUDE_DIRS})
include_directories(${QCef_INCLUDE_DIRS})
include_directories(third_party/cppjieba/deps)
include_directories(third_party/cppjieba/include)

set(LINK_LIBS
    Qt5::Core
    Qt5::Sql
    Qt5::Widgets
    Qt5::WebChannel

    ${Dtk_LDFLAGS}
    ${QCef_LDFLAGS}
    )

set(BASE_FILES
    base/consts.cpp
    base/consts.h
    base/file_util.cpp
    base/file_util.h
    )

set(CONTROLLER_FILES
    controller/search_db.cpp
    controller/search_db.h
    controller/search_manager.cpp
    controller/search_manager.h
    controller/search_result.h
    controller/window_manager.cpp
    controller/window_manager.h
    )

set(RESOURCES_FILES
    resources/themes/images.cpp
    resources/themes/images.h
    resources/themes/themes.qrc
    )

set(VIEW_FILES
    view/search_proxy.cpp
    view/search_proxy.h
    view/theme_manager.cpp
    view/theme_manager.h
    view/title_bar_proxy.cpp
    view/title_bar_proxy.h
    view/web_window.cpp
    view/web_window.h

    view/widget/search_completion_window.cpp
    view/widget/search_completion_window.h
    view/widget/search_edit.cpp
    view/widget/search_edit.h
    view/widget/title_bar.cpp
    view/widget/title_bar.h
    )

# Main executable
# Generate .qm files from .ts files.
file(GLOB DMAN_TRANSLATION_TS
     ${CMAKE_SOURCE_DIR}/i18n/dman-*.ts)
set_source_files_properties(${DMAN_TRANSLATION_TS}
                            PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/i18n)
qt5_add_translation(DMAN_TRANSLATION_QM ${DMAN_TRANSLATION_TS})
add_custom_target(update-qm DEPENDS ${DMAN_TRANSLATION_QM})

add_executable(dman
               app/dman.cpp
               ${BASE_FILES}
               ${CONTROLLER_FILES}
               ${RESOURCES_FILES}
               ${VIEW_FILES}
               )
target_link_libraries(dman ${LINK_LIBS})
add_dependencies(dman update-qm)

set(SEARCH_DEMO_FILES
    app/search_demo.cpp
    base/consts.cpp
    base/consts.h
    controller/search_db.cpp
    controller/search_db.h
    controller/search_manager.cpp
    controller/search_manager.h
    )
add_executable(search-demo ${SEARCH_DEMO_FILES})
target_link_libraries(search-demo ${LINK_LIBS})

install(TARGETS dman
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

# Jieba Dictionary
set(JIEBA_DICT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cppjieba/dict)
if (NOT DEFINED JIEBA_DICT)
  set(JIEBA_DICT ${JIEBA_DICT_SRC})
endif ()
add_definitions(-DJIEBA_DICT="${JIEBA_DICT}")

install(DIRECTORY ${JIEBA_DICT_SRC}
        DESTINATION ${JIEBA_DICT})