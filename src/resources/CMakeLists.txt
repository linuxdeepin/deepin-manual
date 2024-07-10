if(DEFINED ENV{PREFIX})
   set(CMAKE_INSTALL_PREFIX $ENV{PREFIX})
else()
   set(CMAKE_INSTALL_PREFIX /usr)
endif()

install(FILES deepin-manual.desktop
        DESTINATION share/applications/)

install(FILES themes/common/images/deepin-manual.svg
        DESTINATION share/icons/hicolor/scalable/apps/)

install(FILES org.deepin.manual.metainfo.xml DESTINATION share/metainfo)