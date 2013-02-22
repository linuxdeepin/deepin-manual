#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os

ALPHA_PRECISION = 16
PARAM_PRECISION = 7

def get_system_font():
    import gtk
    font_widget = gtk.Button()
    font_name = ' '.join(str(font_widget.get_pango_context().get_font_description()).split(" ")[0:-1])
    return font_name

DEFAULT_FONT = get_system_font()
DEFAULT_FONT_SIZE = 10

LANGUAGE = "zh_CN"

CONTENT_CONFIG_PATH = os.path.realpath("../contents/%s/index.json" % LANGUAGE)
