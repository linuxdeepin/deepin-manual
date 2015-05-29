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

from PyQt5.QtCore import pyqtSlot
from PyQt5.Qt import QFileDialog

class FileDialog(QFileDialog):

    def __init__(self, parent, caption = '', directory = ''):
    	super(FileDialog, self).__init__(parent, caption, directory)

    # 设置对话框标题
    @pyqtSlot(str)
    def setCaption(self, caption):
    	self.setWindowTitle(caption)

    # 设置对话框目录
    @pyqtSlot(str)
    def setDirectory(self, directory):
    	super(FileDialog, self).setDirectory(directory)

    # 设置打开、保存模式
    # AcceptMode {0: AcceptOpen, 1: AcceptSave}
    @pyqtSlot(int)
    def setAcceptMode(self, acceptMode):
    	super(FileDialog, self).setAcceptMode(acceptMode)

    # 设置文件选择模式
    # FileMode {0: AnyFile, 1: ExistingFile, 2: Directory, 3: ExistingFiles, 4: DirectoryOnly}
    @pyqtSlot(int)
    def setFileMode(self, fileMode):
    	super(FileDialog, self).setFileMode(fileMode)

    # 设置默认保存扩展名
    @pyqtSlot(str)
    def setDefaultSuffix(self, defaultSuffix):
    	super(FileDialog, self).setDefaultSuffix(defaultSuffix)

    # 设置默认选中文件
    @pyqtSlot(str)
    def selectFile(self, filename):
    	super(FileDialog, self).selectFile(filename)

    # 选中过滤器
    @pyqtSlot(str)
    def setFilters(self, filters):
    	self.setNameFilter(filters)

    # 添加过滤器
    @pyqtSlot(str)
    def addFilter(self, newFilter):
    	filters = self.nameFilters()
    	filters.append(newFilter)
    	self.setNameFilters(filters)

    # 设置默认过滤器
    @pyqtSlot(str)
    def selectFilter(self, defFilter):
    	self.selectNameFilter(defFilter)

    # 显示文件选择对话框
    @pyqtSlot(result = str)
    def exec_(self):
    	if(super(FileDialog, self).exec_()):
    		return '\n'.join(self.selectedFiles())
    	return ''
