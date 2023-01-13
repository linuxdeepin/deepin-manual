/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wangmingliang@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
