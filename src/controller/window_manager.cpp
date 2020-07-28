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

#define WM_SENDER_NAME "Sender"
const int kWinMinWidth = 800;
const int kWinMinHeight = 600;

namespace dman {
WindowManager::WindowManager(QObject *parent)
    : QObject(parent)
    , search_manager_(nullptr)
    , curr_app_name_("")
    , curr_keyword_("")
    , curr_title_name_("")
{
}

WindowManager::~WindowManager()
{
}

/**
 * @brief WindowManager::initDBus 初始化前后端通信Dbus,服务端创建在前端.....wait
 */
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

void WindowManager::initWebWindow()
{
    window = new WebWindow;
    connect(window, &WebWindow::closed, this, &WindowManager::onWindowClosed);
    connect(window, &WebWindow::shown, this, &WindowManager::onWindowShown);
    setWindow(window);
    window->show();
    window->activateWindow();
}

void WindowManager::activeOrInitWindow()
{
    qDebug() << Q_FUNC_INFO;
    // 单页面该锁可能无用......
    QMutexLocker locker(&_mutex);
    /*** 只要有窗口就不再创建新窗口 2020-06-22 16:57:50 wangml ***/
    if (window != nullptr) {
        this->setWindow(window);
        window->show();
        window->raise();
        window->activateWindow();
        window->openjsPage(curr_app_name_, curr_title_name_);
        return;
    }
    initWebWindow();
}

/**
 * @brief WindowManager::SendMsg 通过dbus接口来实现前后端通信,
 * @param msg
 */
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
        qDebug() << Q_FUNC_INFO << " sendMsg success";
    } else {
        qDebug() << Q_FUNC_INFO << "sendMsg failed";
    }
}

/**
 * @brief WindowManager::moveWindow 设置window窗口属性,UI居中显示
 * @param window 主页面对象
 * @note 设置窗口最小尺寸,设置窗口大小,设置窗口居中
 */
void WindowManager::setWindow(WebWindow *window)
{
    //获取窗口上次保存尺寸,加载上次保存尺寸.
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(CONFIG_WINDOW_INFO);
    int saveWidth = setting->value(CONFIG_WINDOW_WIDTH).toInt();
    int saveHeight = setting->value(CONFIG_WINDOW_HEIGHT).toInt();
    setting->endGroup();
    // 如果配置文件没有数据
    if (saveWidth == 0 || saveHeight == 0) {
        saveWidth = 1024;
        saveHeight = 680;
    }

    //设置window窗口属性
    window->resize(saveWidth, saveHeight);
    window->setMinimumSize(kWinMinWidth, kWinMinHeight);
    window->move((QApplication::desktop()->width() - saveWidth) / 2, (QApplication::desktop()->height() - saveHeight) / 2);
}

/**
 * @brief WindowManager::onNewAppOpen 已存在dman时,再重启窗口时,通知后端将已存在的dman窗口active.
 */
void WindowManager::onNewAppOpen()
{
    qDebug() << Q_FUNC_INFO << qApp->applicationPid();
    QDBusMessage msg =
        QDBusMessage::createMethodCall(dman::kManualSearchService,
                                       dman::kManualSearchIface,
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

/*** F1快捷启动　2020-06-28 18:07:49 wangml ***/
void WindowManager::openManual(const QString &app_name, const QString &title_name)
{
    curr_app_name_ = app_name;
    curr_keyword_ = "";
    curr_title_name_ = title_name;
    activeOrInitWindow();
}


/*** 供　manual_open_proxy::search()接口调用 2020-06-29 18:49:13 wangml ***/
void WindowManager::openManualWithSearch(const QString &app_name, const QString &keyword)
{
    curr_app_name_ = app_name;
    curr_keyword_ = keyword;
    activeOrInitWindow();
    qDebug() << Q_FUNC_INFO << app_name << curr_keyword_;
}


void WindowManager::onWindowClosed()
{
    SendMsg(QString::number(window->winId()) + "|close");
}

/**
 * @brief WindowManager::onWindowShown web页面加载完后触发槽
 */
void WindowManager::onWindowShown()
{
    initDBus();
    //创建search_manager
//    search_manager_ = new SearchManager(this);
//    window->setSearchManager(search_manager_);

    window->setAppName(curr_app_name_);
    window->setTitleName(curr_title_name_);
    window->setSearchKeyword(curr_keyword_);

    SendMsg(QString::number(window->winId()));
}

}  // namespace dman
