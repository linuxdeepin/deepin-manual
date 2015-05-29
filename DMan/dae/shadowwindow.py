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

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.Qt import Qt

from PyQt5.QtCore import pyqtSlot

import dae.xutils as xutils

class ShadowWindow(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super(ShadowWindow, self).__init__(parent)

        self.setAttribute(Qt.WA_QuitOnClose, True)
        self.setAttribute(Qt.WA_DeleteOnClose, True)
        self.setAttribute(Qt.WA_TranslucentBackground, True)

        self.shadowWidth = 0
        self.radius = 0
        self.shadowMargins = None
        self.setShadowWidth(10)
        self.setRadius(5)

        self.verticalLayout = QtWidgets.QVBoxLayout(self)
        self.verticalLayout.setObjectName("verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)

    @pyqtSlot(int)
    def setShadowWidth(self, width):
        self.shadowWidth = width
        self.shadowMargins = QtCore.QMargins(self.shadowWidth, self.shadowWidth, self.shadowWidth, self.shadowWidth)
        self.setContentsMargins(self.shadowMargins)
        _dropShadow = QtWidgets.QGraphicsDropShadowEffect(self)
        _dropShadow.setBlurRadius(self.shadowWidth)
        _dropShadow.setColor(Qt.black)
        _dropShadow.setOffset(0.0)
        self.setGraphicsEffect(_dropShadow)
        self.update(self.rect())

        xutils.show_shadow(xutils.get_xwindow(int(self.winId())), str(width))

    @pyqtSlot(result = int)
    def getShadowWidth(self):
        return self.shadowWidth

    @pyqtSlot(int)
    def setRadius(self, radius):
        self.radius = radius
        self.update(self.rect())

    @pyqtSlot(result = int)
    def getRadius(self):
        return self.radius

    def paintEvent(self, event):
        rect = self.rect()
        if self.shadowMargins:
            rect = rect.marginsRemoved(self.shadowMargins)
        rect = QtCore.QRectF(rect)

        painter = QtGui.QPainter(self)
        painter.setRenderHint(QtGui.QPainter.Antialiasing)

        border = QtGui.QPainterPath()
        border.addRoundedRect(rect, self.radius, self.radius)
        painter.fillPath(border, Qt.white)

if __name__ == "__main__":
    import  sys

    app = QtWidgets.QApplication(sys.argv)
    main = ShadowWindow()
    main.show()
    sys.exit(app.exec_())
