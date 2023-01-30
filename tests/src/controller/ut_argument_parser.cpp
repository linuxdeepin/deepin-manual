// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_argument_parser.h"
#include "controller/window_manager.h"
#include "src/third-party/stub/stub.h"
#include "controller/argument_parser.h"
#include "dbus/dbus_consts.h"
#include "dbus/manual_open_proxy.h"
#include "dbus/manual_open_adapter.h"

#include <QCommandLineParser>
#include <QDBusInterface>

ut_argument_parser_test::ut_argument_parser_test()
{

}

bool stubregisterServicefalse()
{
    return  false;
}


TEST_F(ut_argument_parser_test, parseArguments)
{
    ArgumentParser ap;
    ap.parseArguments();
    Stub st;
    st.set((bool (QDBusConnection::*)(const QString &))ADDR(QDBusConnection, registerService), stubregisterServicefalse);
    ASSERT_FALSE(ap.parseArguments());

    st.set((bool (QStringList::*)() const)ADDR(QStringList, isEmpty), stubregisterServicefalse);
    ASSERT_FALSE(ap.parseArguments());
}

TEST_F(ut_argument_parser_test, parseArguments2)
{
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(QCommandLineOption("dbus", "enable daemon mode"));
    QStringList list;
    list << "deepin-app-store";
    parser.parse(list);

    // 注册Dbus open服务,对外主要接口
    Stub st;
    st.set((bool (QDBusConnection::*)(const QString &))ADDR(QDBusConnection, registerService), stubregisterServicefalse);

    ArgumentParser ap;
    qWarning() << "pars eArguments.bool-->" << ap.parseArguments();
}

TEST_F(ut_argument_parser_test, openManualsDelay)
{
    ArgumentParser *ap = new ArgumentParser;
    WindowManager *wm = new WindowManager;
    QObject::connect(ap, &ArgumentParser::openManualRequested,
                     wm, &WindowManager::openManual);
    ap->curManual = "deepin-app-store";
    ap->openManualsDelay();
    ASSERT_EQ(ap->curManual, wm->curr_app_name_);
    delete ap;
    delete wm;
}

TEST_F(ut_argument_parser_test, onSearchRequested)
{
    ArgumentParser ap;

    WindowManager *wm = new WindowManager;

    QObject::connect(&ap, &ArgumentParser::openManualWithSearchRequested,
                     wm, &WindowManager::openManualWithSearch);
    ap.onSearchRequested("应用");
    ASSERT_EQ("应用", wm->curr_keyword_);
    delete wm;
}

TEST_F(ut_argument_parser_test, onOpenAppRequested)
{
    ArgumentParser ap;
    WindowManager *wm = new WindowManager;
    QObject::connect(&ap, &ArgumentParser::openManualRequested,
                     wm, &WindowManager::openManual);
    ap.onOpenAppRequested("应用", "打开");
    ASSERT_EQ("应用", wm->curr_app_name_);
    ASSERT_EQ("打开", wm->curr_title_name_);
    wm->deleteLater();
}
