/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "controller/window_manager.h"

#include <DWidgetUtil>
#include "view/web_window.h"

namespace dman {

WindowManager::WindowManager(QObject* parent)
    : QObject(parent),
      windows_() {
}

WindowManager::~WindowManager() {
  // TODO(Shaohua): Close all of web windows.
}

void WindowManager::openManual(const QString& app_name) {
  for (WebWindow* window : windows_) {
    if (window->appName() == app_name) {
      window->show();
      window->raise();
      return;
    }
  }
  WebWindow* window = new WebWindow();
  window->setAppName(app_name);
  window->resize(998, 662);
  window->show();

  // Move to center of screen. No need to reposition current window with
  // existing web windows.
  Dtk::Widget::moveToCenter(window);

  // TODO(Shaohua): Handle window close event.
  windows_.append(window);
}

}  // namespace dman