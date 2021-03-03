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
    qDebug() << "" << info;
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
    QDataStream qd;
    qd << info;
}

TEST_F(ut_AppInfo, OPERATOR_QDBusArgument2)
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
    qd >> info;
}
