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
#include "base/consts.h"
#include "base/utils.h"
#include "controller/search_manager.h"
#include "dbus/dbus_consts.h"
#include "view/web_window.h"
#include <unistd.h>
#include "controller/config_manager.h"

#include <DLog>
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>

namespace dman {

namespace {

const int kWinMinWidth = 800;
const int kWinMinHeight = 600;
const int kWinOffset = 30;

const char kEnableDomStorageFlush[] = "--enable-aggressive-domstorage-flushing";
const char kDisableGpu[] = "--disable-gpu";
const char kEnableLogging[] = "--enable-logging";
const char kLogLevel[] = "--log-level";

}  // namespace

#define WM_SENDER_NAME "Sender"

WindowManager::WindowManager(QObject *parent)
    : QObject(parent)
    , search_manager_(nullptr)
    , curr_app_name_("")
    , curr_keyword_("")
    , curr_title_name_("")
{
}

WindowManager::~WindowManager() {}

void WindowManager::initDBus()
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, WM_SENDER_NAME);
    if (!dbusConn.isConnected()) {
        qDebug() << WM_SENDER_NAME << "connectToBus() failed";
        return;
    }

    if (!dbusConn.registerService(dman::kManualSearchService + QString(WM_SENDER_NAME)) ||
            !dbusConn.registerObject(dman::kManualSearchIface + QString(WM_SENDER_NAME), this)) {
        qCritical() << WM_SENDER_NAME << " failed to register dbus service!";

        return;
    } else {
        qDebug() << WM_SENDER_NAME << " register dbus service success!";
    }
}

void WindowManager::SendMsg(const QString &msg)
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, WM_SENDER_NAME);
    qDebug() << "start send keyword:" << QString::number(qApp->applicationPid());
    QDBusMessage dbusMsg = QDBusMessage::createSignal(
                               dman::kManualSearchIface + QString(WM_SENDER_NAME),
                               dman::kManualSearchService + QString(WM_SENDER_NAME), "SendWinInfo");

    dbusMsg << QString::number(qApp->applicationPid()) + "|" + msg;

    //将进程号+窗口WinId拼接后发给dman-search后台进程
    bool isSuccess = dbusConn.send(dbusMsg);
    if (isSuccess) {
        qDebug() << "send success";
    } else {
        qDebug() << "send failed";
    }
}

void WindowManager::onNewAppOpen()
{
    qDebug() << "slot onNewAppOpen";
    qDebug() << qApp->applicationPid();

    QDBusMessage msg =
        QDBusMessage::createMethodCall(dman::kManualSearchService + QString("Receiver"),
                                       dman::kManualSearchIface + QString("Receiver"),
                                       dman::kManualSearchService, "OnNewWindowOpen");

    msg << QString::number(qApp->applicationPid());
    QDBusMessage response = QDBusConnection::sessionBus().call(msg);

    if (response.type() == QDBusMessage::ReplyMessage) {
        qDebug() << "ReplyMessage";
    }

    if (QDBusMessage::ErrorMessage == response.type()) {
        qDebug() << "ErrorMessage";
    }
}

void WindowManager::initWebWindow()
{
    /*** 2020-06-22 17:04:18 wangml ***/
    window = new WebWindow;
    connect(window, &WebWindow::closed, this, &WindowManager::onWindowClosed);
    connect(window, &WebWindow::shown, this, &WindowManager::onWindowShown);
    //windows_.insert(curr_app_name_, window);
    moveWindow(window);
    window->show();
    window->activateWindow();
}

void WindowManager::activeOrInitWindow(const QString &app_name)
{
    qDebug() << Q_FUNC_INFO << app_name;
    QMutexLocker locker(&_mutex);
    /*** 只要有窗口就不再创建新窗口 2020-06-22 16:57:50 wangml ***/
    if (window != nullptr) {
        this->moveWindow(window);
        window->show();
        window->raise();
        window->activateWindow();
        window->openjsPage(app_name, curr_title_name_);
        return;
    }
    initWebWindow();
}

/*** F1快捷启动　2020-06-28 18:07:49 wangml ***/
void WindowManager::openManual(const QString &app_name, const QString &title_name)
{

    curr_app_name_ = app_name;
    curr_keyword_ = "";
    curr_title_name_ = title_name;
    activeOrInitWindow(app_name);
    qDebug() << Q_FUNC_INFO << app_name << curr_keyword_;
}


/*** 供　manual_open_proxy::search()接口调用 2020-06-29 18:49:13 wangml ***/
void WindowManager::openManualWithSearch(const QString &app_name, const QString &keyword)
{
    curr_app_name_ = app_name;
    curr_keyword_ = keyword;
    activeOrInitWindow(app_name);
    qDebug() << Q_FUNC_INFO << app_name << curr_keyword_;
}

void WindowManager::moveWindow(WebWindow *window)
{
    window->setMinimumSize(kWinMinWidth, kWinMinHeight);
    const QPoint pos = this->newWindowPosition();
    window->move(pos);
}

QPoint WindowManager::newWindowPosition()
{
    // If there is no window created, move new window to center of screen.
    // Else stack window one after another.
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(CONFIG_WINDOW_INFO);
    int saveWidth = setting->value(CONFIG_WINDOW_WIDTH).toInt();
    int saveHeight = setting->value(CONFIG_WINDOW_HEIGHT).toInt();
    setting->endGroup();
    qDebug() << "load window_width: " << saveWidth;
    qDebug() << "load window_height: " << saveHeight;
    // 如果配置文件没有数据
    if (saveWidth == 0 || saveHeight == 0) {
        saveWidth = 1024;
        saveHeight = 680;
    }

    QDesktopWidget *desktop = QApplication::desktop();
    Q_ASSERT(desktop != nullptr);
    /*** 2020-06-30 09:21:36 wangml ***/
    const QRect geometry = desktop->availableGeometry(QCursor::pos());
    const QPoint center = geometry.center();
    if (window != nullptr) {
        return QPoint(center.x() - window->width() / 2, center.y() - window->height() / 2);
    } else {
        return QPoint(center.x() - saveWidth / 2, center.y() - saveHeight / 2);
    }
}

void WindowManager::onWindowClosed()
{
    SendMsg(QString::number(window->winId()) + "|close");
}

SearchManager *WindowManager::currSearchManager()
{
    if (nullptr == search_manager_) {
        qDebug() << "start init SearchManager" << endl;
        search_manager_ = new SearchManager(this);
        initDBus();
    }

    return search_manager_;
}

/***  2020-06-29 18:04:34 wangml ***/
void WindowManager::onWindowShown()
{
    //创建search_manager
    window->setAppName(curr_app_name_);
    window->setTitleName(curr_title_name_);

    search_manager_ = currSearchManager();
    window->setSearchManager(search_manager_);
    window->setAppName(curr_app_name_);
    window->setTitleName(curr_title_name_);

    if (curr_keyword_.length() > 0) {
        window->setSearchKeyword(curr_keyword_);
    }
    SendMsg(QString::number(window->winId()));
}

}  // namespace dman
