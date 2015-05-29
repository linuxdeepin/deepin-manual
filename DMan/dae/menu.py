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

from PyQt5.QtCore import QObject, pyqtSlot, pyqtSignal
from PyQt5.Qt import QIcon, QMenu, QAction, QCursor, QPoint, QKeySequence

class MenuItem(QAction):

    onhover = pyqtSignal()
    ontrigger = pyqtSignal(bool)

    def __init__(self, parent, text, icon = '', checkable = False):
        super(MenuItem, self).__init__(QIcon(icon), text, parent)
        if isinstance(icon, bool):
            checkable = icon
            icon = ''
        self.setObjectName('menuItem')
        self.setCheckable(checkable)
        self.hovered.connect(self.onhover)
        self.triggered.connect(self.ontrigger)

    # 设置菜单项文字
    @pyqtSlot(str)
    def setText(self, text):
        super(MenuItem, self).setText(text)

    # 设置菜单项图标
    @pyqtSlot(str)
    def setIcon(self, icon):
        super(MenuItem, self).setIcon(QIcon(icon))

    # 设置菜单项可选性
    @pyqtSlot(bool)
    def setCheckable(self, checkable):
        super(MenuItem, self).setCheckable(checkable)

    # 设置菜单项快捷键
    @pyqtSlot(str)
    def setShortcut(self, shortcut):
        if isinstance(shortcut, str):
            shortcut = QKeySequence(shortcut)
        super(MenuItem, self).setShortcut(shortcut)

    # 触发鼠标经过事件
    # hover

    # 触发鼠标选中事件
    # trigger

    # 切换选中状态
    # toggle

    # 设置可用性
    # setEnabled

    # 设置是否选中
    # setChecked

class Menu(QMenu):

    def __init__(self, parent, title = '', icon = ''):
        super(Menu, self).__init__(title, parent)
        self.setObjectName('menu')
        self.setIcon(QIcon(icon))

    # 设置菜单标题
    @pyqtSlot(str)
    def setTitle(self, text):
        super(Menu, self).setTitle(text)

    # 设置菜单图标
    @pyqtSlot(str)
    def setIcon(self, icon):
        super(Menu, self).setIcon(QIcon(icon))

    # 添加子菜单、菜单项或分割线（默认为分割线）
    @pyqtSlot(str)
    @pyqtSlot(QObject)
    def addItem(self, item = None):
        if isinstance(item, str):
            self.addAction(MenuItem(self.parent(), item))
        elif isinstance(item, QAction):
            self.addAction(item)
        elif isinstance(item, QMenu):
            self.addMenu(item)
        else:
            self.addSeparator()

    # 显示菜单（默认为鼠标位置）
    @pyqtSlot()
    @pyqtSlot(int, int)
    def exec_(self, x = None, y = None):
        if x is None:
            super(Menu, self).exec_(QCursor.pos())
        else:
            super(Menu, self).exec_(QPoint(x, y))
