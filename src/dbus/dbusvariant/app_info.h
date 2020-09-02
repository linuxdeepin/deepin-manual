/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEEPIN_MANUAL_DBUS_DBUS_VARIANT_APP_INFO_H
#define DEEPIN_MANUAL_DBUS_DBUS_VARIANT_APP_INFO_H

#include <QtDBus>

struct AppInfo {
public:
    AppInfo();
    ~AppInfo();

    static void registerMetaType();
    inline bool operator==(const AppInfo &other) const
    {
        return desktop == other.desktop;
    }

    friend QDebug operator<<(QDebug debug, const AppInfo &info);
    friend QDBusArgument &operator<<(QDBusArgument &argument,
                                     const AppInfo &info);
    friend QDataStream &operator<<(QDataStream &stream, const AppInfo &info);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument,
                                           AppInfo &info);
    friend const QDataStream &operator>>(QDataStream &stream, AppInfo &info);

    QString desktop;
    QString name;
    QString key;
    QString icon_key;
    qlonglong category_id;
    qlonglong installed_time;
};

typedef QList<AppInfo> AppInfoList;

Q_DECLARE_METATYPE(AppInfo)

Q_DECLARE_METATYPE(AppInfoList)

#endif // DEEPIN_MANUAL_DBUS_DBUS_VARIANT_APP_INFO_H
