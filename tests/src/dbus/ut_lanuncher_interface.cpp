// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_lanuncher_interface.h"
#include "src/third-party/stub/stub.h"
#include "dbus/launcher_interface.h"

#include <QDBusAbstractInterface>

ut_lanuncher_interface_test::ut_lanuncher_interface_test()
{
}

void ut_lanuncher_interface_test::SetUp()
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    li = new LauncherInterface("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", dbusConn);
}

void ut_lanuncher_interface_test::TearDown()
{
    delete li;
}

void stub_asyncCallWithArgumentList()
{

    return ;
}

TEST_F(ut_lanuncher_interface_test, LauncherInterface)
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    LauncherInterface li("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", dbusConn);

    li.deleteLater();
}

TEST_F(ut_lanuncher_interface_test, GetAllItemInfos)
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    LauncherInterface li("com.deepin.dde.daemon.Launcher", "/com/deepin/dde/daemon/Launcher", dbusConn);
    Stub st;
    //st.set((QDBusPendingCall(QDBusAbstractInterface::*)(const QString &, const QList<QVariant> &))ADDR(QDBusAbstractInterface, asyncCallWithArgumentList), stub_asyncCallWithArgumentList);
    st.set((void (QDBusPendingReply<AppInfoList>::*)())ADDR(QDBusPendingReply<AppInfoList>, calculateMetaTypes), stub_asyncCallWithArgumentList);

    QDBusPendingReply<AppInfoList> applist = li.GetAllItemInfos();

    ASSERT_FALSE(applist.isValid());
}

TEST_F(ut_lanuncher_interface_test, GetItemInfo)
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    LauncherInterface li("com.deepin.dde.daemon.Launcher", "/com/deepin/dde/daemon/Launcher", dbusConn);
    Stub st;
    //st.set((QDBusPendingCall(QDBusAbstractInterface::*)(const QString &, const QList<QVariant> &))ADDR(QDBusAbstractInterface, asyncCallWithArgumentList), stub_asyncCallWithArgumentList);
    st.set((void (QDBusPendingReply<AppInfo>::*)())ADDR(QDBusPendingReply<AppInfo>, calculateMetaTypes), stub_asyncCallWithArgumentList);

    QDBusPendingReply<AppInfo> app = li.GetItemInfo("launcher");
    ASSERT_FALSE(app.isValid());
}

TEST_F(ut_lanuncher_interface_test, LauncherInterface2)
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    LauncherInterface li("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", dbusConn);

    com::deepin::dde::daemon::Launcher("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", dbusConn);
    li.deleteLater();
}

TEST_F(ut_lanuncher_interface_test, LauncherInterface3)
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    LauncherInterface li();
}
