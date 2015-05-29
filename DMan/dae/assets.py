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

fs = None
sys = None
dataJar = None
version = '1.1.0'
windows = []
trayicon = None
pluginobjects = []
manifest = {
        'name': 'Deepin App Engine',
        'version': version,
        'description': '深度桌面应用引擎',
        'path': '',
        'plugins': '',
        'icon': '',
        'width': 800,
        'height': 600,
        'visible': True,
        'resizable': True,
        'frameless': False,
        'debug': True,
        'transBackground': False
}
