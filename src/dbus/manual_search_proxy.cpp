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

#include "dbus/manual_search_proxy.h"
#include "dbus/dbus_consts.h"

#include <QtDBus/QtDBus>

ManualSearchProxy::ManualSearchProxy(QObject *parent)
    : QObject(parent)
    , m_bWindowState(false)
{
    this->setObjectName("ManualSearchProxy");
    connectToSender();
}

ManualSearchProxy::~ManualSearchProxy()
{
}

/**
 * @brief ManualSearchProxy::connectToSender
 * 连接DBus服务，获取SendWinInfo信息
 */
void ManualSearchProxy::connectToSender()
{
    QDBusConnection senderConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");

    if (!senderConn.connect(
                kManualSearchService + QString("Sender"),  // sender's service name
                kManualSearchIface + QString("Sender"),    // sender's path name
                kManualSearchService + QString("Sender"),  // interface
                "SendWinInfo",                                   // sender's signal name
                this,                                            // receiver
                SLOT(RecvMsg(const QString &)))) {               // slot

        qDebug() << "connectToBus()::connect() Sender SendWinInfo failed";
    } else {
        qDebug() << "connectToBus()::connect() Sender SendWinInfo success";
    }
}

/**
 * @brief ManualSearchProxy::RecvMsg
 * @param data 窗口状态
 * Dbus槽函数，读取SendWinInfo信息，获取窗口状态， 关闭或打开
 */
void ManualSearchProxy::RecvMsg(const QString &data)
{
    qDebug() << "RecvMsg data is: " << data;
    QStringList dataList = data.split("|");
    if (dataList.size() < 2) {
        qDebug() << "wrong data style! " << data;
        return;
    }
    QString flag = dataList.last();
    if ("close" == flag) {
        m_sApplicationPid = nullptr;
        m_bWindowState = false;
    } else {
        m_bWindowState = true;
        m_sApplicationPid = dataList.last();
    }

    return;
}

/**
 * @brief ManualSearchProxy::OnNewWindowOpen
 * @param data
 * @note 配合WindowManager类用于多窗口管理，帮助手册窗口的打开/激活
 */
void ManualSearchProxy::OnNewWindowOpen(const QString &data)
{
    qDebug() << "Search data is: " << data;
    if (m_bWindowState) {
        qDebug() << "Window:process" << m_sApplicationPid;
        quintptr winId = m_sApplicationPid.toULong();
        // new interface use applicationName as id
        QDBusInterface manual("com.deepin.dde.daemon.Dock", "/com/deepin/dde/daemon/Dock",
                              "com.deepin.dde.daemon.Dock");
        QDBusReply<void> reply = manual.call("ActivateWindow", winId);
        if (reply.isValid()) {
            qDebug() << "call com.deepin.dde.daemon.Dock success";
            return;
        }
        qDebug() << "call com.deepin.dde.daemon.Dock failed" << reply.error();
    }
}

/**
 * @brief ManualSearchProxy::ManualExists
 * @param app_name
 * @return
 * @note 根据给定的应用名称，判断对应的帮助手册内容是否存在
 */
bool ManualSearchProxy::ManualExists(const QString &app_name)
{
    QStringList moduleList;
    QString strManualPath = DMAN_MANUAL_DIR;
    for (const QString &type : QDir(strManualPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
        if (type == "system" || type == "application") {
            QString typePath = strManualPath + "/" + type;
            for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                moduleList.append(module);
            }
        }
    }
    return moduleList.contains(app_name);
}
