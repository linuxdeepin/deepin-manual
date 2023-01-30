// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_manual_filesupdate_adapter.h"
#include "../src/dbus/manual_filesupdate_adapter.h"

ut_ManualFilesUpdateAdapter::ut_ManualFilesUpdateAdapter()
{
}

void ut_ManualFilesUpdateAdapter::SetUp()
{
    adapter = new ManualFilesUpdateAdapter(this);
}

void ut_ManualFilesUpdateAdapter::OnFilesUpdate(const QStringList &list)
{
    isOnFilesUpdate = true;
}

void ut_ManualFilesUpdateAdapter::TearDown()
{
    delete adapter;
}

TEST_F(ut_ManualFilesUpdateAdapter, OnFilesUpdate)
{
    QStringList list;
    list << "aaa";
    list << "bb";
    adapter->OnFilesUpdate(list);
    ASSERT_TRUE(isOnFilesUpdate);
}
