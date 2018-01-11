#!/usr/bin/env bash

qdbusxml2cpp com.deepin.Manual.Open.xml \
  -a manual_open_adapter \
  -c ManualOpenAdapter

qdbusxml2cpp com.deepin.Manual.Search.xml \
  -a manual_search_adapter \
  -c ManualSearchAdapter