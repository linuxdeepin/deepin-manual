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

#include <QDir>
#include <QtDBus/QtDBus>

#include <DLog>

ManualSearchProxy::ManualSearchProxy(QObject *parent)
    : QObject(parent)
    , m_dbusConn(QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Receiver"))
{
    this->setObjectName("ManualSearchProxy");
    m_bWindowState = false;

    initDBus();
    connectToSender();
}

ManualSearchProxy::~ManualSearchProxy() {}

void ManualSearchProxy::initDBus()
{
    if (!m_dbusConn.isConnected()) {
        qDebug() << "Receiver"
                 << "connectToBus() failed";
        return;
    }

    if (!m_dbusConn.registerService(dman::kManualSearchService + QString("Receiver")) ||
            !m_dbusConn.registerObject(dman::kManualSearchIface + QString("Receiver"), this)) {
        qCritical() << "Receiver failed to register dbus service";
        return;
    } else {
        qDebug() << "Receiver register dbus service success!";
    }
}

void ManualSearchProxy::connectToSender()
{
    QDBusConnection senderConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");

    if (!senderConn.connect(
                dman::kManualSearchService + QString("Sender"),  // sender's service name
                dman::kManualSearchIface + QString("Sender"),    // sender's path name
                dman::kManualSearchService + QString("Sender"),  // interface
                "SendWinInfo",                                   // sender's signal name
                this,                                            // receiver
                SLOT(RecvMsg(const QString &)))) {               // slot

        qDebug() << "connectToBus()::connect() Sender SendWinInfo failed";
    } else {
        qDebug() << "connectToBus()::connect() Sender SendWinInfo success";
    }
}

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

bool ManualSearchProxy::ManualExists(const QString &app_name)
{
    QString strManualPath = DMAN_MANUAL_DIR;
    int nType = Dtk::Core::DSysInfo::deepinType();
    if (Dtk::Core::DSysInfo::DeepinServer == (Dtk::Core::DSysInfo::DeepinType)nType) {
        strManualPath += "/server";
    } else if (Dtk::Core::DSysInfo::DeepinPersonal == (Dtk::Core::DSysInfo::DeepinType)nType) {
        strManualPath += "/personal";
    } else {
        if (Dtk::Core::DSysInfo::isCommunityEdition()) {
            strManualPath += "/community";
        } else {
            strManualPath += "/professional";
        }
    }

    QDir manual_dir(strManualPath);
    return manual_dir.exists(app_name);
}
