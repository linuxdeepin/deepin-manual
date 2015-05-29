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

import base64
import codecs
from PyQt5.QtCore import QObject, pyqtSlot

class File(QObject):

	def __init__(self, parent, filename, mode = 'r', encoding = 'utf-8'):
		super(File, self).__init__(parent)
		self.mode = mode
		if 'b' in mode:
			self.file = codecs.open(filename, mode)
		else:
			self.file = codecs.open(filename, mode, encoding)

	# Slots

	# 读取文本
	@pyqtSlot(result = str)
	@pyqtSlot(int, result = str)
	def readText(self, size = -1):
		try:
			data = self.file.read(size)
			if isinstance(data, bytes):
				return data.decode()
			else:
				return data
		except:
			return ''

	# 读取Base64形式数据
	@pyqtSlot(result = str)
	@pyqtSlot(int, result = str)
	def readBase64(self, size = -1):
		try:
			return base64.b64encode(self.file.read(size)).decode()
		except:
			return ''

	# 读取文本行
	@pyqtSlot(result = str)
	@pyqtSlot(int, result = str)
	def readTextLine(self, size = -1):
		try:
			data = self.file.readline(size)
			if isinstance(data, bytes):
				return data.decode()
			else:
				return data
		except:
			return ''

	# 读取行Base64数据
	@pyqtSlot(result = str)
	@pyqtSlot(int, result = str)
	def readBase64Line(self, size = -1):
		try:
			return base64.b64encode(self.file.readline(size)).decode()
		except:
			return ''

	# 写入文本数据
	@pyqtSlot(str, result = bool)
	def writeText(self, data):
		try:
			if 'b' in self.mode:
				self.file.write(data.encode())
			else:
				self.file.write(data)
			return True
		except:
			return False

	# 写入Base64数据
	@pyqtSlot(str, result = bool)
	def writeBase64(self, data):
		try:
			self.file.write(base64.b64decode(data))
			return True
		except:
			return False

	# 移动文件指针位置
	@pyqtSlot(int)
	@pyqtSlot(int, int)
	def seek(self, offset, whence = 0):
		self.file.seek(offset, whence)

	# 获取文件指针位置
	@pyqtSlot(result = int)
	def tell(self):
		return self.file.tell()

	# 关闭文件
	@pyqtSlot()
	def close(self):
		self.file.close()
