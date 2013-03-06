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

from dtk.ui.init_skin import init_skin
from deepin_utils.file import get_parent_dir
import os
app_theme = init_skin(
    "deepin-user-manual", 
    "1.0",
    "01",
    os.path.join(get_parent_dir(__file__, 2), "skin"),
    os.path.join(get_parent_dir(__file__, 2), "app_theme"),
    )

from dtk.ui.new_slider import HSlider
from color import color_hex_to_cairo
from button import SelectButton, SelectButtonGroup, ImageButton
from window import Window
from titlebar import  home_title_bar, index_title_bar, back 
from webview import ContentWebView
from parse_content import (get_home_item_values, 
                            get_book_contents,
                            get_book_pages_id,
                            get_book_unread_pages,
                            get_last_page,
                            write_last_page,
                            write_unread_pages_data
                            )

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
        self.set_position(gtk.WIN_POS_CENTER)
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
        self.last_page = get_last_page()

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
        self.web_view.enable_inspector()
        self.web_view.connect("load-committed", self.load_committed_cb)
        self.web_view.connect("load-finished", self.load_finished_cb)

        # init velues for home
        self.push_data_to_web_view(self.home_html_str, self.home_values_to_list())
        main_v_box.pack_start(self.web_view)
        
        self.main_alignment.add(main_v_box)
        self.add_widget(self.main_alignment)
        
        back.connect("button-release-event", self.page_go_back, self.web_view)
        home_title_bar.min_button.connect("clicked", lambda w: self.iconify())
        index_title_bar.min_button.connect("clicked", lambda w: self.iconify())
        home_title_bar.close_button.connect("clicked", self.close_window)
        index_title_bar.close_button.connect("clicked", self.close_window)
        self.add_move_event(home_title_bar)
        self.add_move_event(index_title_bar)
        self.web_view.connect("title-changed", self.title_changed_handler)
        #chapter_buttons_group.connect("button-press", lambda w, b: self.print_info(b.chapter_index))

    def close_window(self, widget):
        write_last_page(self.last_page)
        write_unread_pages_data(self.home_values)
        gtk.main_quit()
        
    def title_changed_handler(self, widget, webframe, data):
        data_dict = eval(data)
        if data_dict["type"]=="external_link":
            webbrowser.open(data_dict["data"])
        elif data_dict["type"]=="home_item_link":
            book = data_dict["data"]
            book_contents = get_book_contents(book)
            chapter_index = 0
            page_id = self.home_values[book]["all_pages"][chapter_index][0]
            self.push_data_to_web_view(
                    self.index_html_str, 
                    book_contents,  # Values[0]
                    book, # Values[1]
                    chapter_index, # Values[2]
                    page_id, # Values[3]
                    self.home_values[book]["unread_pages"][chapter_index]) # Values[4]
            self.chapter_group = self.get_chapter_button_group(book, book_contents, chapter_index)
            center_align_child = index_title_bar.center_align.get_child()
            if center_align_child:
                index_title_bar.center_align.remove(center_align_child)
            index_title_bar.center_align.add(self.chapter_group)
            self.slider.to_page(index_title_bar, "right")
        elif data_dict["type"] == "after_slider_change":
            page_info = eval(data_dict["data"])
            book, chapter_index, page_id = page_info["book"], page_info["chapter_index"], page_info["page_id"]
            if chapter_index == 0 and self.home_values[book]["all_pages"][chapter_index][0] == page_id[1:]:
                self.web_view.execute_script('change_nav_status("Left", "none")')
            elif chapter_index == len(self.home_values[book]["all_pages"])-1 and page_id[1:] == self.home_values[book]["all_pages"][chapter_index][-1]:
                self.web_view.execute_script('change_nav_status("Right", "none")')
            else:
                self.web_view.execute_script('change_nav_status("Left", "block")')
                self.web_view.execute_script('change_nav_status("Right", "block")')
                
        elif data_dict["type"] == "before_slider_change":
            page_info = eval(data_dict["data"])
            book, chapter_index, page_id = page_info["book"], page_info["chapter_index"], page_info["page_id"]
            self.remove_read_page(book, chapter_index, page_id)
            self.web_view.execute_script("var all_pages=%s" % json.dumps(self.home_values[book]["all_pages"], encoding="UTF-8", ensure_ascii=False))
        elif data_dict["type"] == "redirect_next_chapter":
            page_info = eval(data_dict["data"])
            book, chapter_index, page_id = page_info["book"], page_info["chapter_index"], page_info["page_id"]
            self.emit_group_button_press(self.chapter_group, book, chapter_index, page_id)
            
    def emit_group_button_press(self, button_group, book, chapter_index, page_id):
        buttons = button_group.buttons
        active_button = buttons[chapter_index]
        if button_group.current_active != active_button:
            button_group.current_active = active_button
        for button in buttons:
            button.selected = (button == active_button)
        book_contents = get_book_contents(book)
        self.push_data_to_web_view(
                self.index_html_str,
                book_contents, # Values[0]
                book, # Values[1]
                chapter_index, # Values[2]
                page_id, # Values[3]
                self.home_values[book]["unread_pages"][chapter_index]) # Values[4]

    def get_chapter_button_group(self, book, book_contents, active_index):
        chapters = book_contents["content"]
        chapter_buttons = []
        for i in range(len(chapters)):
            chapter_buttons.append(SelectButton(i, chapters[i].get("title")))
        chapter_buttons_group = SelectButtonGroup(chapter_buttons)
        chapter_buttons[int(active_index)].selected = True
        chapter_buttons_group.connect("button-press", self.chapter_button_press, book, book_contents)
        return chapter_buttons_group

    def push_data_to_web_view(self, html_string, *data):
        self.load_data = []
        for d in data:
            self.load_data.append(d)
        self.web_view.load(html_string, self.html_base_url)

    def chapter_button_press(self, group, active_button, book, book_contents):
        self.last_page[book][0] = active_button.chapter_index
        self.last_page[book][1] = book_contents["content"][active_button.chapter_index]["page"][0]["id"]
        chapter_index = active_button.chapter_index
        page_id = self.home_values[book]["all_pages"][chapter_index][0]
        self.push_data_to_web_view(
                self.index_html_str, 
                book_contents, # Values[0]
                book, # Values[1]
                chapter_index, # Values[2]
                page_id, # Values[3]
                self.home_values[book]["unread_pages"][chapter_index]) # Values[4]

    def page_go_back(self, widget, event, web):
        self.fresh_read_percent()
        self.push_data_to_web_view(self.home_html_str, self.home_values_to_list())
        self.slider.to_page(home_title_bar, "left")

    def load_committed_cb(self, view, frame):
        self.web_view.execute_script("var Values=%s" % json.dumps(self.load_data, encoding="UTF-8", ensure_ascii=False))

    def load_finished_cb(self, view, frame):
        if len(self.load_data) > 1:
            book = self.load_data[1]
            chapter_index = self.load_data[2]
            page_id = self.load_data[3]
            self.remove_read_page(book, chapter_index, page_id)
            self.web_view.execute_script("load_page();")
            if chapter_index == 0:
                self.web_view.execute_script('change_nav_status("Left", "none")')

    def init_progress_data(self):
        for book in self.home_values:
            all_pages = get_book_pages_id(book)
            self.home_values[book]["all_pages"] = all_pages
            unread_pages = get_book_unread_pages(self.home_values[book]["book"])
            self.home_values[book]["unread_pages"] = unread_pages
        self.fresh_read_percent()

    def fresh_read_percent(self):
        for key in self.home_values:
            unread_pages = self.home_values[key]["unread_pages"]
            all_pages = self.home_values[key]["all_pages"]
            unread_length = 0
            all_length = 0
            for i in range(len(all_pages)):
                unread_length += len(unread_pages[i])
                all_length += len(all_pages[i])
            percent = 1 - float(unread_length)/all_length
            self.home_values[key]["percent"] = "%.2f" % percent

    def remove_read_page(self, book, chapter_index, page_id):
        if page_id in self.home_values[book]["unread_pages"][chapter_index]:
            self.home_values[book]["unread_pages"][chapter_index].remove(page_id)
        
    def home_values_to_list(self):
        data = []
        for key in self.home_values:
            data.append(self.home_values[key])
        return data

if __name__ == "__main__":
    UserManual()
