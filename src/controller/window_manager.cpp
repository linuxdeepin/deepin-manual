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
#include "base/consts.h"
#include "dbus/dbus_consts.h"

#include <qcef_context.h>
#include <qcef_web_settings.h>
#include <qcef_global_settings.h>

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

const char kEnableDomStorageFlush[] = "--enable-aggressive-domstorage-flushing";
const char kDisableGpu[] = "--disable-gpu";
const char kEnableLogging[] = "--enable-logging";
const char kLogLevel[] = "--log-level";

}  // namespace

#define WM_SENDER_NAME "Sender"

WindowManager::WindowManager(QObject *parent)
    : QObject(parent)
    , windows_()
    , search_manager_(nullptr)
    , curr_app_name_("")
    , curr_keyword_("")
{
}

WindowManager::~WindowManager() {}

void WindowManager::initDBus()
{
    QDBusConnection dbusConn = QDBusConnection::connectToBus(QDBusConnection::SessionBus, WM_SENDER_NAME);
    if (!dbusConn.isConnected()) {
        qDebug() << WM_SENDER_NAME << "connectToBus() failed";
        return;
    }

    if (!dbusConn.registerService(dman::kManualSearchService+QString(WM_SENDER_NAME)) ||
        !dbusConn.registerObject(dman::kManualSearchIface+QString(WM_SENDER_NAME), this)) {
        qCritical() << WM_SENDER_NAME << " failed to register dbus service!";

        return;
    }
    else {
        qDebug() << WM_SENDER_NAME << " register dbus service success!";
    }
}

void WindowManager::SendMsg(const QString &msg)
{
    QDBusConnection dbusConn = QDBusConnection::connectToBus(QDBusConnection::SessionBus, WM_SENDER_NAME);
    qDebug() << "start send keyword:" << QString::number(qApp->applicationPid());
    QDBusMessage dbusMsg = QDBusMessage::createSignal(
                                dman::kManualSearchIface + QString(WM_SENDER_NAME),
                                dman::kManualSearchService + QString(WM_SENDER_NAME),
                                "SendWinInfo");

    dbusMsg << QString::number(qApp->applicationPid()) + "|" + msg;

    //将进程号+窗口WinId拼接后发给dman-search后台进程
    bool isSuccess = dbusConn.send(dbusMsg);
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
    qDebug() << qApp->applicationPid();

    QDBusMessage msg = QDBusMessage::createMethodCall(
                dman::kManualSearchService+QString("Receiver"),
                dman::kManualSearchIface+QString("Receiver"),
                dman::kManualSearchService,
                "OnNewWindowOpen");

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

int WindowManager::initQCef(int argc, char **argv)
{
    QCefGlobalSettings settings;
    // Do not use sandbox.
    settings.setNoSandbox(true);

    if (qEnvironmentVariableIntValue("QCEF_DEBUG") == 1) {
        // Open http://localhost:9222 in chromium browser to see dev tools.
        settings.setRemoteDebug(true);
        settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Verbose);
    } else {
        settings.setRemoteDebug(false);
        settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Error);
    }

    // Disable GPU process.
    settings.addCommandLineSwitch(dman::kDisableGpu, "");

    // Enable aggressive storage commit to minimize data loss.
    // See public/common/content_switches.cc.
    settings.addCommandLineSwitch(dman::kEnableDomStorageFlush, "");

    // Set web cache folder.
    QDir cache_dir(dman::GetCacheDir());
    cache_dir.mkpath(".");
    settings.setCachePath(cache_dir.filePath("cache"));
    settings.setUserDataPath(cache_dir.filePath("cef-storage"));

    // TODO: Rotate console log.
    settings.setLogFile(cache_dir.filePath("web-console.log"));
    settings.addCommandLineSwitch(dman::kEnableLogging, "");
    settings.addCommandLineSwitch(dman::kLogLevel, "0");
    settings.addCommandLineSwitch("--use-views", "");

    return QCefInit(argc, argv, settings);
}

void WindowManager::initWebWindow()
{
    WebWindow *window = new WebWindow;
    connect(window, &WebWindow::closed, this, &WindowManager::onWindowClosed);
    connect(window, &WebWindow::shown, this, &WindowManager::onWindowShown);
    moveWindow(window);
    window->show();
    window->activateWindow();
}

void WindowManager::activeExistingWindow()
{
    qDebug() << "openManual contains:" << curr_app_name_;
    WebWindow *window = windows_.value(curr_app_name_);
    if (window != nullptr) {
        window->show();
        window->raise();
        window->activateWindow();

        SendMsg(QString::number(window->winId()));
    }

    if (curr_keyword_.length() > 0) {
        emit window->manualSearchByKeyword(curr_keyword_);
    }
}

void WindowManager::openManual(const QString &app_name)
{
    curr_app_name_ = app_name;
    curr_keyword_ = "";
    qDebug() << Q_FUNC_INFO << app_name;
    if (windows_.contains(app_name)) {
        activeExistingWindow();
        return;
    }

    initWebWindow();
}

void WindowManager::openManualWithSearch(const QString &app_name, const QString &keyword)
{
    curr_keyword_ = keyword;
    openManual(app_name);
}

SearchManager* WindowManager::currSearchManager()
{
    if (nullptr == search_manager_) {
        qDebug() << "start init SearchManager" << endl;
        search_manager_ = new SearchManager(this);
        initDBus();
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

void WindowManager::onWindowShown(WebWindow *window)
{
    // Add a placeholder record.
    windows_.insert(curr_app_name_, nullptr);
    windows_.insert(curr_app_name_, window);
    search_manager_ = currSearchManager();
    window->setSearchManager(search_manager_);
    window->setAppName(curr_app_name_);
    if (curr_keyword_.length() > 0) {
        window->setSearchKeyword(curr_keyword_);
    }

    SendMsg(QString::number(window->winId()));
}

}  // namespace dman
