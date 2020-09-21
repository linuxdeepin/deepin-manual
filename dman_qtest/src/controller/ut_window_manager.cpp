#include "ut_window_manager.h"

#define private public
#include "controller/window_manager.h"
#undef private
#include "view/web_window.h"
#include "dbus/dbus_consts.h"
#include "dbus/dbus_consts.h"
#include <QDBusConnection>

namespace dman {

ut_window_manager_test::ut_window_manager_test()
{

}

TEST_F(ut_window_manager_test, initDBus)
{
    WindowManager wm;
    wm.initDBus();
}

TEST_F(ut_window_manager_test, initDBus2)
{
    WindowManager wm;
    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    dbusConn.registerService(dman::kManualSearchService + QString("Sender"));
    dbusConn.registerObject(dman::kManualSearchIface + QString("Sender"), &wm);
    wm.initDBus();
    wm.deleteLater();
}

TEST_F(ut_window_manager_test, initWebWindow)
{
    WindowManager wm;
    wm.initWebWindow();
    ASSERT_TRUE(wm.window->isEnabled());
}

TEST_F(ut_window_manager_test, activeOrInitWindow)
{
    WindowManager wm;
    //wm.initWebWindow();
    wm.activeOrInitWindow();
}

TEST_F(ut_window_manager_test, activeOrInitWindow2)
{
    WindowManager wm;
    wm.activeOrInitWindow();
}

TEST_F(ut_window_manager_test, SendMsg)
{
//    QProcess p;
//    QString cmd = "mv /usr/share/dbus-1/services/com.deepin.Manual.Search.service "
//                  "/usr/share/dbus-1/services/com.deepin.Manual.Search.service-test";
//    qDebug() << cmd;
//    p.start(cmd);
//    p.close();
    WindowManager wm;
    wm.SendMsg("中国");
//    QProcess p2;
//    QString cmd2 = "sudo mv /usr/share/dbus-1/services/com.deepin.Manual.Search.service-test "
//                   "/usr/share/dbus-1/services/com.deepin.Manual.Search.service";
//    p2.start(cmd2);
//    p2.close();
}

TEST_F(ut_window_manager_test, onNewAppOpen)
{
    WindowManager vm;
    vm.onNewAppOpen();
}

TEST_F(ut_window_manager_test, openManual)
{
    WindowManager mv;
    mv.openManual("输入法", "概述");
}

TEST_F(ut_window_manager_test, openManualWithSearch)
{
    WindowManager mv;
    mv.openManualWithSearch("输入法", "");
}

}
