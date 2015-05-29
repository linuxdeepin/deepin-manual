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

import Xlib.X
from dae.ewmh import EWMH
ewmh = EWMH()

def get_xwindow(win_id):
    xwin = ewmh._createWindow(win_id)
    return xwin

def show_shadow(xwin, shadow_width):
    ewmh.setXWindowProperty(xwin, "DEEPIN_WINDOW_SHADOW", str(shadow_width))

def hide_shadow(xwin):
    ewmh.setXWindowProperty(xwin, "DEEPIN_WINDOW_SHADOW", "0")

def set_geometry(xwin, x, y, width, height):
    if x>=0 and y>=0:
        ewmh.setMoveResizeWindow(xwin, Xlib.X.NorthWestGravity, x, y, width, height)
        ewmh.display.flush()

def is_maximizable(xwin):
    return ewmh.getWmAllowedActions(xwin, "_NET_WM_ACTION_MAXIMIZE_HORZ") \
            and ewmh.getWmAllowedActions(xwin, "_NET_WM_ACTION_MAXIMIZE_VERT")

def set_maximizable(xwin, maximize):
    pass

def do_maximize(xwin):
    ewmh.setWmState(xwin, 1, "_NET_WM_STATE_MAXIMIZED_VERT", "_NET_WM_STATE_MAXIMIZED_HORZ")
    ewmh.display.flush()

def undo_maxmize(xwin):
    ewmh.setWmState(xwin, 0, "_NET_WM_ACTION_MAXIMIZE_HORZ", "_NET_WM_ACTION_MAXIMIZE_VERT")
    ewmh.display.flush()

def is_stay_on_top(xwin):
    return u"_NET_WM_STATE_ABOVE" in ewmh.getWmState(xwin, True)

def do_stay_on_top(xwin):
    ewmh.setWmState(xwin, 1, "_NET_WM_STATE_ABOVE")
    ewmh.display.flush()

def undo_stay_on_top(xwin):
    ewmh.setWmState(xwin, 0, "_NET_WM_STATE_ABOVE")
    ewmh.display.flush()

