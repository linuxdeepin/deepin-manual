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
import uuid
import platform
from PyQt5.QtCore import QObject, pyqtSlot

class System(QObject):

    # 获取系统类型
    @pyqtSlot(result = str)
    def type(self):
    	return platform.system()

    # 获取系统平台
    @pyqtSlot(result = str)
    def platform(self):
    	return sys.platform

    # 获取系统位数
    @pyqtSlot(result = str)
    def arch(self):
    	return platform.architecture()[0]

    # 获取系统版本
    @pyqtSlot(result = str)
    def release(self):
    	return platform.release()

    # 获取系统版本
    @pyqtSlot(result = str)
    def version(self):
    	return platform.version()

    # 获取机器类型
    @pyqtSlot(result = str)
    def machine(self):
    	return platform.machine()

    # 获取CPU信息
    @pyqtSlot(result = str)
    def processor(self):
    	return platform.processor()

    # 获取当前用户名
    @pyqtSlot(result = str)
    def hostname(self):
    	return platform.node()

    # 获取UUID
    @pyqtSlot(result = str)
    def uuid(self):
    	return str(uuid.uuid1())

    # 获取应用程序参数
    @pyqtSlot(result = str)
    def argvs(self):
    	return '\n'.join(sys.argv)

    # 执行命令并返回输出内容
    @pyqtSlot(str, result = str)
    def execute(self, command):
    	return os.popen(command).read()

    # 执行命令并返回
    @pyqtSlot(str, result = str)
    def system(self, command):
    	return os.system(command)

    # 打开文件
    @pyqtSlot(str, result = bool)
    def run(self, path):
    	try:
    		os.startfile(path)
    		return True
    	except:
    		return False

    # 退出
    @pyqtSlot()
    @pyqtSlot(str)
    def exit(self, message = 0):
    	sys.exit(message)

    # TODO 重启
    @pyqtSlot()
    def reboot(self):
    	os.system('reboot')

    # TODO 关机
    @pyqtSlot()
    def shutdown(self):
    	os.system('halt')
