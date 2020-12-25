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
    widget = new QWidget;
    adapter = new ManualOpenAdapter(widget);
}

void ut_ManualOpenAdapter::TearDown()
{
    delete adapter;
    delete widget;
}

TEST_F(ut_ManualOpenAdapter, open)
{
    adapter->Open("dde");
}

TEST_F(ut_ManualOpenAdapter, OpenTitle)
{
    adapter->OpenTitle("deepin-a", "a");
}

TEST_F(ut_ManualOpenAdapter, Search)
{
    adapter->Search("a");
}

TEST_F(ut_ManualOpenAdapter, ShowManual)
{
    adapter->ShowManual("deepin");
}
