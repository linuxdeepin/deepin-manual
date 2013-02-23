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

import os
from constant import CONFIG_FILE_PATH, LANGUAGE
from ConfigParser import ConfigParser

def get_category():
    config = ConfigParser()
    config.read(CONFIG_FILE_PATH)
    return eval(config.get("config", "category"))

def get_all_contents():
    all_contents = []
    categorys = get_category()
    for c in categorys:
        json_file = os.path.realpath("../contents/%s/%s/content.json" % (LANGUAGE, c))
        js_dict = eval(open(json_file).read())
        for subject in js_dict["content"]:
            for page in subject["page"]:
                # image path for html file
                page["iamge"] = "../%s/%s/%s" % (LANGUAGE, c, page["image"])   
        all_contents.append(js_dict)

    return all_contents

if __name__ == "__main__":
    print get_all_contents()
