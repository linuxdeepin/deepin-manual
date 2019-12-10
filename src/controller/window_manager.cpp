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
#include "controller/search_manager.h"
#include "view/web_window.h"
#include "base/utils.h"
#include "dbus/dbus_consts.h"

#include <unistd.h>

#include <QApplication>
#include <DLog>
#include <QDesktopWidget>
#include <QFile>

namespace dman {

namespace {

const int kWinWidth = 1024;
const int kWinHeight = 680;
const int kWinMinWidth = 800;
const int kWinMinHeight = 600;
const int kWinOffset = 30;

}  // namespace

#define WM_SENDER_NAME "Sender"

WindowManager::WindowManager(QObject *parent)
    : QObject(parent)
    , windows_()
    , search_manager_(nullptr)
    , m_dbusConn(QDBusConnection::connectToBus(QDBusConnection::SessionBus, WM_SENDER_NAME))
{
    initDBus();
}

WindowManager::~WindowManager() {}

void WindowManager::initDBus()
{
    if (!m_dbusConn.isConnected()) {
        qDebug() << WM_SENDER_NAME << "connectToBus() failed";
        return;
    }

    if (!m_dbusConn.registerService(dman::kManualSearchService+QString(WM_SENDER_NAME)) ||
        !m_dbusConn.registerObject(dman::kManualSearchIface+QString(WM_SENDER_NAME), this)) {
        qCritical() << WM_SENDER_NAME << " failed to register dbus service!";

        return;
    }
    else {
        qDebug() << WM_SENDER_NAME << " register dbus service success!";
    }
}

void WindowManager::SendMsg(const QString &msg)
{
    qDebug() << "start send keyword:" << QString::number(qApp->applicationPid());
    QDBusMessage dbusMsg = QDBusMessage::createSignal(
                                dman::kManualSearchIface + QString(WM_SENDER_NAME),
                                dman::kManualSearchService + QString(WM_SENDER_NAME),
                                "Signal_Search");

    dbusMsg << QString::number(qApp->applicationPid()) + "|" + msg;

    //发射信号
    bool isSuccess = m_dbusConn.send(dbusMsg);
    if (isSuccess) {
        qDebug() << "send success";
    }
    else {
        qDebug() << "send failed";
    }
}

void WindowManager::RecvMsg(const QString &data)
{
    qDebug() << "sync:" << data;
}

void WindowManager::onNewAppOpen()
{
    qDebug() << "slot onNewAppOpen";
    qDebug() << qApp->applicationPid(); // 进程id

    // 传参数
    QDBusMessage msg = QDBusMessage::createMethodCall(
                dman::kManualSearchService+QString("Receiver"),
                dman::kManualSearchIface+QString("Receiver"),
                dman::kManualSearchService,
                "Search");

    msg << QString::number(qApp->applicationPid());
    QDBusMessage response = QDBusConnection::sessionBus().call(msg);

    if(response.type() == QDBusMessage::ReplyMessage)
    {
        qDebug() << "ReplyMessage";
    }

    if (QDBusMessage::ErrorMessage == response.type())
    {
        qDebug() << "ErrorMessage";
    }
}

void WindowManager::openManual(const QString &app_name)
{
    qDebug() << Q_FUNC_INFO << app_name;
    if (windows_.contains(app_name)) {
        qDebug() << "openManual contains:" << app_name;
        WebWindow *window = windows_.value(app_name);
        if (window != nullptr) {
            window->show();
            window->raise();
            window->activateWindow();
            window->setSearchManager(currSearchManager());

            SendMsg(QString::number(window->winId()));
        }
        return;
    }

    // Add a placeholder record.
    windows_.insert(app_name, nullptr);

    WebWindow *window = new WebWindow;
    window->setAppName(app_name);
    window->show();
    window->activateWindow();

    moveWindow(window);
    windows_.insert(app_name, window);

    search_manager_ = currSearchManager();
    window->setSearchManager(search_manager_);
    connect(window, &WebWindow::closed, this, &WindowManager::onWindowClosed);

    SendMsg(QString::number(window->winId()));
}

void WindowManager::openManualWithSearch(const QString &app_name, const QString &keyword)
{
    qDebug() << "openManualWithSearch: " << keyword << endl;

    qDebug() << Q_FUNC_INFO << app_name;
    if (windows_.contains(app_name)) {
        qDebug() << "openManual contains:" << app_name;
        WebWindow *window = windows_.value(app_name);
        if (window != nullptr) {
            window->show();
            window->raise();
            window->activateWindow();
            window->setSearchManager(currSearchManager());

            SendMsg(QString::number(window->winId()));
            emit window->manualSearchByKeyword(keyword);
        }
        return;
    }

    // Add a placeholder record.
    windows_.insert(app_name, nullptr);

    WebWindow *window = new WebWindow;
    window->setSearchKeyword(keyword);
    window->setAppName(app_name);
    window->show();
    window->activateWindow();

    moveWindow(window);
    windows_.insert(app_name, window);

    search_manager_ = currSearchManager();
    window->setSearchManager(search_manager_);
    connect(window, &WebWindow::closed, this, &WindowManager::onWindowClosed);

    SendMsg(QString::number(window->winId()));
}

SearchManager* WindowManager::currSearchManager()
{
    if (nullptr == search_manager_) {
        qDebug() << "init SearchManager" << endl;
        search_manager_ = new SearchManager(this);
    }

    return search_manager_;
}

void WindowManager::moveWindow(WebWindow *window)
{
    window->resize(kWinWidth, kWinHeight);
    window->setMinimumSize(kWinMinWidth, kWinMinHeight);
    const QPoint pos = this->newWindowPosition();
    window->move(pos);
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
    WebWindow *window = windows_.value(app_name);
    SendMsg(QString::number(window->winId()) + "|close");
    windows_.remove(app_name);
}

}  // namespace dman
