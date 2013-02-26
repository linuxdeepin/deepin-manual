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

import os, sys
from constant import CONFIG_FILE_PATH, LANGUAGE
from ConfigParser import ConfigParser
from deepin_utils.config import Config
from deepin_utils.file import touch_file

def get_home_item_values():
    ini = Config(CONFIG_FILE_PATH)
    ini.load()

    home_item_values = []
    categorys = eval(ini.get("config", "category"))
    for category in categorys:
        item = {}
        item["category"] = category
        item["title"] = ini.get("title", category)
        item["icon_path"] = "../%s/%s/icon.png" % (LANGUAGE, category)
        item["percent"] = get_percent(category)
        home_item_values.append(item)

    return home_item_values

def get_category_contents(category):
    json_file = os.path.realpath("../contents/%s/%s/content.json" % (LANGUAGE, category))
    try:
        content_dict = eval(open(json_file).read())
    except Exception, e:
        print "there is something wrong in content file: %s" % json_file
        traceback.print_exc(file=sys.stdout)
        sys.exit(1)

    for subject in content_dict["content"]:
        for page in subject["page"]:
            # image path for html file
            page["image"] = "../%s/%s/%s" % (LANGUAGE, category, page["image"])

    return content_dict

def get_percent(category):
    progress_file = os.path.expanduser("~/.config/deepin-user-manual/progress.ini")
    progress_config = Config(progress_file)

    if os.path.exists(progress_file):
        progress_config.load()
    else:
        touch_file(progress_file)
        progress_config.load()

    if progress_config.has_option("progress", category):
        return progress_config.get("progress", category)
    else:
        progress_config.set("progress", category, 0)
        progress_config.write()
        return 0

if __name__ == "__main__":
    print get_home_item_values()
