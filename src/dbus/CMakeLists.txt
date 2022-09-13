# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: CC0-1.0

install(FILES
        com.deepin.Manual.Open.service
        com.deepin.Manual.Search.service
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/services/)
