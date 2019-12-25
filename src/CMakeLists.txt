project(deepin-manual C CXX)

#set(CMAKE_C_COMPILER  /usr/bin/clang)
#set(CMAKE_CXX_COMPILER  /usr/bin/clang++)

set(CMAKE_INSTALL_PREFIX /usr)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  add_definitions(-DDMAN_SEARCH_DB="${CMAKE_INSTALL_PREFIX}/share/deepin-manual/search.db")
  add_definitions(-DDMAN_WEB_DIR="${CMAKE_CURRENT_SOURCE_DIR}/web_dist")
  add_definitions(-DDMAN_MANUAL_DIR="${CMAKE_SOURCE_DIR}/manual")
else()
  add_definitions(-DDMAN_SEARCH_DB="${CMAKE_INSTALL_PREFIX}/share/deepin-manual/search.db")
  add_definitions(-DDMAN_WEB_DIR="${CMAKE_INSTALL_PREFIX}/share/deepin-manual/web_dist")
  add_definitions(-DDMAN_MANUAL_DIR="${CMAKE_INSTALL_PREFIX}/share/deepin-manual/manual")
endif ()

configure_file(environments.h.in environments.h @ONLY)

# These macros ared used at compile time only.
add_definitions(-DDMAN_SEARCH_ORIG_DB="${CMAKE_CURRENT_BINARY_DIR}/search.db")
add_definitions(-DDMAN_ORIG_MANUAL_DIR="${CMAKE_SOURCE_DIR}/manual")
add_definitions(-DDMAN_SEARCH_WORK_DIR="${CMAKE_CURRENT_SOURCE_DIR}/web")

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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wextra -fno-rtti -fno-exceptions")
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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
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

pkg_search_module(DtkWidget REQUIRED dtkwidget)
pkg_search_module(DtkGui REQUIRED dtkgui)
pkg_search_module(LibQCef REQUIRED libqcef)

include_directories(${DtkWidget_INCLUDE_DIRS})
include_directories(${DtkGui_INCLUDE_DIRS})
include_directories(${LibQCef_INCLUDE_DIRS})

set(LINK_LIBS
    Qt5::Core
    Qt5::DBus
    Qt5::Sql
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::WebChannel

    ${DtkWidget_LDFLAGS}
    ${DtkGui_LDFLAGS}
    ${LibQCef_LDFLAGS}

    -lX11
    )

set(BASE_FILES
    base/command.cpp
    base/command.h
    base/consts.cpp
    base/consts.h
    base/file_util.cpp
    base/file_util.h
    base/utils.cpp
    base/utils.h
    base/sysinfo.cpp
    base/sysinfo.h
    )

set(CONTROLLER_FILES
    controller/argument_parser.cpp
    controller/argument_parser.h
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
    view/theme_proxy.cpp
    view/theme_proxy.h
    view/theme_manager.cpp
    view/theme_manager.h
    view/title_bar_proxy.cpp
    view/title_bar_proxy.h
    view/settings_proxy.h
    view/settings_proxy.cpp
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
    view/widget/search_completion_listview.cpp
    view/widget/search_completion_listview.h
    view/widget/search_completion_delegate.cpp
    view/widget/search_completion_delegate.h
    view/widget/search_edit.cpp
    view/widget/search_edit.h
    view/widget/manual_webview.cpp
    view/widget/manual_webview.h
    )

# Main executable
set(SEARCH_FILES
    app/generate_search_db.cpp
    base/command.cpp
    base/command.h
    controller/search_db.cpp
    controller/search_db.h
    )
add_executable(generate-search-db
               ${SEARCH_FILES})
target_link_libraries(generate-search-db ${LINK_LIBS})

# TODO(Shaohua): Generate search db by hand.
add_custom_command(OUTPUT _update-search-db
                   COMMAND pwd
                   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/generate-search-db
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                   )
add_custom_target(update-search-db DEPENDS _update-search-db)
add_dependencies(update-search-db
                 generate-search-db)

# Generate .qm files from .ts files.
file(GLOB DMAN_TRANSLATION_TS
     ${CMAKE_SOURCE_DIR}/translations/deepin-manual*.ts)
set_source_files_properties(${DMAN_TRANSLATION_TS}
                            PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/translations)
qt5_add_translation(DMAN_TRANSLATION_QM ${DMAN_TRANSLATION_TS})
add_custom_target(update-qm DEPENDS ${DMAN_TRANSLATION_QM})

# Build web project and generate js files.
add_custom_command(OUTPUT rebuild-web
                   COMMAND pwd
                   COMMAND /usr/bin/node /usr/bin/gulp build -d ${CMAKE_CURRENT_SOURCE_DIR}/web_dist
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/web/
                   )
add_custom_target(update-web DEPENDS rebuild-web)

add_executable(dman
               app/dman.cpp
               ${BASE_FILES}
               ${CONTROLLER_FILES}
               ${DBUS_FILES}
               ${RESOURCES_FILES}
               ${VIEW_FILES}
               )

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${LibQCef_LIBDIR}/qcef")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set_target_properties(dman
                      PROPERTIES INSTALL_RPATH "${LibQCef_LIBDIR}/qcef")
target_link_libraries(dman ${LibQCef_LIBDIR}/qcef/libcef.so
                      ${LINK_LIBS})

# Auto generate qm files and search.db in release mode.
if (CMAKE_BUILD_TYPE MATCHES Release)
  add_dependencies(dman
                   update-search-db
                   update-qm)
endif ()

add_executable(dman-search
               app/dman_search.cpp
               ${DBUS_FILES}
               )
target_link_libraries(dman-search
                      Qt5::Core
                      Qt5::DBus)

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
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-manual)
# NOTE(Shaohua): Add this script to by pass platform plugin issue in Qt.
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/app/dman.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        PERMISSIONS OWNER_READ OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
        RENAME dman)
install(TARGETS dman-search
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(DIRECTORY ${CMAKE_BINARY_DIR}/translations
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-manual)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/manual
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-manual)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/web_dist
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-manual)

string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} LOWERCASE_CMAKE_SYSTEM_PROCESSOR)

install(FILES ${CMAKE_SOURCE_DIR}/search.db
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-manual)

