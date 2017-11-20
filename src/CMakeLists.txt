project(deepin-manual C CXX)

#set(CMAKE_C_COMPILER  /usr/bin/clang)
#set(CMAKE_CXX_COMPILER  /usr/bin/clang++)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (NOT DEFINED DMAN_MANUAL_DIR)
  set(DMAN_MANUAL_DIR ${CMAKE_CURRENT_BINARY_DIR}/dman)
endif ()
message("DMAN_MANUAL_DIR: ${DMAN_MANUAL_DIR}")

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
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  # -fsanitize=leak, Enable memory leak sanitizer.
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
  # -fsanitize=memory, Enable detecting uninitited memory sanitizer.
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory "
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
find_package(Qt5Widgets REQUIRED)

pkg_search_module(Dtk REQUIRED dtkwidget)
pkg_search_module(QCef REQUIRED libqcef)

include_directories(${Dtk_INCLUDE_DIRS})
include_directories(${QCef_INCLUDE_DIRS})

set(LINK_LIBS
    Qt5::Core
    Qt5::Widgets

    ${Dtk_LDFLAGS}
    ${QCef_LDFLAGS}
    )

set(BASE_FILES
    base/consts.cpp
    base/consts.h)

set(CONTROLLER_FILES
    controller/window_manager.cpp
    controller/window_manager.h)

set(RESOURCES_FILES
    resources/images/images.qrc
    resources/images/images.cpp
    resources/images/images.h
    )

set(VIEW_FILES
    view/web_window.cpp
    view/web_window.h

    view/widget/search_edit.cpp
    view/widget/search_edit.h
    view/widget/title_bar.cpp
    view/widget/title_bar.h
    )

add_executable(dman
               app/dman.cpp
               ${BASE_FILES}
               ${CONTROLLER_FILES}
               ${RESOURCES_FILES}
               ${VIEW_FILES}
               )
target_link_libraries(dman ${LINK_LIBS})

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/dman
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)