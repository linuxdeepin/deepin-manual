// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller/argument_parser.h"
#include "base/consts.h"
#include "base/utils.h"
#include "base/ddlog.h"
#include "dbus/dbus_consts.h"
#include "dbus/manual_open_adapter.h"
#include "dbus/manual_open_interface.h"
#include "dbus/manual_open_proxy.h"
#include "view/web_window.h"
#include "window_manager.h"
#include "base/eventlogutils.h"
#include <DLog>
#include <DGuiApplicationHelper>

#include <QCommandLineParser>
#include <QDBusConnection>

namespace {

QString ConvertOldDmanPath(const QString &app_name)
{
    qCDebug(app) << "Converting old dman path:" << app_name;
    const QStringList parts = app_name.split('/');
    const int dman_index = parts.indexOf("dman");
    if (dman_index > 0 && dman_index < parts.length() - 1) {
        qCDebug(app) << "Found dman index:" << dman_index;
        return parts.at(dman_index + 1);
    }
    qCDebug(app) << "Failed to convert old dman path:" << app_name;
    return app_name;
}

} // namespace

ArgumentParser::ArgumentParser(QObject *parent)
    : QObject(parent)
    , bIsDbus(false)
{
    qCDebug(app) << "Creating ArgumentParser instance";
}

ArgumentParser::~ArgumentParser()
{
    qCDebug(app) << "Destroying ArgumentParser instance";
}

/**
 * @brief ArgumentParser::parseArguments 解析帮助手册启动参数,同时注册对外dbus接口
 * @return false: 注册dbus失败,已存在dman,关闭当前进程.  true:注册dbus成功,同时获取当前模块参数.
 */
bool ArgumentParser::parseArguments()
{
    qCDebug(app) << "Parsing command line arguments";
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(QCommandLineOption("dbus", "enable daemon mode"));
    parser.process(*(qobject_cast<QGuiApplication *>(this)));

    // 注册Dbus open服务,对外主要接口
    qCDebug(app) << "Registering DBus services";
    QDBusConnection conn = QDBusConnection::sessionBus();
    qCDebug(app) << "Session bus connected:" << conn.isConnected();
    ManualOpenProxy *proxy = new ManualOpenProxy(this);
    connect(proxy, &ManualOpenProxy::openManualRequested, this,
            &ArgumentParser::onOpenAppRequested);
    connect(proxy, &ManualOpenProxy::searchRequested, this,
            &ArgumentParser::onSearchRequested);
    ManualOpenAdapter *adapter = new ManualOpenAdapter(proxy);
    Q_UNUSED(adapter);

    //20210705 由于新版本dtk不启动dmanHelper,dman尝试启动dmanHelper
    //20211202 之前同步调用会增加启动时间,改为异步调用
    qCDebug(app) << "Checking if manual exists via DBus";
    QDBusMessage msg = QDBusMessage::createMethodCall(kManualSearchService,
                                                      kManualSearchIface,
                                                      kManualSearchService,
                                                      "ManualExists");
    QDBusConnection::sessionBus().asyncCall(msg);
    qCDebug(app) << "Async DBus call sent";

    //注册Open服务, 如果注册失败,则说明已存在一个dman.
    if (!conn.registerService(kManualOpenService)
            || !conn.registerObject(kManualOpenIface, proxy)) {
        qCDebug(app) << "Failed to register dbus";
        const QStringList position_args = parser.positionalArguments();
        QDBusInterface manual(kManualOpenService, kManualOpenIface, kManualOpenService);
        if (!position_args.isEmpty()) {
            qCDebug(app) << "Positional arguments found:" << position_args;
            for (const QString &arg : position_args) {
                qCDebug(app) << "Calling Open on existing manual with arg:" << arg;
                QDBusReply<void> reply = manual.call("Open", arg);
            }
        } else {
            qCDebug(app) << "position_args is empty";
            //激活已有dman
            QDBusReply<void> reply = manual.call("Open", "");
        }
        qCDebug(app) << "returing false";
        return false;
    } else {
        qCDebug(app) << "Register dbus service successfully";
        const QStringList position_args = parser.positionalArguments();
        // 不带参为首页,带参跳转到具体模块.
        if (position_args.isEmpty()) {
            qCDebug(app) << "No positional arguments found, opening home page";
            //如果通过dbus打开dman, 则parser.isSet("dbus")为true
            if (parser.isSet("dbus")) {
                qCDebug(app) << "DBus mode detected";
                bIsDbus = true;
            } else {
                qCDebug(app) << "Running in normal mode";
            }
        } else {
            //获取命令行第一个参数应用名
            curManual = position_args.at(0);
            qCDebug(app) << "Manual specified:" << curManual;
        }
        qCDebug(app) << "returing true";
        return true;
    }
}

/**
 * @brief ArgumentParser::openManualsDelay 发送请求信号
 */
void ArgumentParser::openManualsDelay()
{
    qCDebug(app) << "Opening manual with delay, current manual:" << curManual;

    //判断是否为dbus服务，否则打开帮助手册窗口
    if (!bIsDbus) {
        qCDebug(app) << "Not in DBus mode, directly opening manual";
        this->onOpenAppRequested(curManual);
    }
}

/**
 * @brief ArgumentParser::onOpenAppRequested 打开对应模块帮助手册
 * @param app_name   模块名称
 * @param title_name 需要定位到的标题名称,如果为空,则定位在概述.
 */
void ArgumentParser::onOpenAppRequested(const QString &app_name, const QString &title_name)
{
    qCDebug(app) << "Opening app requested:" << app_name << "title:" << title_name;
    //应用F1和帮助打开帮助手册 数据埋点统计
    QJsonObject objStartEvent;
    objStartEvent.insert("tid", Eventlogutils::DbusStartUp);
    objStartEvent.insert("version", qApp->applicationVersion());
    objStartEvent.insert("appname", app_name);
    objStartEvent.insert("titlename", title_name);

    auto jsonStr = QJsonDocument(objStartEvent).toJson(QJsonDocument::Compact);
    qCInfo(app) << "jsonStr:" << jsonStr;

    Eventlogutils::GetInstance()->writeLogs(objStartEvent);
    //解析老的应用名为路径，解析出dman后的应用名称
    const QString compact_app_name = ConvertOldDmanPath(app_name);
    qCDebug(app) << "Converted app name:" << compact_app_name << "from:" << app_name;
    //openManualRequested---->WindowManager::openManual
    emit this->openManualRequested(compact_app_name, title_name);
    qCDebug(app) << "Emitted openManualRequested signal";
}

/**
 * @brief ArgumentParser::onSearchRequested 搜索触发槽
 * @param keyword 搜索关键字
 */
void ArgumentParser::onSearchRequested(const QString &keyword)
{
    qCDebug(app) << "Search requested with keyword:" << keyword;
    emit this->openManualWithSearchRequested("", keyword);
}
