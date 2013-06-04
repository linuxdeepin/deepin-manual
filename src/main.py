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

from constant import (APP_IMAGE_PATH, 
                      CONTENTS_PATH, 
                      DEEPIN_USER_MANUAL_NAME,
                      DEEPIN_USER_MANUAL_PATH,
                      LANGUAGE,
                      )

from button import SelectButton, SelectButtonGroup
from window import Window
from titlebar import  home_title_bar, index_title_bar, back, TitleLabel
from webview import ContentWebView
from parse_content import (get_home_item_values, 
                            get_book_contents,
                            get_book_pages_id,
                            get_book_unread_pages,
                            write_unread_pages_data
                            )

import os
import gtk
import webbrowser
import json

import dbus
import dbus.service
from dbus.mainloop.glib import DBusGMainLoop
from deepin_utils.ipc import is_dbus_name_exists

from nls import _

class UserManual(dbus.service.Object):
    def __init__(self, session_bus):
        dbus.service.Object.__init__(self, session_bus, DEEPIN_USER_MANUAL_PATH)

        self.window = Window()
        self._init_values()
        self._init_settings()
        self._init_wedget()
        
        self.window.connect("destroy", gtk.main_quit)
        self.window.set_position(gtk.WIN_POS_CENTER)
        self.window.show_all()
        gtk.main()

    def _init_values(self):
        self.width = 685
        self.height = 500
        self.titlebar_height = 62
        self.book_name_label_color = "#6fb8ef"
        self.html_base_url = "file://" + os.path.join(CONTENTS_PATH, "html") + "/"
        self.home_html_str = open(os.path.join(CONTENTS_PATH, "html", "home.html")).read()
        self.index_html_str = open(os.path.join(CONTENTS_PATH, "html", "index.html")).read()
        self.home_values = get_home_item_values()
        self.init_progress_data()

    def _init_settings(self):
        self.window.set_decorated(False)
        self.window.set_icon_from_file(os.path.join(APP_IMAGE_PATH, "deepin-user-manual.png"))
        self.window.set_title(_("LinuxDeepin User Manual"))

    def _init_wedget(self):
        main_v_box = gtk.VBox()

        self.title_align = gtk.Alignment(0, 0.5, 1, 1)
        self.title_align.set_padding(0, 0, 0, 0)
        self.title_align.add(home_title_bar)
        
        self.web_view = ContentWebView(self.width, self.height - self.titlebar_height)
        #self.web_view.enable_inspector()
        self.web_view.connect("load-committed", self.load_committed_cb)
        self.web_view.connect("load-finished", self.load_finished_cb)

        # init velues for home
        self.push_data_to_web_view(self.home_html_str, self.home_values_to_list())

        main_v_box.pack_start(self.title_align)
        main_v_box.pack_start(self.web_view)
        
        self.main_alignment = gtk.Alignment(0.5, 0.5, 0, 0)
        self.main_alignment.set_padding(0, 0, 0, 0)
        self.main_alignment.add(main_v_box)
        self.window.add_widget(self.main_alignment)
        
        back.connect("button-release-event", self.page_go_back)
        home_title_bar.min_button.connect("clicked", lambda w: self.window.iconify())
        index_title_bar.min_button.connect("clicked", lambda w: self.window.iconify())
        home_title_bar.close_button.connect("clicked", self.close_window)
        index_title_bar.close_button.connect("clicked", self.close_window)
        self.window.add_move_event(home_title_bar)
        self.window.add_move_event(index_title_bar)
        self.web_view.connect("title-changed", self.title_changed_handler)

    def close_window(self, widget):
        write_unread_pages_data(self.home_values)
        gtk.main_quit()
        
    def title_changed_handler(self, widget, webframe, data):
        data_dict = eval(data)

        if data_dict["type"]=="external_link":
            webbrowser.open(data_dict["data"])

        elif data_dict["type"]=="home_item_link":
            book = data_dict["data"]
            self.home_item_click_handler(book)

        elif data_dict["type"] == "after_slider_change":
            page_info = eval(data_dict["data"])
            book, chapter_index, page_id = page_info["book"], page_info["chapter_index"], page_info["page_id"]
            self.remove_read_page(book, chapter_index, page_id)
            if chapter_index == 0 and self.home_values[book]["all_pages"][chapter_index][0] == page_id:
                self.web_view.execute_script('change_nav_status("Left", "none")')
                self.web_view.execute_script('change_nav_status("Right", "block")')
                self.web_view.execute_script('$("#msg").css("display", "none")')
            elif chapter_index == len(self.home_values[book]["all_pages"])-1 and page_id == self.home_values[book]["all_pages"][chapter_index][-1]:
                self.web_view.execute_script('change_nav_status("Left", "block")')
            else:
                self.web_view.execute_script('change_nav_status("Left", "block")')
                self.web_view.execute_script('change_nav_status("Right", "block")')
                self.web_view.execute_script('$("#msg").css("display", "none")')
                
        elif data_dict["type"] == "before_slider_change":
            page_info = eval(data_dict["data"])
            book, chapter_index, page_id = page_info["book"], page_info["chapter_index"], page_info["page_id"]
            self.remove_read_page(book, chapter_index, page_id)
            self.web_view.execute_script("var all_pages=%s" % json.dumps(self.home_values[book]["all_pages"], encoding="UTF-8", ensure_ascii=False))
            if chapter_index == len(self.home_values[book]["all_pages"])-1 and page_id == self.home_values[book]["all_pages"][chapter_index][-1]:
                self.web_view.execute_script('document.getElementById("return").innerHTML = %s' % json.dumps(_("Read Again"), encoding="UTF-8", ensure_ascii=False))
                self.web_view.execute_script('document.getElementById("next").innerHTML = %s' % json.dumps(_("Next Chapter"), encoding="UTF-8", ensure_ascii=False))
                self.web_view.execute_script('$("#msg").css("display", "block")')
                self.web_view.execute_script('change_nav_status("Right", "none")')
            if book == "7":
                self.web_view.execute_script('document.getElementById("next").innerHTML = %s' % json.dumps(_("HomePage"), encoding="UTF-8", ensure_ascii=False))
            
        elif data_dict["type"] == "redirect_next_chapter":
            page_info = eval(data_dict["data"])
            book, chapter_index, page_id = page_info["book"], page_info["chapter_index"], page_info["page_id"]
            self.emit_group_button_press(self.chapter_group, book, chapter_index, page_id)

        elif data_dict["type"] == "msg_link":
            button, book = data_dict["data"]
            if button == "next":
                if book == "7":
                    self.page_go_back(None, None)
                    return
                else:
                    book = str(int(book)+1)
            self.home_item_click_handler(book)
            
    def home_item_click_handler(self, book):
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
        # book name label
        if LANGUAGE == "en_US":
            book_name_label = TitleLabel(self.home_values[book]["title"].replace("<br/>", " "), font_size=13, font_color=self.book_name_label_color)
        else:
            book_name_label = TitleLabel(self.home_values[book]["title"], font_size=15, font_color=self.book_name_label_color)

        center_align_child = index_title_bar.center_align.get_child()
        if index_title_bar.center_align.get_child():
            index_title_bar.center_align.remove(center_align_child)
        index_title_bar.center_align.add(book_name_label)
        # chapter button
        self.chapter_group = self.get_chapter_button_group(book, book_contents, chapter_index)
        chapter_button_align_child = index_title_bar.chapter_button_align.get_child()
        if chapter_button_align_child:
            index_title_bar.chapter_button_align.remove(chapter_button_align_child)
        if len(self.home_values[book]["all_pages"]) > 1:
            index_title_bar.chapter_button_align.add(self.chapter_group)
        title_align_child = self.title_align.get_child()
        if title_align_child:
            self.title_align.remove(title_align_child)
        back.set_state(gtk.STATE_NORMAL)
        self.title_align.add(index_title_bar)
        self.window.show_all()

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
        chapter_index = active_button.chapter_index
        page_id = self.home_values[book]["all_pages"][chapter_index][0]
        self.push_data_to_web_view(
                self.index_html_str, 
                book_contents, # Values[0]
                book, # Values[1]
                chapter_index, # Values[2]
                page_id, # Values[3]
                self.home_values[book]["unread_pages"][chapter_index]) # Values[4]

    def page_go_back(self, widget, event):
        self.fresh_read_percent()
        self.push_data_to_web_view(self.home_html_str, self.home_values_to_list())
        title_align_child = self.title_align.get_child()
        if title_align_child:
            self.title_align.remove(title_align_child)
        self.title_align.add(home_title_bar)
        back.set_state(gtk.STATE_NORMAL)
        self.window.show_all()

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
                self.web_view.execute_script('$("#msg").css("display", "none")')

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

    @dbus.service.method(DEEPIN_USER_MANUAL_NAME, in_signature="", out_signature="")    
    def hello(self):
        self.window.present()

if __name__ == "__main__":
    DBusGMainLoop(set_as_default=True)
    session_bus = dbus.SessionBus()
    
    if is_dbus_name_exists(DEEPIN_USER_MANUAL_NAME, True):
        print "deepin user manual has running!"
        
        bus_object = session_bus.get_object(DEEPIN_USER_MANUAL_NAME,
                                            DEEPIN_USER_MANUAL_PATH)
        bus_interface = dbus.Interface(bus_object, DEEPIN_USER_MANUAL_NAME)
        bus_interface.hello()
        
    else:
        # Init dbus.
        bus_name = dbus.service.BusName(DEEPIN_USER_MANUAL_NAME, session_bus)
            
        try:
            user_manual = UserManual(session_bus)
        except KeyboardInterrupt:
            pass
