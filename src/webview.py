#! /usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2011~2012 Deepin, Inc.
#               2011~2012 Kaisheng Ye
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
import webkit
import javascriptcore as jscore

class ContentWebView(webkit.WebView):
    def __init__(self, width, height):
        webkit.WebView.__init__(self)
        self.__index_file = None
        self.set_size_request(width, height)
        settings = self.get_settings()
        settings.set_property("enable-default-context-menu", False)
        settings.set_property("tab-key-cycles-through-elements", False)
        settings.set_property("default-font-family", "WenQuanYi Micro Hei")
        
    @property
    def index_file(self):
        return self.__index_file

    @index_file.setter
    def index_file(self, new_index_file):
        self.__index_file = new_index_file
        self.base_uri = "file://"+os.path.realpath(os.path.dirname(self.__index_file))+"/"
        self.original_index_string = open(self.__index_file).read()
        self.load(self.original_index_string, self.base_uri)
        #self.reload()

    @index_file.getter
    def index_file(self):
        return self.__index_file
    
    def load(self, string, base_uri):
        self.load_string(string, "text/html", "utf8", base_uri)

