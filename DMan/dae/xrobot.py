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

from __future__ import print_function

import Xlib.display
import Xlib.X
import Xlib.XK
import Xlib.Xatom
import Xlib.protocol.rq
import Xlib.ext.record

xlib_display = Xlib.display.Display()
xlib_screen = xlib_display.screen()
xlib_root_window = xlib_screen.root

def screen_resolution():
    return (xlib_screen["width_in_pixels"], xlib_screen["height_in_pixels"])

screen_width, screen_height = screen_resolution()

def mouse_pos():
    mouse_data = xlib_root_window.query_pointer()._data
    return mouse_data['root_x'], mouse_data['root_y']

def get_window_by_id(win_id):
    return xlib_display.create_resource_object("window", win_id)

def set_window_property(xwindow, property_type, property_content):
    xwindow.change_property(
        xlib_display.get_atom(property_type),
        Xlib.Xatom.STRING,
        8,
        property_content,
        )
    xlib_display.sync()

def get_window_property(xwindow, property_type):
    try:
        return xwindow.get_full_property(
            xlib_display.get_atom(property_type),
            Xlib.Xatom.STRING
            ).value
    except:
        return None

def set_window_property_by_id(window_id, property_type, property_content):
    xwin = get_window_by_id(window_id)
    set_window_property(xwin, property_type, property_content)

def get_window_property_by_id(window_id, property_type):
    xwin = get_window_by_id(window_id)
    return get_window_property(xwin, property_type)

def set_geometry_by_id(win_id, x, y, width, height):
    xwin = get_window_by_id(win_id)
    xwin.configure(x=x, y=y, width=width, height=height)
    xwin.change_attributes(win_gravity=Xlib.X.NorthWestGravity, bit_gravity=Xlib.X.StaticGravity)

def record_event(record_callback):
    ctx = xlib_display.record_create_context(
        0,
        [Xlib.ext.record.AllClients],
        [{
                'core_requests': (0, 0),
                'core_replies': (0, 0),
                'ext_requests': (0, 0, 0, 0),
                'ext_replies': (0, 0, 0, 0),
                'delivered_events': (0, 0),
                'device_events': (Xlib.X.KeyPress, Xlib.X.MotionNotify),
                'errors': (0, 0),
                'client_started': False,
                'client_died': False,
                }])

    xlib_display.record_enable_context(ctx, record_callback)
    xlib_display.record_free_context(ctx)

def get_event_data(data):
    return Xlib.protocol.rq.EventField(None).parse_binary_value(data, xlib_display.display, None, None)

def get_keyname(event):
    keysym = xlib_display.keycode_to_keysym(event.detail, 0)
    for name in dir(Xlib.XK):
        if name[:3] == "XK_" and getattr(Xlib.XK, name) == keysym:
            return name[3:]
    return "[%d]" % keysym

def check_valid_event(reply):
    if reply.category != Xlib.ext.record.FromServer:
        return
    if reply.client_swapped:
        return
    if not len(reply.data) or ord(str(reply.data[0])) < 2:
        return

def is_ctrl_key(keyname):
    return keyname in ["Control_L", "Control_R"]

def is_alt_key(keyname):
    return keyname in ["Alt_L", "Alt_R"]
