project(deepin-manual C CXX)

#set(CMAKE_C_COMPILER  /usr/bin/clang)
#set(CMAKE_CXX_COMPILER  /usr/bin/clang++)

add_definitions(-DDMAN_RESOURCE_DIR="${DMAN_RESOURCE_DIR}")
add_definitions(-DDMAN_MANUAL_DIR="${DMAN_MANUAL_DIR}")
message("DMAN_RESOURCE_DIR: ${DMAN_RESOURCE_DIR}")

add_subdirectory(dbus)
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
find_package(Qt5DBus REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5LinguistTools)
find_package(Qt5Sql REQUIRED)
find_package(Qt5WebChannel REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

pkg_search_module(DtkWidget REQUIRED dtkwidget)
pkg_search_module(QCef REQUIRED libqcef)

add_subdirectory(third_party/cppjieba)

include_directories(${DtkWidget_INCLUDE_DIRS})
include_directories(${QCef_INCLUDE_DIRS})
include_directories(third_party/cppjieba/deps)
include_directories(third_party/cppjieba/include)

set(LINK_LIBS
    Qt5::Core
    Qt5::DBus
    Qt5::Sql
    Qt5::Widgets
    Qt5::WebChannel

    ${DtkWidget_LDFLAGS}
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

set(DBUS_FILES
    dbus/dbus_consts.cpp
    dbus/dbus_consts.h
    dbus/dbusvariant/app_info.cpp
    dbus/dbusvariant/app_info.h
    dbus/launcher_interface.cpp
    dbus/launcher_interface.h
    dbus/manual_open_adapter.cpp
    dbus/manual_open_adapter.h
    dbus/manual_open_interface.cpp
    dbus/manual_open_interface.h
    dbus/manual_open_proxy.cpp
    dbus/manual_open_proxy.h
    dbus/manual_search_adapter.cpp
    dbus/manual_search_adapter.h
    dbus/manual_search_proxy.cpp
    dbus/manual_search_proxy.h
    )

set(RESOURCES_FILES
    resources/themes/images.cpp
    resources/themes/images.h
    resources/themes/themes.qrc
    )

set(VIEW_FILES
    view/i18n_proxy.cpp
    view/i18n_proxy.h
    view/image_viewer_proxy.cpp
    view/image_viewer_proxy.h
    view/manual_proxy.cpp
    view/manual_proxy.h
    view/search_proxy.cpp
    view/search_proxy.h
    view/theme_manager.cpp
    view/theme_manager.h
    view/title_bar_proxy.cpp
    view/title_bar_proxy.h
    view/web_event_delegate.cpp
    view/web_event_delegate.h
    view/web_window.cpp
    view/web_window.h

    view/widget/image_viewer.cpp
    view/widget/image_viewer.h
    view/widget/search_button.cpp
    view/widget/search_button.h
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
     ${CMAKE_SOURCE_DIR}/translations/deepin-manual*.ts)
set_source_files_properties(${DMAN_TRANSLATION_TS}
                            PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/translations)
qt5_add_translation(DMAN_TRANSLATION_QM ${DMAN_TRANSLATION_TS})
add_custom_target(update-qm DEPENDS ${DMAN_TRANSLATION_QM})

add_executable(dman
               app/dman.cpp
               ${BASE_FILES}
               ${CONTROLLER_FILES}
               ${DBUS_FILES}
               ${RESOURCES_FILES}
               ${VIEW_FILES}
               )
target_link_libraries(dman ${LINK_LIBS})
add_dependencies(dman update-qm)


add_executable(dman-search
               app/dman_search.cpp
               ${DBUS_FILES}
               )
target_link_libraries(dman-search ${LINK_LIBS})

if (CMAKE_BUILD_TYPE MATCHES Debug)
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

  set(IMAGE_VIEWER_DEMO_FILES
      app/image_viewer_demo.cpp
      base/file_util.cpp
      base/file_util.h
      view/theme_manager.cpp
      view/theme_manager.h
      view/widget/image_viewer.cpp
      view/widget/image_viewer.h
      )
  add_executable(image-viewer-demo
                 ${IMAGE_VIEWER_DEMO_FILES}
                 ${RESOURCES_FILES}
                 )
  target_link_libraries(image-viewer-demo ${LINK_LIBS})
endif ()


## Installation
install(TARGETS dman
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS dman-search
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(DIRECTORY ${CMAKE_BINARY_DIR}/translations
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-manual)

# Jieba Dictionary
set(JIEBA_DICT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cppjieba/dict)
if (NOT DEFINED JIEBA_DICT)
  set(JIEBA_DICT ${JIEBA_DICT_SRC})
endif ()
add_definitions(-DJIEBA_DICT="${JIEBA_DICT}")

install(DIRECTORY ${JIEBA_DICT_SRC}/
        DESTINATION ${JIEBA_DICT}
        PATTERN "pos_dict" EXCLUDE
        PATTERN "README.md" EXCLUDE
        )
