project(deepin-manual C CXX)

#set(CMAKE_C_COMPILER  /usr/bin/clang)
#set(CMAKE_CXX_COMPILER  /usr/bin/clang++)

if (DMAN_RELEASE)
  add_definitions(-DDMAN_SEARCH_DB="${CMAKE_INSTALL_PREFIX}/share/deepin-manual/search.db")
  add_definitions(-DDMAN_WEB_DIR="${CMAKE_INSTALL_PREFIX}/share/deepin-manual/web")
  add_definitions(-DDMAN_MANUAL_DIR="${CMAKE_INSTALL_PREFIX}/share/deepin-manual/manual")
else()
  add_definitions(-DDMAN_SEARCH_DB="${CMAKE_CURRENT_BINARY_DIR}/search.db")
  add_definitions(-DDMAN_WEB_DIR="${CMAKE_CURRENT_BINARY_DIR}/web")
  add_definitions(-DDMAN_MANUAL_DIR="${CMAKE_SOURCE_DIR}/manual")
endif ()
add_definitions(-DDMAN_SEARCH_ORIG_DB="${CMAKE_CURRENT_BINARY_DIR}/search.db")
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

include_directories(${DtkWidget_INCLUDE_DIRS})
include_directories(${QCef_INCLUDE_DIRS})

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
    base/command.cpp
    base/command.h
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
set(SEARCH_FILES
    app/update_search_db.cpp
    base/command.cpp
    base/command.h
    controller/search_db.cpp
    controller/search_db.h
    )
add_executable(update-search-db
               ${SEARCH_FILES})
target_link_libraries(update-search-db ${LINK_LIBS})

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
                   COMMAND ./node_modules/.bin/gulp build -d ${CMAKE_CURRENT_BINARY_DIR}/web
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
target_link_libraries(dman ${LINK_LIBS})
add_dependencies(dman
                 update-qm
                 update-web)

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

install(DIRECTORY ${CMAKE_SOURCE_DIR}/manual
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-manual)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/web
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-manual)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/search.db
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-manual)