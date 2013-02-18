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

import gtk
import cairo
from dtk.ui.application import Application
from dtk.ui.new_slider import HSlider
from dtk.ui.utils import color_hex_to_cairo
from dtk.ui.box import EventBox, ImageBox, BackgroundBox

MAIN_TITLE_TEXT = "LinuxDeepin 用户手册"

class UserManual(gtk.Window):
    def __init__(self):
        gtk.Window.__init__(self)
        self.set_size_request(685, 500)
        self.set_decorated(False)
        
        self.main_title_bar = MainTitleBar()

        self.icon_image_box = ImageBox(app_theme.get_pixbuf("logo.png"))
        self.icon_align = gtk.Alignment()
        self.icon_align.set(0.5, 0.5, 0.0, 0.0)
        self.icon_align.set_padding(5, 5, 5, 0)
        self.icon_align.add(self.icon_image_box)
        h_layout_box = gtk.HBox()
        h_layout_box.pack_start(self.icon_align, False, False)
        self.main_title_bar.v_layout_box.pack_start(h_layout_box)
        
        v_layout_box = gtk.VBox()
        v_layout_box.pack_start(self.main_title_bar, False, False)
        self.add(v_layout_box)

        self.connect("destroy", gtk.main_quit)
        self.connect("expose-event", self.expose)
        self.show_all()
        gtk.main()

    def expose(self, widget, event):
        cr = widget.window.cairo_create()
        rect = widget.allocation
        print rect

        # draw background
        cr.set_source_rgb(247, 247, 247)
        cr.rectangle(rect.x, rect.y, rect.width, rect.height)
        cr.fill()
    
class MainTitleBar(EventBox):
    def __init__(self, height=62):
        EventBox.__init__(self)
        self.set_size_request(-1, height)
        self.v_layout_box = gtk.VBox()
        self.add(self.v_layout_box)

        #self.connect_after("expose-event", self.expose)

    def expose(self, widget, event):
        cr = widget.window.cairo_create()
        rect = widget.allocation
        print rect

        # draw background
        cr.set_source_rgba(247, 247, 247, 0.9)
        cr.rectangle(rect.x, rect.y, rect.width, rect.height)
        cr.fill()

        cr.set_source_rgb(0, 0, 0)
        cr.select_font_face("WenQuanYi Micro Hei", cairo.FONT_SLANT_NORMAL, 
            cairo.FONT_WEIGHT_NORMAL)
        cr.set_font_size(30)

        (x, y, width, height, dx, dy) = cr.text_extents(MAIN_TITLE_TEXT)
        cr.move_to(70, 42)
        cr.show_text(MAIN_TITLE_TEXT)
        
        return True

if __name__ == "__main__":
    UserManual()
