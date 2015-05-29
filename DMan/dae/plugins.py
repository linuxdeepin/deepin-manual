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

import imp
import os
import sys
import traceback

class PluginsManager(object):

    def __init__(self, plugindir, load=True):
        self.plugindir = plugindir
        self.loaded_plugins = {}
        self.enabled_plugins = {}

    def __find_plugin(self, plugin_name):
        if not plugin_name:
            return None
        plugin_dir = self.plugindir
        plugin_path = os.path.join(plugin_dir, plugin_name)
        init_path = os.path.join(plugin_path, "__init__.py")
        if os.path.exists(init_path):
            return plugin_path
        return None

    def load_plugin(self, plugin_name, reload=False):
        if not reload and plugin_name in self.loaded_plugins:
            return self.loaded_plugins[plugin_name]

        path = self.__find_plugin(plugin_name)
        if path is None:
            return False
        sys.path.insert(0, path)
        plugin = imp.load_source(plugin_name, os.path.join(path, '__init__.py'))
        sys.path = sys.path[1:]
        self.loaded_plugins[plugin_name] = plugin
        return plugin

    def get_all_plugins(self):
        pluginlist = []
        plugin_dir = self.plugindir
        if os.path.exists(plugin_dir):
            for plugin_file in os.listdir(plugin_dir):
                if plugin_file not in pluginlist and \
                        os.path.isdir(os.path.join(plugin_dir, plugin_file)) and \
                        os.path.exists(os.path.join(plugin_dir, plugin_file, "__init__.py")):
                    pluginlist.append(plugin_file)
        return pluginlist

    def get_plugin_object(self, plugin_name):
        plugin = self.load_plugin(plugin_name)
        all_objs = []
        for obj in plugin.export_objects():
            name = "%s_%s" % (plugin_name, obj["name"])
            all_objs.append(dict(name=name, obj=obj["obj"]))
        return all_objs

    def load(self, assets):
        pluginlist = self.get_all_plugins()
        for plugin_name in pluginlist:
            try:
                assets.pluginobjects += self.get_plugin_object(plugin_name)
            except Exception as e:
                print("Load \"%s\" plugin error: %s" % (plugin_name, e))
                traceback.print_exc(file=sys.stdout)

