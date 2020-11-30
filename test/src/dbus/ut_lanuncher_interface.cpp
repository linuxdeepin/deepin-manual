#include "ut_lanuncher_interface.h"

//#define protected public
//#include "dbus/launcher_interface.h"
//#define protected
#include <QDBusAbstractInterface>

ut_lanuncher_interface_test::ut_lanuncher_interface_test()
{

}

void ut_lanuncher_interface_test::SetUp()
{

}

void ut_lanuncher_interface_test::TearDown()
{

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

TEST_F(ut_lanuncher_interface_test, GetAllItemInfos)
{
//    QDBusConnection dbusConn =
//        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
//    LauncherInterface li("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", dbusConn);
//    li.GetAllItemInfos();
}

TEST_F(ut_lanuncher_interface_test, GetItemInfo)
{
//    QDBusConnection dbusConn =
//        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
//    LauncherInterface li("com.deepin.dde.daemon.Launcher", "com/deepin/dde/daemon/Launcher", dbusConn);
//    li.GetItemInfo("");
}
