#!/usr/bin/python
# -*- coding: utf-8 -*-

from draw import draw_text, draw_pixbuf
from utils import get_text_size, cairo_disable_antialias, propagate_expose
from color import color_hex_to_cairo
import gtk
import gobject
import threading

class SelectButton(gtk.Button):        
    def __init__(self,
                 chapter_index,
                 text,
                 ali_padding=13,
                 font_size=10):
        gtk.Button.__init__(self)
        # init values.
        self.chapter_index = chapter_index
        self.text = text
        self.font_size=font_size 
        self.ali_padding = ali_padding
        self.selected_bg_color = "#62b8ef"
        self.normal_bg_color = "#dfdfdf"
        self.selected_font_color = "#ffffff"
        self.normal_font_color = "#797979"
        self.draw_check = False
        self.__selected = False
        self.hover = False
        width, height = get_text_size(self.text, font_size)
        self.set_size_request(width+self.ali_padding*2, 23)
        # init events.
        self.add_events(gtk.gdk.ALL_EVENTS_MASK)
        self.connect("expose-event", self.select_button_expose_event)        
        self.connect("enter-notify-event", self._cursor_enter_redraw)
        self.connect("leave-notify-event", self._cursor_leave_redraw)

    @property
    def selected(self):
        return self.__selected

    @selected.setter
    def selected(self, selected):
        if self.__selected != selected:
            self.__selected = selected
            self.queue_draw()
       
    @selected.getter
    def selected(self):
        return self.__selected

    def _cursor_enter_redraw(self, widget, event):
        self.hover = True
        self.queue_draw()

    def _cursor_leave_redraw(self, widget, event):
        self.hover = False
        self.queue_draw()

    def select_button_expose_event(self, widget, event):
        cr = widget.window.cairo_create()
        rect = widget.allocation
        # 
        # get font width/height.
        font_w, font_h = get_text_size(self.text, text_size=self.font_size)
        # draw text.
        x_padding = rect.x + self.ali_padding
        x_padding = max(20, x_padding)

        if self.__selected or self.hover:
            # draw rectangle.
            with cairo_disable_antialias(cr):
                cr.set_source_rgb(*color_hex_to_cairo(self.selected_bg_color))
                cr.rectangle(rect.x, 
                            rect.y, 
                            rect.width, 
                            rect.height)
                cr.fill()
        
            draw_text(cr, self.text,
                    x_padding,
                    rect.y + rect.height/2 - font_h/2,
                    text_size=self.font_size, 
                    text_color=self.selected_font_color)        
        else:
            with cairo_disable_antialias(cr):
                cr.set_source_rgb(*color_hex_to_cairo(self.normal_bg_color))
                cr.rectangle(rect.x, 
                            rect.y, 
                            rect.width, 
                            rect.height)
                cr.fill()

            draw_text(cr, self.text,
                    x_padding,
                    rect.y + rect.height/2 - font_h/2,
                    text_size=self.font_size, 
                    text_color=self.normal_font_color)        
        # set size.
        if font_h > rect.height:
            widget.set_size_request(rect.width, font_h)
        return True

class SelectButtonGroup(gtk.HBox):
    __gsignals__ = {
            "button-press": (gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, (gobject.TYPE_OBJECT,)),
            }
            
    def __init__(self, buttons):
        gtk.HBox.__init__(self)
        self.buttons = buttons
        self.current_active = None
        if len(self.buttons) > 0:
            self.current_active = self.buttons[0]

        self.init_group_widget()
    
    def init_group_widget(self):
        for button in self.buttons:
            button.connect("clicked", self.button_clicked_event)
            chapter_button_align = gtk.Alignment(0, 0.5, 0, 0)
            chapter_button_align.set_padding(0, 0, 5, 5)
            chapter_button_align.add(button)
            self.pack_start(chapter_button_align, False, False)
    
    def button_clicked_event(self, active_button):
        self.emit("button-press", active_button)
        if self.current_active != active_button:
            self.current_active = active_button
        for button in self.buttons:
            button.selected = (button == active_button)

    def get_active_button(self):
        return self.current_active

class ImageButton(gtk.Button):        
    def __init__(self, normal_image, hover_image=None, press_image=None):
        gtk.Button.__init__(self)
        # init values.
        self.normal_image_pixbuf = gtk.gdk.pixbuf_new_from_file(normal_image)
        self.hover_image_pixbuf = self.normal_image_pixbuf
        self.press_image_pixbuf = self.normal_image_pixbuf
        self.draw_check = False
        self.set_size_request(self.normal_image_pixbuf.get_width(), self.normal_image_pixbuf.get_height())
        # init events.
        self._current_image_pixbuf = self.normal_image_pixbuf
        if hover_image:
            self.hover_image_pixbuf = gtk.gdk.pixbuf_new_from_file(hover_image)
        if press_image:
            self.press_image_pixbuf = gtk.gdk.pixbuf_new_from_file(press_image)
        self.add_events(gtk.gdk.ALL_EVENTS_MASK)
        self.connect("expose-event", self.image_button_expose_event)

    def image_button_expose_event(self, widget, event):
        cr = widget.window.cairo_create()
        rect = widget.allocation
        if widget.state == gtk.STATE_NORMAL:
            image = self.normal_image_pixbuf
        elif widget.state == gtk.STATE_PRELIGHT:
            image = self.hover_image_pixbuf
        elif widget.state == gtk.STATE_ACTIVE:
            image = self.press_image_pixbuf

        draw_pixbuf(cr, image, rect.x, rect.y)

        propagate_expose(widget, event)
        return True
