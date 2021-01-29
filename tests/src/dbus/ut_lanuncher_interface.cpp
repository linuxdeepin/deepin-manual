#include "ut_lanuncher_interface.h"

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

TEST_F(ut_lanuncher_interface_test, LauncherInterface)
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    LauncherInterface li("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", dbusConn);
    li.deleteLater();
}

TEST_F(ut_lanuncher_interface_test, LauncherInterface2)
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    LauncherInterface li("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", dbusConn);
    //QDBusAbstractInterface qdbusInf("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", li.staticInterfaceName(), dbusConn, &li);
    //    li.staticInterfaceName();
    //LauncherInterface li("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", dbusConn);

    com::deepin::dde::daemon::Launcher("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", dbusConn);
    li.deleteLater();
}

TEST_F(ut_lanuncher_interface_test, LauncherInterface3)
{
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    //    LauncherInterface li("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", dbusConn);
    LauncherInterface li();
    //    li.deleteLater();
}
