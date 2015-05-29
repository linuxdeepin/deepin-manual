#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2011~2014 Deepin, Inc.
#               2011~2014 Kaisheng Ye
#
# Author:     Kaisheng Ye <kaisheng.ye@gmail.com>
# Maintainer: Kaisheng Ye <kaisheng.ye@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import dae.assets as assets

from PyQt5.Qt import QSettings
from PyQt5.QtNetwork import QNetworkCookie, QNetworkCookieJar

class DataJar(QNetworkCookieJar):
    def __init__(self):
        super(DataJar, self).__init__()
        self.settings = QSettings(assets.fs.dataPath() + '/data.ini', QSettings.IniFormat)
        self.load()

    def load(self):
        strCookies = self.settings.value('cookies')
        if strCookies:
            self.setAllCookies(QNetworkCookie.parseCookies(strCookies))

    def save(self):
        cookies = self.allCookies()
        strCookies = ''
        for cookie in cookies:
            strCookies += cookie.toRawForm() + '\n'
        self.settings.setValue('cookies', strCookies)

    def set(self, name, value):
        self.settings.setValue('Data/' + name, value)

    def get(self, name):
        return self.settings.value('Data/' + name)

    def remove(self, name = None):
        if name is None:
            self.settings.remove('Data')
        else:
            self.settings.remove('Data/' + name)
