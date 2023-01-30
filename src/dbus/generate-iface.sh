#!/usr/bin/env bash

qdbusxml2cpp com.deepin.Manual.Open.xml \
  -a manual_open_adapter \
  -c ManualOpenAdapter

qdbusxml2cpp com.deepin.Manual.Search.xml \
  -a manual_search_adapter \
  -c ManualSearchAdapter

qdbusxml2cpp com.deepin.dde.daemon.Launcher.xml \
  -p launcher_interface \
  -i dbus/dbusvariant/app_info.h \
  -c LauncherInterface

qdbusxml2cpp com.deepin.Manual.Open.xml \
  -p manual_open_interface \
  -c ManualOpenInterface
