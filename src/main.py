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

from dtk.ui.new_slider import HSlider
from color import color_hex_to_cairo
from button import SelectButton, SelectButtonGroup, ImageButton
from window import Window
from titlebar import TitleBar, home_title_bar, index_title_bar, back 
from webview import ContentWebView
from parse_content import get_home_item_values, get_category_contents

import os
import gtk
import webbrowser
import javascriptcore as jscore


class UserManual(Window):
    def __init__(self):
        Window.__init__(self)

        self._init_values()
        self._init_settings()
        self._init_wedget()
        
        self.connect("destroy", gtk.main_quit)
        self.show_all()
        gtk.main()

    def _init_values(self):
        self.width = 685
        self.height = 500
        self.titlebar_height = 62

    def _init_settings(self):
        self.set_decorated(False)

    def _init_wedget(self):
        self.main_alignment = gtk.Alignment(0.5, 0.5, 0, 0)
        self.main_alignment.set_padding(0, 0, 0, 0)
        main_v_box = gtk.VBox()

        self.slider = HSlider()
        self.slider.set_size_request(-1, 62)
        self.slider.to_page_now(home_title_bar)
        main_v_box.pack_start(self.slider)

        self.web_view = ContentWebView(self.width, self.height - self.titlebar_height)
        self.web_view.connect("load-committed", self.load_finished_cb)
        self.home_html_str = open("../contents/html/home.html").read()
        self.web_view.load(self.home_html_str, "file:///home/iceleaf/Projects/deepin/deepin-user-manual/contents/html/")
        main_v_box.pack_start(self.web_view)

        self.main_alignment.add(main_v_box)
        self.add_widget(self.main_alignment)

        back.connect("button-press-event", self.__page_go_back, self.web_view)

        self.web_view.connect("title-changed", self.title_changed_handler)
        #subject_buttons_group.connect("button-press", lambda w, b: self.print_info(b.subject_index))

    def title_changed_handler(self, widget, webframe, data):
        print data
        data_dict = eval(data)
        if data_dict["data"].startswith("http://") or data.startswith("https://"):
            webbrowser.open(data_dict["data"])
        elif data_dict["type"]=="link" and data_dict["data"].endswith("index.html"):
            self.web_view.index_file = "../contents/html/index.html"
            self.slider.to_page(index_title_bar, None)

    def __page_go_back(self, widget, event, web):
        web.index_file = "../contents/html/home.html"
        self.slider.to_page(home_title_bar, None)
        widget.emit("leave-notify-event", gtk.gdk.Event(gtk.gdk.LEAVE_NOTIFY))

    def load_finished_cb(self, view, frame):
        self.web_ctx = jscore.JSContext(self.web_view.get_main_frame().get_global_context())
        document = self.web_ctx.globalObject.document
        self.web_ctx.globalObject.homeItemValues = get_home_item_values()

    def print_info(self, *data):
        print data

if __name__ == "__main__":
    UserManual()
