// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_appinfo.h"
#include "../src/dbus/dbusvariant/app_info.h"

#include <QtDebug>

ut_AppInfo::ut_AppInfo()
{
}

void ut_AppInfo::SetUp()
{
    app = new AppInfo;
}

void ut_AppInfo::TearDown()
{
    delete app;
}

TEST_F(ut_AppInfo, OPERATOR_qDebug)
{
    AppInfo info;
    info.key = "key";
    info.installed_time = 0002;
    info.desktop = "desktop";
    info.name = "name";
    info.category_id = 001;
    info.icon_key = "icon";
    QDebug db = qDebug() << "" << info;
    qWarning() << db.stream->buffer;
    ASSERT_TRUE(db.stream->buffer.contains("desktop"));
}

TEST_F(ut_AppInfo, OPERATOR_QDataStream)
{
    AppInfo info;
    info.key = "key";
    info.installed_time = 0002;
    info.desktop = "desktop";
    info.name = "name";
    info.category_id = 001;
    info.icon_key = "icon";
    QByteArray ba;
    QDataStream qd(&ba, QIODevice::ReadWrite);
    qd << info;
    ASSERT_TRUE(ba.size() > 0);
}

TEST_F(ut_AppInfo, OPERATOR_QDBusArgument)
{
    AppInfo info;
    info.key = "key";
    info.installed_time = 0002;
    info.desktop = "desktop";
    info.name = "name";
    info.category_id = 001;
    info.icon_key = "icon";
    QDBusArgument qd;
    qd << info;

    qd.beginStructure();
    qd >> info;
    qd.endStructure();

    ASSERT_EQ(info.name, "name");
}

TEST_F(ut_AppInfo, OPERATOR_QDataStream2)
{
    AppInfo info;
    info.key = "key";
    info.installed_time = 0002;
    info.desktop = "desktop";
    info.name = "name";
    info.category_id = 001;
    info.icon_key = "icon";
    QDataStream qd;
    qd << info;
    AppInfo info1;
    qd >> info1;
    qWarning() << info1.desktop;
    //ASSERT_EQ(info.name, "name");
}
