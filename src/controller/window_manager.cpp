// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller/window_manager.h"
#include "base/consts.h"
#include "base/utils.h"
#include "base/ddlog.h"
#include "dbus/dbus_consts.h"
#include "view/web_window.h"
#include "controller/config_manager.h"
#include "dbus/manual_filesupdate_proxy.h"
#include "dbus/manual_filesupdate_adapter.h"

#include <DWidgetUtil>

#include <QApplication>

#define WM_SENDER_NAME "Sender"
const int kWinMinWidth = 680;
const int kWinMinHeight = 300;

WindowManager::WindowManager(QObject *parent)
    : QObject(parent)
    , curr_app_name_("")
    , curr_keyword_("")
    , curr_title_name_("")
{
    qCDebug(app) << "WindowManager constructor";
}

WindowManager::~WindowManager()
{
    qCDebug(app) << "WindowManager destructor start";
    //退出dmanHelper
    SendMsg("closeDmanHelper");
    updateDb();
    restartDmanHelper();
    ConfigManager::releaseInstance();
    delete window;
    qCDebug(app) << "WindowManager destructor end";
}

void WindowManager::setStartTime(qint64 startTime)
{
    qCDebug(app) << "Setting app start time:" << startTime;
    this->appStartTime = startTime;
}

/**
 * @brief WindowManager::initDBus
 * 初始化前后端通信Dbus,服务端创建在前端.....wait
 */
void WindowManager::initDBus()
{
    qCDebug(app) << "initDBus start";
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, WM_SENDER_NAME);
    if (!dbusConn.isConnected()) {
        qCDebug(app) << WM_SENDER_NAME << "connectToBus() failed";
        return;
    }

    if (!dbusConn.registerService(kManualSearchService + QString(WM_SENDER_NAME))
            || !dbusConn.registerObject(kManualSearchIface + QString(WM_SENDER_NAME), this)) {
        qCCritical(app) << WM_SENDER_NAME << " failed to register dbus service!";

        return;
    } else {
        qCDebug(app) << WM_SENDER_NAME << " register dbus service success!";
    }
    qCDebug(app) << "initDBus end";

    // 注册Dbus filesUpdate服务
    QDBusConnection conn = QDBusConnection::sessionBus();
    ManualFilesUpdateProxy *proxy = new ManualFilesUpdateProxy(this);
    connect(proxy, &ManualFilesUpdateProxy::FilesUpdate, this, &WindowManager::onFilesUpdate);
    ManualFilesUpdateAdapter *adapter = new ManualFilesUpdateAdapter(proxy);
    Q_UNUSED(adapter);
    //注册服务, 如果注册失败,则说明已存在一个dman.
    if (!conn.registerService(kManualFilesUpdateService)
            || !conn.registerObject(kManualFilesUpdateIface, proxy)) {
        qCCritical(app) << "filesUpdate failed to register dbus service";
    } else {
        qCDebug(app) << "filesUpdate register dbus service success!";
    }

}

/**
 * @brief WindowManager::initWebWindow 初始化主窗口
 */
void WindowManager::initWebWindow()
{
    qCDebug(app) << "initWebWindow start";
    window = new WebWindow;
    //设置窗口大小
    setWindow(window);
    window->show();
    window->setAppProperty(curr_app_name_, curr_title_name_, curr_keyword_);
    qCDebug(app) << "Window properties applied";

    QTimer::singleShot(0, [ = ]() {
        //dbus发送窗口ID给search服务
        SendMsg(QString::number(window->winId()));
        //初始化web窗口
        window->initWeb();

        qCDebug(app) << "Initializing DBus connections...";
        initDBus();
    });
    qCDebug(app) << "initWebWindow end";
}

/**
 * @brief WindowManager::activeOrInitWindow
 * 初始化窗口管理，应用启动时进行判断，如果窗口已经创建则激活
 */
void WindowManager::activeOrInitWindow()
{
    qCDebug(app) << "activeOrInitWindow";

    // 单页面该锁可能无用......
    QMutexLocker locker(&_mutex);
    /*** 只要有窗口就不再创建新窗口 2020-06-22 16:57:50 wangml ***/
    if (window != nullptr) {
        //2020-01-15 kyz 在专业服务器版最小化后show的方式可能无法激活窗口桌面版正常，可能是桌面环境问题，优先采用dock接口激活，如果失败再使用其它激活
        if (Q_LIKELY(Utils::activeWindow(window->winId()))) {
            qCDebug(app) << "Window activated via Utils::activeWindow, winId:" << window->winId();
            window->saveWindowSize();
            Dtk::Widget::moveToCenter(window);
            qCDebug(app) << "Window moved to center";
        } else {
            qCDebug(app) << "Fallback activation method for winId:" << window->winId();
            setWindow(window);
            window->show();
            window->raise();
            window->activateWindow();
            qCDebug(app) << "Window shown and activated";
        }
        window->openjsPage(curr_app_name_, curr_title_name_);
        qCDebug(app) << "openjsPage end";
        return;
    }

    qCDebug(app) << "No existing window found, initializing new window...";
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
    qCDebug(app) << "Sending DBus message:" << msg;
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, WM_SENDER_NAME);
    qCDebug(app) << "Connected to session bus, PID:" << QString::number(qApp->applicationPid());
    QDBusMessage dbusMsg = QDBusMessage::createSignal(
                               kManualSearchIface + QString(WM_SENDER_NAME),
                               kManualSearchService + QString(WM_SENDER_NAME), "SendWinInfo");

    dbusMsg << QString::number(qApp->applicationPid()) + "|" + msg;
    bool isSuccess = true;
    if (msg == "closedmanHelper") {
        //关闭dmanhelper进程需要等待返回
        dbusConn.call(dbusMsg);
        qCDebug(app) << "Sent message to close dmanhelper";
    } else {
        //将进程号+窗口WinId拼接后发给dman-search后台进程 发送信号SendWinInfo－＞RecvMsg
        isSuccess = dbusConn.send(dbusMsg);
        qCDebug(app) << "Sent message to dman-search, isSuccess:" << isSuccess;
    }
    qCDebug(app) << "DBus message sent, isSuccess:" << isSuccess;
}

/**
 * @brief WindowManager::moveWindow 设置window窗口属性,UI居中显示
 * @param window 主页面对象
 * @note 设置窗口最小尺寸,设置窗口大小,设置窗口居中
 */
void WindowManager::setWindow(WebWindow *window)
{
    qCDebug(app) << "setWindow with size:" << window->size();
    //获取窗口上次保存尺寸,加载上次保存尺寸.
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(QString(kConfigWindowInfo));
    int saveWidth = setting->value(QString(kConfigWindowWidth)).toInt();
    int saveHeight = setting->value(QString(kConfigWindowHeight)).toInt();
    setting->endGroup();
    // 如果配置文件没有数据
    if (saveWidth == 0 || saveHeight == 0) {
        saveWidth = 1024;
        saveHeight = 680;
        qCDebug(app) << "Using default window size:" << saveWidth << "x" << saveHeight;
    }

    //设置window窗口属性
    window->resize(saveWidth, saveHeight);

    qCDebug(app) << "Setting minimum window size to:" << kWinMinWidth << "x" << kWinMinHeight;
    window->setMinimumSize(kWinMinWidth, kWinMinHeight);

    qCDebug(app) << "Window properties configured successfully";
}

void WindowManager::updateDb()
{
    qCDebug(app) << "updateDb";
    if (window) {
        qCDebug(app) << "Calling window->updateDb()";
        window->updateDb();
    }
    qCDebug(app) << "updateDb end";
}

void WindowManager::restartDmanHelper()
{
    qCDebug(app) << "restartDmanHelper start";
    QDBusInterface interface(kManualSearchService, kManualSearchIface,
                                 kManualSearchService,
                                 QDBusConnection::sessionBus());

    if (!interface.isValid()) {
        qCDebug(app) << qPrintable(QDBusConnection::sessionBus().lastError().message());
        exit(1);
    }

    // 调用远程对象的方法 setName()
    QDBusReply<QString> reply = interface.call("ManualExists");

    if (reply.isValid()) {
        QString value = reply.value();
        qCDebug(app) << "value = " << value ;
    }
    qCDebug(app) << "restartDmanHelper end";
}

/**
 * @brief WindowManager::openManual F1快捷启动
 * @param app_name 应用名称
 * @param title_name 标签名称
 */
void WindowManager::openManual(const QString &app_name, const QString &title_name)
{
    qCDebug(app) << "Opening manual for app:" << app_name << "title:" << title_name;
    curr_app_name_ = app_name;
    curr_keyword_ = "";
    curr_title_name_ = title_name;
    activeOrInitWindow();
    qCDebug(app) << "openManual end";
}

/**
 * @brief WindowManager::openManualWithSearch
 * @param app_name
 * @param keyword
 * @note 供　manual_open_proxy::search()接口调用
 */
void WindowManager::openManualWithSearch(const QString &app_name, const QString &keyword)
{
    qCDebug(app) << "Opening manual for app:" << app_name << "keyword:" << keyword;
    curr_app_name_ = app_name;
    curr_keyword_ = keyword;
    activeOrInitWindow();
    qCDebug(app) << app_name << curr_keyword_;
}

/**
 * @brief WindowManager::onFilesUpdate
 * @param filesList
 * 文件更新提示
 */
void WindowManager::onFilesUpdate(const QStringList &filesList)
{
    qCDebug(app) << filesList;
    if (window && !filesList.isEmpty()) {
        window->updatePage(filesList);
        qCDebug(app) << "onFilesUpdate updatePage end";
    }
    //window->openjsPage()
    qCDebug(app) << "onFilesUpdate end";
}
