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
import hashlib
from PyQt5.QtCore import QObject, pyqtSlot

class Codec(QObject):

	# Base64编码
	@pyqtSlot(str, result = str)
	def base64Encode(self, data):
		return str(base64.b64encode(data.encode()), 'utf-8')

	# Base64解码
	@pyqtSlot(str, result = str)
	def base64Decode(self, data):
		return str(base64.b64decode(data.encode()), 'utf-8')

	# md5编码
	@pyqtSlot(str, result = str)
	def md5(self, data):
		return hashlib.md5(data.encode()).hexdigest()

	# sha1编码
	@pyqtSlot(str, result = str)
	def sha1(self, data):
		return hashlib.sha1(data.encode()).hexdigest()

	# sha224编码
	@pyqtSlot(str, result = str)
	def sha224(self, data):
		return hashlib.sha224(data.encode()).hexdigest()

	# sha256编码
	@pyqtSlot(str, result = str)
	def sha256(self, data):
		return hashlib.sha256(data.encode()).hexdigest()

	# sha384编码
	@pyqtSlot(str, result = str)
	def sha384(self, data):
		return hashlib.sha384(data.encode()).hexdigest()

	# sha512编码
	@pyqtSlot(str, result = str)
	def sha512(self, data):
		return hashlib.sha512(data.encode()).hexdigest()
