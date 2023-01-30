// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_manual_open_adapter.h"
#include "../src/dbus/manual_open_adapter.h"

ut_ManualOpenAdapter::ut_ManualOpenAdapter()
{
}

void ut_ManualOpenAdapter::SetUp()
{
    adapter = new ManualOpenAdapter(this);
}

void ut_ManualOpenAdapter::TearDown()
{
    delete adapter;
}

void ut_ManualOpenAdapter::Open(const QString &app_name)
{
    isopen = true;
}

void ut_ManualOpenAdapter::OpenTitle(const QString &app_name, const QString &title_name)
{
    isOpenTitle = true;
}
void ut_ManualOpenAdapter::ShowManual(const QString &app_name)
{
    isShowManual = true;
}
void ut_ManualOpenAdapter::Search(const QString &keyword)
{
    isSearch = true;
}

TEST_F(ut_ManualOpenAdapter, open)
{
    adapter->Open("dde");
    ASSERT_TRUE(isopen);
}

TEST_F(ut_ManualOpenAdapter, OpenTitle)
{
    adapter->OpenTitle("deepin-a", "a");
    ASSERT_TRUE(isOpenTitle);
}

TEST_F(ut_ManualOpenAdapter, Search)
{
    adapter->Search("a");
    ASSERT_TRUE(isSearch);
}

TEST_F(ut_ManualOpenAdapter, ShowManual)
{
    adapter->ShowManual("deepin");
    ASSERT_TRUE(isShowManual);
}
