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
}
