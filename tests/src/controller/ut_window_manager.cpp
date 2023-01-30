// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_window_manager.h"
#include "controller/window_manager.h"
#include "src/third-party/stub/stub.h"
#include "view/web_window.h"
#include "dbus/dbus_consts.h"
#include "dbus/dbus_consts.h"
#include "view/search_proxy.h"
#include "controller/config_manager.h"
#include "base/utils.h"

#include <QDBusConnection>
#include <QSignalSpy>

int restcnt = 0;

ut_window_manager_test::ut_window_manager_test()
{

}


TEST_F(ut_window_manager_test, setStartTime)
{
    WindowManager wm;
    wm.setStartTime(1233444);
    ASSERT_EQ(wm.appStartTime, 1233444);
}

bool stu_activeWindow()
{
    return true;
}

bool stu_isConnected()
{
    if(restcnt == 0)
    {
        restcnt++;
        return  false;
    }
    else if(restcnt == 1)
    {
        restcnt++;
        return  true;
    }
    else if(restcnt == 2)
    {
        restcnt++;
        return  true;
    }
    return false;
}


TEST_F(ut_window_manager_test, initDBus)
{
    WindowManager wm;

    QDBusInterface interface("com.deepin.Manual.SearchSender",
                             "/com/deepin/Manual/SearchSender",
                             "org.freedesktop.DBus.Peer");

    Stub st;
    QDBusReply<QString> reply = interface.call("GetMachineId");
    ASSERT_FALSE(reply.isValid());

    st.set((bool (QDBusConnection::*)(const QString &))ADDR(QDBusConnection, registerService), stu_isConnected);
    st.set((bool (QDBusConnection::*)(const QString &, QObject *,QDBusConnection::RegisterOptions ))ADDR(QDBusConnection, registerObject), stu_isConnected);
    wm.initDBus();

    QDBusConnection dbusConn =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, "testsned");
    if (!dbusConn.isConnected()) {
        qDebug() << "testsned connectToBus() failed";
        return;
    }

    reply = interface.call("GetMachineId");
    ASSERT_FALSE(reply.isValid());
    st.reset((bool (QDBusConnection::*)(const QString &))ADDR(QDBusConnection, registerService));
    st.reset((bool (QDBusConnection::*)(const QString &, QObject *, QDBusConnection::RegisterOptions))ADDR(QDBusConnection, registerObject));

    restcnt = 0;
    st.set((bool (QDBusConnection::*)())ADDR(QDBusConnection, isConnected), stu_isConnected);
    wm.initDBus();
    st.reset((bool (QDBusConnection::*)())ADDR(QDBusConnection, isConnected));

    wm.initDBus();

    reply = interface.call("GetMachineId");
    ASSERT_TRUE(reply.isValid());
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
    ASSERT_TRUE(wm.window == nullptr);
    //wm.initWebWindow();
    wm.activeOrInitWindow();
    ASSERT_FALSE(wm.window == nullptr);
    WId wid = wm.window->winId();
    wm.activeOrInitWindow();
    ASSERT_EQ(wid, wm.window->winId());
    Stub st;
    st.set(ADDR(Utils, activeWindow), stu_activeWindow);
    wm.window->setFixedSize(500, 500);
    wm.activeOrInitWindow();
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup("window_info");
    ASSERT_EQ(setting->value("window_width"), 500);
    ASSERT_EQ(setting->value("window_height"), 500);
    setting->endGroup();
}

TEST_F(ut_window_manager_test, SendMsg)
{
    restcnt = 0;
    WindowManager wm;
    wm.SendMsg("中国");

    Stub st;
    st.set((bool (QDBusConnection::*)(const QDBusMessage &))ADDR(QDBusConnection, send), stu_isConnected);
    wm.SendMsg("中国");
    ASSERT_EQ(restcnt, 1);
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
    ASSERT_EQ(mv.curr_app_name_, "输入法");
    ASSERT_EQ(mv.curr_title_name_, "概述");
}

TEST_F(ut_window_manager_test, openManualWithSearch)
{
    WindowManager mv;
    mv.openManualWithSearch("输入法", "");
    ASSERT_EQ(mv.curr_app_name_, "输入法");
    ASSERT_TRUE(mv.window->isEnabled());
}



TEST_F(ut_window_manager_test, onFilesUpdate)
{
    WindowManager mv;
    mv.initWebWindow();
    mv.window->search_proxy_ = new SearchProxy;
    QSignalSpy spy(mv.window->search_proxy_, SIGNAL(reloadPage(const QStringList &)));

    QStringList upfileList;
    upfileList.append("/usr/share/deepin-manual/manual-assets/application/dde-calendar/calendar/zh_HK/calendar.md");
    upfileList.append("/usr/share/deepin-manual/manual-assets/application/deepin-terminal/terminal/zh_CN/terminal.md");
    mv.onFilesUpdate(upfileList);
    ASSERT_EQ(spy.count(), 1);
    mv.window->search_proxy_->deleteLater();
}
