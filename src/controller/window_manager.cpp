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
#include "dbus/dbus_consts.h"
#include "view/web_window.h"
#include "controller/config_manager.h"
#include "dbus/manual_filesupdate_proxy.h"
#include "dbus/manual_filesupdate_adapter.h"

#include <DWidgetUtil>

#include <QApplication>
#include <QDesktopWidget>

#define WM_SENDER_NAME "Sender"
const int kWinMinWidth = 680;
const int kWinMinHeight = 300;

WindowManager::WindowManager(QObject *parent)
    : QObject(parent)
    , curr_app_name_("")
    , curr_keyword_("")
    , curr_title_name_("")
{
}

WindowManager::~WindowManager()
{
    delete window;
}

void WindowManager::setStartTime(qint64 startTime)
{
    this->appStartTime = startTime;
}

/**
 * @brief WindowManager::initDBus
 * 初始化前后端通信Dbus,服务端创建在前端.....wait
 */
void WindowManager::initDBus()
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, WM_SENDER_NAME);
    if (!dbusConn.isConnected()) {
        qDebug() << WM_SENDER_NAME << "connectToBus() failed";
        return;
    }

    if (!dbusConn.registerService(kManualSearchService + QString(WM_SENDER_NAME))
            || !dbusConn.registerObject(kManualSearchIface + QString(WM_SENDER_NAME), this)) {
        qCritical() << WM_SENDER_NAME << " failed to register dbus service!";

        return;
    } else {
        qDebug() << WM_SENDER_NAME << " register dbus service success!";
    }

    // 注册Dbus filesUpdate服务
    QDBusConnection conn = QDBusConnection::sessionBus();
    ManualFilesUpdateProxy *proxy = new ManualFilesUpdateProxy(this);
    connect(proxy, &ManualFilesUpdateProxy::FilesUpdate, this, &WindowManager::onFilesUpdate);
    ManualFilesUpdateAdapter *adapter = new ManualFilesUpdateAdapter(proxy);
    Q_UNUSED(adapter);
    //注册服务, 如果注册失败,则说明已存在一个dman.
    if (!conn.registerService(kManualFilesUpdateService)
            || !conn.registerObject(kManualFilesUpdateIface, proxy)) {
        qCritical() << "filesUpdate failed to register dbus service";
    } else {
        qDebug() << "filesUpdate register dbus service success!";
    }

}

/**
 * @brief WindowManager::initWebWindow 初始化主窗口
 */
void WindowManager::initWebWindow()
{
    window = new WebWindow;
    //设置窗口大小
    setWindow(window);
    window->show();
    window->setAppProperty(curr_app_name_, curr_title_name_, curr_keyword_);

    QTimer::singleShot(0, [=]() {
        //dbus发送窗口ID给search服务
        SendMsg(QString::number(window->winId()));
        //初始化web窗口
        window->initWeb();
        initDBus();
    });
}

/**
 * @brief WindowManager::activeOrInitWindow
 * 初始化窗口管理，应用启动时进行判断，如果窗口已经创建则激活
 */
void WindowManager::activeOrInitWindow()
{
    qDebug() << Q_FUNC_INFO;
    // 单页面该锁可能无用......
    QMutexLocker locker(&_mutex);
    /*** 只要有窗口就不再创建新窗口 2020-06-22 16:57:50 wangml ***/
    if (window != nullptr) {
        //2020-01-15 kyz 在专业服务器版最小化后show的方式可能无法激活窗口桌面版正常，可能是桌面环境问题，优先采用dock接口激活，如果失败再使用其它激活
        if (Q_LIKELY(Utils::activeWindow(window->winId()))) {
            window->saveWindowSize();
            Dtk::Widget::moveToCenter(window);
        } else {
            setWindow(window);
            window->show();
            window->raise();
            window->activateWindow();
        }
        window->openjsPage(curr_app_name_, curr_title_name_);
        return;
    }
    //不存在窗口,则初始化窗口
    initWebWindow();
}

/**
 * @brief WindowManager::SendMsg 通过dbus接口来实现前后端通信,
 * @note 通过dbus接口来实现前后端通信Search 发送窗口ID号进程号
 * @param msg 信息内容
 */
void WindowManager::SendMsg(const QString &msg)
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, WM_SENDER_NAME);
    qDebug() << "start send keyword:" << QString::number(qApp->applicationPid());
    QDBusMessage dbusMsg = QDBusMessage::createSignal(
                               kManualSearchIface + QString(WM_SENDER_NAME),
                               kManualSearchService + QString(WM_SENDER_NAME), "SendWinInfo");

    dbusMsg << QString::number(qApp->applicationPid()) + "|" + msg;

    //将进程号+窗口WinId拼接后发给dman-search后台进程 发送信号SendWinInfo－＞RecvMsg
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
    setting->beginGroup(kConfigWindowInfo);
    int saveWidth = setting->value(kConfigWindowWidth).toInt();
    int saveHeight = setting->value(kConfigWindowHeight).toInt();
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
 * @brief WindowManager::openManual F1快捷启动
 * @param app_name 应用名称
 * @param title_name 标签名称
 */
void WindowManager::openManual(const QString &app_name, const QString &title_name)
{
    curr_app_name_ = app_name;
    curr_keyword_ = "";
    curr_title_name_ = title_name;
    activeOrInitWindow();
}

/**
 * @brief WindowManager::openManualWithSearch
 * @param app_name
 * @param keyword
 * @note 供　manual_open_proxy::search()接口调用
 */
void WindowManager::openManualWithSearch(const QString &app_name, const QString &keyword)
{
    curr_app_name_ = app_name;
    curr_keyword_ = keyword;
    activeOrInitWindow();
    qDebug() << Q_FUNC_INFO << app_name << curr_keyword_;
}

/**
 * @brief WindowManager::onFilesUpdate
 * @param filesList
 * 文件更新提示
 */
void WindowManager::onFilesUpdate(const QStringList &filesList)
{
    qDebug() << Q_FUNC_INFO << filesList;
    if (window && !filesList.isEmpty()) {
        window->updatePage(filesList);
    }
    //window->openjsPage()
}
