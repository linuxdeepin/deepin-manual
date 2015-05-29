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

import os
import sys

from PyQt5.QtCore import QUrl, pyqtSlot
from PyQt5.Qt import QDialog, QApplication, Qt, QPalette
from PyQt5.QtQuickWidgets import QQuickWidget

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.Qt import Qt

from PyQt5.QtCore import pyqtSlot

ROOT_DIR = os.path.dirname(os.path.realpath(__file__))

class QmlWindow(QtWidgets.QWidget):
    def __init__(self, url):
        super(QmlWindow, self).__init__()

        self.setAttribute(Qt.WA_QuitOnClose, True)
        self.setAttribute(Qt.WA_DeleteOnClose, True)
        self.setAttribute(Qt.WA_TranslucentBackground, True)
        #self.setTransBackground(True)

        self.qmlWindow = QQuickWidget(self)
        self.qmlWindow.setResizeMode(QQuickWidget.SizeRootObjectToView)
        self.qmlWindow.rootContext().setContextProperty("qmlWindow", self.qmlWindow)
        self.qmlWindow.setSource(QUrl(url))

        windowFlags = Qt.FramelessWindowHint
        self.setWindowFlags(windowFlags)

        self.show()

    @pyqtSlot(bool)
    def setTransBackground(self, transBackground):
        palette = self.palette()
        if transBackground:
            palette.setBrush(QPalette.Base, Qt.transparent)
        else:
            palette.setBrush(QPalette.Base, Qt.white)
        self.setPalette(palette)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = QmlWindow("file:///data/Projects/deepin/youdao-dict/app/qml/main.qml")
    sys.exit(app.exec_())
