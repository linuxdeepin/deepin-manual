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

import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

import sys
import os
import json
import codecs

from PyQt5.Qt import QApplication
# from PyQt5.QtCore import QTranslator, QLocale, QLibraryInfo

import dae.assets as assets
# from dae.codec import Codec
from dae.window import Window
# from dae.system import System
# from dae.datajar import DataJar
# from dae.filesystem import FileSystem
from dae.plugins import PluginsManager

class DAEClient:
    def __init__(self, app_dir):

        app_dir = os.path.realpath(app_dir)
        manifest_path = os.path.join(app_dir, "manifest.json")

        try:
            manifest = json.load(codecs.open(manifest_path, 'r', 'utf-8'))
        except:
            manifest = {}

        for key in manifest:
            assets.manifest[key] = manifest[key]

        app = QApplication(sys.argv)
        app.setApplicationName(assets.manifest['name'])
        app.setApplicationVersion(assets.manifest['version'])

        index_path = os.path.join(app_dir, assets.manifest['path'] + 'index.html')
        self.window = Window(None, index_path)
        self.window.setResizerSize(5)
        self.window.setTransBackground(True)
        self.window.setFrameless(True)
        self.window.setClosable(True)

        # assets.sys = System()
        # assets.codec = Codec()
        # assets.fs = FileSystem()
        # assets.dataJar = DataJar()

        # translator = QTranslator()
        # translateLocaleName = "qt_" + QLocale.system().name()
        # translator.load(translateLocaleName, QLibraryInfo.location(QLibraryInfo.TranslationsPath))
        # app.installTranslator(translator)

        if assets.manifest.get("plugins"):
            plugindir = os.path.join(app_dir, assets.manifest["plugins"])
            plugin_manager = PluginsManager(plugindir)
            plugin_manager.load(assets)

        assets.manifest['icon'] = os.path.join(app_dir, assets.manifest["icon"])
        assets.manifest['app_dir'] = app_dir
        if "--debug" in sys.argv:
            assets.manifest['debug'] = True

        sys.exit(app.exec_())

if __name__ == '__main__':
    if os.path.isdir(sys.argv[-1]):
        DAEClient(sys.argv[-1])
