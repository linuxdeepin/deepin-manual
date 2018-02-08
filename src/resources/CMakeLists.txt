install(FILES deepin-manual.desktop
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/)

install(FILES themes/common/images/deepin-manual.svg
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps/)

install(FILES libdxcb
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-manual)