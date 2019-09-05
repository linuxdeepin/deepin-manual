/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

const int kWinWidth = 1024;
const int kWinHeight = 680;
const int kWinMinWidth = 800;
const int kWinMinHeight = 600;
const int kWinOffset = 30;

}  // namespace

WindowManager::WindowManager(QObject *parent)
    : QObject(parent)
    , windows_()
    , search_manager_(new SearchManager(this))
{
}

WindowManager::~WindowManager() {}

void WindowManager::openManual(const QString &app_name)
{
    qDebug() << Q_FUNC_INFO << app_name;
    if (windows_.contains(app_name)) {
        WebWindow *window = windows_.value(app_name);
        if (window != nullptr) {
            window->show();
            window->raise();
            window->activateWindow();
        }
        return;
    }

    // Add a placeholder record.
    windows_.insert(app_name, nullptr);

    WebWindow *window = new WebWindow(search_manager_);
    window->setAppName(app_name);
    windows_.insert(app_name, window);
    window->resize(kWinWidth, kWinHeight);
    window->setMinimumSize(kWinMinWidth, kWinMinHeight);
    window->setWindowRadius(18);
    window->show();
    window->activateWindow();

    const QPoint pos = this->newWindowPosition();
    window->move(pos);

    connect(window, &WebWindow::closed, this, &WindowManager::onWindowClosed);
}

QPoint WindowManager::newWindowPosition()
{
    // If there is no window created, move new window to center of screen.
    // Else stack window one after another.
    QDesktopWidget *desktop = QApplication::desktop();
    Q_ASSERT(desktop != nullptr);
    const QRect geometry = desktop->availableGeometry(QCursor::pos());
    if (windows_.isEmpty() || windows_.size() == 1) {
        const QPoint center = geometry.center();
        return QPoint(center.x() - kWinWidth / 2, center.y() - kWinHeight / 2);
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

void WindowManager::onWindowClosed(const QString &app_name)
{
    windows_.remove(app_name);
}

}  // namespace dman
