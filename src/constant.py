#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os
from deepin_utils.file import get_parent_dir
from nls import get_locale_code

ALPHA_PRECISION = 16
PARAM_PRECISION = 7

CONTENTS_PATH = os.path.join(get_parent_dir(__file__, 2), "contents")
LANGUAGE = get_locale_code()

if not os.path.exists(os.path.join(CONTENTS_PATH, LANGUAGE)):
    LANGUAGE = 'en_US'

def get_system_font():
    import gtk
    font_widget = gtk.Button()
    font_name = ' '.join(str(font_widget.get_pango_context().get_font_description()).split(" ")[0:-1])
    return font_name

DEFAULT_FONT = get_system_font()
DEFAULT_FONT_SIZE = 10

APP_IMAGE_PATH = os.path.join(get_parent_dir(__file__), "app_image")

CONFIG_FILE_PATH = os.path.join(CONTENTS_PATH, LANGUAGE, "config.ini")

DEEPIN_USER_MANUAL_NAME = "com.linuxdeepin.deepinusermanual"
DEEPIN_USER_MANUAL_PATH = "/com/linuxdeepin/deepinusermanual"
