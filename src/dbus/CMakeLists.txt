if(DEFINED ENV{PREFIX})
   set(CMAKE_INSTALL_PREFIX $ENV{PREFIX})
else()
   set(CMAKE_INSTALL_PREFIX /usr)
endif()
install(FILES
        com.deepin.Manual.Open.service
        com.deepin.Manual.Search.service
        DESTINATION share/dbus-1/services/)
