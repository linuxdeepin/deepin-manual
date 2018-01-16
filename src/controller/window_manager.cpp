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
#include <QtCore/QCommandLineParser>
#include <QtDBus/QDBusConnection>
#include <QDesktopWidget>

#include "controller/search_manager.h"
#include "dbus/dbus_consts.h"
#include "dbus/manual_open_adapter.h"
#include "dbus/manual_open_interface.h"
#include "dbus/manual_open_proxy.h"
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
  WebWindow* window = new WebWindow(search_manager_);
  window->setAppName(app_name);
  window->resize(kWinWidth, kWinHeight);
  window->show();

  const QPoint pos = this->newWindowPosition();
  window->move(pos);

  // TODO(Shaohua): Handle window close event.
  windows_.append(window);
}

void WindowManager::parseArguments() {
  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption(QCommandLineOption(
      "dbus", "enable daemon mode"
  ));
  parser.parse(qApp->arguments());

  // Register dbus service.
  QDBusConnection conn = QDBusConnection::sessionBus();
  ManualOpenProxy* proxy = new ManualOpenProxy(this);
  connect(proxy, &ManualOpenProxy::openManualRequested,
          this, &WindowManager::openManual);

  ManualOpenAdapter* adapter = new ManualOpenAdapter(proxy);
  Q_UNUSED(adapter);

  if (!conn.registerService(kManualOpenService) ||
      !conn.registerObject(kManualOpenIface, proxy)) {
    // Failed to register dbus service.
    // Open appName list with dbus interface.
    const QStringList position_args = parser.positionalArguments();
    if (!position_args.isEmpty()) {
      ManualOpenInterface* iface = new ManualOpenInterface(
          kManualOpenService,
          kManualOpenIface,
          QDBusConnection::sessionBus(),
          this);
      // Only parse positional arguments.
      for (const QString& arg : position_args) {
        iface->Open(arg);
      }

      // Exit process after 1000ms.
      QTimer::singleShot(1000, []() {
        qApp->quit();
      });
    }

  } else {
    const QStringList position_args = parser.positionalArguments();
    if (position_args.isEmpty()) {
      // Open index page if not in dbus daemon mode.
      if (!parser.isSet("dbus")) {
        this->openManual("");
      }
    } else {
      // Only parse positional arguments.
      for (const QString& arg : position_args) {
        this->openManual(arg);
      }
    }
  }
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

}  // namespace dman