// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_manual_open_proxy.h"
#include "../src/dbus/manual_open_proxy.h"

#include <QSignalSpy>

ut_manual_open_proxy::ut_manual_open_proxy()
{

}

void ut_manual_open_proxy::SetUp()
{
    mopen = new ManualOpenProxy();
}

void ut_manual_open_proxy::TearDown()
{
    delete mopen;
}

TEST_F(ut_manual_open_proxy, Open)
{
    QSignalSpy spy(mopen, SIGNAL(openManualRequested(const QString &, const QString &)));
    mopen->Open("dde");
    ASSERT_EQ(spy.count(), 1);
    QList<QVariant> argu = spy.takeFirst();
    ASSERT_EQ(argu.at(0).toString(), "dde");
}

TEST_F(ut_manual_open_proxy, OpenTitle)
{
    QSignalSpy spy(mopen, SIGNAL(openManualRequested(const QString &, const QString &)));
    mopen->OpenTitle("dde", "概述");
    ASSERT_EQ(spy.count(), 1);
    QList<QVariant> argu = spy.takeFirst();
    ASSERT_EQ(argu.at(0).toString(), "dde");
    ASSERT_EQ(argu.at(1).toString(), "概述");
}

TEST_F(ut_manual_open_proxy, ShowManual)
{
    QSignalSpy spy(mopen, SIGNAL(openManualRequested(const QString &, const QString &)));
    mopen->ShowManual("dde");
    ASSERT_EQ(spy.count(), 1);
    QList<QVariant> argu = spy.takeFirst();
    ASSERT_EQ(argu.at(0).toString(), "dde");
}

TEST_F(ut_manual_open_proxy, Search)
{
    QSignalSpy spy(mopen, SIGNAL(searchRequested(const QString &)));
    mopen->Search("应用");
    ASSERT_EQ(spy.count(), 1);
    QList<QVariant> argu = spy.takeFirst();
    ASSERT_EQ(argu.at(0).toString(), "应用");
}
