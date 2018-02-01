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

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>

#include "controller/search_manager.h"
#include "view/web_window.h"

namespace dman {

namespace {

const int kWinWidth = 1000;
const int kWinHeight = 715;
const int kWinOffset = 30;

}  // namespace

WindowManager::WindowManager(QObject* parent)
    : QObject(parent),
      windows_(),
      search_manager_(new SearchManager(this)) {
}

WindowManager::~WindowManager() {
}

void WindowManager::openManual(const QString& app_name) {
  qDebug() << Q_FUNC_INFO << app_name;
  for (WebWindow* window : windows_) {
    if (window->appName() == app_name) {
      window->show();
      window->raise();
      return;
    }
  }
  WebWindow* window = new WebWindow(search_manager_);
  window->setAppName(app_name);
  window->resize(kWinWidth, kWinHeight);
  window->show();

  const QPoint pos = this->newWindowPosition();
  window->move(pos);

  connect(window, &WebWindow::closed,
          this, &WindowManager::onWindowClosed);
  windows_.append(window);
}

QPoint WindowManager::newWindowPosition() {
  // If there is no window created, move new window to center of screen.
  // Else stack window one after another.
  QDesktopWidget* desktop = QApplication::desktop();
  Q_ASSERT(desktop != nullptr);
  const QRect geometry = desktop->availableGeometry(QCursor::pos());
  if (windows_.isEmpty()) {
    const QPoint center = geometry.center();
    return QPoint(center.x() - kWinWidth / 2, center.y() - kWinHeight / 2);
  } else if (windows_.size() == 1) {
    last_new_window_pos_ = QPoint(0, 0);
    return last_new_window_pos_;
  } else {
    last_new_window_pos_.setX(last_new_window_pos_.x() + kWinOffset);
    last_new_window_pos_.setY(last_new_window_pos_.y() + kWinOffset);
    if ((last_new_window_pos_.x() + kWinWidth >= geometry.width()) ||
        (last_new_window_pos_.y() + kWinHeight >= geometry.height())) {
      last_new_window_pos_.setX(0);
      last_new_window_pos_.setY(0);
    }
    return last_new_window_pos_;
  }
}

void WindowManager::onWindowClosed(const QString& app_name) {
  for (int i = 0; i < windows_.size(); ++i) {
    if (windows_.at(i)->appName() == app_name) {
      windows_.remove(i);
      break;
    }
  }
}

}  // namespace dman