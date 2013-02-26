#! /usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2012 Deepin, Inc.
#               2012 Kaisheng Ye
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

import cairo
import gtk
from color import alpha_color_hex_to_cairo, color_hex_to_cairo
from utils import new_surface, propagate_expose
from utils import cairo_popover, cairo_popover_rectangle 
from dtk_cairo_blur import gaussian_blur

SAHOW_VALUE = 2 

DRAW_WIN_TYPE_BG = "bg"
DRAW_WIN_TYPE_FG = "fg"

class Window(gtk.Window):
    def __init__(self, type=gtk.WINDOW_TOPLEVEL):
        gtk.Window.__init__(self, type)
        self.__init_values()
        self.__init_settings()
        self.__init_widgets()
        self.__init_events()

    def __init_values(self):
        self.draw_rectangle_bool = True
        self.surface = None
        self.old_w = 0
        self.old_h = 0
        self.old_offset = 0
        self.trayicon_x = SAHOW_VALUE * 2  
        self.trayicon_y = SAHOW_VALUE * 2
        self.trayicon_border = 3
        self.radius = 0 
        self.ali_left = 8 
        self.ali_right = 8
        self.ali_top  = 8
        self.ali_bottom = 7
        # pixbuf.
        self.draw_win_type = DRAW_WIN_TYPE_FG
        self.bg_pixbuf = None
        self.bg_alpha = 1.0
        self.bg_x, self.bg_y = 0,0
        self.fg_alpha = 0.8
        # colors.
        self.sahow_color = ("#000000", 0.1)
        self.border_out_color = ("#cacaca", 1.0)

    def __init_settings(self):
        self.set_colormap(gtk.gdk.Screen().get_rgba_colormap())
        self.set_decorated(False)
        self.set_app_paintable(True)
        #
        
    def __init_widgets(self):
        self.__draw = gtk.EventBox()
        self.main_ali  = gtk.Alignment(0.5, 0.5, 0, 0)
        # set main_ali padding size.
        self.main_ali.set_padding(self.ali_top,
                                  self.ali_bottom,
                                  self.ali_left,
                                  self.ali_right)
        self.__draw.add(self.main_ali)
        self.add(self.__draw)

    def __init_events(self):
        self.add_events(gtk.gdk.ALL_EVENTS_MASK)
        self.connect("size-allocate", self.__on_size_allocate)
        self.__draw.connect("expose-event", self.__draw_expose_event)
        self.connect("destroy", lambda w : gtk.main_quit())

    def __draw_expose_event(self, widget, event):
        cr = widget.window.cairo_create()
        rect = widget.allocation
        #
        cr.rectangle(*rect)
        cr.set_source_rgba(1, 1, 1, 0.0)
        cr.set_operator(cairo.OPERATOR_SOURCE)
        cr.paint()

        cr = widget.window.cairo_create()
        x, y, w, h = rect
        # draw bg type background.
        if self.draw_win_type == DRAW_WIN_TYPE_BG:
            self.draw_background(cr, rect)
        #
        self.__expose_event_draw(cr)
        # draw fg type background.
        if self.draw_win_type == DRAW_WIN_TYPE_FG:
            self.draw_background(cr, rect)
        #
        propagate_expose(widget, event)    
        return True

    def draw_background(self, cr, rect):
        x, y, w, h = rect
        cr.save()
        cairo_popover_rectangle(self, cr, 
                      self.trayicon_x + self.trayicon_border + 1, 
                      self.trayicon_y + self.trayicon_border + 1, 
                      w, h + 1, 
                      self.radius) 
        cr.clip()
        if self.bg_pixbuf:
            cr.set_source_pixbuf(self.bg_pixbuf, self.bg_x, self.bg_y)
            cr.paint_with_alpha(self.bg_alpha)
        else:
            cr.set_source_rgb(1, 1, 1)
            cr.rectangle(x, y, w, h)
            cr.fill()
        cr.restore()

    def __on_size_allocate(self, widget, alloc):
        x, y, w, h = self.allocation
        # !! no expose and blur.
        if ((self.old_w == w and self.old_h == h)):
            return False
        # 
        self.surface, self.surface_cr = new_surface(w, h)
        self.__compute_shadow(w, h)
        self.old_w = w
        self.old_h = h

    def __compute_shadow(self, w, h):
        # sahow.
        cairo_popover_rectangle(self, self.surface_cr, 
                      self.trayicon_x, self.trayicon_y, 
                      w, h,
                      self.radius)
        self.surface_cr.set_source_rgba( # set sahow color.
                *alpha_color_hex_to_cairo((self.sahow_color)))
        self.surface_cr.fill_preserve()
        gaussian_blur(self.surface, SAHOW_VALUE)
        # border.
        if self.draw_rectangle_bool:
            # out border.
            self.surface_cr.clip()
            cairo_popover_rectangle(self, self.surface_cr, 
                          self.trayicon_x + self.trayicon_border, 
                          self.trayicon_y + self.trayicon_border, 
                          w, h + 1, 
                          self.radius) 
            self.surface_cr.set_source_rgba( # set out border color.
                    *alpha_color_hex_to_cairo(self.border_out_color))
            self.surface_cr.set_line_width(self.border_width)
            self.surface_cr.fill()
            self.draw_in_border(w, h)

    def draw_in_border(self, w, h):
        # in border.
        self.surface_cr.reset_clip()
        cairo_popover_rectangle(self, self.surface_cr, 
                      self.trayicon_x + self.trayicon_border + 1, 
                      self.trayicon_y + self.trayicon_border + 1, 
                      w, h + 1, 
                      self.radius) 
        self.surface_cr.set_source_rgba(1, 1, 1, 1.0) # set in border color.
        self.surface_cr.set_line_width(self.border_width)
        self.surface_cr.fill()

    def __expose_event_draw(self, cr):
        if self.surface:
            cr.set_source_surface(self.surface, 0, 0)
            cr.paint_with_alpha(self.fg_alpha)

    def set_bg_pixbuf(self, pixbuf, x=0, y=0, alpha=1.0):
        self.bg_pixbuf = pixbuf
        self.bg_x = x
        self.bg_y = y
        self.bg_alpha = alpha
        self.queue_draw()

    def set_draw_win_type(self, type=DRAW_WIN_TYPE_FG):
        self.draw_win_type = type
        self.queue_draw()

    def add_widget(self, widget):
        self.main_ali.add(widget)


if __name__ == "__main__":
    test = Window()
    test.resize(372, 168)
    test.show_all()
    gtk.main()
