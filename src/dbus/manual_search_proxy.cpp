// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dbus/manual_search_proxy.h"
#include "dbus/dbus_consts.h"
#include "base/utils.h"
#include "base/ddlog.h"

#include <QtDBus/QtDBus>

const QStringList systemType = {"professional", "server", "community", "personal"};

ManualSearchProxy::ManualSearchProxy(QObject *parent)
    : QObject(parent)
    , m_bWindowState(false)
{
    qCDebug(app) << "ManualSearchProxy constructor called";
    this->setObjectName("ManualSearchProxy");
    connectToSender();
    qCDebug(app) << "ManualSearchProxy initialized";
}

ManualSearchProxy::~ManualSearchProxy()
{
    qCDebug(app) << "ManualSearchProxy destructor called";
}

/**
 * @brief ManualSearchProxy::connectToSender
 * 连接DBus服务，获取SendWinInfo信息
 */
void ManualSearchProxy::connectToSender()
{
    qCDebug(app) << "connectToBus()::connect() to Sender";
    QDBusConnection senderConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");

    if (!senderConn.connect(
                kManualSearchService + QString("Sender"), // sender's service name
                kManualSearchIface + QString("Sender"), // sender's path name
                kManualSearchService + QString("Sender"), // interface
                "SendWinInfo", // sender's signal name
                this, // receiver
                SLOT(RecvMsg(const QString &)))) { // slot

        qCDebug(app) << "connectToBus()::connect() Sender SendWinInfo failed";
    } else {
        qCDebug(app) << "connectToBus()::connect() Sender SendWinInfo success";
    }
}

/**
 * @brief ManualSearchProxy::RecvMsg
 * @param data 窗口状态
 * Dbus槽函数，读取SendWinInfo信息，获取窗口状态， 关闭或打开
 */
void ManualSearchProxy::RecvMsg(const QString &data)
{
    qCDebug(app) << "RecvMsg data is: " << data;
    QStringList dataList = data.split("|");
    if (dataList.size() < 2) {
        qCDebug(app) << "wrong data style! " << data;
        return;
    }
    QString flag = dataList.last();
    if ("close" == flag) {
        qCDebug(app) << "Processing window close message";
        m_sApplicationPid = nullptr;
        m_bWindowState = false;
    } else {
        qCDebug(app) << "Processing window open message, PID:" << dataList.last();
        m_bWindowState = true;
        m_sApplicationPid = dataList.last();
        //关闭dmanhelper
        if ("closeDmanHelper" == flag) {
            qCDebug(app) << "Received closeDmanHelper command, exiting";
            exit(0);
        }
    }

    qCDebug(app) << "Window state updated - isOpen:" << m_bWindowState;
}

/**
 * @brief ManualSearchProxy::OnNewWindowOpen
 * @param data
 * @note 配合WindowManager类用于多窗口管理，帮助手册窗口的打开/激活
 */
void ManualSearchProxy::OnNewWindowOpen(const QString &data)
{
    qCDebug(app) << "Search data is: " << data;
    if (m_bWindowState) {
        qCDebug(app) << "Window:process" << m_sApplicationPid;
        quintptr winId = m_sApplicationPid.toULong();
        Utils::activeWindow(winId);
    }
    qCDebug(app) << "Window state updated - isOpen:" << m_bWindowState;
}

/**
 * @brief ManualSearchProxy::ManualExists
 * @param app_name
 * @return
 * @note 根据给定的应用名称，判断对应的帮助手册内容是否存在
 */
bool ManualSearchProxy::ManualExists(const QString &app_name)
{
    qCDebug(app) << "Checking if manual exists for:" << app_name;
    QStringList moduleList;
    QStringList strManualPathList = Utils::getMdsourcePath();
    foreach (auto strManualPath, strManualPathList) {
        qCDebug(app) << "Processing path:" << strManualPath;
        for (const QString &type : QDir(strManualPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            if (type == "system" || type == "application") {
                QString typePath = strManualPath + "/" + type;
                qCDebug(app) << "Found system/application dir:" << typePath;
                for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                    moduleList.append(module);
                    qCDebug(app) << "Added module:" << module;
                }
            }
#if 1 //旧文案结构兼容
            if (systemType.contains(type)) {
                QString typePath = strManualPath + "/" + type;
                qCDebug(app) << "Found legacy dir:" << typePath;
                for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                    moduleList.append(module);
                    qCDebug(app) << "Added legacy module:" << module;
                }
            }
#endif
        }
    }

    bool exists = moduleList.contains(app_name);
    qCDebug(app) << "Manual exists check result:" << exists;
    return exists;
}
