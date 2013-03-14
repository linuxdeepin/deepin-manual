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

import gtk
from utils import propagate_expose, get_text_size
from color import color_hex_to_cairo 
from draw import draw_text
from button import ImageButton 

BACKGROUND_COLOR = "#ededed"
SEPERATOR_COLOR_UP = "#a3a3a3"
SEPERATOR_COLOR_DOWN = "#cdcdcd"

class TitleBar(gtk.EventBox):
    def __init__(self):
        gtk.EventBox.__init__(self)
        
        self._init_wedget()
        self.set_size_request(685, 62)
        self.min_button_callback = None

    def _init_wedget(self):
        self.align = gtk.Alignment(1, 1, 1, 1)
        self.align.set_padding(0, 0, 0, 0)

        self.h_box = gtk.HBox()
        self.align.add(self.h_box)
        self.add(self.align)
        
        self.align.connect("expose-event", self.expose)
        
        self.close_button = ImageButton("app_image/window_close_normal.png", "./app_image/window_close_hover.png", "app_image/window_close_press.png")
        close_align = gtk.Alignment(1, 0, 0, 0)
        close_align.set_padding(0, 0, 0, 0)
        close_align.add(self.close_button)

        self.min_button = ImageButton("app_image/window_min_normal.png", "app_image/window_min_hover.png", "app_image/window_min_press.png")
        min_align = gtk.Alignment(1, 0, 0, 0)
        min_align.set_padding(0, 0, 0, 0)
        min_align.add(self.min_button)

        button_h_box = gtk.HBox()
        button_h_box.pack_start(min_align, False, False)
        button_h_box.pack_start(close_align, False, False)

        button_align = gtk.Alignment(1, 0, 0, 0)
        button_align.add(button_h_box)

        self.chapter_button_align = gtk.Alignment(1, 0.5, 0, 0)
        self.chapter_button_align.set_padding(0, 0, 0, 0)

        button_v_box = gtk.VBox()
        button_v_box.pack_start(button_align, False, False)
        button_v_box.pack_start(self.chapter_button_align)

        self.h_box.pack_end(button_v_box, False, False)

        self.center_align = gtk.Alignment(0, 0.5, 0, 0)
        self.h_box.pack_end(self.center_align)

    def expose(self, widget, event):
        cr = widget.window.cairo_create()
        rect = widget.allocation

        # draw backgound
        cr.rectangle(*rect)
        cr.set_source_rgb(*color_hex_to_cairo(BACKGROUND_COLOR))
        cr.fill()

        # draw seperator
        cr.set_source_rgb(*color_hex_to_cairo(SEPERATOR_COLOR_UP))
        cr.rectangle(0, rect.height-2, rect.width, 1)
        cr.fill()

        cr.set_source_rgb(*color_hex_to_cairo(SEPERATOR_COLOR_DOWN))
        cr.rectangle(0, rect.height-1, rect.width, 1)
        cr.fill()

        # 
        propagate_expose(widget, event)
        return True

class TitleLabel(gtk.Label):
    def __init__(self, text, font_size=19, font_color="#373737"):
        gtk.Label.__init__(self)

        self.text = text
        self.font_size = font_size
        self.font_color = font_color
        width, height = get_text_size(self.text, self.font_size)
        self.set_size_request(width, height)
        
        self.connect("expose-event", self.expose)

    def expose(self, widget, event):
        cr = widget.window.cairo_create()
        rect = widget.allocation

        draw_text(cr, self.text, rect.x, rect.y, self.font_size, self.font_color)

icon = ImageButton("app_image/icon.png")
icon_align = gtk.Alignment(0, 0.5, 0, 0)
icon_align.set_padding(0, 0, 13, 10)
icon_align.add(icon)


title = TitleLabel("LinuxDeepin 用户手册")
title_align = gtk.Alignment(0, 0.5, 0, 0)
title_align.set_padding(0, 0, 5, 0)
title_align.add(title)

back = ImageButton("app_image/back.png", "app_image/back-hover.png", "app_image/back-hover.png")
back_align = gtk.Alignment(0, 0.5, 0, 0)
back_align.set_padding(0, 0, 13, 10)
back_align.add(back)


home_title_bar = TitleBar()
home_title_bar.h_box.pack_start(icon_align, False, False)
home_title_bar.center_align.add(title_align)


index_title_bar = TitleBar()
index_title_bar.h_box.pack_start(back_align, False, False)

if __name__ == "__main__":
    win = gtk.Window()
    win.set_decorated(False)
    titlebar = home_title_bar
    v_box = gtk.VBox()
    v_box.pack_start(titlebar)
    win.add(v_box)
    win.set_size_request(685, 62)
    win.connect("destroy", gtk.main_quit)
    win.show_all()
    gtk.main()
