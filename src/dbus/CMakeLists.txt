if(DEFINED ENV{PREFIX})
   set(CMAKE_INSTALL_PREFIX $ENV{PREFIX})
else()
   set(CMAKE_INSTALL_PREFIX /usr)
endif()

configure_file(com.deepin.Manual.Search.service.in com.deepin.Manual.Search.service @ONLY)

install(FILES
        com.deepin.Manual.Open.service
        ${CMAKE_CURRENT_BINARY_DIR}/com.deepin.Manual.Search.service
        DESTINATION share/dbus-1/services/)
