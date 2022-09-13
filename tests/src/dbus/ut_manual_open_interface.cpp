// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_manual_open_interface.h"
#include "../src/dbus/manual_open_interface.h"

ut_ManualOpenInterface::ut_ManualOpenInterface()
{
}

void ut_ManualOpenInterface::SetUp()
{
    const QString service = "com.deepin.dde.daemon.Launcher";
    const QString path = "com/deepin/dde/daemon/Launcher";
    QDBusConnection dbusConn = QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    interface = new ManualOpenInterface(service, path, dbusConn);
}

void ut_ManualOpenInterface::TearDown()
{
    delete interface;
}

TEST_F(ut_ManualOpenInterface, ManualOpenInterface)
{
    QDBusPendingReply<bool> reply = interface->Open("lancher");
    ASSERT_TRUE(!reply.isValid());
    reply = interface->Search("lancher");
    ASSERT_TRUE(!reply.isValid());
    reply = interface->ShowManual("lancher");
    ASSERT_TRUE(!reply.isValid());
}
