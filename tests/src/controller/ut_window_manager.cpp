#include "ut_window_manager.h"

#include "controller/window_manager.h"

#include "view/web_window.h"
#include "dbus/dbus_consts.h"
#include "dbus/dbus_consts.h"
#include <QDBusConnection>


ut_window_manager_test::ut_window_manager_test()
{

}


TEST_F(ut_window_manager_test, setStartTime)
{
    WindowManager wm;
    wm.setStartTime(1233444);
    ASSERT_EQ(wm.appStartTime, 1233444);
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
    dbusConn.registerService(kManualSearchService + QString("Sender"));
    dbusConn.registerObject(kManualSearchIface + QString("Sender"), &wm);
    wm.initDBus();
    wm.deleteLater();
}

TEST_F(ut_window_manager_test, initWebWindow)
{
    WindowManager wm;
    wm.initWebWindow();
//    ASSERT_TRUE(wm.window->isEnabled());
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
    WindowManager wm;
    wm.SendMsg("中国");

}


TEST_F(ut_window_manager_test, setWindow)
{
    WindowManager mv;
    mv.initWebWindow();
    mv.window->setFixedWidth(900);
    mv.window->setFixedHeight(1200);
    mv.window->saveWindowSize();
    mv.setWindow(mv.window);

    ASSERT_EQ(mv.window->width(), 900);
    ASSERT_EQ(mv.window->height(), 1200);
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


TEST_F(ut_window_manager_test, onAppStartTimeCount)
{
    WindowManager mv;
    mv.onAppStartTimeCount(123);
}

TEST_F(ut_window_manager_test, onFilesUpdate)
{
    WindowManager mv;

    QStringList upfileList;
    upfileList.append("/usr/share/deepin-manual/manual-assets/application/dde-calendar/calendar/zh_HK/calendar.md");
    upfileList.append("/usr/share/deepin-manual/manual-assets/application/deepin-terminal/terminal/zh_CN/terminal.md");
    mv.onFilesUpdate(upfileList);
}
