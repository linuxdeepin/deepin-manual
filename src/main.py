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
from titlebar import  home_title_bar, index_title_bar, back 
from webview import ContentWebView
from parse_content import (get_home_item_values, 
                            get_category_contents,
                            get_category_pages_id,
                            get_category_unread_pages)

import os
import gtk
import webbrowser
import json

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
        self.html_base_url = "file://" + os.path.realpath("../contents/html/")+"/"
        self.home_html_str = open(os.path.realpath("../contents/html/home.html")).read()
        self.index_html_str = open(os.path.realpath("../contents/html/index.html")).read()
        self.home_values = get_home_item_values()
        self.init_progress_data()

    def _init_settings(self):
        self.set_decorated(False)

    def _init_wedget(self):
        self.main_alignment = gtk.Alignment(0.5, 0.5, 0, 0)
        self.main_alignment.set_padding(0, 0, 0, 0)
        main_v_box = gtk.VBox()

        self.slider = HSlider(500)
        self.slider.set_size_request(self.width, self.titlebar_height)
        self.slider.to_page_now(home_title_bar)
        main_v_box.pack_start(self.slider)
        
        self.web_view = ContentWebView(self.width, self.height - self.titlebar_height)
        self.web_view.connect("load-committed", self.load_finished_cb)

        # init velues for home
        self.push_data_to_web_view(self.home_html_str, self.home_values)
        main_v_box.pack_start(self.web_view)
        
        self.main_alignment.add(main_v_box)
        self.add_widget(self.main_alignment)
        
        back.connect("button-release-event", self.page_go_back, self.web_view)
        home_title_bar.min_button.connect("clicked", lambda w: self.iconify())
        index_title_bar.min_button.connect("clicked", lambda w: self.iconify())
        self.add_move_event(home_title_bar)
        self.add_move_event(index_title_bar)
        self.web_view.connect("title-changed", self.title_changed_handler)
        #subject_buttons_group.connect("button-press", lambda w, b: self.print_info(b.subject_index))
        
    def title_changed_handler(self, widget, webframe, data):
        print "data from web: %s" % data
        data_dict = eval(data)
        if data_dict["type"]=="external_link":
            webbrowser.open(data_dict["data"])
        elif data_dict["type"]=="home_item_link":
            category = data_dict["data"]
            category_contents = get_category_contents(category)
            subject_index = 0
            page_id = "A1"
            self.push_data_to_web_view(self.index_html_str, category_contents, subject_index, page_id)
            self.remove_read_page(category, subject_index, page_id)
            group = self.get_subject_button_group(category_contents, subject_index)
            center_align_child = index_title_bar.center_align.get_child()
            if center_align_child:
                index_title_bar.center_align.remove(center_align_child)
            index_title_bar.center_align.add(group)
            self.slider.to_page(index_title_bar, "right")
        elif data_dict["type"] == "slider_change":
            pass

    def get_subject_button_group(self, category_contents, active_index=0):
        subjects = category_contents["content"]
        subject_buttons = []
        for i in range(len(subjects)):
            subject_buttons.append(SelectButton(i, subjects[i].get("title"))) 
        subject_buttons_group = SelectButtonGroup(subject_buttons)
        subject_buttons[active_index].selected = True
        subject_buttons_group.connect("button-press", self.subject_button_press, category_contents)
        return subject_buttons_group

    def push_data_to_web_view(self, html_string, *data):
        self.load_data = []
        for d in data:
            self.load_data.append(d)
        self.web_view.load(html_string, self.html_base_url)

    def subject_button_press(self, group, active_button, category_contents):
        self.push_data_to_web_view(self.index_html_str, category_contents, active_button.subject_index)

    def page_go_back(self, widget, event, web):
        self.fresh_read_percent()
        self.push_data_to_web_view(self.home_html_str, self.home_values)
        self.slider.to_page(home_title_bar, "left")

    def load_finished_cb(self, view, frame):
        self.web_view.execute_script("var Values=%s" % json.dumps(self.load_data, encoding="UTF-8", ensure_ascii=False))

    def init_progress_data(self):
        for item in self.home_values:
            all_pages = get_category_pages_id(item["category"])
            item["all_pages"] = all_pages
            unread_pages = get_category_unread_pages(item["category"])
            item["unread_pages"] = unread_pages
        self.fresh_read_percent()

    def fresh_read_percent(self):
        for item in self.home_values:
            unread = item["unread_pages"]
            all_pages = item["all_pages"]
            percent = 1 - float(len(unread))/len(all_pages)
            item["percent"] = "%.2f" % percent

    def remove_read_page(self, category, subject_index, page_id):
        for item in self.home_values:
            if item["category"] == category:
                for page in item["unread_pages"]:
                    if page == (subject_index, page_id):
                        item["unread_pages"].remove((subject_index, page_id))
                return

    def print_info(self, *data):
        print data

if __name__ == "__main__":
    UserManual()
