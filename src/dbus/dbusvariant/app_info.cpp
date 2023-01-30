// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dbus/dbusvariant/app_info.h"

AppInfo::AppInfo()
    : desktop()
    , name()
    , key()
    , icon_key()
    , category_id(-1)
    , installed_time(0)
{
}

AppInfo::~AppInfo()
{
}

// static
void AppInfo::registerMetaType()
{
    qRegisterMetaType<AppInfo>("AppInfo");
    qDBusRegisterMetaType<AppInfo>();
    qRegisterMetaType<AppInfoList>("AppInfoList");
    qDBusRegisterMetaType<AppInfoList>();
}

QDebug operator<<(QDebug debug, const AppInfo &info)
{
    debug << info.category_id
          << info.installed_time
          << info.desktop
          << info.name
          << info.key
          << info.icon_key;
    return debug;
}

QDBusArgument &operator<<(QDBusArgument &argument,
                          const AppInfo &info)
{
    argument.beginStructure();
    argument << info.desktop
             << info.name
             << info.key
             << info.icon_key
             << info.category_id
             << info.installed_time;
    argument.endStructure();
    return argument;
}

/**
 * @brief operator <<
 * @note 重载<< 输出info信息
 * @param stream
 * @param info
 * @return
 */
QDataStream &operator<<(QDataStream &stream, const AppInfo &info)
{
    stream << info.desktop
           << info.name
           << info.key
           << info.icon_key
           << info.category_id
           << info.installed_time;
    return stream;
}

/**
 * @brief operator >>
 * @note 重载>> 对AppInfo赋值
 * @param argument
 * @param info
 * @return
 */
const QDBusArgument &operator>>(const QDBusArgument &argument,
                                AppInfo &info)
{
    argument.beginStructure();
    argument >> info.desktop
        >> info.name
        >> info.key
        >> info.icon_key
        >> info.category_id
        >> info.installed_time;
    argument.endStructure();
    return argument;
}


/**
 * @brief operator >>
 * @node 从QDataStream中赋值到AppInfo
 * @param stream
 * @param info
 * @return
 */
const QDataStream &operator>>(QDataStream &stream, AppInfo &info)
{
    stream >> info.desktop
        >> info.name
        >> info.key
        >> info.icon_key
        >> info.category_id
        >> info.installed_time;
    return stream;
}
