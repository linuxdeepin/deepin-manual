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

ManualSearchProxy::ManualSearchProxy(QObject* parent)
    : QObject(parent)
    , m_conn(QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender"))
{
    this->setObjectName("ManualSearchProxy");

    if (!m_conn.isConnected()) {
        qDebug() << "SearchSender" << "connectToBus() failed";
        return;
    }

    if (!m_conn.registerService(dman::kManualSearchService+QString("Sender")) ||
        !m_conn.registerObject(dman::kManualSearchIface+QString("Sender"), this)) {
        qCritical() << "SearchSender failed to register dbus service";
        return;
    }
    else {
        qDebug() << "SearchSender register dbus service success!";
    }
}

ManualSearchProxy::~ManualSearchProxy() {

}

bool ManualSearchProxy::ManualExists(const QString &app_name)
{
    QDir manual_dir(DMAN_MANUAL_DIR);
    return manual_dir.exists(app_name);
}

void ManualSearchProxy::Slot_ManualSearchByKeyword(const QString &keyword)
{
    qDebug() << "start send keyword:" << keyword;
    QDBusMessage msg = QDBusMessage::createSignal(
                            dman::kManualSearchIface + QString("Sender"),
                            dman::kManualSearchService + QString("Sender"),
                            "Signal_ManualSearchByKeyword");

    msg << keyword;

    //发射信号
    m_conn.send(msg);
}
