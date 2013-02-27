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
import traceback

def get_home_item_values():
    ini = Config(CONFIG_FILE_PATH)
    ini.load()
    categorys = eval(ini.get("config", "category"))
    home_item_values = []
    for category in categorys:
        write_category_pages_id_to_config(category)
        item = {}
        item["category"] = category
        item["title"] = ini.get("title", category)
        item["icon_path"] = "../%s/%s/icon.png" % (LANGUAGE, category)
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

def get_category_pages_id(category):
    data = []
    category_contents = get_category_contents(category)
    subjects = category_contents["content"]
    for subject_index in range(len(subjects)):
        for page in subjects[subject_index]["page"]:
            data.append((subject_index, page["id"]))

    return data

def get_progress_config():
    progress_file = os.path.expanduser("~/.config/deepin-user-manual/progress.ini")
    progress_config = Config(progress_file)

    if os.path.exists(progress_file):
        progress_config.load()
    else:
        touch_file(progress_file)
        progress_config.load()

    return progress_config

def write_category_pages_id_to_config(category):
    ids = str(get_category_pages_id(category))
    progress_config = get_progress_config()
    if (not progress_config.has_option("all", category)) or (progress_config.get("all", category) != ids):
        progress_config.set("all", category, ids)
        progress_config.write()

def get_category_unread_pages(category):
    progress_config = get_progress_config()
    all_pages_id = eval(progress_config.get("all", category))
    if progress_config.has_option("unread", category):
        unread_pages_id = eval(progress_config.get("unread", category))
        if set(unread_pages_id) < set(all_pages_id):
            return unread_pages_id
    progress_config.set("unread", category, str(all_pages_id))
    progress_config.write()
    return all_pages_id

def write_unread_pages_data(home_value):
    progress_file = get_progress_config()
    for item in home_value:
        category = item["category"]
        unread_pages = item["unread_pages"]
        progress_config.set("unread", category, unread_pages)

if __name__ == "__main__":
    pass
